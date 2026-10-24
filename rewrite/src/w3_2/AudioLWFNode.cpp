// AudioLWFNode: a cocos2d::LWFNode (an LWF Flash animation) that also plays the sound effects / BGM its timeline
// asks for. The LWF scripts call back through a CommandParser ("se:..." / "bgm:..." commands); the parser's
// callbacks set a pending SE / BGM id, and update() plays them on the next frame.
#include "../scene/SceneCommon.hpp"

namespace {
using scene::call;
using scene::ld32;
using scene::ld8;
using scene::st32;
using scene::st8;

// layout (0x640 bytes; cocos2d::LWFNode first)
constexpr uint32_t SIZE = 0x640;
constexpr uint32_t SECOND_VTABLE = 0x270;      // the LWFNode's secondary base (vtable + 0x308)
constexpr uint32_t LWF = 0x3E0;                // LWF::LWF* (LWFNode member)
constexpr uint32_t SE_HANDLES = 0x3FC;         // std::vector<int>: handles of the SEs started by this node
constexpr uint32_t SE_HANDLES_END = 0x400;
constexpr uint32_t SE_HANDLES_CAP = 0x404;
constexpr uint32_t PARSER = 0x408;             // CommandParser (its std::function callbacks every 0x18 bytes)
constexpr uint32_t PARSER_ON_STOP = 0x438;     // std::function<void()>
constexpr uint32_t PARSER_ON_SE = 0x468;       // std::function<void(const char*)>
constexpr uint32_t PARSER_ON_BGM = 0x480;      // std::function<void(const char*)>
constexpr uint32_t SOUND_PENDING = 0x630;      // bool: a pending SE / BGM is waiting for update()
constexpr uint32_t PENDING_SE = 0x634;         // audio::ResourceID (0 = none)
constexpr uint32_t PENDING_BGM = 0x638;        // audio::ResourceID (0 = none)
// cocos2d::Node std::function<void()> callbacks
constexpr uint32_t NODE_ON_ENTER = 0x210;
constexpr uint32_t NODE_ON_EXIT = 0x228;
// LWF::LWF: the std::function<void(const string&, Movie*, Button*)> fscommand handler
constexpr uint32_t LWF_COMMAND_HANDLER = 0xF8;

constexpr uint32_t VTABLE = 0xF94F48;              // _ZTV12AudioLWFNode + 8
constexpr uint32_t VTABLE_SECOND = 0xF95248;       // _ZTV12AudioLWFNode + 0x308

// AudioLWFNode::init lambdas (__func vtables + 8), each capturing `this`
constexpr uint32_t VT_INIT_0 = 0xF5DDB4;    // parser "stop": stopAllSEs
constexpr uint32_t VT_INIT_1 = 0xF5DD88;    // parser SE command
constexpr uint32_t VT_INIT_2 = 0xF5DD5C;    // parser BGM command
constexpr uint32_t VT_INIT_3 = 0xF5DD30;    // onEnter
constexpr uint32_t VT_INIT_4 = 0xF5DD04;    // onExit
// std::bind(&CommandParser::parse, parser, _1, _2, _3) in a std::function (__func vtable + 8)
constexpr uint32_t VT_BIND_PARSE = 0xF8F52C;
constexpr uint32_t CommandParser_parse = 0x4F28F9;

constexpr uint32_t AEABI_MEMSET = 0x40E7A0;            // __aeabi_memset(dst, n, c) (import)
constexpr uint32_t STRING_INIT = 0x47CD93;             // std::string::__init(const char*, size_t)
constexpr uint32_t LWFNode_ctor = rwlib::cocos2d::LWFNode::ctor_base;
constexpr uint32_t LWFNode_dtor = rwlib::cocos2d::LWFNode::dtor_base;
constexpr uint32_t LWFNode_initWithLWFFile = rwlib::cocos2d::LWFNode::initWithLWFFile;  // (const std::string&, void* userData)
constexpr uint32_t LWFNode_update = rwlib::cocos2d::LWFNode::update;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t CommandParser_dtor = 0x5C3831;
constexpr uint32_t VectorInt_base_dtor = 0x478961;      // std::__vector_base<int>::~__vector_base
constexpr uint32_t VectorInt_push_back_slow = 0x47D709; // std::vector<int>::__push_back_slow_path(int&&)
constexpr uint32_t FunctionVV_swap = 0x4EE86D;          // std::function<void()>::swap
constexpr uint32_t FunctionVS_swap = 0x4E2961;          // std::function<void(const char*)>::swap
constexpr uint32_t FunctionVV_assign = 0x4EB9CD;        // std::function<void()>::operator=(const function&)
constexpr uint32_t FunctionCommand_assign = 0x5C9BE5;   // std::function<void(const string&, Movie*, Button*)>::operator=
constexpr uint32_t Audio_getInstance = 0x5BECFD;        // audio::Manager::getInstance()
constexpr uint32_t Audio_getLayer = 0x5BF33D;           // (shared_ptr<Layer>* out, Manager*, int layer)
constexpr uint32_t Audio_stop = 0x5BF3AD;               // Manager::stop(int handle, int fadeMs)
constexpr uint32_t AudioLayer_playSE = 0x5BF785;        // Layer::playSE(ResourceID, int, int) -> handle
constexpr uint32_t AudioReplaceGameMainBGM = 0x5C0FA1;  // (ResourceID, int fadeMs)
constexpr uint32_t SHARED_RELEASE = 0xC8DBD1;

constexpr uint32_t V_DELETE = 0x04;   // deleting destructor (vtable byte offset)

// a std::function on the guest stack holding an inline lambda {vtable, captures}
uint32_t inlineFunction(scene::Frame &f, uint32_t vtable, std::initializer_list<uint32_t> captures) {
    return scene::newFunction(f, vtable, captures);
}

}  // namespace

struct AudioLWFNode {
    // stop every SE this node started (the handles stay in the vector)
    static void stopAllSEs(uint32_t self) {
        uint32_t end = ld32(self + SE_HANDLES_END);
        for (uint32_t it = ld32(self + SE_HANDLES); it != end; it += 4) {
            uint32_t mgr = call(Audio_getInstance, {});
            call(Audio_stop, {mgr, ld32(it), 250});
        }
    }

    static uint32_t init(uint32_t self, uint32_t file, uint32_t userData) {
        uint32_t ok;
        {
            scene::Frame f;
            uint32_t path = f.alloc(12);
            call(STRING_INIT, {path, file, scene::host_strlen(file)});
            ok = call(LWFNode_initWithLWFFile, {self, path, userData});
            scene::strFree(path);
        }
        if (!ok) return 0;
        // the command parser's callbacks (swapped into place: the old ones are destroyed with the temporaries)
        {
            scene::Frame f;
            uint32_t fo = inlineFunction(f, VT_INIT_0, {self});
            call(FunctionVV_swap, {fo, self + PARSER_ON_STOP});
            scene::functionDestroy(fo);
        }
        {
            scene::Frame f;
            uint32_t fo = inlineFunction(f, VT_INIT_1, {self});
            call(FunctionVS_swap, {fo, self + PARSER_ON_SE});
            scene::functionDestroy(fo);
        }
        {
            scene::Frame f;
            uint32_t fo = inlineFunction(f, VT_INIT_2, {self});
            call(FunctionVS_swap, {fo, self + PARSER_ON_BGM});
            scene::functionDestroy(fo);
        }
        // lwf->fscommand handler = std::bind(&CommandParser::parse, &parser, _1, _2, _3)
        {
            uint32_t lwf = ld32(self + LWF);
            scene::Frame f;
            uint32_t fo = inlineFunction(f, VT_BIND_PARSE, {CommandParser_parse, 0, self + PARSER});
            call(FunctionCommand_assign, {lwf + LWF_COMMAND_HANDLER, fo});
            scene::functionDestroy(fo);
        }
        {
            scene::Frame f;
            uint32_t fo = inlineFunction(f, VT_INIT_3, {self});
            call(FunctionVV_assign, {self + NODE_ON_ENTER, fo});
            scene::functionDestroy(fo);
        }
        {
            scene::Frame f;
            uint32_t fo = inlineFunction(f, VT_INIT_4, {self});
            call(FunctionVV_assign, {self + NODE_ON_EXIT, fo});
            scene::functionDestroy(fo);
        }
        return 1;
    }

    static uint32_t create(uint32_t file, uint32_t userData) {
        uint32_t self = call(scene::fn::operator_new, {SIZE});
        call(AEABI_MEMSET, {self, SIZE, 0});
        call(LWFNode_ctor, {self});
        st32(self, VTABLE);
        st32(self + SECOND_VTABLE, VTABLE_SECOND);
        // the inline member constructors: the SE handle vector, the parser's empty std::function slots, the
        // pending sound state
        st32(self + SE_HANDLES, 0);
        st32(self + SE_HANDLES_END, 0);
        st32(self + SE_HANDLES_CAP, 0);
        for (uint32_t fn = 0x418; fn <= 0x628; fn += 0x18) st32(self + fn, 0);
        st8(self + SOUND_PENDING, 0);
        st32(self + PENDING_SE, 0);
        st32(self + PENDING_BGM, 0);
        if (init(self, file, userData) == 1) {
            call(Ref_autorelease, {self});
            return self;
        }
        scene::vcall(self, V_DELETE, {self});
        return 0;
    }

    static uint32_t destruct(uint32_t self) {
        st32(self, VTABLE);
        st32(self + SECOND_VTABLE, VTABLE_SECOND);
        stopAllSEs(self);
        call(CommandParser_dtor, {self + PARSER});
        call(VectorInt_base_dtor, {self + SE_HANDLES});
        call(LWFNode_dtor, {self});
        return self;
    }

    static void destructDelete(uint32_t self) { call(scene::fn::operator_delete, {destruct(self)}); }

    // plays the SE / BGM the timeline asked for since the last frame
    static void update(uint32_t self, float dt) {
        call(LWFNode_update, {self, scene::fbits(dt)});
        if (!ld8(self + SOUND_PENDING)) return;
        st8(self + SOUND_PENDING, 0);
        uint32_t se = ld32(self + PENDING_SE);
        if (se) {
            scene::Frame f;
            uint32_t layer = f.alloc(8);    // shared_ptr<audio::Layer>
            uint32_t handle = f.alloc(4);
            call(Audio_getLayer, {layer, call(Audio_getInstance, {}), 0});
            uint32_t h = call(AudioLayer_playSE, {ld32(layer), se, 0, 0});
            if (ld32(layer + 4)) call(SHARED_RELEASE, {ld32(layer + 4)});
            rw::store32(rw::frame_ptr<uint32_t>(handle), h);
            // seHandles.push_back(h)
            uint32_t end = ld32(self + SE_HANDLES_END);
            if (end < ld32(self + SE_HANDLES_CAP)) {
                uint32_t base = 0;
                if (end != 0) {
                    st32(end, h);
                    base = ld32(self + SE_HANDLES_END);
                }
                st32(self + SE_HANDLES_END, base + 4);
            } else {
                call(VectorInt_push_back_slow, {self + SE_HANDLES, handle});
            }
            st32(self + PENDING_SE, 0);
        }
        uint32_t bgm = ld32(self + PENDING_BGM);
        if (bgm) {
            call(AudioReplaceGameMainBGM, {bgm, 500});
            st32(self + PENDING_BGM, 0);
        }
    }
};

RW_FUNCTION(0x5EB6B4, "AudioLWFNode::create(const char*, void*)", AudioLWFNode::create);
RW_FUNCTION(0x5EB800, "AudioLWFNode::init(const char*, void*)", AudioLWFNode::init);
RW_FUNCTION(0x5EBA80, "AudioLWFNode::~AudioLWFNode", AudioLWFNode::destruct);
RW_FUNCTION(0x5EBB00, "AudioLWFNode::stopAllSEs", AudioLWFNode::stopAllSEs);
RW_FUNCTION(0x5EBB38, "AudioLWFNode::~AudioLWFNode (deleting)", AudioLWFNode::destructDelete);
RW_FUNCTION(0x5EBB58, "AudioLWFNode::update(float)", AudioLWFNode::update);
