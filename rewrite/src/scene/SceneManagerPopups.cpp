// SceneManager: the popup stack (openPopup / closeCurrentPopup), scene layer changes and the background.
#include "SceneManager.hpp"
#include "SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t FunctionBool_copy = 0x46A3B5;           // std::function<bool()>(const function&)
constexpr uint32_t FunctionBool_call = 0x51022D;
constexpr uint32_t FunctionB_call = 0x474139;              // std::function<void(bool)>::operator()(bool)
constexpr uint32_t CanCloseVector_pushBackSlow = 0x622D01; // vector<function<bool()>>::__push_back_slow_path(const&)
constexpr uint32_t OnCloseVector_pushBackSlow = 0x622B65;  // vector<function<void(bool)>>::__push_back_slow_path
constexpr uint32_t BoolVector_pushBack = 0x62161D;         // vector<bool>::push_back(const bool&)
constexpr uint32_t LayerFactory_assign = 0x612E29;         // std::function<Layer*()>::operator=(const function&)
constexpr uint32_t EventDispatcher_pauseEventListenersForTarget = 0x8F2CCD;
constexpr uint32_t AppState_getNewAvatarBoardIDs = 0x4EBE3B;
constexpr uint32_t AppState_removeNewAvatarBoardIDs = 0x4EBF6D;
constexpr uint32_t IntVector_copy = 0x4AF707, IntVector_dtor = 0x478961;
constexpr uint32_t Sphere_create = 0x78CF21;
constexpr uint32_t ImageView_create = 0x8394C1;
constexpr uint32_t LWFNode_create = 0x7EDF31;
constexpr uint32_t String_assign = 0x498A1D;               // std::string::operator=(const string&)
constexpr uint32_t COLOR3B_BLACK = 0x1007E9A;

uint32_t mgrAddr(SceneManager *self) { return rw::guest_addr(self); }

// pop the last entry of a vector of 24-byte std::function slots
void popFunction(rw::Field<uint32_t> *vec) {
    uint32_t end = vec[1];
    uint32_t target = end - 24;
    do {
        uint32_t entry = end - 24;
        vec[1] = entry;
        functionDestroy(entry);
        end = vec[1];
    } while (end != target);
}

void clearFunctions(rw::Field<uint32_t> *vec) {
    uint32_t begin = vec[0], end = vec[1];
    while (end != begin) {
        uint32_t entry = end - 24;
        vec[1] = entry;
        functionDestroy(entry);
        end = vec[1];
    }
}

// push_back(const std::function&) on a vector of 24-byte std::function slots
void pushFunction(rw::Field<uint32_t> *vec, uint32_t copyCtor, uint32_t slowPush, uint32_t function) {
    uint32_t end = vec[1];
    if (end == vec[2]) {
        call(slowPush, {rw::guest_addr(vec), function});
        return;
    }
    uint32_t next = 0;
    if (end) {
        call(copyCtor, {end, function});
        next = vec[1];
    }
    vec[1] = next + 24;
}

bool anyChildren(uint32_t layer) { return int32_t(vcall(layer, node::getChildrenCount, {layer})) > 0; }
}  // namespace

// ------------------------------------------------------------------------------------------------ popups

// Register an open popup: `canClose` (std::function<bool()>) decides whether the back key may close it,
// `onClose` (std::function<void(bool)>) runs when it closes.
void SceneManager::openPopup(SceneManager *self, Addr canClose, Addr onClose, uint32_t flag) {
    Frame f;
    uint32_t flagByte = f.alloc(4);
    st8(flagByte, uint8_t(flag));
    pushFunction(self->popupCanClose, FunctionBool_copy, CanCloseVector_pushBackSlow, canClose);
    pushFunction(self->popupOnClose, fn::function_vb_copy, OnCloseVector_pushBackSlow, onClose);
    call(BoolVector_pushBack, {mgrAddr(self) + 0x5A4, flagByte});
}
RW_FUNCTION(0x61524C, "SceneManager::openPopup", SceneManager::openPopup);

// Close the top popup when its canClose callback agrees; its onClose gets `byBackKey`. Returns whether it closed.
bool SceneManager::closeCurrentPopup(SceneManager *self, uint32_t byBackKey) {
    uint32_t end = self->popupCanClose[1];
    if (end == self->popupCanClose[0]) return false;
    if (call(FunctionBool_call, {end - 24}) != 1) return false;
    Frame f;
    uint32_t onClose = f.alloc(20);
    call(fn::function_vb_copy, {onClose, self->popupOnClose[1] - 24});
    popFunction(self->popupCanClose);
    popFunction(self->popupOnClose);
    self->popupFlags[1] -= 1;
    if (ld32(onClose + 16)) {
        call(FunctionB_call, {onClose, byBackKey});
        functionDestroy(onClose);
    }
    return true;
}
RW_FUNCTION(0x6216A0, "SceneManager::closeCurrentPopup", SceneManager::closeCurrentPopup);

bool SceneManager::isOpenedPopup(SceneManager *self) {
    if (self->globalMenuOpen) return true;
    return anyChildren(self->popupLayer) || anyChildren(self->popupLayer2) || anyChildren(self->popupLayer3) ||
           anyChildren(self->popupLayerDerived) || anyChildren(self->popupLayerDerived2);
}
RW_FUNCTION(0x621518, "SceneManager::isOpenedPopup", SceneManager::isOpenedPopup);

bool SceneManager::isOpenedPopupOnly(SceneManager *self) {
    return anyChildren(self->popupLayer) || anyChildren(self->popupLayer2) || anyChildren(self->popupLayer3) ||
           anyChildren(self->popupLayerDerived) || anyChildren(self->popupLayerDerived2);
}
RW_FUNCTION(0x62158C, "SceneManager::isOpenedPopupOnly", SceneManager::isOpenedPopupOnly);

// Forget every registered popup (without running their callbacks).
void SceneManager::openPopupAllClear(SceneManager *self) {
    clearFunctions(self->popupCanClose);
    clearFunctions(self->popupOnClose);
    if (self->popupFlags[1]) self->popupFlags[1] = 0;
}
RW_FUNCTION(0x6217DC, "SceneManager::openPopupAllClear", SceneManager::openPopupAllClear);

// ------------------------------------------------------------------------------------------------ scenes

// Start a scene change: drop the old scene's callbacks (unless abortOnNextScene keeps them), clear seen avatar
// board ids, remember the fade settings and the factory of the next scene layer, and pause the current one.
void SceneManager::changeSceneLayer(SceneManager *self, Addr factory, uint32_t fadeOut, uint32_t fadeIn,
                                    uint32_t fadeType) {
    uint32_t m = mgrAddr(self);
    SceneUtil::getNexTutorialphase();
    self->flag588 = 0;
    if (!self->abortOnNextScene) {
        self->flag4CD = 1;
    } else {
        // finishSceneCallback, prepareFinishSceneCallback, onChangeProfile, function500, onBackKey: emptied
        for (uint32_t fnOff : {0x4E8u, 0x4D0u, 0x530u, 0x500u, 0x5B0u}) {
            functionDestroy(m + fnOff);
            st32(m + fnOff + 16, 0);
        }
        self->flag4CD = 0;
    }
    if (getInstance()->avatarBoardDirty) {
        Frame f;
        uint32_t ids = f.alloc(12);
        call(IntVector_copy, {ids, call(AppState_getNewAvatarBoardIDs, {call(fn::AppState_getInstance, {})})});
        if (ld32(ids + 4) != ld32(ids)) {
            uint32_t k = 0;
            do {
                int32_t id = int32_t(ld32(ld32(ids) + 4 * k));
                Frame g;
                uint32_t sphere = masterRow(g, Sphere_create, uint32_t(id));
                int32_t kind = safe32(ld32(sphere), 0x98);
                uint32_t mode = getInstance()->avatarBoardMode;
                if (uint32_t(kind) == mode || (kind == 2 && getInstance()->avatarBoardMode == 1))
                    call(AppState_removeNewAvatarBoardIDs, {call(fn::AppState_getInstance, {}), uint32_t(id)});
                sharedRelease(sphere);
                k++;
            } while (k < (ld32(ids + 4) - ld32(ids)) >> 2);
        }
        getInstance()->avatarBoardDirty = 0;
        call(IntVector_dtor, {ids});
    }
    self->flag31C = uint8_t(fadeOut);
    self->flag31D = uint8_t(fadeIn);
    self->flag31E = fadeType != 0 ? 1 : 0;
    if (fadeType == 2 || fadeType == 1) vcall(self->fadeLayer, node::setColor, {self->fadeLayer, COLOR3B_BLACK});
    call(LayerFactory_assign, {m + 0x3C8, factory});
    call(EventDispatcher_pauseEventListenersForTarget, {ld32(director() + 0x84), self->sceneLayer, 1});
}
RW_FUNCTION(0x612B98, "SceneManager::changeSceneLayer", SceneManager::changeSceneLayer);

// Replace the background (z 1): 0 an image, 1 an image at double scale, 2 a cocostudio scene file, 3 an LWF
// animation; nothing happens when the same background is already shown.
void SceneManager::changeBackground(SceneManager *self, int32_t type, Addr name) {
    uint32_t m = mgrAddr(self);
    if (self->backgroundType == type) {
        uint32_t current = m + 0x560;
        uint32_t n = strSize(name);
        if (n == strSize(current) &&
            std::memcmp(rw::host_ptr<uint8_t>(strData(name)), rw::host_ptr<uint8_t>(strData(current)), n) == 0)
            return;
    }
    if (self->background) vcall(m, node::removeChild, {m, self->background, 1});
    uint32_t node;
    Frame f;
    switch (type) {
    case 0:
    case 1: {
        node = call(ImageView_create, {});
        call(fn::ImageView_loadTexture, {node, name, 0});
        Size size = visibleSize();
        vcall(node, node::setPosition, {node, newVec2(f, size.width * 0.5f, size.height * 0.5f)});
        if (type == 1) vcall(node, node::setScale, {node, fbits(2.0f)});
        self->background = node;
        break;
    }
    case 2:
        node = SceneUtil::createNodeWithSceneFile(name);
        self->background = node;
        break;
    case 3: {
        node = call(LWFNode_create, {strData(name), 0});
        Size size = visibleSize();
        vcall(node, node::setPosition, {node, newVec2(f, size.width * 0.5f, size.height * 0.5f)});
        self->background = node;
        break;
    }
    default:
        node = self->background;
        break;
    }
    vcall(m, node::addChildZ, {m, node, 1});
    call(String_assign, {m + 0x560, name});
}
RW_FUNCTION(0x6128EC, "SceneManager::changeBackground", SceneManager::changeBackground);
