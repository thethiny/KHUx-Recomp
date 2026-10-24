// DeckMedalViewManager: the DeckMedalView registry, cached layouts and the plate blink cycle.
#include "DeckMedalViewManager.hpp"

#include "DeckMedalView.hpp"

using namespace scene;

namespace {
constexpr uint32_t Memset = 0x40E7A0;                    // __aeabi_memset(dst, n, c)
constexpr uint32_t Ref_retain = rwlib::cocos2d::Ref::retain;
constexpr uint32_t Ref_release = rwlib::cocos2d::Ref::release;
constexpr uint32_t Vector_dtor = 0x5271DD;               // cocos2d::Vector<DeckMedalView*>::~Vector
constexpr uint32_t Vector_pushBack = 0x527565;           // cocos2d::Vector<DeckMedalView*>::pushBack
constexpr uint32_t Vector_eraseObject = 0x5275D9;        // cocos2d::Vector<DeckMedalView*>::eraseObject(obj, all)
constexpr uint32_t DeckMedalView_updateLayout = 0x5276D9;

constexpr uint32_t STR_SmallMedalJson = 0xDFDC91;        // "cocostudio/publish/DeckEdit_SmallMedal.json"
constexpr uint32_t STR_RevolverMedalJson = 0xDFDCBD;     // "cocostudio/publish/DeckEdit_RevolverMedal_ver120.json"
constexpr uint32_t STR_MedalInfoJson = 0xDFDCF3;         // "cocostudio/publish/DeckEdit_Deck_MedalInfo_ver120.json"
constexpr uint32_t STR_OpenInfoJson = 0xDFDD2A;          // "cocostudio/publish/DeckEdit_OpenInfo.json"

// The layout cached in `slot`, loaded and retained on first use.
uint32_t cachedLayout(rw::Field<uint32_t> &slot, uint32_t json) {
    if (slot == 0) {
        slot = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), json});
        call(Ref_retain, {slot});
    }
    return slot;
}
}  // namespace

DeckMedalViewManager::Addr DeckMedalViewManager::getInstance() {
    uint32_t instance = ld32(INSTANCE);
    if (instance == 0) {
        instance = call(fn::operator_new, {SIZE});
        call(Memset, {instance, SIZE, 0});
        st32(INSTANCE, instance);
    }
    return instance;
}
RW_FUNCTION(0x527130, "DeckMedalViewManager::getInstance", DeckMedalViewManager::getInstance);

DeckMedalViewManager *DeckMedalViewManager::construct(DeckMedalViewManager *self) {
    call(Memset, {rw::guest_addr(self), SIZE, 0});
    return self;
}
RW_FUNCTION(0x527174, "DeckMedalViewManager::DeckMedalViewManager", DeckMedalViewManager::construct);

DeckMedalViewManager *DeckMedalViewManager::destruct(DeckMedalViewManager *self) {
    if (self->smallPanel) call(Ref_release, {self->smallPanel});
    if (self->normalPanel) call(Ref_release, {self->normalPanel});
    if (self->medalInfoPanel) call(Ref_release, {self->medalInfoPanel});
    if (self->openInfoPanel) call(Ref_release, {self->openInfoPanel});
    call(Vector_dtor, {rw::guest_addr(self) + 8});
    return self;
}
RW_FUNCTION(0x527188, "DeckMedalViewManager::~DeckMedalViewManager", DeckMedalViewManager::destruct);

// Advance the blink cycle and push the two plate opacities (status plate, fusion plate) to every view:
// phase 0 (2 s) status shown; 1 (1 s) fade status out / fusion in; 2 (2 s) fusion shown; 3 (1 s) fade back.
void DeckMedalViewManager::update(DeckMedalViewManager *self, float dt) {
    // an unknown phase passes on whatever the caller left in r5 / r6
    uint32_t a = rw::cpu->r[5], b = rw::cpu->r[6];
    float t = self->timer + dt;
    self->timer = t;
    switch (uint32_t(self->state.get())) {
    case 0:
        a = 0xFF;
        b = 0;
        if (!(t <= 2.0f || t != t)) {
            self->timer = 0.0f;
            self->state = 1;
        }
        break;
    case 1:
        if (t <= 1.0f || t != t) {
            b = f2u(t * 255.0f);
            a = 0xFF ^ b;
        } else {
            self->timer = 0.0f;
            self->state = 2;
            b = 0xFF;
            a = 0;
        }
        break;
    case 2:
        a = 0;
        b = 0xFF;
        if (!(t <= 2.0f || t != t)) {
            self->timer = 0.0f;
            self->state = 3;
        }
        break;
    case 3:
        if (t <= 1.0f || t != t) {
            a = f2u(t * 255.0f);
            b = 0xFF ^ a;
        } else {
            self->timer = 0.0f;
            self->state = 0;
            a = 0xFF;
            b = 0;
        }
        break;
    default:
        break;
    }
    if (int32_t(self->viewsEnd - self->viewsBegin) < 1) return;
    for (int32_t k = 0;;) {
        uint32_t view = ld32(self->viewsBegin + 4 * uint32_t(k));
        DeckMedalView::updateStatusAndMixedPlate(rw::host_ptr<DeckMedalView>(view), uint8_t(a), uint8_t(b));
        k++;
        if (k >= int32_t(self->viewsEnd - self->viewsBegin) >> 2) break;
    }
}
RW_FUNCTION(0x527208, "DeckMedalViewManager::update", DeckMedalViewManager::update);

void DeckMedalViewManager::addDeckMedalView(Addr self, Addr view) { call(Vector_pushBack, {self + 8, view}); }
RW_FUNCTION(0x527558, "DeckMedalViewManager::addDeckMedalView", DeckMedalViewManager::addDeckMedalView);

void DeckMedalViewManager::removeDeckMedalView(Addr self, Addr view) { call(Vector_eraseObject, {self + 8, view, 0}); }
RW_FUNCTION(0x5275CC, "DeckMedalViewManager::removeDeckMedalView", DeckMedalViewManager::removeDeckMedalView);

void DeckMedalViewManager::updateDeckMedalViews(DeckMedalViewManager *self) {
    if (int32_t(self->viewsEnd - self->viewsBegin) < 1) return;
    for (int32_t k = 0;;) {
        call(DeckMedalView_updateLayout, {ld32(self->viewsBegin + 4 * uint32_t(k)), 0});
        k++;
        if (k >= int32_t(self->viewsEnd - self->viewsBegin) >> 2) break;
    }
}
RW_FUNCTION(0x5276AA, "DeckMedalViewManager::updateDeckMedalViews", DeckMedalViewManager::updateDeckMedalViews);

DeckMedalViewManager::Addr DeckMedalViewManager::getSmallMedalPanel(DeckMedalViewManager *self) {
    return cachedLayout(self->smallPanel, STR_SmallMedalJson);
}
RW_FUNCTION(0x52B9A0, "DeckMedalViewManager::getSmallMedalPanel", DeckMedalViewManager::getSmallMedalPanel);

DeckMedalViewManager::Addr DeckMedalViewManager::getNormalMedalPanel(DeckMedalViewManager *self) {
    return cachedLayout(self->normalPanel, STR_RevolverMedalJson);
}
RW_FUNCTION(0x52B9D4, "DeckMedalViewManager::getNormalMedalPanel", DeckMedalViewManager::getNormalMedalPanel);

DeckMedalViewManager::Addr DeckMedalViewManager::getMedalInfoPanel(DeckMedalViewManager *self) {
    return cachedLayout(self->medalInfoPanel, STR_MedalInfoJson);
}
RW_FUNCTION(0x52BA08, "DeckMedalViewManager::getMedalInfoPanel", DeckMedalViewManager::getMedalInfoPanel);

DeckMedalViewManager::Addr DeckMedalViewManager::getOpenInfoPanel(DeckMedalViewManager *self) {
    return cachedLayout(self->openInfoPanel, STR_OpenInfoJson);
}
RW_FUNCTION(0x52BA3C, "DeckMedalViewManager::getOpenInfoPanel", DeckMedalViewManager::getOpenInfoPanel);
