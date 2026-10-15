// SceneAdventureSelect: the colosseum panel and its unlock animation.
#include "SceneAdventureSelect.hpp"
#include "SceneAdventureSelect_lib.hpp"

using namespace scene;
using namespace sas;
using Self = SceneAdventureSelect;

namespace {
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;       // (const char* path, void*)

// settingTitleLabel(node, "Txt", textId) with the short string built in place the way the compiler does
void txtLabel(uint32_t node, int32_t textId) {
    Frame f;
    uint32_t s = f.alloc(12);
    st8(s, 6);              // short form, 3 characters
    st8(s + 2, 'x');
    st8(s + 1, 'T');
    st8(s + 3, 't');
    st8(s + 4, 0);
    SceneUtil::settingTitleLabel(node, s, textId);
    strFree(s);
}
}  // namespace

// The colosseum unlock: over a shadow on the popup layer, a copy of the adventure buttons panel where the
// colosseum button is revealed by the Colosseum_Open LWF; lambda $_50 shows the button after 2.4 s, the shadow
// fades and lambda $_51 {layer, shadow, button, done} cleans up after 4.5 s.
void SceneAdventureSelect::colosseumOpenAnimation(Addr self, Addr done) {
    Self *s = rw::host_ptr<Self>(self);
    uint32_t layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
    uint32_t shadow = SceneUtil::createShadowSprite();
    vcall(layer, node::addChild, {layer, shadow});
    uint32_t panel = call(fn::GUIReader_widgetFromJsonFile,
                          {call(fn::GUIReader_getInstance, {}), STR_cocostudio_publish_AdventureTop2_Adv_But});
    vcall(shadow, node::addChild, {shadow, panel});
    uint32_t ownColosseum = findNode(s->panel_3B0.node, STR_Button_Colosseum, 0x10);
    setVisible(ownColosseum, 0);
    uint32_t ev = findNode(panel, STR_Button_EV, 9);
    txtLabel(ev, 102920002);
    setVisible(ev, 0);
    uint32_t colosseum = findNode(panel, STR_Button_Colosseum, 0x10);
    txtLabel(colosseum, 102920003);
    uint32_t relief = findNode(panel, STR_Button_Relief, 0xD);
    txtLabel(relief, 102900008);
    setVisible(relief, 0);
    {
        uint32_t setPosition = ld32(ld32(panel) + node::setPosition);
        float margin = SceneUtil::getSideMargin();
        float x = ldf(vcall(s->advButton, node::getPosition, {s->advButton}));
        uint32_t pos = vcall(s->advButton, node::getPosition, {s->advButton});
        Frame f;
        uint32_t v = f.alloc(8);
        call(lib::Vec2_ctorXY, {v, fbits(margin + x), rw::load<uint32_t>(pos + 4)});
        call(setPosition, {panel, v});
        call(lib::Vec2_dtor, {v});
    }
    setOpacity(shadow, 0);
    setVisible(colosseum, 0);
    uint32_t lwf = call(AudioLWFNode_create, {STR_lwf_adventure_Colosseum_Open_Colosseum_O, 0});
    {
        Frame f;
        uint32_t name = newStr(f, STR_lwf, 3);
        vcall(panel, node::addChildZName, {panel, lwf, 0, name});
        strFree(name);
    }
    {
        uint32_t lwfSetPosition = ld32(ld32(lwf) + node::setPosition);
        call(lwfSetPosition, {lwf, vcall(colosseum, node::getPosition, {colosseum})});
    }
    {
        uint32_t delay = call(fn::DelayTime_create, {0x401AAAABu /* 2.4166667f */});
        Frame f;
        uint32_t cb = newFunction(f, VT_colosseumOpenAnimation_50, {colosseum});
        uint32_t show = call(fn::CallFunc_create, {cb});
        uint32_t seq = call(fn::Sequence_create, {delay, show, 0});
        runEased(colosseum, seq);
        functionDestroy(cb);
    }
    uint32_t fade1 = call(fn::Spawn_create, {call(fn::FadeTo_create, {0x3F8CCCCDu /* 1.1f */, 128}), 0});
    uint32_t fade2 = call(fn::Spawn_create, {call(fn::FadeTo_create, {0x402AAAABu /* 2.6666667f */, 128}), 0});
    uint32_t fade3 = call(fn::Spawn_create, {call(fn::FadeTo_create, {0x40500000u /* 3.25f */, 0}), 0});
    uint32_t wait = call(fn::DelayTime_create, {0x40900000u /* 4.5f */});
    uint32_t last;
    {
        Frame f;
        uint32_t copy = f.alloc(20);
        call(fn::function_vv_copy, {copy, done});
        uint32_t cb = f.alloc(20);
        st32(cb + 16, 0);
        uint32_t obj = call(fn::operator_new, {0x30});
        if (obj) {
            st32(obj, VT_colosseumOpenAnimation_51);
            st32(obj + 8, layer);
            st32(obj + 0xC, shadow);
            st32(obj + 0x10, ownColosseum);
            call(fn::function_vv_move, {obj + 0x18, copy});
        }
        st32(cb + 16, obj);
        uint32_t finish = call(fn::CallFunc_create, {cb});
        last = call(fn::Spawn_create, {wait, finish, 0});
        functionDestroy(cb);
        functionDestroy(copy);
    }
    uint32_t seq = call(fn::Sequence_create, {fade1, fade2, fade3, last, 0});
    runEased(shadow, seq);
}
RW_FUNCTION(0x726590, "SceneAdventureSelect::colosseumOpenAnimation", SceneAdventureSelect::colosseumOpenAnimation);
