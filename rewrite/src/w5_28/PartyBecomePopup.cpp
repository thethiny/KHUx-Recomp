// SceneManager::openPartyBecomePopup: "You joined a party" after an invitation is accepted - PartyTop_Join.json on
// popup layer 2 over a fading shadow, the party's name and rank icon, and OK ($_114: closes, then the callback).
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
constexpr uint32_t API_MEMBERS_CTL = 0x200;      // APIManager: the member list shared_ptr's control (held)
constexpr uint32_t API_PARTY = 0x84;             // APIManager: shared_ptr<Party> (control at +4)
constexpr uint32_t PARTY_RANK = 0x2C, PARTY_NAME = 0x30;
constexpr uint32_t SM_POPUP_LAYER2 = 0x3EC;
constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054;
constexpr uint32_t VT_Ok = 0xF5F110;             // $_114 {function<void(bool)>, popup, layer, shadow} (heap 0x30)
constexpr uint32_t S_Json = 0xE035A6;            // "cocostudio/publish/PartyTop_Join.json"
constexpr uint32_t S_TxtWording1 = 0xDFF14C, S_TxtWording2 = 0xDF863B, S_TxtName = 0xDFDA10, S_TxtOK = 0xDF869F;
constexpr uint32_t S_RankIcon = 0xDFDA07, S_ButtonOK = 0xDF8688;
constexpr int32_t TXT_JOINED = 103810007, TXT_JOINED_PARTY = 103800056, TXT_OK = 100100001;
constexpr uint32_t V_widgetSetTouchEnabled = 0x29C;

void label(uint32_t popup, uint32_t name, uint32_t len, int32_t textId) {
    const uint32_t text = findNode(popup, name, len);
    SceneUtil::setShadow(text);
    setTextId(text, textId);
}

void openPartyBecomePopup(uint32_t /*sm*/, uint32_t done) {
    call(fn::AudioPlaySystemSEPopUp, {});
    const uint32_t membersCtl = ld32(call(fn::APIManager_getInstance, {}) + API_MEMBERS_CTL);
    if (membersCtl) call(fn::shared_add, {membersCtl});
    const uint32_t a = call(fn::APIManager_getInstance, {});
    const uint32_t party = ld32(a + API_PARTY), partyCtl = ld32(a + API_PARTY + 4);
    if (partyCtl) call(fn::shared_add, {partyCtl});
    const uint32_t layer = ld32(sceneManager() + SM_POPUP_LAYER2);
    const uint32_t shadow = SceneUtil::createShadowSprite();
    const uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
    vcall(shadow, node::setOpacity, {shadow, 0});
    vcall(layer, node::addChildZ, {layer, shadow, 0});
    const uint32_t popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_Json});
    vcall(popup, node::setAnchorPoint, {popup, ld32(GOT_ANCHOR_MIDDLE)});
    {
        const float px = bitsf(vcall(popup, node::getPositionX, {popup}));
        const float w = ldf(vcall(popup, node::getContentSize, {popup}));
        const float x = SceneUtil::getSideMargin() + (px + w * 0.5f);
        const float py = bitsf(vcall(popup, node::getPositionY, {popup}));
        const float h = ldf(vcall(popup, node::getContentSize, {popup}) + 4);
        const float y = SceneUtil::getSideHeightMargin() + (py + h * 0.5f);
        Frame f;
        vcall(popup, node::setPosition, {popup, newVec2(f, x, y)});
    }
    vcall(layer, node::addChildZ, {layer, popup, 1});
    label(popup, S_TxtWording1, 0xC, TXT_JOINED);
    label(popup, S_TxtWording2, 0xC, TXT_JOINED_PARTY);
    {
        const uint32_t name = findNode(popup, S_TxtName, 8);
        SceneUtil::setShadow(name);
        call(fn::Text_setString, {name, party + PARTY_NAME});
    }
    label(popup, S_TxtOK, 6, TXT_OK);
    {
        const uint32_t icon = findNode(popup, S_RankIcon, 8);
        Frame f;
        const uint32_t path = f.alloc(12);
        SceneUtil::createPartyRankImagePath(path, int32_t(ld32(party + PARTY_RANK)));
        const uint32_t s = newStr(f, strData(path));
        call(fn::ImageView_loadTexture, {icon, s, 0});
        strFree(s);
        strFree(path);
    }
    const uint32_t ok = findNode(popup, S_ButtonOK, 9);
    vcall(ok, V_widgetSetTouchEnabled, {ok, 1});
    {
        Frame f;
        const uint32_t copy = f.alloc(20);
        call(fn::function_vb_copy, {copy, done});
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x30});
        if (obj) {
            st32(obj, VT_Ok);
            call(fn::function_vb_move, {obj + 8, copy});
            st32(obj + 0x20, popup);
            st32(obj + 0x24, layer);
            st32(obj + 0x28, shadow);
        }
        st32(fo + 16, obj);
        call(fn::Widget_addTouchEventListener, {ok, fo});
        functionDestroy(fo);
        functionDestroy(copy);
    }
    vcall(popup, node::setVisible, {popup, 1});
    call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(0.1f), opacity})});
    {
        Frame f;
        const uint32_t none = newEmptyFunction(f);
        call(fn::Node_runAction, {popup, SceneUtil::createOpenPopupAnimation(popup, none)});
        functionDestroy(none);
    }
    if (partyCtl) call(fn::shared_release, {partyCtl});
    if (membersCtl) call(fn::shared_release, {membersCtl});
}
}  // namespace

RW_FUNCTION(0x620DE4, "SceneManager::openPartyBecomePopup", openPartyBecomePopup);
