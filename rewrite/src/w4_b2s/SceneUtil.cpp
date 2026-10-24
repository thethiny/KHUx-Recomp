// SceneUtil: changePartnerPanel, setItemImageData, openNewLuxPopup, updateDeckView, changePartnerAvatarPanel and
// updateMixCircle (the partner / deck panels of the deck edit and quest start screens).
//
// These functions keep big frames with SupportUser rows, medal / keyblade structs passed by value and lambdas that
// capture the address of a local; every frame is reproduced with the original offsets (guest::Frame) and every
// call keeps the callee frames in place (call_exact).
#include "B2sCommon.hpp"

using namespace b2s;

namespace {
constexpr Addr SceneUtil_setShadow = 0x45BD15;
constexpr Addr SceneUtil_settingTitleLabel = 0x45D8D1;
constexpr Addr StringBuffer_ctor = 0x49140D;
constexpr Addr TextData_get = 0x4928AD;
constexpr Addr BattleUtil_calcMedalStatus = 0x4966BD;
constexpr Addr MedalDataManager_getInstance = 0x76AD69;
constexpr Addr MedalDataManager_getMedalData = 0x76ADB5;
constexpr Addr KeyBladeDataManager_getInstance = 0x76B60D;
constexpr Addr KeyBladeDataManager_getKeyBladeData = 0x76B659;   // (sret, mgr, u64, unsigned, UserKeyblade::Keyblade)
constexpr Addr ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE;
constexpr Addr Text_setString = rwlib::cocos2d::ui::Text::setString;
}  // namespace

// ---------------------------------------------------------------------------------------------------------------
// SceneUtil::changePartnerPanel(Widget *panel, int row, int extra): fill the partner panel with the support user
// at `row` of the support list (name, kizuna rank, total attack from the medal and keyblade data).
void SceneUtil_changePartnerPanel(uint32_t panel, int32_t row, uint32_t extra) {
    guest::Frame F(0x6F8);
    auto fa = [&](uint32_t off) { return F.addr(off); };
    const Addr U = fa(0x5B0);                       // SupportUser row (0x128 bytes)
    const Addr kizuna = fa(0x5EC);                  // U + 0x3C: kizuna level

    // ---- the row from the support list
    const Addr api = cx(APIMANAGER_GETINSTANCE, {});
    const Addr list = ld32(api + 0x134);
    const Addr listCtrl = ld32(api + 0x138);
    sharedAddRef(listCtrl);
    {
        const uint32_t offset = uint32_t(row) * 0x128;
        cx(SAFEBUFFER_READ, {list, U, ld32(list + 0xC) + offset, 0x128, offset});
    }
    // the medal of the user (MedalDataManager::getMedalData(id 0, 0, UserMedal::Medal = U + 0x48))
    const Addr medalMgr = cx(MedalDataManager_getInstance, {});
    {
        auto w = [&](uint32_t k) { return ld32(U + 0x48 + 4 * k); };
        cx(MedalDataManager_getMedalData,
           {fa(0x130), medalMgr, 0, 0, 0, 0, w(0), w(1), w(2), w(3), w(4), w(5), w(6), w(7), w(8), w(9), w(10), w(11),
            w(12), w(13), w(14), w(15), w(16), w(17)});
    }

    // ---- name
    const Addr supportPanel = findNode(panel, fa(0x124), 0xDF8F8B, 0xD);       // "Support_Panel"
    const Addr partnerLabel = findNode(supportPanel, fa(0x118), 0xDF8F99, 0x11);   // "Txt_Partner_Label"
    cx(SceneUtil_setShadow, {partnerLabel});
    {
        const Addr nameChars = U + 0xC;                                         // the name: a char array in the row
        strInit(fa(0x10C), nameChars, cx(STRLEN, {nameChars}));
        cx(Text_setString, {partnerLabel, fa(0x10C)});
        strFree(fa(0x10C));
    }

    // ---- kizuna: the rank icon (kizuna >= 1) and the "friend" label
    const Addr kizunaIcon = findNode(supportPanel, fa(0x100), 0xDF8FAB, 6);      // "Kizuna"
    if (kizunaIcon != 0) {
        const Addr setVisible = ld32(ld32(kizunaIcon) + slot::setVisible);
        if (int32_t(ld32(kizuna)) >= 1) {
            cx(setVisible, {kizunaIcon, 1});
            if (ld32(kizuna) == 5) {
                strInit(fa(0xF4), 0xDF8FB2, 0x1E);                            // "img/ui/PartnerSel_Kizuna05.png"
                cx(ImageView_loadTexture, {kizunaIcon, fa(0xF4), 0});
                strFree(fa(0xF4));
            } else {
                strInit(fa(0xE8), 0xDF8FD1, 0x1E);                            // "img/ui/PartnerSel_Kizuna10.png"
                cx(ImageView_loadTexture, {kizunaIcon, fa(0xE8), 0});
                strFree(fa(0xE8));
            }
        } else {
            cx(setVisible, {kizunaIcon, 0});
        }
    }
    const Addr friendLabel = findNode(supportPanel, fa(0xDC), 0xDF8FF0, 0x10);     // "Txt_Friend_Label"
    if (friendLabel != 0) {
        const Addr setVisible = ld32(ld32(friendLabel) + slot::setVisible);
        if (int32_t(ld32(kizuna)) >= 1) {
            cx(setVisible, {friendLabel, 1});
            cx(SceneUtil_setShadow, {friendLabel});
            cx(TextData_get, {fa(0xC8), 0, 0x623A7C2});
            const Addr format = ld32(ld32(fa(0xC8)) + 4);
            cx(StringBuffer_ctor, {fa(0x1AC), format, ld32(kizuna)});
            strInit(fa(0xD0), fa(0x1AC), cx(STRLEN, {fa(0x1AC)}));
            cx(Text_setString, {friendLabel, fa(0xD0)});
            strFree(fa(0xD0));
            sharedRelease(ld32(fa(0xCC)));
        } else {
            cx(setVisible, {friendLabel, 0});
        }
    }

    // ---- total attack of the medal on the keyblade
    const Addr totalAtk = findNode(supportPanel, fa(0xB8), 0xDF9001, 0xD);         // "Base_TotalATK"
    st32(fa(0xC4), totalAtk);
    strInit(fa(0xAC), 0xDF900F, 0xC);                                              // "Txt_TotalATK"
    cx(SceneUtil_settingTitleLabel, {totalAtk, fa(0xAC), 0x6222144});
    strFree(fa(0xAC));
    const Addr keybladeMgr = cx(KeyBladeDataManager_getInstance, {});
    // UserKeyblade::Keyblade by value: a zeroed struct with two unwritten gaps (+0x14, +0x4D..0x4F, +0x54)
    st32(fa(0x1A0), 0);
    cx(AEABI_MEMSET, {fa(0x150), 0x14, 0});
    cx(AEABI_MEMSET, {fa(0x150) + 0x18, 0x35, 0});
    {
        auto w = [&](uint32_t k) { return ld32(fa(0x150) + 4 * k); };
        cx(KeyBladeDataManager_getKeyBladeData,
           {fa(0xA4), keybladeMgr, 0, 0, extra, 0, w(0), w(1), w(2), w(3), w(4), w(5), w(6), w(7), w(8), w(9), w(10),
            w(11), w(12), w(13), w(14), w(15), w(16), w(17), w(18), w(19), w(20), w(21)});
    }
    // the status calculation: a lambda (capturing the address of the label slot) receives the numbers
    {
        const Addr fn = fa(0x138);
        st32(fn + 0x10, fn);
        st32(fn, 0xF54A04);
        st32(fn + 4, fa(0xC4));
        const Addr keybladeData = ld32(fa(0xA4));                                  // UserKeyBladeData object
        st32(fa(0x9C), ld32(keybladeData + 0x20));                                 // a shared_ptr copy (obj, ctrl)
        st32(fa(0xA0), ld32(keybladeData + 0x24));
        sharedAddRef(ld32(fa(0xA0)));
        const Addr medalSp = ld32(fa(0x130)) + 0x58;                                // shared_ptr<master::Medal> of the medal data
        cx(BattleUtil_calcMedalStatus, {medalSp, fa(0x5F8), fn, fa(0x9C), 0xFFFFFFFFu});
        sharedRelease(ld32(fa(0xA0)));
        fnDestroy(fn);
    }
    const Addr select = findNode(panel, fa(0x90), 0xDF901C, 6);                    // "Select"
    if (select != 0) vcx(select, slot::setVisible, {select, 0});
    sharedRelease(ld32(fa(0xA8)));                                                 // keyblade data control block
    sharedRelease(ld32(fa(0x134)));                                                // medal data control block
    sharedRelease(listCtrl);
}

RW_FUNCTION(0x46BE74, "SceneUtil::changePartnerPanel(Widget*, int, int)", SceneUtil_changePartnerPanel);

// ---------------------------------------------------------------------------------------------------------------
// SceneUtil::openNewLuxPopup(int rank, std::function<void()> callback): the "new Lux rank" popup of the result
// screen (BattleResult_LB): the sphere reward of rank `rank`, and an Ok button that runs a copy of the callback.
namespace {
constexpr Addr FadeTo_create = rwlib::cocos2d::FadeTo::create;                 // FadeTo::create(float duration, uint8 opacity)
constexpr Addr GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance;
constexpr Addr GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr Addr Director_getInstance = rwlib::cocos2d::Director::getInstance;
constexpr Addr Director_getVisibleSize = rwlib::cocos2d::Director::getVisibleSize;       // (Size* out, Director*)
constexpr Addr Vec2_ctorXY = rwlib::cocos2d::Vec2::ctor_base__ff;
constexpr Addr Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr Addr Sphere_create = 0x78CF21;
constexpr Addr FunctionVoid_copy = 0x4683CD;
constexpr Addr FunctionVoid_move = 0x47F905;
constexpr Addr Widget_addTouchEventListener = rwlib::cocos2d::ui::Widget::addTouchEventListener;
constexpr Addr createShadowSprite = 0x45AD69;
constexpr Addr createOpenPopupAnimation = 0x45BC05;
constexpr Addr EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr Addr audio_Manager_getInstance = 0x5BECFD;
constexpr Addr audio_Manager_load = 0x5BEEF5;
constexpr Addr VectorThrowOutOfRange = 0x47CCE1;
constexpr Addr SceneManager_getPopupLayer = 0x613E69;
constexpr Addr GOT_ANCHOR_MIDDLE = 0xFD6054;
constexpr uint32_t SOUND_NEW_LUX = 0x1E584184;
constexpr Addr VT_closeTouch = 0xF54744;                 // heap lambda (popup, layer, shadow, rank, callback)
constexpr Addr VT_openDone = 0xF54718;
}  // namespace

void SceneUtil_openNewLuxPopup(int32_t rank, uint32_t callback) {
    guest::Frame F(0xE28);
    auto fa = [&](uint32_t off) { return F.addr(off); };
    auto sm = [&]() { return cx(SCENEMANAGER_GETINSTANCE, {}); };

    const Addr layer = cx(SceneManager_getPopupLayer, {sm()});
    // ---- modal shadow, fading in
    const Addr shadow = cx(createShadowSprite, {});
    const uint32_t opacity = vcx(shadow, 0x20C, {shadow});            // getOpacity
    vcx(shadow, slot::setOpacity, {shadow, 0});
    cx(RUN_ACTION, {shadow, cx(FadeTo_create, {0x3DCCCCCD, opacity})});
    vcx(layer, slot::addChild, {layer, shadow});

    // ---- the popup, centred on the visible area
    const Addr reader = cx(GUIReader_getInstance, {});
    const Addr popup = cx(GUIReader_widgetFromJsonFile, {reader, 0xDF94A2});       // "BattleResult_LB_ver120.json"
    vcx(popup, slot::setAnchorPoint, {popup, ld32(GOT_ANCHOR_MIDDLE)});
    {
        const Addr setPosition = ld32(ld32(popup) + slot::setPosition);
        const uint32_t posX = vcx(popup, slot::getPositionX, {popup});
        const Addr sizeA = vcx(popup, slot::getContentSize, {popup});
        const uint32_t width = ld32(sizeA);
        cx(Director_getVisibleSize, {fa(0x108), cx(Director_getInstance, {})});
        const uint32_t visibleWidth = ld32(fa(0x108));
        const uint32_t posY = vcx(popup, slot::getPositionY, {popup});
        const Addr sizeB = vcx(popup, slot::getContentSize, {popup});
        const uint32_t height = ld32(sizeB + 4);
        cx(Director_getVisibleSize, {fa(0x108), cx(Director_getInstance, {})});
        const uint32_t halfW = cx(FMUL, {width, 0x3F000000});
        const uint32_t sumX = cx(FADD, {posX, halfW});
        const uint32_t marginW = cx(FADD, {visibleWidth, 0xC4700000});            // visible width - 960
        const uint32_t halfMarginW = cx(FMUL, {marginW, 0x3F000000});
        const uint32_t x = cx(FADD, {sumX, halfMarginW});
        const uint32_t halfH = cx(FMUL, {height, 0x3F000000});
        const uint32_t sumY = cx(FADD, {posY, halfH});
        const uint32_t marginH = cx(FADD, {ld32(fa(0x10C)), 0xC4200000});         // visible height - 640
        const uint32_t halfMarginH = cx(FMUL, {marginH, 0x3F000000});
        const uint32_t y = cx(FADD, {sumY, halfMarginH});
        cx(Vec2_ctorXY, {fa(0x100), x, y});
        cx(setPosition, {popup, fa(0x100)});
        cx(Vec2_dtor, {fa(0x100)});
    }
    vcx(layer, slot::addChild, {layer, popup});

    // ---- the sphere reward of this rank
    const Addr api = cx(APIMANAGER_GETINSTANCE, {});
    {
        const uint32_t first = ld32(api + 0x308), last = ld32(api + 0x30C);
        const uint32_t count = uint32_t(int32_t(last - first) >> 2);
        if (!(count > uint32_t(rank))) {
            cx(VectorThrowOutOfRange, {api + 0x308});
            return;
        }
        cx(Sphere_create, {fa(0xF8), ld32(first + uint32_t(rank) * 4)});
    }
    const Addr sphere = ld32(fa(0xF8));
    cx(SAFEBUFFER_READ, {sphere, fa(0x110), ld32(sphere + 0xC) + 0x98, 4, 0x98});     // kind of the sphere reward
    {
        // the title text id by kind (2..5), a default for the others
        const uint32_t kind = ld32(fa(0x110));
        uint32_t textId = 0x629C27C;
        Addr tmp = fa(0xBC);
        if (kind - 2 <= 3) {
            static const uint32_t ids[4] = {0x629C27C, 0x629C27E, 0x629C27F, 0x629C280};
            static const uint32_t tmps[4] = {0xEC, 0xE0, 0xD4, 0xC8};
            textId = ids[kind - 2];
            tmp = fa(tmps[kind - 2]);
        }
        strInit(tmp, 0xDF94D1, 3);                                                       // "Txt"
        cx(SceneUtil_settingTitleLabel, {popup, tmp, textId});
        strFree(tmp);
    }
    cx(SAFEBUFFER_READ, {sphere, fa(0x114), ld32(sphere + 0xC) + 0, 4, 0});
    const uint32_t valueA = ld32(fa(0x114));
    cx(SAFEBUFFER_READ, {sphere, fa(0x118), ld32(sphere + 0xC) + 0x8C, 4, 0x8C});
    const uint32_t valueB = ld32(fa(0x118));
    cx(SAFEBUFFER_READ, {sphere, fa(0x11C), ld32(sphere + 0xC) + 0x88, 4, 0x88});
    const uint32_t valueC = ld32(fa(0x11C));
    const Addr selectBase = findNode(popup, fa(0xB0), 0xDF94D5, 0xB);                    // "Select_Base"
    const Addr selectBoard = findNode(popup, fa(0xA4), 0xDF94E1, 0xC);                   // "Select_Board"
    const Addr selectKind = findNode(popup, fa(0x98), 0xDF94EE, 0xB);                    // "Select_Kind"
    cx(StringBuffer_ctor, {fa(0xA08), 0xDF94FA, valueA});                                // "img/sphere_incentive/%d.png"
    strInit(fa(0x8C), fa(0xA08), cx(STRLEN, {fa(0xA08)}));
    cx(ImageView_loadTexture, {selectBase, fa(0x8C), 0});
    strFree(fa(0x8C));
    cx(StringBuffer_ctor, {fa(0x604), 0xDF9516, valueB});                                // "img/sphere_board/Board_%04d.png"
    strInit(fa(0x80), fa(0x604), cx(STRLEN, {fa(0x604)}));
    cx(ImageView_loadTexture, {selectBoard, fa(0x80), 0});
    strFree(fa(0x80));
    if (valueC != 0) {
        cx(StringBuffer_ctor, {fa(0x200), 0xDF9536, valueC});                            // "img/sphere_kind/Kind_%04d.png"
        strInit(fa(0x74), fa(0x200), cx(STRLEN, {fa(0x200)}));
        cx(ImageView_loadTexture, {selectKind, fa(0x74), 0});
        strFree(fa(0x74));
        vcx(selectKind, slot::setVisible, {selectKind, 1});
    } else {
        vcx(selectKind, slot::setVisible, {selectKind, 0});
    }
    const Addr titleLabel = findNode(popup, fa(0x68), 0xDF9554, 0xF);                    // "Txt_Title_Label"
    cx(SceneUtil_setShadow, {titleLabel});
    cx(SAFEBUFFER_READ, {sphere, fa(0x17C), ld32(sphere + 0xC) + 4, 0x81, 4});          // name of the sphere
    strInit(fa(0x5C), fa(0x17C), cx(STRLEN, {fa(0x17C)}));
    cx(Text_setString, {titleLabel, fa(0x5C)});
    strFree(fa(0x5C));
    const Addr okButton = findNode(popup, fa(0x50), 0xDF9564, 9);                        // "Button_Ok"
    strInit(fa(0x44), 0xDF869F, 6);                                                      // "Txt_OK"
    cx(SceneUtil_settingTitleLabel, {okButton, fa(0x44), 0x5F767A1});
    strFree(fa(0x44));

    // ---- the Ok button closes the popup and calls the callback (a copy of it lives in the heap lambda)
    {
        st32(fa(0x138), popup);
        st32(fa(0x13C), layer);
        st32(fa(0x140), shadow);
        st32(fa(0x144), uint32_t(rank));
        const Addr cbCopy = fa(0x148);
        cx(FunctionVoid_copy, {cbCopy, callback});
        const Addr touchFn = fa(0x160);
        st32(touchFn + 0x10, 0);
        const Addr lambda = cx(OPERATOR_NEW, {0x30});
        if (lambda) {
            st32(lambda, VT_closeTouch);
            st32(lambda + 8, ld32(fa(0x138)));
            st32(lambda + 0xC, ld32(fa(0x13C)));
            st32(lambda + 0x10, ld32(fa(0x140)));
            st32(lambda + 0x14, ld32(fa(0x144)));
            cx(FunctionVoid_move, {lambda + 0x18, cbCopy});
        }
        st32(touchFn + 0x10, lambda);
        cx(Widget_addTouchEventListener, {okButton, touchFn});
        fnDestroy(touchFn);
        fnDestroy(cbCopy);
    }
    cx(audio_Manager_load, {cx(audio_Manager_getInstance, {}), SOUND_NEW_LUX});
    {
        const Addr fn = fa(0x120);
        st32(fn + 0x10, fn);
        st32(fn, VT_openDone);
        st32(fn + 4, popup);
        const Addr action = cx(createOpenPopupAnimation, {popup, fn});
        cx(RUN_ACTION, {popup, cx(EaseSineOut_create, {action})});
        fnDestroy(fn);
    }
    sharedRelease(ld32(fa(0xFC)));                                                       // control block of the sphere row
}

RW_FUNCTION(0x46FDC4, "SceneUtil::openNewLuxPopup(int, std::function<void()>)", SceneUtil_openNewLuxPopup);

// ---------------------------------------------------------------------------------------------------------------
// SceneUtil::updateDeckView(Widget *panel, UserKeyblade::Keyblade keyblade, std::function<...> onMedalInfo): the
// deck panel of a user (keyblade name, ability, max burst, image, medal revolver).
// `keyblade` is a 0x58 byte struct passed by value: its first two words (the 64 bit keyblade id) arrive in r2 / r3
// (r1 is skipped for the 8-byte alignment), the rest sits on the caller's stack (+0x00 ...), followed by the
// function pointer at +0x50. The frame (0x1380) is reproduced so that the home area of r2 / r3 and the caller's
// words line up like in the original (struct S at frame +0x1378).
namespace {
constexpr Addr DeckRevolverView_create = 0x52C471;
constexpr Addr DeckRevolverView_setKeyBladeData = 0x52CC21;
constexpr Addr DeckRevolverView_setMedalDatas = 0x52D135;
constexpr Addr DeckRevolverView_setOpenMedalInfoPopupEnabled = 0x52D5A5;
constexpr Addr FunctionMedalInfo_assign = 0x46B05D;
constexpr Addr SceneUtil_updateDeckStauts = 0x46A4AD;
constexpr Addr MedalVector_pushBackSlow = 0x487A39;      // vector<shared_ptr<UserMedalData>>::__push_back_slow_path
constexpr Addr MedalVector_dtor = 0x46B119;
constexpr Addr KeyBladeDataManager_getKeyBladeImagePath = 0x76B809;
constexpr Addr aeabi_divsi3 = 0xCD5E8D;
}  // namespace

void SceneUtil_updateDeckView(uint32_t panel, uint32_t /*r1*/, uint32_t r2, uint32_t r3) {
    guest::Frame F(0x1380);
    auto fa = [&](uint32_t off) { return F.addr(off); };
    auto fw = [&](uint32_t off) -> uint32_t & { return *F.at<uint32_t>(off); };
    const Addr S = fa(0x1378);                       // the keyblade struct
    st32(fa(0x1378), r2);
    st32(fa(0x137C), r3);
    const Addr onMedalInfo = ld32(fa(0x13D0));       // std::function<void(MedalInfoButtonType, bool, int)>*
    const Addr V = fa(0x18C);                        // std::vector<shared_ptr<UserMedalData>>
    fw(0x18C) = 0;
    fw(0x190) = 0;
    fw(0x194) = 0;

    // ---- the keyblade's medals (64 bit ids at S + 0x18, count at S + 0x10)
    const uint32_t count = ld32(fa(0x1388));
    if (count != 0) {
        Addr ids = S + 0x18;
        for (uint32_t i = 0; i < count; i++, ids += 8) {
            const Addr mgr = cx(MedalDataManager_getInstance, {});
            const uint32_t lo = ld32(ids), hi = ld32(ids + 4);
            cx(AEABI_MEMSET, {fa(0x1310), 0x2C, 0});                 // UserMedal::Medal by value (a stale word at +0x2C)
            cx(AEABI_MEMSET, {fa(0x1340), 0x18, 0});
            {
                auto w = [&](uint32_t k) { return ld32(fa(0x1310) + 4 * k); };
                cx(MedalDataManager_getMedalData,
                   {fa(0x184), mgr, lo, hi, 0, 0, w(0), w(1), w(2), w(3), w(4), w(5), w(6), w(7), w(8), w(9), w(10),
                    w(11), w(12), w(13), w(14), w(15), w(16), w(17)});
            }
            const uint32_t end = fw(0x190), cap = fw(0x194);
            if (end == cap) {
                cx(MedalVector_pushBackSlow, {V, fa(0x184)});
            } else {
                if (end != 0) {
                    st32(end, fw(0x184));
                    const uint32_t ctrl = fw(0x188);
                    st32(end + 4, ctrl);
                    sharedAddRef(ctrl);
                }
                fw(0x190) = fw(0x190) + 8;
            }
            sharedRelease(fw(0x188));
        }
    }

    // ---- the keyblade data of the user
    const Addr kbMgr = cx(KeyBladeDataManager_getInstance, {});
    {
        // UserKeyblade::Keyblade by value: zeroed, with stale gaps at +0x14, +0x4D..0x4F and +0x54..0x57
        fw(0x1308) = 0;
        cx(AEABI_MEMSET, {fa(0x12B8), 0x14, 0});
        cx(AEABI_MEMSET, {fa(0x12B8) + 0x18, 0x35, 0});
        auto w = [&](uint32_t k) { return ld32(fa(0x12B8) + 4 * k); };
        cx(KeyBladeDataManager_getKeyBladeData,
           {fa(0x17C), kbMgr, ld32(S), ld32(S + 4), 0, 0, w(0), w(1), w(2), w(3), w(4), w(5), w(6), w(7), w(8), w(9),
            w(10), w(11), w(12), w(13), w(14), w(15), w(16), w(17), w(18), w(19), w(20), w(21)});
    }
    // ---- name, ability, max burst
    const Addr nameLabel = findNode(panel, fa(0x170), 0xDF880D, 0x13);                  // "KeyBlade_Name_Label"
    cx(SceneUtil_setShadow, {nameLabel});
    // name = the %s of the keyblade name (a 0x41 byte field of the user keyblade data)
    auto keybladeText = [&](Addr node, uint32_t fieldOffset, uint32_t fieldSize, Addr nameBuf, Addr fmtBuf, Addr tmp) {
        const Addr kb = ld32(ld32(fa(0x17C)) + 0x20);                                  // the data SafeBuffer
        cx(SAFEBUFFER_READ, {kb, nameBuf, ld32(kb + 0xC) + fieldOffset, fieldSize, fieldOffset});
        cx(StringBuffer_ctor, {fmtBuf, 0xDF8821, nameBuf});                            // "%s"
        strInit(tmp, fmtBuf, cx(STRLEN, {fmtBuf}));
        cx(Text_setString, {node, tmp});
        strFree(tmp);
    };
    keybladeText(nameLabel, 4, 0x41, fa(0xE70), fa(0xEB4), fa(0x164));
    const Addr kbNameLabel = findNode(panel, fa(0x158), 0xDF8F04, 0x10);                // "Txt_KBName_Label"
    if (kbNameLabel != 0) {
        cx(SceneUtil_setShadow, {kbNameLabel});
        keybladeText(kbNameLabel, 4, 0x41, fa(0xA28), fa(0xA6C), fa(0x14C));
    }
    const Addr abilityLabel = findNode(panel, fa(0x140), 0xDF8824, 0xD);                // "Ability_Label"
    cx(SceneUtil_setShadow, {abilityLabel});
    keybladeText(abilityLabel, 0x45, 0x81, fa(0x5A0), fa(0x624), fa(0x134));
    const Addr maxBurst = findNode(panel, fa(0x128), 0xDF8F15, 0xC);                    // "Txt_MaxBurst"
    cx(SceneUtil_setShadow, {maxBurst});
    cx(TextData_get, {fa(0x114), 0, 0x612DEE7});
    {
        const Addr text = ld32(ld32(fa(0x114)) + 4);
        strInit(fa(0x11C), text, cx(STRLEN, {text}));
        cx(Text_setString, {maxBurst, fa(0x11C)});
        strFree(fa(0x11C));
        sharedRelease(ld32(fa(0x118)));
    }
    const Addr maxBurstLabel = findNode(panel, fa(0x108), 0xDF8F22, 0x12);              // "Txt_MaxBurst_Label"
    cx(SceneUtil_setShadow, {maxBurstLabel});
    {
        const uint32_t burstGauge = ld32(S + 0x40);                                    // S[16]
        const Addr kb = ld32(ld32(fa(0x17C)) + 0x20);
        cx(SAFEBUFFER_READ, {kb, fa(0x198), ld32(kb + 0xC) + 0xE0, 4, 0xE0});
        const uint32_t level = cx(aeabi_divsi3, {burstGauge, 10000});
        cx(StringBuffer_ctor, {fa(0x19C), 0xDF8F35, level, ld32(fa(0x198))});          // "%d/%d"
        strInit(fa(0xFC), fa(0x19C), cx(STRLEN, {fa(0x19C)}));
        cx(Text_setString, {maxBurstLabel, fa(0xFC)});
        strFree(fa(0xFC));
    }

    // ---- keyblade image
    const Addr dummy = findNode(panel, fa(0xF0), 0xDF8832, 0xE);                        // "KeyBlade_Dummy"
    {
        const Addr imageMgr = cx(KeyBladeDataManager_getInstance, {});
        st32(fa(0xDC), fw(0x17C));                                                    // shared_ptr<UserKeyBladeData> argument
        st32(fa(0xE0), fw(0x180));
        sharedAddRef(fw(0xE0));
        const Addr path = cx(KeyBladeDataManager_getKeyBladeImagePath, {imageMgr, fa(0xDC)});
        strInit(fa(0xE4), path, cx(STRLEN, {path}));
        cx(ImageView_loadTexture, {dummy, fa(0xE4), 0});
        strFree(fa(0xE4));
        sharedRelease(fw(0xE0));
    }
    // ---- deck status and the medal revolver
    st32(fa(0xD4), fw(0x17C));
    const uint32_t keybladeCtrl = fw(0x180);
    st32(fa(0xD8), keybladeCtrl);
    sharedAddRef(keybladeCtrl);
    cx(SceneUtil_updateDeckStauts, {panel, fa(0xD4), V});
    sharedRelease(keybladeCtrl);
    const Addr revolver = findNode(panel, fa(0xC8), 0xDF8F3B, 0xC);                     // "revolverView"
    if (revolver != 0) {
        st32(fa(0xC0), fw(0x17C));
        st32(fa(0xC4), fw(0x180));
        sharedAddRef(fw(0xC4));
        cx(DeckRevolverView_setKeyBladeData, {revolver, fa(0xC0)});
        sharedRelease(fw(0xC4));
        cx(DeckRevolverView_setMedalDatas, {revolver, V});
    } else {
        const Addr area = findNode(panel, fa(0xB4), 0xDF8F48, 0xD);                     // "Area_Revolver"
        st32(fa(0xAC), fw(0x17C));
        st32(fa(0xB0), fw(0x180));
        sharedAddRef(fw(0xB0));
        st32(fa(0xA4), 0);                                                              // no UserDetail
        st32(fa(0xA8), 0);
        const Addr view = cx(DeckRevolverView_create, {fa(0xAC), fa(0xA4)});
        sharedRelease(fw(0xA8));
        sharedRelease(fw(0xB0));
        cx(DeckRevolverView_setMedalDatas, {view, V});
        cx(DeckRevolverView_setOpenMedalInfoPopupEnabled, {view, 1});
        st8(view + 0x314, 1);
        cx(FunctionMedalInfo_assign, {view + 0x270, onMedalInfo});
        const Addr setPosition = ld32(ld32(view) + slot::setPosition);
        const Addr sizeA = vcx(view, slot::getContentSize, {view});
        const Addr sizeB = vcx(view, slot::getContentSize, {view});
        const uint32_t halfW = cx(FMUL, {ld32(sizeA), 0x3F000000});
        const uint32_t halfH = cx(FMUL, {ld32(sizeB + 4), 0x3F000000});
        cx(Vec2_ctorXY, {fa(0x9C), halfW, halfH});
        cx(setPosition, {view, fa(0x9C)});
        cx(Vec2_dtor, {fa(0x9C)});
        const Addr addChildZName = ld32(ld32(area) + slot::addChildZName);
        strInit(fa(0x90), 0xDF8F3B, 0xC);                                               // "revolverView"
        cx(addChildZName, {area, view, 0, fa(0x90)});
        strFree(fa(0x90));
    }
    sharedRelease(fw(0x180));
    cx(MedalVector_dtor, {V});
}

RW_FUNCTION(0x46A6E4, "SceneUtil::updateDeckView(Widget*, UserKeyblade::Keyblade, std::function<void(MedalInfoButtonType, bool, int)>)", SceneUtil_updateDeckView);

// ---------------------------------------------------------------------------------------------------------------
// SceneUtil::changePartnerAvatarPanel(Widget *panel, StageSupportList::SupportUser user, AvatarIconCache *cache):
// show the partner's avatar icon (from the icon cache, a temporary cache is made when none is given), the medal
// icon and the skill icon in the partner panel, with touch buttons carrying a copy of the user.
// `user` is a 0x128 byte struct passed by value: its first two words (64 bit id) arrive in r2 / r3, the rest sits on
// the caller's stack, followed by `cache` at +0x120. The frame (0x6F8) is reproduced with the original offsets: the
// struct S lives at frame +0x6F0, and the touch lambdas copy one stale stack word in front of it.
namespace {
constexpr Addr AvatarIconCache_ctor = 0x5CA741;          // (this, Node*, const Vec2&, float, float, vector<Expression>&)
constexpr Addr AvatarIconCache_dtor = 0x5CA8FD;
constexpr Addr AvatarIconCache_find = 0x5CA9A9;          // (this, u64 id)
constexpr Addr AvatarIconCache_add = 0x5CAA57;           // (this, u64 id, const Avatar::Style&)
constexpr Addr ExpressionVector_allocate = 0x487635;
constexpr Addr ExpressionVector_dtor = 0x46BE0D;
constexpr Addr AvatarUtil_makeStyle = 0x497105;
constexpr Addr SafeBuffer_dtor = 0x45A13D;
constexpr Addr ImageButton_create = 0x531239;
constexpr Addr ImageButton_addTouchEventListener = 0x5315B1;
constexpr Addr ImageButton_setTargetView = 0x5318C1;
constexpr Addr MedalDataManager_getMedalSmallImagePath = 0x76B17D;
constexpr Addr SkillDataManager_getInstance = 0x76BA0D;
constexpr Addr SkillDataManager_getData = 0x76BA59;
constexpr Addr SkillDataManager_getImagePath = 0x76BE8D;
constexpr Addr SkillVector_pushBackSlow = 0x487311;
constexpr Addr SkillVector_dtor = 0x46BE37;
constexpr uint32_t REMOVE_FROM_PARENT = 0x124;                   // Node::removeFromParent (vtable offset)
constexpr Addr VT_avatarTouch = 0xF54A5C, VT_medalTouch = 0xF54A30;     // heap lambdas (panel + copy of the user)
constexpr Addr STR_Avatar = 0xDF8F56, STR_AvatarIconObj = 0xDF8F5D, STR_imageButton = 0xDF8F6D, STR_Medal_S = 0xDF8F79,
               STR_IconSkil = 0xDF8F81, STR_Empty = 0xDF8A1A;
}  // namespace

void SceneUtil_changePartnerAvatarPanel(uint32_t panel, uint32_t /*r1*/, uint32_t r2, uint32_t r3) {
    guest::Frame F(0x6F8);
    auto fa = [&](uint32_t off) { return F.addr(off); };
    auto fw = [&](uint32_t off) -> uint32_t & { return *F.at<uint32_t>(off); };
    const Addr S = fa(0x6F0);                         // the SupportUser
    st32(fa(0x6F0), r2);
    st32(fa(0x6F4), r3);

    // ---- the icon cache: the given one, or a temporary one owned by this call
    Addr cache = ld32(fa(0x818));
    const bool given = cache != 0;
    if (!given) {
        const Addr avatarNode0 = findNode(panel, fa(0x1DC), STR_Avatar, 6);
        const Addr parent = vcx(avatarNode0, 0x2D8, {avatarNode0});
        const Addr scene = vcx(parent, 0x280, {parent});
        cache = cx(OPERATOR_NEW, {0x60});
        cx(Vec2_ctorXY, {fa(0x1D4), 0xC2580000, 0x42A00000});             // (-54, 80)
        st32(fa(0x1E8), 1);                                                 // Avatar::Expression 1
        fw(0x1C8) = 0;
        fw(0x1CC) = 0;
        fw(0x1D0) = 0;
        cx(ExpressionVector_allocate, {fa(0x1C8), 1});
        {
            const uint32_t end = fw(0x1CC);
            if (end != 0) st32(end, fw(0x1E8));
            fw(0x1CC) = (end != 0 ? fw(0x1CC) : 0) + 4;
        }
        cx(AvatarIconCache_ctor, {cache, scene, fa(0x1D4), 0xBF68F5C3, 0x3F68F5C3, fa(0x1C8)});
        cx(ExpressionVector_dtor, {fa(0x1C8)});
        cx(Vec2_dtor, {fa(0x1D4)});
    }
    const Addr avatarNode = findNode(panel, fa(0x1BC), STR_Avatar, 6);
    const Addr iconObj = findNode(panel, fa(0x1B0), STR_AvatarIconObj, 0xF);
    Addr icon = cx(AvatarIconCache_find, {cache, 1, ld32(S), ld32(S + 4)});
    // the cached icon for this user (made from the user avatar style when missing)
    auto makeIcon = [&](Addr styleBuf) {
        const uint32_t lo = ld32(S), hi = ld32(S + 4);
        cx(AvatarUtil_makeStyle, {styleBuf, S + 0xE8, 0});
        const Addr added = cx(AvatarIconCache_add, {cache, 0, lo, hi, styleBuf});
        cx(SafeBuffer_dtor, {styleBuf});
        return added;
    };
    if (iconObj != 0) {
        if (icon == 0) icon = makeIcon(fa(0x190));
        if (icon != iconObj) {
            const Addr setPosition = ld32(ld32(icon) + slot::setPosition);
            const Addr position = vcx(iconObj, slot::getPosition, {iconObj});
            cx(setPosition, {icon, position});
            const Addr parent = vcx(iconObj, 0x11C, {iconObj});                // getParent
            vcx(icon, REMOVE_FROM_PARENT, {icon});                           // removeFromParent (+0x124)
            const Addr addChildZName = ld32(ld32(parent) + slot::addChildZName);
            const uint32_t z = vcx(iconObj, slot::getLocalZOrder, {iconObj});
            strInit(fa(0x184), STR_AvatarIconObj, 0xF);
            cx(addChildZName, {parent, icon, z, fa(0x184)});
            strFree(fa(0x184));
            vcx(iconObj, REMOVE_FROM_PARENT, {iconObj});
        }
    } else {
        if (icon == 0) icon = makeIcon(fa(0x164));
        const Addr setPosition = ld32(ld32(icon) + slot::setPosition);
        const Addr position = vcx(avatarNode, slot::getPosition, {avatarNode});
        cx(setPosition, {icon, position});
        vcx(icon, REMOVE_FROM_PARENT, {icon});
        const Addr parent = vcx(avatarNode, 0x11C, {avatarNode});
        const Addr addChildZName = ld32(ld32(parent) + slot::addChildZName);
        const uint32_t z = vcx(avatarNode, slot::getLocalZOrder, {avatarNode});
        strInit(fa(0x158), STR_AvatarIconObj, 0xF);
        cx(addChildZName, {parent, icon, z, fa(0x158)});
        strFree(fa(0x158));
        vcx(avatarNode, slot::setOpacity, {avatarNode, 0});
    }

    // ---- the touch button over the avatar (a lambda holding the panel and a copy of the user)
    Addr button = findNode(avatarNode, fa(0x14C), STR_imageButton, 0xB);
    if (button == 0) {
        strInit(fa(0x140), STR_Empty, 0);
        button = cx(ImageButton_create, {fa(0x140), 1});
        strFree(fa(0x140));
        cx(ImageButton_setTargetView, {button, avatarNode, 0x3F800000});
        const Addr addChildZName = ld32(ld32(avatarNode) + slot::addChildZName);
        strInit(fa(0x134), STR_imageButton, 0xB);
        cx(addChildZName, {avatarNode, button, 0, fa(0x134)});
        strFree(fa(0x134));
    }
    cx(MEMCPY, {fa(0x338), S, 0x128});
    cx(MEMCPY, {fa(0x47C), fa(0x334), 0x12C});                   // starts with one stale word of the frame
    st32(fa(0x470), 0);
    {
        const Addr lambda = cx(OPERATOR_NEW, {0x138});
        if (lambda) {
            st32(lambda, VT_avatarTouch);
            st32(lambda + 8, panel);
            cx(MEMCPY, {lambda + 0xC, fa(0x47C), 0x12C});
        }
        st32(fa(0x470), lambda);
        cx(ImageButton_addTouchEventListener, {button, fa(0x460)});
        fnDestroy(fa(0x460));
    }

    // ---- the medal icon
    const Addr medalNode = findNode(panel, fa(0x128), STR_Medal_S, 7);
    if (medalNode != 0) {
        const Addr mgr = cx(MedalDataManager_getInstance, {});
        {
            auto w = [&](uint32_t k) { return ld32(S + 0x48 + 4 * k); };
            cx(MedalDataManager_getMedalData,
               {fa(0x120), mgr, 0, 0, 0, 0, w(0), w(1), w(2), w(3), w(4), w(5), w(6), w(7), w(8), w(9), w(10), w(11),
                w(12), w(13), w(14), w(15), w(16), w(17)});
        }
        const Addr mgr2 = cx(MedalDataManager_getInstance, {});
        st32(fa(0x10C), fw(0x120));                                   // shared_ptr<UserMedalData> argument
        st32(fa(0x110), fw(0x124));
        sharedAddRef(fw(0x110));
        {
            const Addr path = cx(MedalDataManager_getMedalSmallImagePath, {mgr2, fa(0x10C)});
            strInit(fa(0x114), path, cx(STRLEN, {path}));
            cx(ImageView_loadTexture, {medalNode, fa(0x114), 0});
            strFree(fa(0x114));
        }
        sharedRelease(fw(0x110));
        Addr medalButton = findNode(medalNode, fa(0x100), STR_imageButton, 0xB);
        if (medalButton == 0) {
            strInit(fa(0xF4), STR_Empty, 0);
            medalButton = cx(ImageButton_create, {fa(0xF4), 0});
            strFree(fa(0xF4));
            cx(ImageButton_setTargetView, {medalButton, medalNode, 0x3F800000});
            const Addr addChildZName = ld32(ld32(medalNode) + slot::addChildZName);
            strInit(fa(0xE8), STR_imageButton, 0xB);
            cx(addChildZName, {medalNode, medalButton, 0, fa(0xE8)});
            strFree(fa(0xE8));
        }
        cx(MEMCPY, {fa(0x1F0), S, 0x128});
        cx(MEMCPY, {fa(0x5A8), fa(0x1EC), 0x12C});                   // starts with one stale word of the frame
        st32(fa(0x328), 0);
        const Addr lambda = cx(OPERATOR_NEW, {0x138});
        if (lambda) {
            st32(lambda, VT_medalTouch);
            st32(lambda + 8, panel);
            cx(MEMCPY, {lambda + 0xC, fa(0x5A8), 0x12C});
        }
        st32(fa(0x328), lambda);
        cx(ImageButton_addTouchEventListener, {medalButton, fa(0x318)});
        fnDestroy(fa(0x318));
        sharedRelease(fw(0x124));
    }
    // the temporary cache is destroyed
    if (!given) {
        const Addr r = cx(AvatarIconCache_dtor, {cache});
        cx(OPERATOR_DELETE, {r});
    }

    // ---- the skill icon of the medal (the first skill of the medal that the user has)
    const Addr skillIcon = findNode(panel, fa(0xDC), STR_IconSkil, 9);
    if (skillIcon == 0) return;
    vcx(skillIcon, slot::setVisible, {skillIcon, 0});
    {
        const Addr mgr = cx(MedalDataManager_getInstance, {});
        auto w = [&](uint32_t k) { return ld32(S + 0x48 + 4 * k); };
        cx(MedalDataManager_getMedalData,
           {fa(0xD4), mgr, 0, 0, 0, 0, w(0), w(1), w(2), w(3), w(4), w(5), w(6), w(7), w(8), w(9), w(10), w(11), w(12),
            w(13), w(14), w(15), w(16), w(17)});
    }
    const Addr medalData = fw(0xD4);
    if (medalData != 0 && int32_t(ld32(medalData + 0x48)) >= 1) {
        for (int32_t k = 0;;) {
            const Addr m = fw(0xD4);
            const uint32_t idLo = ld32(m + 0x38 + uint32_t(k) * 8);
            const uint32_t idHi = ld32(m + 0x3C + uint32_t(k) * 8);
            if ((idLo | idHi) != 0) {
                fw(0xC8) = 0;                                          // std::vector<Skill> at +0xC8
                fw(0xCC) = 0;
                fw(0xD0) = 0;
                const uint32_t n = ld32(S + 0x90);
                Addr src = S + 0x98;
                for (uint32_t i = 0; i < n; i++, src += 0x28) {
                    const uint32_t end = fw(0xCC), cap = fw(0xD0);
                    if (end == cap) {
                        cx(SkillVector_pushBackSlow, {fa(0xC8), src});
                    } else {
                        if (end != 0) {
                            for (uint32_t b = 0; b < 0x28; b += 4) st32(end + b, ld32(src + b));
                        }
                        fw(0xCC) = end + 0x28;
                    }
                }
                Addr found = 0;
                const Addr vbegin = fw(0xC8), vend = fw(0xCC);
                if (vbegin != vend) {
                    Addr e = vbegin;
                    do {
                        if ((ld32(e + 4) ^ idHi) == 0 && (ld32(e) ^ idLo) == 0) { found = e; break; }
                        e += 0x28;
                    } while (e != vend);
                }
                Addr obj = 0, ctrl = 0;
                bool haveObj = false;
                if (found) {
                    const Addr skillMgr = cx(SkillDataManager_getInstance, {});
                    cx(SkillDataManager_getData,
                       {fa(0xC0), skillMgr, 0, 0, idLo, idHi, ld32(found + 8), ld32(found + 0xC), ld32(found + 0x10),
                        ld32(found + 0x14), ld32(found + 0x18), ld32(found + 0x1C), ld32(found + 0x20),
                        ld32(found + 0x24), 0});
                    obj = fw(0xC0);
                    ctrl = fw(0xC4);
                    fw(0xC0) = 0;
                    fw(0xC4) = 0;
                    haveObj = obj != 0;
                }
                if (!haveObj) {
                    const Addr skillMgr = cx(SkillDataManager_getInstance, {});
                    cx(SkillDataManager_getData, {fa(0xB8), skillMgr, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
                    const Addr obj2 = fw(0xB8);
                    const Addr ctrl2 = fw(0xBC);
                    fw(0xB8) = 0;
                    fw(0xBC) = 0;
                    if (ctrl != 0) {
                        cx(guest::SHARED_RELEASE, {ctrl});
                        if (fw(0xBC) != 0) cx(guest::SHARED_RELEASE, {fw(0xBC)});
                    }
                    obj = obj2;
                    ctrl = ctrl2;
                }
                const Addr skillMgr2 = cx(SkillDataManager_getInstance, {});
                fw(0xA4) = obj;
                fw(0xA8) = ctrl;
                sharedAddRef(ctrl);
                {
                    const Addr path = cx(SkillDataManager_getImagePath, {skillMgr2, fa(0xA4)});
                    strInit(fa(0xAC), path, cx(STRLEN, {path}));
                    cx(ImageView_loadTexture, {skillIcon, fa(0xAC), 0});
                    strFree(fa(0xAC));
                }
                sharedRelease(fw(0xA8));
                cx(SkillVector_dtor, {fa(0xC8)});
                sharedRelease(ctrl);
            }
            k++;
            if (k >= int32_t(ld32(fw(0xD4) + 0x48))) break;
        }
        vcx(skillIcon, slot::setVisible, {skillIcon, 1});
    }
    sharedRelease(fw(0xD8));
}

RW_FUNCTION(0x46B15C, "SceneUtil::changePartnerAvatarPanel(Widget*, StageSupportList::SupportUser, AvatarIconCache*)", SceneUtil_changePartnerAvatarPanel);
