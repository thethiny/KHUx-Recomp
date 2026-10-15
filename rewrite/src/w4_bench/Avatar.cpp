// Avatar: construction, style / part changes, LWF paths and motions. (Events: Avatar_events.cpp; preloading and
// the InstanceInfo constructors: Avatar_misc.cpp.)
//
// Functions that keep objects on the guest stack whose addresses reach the heap or globals (the constructor and
// addInstance build an InstanceInfo there: its SafeBuffer registers itself in the global update list) reproduce the
// original frame and call with call_exact; the others do too, so callee frames sit where the original put them.
#include "Avatar.hpp"

using namespace avatar_fn;
using avatar_detail::cx;
using avatar_detail::destroyFunction;

namespace {

constexpr uint32_t INFO = AvatarInstanceInfo::SIZE;

uint32_t self_addr(const void *p) { return rw::guest_addr(p); }

// a virtual call made like the compiled one (the callee frame at the current sp)
uint32_t vcx(uint32_t obj, uint32_t slot, std::initializer_list<uint32_t> args) {
    uint32_t fn = rw::load<uint32_t>(rw::load<uint32_t>(obj) + slot * 4);
    return cx(fn, args);
}

// The inlined InstanceInfo move constructor (vector::push_back(InstanceInfo&&) with room left): the SafeBuffer part
// is moved by its constructor, the name and the 45 part slots are taken over and cleared in the source.
void moveInstanceInfo(uint32_t dst, uint32_t src) {
    uint32_t d = cx(sb_move, {dst, src});
    rw::save<uint32_t>(d, AvatarStyle::VTABLE);
    for (uint32_t k = 0; k < 12; k += 4) rw::save<uint32_t>(d + 0x20 + k, rw::load<uint32_t>(src + 0x20 + k));
    for (uint32_t k = 0; k < 12; k += 4) *rw::frame_ptr<uint32_t>(src + 0x20 + k) = 0;
    for (uint32_t k = 0; k < AvatarInstanceInfo::PART_COUNT; k++) {
        uint32_t dp = d + 24 * k, sp = src + 24 * k;
        rw::save<uint8_t>(dp + 0x2C, rw::load<uint8_t>(sp + 0x2C));
        rw::save<uint32_t>(dp + 0x30, rw::load<uint32_t>(sp + 0x30));   // shared_ptr<LWF>
        rw::save<uint32_t>(dp + 0x34, rw::load<uint32_t>(sp + 0x34));
        *rw::frame_ptr<uint32_t>(sp + 0x30) = 0;
        *rw::frame_ptr<uint32_t>(sp + 0x34) = 0;
        uint32_t s0 = rw::load<uint32_t>(sp + 0x38);                   // path string
        rw::save<uint32_t>(dp + 0x40, rw::load<uint32_t>(sp + 0x40));
        rw::save<uint32_t>(dp + 0x3C, rw::load<uint32_t>(sp + 0x3C));
        rw::save<uint32_t>(dp + 0x38, s0);
        *rw::frame_ptr<uint32_t>(sp + 0x40) = 0;
        *rw::frame_ptr<uint32_t>(sp + 0x3C) = 0;
        *rw::frame_ptr<uint32_t>(sp + 0x38) = 0;
    }
}

// instances.push_back(std::move(local InstanceInfo at src))
void pushInstance(Avatar *self, uint32_t src) {
    uint32_t end = self->instances.end;
    if (end >= self->instances.cap) {
        cx(instances_push_slow, {self_addr(&self->instances), src});
        return;
    }
    uint32_t base = 0;
    if (end) {
        moveInstanceInfo(end, src);
        base = self->instances.end;
    }
    self->instances.end = base + INFO;
}

// ~InstanceInfo() of the local at info (parts in reverse, then the name, then the SafeBuffer)
void destroyLocalInstance(uint32_t info) {
    for (int k = AvatarInstanceInfo::PART_COUNT - 1; k >= 0; k--) {
        uint32_t ctrl = info + 0x30 + 24 * k + 4;
        if (rw::load<uint8_t>(ctrl + 4) & 1) cx(guest::OPERATOR_DELETE, {rw::load<uint32_t>(ctrl + 12)});
        uint32_t c = rw::load<uint32_t>(ctrl);
        if (c) cx(guest::SHARED_RELEASE, {c});
    }
    if (rw::load<uint8_t>(info + 0x20) & 1) cx(guest::OPERATOR_DELETE, {rw::load<uint32_t>(info + 0x28)});
    cx(sb_dtor, {info});
}

// the constructor's command lambdas: std::function(lambda).swap(member), then destroy the temporary
void setCommand(const guest::Frame &f, uint32_t self, uint32_t vtable, uint32_t memberOff, uint32_t swapFn) {
    uint32_t buf = f.addr(0x4B0);
    *f.at<uint32_t>(0x4C0) = buf;
    *f.at<uint32_t>(0x4B0) = vtable;
    *f.at<uint32_t>(0x4B4) = self;
    cx(swapFn, {buf, self + memberOff});
    destroyFunction(buf, true);
}

// SafeBuffer::read of 4 bytes at `off` of the style data into the frame slot `dst`
int32_t readStyle(uint32_t style, uint32_t dst, uint32_t off) {
    cx(sb_read, {style, dst, rw::load<uint32_t>(style + 12) + off, 4, off});
    return rw::load<int32_t>(dst);
}

// change one style value of the current instance and rebuild the paths of its part kinds [from, to)
void changePart(Avatar *self, int32_t id, uint32_t off, int32_t from, int32_t to, uint32_t dir) {
    guest::Frame f(0x28);
    *f.at<int32_t>(0xC) = id;
    uint32_t info = self->instances.begin + self->currentInstance * INFO;
    cx(sb_write, {info, rw::load<uint32_t>(info + 12) + off, f.addr(0xC), 4, off});
    Avatar::updateLWFPath(self, rw::host_ptr<AvatarInstanceInfo>(info), from, to, dir);
}

}  // namespace

// ------------------------------------------------------------------------------------------- construction
uint32_t Avatar::create(uint32_t style, uint32_t noCommands) {
    uint32_t obj = guest::newObject(SIZE);
    rw::call(AV_CTOR_STYLE, {obj, style, noCommands});
    rw::call(ref_autorelease, {obj});
    return obj;
}

Avatar *Avatar::construct(Avatar *self, uint32_t noCommands) {
    guest::Frame f(0x4E8);
    uint32_t a = self_addr(self);
    cx(flash_ctor, {a, noCommands});
    self->vtable = VTABLE;
    self->burstId = 0;
    self->motion = MOTION_NONE;
    self->burstSize = 0;
    self->burstFirst = 0;
    self->burstBucketCount = 0;
    self->burstBuckets = 0;
    self->burstMaxLoad = 1.0f;
    self->bodyMovie = 0;
    rw::set<uint32_t>(self, 1344, 0x7F7FFFFFu);   // bodyBaseY = FLT_MAX
    self->shadowScaleX = 1.0f;
    self->shadowScaleY = 1.0f;
    self->expressionDirty = 0;
    self->expression = 1;
    self->pendingExpression = 1;
    self->expansionPending = 0;
    self->expansionScale = 100;
    self->expansionFrames = 0;
    self->field_564 = 1;
    self->instances.cap = 0;
    self->instances.end = 0;
    self->instances.begin = 0;
    self->currentInstance = 0;

    // the static expression name -> Expression map (values 1..13)
    if (rw::load<uint8_t>(EXPRESSION_MAP_INITIALIZED) == 0) {
        rw::save<uint8_t>(EXPRESSION_MAP_INITIALIZED, 1);
        for (uint32_t i = 0; i < 13;) {
            uint32_t name = rw::load<uint32_t>(EXPRESSION_NAMES + 4 * i);
            i++;
            *f.at<uint32_t>(0x3C) = name;
            *f.at<uint32_t>(0x40) = i;
            cx(expr_map_insert, {f.addr(0x44), EXPRESSION_MAP, f.addr(0x3C)});
        }
    }

    // instances.push_back(InstanceInfo())
    uint32_t local = f.addr(0x4C);
    cx(AV_INSTANCE_INFO_CTOR, {local});
    pushInstance(self, local);

    if (!noCommands) {
        setCommand(f, a, FUNC_CTOR_1, 744, fn_void_swap);
        setCommand(f, a, FUNC_CTOR_2, 840, fn_void_swap);
        setCommand(f, a, FUNC_CTOR_3, 864, fn_cstr_swap);
        setCommand(f, a, FUNC_CTOR_4, 888, fn_void_swap);
        setCommand(f, a, FUNC_CTOR_5, 912, fn_int_swap);
        setCommand(f, a, FUNC_CTOR_6, 936, fn_int_swap);
    }
    destroyLocalInstance(local);
    return self;
}

Avatar *Avatar::constructWithStyle(Avatar *self, AvatarStyle *style, uint32_t noCommands) {
    construct(self, noCommands);
    changeStyle(self, style);
    return self;
}

void Avatar::changeStyle(Avatar *self, AvatarStyle *style) {
    guest::Frame f(0x40);
    uint32_t s = self_addr(style), v = f.addr(0x10);
    changeHair(self, readStyle(s, v, AvatarStyle::HAIR));
    changeHairColor(self, readStyle(s, v, AvatarStyle::HAIR_COLOR));
    changeFace(self, readStyle(s, v, AvatarStyle::FACE));
    changeCostume(self, readStyle(s, v, AvatarStyle::COSTUME));
    changeBody(self, readStyle(s, v, AvatarStyle::BODY));
    int32_t id = readStyle(s, v, AvatarStyle::KEYBLADE_ID);
    *f.at<int32_t>(4) = id;
    int32_t level = readStyle(s, v, AvatarStyle::KEYBLADE_LEVEL);
    changeKeyblade(self, id, level);
    uint32_t acc = f.addr(0x14);
    cx(sb_read, {s, acc, rw::load<uint32_t>(s + 12) + AvatarStyle::ACCESSORIES, 0x14, AvatarStyle::ACCESSORIES});
    changeAccessories(self, rw::host_ptr<int32_t>(acc));
}

Avatar *Avatar::destruct(Avatar *self) {
    uint32_t a = self_addr(self);
    self->vtable = VTABLE;
    removeDetachHandlers(self);
    clearMovieEventHandlers(self);
    rw::call(instances_dtor, {a + 1388});
    rw::call(intset_dtor, {a + 1320});
    self->vtable = FlashNode::VTABLE;
    rw::call(intvec_dtor, {a + 1284});
    rw::call(actflag_deque_dtor, {a + 1248});
    rw::call(int_deque_dtor, {a + 1224});
    rw::call(cmdparser_dtor, {a + 672});
    rw::call(vec4_dtor, {a + 632});
    rw::call(node_dtor, {a});
    return self;
}

// every attached part LWF loses its detach handler (std::function at LWF + 48)
void Avatar::removeDetachHandlers(Avatar *self) {
    guest::Frame f(0x18);
    uint32_t it = self->instances.begin, end = self->instances.end;
    for (; it != end; it += INFO) {
        for (uint32_t k = 0; k < AvatarInstanceInfo::PART_COUNT; k++) {
            uint32_t lwf = rw::load<uint32_t>(it + 0x30 + 24 * k);
            if (!lwf) continue;
            destroyFunction(lwf + 0x30, true);
            rw::save<uint32_t>(lwf + 0x40, 0);
        }
    }
}

void Avatar::clearMovieEventHandlers(Avatar *self) {
    guest::Frame f(0x28);
    uint32_t node = self->lwfNode;
    if (node) {
        uint32_t lwf = rw::load<uint32_t>(node + LWF_NODE_LWF);
        if (lwf) {
            uint32_t str = f.addr(4);
            cx(string_init, {str, S_BODY, 4});
            cx(lwf_clear_handler, {lwf, str});
            if (rw::load<uint8_t>(str) & 1) cx(guest::OPERATOR_DELETE, {rw::load<uint32_t>(f.addr(0xC))});
        }
    }
    self->bodyMovie = 0;
}

void Avatar::addBurst(Avatar *self, int32_t id) {
    guest::Frame f(0x28);
    *f.at<int32_t>(8) = id;
    cx(intset_insert, {f.addr(0xC), self_addr(self) + 1320, f.addr(8)});
}

void Avatar::update(Avatar *self, float dt) {
    guest::Frame f(0x28);
    if (self->motion == MOTION_NONE) {
        self->motion = MOTION_WAIT;
        changeLWF(self, S_WAIT_LWF);
        cx(flash_change_sub, {self_addr(self)});
    }
    cx(flash_update, {self_addr(self), rw::fbits(dt)});
    if (!self->expressionDirty) return;
    self->expressionDirty = 0;
    int32_t pending = self->pendingExpression;
    if (pending == self->expression) return;
    uint32_t begin = self->instances.begin, end = self->instances.end;
    // instances.size() (exact division by 1124 as the compiler emits it)
    uint32_t count = uint32_t(int32_t(end - begin) >> 2) * 0x749CB29u;
    if (end != begin) {
        uint32_t i = 0;
        for (;;) {
            uint32_t idx = self->currentInstance;
            self->expression = pending;
            updateLWFPath(self, rw::host_ptr<AvatarInstanceInfo>(begin + idx * INFO), 14, 15, S_EXPRESSION);
            if (++i >= count) break;
            begin = self->instances.begin;
            pending = self->pendingExpression;
        }
    }
    self->currentInstance = 0;
}

void Avatar::changeMotion(Avatar *self, int32_t motion) {
    guest::Frame f(0x430);
    uint32_t sb = f.addr(0x14);
    *f.at<uint32_t>(0x414) = 0;   // StringBuffer: empty
    *f.at<uint8_t>(0x14) = 0;
    uint32_t path;
    if (motion == MOTION_BURST) {
        cx(burst_create, {f.addr(8), uint32_t(self->burstId)});
        uint32_t burst = *f.at<uint32_t>(8);
        cx(sb_read, {burst, f.addr(0x10), rw::load<uint32_t>(burst + 12) + 4, 4, 4});
        uint32_t lwfId = *f.at<uint32_t>(0x10);
        cx(sbuf_fset, {sb, S_BURST_LWF, lwfId, lwfId});
        uint32_t ctrl = *f.at<uint32_t>(0xC);
        if (ctrl) cx(guest::SHARED_RELEASE, {ctrl});
        path = sb;
    } else {
        path = rw::load<uint32_t>(MOTION_PATHS + uint32_t(motion) * 4);
    }
    self->motion = motion;
    changeLWF(self, path);
    cx(flash_change_sub, {self_addr(self)});
}

void Avatar::changeExpression(Avatar *self, int32_t expression) {
    guest::Frame f(0x10);
    uint32_t begin = self->instances.begin, idx = self->currentInstance;
    self->expression = expression;
    updateLWFPath(self, rw::host_ptr<AvatarInstanceInfo>(begin + idx * INFO), 14, 15, S_EXPRESSION);
}

// The LWF path of part `kind` for a style into the StringBuffer sb (dir: the part directory, name: the part movie)
void Avatar::makeLWFPath(Avatar *self, int32_t kind, AvatarStyle *style, uint32_t dir, uint32_t name, uint32_t sb) {
    guest::Frame f(0x38);
    uint32_t s = self_addr(style), v = f.addr(0x1C);
    if (kind == 30) {
        int32_t id = readStyle(s, v, AvatarStyle::KEYBLADE_ID);
        int32_t level = readStyle(s, v, AvatarStyle::KEYBLADE_LEVEL);
        // "lwf/avatar/%s/keyblade_%04d_%02d/keyblade_%04d_%02d.lwf"
        cx(sbuf_fset, {sb, 0xE00D90, dir, uint32_t(id), uint32_t(level), uint32_t(id), uint32_t(level)});
        return;
    }
    uint32_t fmt, value;
    if (kind <= 13) {
        value = uint32_t(readStyle(s, v, AvatarStyle::COSTUME));
        fmt = 0xE00DC8;   // "lwf/avatar/%s/%03d/%s/%s.lwf"
    } else if (kind <= 14) {
        value = uint32_t(readStyle(s, v, AvatarStyle::FACE));
        cx(sbuf_fset, {sb, 0xE00DE5, dir, value, value});   // "lwf/avatar/%s/%05d/%05d.lwf"
        return;
    } else if (kind <= 27) {
        value = uint32_t(readStyle(s, v, AvatarStyle::BODY));
        fmt = 0xE00E01;   // "lwf/avatar/%s/%05d/%s/%s.lwf"
    } else if (kind <= 29) {
        value = uint32_t(readStyle(s, v, AvatarStyle::HAIR));
        fmt = 0xE00E01;
    } else if (kind <= 44) {
        value = uint32_t(self->accessoryId);
        fmt = 0xE00E1E;   // "lwf/avatar/%s/%d/%s/%s.lwf"
    } else {
        return;
    }
    cx(sbuf_fset, {sb, fmt, dir, value, name, name});
}

// Show the frame a part LWF needs: the face shows the current expression, the hair its colour (% 1000).
void Avatar::gotoAndStopMovie(Avatar *self, AvatarInstanceInfo *info, int32_t kind, const uint32_t *lwfRef) {
    guest::Frame f(0x28);
    uint32_t lwf = *lwfRef;
    if (!lwf) return;
    if (kind == 14) {
        cx(movie_goto_stop, {rw::load<uint32_t>(lwf + 0x18), uint32_t(self->expression)});
    } else if (kind == 28) {
        uint32_t root = rw::load<uint32_t>(lwf + 0x18);
        uint32_t i = self_addr(info);
        cx(sb_read, {i, f.addr(0xC), rw::load<uint32_t>(i + 12) + 4, 4, 4});
        int32_t color = *f.at<int32_t>(0xC);
        cx(movie_goto_stop, {root, uint32_t(color % 1000)});
    }
}

// Is the movie's grandparent one of our instances (by name)? *index = that instance. A single instance without a
// name matches anything (index 0).
uint32_t Avatar::verifyGrandparent(Avatar *self, uint32_t movie, const int32_t *index) {
    guest::Frame f(0x30);
    uint32_t begin = self->instances.begin;
    int32_t bytes = int32_t(self->instances.end - begin);
    int32_t count = int32_t(uint32_t(bytes >> 2) * 0x749CB29u);
    if (bytes == int32_t(INFO) && avatar_detail::strSize(begin + 0x20) == 0) {
        rw::store32(index, 0);
        return 1;
    }
    uint32_t parent = rw::load<uint32_t>(movie + 8);
    if (!parent || !rw::load<uint32_t>(parent + 8)) return 0;
    if (bytes < 1) return 0;
    uint32_t gp = rw::load<uint32_t>(parent + 8);
    uint32_t gpName = gp + 0x70;   // LWF::Movie name (std::string)
    bool gpLong = rw::load<uint8_t>(gpName) & 1;
    int32_t i = 0;
    do {   // (entered once even if count is 0)
        uint32_t name = begin + uint32_t(i) * INFO + 0x20;
        uint32_t n = gpLong ? rw::load<uint32_t>(gp + 0x74) : uint32_t(rw::load<uint8_t>(gpName) >> 1);
        if (n != avatar_detail::strSize(name)) continue;
        uint32_t a = gpLong ? rw::load<uint32_t>(gp + 0x78) : gp + 0x71;
        uint32_t b = avatar_detail::strData(name);
        if (gpLong) {
            if (cx(memcmp_, {a, b, n}) != 0) continue;
        } else {
            bool same = true;
            for (; n; n--, a++, b++)
                if (rw::load<uint8_t>(b) != rw::load<uint8_t>(a)) { same = false; break; }
            if (!same) continue;
        }
        rw::store32(index, uint32_t(i));
        return 1;
    } while (++i < count);
    return 0;
}

// Rebuild the LWF paths of part kinds [from, to) of an instance (and mark them dirty).
void Avatar::updateLWFPath(Avatar *self, AvatarInstanceInfo *info, int32_t from, int32_t to, uint32_t dir) {
    guest::Frame f(0x440);
    uint32_t sb = f.addr(0x24);
    *f.at<uint32_t>(0x424) = 0;
    *f.at<uint8_t>(0x24) = 0;
    if (from >= to) return;
    uint32_t i = self_addr(info);
    int32_t kind = from;
    do {
        makeLWFPath(self, kind, info, dir, rw::load<uint32_t>(PART_NAMES + uint32_t(kind) * 4), sb);
        uint32_t len = cx(strlen_, {sb});
        cx(string_assign, {i + 0x38 + 24 * uint32_t(kind), sb, len});
        rw::save<uint8_t>(i + 0x2C + 24 * uint32_t(kind), 1);
        kind++;
    } while (to != kind);
}

// Clear the LWF paths of part kinds [from, to) (and mark them dirty).
void Avatar::removeLWFPath(Avatar *, AvatarInstanceInfo *info, int32_t from, int32_t to, uint32_t) {
    if (from >= to) return;
    uint32_t n = uint32_t(to - from);
    uint32_t p = self_addr(info) + 24 * uint32_t(from) + 0x38;   // path string
    do {
        if (rw::load<uint8_t>(p) & 1) {
            rw::save<uint8_t>(rw::load<uint32_t>(p + 8), 0);
            rw::save<uint32_t>(p + 4, 0);
        } else {
            rw::save<uint8_t>(p + 1, 0);
            rw::save<uint8_t>(p, 0);
        }
        rw::save<uint8_t>(p - 12, 1);
        p += 24;
    } while (--n);
}

void Avatar::changeCostume(Avatar *self, int32_t id) { changePart(self, id, AvatarStyle::COSTUME, 0, 14, S_COSTUME); }
void Avatar::changeFace(Avatar *self, int32_t id) { changePart(self, id, AvatarStyle::FACE, 14, 15, S_EXPRESSION); }
void Avatar::changeBody(Avatar *self, int32_t id) { changePart(self, id, AvatarStyle::BODY, 15, 28, S_BODY); }
void Avatar::changeHair(Avatar *self, int32_t id) { changePart(self, id, AvatarStyle::HAIR, 28, 30, S_HAIR); }
void Avatar::changeHairColor(Avatar *self, int32_t id) {
    changePart(self, id, AvatarStyle::HAIR_COLOR, 28, 30, S_HAIR);
}

void Avatar::changeKeyblade(Avatar *self, int32_t id, int32_t level) {
    guest::Frame f(0x28);
    *f.at<int32_t>(8) = level;
    *f.at<int32_t>(0xC) = id;
    uint32_t info = self->instances.begin + self->currentInstance * INFO;
    cx(sb_write, {info, rw::load<uint32_t>(info + 12) + 20, f.addr(0xC), 4, 20});
    cx(sb_write, {info, rw::load<uint32_t>(info + 12) + 24, f.addr(8), 4, 24});
    updateLWFPath(self, rw::host_ptr<AvatarInstanceInfo>(info), 30, 31, S_KEYBLADE);
}

// keyblade id * 100 + level
void Avatar::changeKeybladeCombined(Avatar *self, int32_t idLevel) {
    guest::Frame f(0x18);
    changeKeyblade(self, idLevel / 100, idLevel % 100);
}

// fn(info, firstKind, endKind, "accessory") for each accessory set in the instance's style; the accessory
// category (id / 1000 % 100, 1-based) picks the part slot range.
void Avatar::forEachAccessory(Avatar *self, AvatarInstanceInfo *info, uint32_t fn) {
    guest::Frame f(0x40);
    uint32_t i = self_addr(info), ids = f.addr(0x14);
    cx(sb_read, {i, ids, rw::load<uint32_t>(i + 12) + 28, 0x14, 0x1C});
    for (uint32_t k = 0; k < 5; k++) {
        int32_t id = rw::load<int32_t>(ids + 4 * k);
        if (!id) continue;
        self->accessoryId = id;
        int32_t category = (rw::load<int32_t>(ids + 4 * k) / 1000) % 100;
        uint32_t range = ACCESSORY_RANGES + uint32_t(category) * 8;
        cx(fn_instance_call, {fn, i, rw::load<uint32_t>(range - 8), rw::load<uint32_t>(range - 4), S_ACCESSORY});
    }
}

void Avatar::changeAccessories(Avatar *self, const int32_t *ids) {
    guest::Frame f(0x68);
    uint32_t a = self_addr(self);
    uint32_t begin = self->instances.begin, idx = self->currentInstance;
    uint32_t info = begin + idx * INFO;
    // std::function(std::bind(&Avatar::removeLWFPath, this, ...))
    *f.at<uint32_t>(0x40) = f.addr(0x30);
    *f.at<uint32_t>(0x30) = FUNC_BIND_INSTANCE;
    *f.at<uint32_t>(0x34) = AV_REMOVE_LWF_PATH;
    *f.at<uint32_t>(0x38) = 0;
    *f.at<uint32_t>(0x3C) = a;
    forEachAccessory(self, rw::host_ptr<AvatarInstanceInfo>(info), f.addr(0x30));
    destroyFunction(f.addr(0x30), true);
    cx(sb_write, {info, rw::load<uint32_t>(begin + idx * INFO + 12) + 28, rw::guest_addr(ids), 0x14, 0x1C});
    // std::function(std::bind(&Avatar::updateLWFPath, this, ...))
    *f.at<uint32_t>(0x28) = f.addr(0x18);
    *f.at<uint32_t>(0x18) = FUNC_BIND_INSTANCE;
    *f.at<uint32_t>(0x1C) = AV_UPDATE_LWF_PATH;
    *f.at<uint32_t>(0x20) = 0;
    *f.at<uint32_t>(0x24) = a;
    forEachAccessory(self, rw::host_ptr<AvatarInstanceInfo>(info), f.addr(0x18));
    destroyFunction(f.addr(0x18), true);
}

uint32_t Avatar::setInstanceName(Avatar *self, uint32_t name) {
    guest::Frame f(0x10);
    uint32_t str = self->instances.begin + 0x20;
    uint32_t len = cx(strlen_, {name});
    return cx(string_assign, {str, name, len});
}

void Avatar::addInstance(Avatar *self, uint32_t name, AvatarStyle *style) {
    guest::Frame f(0x4A8);
    self->currentInstance = int32_t(uint32_t(int32_t(self->instances.end - self->instances.begin) >> 2) * 0x749CB29u);
    uint32_t local = f.addr(0x2C);
    cx(AV_INSTANCE_INFO_CTOR, {local});
    uint32_t len = cx(strlen_, {name});
    cx(string_assign, {local + 0x20, name, len});
    pushInstance(self, local);
    changeStyle(self, style);
    self->currentInstance = 0;
    destroyLocalInstance(local);
}

// Replace the LWF node with a new one for `path` (a motion LWF the parts attach to).
void Avatar::changeLWF(Avatar *self, uint32_t path) {
    guest::Frame f(0x58);
    uint32_t a = self_addr(self);
    if (self->lwfNode) {
        removeDetachHandlers(self);
        clearMovieEventHandlers(self);
        vcx(a, VSLOT_REMOVE_CHILD, {a, self->lwfNode, 1});
    }
    self->lwfNode = cx(lwfnode_create, {path, 0});
    vcx(a, VSLOT_60, {a, self->lwfNode});   // addChild(new node): the original passes it in r1 (create's result)
    vcx(a, VSLOT_158, {a});
    vcx(self->lwfNode, VSLOT_LWFNODE_113, {self->lwfNode, 0});
    uint32_t lwf = rw::load<uint32_t>(self->lwfNode + LWF_NODE_LWF);
    uint32_t str = f.addr(0x34);
    cx(string_init, {str, S_CHAR, 4});
    uint32_t movie = cx(lwf_search_instance, {lwf, str});
    if (rw::load<uint8_t>(str) & 1) cx(guest::OPERATOR_DELETE, {*f.at<uint32_t>(0x3C)});
    if (movie && rw::load<int32_t>(movie + 0x9C) == 1) cx(movie_goto_play, {movie, 2});
    vcx(self->lwfNode, VSLOT_LWFNODE_113, {self->lwfNode, 0});
    self->field_270 = 0;
    self->freezeOnStatus = 0;
    uint32_t k = uint32_t(self->motion - 9);
    if (k <= 30) {
        uint32_t bit = 1u << k;
        bool tint = (bit & 0x403AFFDBu) != 0;
        if (!tint && (bit & 0x40004u)) {
            self->freezeOnStatus = 1;
            tint = true;
        }
        if (tint) {
            uint32_t v = f.addr(0x24);
            cx(vec4_ctor, {v, rw::fbits(1.0f), 0, rw::fbits(1.0f), rw::fbits(1.0f)});
            rw::set<uint32_t>(self, 644, *f.at<uint32_t>(0x30));
            rw::set<uint32_t>(self, 640, *f.at<uint32_t>(0x2C));
            rw::set<uint32_t>(self, 636, *f.at<uint32_t>(0x28));
            rw::set<uint32_t>(self, 632, *f.at<uint32_t>(0x24));
            cx(vec4_dtor, {v});
        }
    }
    getWaistPosition(f.addr(0x1C), self);
    cx(vec2_dtor, {f.addr(0x1C)});
    cx(flash_change_sub, {a});
}

RW_FUNCTION(0x5C5882, "Avatar::create(const Avatar::Style&, bool)", Avatar::create);
RW_FUNCTION(0x5C58C8, "Avatar::Avatar(bool)", Avatar::construct);
RW_FUNCTION(0x5C5EDC, "Avatar::Avatar(const Avatar::Style&, bool)", Avatar::constructWithStyle);
RW_FUNCTION(0x5C5F14, "Avatar::changeStyle", Avatar::changeStyle);
RW_FUNCTION(0x5C6068, "Avatar::~Avatar", Avatar::destruct);
RW_FUNCTION(0x5C616C, "Avatar::removeDetachHandlers", Avatar::removeDetachHandlers);
RW_FUNCTION(0x5C61C8, "Avatar::clearMovieEventHandlers", Avatar::clearMovieEventHandlers);
RW_FUNCTION(0x5C6288, "Avatar::addBurst", Avatar::addBurst);
RW_FUNCTION(0x5C62DC, "Avatar::update", Avatar::update);
RW_FUNCTION(0x5C63C4, "Avatar::changeMotion", Avatar::changeMotion);
RW_FUNCTION(0x5C64B4, "Avatar::changeExpression", Avatar::changeExpression);
RW_FUNCTION(0x5C64F8, "Avatar::makeLWFPath", Avatar::makeLWFPath);
RW_FUNCTION(0x5C6688, "Avatar::gotoAndStopMovie", Avatar::gotoAndStopMovie);
RW_FUNCTION(0x5C670C, "Avatar::verifyGrandparent", Avatar::verifyGrandparent);
RW_FUNCTION(0x5C6BD0, "Avatar::updateLWFPath", Avatar::updateLWFPath);
RW_FUNCTION(0x5C6CA4, "Avatar::removeLWFPath", Avatar::removeLWFPath);
RW_FUNCTION(0x5C6CF0, "Avatar::changeCostume", Avatar::changeCostume);
RW_FUNCTION(0x5C6D84, "Avatar::changeFace", Avatar::changeFace);
RW_FUNCTION(0x5C6E18, "Avatar::changeBody", Avatar::changeBody);
RW_FUNCTION(0x5C6EAC, "Avatar::changeHair", Avatar::changeHair);
RW_FUNCTION(0x5C6F3C, "Avatar::changeHairColor", Avatar::changeHairColor);
RW_FUNCTION(0x5C6FCC, "Avatar::changeKeyblade(int, int)", Avatar::changeKeyblade);
RW_FUNCTION(0x5C7080, "Avatar::changeKeyblade(int)", Avatar::changeKeybladeCombined);
RW_FUNCTION(0x5C70C0, "Avatar::forEachAccessory", Avatar::forEachAccessory);
RW_FUNCTION(0x5C7210, "Avatar::changeAccessories", Avatar::changeAccessories);
RW_FUNCTION(0x5C7358, "Avatar::setInstanceName", Avatar::setInstanceName);
RW_FUNCTION(0x5C7384, "Avatar::addInstance", Avatar::addInstance);
RW_FUNCTION(0x5C7574, "Avatar::changeLWF", Avatar::changeLWF);
