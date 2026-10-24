// Avatar Board: opening the selected sphere (Unlock). SceneSphereBoard's button plays the open LWF, then
// SphereUI::sphereUpdateIncentive sends /user/sphere/update (42) with the picked sphere; its callback opens the
// "you got" popup (openPopUpIncentiveGet -> openPopUpcommon: the reward's text and icon by its type), and the
// popup's OK closes it, then open_incentive pays the sphere's cost and marks it open (or the avatar-parts
// tutorial first).
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t STRLEN = 0x40E08C;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SafeBuffer_read = 0x45A269;             // (sb, out, src, n, index)
constexpr uint32_t StringBuffer_ctor = 0x49140D;           // (buf, fmt, ...)
constexpr uint32_t TextData_format = 0x492929;             // (out string, type, id, ...)
constexpr uint32_t Medal_create = 0x785871, Material_create = 0x7852E5, Keyblade_create = 0x783F9D;
constexpr uint32_t AvatarParts_create = 0x778DE1, Title_create = 0x78FA8D, Skill_create = 0x78BDE9;
constexpr uint32_t SM_getPopupLayer = 0x613E69;
constexpr uint32_t SM_enableLoadingIndicator = 0x60D169, SM_openNoTapScreen = 0x61FB25;
constexpr uint32_t FunctionVB_assign = 0x490109;           // function<void(bool)>::operator=(const function&)
constexpr uint32_t IntSet_insertUnique = 0x5C002F;         // (pair out, unordered_set<int>*, const int&)
constexpr uint32_t IntSet_clear = 0x7998FD;
constexpr uint32_t APIManager_executeUserSphereUpdate = 0x4CCD69;   // (api, board, const set&, int, const function&)
constexpr uint32_t ImageView_loadTextureTex = rwlib::cocos2d::ui::ImageView::loadTexture__PNS_9Texture2DE;    // ImageView::loadTexture(Texture2D*)
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t SceneUtil_createUserTitleImagePath = 0x46ED99, SceneUtil_setUserTitleOutline = 0x45BD95;
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775;  // (const string&, const function<void()>&, bool)
constexpr uint32_t SceneUtil_openTutorialPopup = 0x4684C9;  // (out, id, function, ...)
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;      // (path, 0)
constexpr uint32_t LWFNode_create = rwlib::cocos2d::LWFNode::create;           // (path, 0)
constexpr uint32_t LWF_Movie_GotoAndPlay = rwlib::LWF::Movie::GotoAndPlay__i;
constexpr uint32_t DelayTime_create = rwlib::cocos2d::DelayTime::create, CallFunc_create = rwlib::cocos2d::CallFunc::create, Sequence_create = rwlib::cocos2d::Sequence::create;
constexpr uint32_t Widget_isBright = rwlib::cocos2d::ui::Widget::isBright;
constexpr uint32_t audio_getInstance = 0x5BECFD, audio_getLayer = 0x5BF33D, Layer_playSE = 0x5BF785;
// SphereUI
constexpr uint32_t loadMedalTexture = 0x7962E5, loadMaterialTexture = 0x7963B1, loadKeyBladeTexture = 0x79647D;
constexpr uint32_t loadSkillTexture = 0x796549, loadAvatarSetTexture = 0x796615, loadAvatarTexture = 0x7966E1;
constexpr uint32_t get_skill_image = 0x797F3D;
constexpr uint32_t update_status = 0x7941CD, setOpenCursor = 0x794C0D, is_open_incentive = 0x79597D;
}  // namespace g

// SphereUI fields
constexpr uint32_t UI_SPHERE = 0x14;           // ConnectingSphere*
constexpr uint32_t UI_KIND = 0x2B8, UI_CURSOR = 0x36C, UI_BLOCK = 0x424, UI_STATUS = 0x4D8;
constexpr uint32_t UI_TXT_POINT = 0x598, UI_RELEASE_BUTTON = 0x5A4, UI_MODE = 0x5A8;
// a masu record (k = 1..23): this + 0x18 + 0x1C * k
constexpr uint32_t M_COST = 0x00, M_TYPE = 0x04, M_ID = 0x0C, M_SKILL = 0x14, M_COUNT = 0x18;
inline uint32_t masu(uint32_t self, uint32_t k) { return self + 0x18 + 0x1C * k; }
// SphereBlock fields
constexpr uint32_t B_SELECTED = 0x278, B_ICON_A = 0x288, B_ICON_B = 0x28C, B_ICON_C = 0x290;
// ConnectingSphere fields
constexpr uint32_t CS_PICKED = 0x14, CS_REQUEST = 0x84, CS_POINTS = 0x88, CS_BOARD = 0x8C;
// SceneManager
constexpr uint32_t SM_PARTY_RELOAD = 0x4CC, SM_ON_PARTY = 0x548;
// SceneSphereBoard
constexpr uint32_t SB_UI = 0x28C;

constexpr uint32_t VT_UpdateDone = 0xF6EE94;   // sphereUpdateIncentive $_0 {this}
constexpr uint32_t VT_OkTouch = 0xF6EE10;      // openPopUpcommon $_1 {popup, layer, shadow, tutorial, open, this}
constexpr uint32_t VT_Closed = 0xF6EE3C;       // $_1's lambda {&layer, &shadow, &popup, &tutorial, &open}
constexpr uint32_t VT_Opened = 0xF6EDE4;       // $_7 {open, popup}
constexpr uint32_t VT_Tutorial = 0xF6EE68;     // $_1's lambda's tutorial callback {open, this}
constexpr uint32_t VT_UnlockDone = 0xF66BA8;   // SceneSphereBoard unlock button: the LWF's end {scene}
constexpr uint32_t RARITY_TEXTS = 0xEAC224;    // medal rarity -> TextData id

constexpr uint32_t S_PercentD = 0xDF86DE, S_PopupGetJson = 0xE09BD2;
constexpr uint32_t S_TxtGetLabel = 0xE09BA6, S_TxtOk = 0xE09BCB, S_ButtonOk = 0xDF9564;
constexpr uint32_t S_AVT = 0xDFF863, S_MDL = 0xE09BB4, S_SKL = 0xE09BB8, S_KB = 0xDFEF39, S_MTR = 0xE09BBC;
constexpr uint32_t S_HP = 0xE09BC0, S_AP = 0xDF93C5, S_SG = 0xE09BC3, S_COST = 0xE09BC6;
constexpr uint32_t S_RankTitleBoard = 0xDF940E, S_IconSkil = 0xDF8F81, S_RankTitleName = 0xDF941E;
constexpr uint32_t S_IconAvatar = 0xDFEBAB, S_IconBase = 0xDFEB70, S_IconBaseL = 0xDFEB7A;
constexpr uint32_t S_AreaPoint = 0xDF9FB4, S_ResultRewardLwf = 0xDF9FBF, S_SphereCompleteLwf = 0xE09C62;
constexpr uint32_t S_SpIconOpenLwf = 0xE056D2;
constexpr int32_t TXT_SKILL = 103400044, TXT_GET_FORMAT = 103401015, TXT_GOT_ITEM = 103400016;
constexpr int32_t TXT_TITLE = 103400030, TXT_HP = 103400017, TXT_AVATAR_SET = 103400013, TXT_AP = 103400018;
constexpr int32_t TXT_COST = 103400019, TXT_AVATAR_PARTS = 103400014, TXT_OK = 100100001;
constexpr uint32_t SE_POPUP_GET = 509100420, SE_UNLOCK = 509000510;
constexpr uint32_t V_REMOVE_CHILD = 0x12C, V_SET_TOUCH_ENABLED = 0x29C;

void setTextChars(uint32_t text, uint32_t chars) {
    Frame f;
    const uint32_t s = f.alloc(12);
    strInit(s, chars, call(g::STRLEN, {chars}));
    call(fn::Text_setString, {text, s});
    strFree(s);
}
// out (len bytes at offset) of a master row's SafeBuffer
void sbRead(uint32_t sb, uint32_t out, uint32_t offset, uint32_t len) {
    rw::call(g::SafeBuffer_read, {sb, out, ld32(sb + 0xC) + offset, len, offset});
}
inline void setVisible(uint32_t node, uint32_t v) { vcall(node, node::setVisible, {node, v}); }
void playSE(uint32_t id) {
    Frame f;
    const uint32_t layer = f.alloc(8);
    call(g::audio_getLayer, {layer, call(g::audio_getInstance, {}), 0});
    call(g::Layer_playSE, {ld32(layer), id, 0, 0});
    sharedRelease(layer);
}
}  // namespace

struct SphereIncentive {
    // ------------------------------------------------------------------------------------------------ SphereUI
    // the points of the board's currency less n, shown
    static void decPoint(uint32_t self, uint32_t n) {
        const uint32_t cs = ld32(self + UI_SPHERE);
        st32(cs + CS_POINTS, ld32(cs + CS_POINTS) - n);
        const uint32_t text = ld32(self + UI_TXT_POINT);
        Frame f;
        const uint32_t buf = f.alloc(0x404);
        call(g::StringBuffer_ctor, {buf, S_PercentD, ld32(ld32(self + UI_SPHERE) + CS_POINTS)});
        setTextChars(text, buf);
    }

    // the kind of the selected sphere that can be opened (and its cell), or 0
    static uint32_t selectIncentiveSub(uint32_t self, uint32_t rowRef, uint32_t colRef) {
        st32(rowRef, 0);
        for (;;) {
            st32(colRef, 0);
            for (uint32_t col = 0; col < 9; col++) {
                const uint32_t row = ld32(rowRef);
                const uint32_t at = 36 * row + 4 * col;
                const uint32_t block = ld32(self + UI_BLOCK + at);
                if (block && ld8(block + B_SELECTED) && ld32(self + UI_STATUS + at) == 2) return ld32(self + UI_KIND + at);
                st32(colRef, col + 1);
            }
            const uint32_t next = ld32(rowRef) + 1;
            st32(rowRef, next);
            if (next >= 5) return 0;
        }
    }

    // the selected sphere's block when it can be opened, or 0
    static uint32_t getSelectIncentive(uint32_t self) {
        for (uint32_t row = 0; row < 5; row++)
            for (uint32_t col = 0; col < 9; col++) {
                const uint32_t at = 36 * row + 4 * col;
                const uint32_t block = ld32(self + UI_BLOCK + at);
                if (block && ld8(block + B_SELECTED) && ld32(self + UI_STATUS + at) == 2) return block;
            }
        return 0;
    }

    // pay the selected sphere's cost and mark it open (its icon shown, the others hidden); 1 when done
    static uint32_t openIncentive(uint32_t self) {
        Frame f;
        const uint32_t row = f.alloc(4), col = f.alloc(4);
        const uint32_t kind = selectIncentiveSub(self, row, col);
        if (!kind) return 0;
        const uint32_t cost = ld32(masu(self, kind) + M_COST);
        if (int32_t(ld32(ld32(self + UI_SPHERE) + CS_POINTS)) < int32_t(cost)) return 0;
        decPoint(self, cost);
        const uint32_t at = 36 * ld32(row) + 4 * ld32(col);
        st32(self + UI_STATUS + at, 1);
        const auto block = [&] { return ld32(self + UI_BLOCK + at); };
        setVisible(ld32(block() + B_ICON_A), 1);
        setVisible(ld32(block() + B_ICON_B), 0);
        setVisible(ld32(block() + B_ICON_C), 0);
        st8(block() + B_SELECTED, 0);
        call(g::update_status, {self});
        call(g::setOpenCursor, {self});
        return 1;
    }

    // /user/sphere/update with the selected sphere
    static void sphereUpdateIncentive(uint32_t self) {
        Frame f;
        {
            const uint32_t sm = sceneManager();
            const uint32_t none = newEmptyFunction(f);
            st8(sm + SM_PARTY_RELOAD, 1);
            call(g::FunctionVB_assign, {sm + SM_ON_PARTY, none});
            functionDestroy(none);
        }
        call(g::SM_enableLoadingIndicator, {sceneManager(), 1});
        const uint32_t kind = f.alloc(4), row = f.alloc(4), col = f.alloc(8);
        st32(kind, selectIncentiveSub(self, row, col));
        call(g::IntSet_insertUnique, {col, ld32(self + UI_SPHERE) + CS_PICKED, kind});
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t cs = ld32(self + UI_SPHERE);
        const uint32_t done = newFunction(f, VT_UpdateDone, {self});
        rw::call(g::APIManager_executeUserSphereUpdate, {api, ld32(cs + CS_BOARD), cs + CS_PICKED, 0, done});
        functionDestroy(done);
        call(g::IntSet_clear, {ld32(self + UI_SPHERE) + CS_PICKED});
        st32(ld32(self + UI_SPHERE) + CS_REQUEST, 0);
    }

    // the icon id of a reward: medal (types 0 / 3), material (5), keyblade (13), avatar parts (18-20, 23)
    static uint32_t getIncentiveImage(uint32_t self, uint32_t index) {
        const uint32_t type = ld32(masu(self, index + 1) + M_TYPE), id = ld32(masu(self, index + 1) + M_ID);
        Frame f;
        const uint32_t row = f.alloc(8), out = f.alloc(4);
        if (int32_t(type) > 12) {
            if (type == 13) {
                call(g::Keyblade_create, {row, id});
                sbRead(ld32(row), out, 0xCC, 4);
            } else if (type - 18 < 3 || type == 23) {
                call(g::AvatarParts_create, {row, id});
                sbRead(ld32(row), out, 0, 4);
            } else {
                return 0;
            }
        } else if (type == 0 || type == 3) {
            call(g::Medal_create, {row, id});
            sbRead(ld32(row), out, 4, 4);
        } else if (type == 5) {
            call(g::Material_create, {row, id});
            sbRead(ld32(row), out, 0, 4);
        } else {
            return 0;
        }
        const uint32_t value = ld32(out);
        sharedRelease(row);
        return value;
    }

    static void openPopUpIncentiveGet(uint32_t self) {
        const uint32_t popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_PopupGetJson});
        Frame f;
        const uint32_t row = f.alloc(4), col = f.alloc(4);
        const uint32_t kind = selectIncentiveSub(self, row, col);
        openPopUpCommon(self, popup, kind - 1, 1);
    }

    // The reward popup of masu `index` (+1): on the popup layer over a fading shadow, the reward's text by its
    // type and its icon; OK closes it ($_1).
    static uint32_t openPopUpCommon(uint32_t self, uint32_t popup, uint32_t index, uint32_t open) {
        uint32_t tutorial = 0;
        setVisible(ld32(self + UI_CURSOR), 0);
        const uint32_t layer = call(g::SM_getPopupLayer, {sceneManager()});
        const uint32_t shadow = SceneUtil::createShadowSprite();
        const uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
        vcall(shadow, node::setOpacity, {shadow, 0});
        call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(0.15f), opacity})});
        vcall(layer, node::addChild, {layer, shadow});
        vcall(layer, node::addChild, {layer, popup});
        vcall(popup, node::setAnchorPoint, {popup, VEC2_ANCHOR_MIDDLE});
        {
            const float px = bitsf(vcall(popup, node::getPositionX, {popup}));
            const float w = ldf(vcall(popup, node::getContentSize, {popup}));
            const float marginX = SceneUtil::getSideMargin();
            const float py = bitsf(vcall(popup, node::getPositionY, {popup}));
            const float yHalf = py + ldf(vcall(popup, node::getContentSize, {popup}) + 4) * 0.5f;
            const float y = SceneUtil::getSideHeightMargin() + yHalf;
            Frame f;
            vcall(popup, node::setPosition, {popup, newVec2(f, marginX + (px + w * 0.5f), y)});
        }
        const uint32_t image = getIncentiveImage(self, index);
        const uint32_t m = masu(self, index + 1);
        const uint32_t skill = ld32(m + M_SKILL), count = ld32(m + M_COUNT), id = ld32(m + M_ID);
        const uint32_t type = ld32(m + M_TYPE);
        const uint32_t label = findNode(popup, S_TxtGetLabel, 0xD);
        SceneUtil::setShadow(label);
        // the text
        {
            Frame f;
            const auto format = [&](uint32_t td, uint32_t name) {
                const uint32_t out = f.alloc(12);
                call(g::TextData_format, {out, 0, uint32_t(TXT_GET_FORMAT), textDataStr(td), name});
                setTextChars(label, strData(out));
                strFree(out);
            };
            const auto named = [&](int32_t text, uint32_t create, uint32_t offset, uint32_t len) {
                const uint32_t td = textData(f, text);
                const uint32_t fmt = textDataStr(td);
                const uint32_t row = f.alloc(8), name = f.alloc(len + 3 & ~3u);
                call(create, {row, id});
                sbRead(ld32(row), name, offset, len);
                const uint32_t buf = f.alloc(0x404);
                call(g::StringBuffer_ctor, {buf, fmt, name});
                setTextChars(label, buf);
                sharedRelease(row);
                sharedRelease(td);
            };
            const auto counted = [&](int32_t text) {
                const uint32_t td = textData(f, text);
                const uint32_t buf = f.alloc(0x404);
                call(g::StringBuffer_ctor, {buf, textDataStr(td), count});
                setTextChars(label, buf);
                sharedRelease(td);
            };
            switch (type) {
            case 0: {
                const uint32_t td = textData(f, TXT_SKILL);
                const uint32_t row = f.alloc(8), name = f.alloc(0x44);
                call(g::Skill_create, {row, skill});
                sbRead(ld32(row), name, 0xC, 0x41);
                format(td, name);
                sharedRelease(row);
                sharedRelease(td);
                break;
            }
            case 3: {
                const uint32_t row = f.alloc(8), rarity = f.alloc(4);
                call(g::Medal_create, {row, id});
                sbRead(ld32(row), rarity, 0x454, 4);
                sharedRelease(row);
                const uint32_t td = textData(f, int32_t(ld32(RARITY_TEXTS + 4 * ld32(rarity))));
                const uint32_t row2 = f.alloc(8), name = f.alloc(0x44);
                call(g::Medal_create, {row2, id});
                sbRead(ld32(row2), name, 0x10, 0x41);
                format(td, name);
                sharedRelease(row2);
                sharedRelease(td);
                break;
            }
            case 5: named(TXT_GOT_ITEM, g::Material_create, 0xC, 0x41); break;
            case 7: named(TXT_TITLE, g::Title_create, 8, 0x41); break;
            case 0xC: counted(TXT_HP); break;
            case 0xD: named(TXT_GOT_ITEM, g::Keyblade_create, 4, 0x41); break;
            case 0x12: named(TXT_AVATAR_SET, g::AvatarParts_create, 4, 0x81); tutorial = 1; break;
            case 0x13:
            case 0x14: named(TXT_GOT_ITEM, g::AvatarParts_create, 4, 0x81); tutorial = 1; break;
            case 0x15: counted(TXT_AP); break;
            case 0x16: counted(TXT_COST); break;
            case 0x17: named(TXT_AVATAR_PARTS, g::AvatarParts_create, 4, 0x81); tutorial = 1; break;
            default: break;
            }
        }
        // the icons: all hidden, then the reward's
        const uint32_t avt = findNode(popup, S_AVT, 3), mdl = findNode(popup, S_MDL, 3), skl = findNode(popup, S_SKL, 3);
        const uint32_t kb = findNode(popup, S_KB, 2), mtr = findNode(popup, S_MTR, 3), hp = findNode(popup, S_HP, 2);
        const uint32_t ap = findNode(popup, S_AP, 2), sg = findNode(popup, S_SG, 2), cost = findNode(popup, S_COST, 4);
        const uint32_t titleBoard = findNode(popup, S_RankTitleBoard, 0xF);
        const uint32_t iconSkill = findNode(popup, S_IconSkil, 9);
        const uint32_t titleName = findNode(popup, S_RankTitleName, 0xE);
        for (const uint32_t n : {avt, mdl, skl, kb, mtr, hp, ap, sg, cost, titleBoard, iconSkill}) setVisible(n, 0);
        const auto texture = [&](uint32_t view, uint32_t loader) {
            if (image) call(g::ImageView_loadTextureTex, {view, call(loader, {self, image})});
            setVisible(view, 1);
        };
        const auto avatar = [&](uint32_t loader) {
            if (image) {
                const uint32_t icon = findNode(avt, S_IconAvatar, 0xB);
                call(g::ImageView_loadTextureTex, {icon, call(loader, {self, image})});
                Frame f;
                const uint32_t row = f.alloc(8), kindOut = f.alloc(4);
                call(g::AvatarParts_create, {row, id});
                sbRead(ld32(row), kindOut, 0x8C, 4);
                const bool small = ld32(kindOut) == 2;
                setVisible(findNode(avt, small ? S_IconBase : S_IconBaseL, small ? 9 : 11), 0);
                sharedRelease(row);
            }
            setVisible(avt, 1);
        };
        switch (type) {
        case 0:
        case 3:
            if (image) call(g::ImageView_loadTextureTex, {mdl, call(g::loadMedalTexture, {self, image})});
            setVisible(mdl, 1);
            if (int32_t(skill) >= 2) {
                const uint32_t skillImage = call(g::get_skill_image, {self, skill});
                call(g::ImageView_loadTextureTex, {iconSkill, call(g::loadSkillTexture, {skillImage, skillImage})});
                setVisible(iconSkill, 1);
            }
            break;
        case 5: texture(mtr, g::loadMaterialTexture); break;
        case 7: {
            Frame f;
            const uint32_t row = f.alloc(8), name = f.alloc(0x44), titleImage = f.alloc(4);
            call(g::Title_create, {row, id});
            sbRead(ld32(row), name, 8, 0x41);
            sbRead(ld32(row), titleImage, 0x154, 4);
            const uint32_t path = f.alloc(12);
            call(g::SceneUtil_createUserTitleImagePath, {path, ld32(titleImage)});
            const uint32_t s = newStr(f, strData(path));
            call(fn::ImageView_loadTexture, {titleBoard, s, 0});
            strFree(s);
            strFree(path);
            call(g::SceneUtil_setUserTitleOutline, {titleName, 1});
            setTextChars(titleName, name);
            setVisible(titleBoard, 1);
            sharedRelease(row);
            break;
        }
        case 0xC: setVisible(hp, 1); break;
        case 0xD: texture(kb, g::loadKeyBladeTexture); break;
        case 0x12:
        case 0x17: avatar(g::loadAvatarSetTexture); break;
        case 0x13:
        case 0x14: avatar(g::loadAvatarTexture); break;
        case 0x15: setVisible(ap, 1); break;
        case 0x16: setVisible(cost, 1); break;
        default: break;
        }
        // OK and the open animation
        const uint32_t okText = findNode(popup, S_TxtOk, 6);
        SceneUtil::setShadow(okText);
        setTextId(okText, TXT_OK);
        const uint32_t ok = findNode(popup, S_ButtonOk, 9);
        Frame f;
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x1C});
        if (obj) {
            st32(obj, VT_OkTouch);
            st32(obj + 4, popup);
            st32(obj + 8, layer);
            st32(obj + 12, shadow);
            st32(obj + 16, tutorial);
            st32(obj + 20, open);
            st32(obj + 24, self);
        }
        st32(fo + 16, obj);
        call(fn::Widget_addTouchEventListener, {ok, fo});
        functionDestroy(fo);
        const uint32_t opened = newFunction(f, VT_Opened, {open, popup});
        const uint32_t anim = call(g::EaseSineOut_create, {SceneUtil::createOpenPopupAnimation(popup, opened)});
        call(fn::Node_runAction, {popup, anim});
        functionDestroy(opened);
        return 0;
    }

    // ------------------------------------------------------------------------------------------------ the lambdas
    // $_7 {open, popup}: the popup is in - the reward LWF on Area_Point (or the board-complete LWF)
    static void popupOpened(uint32_t functor) {
        const uint32_t area = findNode(ld32(functor + 8), S_AreaPoint, 10);
        if (ld8(functor + 4)) {
            playSE(SE_POPUP_GET);
            const uint32_t lwf = call(g::LWFNode_create, {S_ResultRewardLwf, 0});
            const uint32_t parent = vcall(area, node::getParent, {area});
            vcall(parent, node::addChild, {parent, lwf});
            vcall(lwf, node::setPosition, {lwf, vcall(area, node::getPosition, {area})});
        } else {
            const uint32_t lwf = call(g::AudioLWFNode_create, {S_SphereCompleteLwf, 0});
            vcall(area, node::addChild, {area, lwf});
        }
    }
    // its __clone(__base*)
    static uint32_t popupOpenedCloneInto(uint32_t self, uint32_t to) {
        if (!to) return self;
        st32(to, VT_Opened);
        st32(to + 4, ld32(self + 4));
        st32(to + 8, ld32(self + 8));
        return ld32(self + 8);
    }

    // $_1 {popup, layer, shadow, tutorial, open, this}: OK - the popup closes ($_1's lambda after it)
    static uint32_t okClone(uint32_t self) {
        const uint32_t obj = call(fn::operator_new, {0x1C});
        if (obj) {
            st32(obj, VT_OkTouch);
            for (uint32_t k = 4; k < 0x1C; k += 4) st32(obj + k, ld32(self + k));
        }
        return obj;
    }
    static void okTouched(uint32_t functor, uint32_t /*sender*/, uint32_t typeRef) {
        if (ld32(typeRef) != 2) return;
        call(g::AudioPlaySystemSEDecide, {});
        const uint32_t popup = ld32(functor + 4);
        Frame f;
        const uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x18});
        if (obj) {
            st32(obj, VT_Closed);
            st32(obj + 4, functor + 8);
            st32(obj + 8, functor + 12);
            st32(obj + 12, functor + 4);
            st32(obj + 16, functor + 16);
            st32(obj + 20, functor + 20);
        }
        st32(fo + 16, obj);
        const uint32_t anim = call(g::SceneUtil_createClosePopupAnimation, {popup, fo});
        call(fn::Node_runAction, {popup, call(g::EaseSineOut_create, {anim})});
        functionDestroy(fo);
    }
    // $_1's lambda {&layer, &shadow, &popup, &tutorial, &open (this follows)}
    static uint32_t closedClone(uint32_t self) {
        const uint32_t obj = call(fn::operator_new, {0x18});
        if (obj) {
            st32(obj, VT_Closed);
            for (uint32_t k = 4; k < 0x18; k += 4) st32(obj + k, ld32(self + k));
        }
        return obj;
    }
    // the popup is gone: shadow and popup removed; the avatar-parts tutorial (17), or open the sphere
    static void closed(uint32_t functor) {
        const uint32_t layer = ld32(ld32(functor + 4));
        vcall(layer, V_REMOVE_CHILD, {layer, ld32(ld32(functor + 8)), 1});
        const uint32_t layer2 = ld32(ld32(functor + 4));
        vcall(layer2, V_REMOVE_CHILD, {layer2, ld32(ld32(functor + 12)), 1});
        const uint32_t openRef = ld32(functor + 20);
        if (ld8(ld32(functor + 16))) {
            Frame f;
            const uint32_t then = newFunction(f, VT_Tutorial, {ld32(openRef), ld32(openRef + 4)});
            const uint32_t out = f.alloc(20);
            st32(out + 16, 0);
            call(g::SceneUtil_openTutorialPopup, {out, 17, then, 0, 0, 0, 0});
            functionDestroy(out);
            functionDestroy(then);
            return;
        }
        if (ld8(openRef)) openIncentive(ld32(openRef + 4));
    }

    // ------------------------------------------------------------------------------------------------ SceneSphereBoard
    // the Unlock button {scene}: with the button lit, the selected sphere's open LWF and the no-tap screen for its
    // length, then the request; or why it cannot open
    static void unlockTouched(uint32_t functor, uint32_t /*sender*/, uint32_t typeRef) {
        if (ld32(typeRef) != 2) return;
        const uint32_t scene = ld32(functor + 4);
        const uint32_t ui = ld32(scene + SB_UI);
        if (call(g::Widget_isBright, {ld32(ui + UI_RELEASE_BUTTON)}) != 1) return;
        int32_t text = 0;
        switch (call(g::is_open_incentive, {ld32(scene + SB_UI), 1})) {
        case 0: {
            const uint32_t button = ld32(ld32(scene + SB_UI) + UI_RELEASE_BUTTON);
            vcall(button, V_SET_TOUCH_ENABLED, {button, 0});
            const uint32_t lwf = call(g::AudioLWFNode_create, {S_SpIconOpenLwf, 0});
            const uint32_t block = getSelectIncentive(ld32(scene + SB_UI));
            vcall(block, node::addChild, {block, lwf});
            const uint32_t movie = ld32(ld32(lwf + 0x3E0) + 24);
            call(g::LWF_Movie_GotoAndPlay, {movie, ld32(movie + 152)});
            const float frames = float(int32_t(ld32(ld32(ld32(lwf + 0x3E0) + 24) + 152))) / 60.0f;
            const float seconds = frames >= 0.1f ? float(int32_t(ld32(ld32(ld32(lwf + 0x3E0) + 24) + 152))) / 60.0f : 0.1f;
            const uint32_t delay = call(g::DelayTime_create, {fbits(seconds)});
            Frame f;
            const uint32_t then = newFunction(f, VT_UnlockDone, {scene});
            const uint32_t cf = call(g::CallFunc_create, {then});
            call(fn::Node_runAction, {lwf, call(g::Sequence_create, {delay, cf, 0})});
            functionDestroy(then);
            playSE(SE_UNLOCK);
            call(g::SM_openNoTapScreen, {sceneManager(), fbits(seconds)});
            return;
        }
        case 1: text = 103400021; break;
        case 2:
        case 4: text = 103400022; break;
        case 3:
            switch (ld32(ld32(scene + SB_UI) + UI_MODE)) {
            case 3: text = 103400031; break;
            case 4: text = 103400032; break;
            case 5: text = 103400033; break;
            default: text = 103400020; break;
            }
            break;
        default: return;
        }
        Frame f;
        const uint32_t td = textData(f, text);
        const uint32_t s = newStr(f, textDataStr(td));
        const uint32_t none = newEmptyFunction(f);
        call(g::SceneUtil_openOkDialog4Line, {s, none, 0});
        functionDestroy(none);
        strFree(s);
        sharedRelease(td);
    }
    // its LWF's end {scene}: the button back on, the request
    static void unlockDone(uint32_t functor) {
        const uint32_t scene = ld32(functor + 4);
        const uint32_t button = ld32(ld32(scene + SB_UI) + UI_RELEASE_BUTTON);
        vcall(button, V_SET_TOUCH_ENABLED, {button, 1});
        sphereUpdateIncentive(ld32(scene + SB_UI));
    }
};

RW_FUNCTION(0x7933D8, "SphereUI::DecPoint", SphereIncentive::decPoint);
RW_FUNCTION(0x795750, "SphereUI::select_incentive_sub", SphereIncentive::selectIncentiveSub);
RW_FUNCTION(0x795934, "SphereUI::get_select_incentive", SphereIncentive::getSelectIncentive);
RW_FUNCTION(0x795668, "SphereUI::open_incentive", SphereIncentive::openIncentive);
RW_FUNCTION(0x7957C8, "SphereUI::sphereUpdateIncentive", SphereIncentive::sphereUpdateIncentive);
RW_FUNCTION(0x7961C8, "SphereUI::get_Incentive_Image", SphereIncentive::getIncentiveImage);
RW_FUNCTION(0x797FB8, "SphereUI::openPopUpIncentiveGet", SphereIncentive::openPopUpIncentiveGet);
RW_FUNCTION(0x7967AC, "SphereUI::openPopUpcommon", SphereIncentive::openPopUpCommon);
RW_FUNCTION(0x799308, "SphereUI::openPopUpcommon::$_7 operator()", SphereIncentive::popupOpened);
RW_FUNCTION(0x7992D8, "SphereUI::openPopUpcommon::$_7 __clone(__base*)", SphereIncentive::popupOpenedCloneInto);
RW_FUNCTION(0x7994F8, "SphereUI::openPopUpcommon::$_1 __clone()", SphereIncentive::okClone);
RW_FUNCTION(0x799568, "SphereUI::openPopUpcommon::$_1 operator()", SphereIncentive::okTouched);
RW_FUNCTION(0x79969C, "SphereUI::openPopUpcommon::$_1::lambda() __clone()", SphereIncentive::closedClone);
RW_FUNCTION(0x799704, "SphereUI::openPopUpcommon::$_1::lambda() operator()", SphereIncentive::closed);
RW_FUNCTION(0x6C0518, "SceneSphereBoard Unlock button lambda operator() (sub_6C0518)", SphereIncentive::unlockTouched);
RW_FUNCTION(0x6C0CFC, "SceneSphereBoard Unlock LWF end lambda operator() (sub_6C0CFC)", SphereIncentive::unlockDone);
