// Avatar: the LWF movie event handlers. updateNode / addEventListeners register std::map<string,
// std::function<void(LWF::Movie*)>> handler sets with LWF::AddMovieEventHandler; the maps and the (key, handler)
// pairs live in the frame (the map nodes point back at the map's end node there, and the "load" keys are copied
// with their unset bytes), so the frames are the original's and every call is made with call_exact.
#include "Avatar.hpp"

using namespace avatar_fn;
using avatar_detail::cx;
using avatar_detail::destroyFunction;

namespace {

constexpr uint32_t INFO = AvatarInstanceInfo::SIZE;

// short std::string "load" in the frame (bytes 6..11 are left as they were)
void putLoadKey(const guest::Frame &f, uint32_t off) {
    *f.at<uint8_t>(off) = 8;
    *f.at<uint8_t>(off + 4) = 'd';
    *f.at<uint8_t>(off + 3) = 'a';
    *f.at<uint8_t>(off + 2) = 'o';
    *f.at<uint8_t>(off + 1) = 'l';
    *f.at<uint8_t>(off + 5) = 0;
}
// short std::string "enterFrame" in the frame
void putEnterFrameKey(const guest::Frame &f, uint32_t off) {
    *f.at<uint8_t>(off) = 20;
    cx(memcpy_, {f.addr(off + 1), S_ENTER_FRAME, 10});
    *f.at<uint8_t>(off + 11) = 0;
}
// short std::string "unload" in the frame
void putUnloadKey(const guest::Frame &f, uint32_t off) {
    *f.at<uint8_t>(off) = 12;
    *f.at<uint8_t>(off + 6) = 'd';
    *f.at<uint8_t>(off + 5) = 'a';
    *f.at<uint8_t>(off + 4) = 'o';
    *f.at<uint8_t>(off + 3) = 'l';
    *f.at<uint8_t>(off + 2) = 'n';
    *f.at<uint8_t>(off + 1) = 'u';
    *f.at<uint8_t>(off + 7) = 0;
}

// map<string, function> handlers{pair}; lwf->AddMovieEventHandler(movieName, handlers); then destroy the map, the
// pair (function, key) and the movie name. pair: string at +0, function at +0x10 (callable pointer at +0x20).
void addHandler(const guest::Frame &f, uint32_t lwf, uint32_t nameOff, uint32_t pairOff, uint32_t mapOff) {
    uint32_t map = f.addr(mapOff);
    *f.at<uint32_t>(mapOff + 4) = 0;   // end node (root)
    *f.at<uint32_t>(mapOff + 8) = 0;   // size
    *f.at<uint32_t>(mapOff) = map + 4; // begin node
    cx(map_insert, {map, map + 4, f.addr(pairOff)});
    cx(lwf_add_handler, {lwf, f.addr(nameOff), map});
    cx(map_destroy, {map, *f.at<uint32_t>(mapOff + 4)});
    destroyFunction(f.addr(pairOff + 0x10), true);
    if (*f.at<uint8_t>(pairOff) & 1) cx(guest::OPERATOR_DELETE, {*f.at<uint32_t>(pairOff + 8)});
    if (*f.at<uint8_t>(nameOff) & 1) cx(guest::OPERATOR_DELETE, {*f.at<uint32_t>(nameOff + 8)});
}

// an updateNode handler: a heap __func {vtable, avatar, kind, part movie name, first kind}
uint32_t newPartHandler(uint32_t vtable, uint32_t avatar, uint32_t kind, uint32_t name, uint32_t first) {
    uint32_t obj = cx(guest::OPERATOR_NEW, {20});
    if (obj) {
        rw::save<uint32_t>(obj, vtable);
        rw::save<uint32_t>(obj + 4, avatar);
        rw::save<uint32_t>(obj + 8, kind);
        rw::save<uint32_t>(obj + 12, name);
        rw::save<uint32_t>(obj + 16, first);
    }
    return obj;
}

uint32_t lwfOf(Avatar *self) { return rw::load<uint32_t>(self->lwfNode + FlashNode::LWF_NODE_LWF); }

// The new part LWF from the LWF node (virtual attach, sret shared_ptr at out) replaces the slot's.
void attachPart(const guest::Frame &f, Avatar *av, uint32_t lambda, uint32_t slot, uint32_t path, uint32_t movie,
                uint32_t outOff, uint32_t filterOff) {
    uint32_t node = av->lwfNode;
    uint32_t fn = rw::load<uint32_t>(rw::load<uint32_t>(node) + VSLOT_LWFNODE_ATTACH * 4);
    uint32_t name = rw::load<uint32_t>(lambda + 12);
    uint32_t filter = cx(fn_filter_copy, {f.addr(filterOff), Avatar::ATTACH_FILTER});
    cx(fn, {f.addr(outOff), node, path, movie, name, filter});
    uint32_t obj = *f.at<uint32_t>(outOff), ctrl = *f.at<uint32_t>(outOff + 4);
    *f.at<uint32_t>(outOff) = 0;
    *f.at<uint32_t>(outOff + 4) = 0;
    rw::save<uint32_t>(slot, obj);
    uint32_t old = rw::load<uint32_t>(slot + 4);
    rw::save<uint32_t>(slot + 4, ctrl);
    if (old) cx(guest::SHARED_RELEASE, {old});
    uint32_t left = *f.at<uint32_t>(outOff + 4);
    if (left) cx(guest::SHARED_RELEASE, {left});
    destroyFunction(f.addr(filterOff), true);
}

}  // namespace

// For part kinds [from, to): drop the attached part LWFs of every instance, then listen to the part movie's
// "load" ($_7: attach the part) and "enterFrame" ($_8: re-attach a changed part).
void Avatar::updateNode(Avatar *self, int32_t from, int32_t to, uint32_t) {
    guest::Frame f(0xF0);
    if (from >= to) return;
    uint32_t a = rw::guest_addr(self);
    int32_t kind = from;
    do {
        uint32_t begin = self->instances.begin, end = self->instances.end;
        if (begin != end) {
            uint32_t bytes = end - begin;
            uint32_t p = begin + 24 * uint32_t(kind) + 0x30;
            do {
                rw::save<uint32_t>(p, 0);
                uint32_t ctrl = rw::load<uint32_t>(p + 4);
                rw::save<uint32_t>(p + 4, 0);
                if (ctrl) cx(guest::SHARED_RELEASE, {ctrl});
                p += INFO;
                bytes -= INFO;
            } while (bytes);
        }
        uint32_t lwf = lwfOf(self);
        uint32_t name = rw::load<uint32_t>(PART_NAMES + uint32_t(kind) * 4);
        uint32_t len = cx(strlen_, {name});
        cx(string_init, {f.addr(0x74), name, len});
        putLoadKey(f, 0xA8);
        *f.at<uint32_t>(0xC8) = 0;
        *f.at<uint32_t>(0xC8) = newPartHandler(FUNC_PART_LOAD, a, uint32_t(kind), name, uint32_t(from));
        addHandler(f, lwf, 0x74, 0xA8, 0x68);

        lwf = lwfOf(self);
        len = cx(strlen_, {name});
        cx(string_init, {f.addr(0x5C), name, len});
        putEnterFrameKey(f, 0x80);
        *f.at<uint32_t>(0xA0) = 0;
        *f.at<uint32_t>(0xA0) = newPartHandler(FUNC_PART_ENTER_FRAME, a, uint32_t(kind), name, uint32_t(from));
        addHandler(f, lwf, 0x5C, 0x80, 0x50);
    } while (++kind < to);
}

// The movie script commands go to the CommandParser; all part movies get their handlers; the "body" movie and the
// "shadow" movie get theirs.
void Avatar::addEventListeners(Avatar *self) {
    guest::Frame f(0x178);
    uint32_t a = rw::guest_addr(self);
    uint32_t lwf = lwfOf(self);
    // lwf->scriptHandler (+248) = std::bind(&CommandParser::parse, &commandParser, _1)
    *f.at<uint32_t>(0x150) = f.addr(0x140);
    *f.at<uint32_t>(0x140) = FUNC_BIND_PARSE;
    *f.at<uint32_t>(0x144) = CMDPARSER_PARSE;
    *f.at<uint32_t>(0x148) = 0;
    *f.at<uint32_t>(0x14C) = a + 672;
    cx(fn_parse_assign, {lwf + 0xF8, f.addr(0x140)});
    destroyFunction(f.addr(0x140), true);

    updateNode(self, 0, 14, 0);    // costume
    updateNode(self, 14, 15, 0);   // expression
    updateNode(self, 15, 28, 0);   // body
    updateNode(self, 28, 30, 0);   // hair
    updateNode(self, 30, 31, 0);   // keyblade
    updateNode(self, 31, 45, 0);   // accessories

    // "body": load ($_9) / unload ($_10)
    lwf = lwfOf(self);
    cx(string_init, {f.addr(0x94), S_BODY, 4});
    putLoadKey(f, 0x118);
    *f.at<uint32_t>(0x138) = f.addr(0x128);
    *f.at<uint32_t>(0x128) = FUNC_BODY_LOAD;
    *f.at<uint32_t>(0x12C) = a;
    addHandler(f, lwf, 0x94, 0x118, 0x88);

    lwf = lwfOf(self);
    cx(string_init, {f.addr(0x7C), S_BODY, 4});
    putUnloadKey(f, 0xF0);
    *f.at<uint32_t>(0x110) = f.addr(0x100);
    *f.at<uint32_t>(0x100) = FUNC_BODY_UNLOAD;
    *f.at<uint32_t>(0x104) = a;
    addHandler(f, lwf, 0x7C, 0xF0, 0x70);

    // "shadow": load ($_11) / enterFrame ($_12)
    lwf = lwfOf(self);
    cx(string_init, {f.addr(0x64), S_SHADOW, 6});
    putLoadKey(f, 0xC8);
    *f.at<uint32_t>(0xE8) = f.addr(0xD8);
    *f.at<uint32_t>(0xD8) = FUNC_SHADOW_LOAD;
    *f.at<uint32_t>(0xDC) = a;
    addHandler(f, lwf, 0x64, 0xC8, 0x58);

    lwf = lwfOf(self);
    cx(string_init, {f.addr(0x4C), S_SHADOW, 6});
    putEnterFrameKey(f, 0xA0);
    *f.at<uint32_t>(0xC0) = f.addr(0xB0);
    *f.at<uint32_t>(0xB0) = FUNC_SHADOW_ENTER_FRAME;
    *f.at<uint32_t>(0xB4) = a;
    addHandler(f, lwf, 0x4C, 0xA0, 0x40);
}

// ------------------------------------------------------------------------------------------- lambda bodies
// constructor $_3: the movie script names an expression: it becomes pendingExpression.
void Avatar::onExpressionCommand(uint32_t lambda, const uint32_t *name) {
    guest::Frame f(0x20);
    *f.at<uint32_t>(8) = *name;
    uint32_t node = cx(expr_map_find, {EXPRESSION_MAP, f.addr(8)});
    if (node) rw::save<uint32_t>(rw::load<uint32_t>(lambda + 4) + 1364, rw::load<uint32_t>(node + 12));
}

// updateNode $_7 (part movie "load"): attach the part's LWF under the movie (or create it from its path), then
// show its frame. Capture: avatar +4, kind +8, part movie name +12, first kind of the group +16.
void Avatar::onPartLoad(uint32_t lambda, const uint32_t *movieRef) {
    guest::Frame f(0xA0);
    uint32_t movie = *movieRef;
    Avatar *av = rw::host_ptr<Avatar>(rw::load<uint32_t>(lambda + 4));
    if (verifyGrandparent(av, movie, rw::frame_ptr<int32_t>(f.addr(0x4C))) != 1) return;
    uint32_t idx = *f.at<uint32_t>(0x4C);
    uint32_t begin = av->instances.begin;
    uint32_t info = begin + idx * INFO;
    uint32_t partOff = 24 * rw::load<uint32_t>(lambda + 8);
    uint32_t slot = info + partOff + 0x30;   // shared_ptr<LWF>
    uint32_t lwf = rw::load<uint32_t>(slot);
    if (lwf) {
        uint32_t root = rw::load<uint32_t>(lwf + 0x18);
        cx(rw::load<uint32_t>(rw::load<uint32_t>(root) + 0x1C), {root});
        // a copy of the shared_ptr, passed by value
        *f.at<uint32_t>(0x44) = rw::load<uint32_t>(slot);
        uint32_t ctrl = rw::load<uint32_t>(begin + idx * INFO + partOff + 0x34);
        *f.at<uint32_t>(0x48) = ctrl;
        if (ctrl) cx(guest::SHARED_ADD_REF, {ctrl});
        uint32_t name = rw::load<uint32_t>(lambda + 12);
        uint32_t len = cx(strlen_, {name});
        cx(string_init, {f.addr(0x38), name, len});
        cx(fn_filter_copy, {f.addr(0x68), ATTACH_FILTER});
        cx(movie_attach_lwf, {movie, f.addr(0x44), f.addr(0x38), f.addr(0x68), rw::load<uint32_t>(movie + 0xA0), 0});
        destroyFunction(f.addr(0x68), true);
        if (*f.at<uint8_t>(0x38) & 1) cx(guest::OPERATOR_DELETE, {*f.at<uint32_t>(0x40)});
        uint32_t c = *f.at<uint32_t>(0x48);
        if (c) cx(guest::SHARED_RELEASE, {c});
    } else {
        uint32_t path = begin + idx * INFO + partOff + 0x38;
        if (avatar_detail::strSize(path) != 0)
            attachPart(f, av, lambda, slot, avatar_detail::strData(path), movie, 0x30, 0x50);
    }
    gotoAndStopMovie(av, rw::host_ptr<AvatarInstanceInfo>(info), rw::load<int32_t>(lambda + 16),
                     rw::host_ptr<uint32_t>(slot));
}

// updateNode $_8 (part movie "enterFrame"): a dirty part is detached and attached again from its new path.
void Avatar::onPartEnterFrame(uint32_t lambda, const uint32_t *movieRef) {
    guest::Frame f(0x90);
    uint32_t movie = *movieRef;
    Avatar *av = rw::host_ptr<Avatar>(rw::load<uint32_t>(lambda + 4));
    if (verifyGrandparent(av, movie, rw::frame_ptr<int32_t>(f.addr(0x54))) != 1) return;
    uint32_t idx = *f.at<uint32_t>(0x54);
    uint32_t begin = av->instances.begin;
    uint32_t info = begin + idx * INFO;
    uint32_t partOff = 24 * rw::load<uint32_t>(lambda + 8);
    uint32_t dirty = info + partOff + 0x2C;
    if (rw::load<uint8_t>(dirty) == 0) return;
    rw::save<uint8_t>(dirty, 0);
    uint32_t slot = begin + idx * INFO + partOff + 0x30;
    uint32_t lwf = rw::load<uint32_t>(slot);
    if (lwf) {
        destroyFunction(lwf + 0x30, true);   // the LWF's detach handler
        rw::save<uint32_t>(lwf + 0x40, 0);
        *f.at<uint32_t>(0x4C) = rw::load<uint32_t>(slot);
        uint32_t ctrl = rw::load<uint32_t>(begin + idx * INFO + partOff + 0x34);
        *f.at<uint32_t>(0x50) = ctrl;
        if (ctrl) cx(guest::SHARED_ADD_REF, {ctrl});
        cx(movie_detach_lwf, {movie, f.addr(0x4C)});
        uint32_t c = *f.at<uint32_t>(0x50);
        if (c) cx(guest::SHARED_RELEASE, {c});
    }
    uint32_t path = begin + idx * INFO + partOff + 0x38;
    if (avatar_detail::strSize(path) != 0)
        attachPart(f, av, lambda, slot, avatar_detail::strData(path), movie, 0x44, 0x58);
    gotoAndStopMovie(av, rw::host_ptr<AvatarInstanceInfo>(info), rw::load<int32_t>(lambda + 16),
                     rw::host_ptr<uint32_t>(slot));
}

// addEventListeners $_9 ("body" load): remember the body movie; in the wait motion its first y is the base y.
void Avatar::onBodyLoad(uint32_t lambda, const uint32_t *movieRef) {
    Avatar *av = rw::host_ptr<Avatar>(rw::load<uint32_t>(lambda + 4));
    uint32_t movie = *movieRef;
    if (av->motion == MOTION_WAIT && av->bodyBaseY == 3.40282347e+38f)
        rw::set<uint32_t>(av, 1344, rw::call(movie_get_y, {movie}));
    av->bodyMovie = movie;
}

// addEventListeners $_11 ("shadow" load): remember the shadow's own scale.
void Avatar::onShadowLoad(uint32_t lambda, const uint32_t *movieRef) {
    Avatar *av = rw::host_ptr<Avatar>(rw::load<uint32_t>(lambda + 4));
    uint32_t movie = *movieRef;
    rw::set<uint32_t>(av, 1348, rw::call(movie_get_scale_x, {movie}));
    rw::set<uint32_t>(av, 1352, rw::call(movie_get_scale_y, {movie}));
}

// addEventListeners $_12 ("shadow" enterFrame): the shadow follows the body's x and shrinks as the body rises
// (gone at 187.5 above the base y).
void Avatar::onShadowEnterFrame(uint32_t lambda, const uint32_t *movieRef) {
    Avatar *av = rw::host_ptr<Avatar>(rw::load<uint32_t>(lambda + 4));
    uint32_t body = av->bodyMovie;
    if (!body) return;
    uint32_t movie = *movieRef;
    uint32_t x = rw::call(movie_get_x, {body});
    rw::call(movie_set_x, {movie, x});
    float rise = av->bodyBaseY - rw::bitsf(rw::call(movie_get_y, {av->bodyMovie}));
    float scale;
    if (rise <= 0.0f) scale = 1.0f;
    else if (rise >= 187.5f || rise != rise) scale = 0.0f;
    else scale = rise / -187.5f + 1.0f;
    rw::call(movie_set_scale_x, {movie, rw::fbits(scale * av->shadowScaleX)});
    rw::call(movie_set_scale_y, {movie, rw::fbits(scale * av->shadowScaleY)});
}

RW_FUNCTION(0x5C67FC, "Avatar::updateNode", Avatar::updateNode);
RW_FUNCTION(0x5C7744, "Avatar::addEventListeners", Avatar::addEventListeners);
RW_FUNCTION(0x5C90B4, "Avatar::Avatar(bool)::$_3::operator()", Avatar::onExpressionCommand);
RW_FUNCTION(0x5C8BBC, "Avatar::updateNode::$_7::operator()", Avatar::onPartLoad);
RW_FUNCTION(0x5C88BC, "Avatar::updateNode::$_8::operator()", Avatar::onPartEnterFrame);
RW_FUNCTION(0x5C8640, "Avatar::addEventListeners::$_9::operator()", Avatar::onBodyLoad);
RW_FUNCTION(0x5C84D8, "Avatar::addEventListeners::$_11::operator()", Avatar::onShadowLoad);
RW_FUNCTION(0x5C836C, "Avatar::addEventListeners::$_12::operator()", Avatar::onShadowEnterFrame);
