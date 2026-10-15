// SceneBattleResult::openLevelUpPopup: the level up dialog of the result screen (BattleResult_Recovery_V2): the old
// and new Lux rank, the recovered coins, the partner avatar and a "tap" animation.
//
// The original frame (0xF10 bytes) is reproduced with the same offsets: the avatar coordinate copy (+0x2B8) stays
// as it was on the stack when no avatar row matches, and every temporary std::string lives where it did.
#include "SceneBattleResult.hpp"

#include "B2sCommon.hpp"

using namespace b2s;

namespace {
constexpr Addr ArmatureDataManager_getInstance = 0x7AC9B5;
constexpr Addr ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;
constexpr Addr SceneUtil_createShadowSprite = 0x45AD69;
constexpr Addr SceneUtil_createNodeWithSceneFile = 0x45AA49;
constexpr Addr SceneUtil_getSideMargin = 0x45AD0D;
constexpr Addr SceneUtil_getSideHeightMargin = 0x45AF59;
constexpr Addr SceneUtil_setShadow = 0x45BD15;
constexpr Addr SceneUtil_settingTitleLabel = 0x45D8D1;
constexpr Addr SceneUtil_setAdditionalKerningForAtlasLabel = 0x466BA9;
constexpr Addr SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr Addr GUIReader_getInstance = 0x7BF1E5;
constexpr Addr GUIReader_widgetFromJsonFile = 0x7BF9FD;
constexpr Addr Vec2_ctorXY = 0x8E4B9D;               // cocos2d::Vec2::Vec2(float, float)
constexpr Addr Vec2_ctorCopy = 0x8E4C03;
constexpr Addr Vec2_dtor = 0x8E4C17;
constexpr Addr Vec2_add = 0x8E4D2D;                  // Vec2 += Vec2
constexpr Addr Armature_create = 0x79F3C7;
constexpr Addr StringBuffer_ctor = 0x49140D;
constexpr Addr TextAtlas_setStringBlend = 0x83AF1F;
constexpr Addr Text_setString = 0x83A2DD;
constexpr Addr TextData_get = 0x4928AD;
constexpr Addr Player_create = 0x7871A9;             // master::Player::create(shared_ptr out, int level)
constexpr Addr HashTable_find_u64 = 0x490275;
constexpr Addr AvatarUtil_makeStyle = 0x497105;
constexpr Addr Avatar_create = 0x5C5883;
constexpr Addr Avatar_changeMotion = 0x5C63C5;
constexpr Addr SafeBuffer_dtor = 0x45A13D;
constexpr Addr ImageButton_create = 0x531239;
constexpr Addr ImageButton_addTouchEventListener = 0x5315B1;
constexpr Addr ImageButton_setTargetView = 0x5318C1;
constexpr Addr EaseSineOut_create = 0x860D09;

constexpr Addr GOT_ANCHOR_MIDDLE = 0xFD6054;         // &cocos2d::Vec2::ANCHOR_MIDDLE
constexpr Addr GOT_BLEND_FUNC = 0xFD6918;            // &BlendFunc used for the atlas labels

constexpr Addr VT_touchClose = 0xF6462C;             // lambda (popup, layer, shadow, this)
constexpr Addr VT_openDone = 0xF64600;               // lambda (popup, armature)

// ---- string literals
constexpr Addr STR_TapAnimExport = 0xDFFCF2;         // "cocostudio/publish/Tap_Screen_Animation.ExportJson" (50)
constexpr Addr STR_RecoveryJson = 0xE04706;          // "cocostudio/publish/BattleResult_Recovery_V2.json"
constexpr Addr STR_RecoverySceneJson = 0xE04737;     // "cocostudio/publish/BattleResultRecoveryScene.json" (49)
constexpr Addr STR_TapAnime = 0xDFFAC3;              // "Tap_Screen_Anime" (16)
constexpr Addr STR_TapAnimation = 0xDFFB2B;          // "Tap_Screen_Animation" (20)
constexpr Addr STR_AtlasNum1 = 0xDFFC9C;             // "AtlasLabel_Num1" (15)
constexpr Addr STR_AtlasNum2 = 0xDFFCAC;             // "AtlasLabel_Num2" (15)
constexpr Addr STR_PercentD = 0xDF86DE;              // "%d"
constexpr Addr STR_TxtCoinLabel = 0xE04769;          // "Txt_Coin_Label" (14)
constexpr Addr STR_TxtRecoveryLabel = 0xE04778;      // "Txt_Recovery_Label" (18)
constexpr Addr STR_TxtRecoveryLabelCenter = 0xE0478B;   // "Txt_Recovery_Label_Center" (25)
constexpr Addr STR_Avatar = 0xDF8F56;                // "Avatar" (6)
constexpr Addr STR_avatarNode = 0xDFE76A;            // "avatarNode" (10)
constexpr Addr STR_Empty = 0xDF8A1A;
constexpr Addr STR_CenterButton = 0xDFFB40;          // "CenterButton" (12)

constexpr uint32_t TEXT_COIN = 0x6252E86;            // TextData ids
constexpr uint32_t TEXT_RECOVERY = 0x6252E91;
}  // namespace

void SceneBattleResult::openLevelUpPopup(SceneBattleResult *self) {
    guest::Frame F(0xF10);
    const Addr me = rw::guest_addr(self);
    auto fa = [&](uint32_t off) { return F.addr(off); };
    auto readSb = [&](Addr sb, Addr out, uint32_t offset, uint32_t n) {
        cx(SAFEBUFFER_READ, {sb, out, ld32(sb + 0xC) + offset, n, offset});
    };
    auto sm = [&]() { return cx(SCENEMANAGER_GETINSTANCE, {}); };

    // ---- the tap animation resource
    {
        const Addr adm = cx(ArmatureDataManager_getInstance, {});
        strInit(fa(0x17C), STR_TapAnimExport, 0x32);
        cx(ArmatureDataManager_addArmatureFileInfo, {adm, fa(0x17C)});
        strFree(fa(0x17C));
    }

    // ---- popup layer, modal shadow, popup layout
    const Addr layer = cx(SCENEMANAGER_GETPOPUPLAYER, {sm()});
    const Addr shadow = cx(SceneUtil_createShadowSprite, {});
    vcx(shadow, slot::setOpacity, {shadow, 0});
    vcx(layer, slot::addChild, {layer, shadow});
    const Addr reader = cx(GUIReader_getInstance, {});
    const Addr popup = cx(GUIReader_widgetFromJsonFile, {reader, STR_RecoveryJson});
    strInit(fa(0x170), STR_RecoverySceneJson, 0x31);
    const Addr sceneNode = cx(SceneUtil_createNodeWithSceneFile, {fa(0x170)});
    strFree(fa(0x170));
    vcx(popup, slot::setAnchorPoint, {popup, ld32(GOT_ANCHOR_MIDDLE)});

    // ---- centre the popup: position + margin + half its size
    const Addr setPosition = ld32(ld32(popup) + slot::setPosition);
    const uint32_t posX = vcx(popup, slot::getPositionX, {popup});
    const Addr sizeA = vcx(popup, slot::getContentSize, {popup});
    const uint32_t width = ld32(sizeA);
    const uint32_t sideWidth = cx(SceneUtil_getSideMargin, {});
    const uint32_t posY = vcx(popup, slot::getPositionY, {popup});
    const Addr sizeB = vcx(popup, slot::getContentSize, {popup});
    {
        const uint32_t halfW = cx(FMUL, {width, 0x3F000000});
        const uint32_t sumX = cx(FADD, {posX, halfW});
        const uint32_t x = cx(FADD, {sideWidth, sumX});
        const uint32_t halfH = cx(FMUL, {ld32(sizeB + 4), 0x3F000000});
        const uint32_t sumY = cx(FADD, {posY, halfH});
        const uint32_t sideHeight = cx(SceneUtil_getSideHeightMargin, {});
        const uint32_t y = cx(FADD, {sideHeight, sumY});
        cx(Vec2_ctorXY, {fa(0x168), x, y});
        cx(setPosition, {popup, fa(0x168)});
        cx(Vec2_dtor, {fa(0x168)});
    }
    vcx(layer, slot::addChild, {layer, popup});

    // ---- the tap animation: placed where the scene's Tap_Screen_Anime node is, hidden at first
    {
        strInit(fa(0x154), STR_TapAnime, 0x10);
        const Addr node = cx(SEARCH_NODE_BY_NAME, {sceneNode, fa(0x154)});
        const Addr position = vcx(node, slot::getPosition, {node});
        cx(Vec2_ctorCopy, {fa(0x160), position});
        strFree(fa(0x154));
    }
    Addr armature;
    {
        strInit(fa(0x148), STR_TapAnimation, 0x14);
        armature = cx(Armature_create, {fa(0x148)});
        strFree(fa(0x148));
    }
    {
        const Addr addChildZ = ld32(ld32(popup) + slot::addChildZ);
        const uint32_t z = vcx(popup, 0x1C, {popup});
        cx(addChildZ, {popup, armature, z + 1});
    }
    {
        const Addr armatureSetPosition = ld32(ld32(armature) + slot::setPosition);
        cx(armatureSetPosition, {armature, fa(0x160)});
        vcx(armature, slot::setVisible, {armature, 0});
    }

    // ---- the two rank numbers
    const Addr api = cx(APIMANAGER_GETINSTANCE, {});
    strInit(fa(0x13C), STR_AtlasNum1, 0xF);
    const Addr atlas1 = cx(SEARCH_NODE_BY_NAME, {popup, fa(0x13C)});
    strFree(fa(0x13C));
    cx(SceneUtil_setAdditionalKerningForAtlasLabel, {atlas1, uint32_t(-14)});
    strInit(fa(0x130), STR_AtlasNum2, 0xF);
    const Addr atlas2 = cx(SEARCH_NODE_BY_NAME, {popup, fa(0x130)});
    strFree(fa(0x130));
    cx(SceneUtil_setAdditionalKerningForAtlasLabel, {atlas2, uint32_t(-14)});

    readSb(ld32(me + 0x294), fa(0x188), 8, 4);
    {
        cx(StringBuffer_ctor, {fa(0xAF0), STR_PercentD, ld32(fa(0x188))});
        const uint32_t len = cx(STRLEN, {fa(0xAF0)});
        strInit(fa(0x124), fa(0xAF0), len);
        cx(TextAtlas_setStringBlend, {atlas1, fa(0x124), ld32(GOT_BLEND_FUNC)});
        strFree(fa(0x124));
    }
    readSb(ld32(api + 0x94), fa(0x18C), 8, 4);
    {
        cx(StringBuffer_ctor, {fa(0x6EC), STR_PercentD, ld32(fa(0x18C))});
        const uint32_t len = cx(STRLEN, {fa(0x6EC)});
        strInit(fa(0x118), fa(0x6EC), len);
        cx(TextAtlas_setStringBlend, {atlas2, fa(0x118), ld32(GOT_BLEND_FUNC)});
        strFree(fa(0x118));
    }
    readSb(ld32(me + 0x294), fa(0x190), 8, 4);

    // ---- recovered coins: the sum over the ranks passed
    uint32_t level = ld32(fa(0x190));
    uint32_t coins = 0;
    for (;;) {
        readSb(ld32(api + 0x94), fa(0x194), 8, 4);
        if (int32_t(level) >= int32_t(ld32(fa(0x194)))) break;
        cx(Player_create, {fa(0x110), level + 1});
        readSb(ld32(fa(0x110)), fa(0x198), 8, 4);
        coins += ld32(fa(0x198));
        const Addr ctrl = ld32(fa(0x114));
        if (ctrl) cx(SHARED_RELEASE, {ctrl});
        level = level + 1;
    }

    strInit(fa(0x104), STR_TxtCoinLabel, 0xE);
    const Addr coinLabel = cx(SEARCH_NODE_BY_NAME, {popup, fa(0x104)});
    strFree(fa(0x104));
    if (coins != 0) {
        cx(TextData_get, {fa(0xF0), 0, TEXT_COIN});
        cx(StringBuffer_ctor, {fa(0x2E8), ld32(ld32(fa(0xF0)) + 4), coins});
        const uint32_t len = cx(STRLEN, {fa(0x2E8)});
        strInit(fa(0xF8), fa(0x2E8), len);
        cx(Text_setString, {coinLabel, fa(0xF8)});
        strFree(fa(0xF8));
        const Addr ctrl = ld32(fa(0xF4));
        if (ctrl) cx(SHARED_RELEASE, {ctrl});
        cx(SceneUtil_setShadow, {coinLabel});
    } else {
        vcx(coinLabel, slot::setVisible, {coinLabel, 0});
    }

    // ---- recovery labels (shown when coins were recovered), the centred one otherwise
    strInit(fa(0xE4), STR_TxtRecoveryLabel, 0x12);
    cx(SceneUtil_settingTitleLabel, {popup, fa(0xE4), TEXT_RECOVERY});
    strFree(fa(0xE4));
    strInit(fa(0xD8), STR_TxtRecoveryLabelCenter, 0x19);
    cx(SceneUtil_settingTitleLabel, {popup, fa(0xD8), TEXT_RECOVERY});
    strFree(fa(0xD8));
    {
        strInit(fa(0xCC), STR_TxtRecoveryLabel, 0x12);
        const Addr label = cx(SEARCH_NODE_BY_NAME, {popup, fa(0xCC)});
        vcx(label, slot::setVisible, {label, coins != 0 ? 1u : 0u});
        strFree(fa(0xCC));
    }
    {
        strInit(fa(0xC0), STR_TxtRecoveryLabelCenter, 0x19);
        const Addr label = cx(SEARCH_NODE_BY_NAME, {popup, fa(0xC0)});
        vcx(label, slot::setVisible, {label, coins == 0 ? 1u : 0u});
        strFree(fa(0xC0));
    }

    // ---- the avatar: the coordinate whose first word matches the user's value (the last match wins; without a
    // match the 48 bytes at +0x2B8 are whatever the stack held)
    readSb(ld32(api + 0x94), fa(0x19C), 0x3C, 4);
    const uint32_t wanted = ld32(fa(0x19C));
    {
        const Addr api2 = cx(APIMANAGER_GETINSTANCE, {});
        Addr rows = ld32(api2 + 0xC4);
        if (ld32(rows + 0x20) != 0) {
            uint32_t offset = 0;
            for (uint32_t index = 0;;) {
                cx(SAFEBUFFER_READ, {rows, fa(0x288), ld32(rows + 0xC) + offset, 0x30, offset});
                if (ld32(fa(0x288)) == wanted) {
                    for (uint32_t k = 0; k < 12; k++) st32(fa(0x2B8) + 4 * k, ld32(fa(0x288) + 4 * k));
                }
                offset += 0x30;
                index++;
                rows = ld32(api2 + 0xC4);
                if (!(index < ld32(rows + 0x20))) break;
            }
        }
    }
    uint32_t styleId;
    {
        const Addr api3 = cx(APIMANAGER_GETINSTANCE, {});
        const Addr users = ld32(api3 + 0xEC);
        const Addr mgr = sm();
        st32(fa(0x1A0), ld32(mgr + 0x2B0));            // the user id (64 bit) to look up
        st32(fa(0x1A4), ld32(mgr + 0x2B4));
        const Addr found = cx(HashTable_find_u64, {users + 0x24, fa(0x1A0)});
        bool ok = false;
        uint32_t row = 0;
        if (found != 0) {
            row = ld32(found + 0x10);
            ok = row != 0xFFFFFFFFu;
        }
        if (ok) {
            const Addr api4 = cx(APIMANAGER_GETINSTANCE, {});
            const Addr table = ld32(api4 + 0xEC);
            const uint32_t offset = row * 0x58;
            cx(SAFEBUFFER_READ, {table, fa(0x230), ld32(table + 0xC) + offset, 0x58, offset});
            styleId = ld32(fa(0x23C));
        } else {
            const Addr api4 = cx(APIMANAGER_GETINSTANCE, {});
            const Addr table = ld32(api4 + 0xEC);
            cx(SAFEBUFFER_READ, {table, fa(0x1D8), ld32(table + 0xC), 0x58, 0});
            styleId = ld32(fa(0x1E4));
        }
    }
    strInit(fa(0xB4), STR_Avatar, 6);
    const Addr avatarSlot = cx(SEARCH_NODE_BY_NAME, {popup, fa(0xB4)});
    strFree(fa(0xB4));
    vcx(avatarSlot, slot::setVisible, {avatarSlot, 0});
    cx(AvatarUtil_makeStyle, {fa(0x94), fa(0x2B8), styleId});
    strInit(fa(0x88), STR_avatarNode, 0xA);
    const Addr oldAvatar = cx(SEARCH_NODE_BY_NAME, {popup, fa(0x88)});
    strFree(fa(0x88));
    if (oldAvatar) vcx(popup, slot::removeChild, {popup, oldAvatar, 1});
    const Addr avatar = cx(Avatar_create, {fa(0x94), 0});
    cx(Avatar_changeMotion, {avatar, 0x28});
    vcx(avatar, 0x48, {avatar, 0x40200000, 0x40200000});          // setScale(2.5, 2.5)
    {
        const Addr avatarSetPosition = ld32(ld32(avatar) + slot::setPosition);
        const Addr slotPosition = vcx(avatarSlot, slot::getPosition, {avatarSlot});
        cx(Vec2_ctorXY, {fa(0x78), 0x41F00000, 0xC3340000});      // (30, -180)
        cx(Vec2_ctorCopy, {fa(0x80), slotPosition});
        cx(Vec2_add, {fa(0x80), fa(0x78)});
        cx(avatarSetPosition, {avatar, fa(0x80)});
        cx(Vec2_dtor, {fa(0x80)});
        cx(Vec2_dtor, {fa(0x78)});
    }
    {
        const Addr addChildZName = ld32(ld32(popup) + slot::addChildZName);
        const uint32_t z = vcx(avatarSlot, slot::getLocalZOrder, {avatarSlot});
        strInit(fa(0x6C), STR_avatarNode, 0xA);
        cx(addChildZName, {popup, avatar, z + 0xA, fa(0x6C)});
        strFree(fa(0x6C));
    }

    // ---- the invisible centre button closes the popup
    Addr button;
    {
        strInit(fa(0x60), STR_Empty, 0);
        button = cx(ImageButton_create, {fa(0x60), 0});
        strFree(fa(0x60));
    }
    cx(ImageButton_setTargetView, {button, popup, 0x3F800000});
    {
        const Addr addChildZName = ld32(ld32(popup) + slot::addChildZName);
        strInit(fa(0x54), STR_CenterButton, 0xC);
        cx(addChildZName, {popup, button, 0, fa(0x54)});
        strFree(fa(0x54));
    }
    {
        const Addr onTouch = fa(0x1C0);                // std::function<void(Ref*, TouchEventType)>, on the heap
        st32(fa(0x1D0), 0);
        const Addr heap = cx(OPERATOR_NEW, {0x14});
        if (heap) {
            st32(heap, VT_touchClose);
            st32(heap + 4, popup);
            st32(heap + 8, layer);
            st32(heap + 0xC, shadow);
            st32(heap + 0x10, me);
        }
        st32(fa(0x1D0), heap);
        cx(ImageButton_addTouchEventListener, {button, onTouch});
        fnDestroy(onTouch);
    }
    {
        const Addr onOpened = fa(0x1A8);               // runs after the open animation (popup, armature)
        st32(onOpened + 16, onOpened);
        st32(onOpened, VT_openDone);
        st32(onOpened + 4, popup);
        st32(onOpened + 8, armature);
        const Addr action = cx(SceneUtil_createOpenPopupAnimation, {popup, onOpened});
        const Addr ease = cx(EaseSineOut_create, {action});
        cx(RUN_ACTION, {popup, ease});
        fnDestroy(onOpened);
    }
    cx(SafeBuffer_dtor, {fa(0x94)});                   // the avatar style
    cx(Vec2_dtor, {fa(0x160)});
}

RW_FUNCTION(0x681E2C, "SceneBattleResult::openLevelUpPopup()", SceneBattleResult::openLevelUpPopup);
