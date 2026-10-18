// LWFNodeGroup::changeInternal: swaps the group's current LWF node for the one of another file, with an
// "enterFrame" handler on its root movie.
#include "scene/SceneCommon.hpp"

using namespace scene;

namespace {
constexpr uint32_t G_FILES = 0x270;     // const char** the group's LWF files
constexpr uint32_t G_INDEX = 0x27C;     // the file showing
constexpr uint32_t G_NODE = 0x288;      // AudioLWFNode*
constexpr uint32_t NODE_LWF = 0x3E0;    // AudioLWFNode: its LWF::LWF*
constexpr uint32_t VT_ENTER_FRAME = 0xF5D51C;   // changeInternal $_?: the root movie's enterFrame handler
constexpr uint32_t S_ROOT = 0xDFE0ED, S_ENTER_FRAME = 0xDFE0F3;   // "_root", "enterFrame"

constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;   // (const char* path, void*)
constexpr uint32_t LWF_AddMovieEventHandler = 0x808645;   // (lwf, const string& instance, const map<string, function>&)
constexpr uint32_t HandlerMap_insertUnique = 0x530869;    // map<string, function<void(Movie*)>>::__insert_unique(hint, pair)
constexpr uint32_t HandlerMap_destroy = 0x530811;
constexpr uint32_t AEABI_MEMCPY = 0x40E164;               // import
constexpr uint32_t Node_removeChild = 0x12C;              // (child, cleanup)
}  // namespace

namespace lwf_node_group {
void changeInternal(uint32_t self, int32_t index) {
    if (const uint32_t current = ld32(self + G_NODE)) vcall(self, Node_removeChild, {self, current, 1});
    const uint32_t node = call(AudioLWFNode_create, {ld32(ld32(self + G_FILES) + 4 * uint32_t(index)), 0});
    st32(self + G_NODE, node);
    const uint32_t lwf = ld32(node + NODE_LWF);

    Frame f;
    const uint32_t instance = newStr(f, S_ROOT, 5);
    // {"enterFrame", handler}: a short string (10 characters) and an inline std::function
    const uint32_t pair = f.alloc(0x28);
    st8(pair, 10 << 1);
    call(AEABI_MEMCPY, {pair + 1, S_ENTER_FRAME, 10});
    st8(pair + 11, 0);
    const uint32_t handler = pair + 0x10;
    st32(handler + 16, handler);
    st32(handler, VT_ENTER_FRAME);
    const uint32_t handlers = f.alloc(12);   // std::map: begin node, end node (its left = root), size
    st32(handlers + 4, 0);
    st32(handlers + 8, 0);
    st32(handlers, handlers + 4);
    call(HandlerMap_insertUnique, {handlers, handlers + 4, pair});
    call(LWF_AddMovieEventHandler, {lwf, instance, handlers});
    call(HandlerMap_destroy, {handlers, ld32(handlers + 4)});
    functionDestroy(handler);
    strFree(pair);
    strFree(instance);

    vcall(self, node::addChild, {self, ld32(self + G_NODE)});
    st32(self + G_INDEX, uint32_t(index));
}

// changeInternal's enterFrame handler: the root movie stops on its last frame (and its playing flag is cleared)
void onEnterFrame(uint32_t, uint32_t moviePtr) {
    const uint32_t movie = ld32(moviePtr);
    if (ld32(movie + 0x9C) != ld32(movie + 0x98)) return;
    call(0x815C33, {movie});   // LWF::Movie::Stop
    st8(movie + 0xAE, 0);
}
}  // namespace lwf_node_group
RW_FUNCTION(0x5D5386, "LWFNodeGroup::changeInternal enterFrame lambda operator()", lwf_node_group::onEnterFrame);
RW_FUNCTION(0x5D5020, "LWFNodeGroup::changeInternal", lwf_node_group::changeInternal);
