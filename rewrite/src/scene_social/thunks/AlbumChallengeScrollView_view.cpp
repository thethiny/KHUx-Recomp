// AlbumChallengeScrollView: construction, rows and the reward popup (the std::function thunks of its lambdas are in
// AlbumChallengeScrollView_thunks.cpp).
#include "AlbumChallengeScrollView.hpp"

#include "../SocialCommon.hpp"

using namespace social;
using Self = AlbumChallengeScrollView;

namespace {
namespace g {
constexpr uint32_t aeabi_memset = 0x40E7A0;                // __aeabi_memset(dst, n, c)
constexpr uint32_t Layout_ctor = rwlib::cocos2d::ui::Layout::ctor_base;                 // cocos2d::ui::Layout::Layout()
constexpr uint32_t IScrollProtocol_dtor = 0x4F57E1;        // IScrollProtocol::~IScrollProtocol
constexpr uint32_t IScrollProtocol_setEnabledScrollBar = 0x4F6FE9;
constexpr uint32_t BaseScrollView_int_init = 0x4FA8A1;     // BaseScrollView<int>::init()
constexpr uint32_t vector_int_dtor = 0x478961;             // std::__vector_base<int>::~__vector_base
constexpr uint32_t throwOutOfRange = 0x47CCE1;             // std::vector::at out of range (throws)
constexpr uint32_t Widget_create = rwlib::cocos2d::ui::Widget::create;
constexpr uint32_t Widget_clone = rwlib::cocos2d::ui::Widget::clone;
constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance;
constexpr uint32_t GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr uint32_t ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE;       // (image, const string&, TextureResType)
constexpr uint32_t TextData_format = 0x492929;             // TextData::format(string* out, Type, id, ...)
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_setItemImageData = 0x46C71D;  // (string& name, string& image, int, int, bool)
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;   // (node, std::function<void()> by value)
constexpr uint32_t SceneUtil_settingTitleLabel = 0x45D8D1;
constexpr uint32_t SceneUtil_updateMedalRarePanel = 0x4619F5;  // (widget, rare, align, float scale, bool, int, int)
constexpr uint32_t AudioPlaySystemSEPopUp = 0x5C0E09;
constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create;               // (float duration, uint8 opacity)
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t SceneManager_getPopupLayer2 = 0x613E71;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;   // (sret, mgr, u64 id, int medalId, Medal by value)
constexpr uint32_t MedalDataManager_getMedalLargeImagePath = 0x76B1E5;   // (mgr, shared_ptr by value) -> path
constexpr uint32_t SkillDataManager_getInstance = 0x76BA0D;
constexpr uint32_t SkillDataManager_getData = 0x76BA59;    // (sret, mgr, u64 id, Skill by value, bool)
constexpr uint32_t SkillDataManager_getImagePath = 0x76BE8D;   // (mgr, shared_ptr by value) -> path
constexpr uint32_t SafeBuffer_read = 0x45A269;             // (sb, dst, src, n, index)
}  // namespace g

constexpr uint32_t GOT_ANCHOR_MIDDLE = 0xFD6054;           // GOT slot: &cocos2d::Vec2::ANCHOR_MIDDLE
constexpr uint32_t GOT_IN_OUT_TIME = 0xFD65C0;             // GOT slot: &SceneUtil::IN_OUT_TIME (float)
constexpr uint32_t BASE_VTABLE = 0xF811D0;                 // _ZTV14BaseScrollViewIiE

// strings
constexpr uint32_t S_empty = 0xDF8A1A;
constexpr uint32_t S_PanelJson = 0xE00BBC;     // "cocostudio/publish/Album_Challenge_Panel_ver112.json"
constexpr uint32_t S_ReceiveJson = 0xE00C79;   // "cocostudio/publish/Album_Challenge_Receive_ver112.json"
constexpr uint32_t S_RewardSetJson = 0xE00C2A; // "cocostudio/publish/Album_Reward_Set_ver112.json"
constexpr uint32_t S_DrawMedalJson = 0xE00CBB; // "cocostudio/publish/DeckEdit_DrawMedal.json"
constexpr uint32_t S_Txt_Mission = 0xE00BF1;
constexpr uint32_t S_Dummy_Panel = 0xDFF409;
constexpr uint32_t S_Clear_Mask = 0xE00BFD;
constexpr uint32_t S_Clear = 0xDFD17B;
constexpr uint32_t S_Txt_Reward = 0xE00C08;
constexpr uint32_t S_Area_Reward = 0xE00C13;
constexpr uint32_t S_Set_Reward = 0xE00C1F;
constexpr uint32_t S_LuxBoard = 0xE00C5A;
constexpr uint32_t S_Incentive = 0xDFEF2F;
constexpr uint32_t S_Medal = 0xDFD05F;
constexpr uint32_t S_KB = 0xDFEF39;
constexpr uint32_t S_Icon_Base = 0xDFEB70;
constexpr uint32_t S_Gender = 0xE00C63;
constexpr uint32_t S_Icon_Avatar = 0xDFEBAB;
constexpr uint32_t S_Stamp = 0xDFED30;
constexpr uint32_t S_Star_Area = 0xDF8C22;
constexpr uint32_t S_Button_Receive = 0xE00C6A;
constexpr uint32_t S_Txt_Receive = 0xDFD6EE;
constexpr uint32_t S_Txt_Title = 0xDFF142;
constexpr uint32_t S_Medal_Area = 0xE00CB0;
constexpr uint32_t S_RankTitle_Board = 0xDF940E;
constexpr uint32_t S_RankTitle_Name = 0xDF941E;
constexpr uint32_t S_Button_OK = 0xDF8688;
constexpr uint32_t S_Txt_OK = 0xDF869F;
constexpr uint32_t S_Icon_Skill = 0xDF8C40;
constexpr uint32_t S_Icon_Skil = 0xDF8F81;

// text ids
constexpr int32_t TEXT_MISSION = 103600007;
constexpr int32_t TEXT_REWARD_TITLE = 103600011;
constexpr int32_t TEXT_REWARD_COUNT = 103601010;   // "%s x%d"-like format: image name, count
constexpr int32_t TEXT_OK = 100100001;

// lambdas (std::function __func vtables + 8)
constexpr uint32_t VT_rowTouch = 0xF5CA5C;      // changeRow $_0 {row, button, view}: the receive button
constexpr uint32_t VT_popupOk = 0xF5CA04;       // openAlbumChallengeGetRewardPopup $_1 {popup, layer, shadow}
constexpr uint32_t VT_popupOpened = 0xF5C9D8;   // openAlbumChallengeGetRewardPopup $_2 {popup}

constexpr uint32_t SLOT_init = 0x248;
constexpr uint32_t SLOT_release = 0x04;
constexpr uint32_t SLOT_changeRow = 0x340;      // AlbumChallengeScrollView::changeRow (virtual)
constexpr uint32_t SLOT_setTouchEnabled = 0x29C;

uint32_t itemCount(const Self *s) { return (s->itemIDs.end - s->itemIDs.begin) >> 2; }

// text->setString(c string at `cstr`), through a std::string temporary
void setText(Addr text, Addr cstr) {
    Frame f;
    Addr s = newStr(f, cstr);
    call(fn::Text_setString, {text, s});
    strFree(s);
}

// TextData::format(TEXT_REWARD_COUNT, image name, 10) into `text`
void setRewardCount(Addr text, Addr imageName) {
    Frame f;
    Addr out = f.alloc(12);
    call(g::TextData_format, {out, 0, uint32_t(TEXT_REWARD_COUNT), strData(imageName), 10});
    setText(text, strData(out));
    strFree(out);
}
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
Self *Self::destroy(Self *self) {
    Addr me = rw::guest_addr(self);
    rw::set<uint32_t>(self, 0, VTABLE);
    rw::set<uint32_t>(self, 0x280, VTABLE2);
    rw::set<uint32_t>(self, 0x350, VTABLE3);
    if (Addr panel = self->panel) vcall(panel, SLOT_release, {panel});
    self->panel = 0;
    for (uint32_t k = 5; k-- > 0;) functionDestroy(me + 0x638 + 24 * k);
    // back to BaseScrollView<int>
    rw::set<uint32_t>(self, 0, BASE_VTABLE + 8);
    rw::set<uint32_t>(self, 0x280, BASE_VTABLE + 0x360);
    rw::set<uint32_t>(self, 0x350, BASE_VTABLE + 0x374);
    call(g::vector_int_dtor, {me + 0x628});
    call(g::IScrollProtocol_dtor, {me});
    return self;
}
RW_FUNCTION(0x5BAC00, "AlbumChallengeScrollView::~AlbumChallengeScrollView", AlbumChallengeScrollView::destroy);

void Self::destroyDelete(Self *self) { call(fn::operator_delete, {rw::guest_addr(destroy(self))}); }
RW_FUNCTION(0x5BAD58, "AlbumChallengeScrollView::~AlbumChallengeScrollView (deleting)",
            AlbumChallengeScrollView::destroyDelete);

Addr Self::create() {
    Addr view = call(fn::operator_new, {SIZE});
    construct(rw::host_ptr<Self>(view));
    if (vcall(view, SLOT_init, {view}) == 1) {
        call(sfn::Ref_autorelease, {view});
        return view;
    }
    vcall(view, SLOT_release, {view});
    return 0;
}
RW_FUNCTION(0x5BAD8C, "AlbumChallengeScrollView::create", AlbumChallengeScrollView::create);

Self *Self::construct(Self *self) {
    Addr me = rw::guest_addr(self);
    call(g::Layout_ctor, {me});
    // IScrollProtocol
    rw::set<float>(self, 0x4FC, 0.1f);
    rw::set<float>(self, 0x500, 0.3f);
    rw::set<uint32_t>(self, 0x504, 0);
    rw::set<uint32_t>(self, 0x508, 0);
    rw::set<uint32_t>(self, 0x50C, 0);
    rw::set<float>(self, 0x510, 200.0f);
    call(g::aeabi_memset, {me + 0x514, 0x24, 0});
    rw::set<uint32_t>(self, 0x538, 1);
    rw::set<uint8_t>(self, 0x53C, 1);
    rw::set<uint32_t>(self, 0x590, 0);
    rw::set<uint8_t>(self, 0x594, 0);
    rw::set<uint32_t>(self, 0x5C4, 0);
    rw::set<uint32_t>(self, 0x5D8, 0);
    rw::set<uint32_t>(self, 0x5F0, 0);
    rw::set<uint32_t>(self, 0x608, 0);
    rw::set<uint32_t>(self, 0x620, 0);
    call(g::aeabi_memset, {me + 0x540, 0x12, 0});
    call(g::aeabi_memset, {me + 0x554, 0x12, 0});
    call(g::aeabi_memset, {me + 0x568, 0x25, 0});
    call(g::aeabi_memset, {me + 0x598, 0x2A, 0});
    // BaseScrollView<int>
    self->itemIDs.begin = 0;
    self->itemIDs.end = 0;
    self->itemIDs.cap = 0;
    // AlbumChallengeScrollView
    rw::set<uint32_t>(self, 0, VTABLE);
    rw::set<uint32_t>(self, 0x280, VTABLE2);
    rw::set<uint32_t>(self, 0x350, VTABLE3);
    for (uint32_t k = 0; k < 5; k++) rw::set<uint32_t>(self, 0x648 + 24 * k, 0);   // empty std::functions
    self->tapEnabled = 0;
    self->field_6B5 = 0;
    self->field_6B8 = 0;
    self->field_6BC = 0;
    self->field_6C0 = 0;
    return self;
}
RW_FUNCTION(0x5BADE0, "AlbumChallengeScrollView::AlbumChallengeScrollView", AlbumChallengeScrollView::construct);

uint32_t Self::init(Self *self) {
    Addr me = rw::guest_addr(self);
    rw::set<float>(self, 0x514, 921.0f);    // item width
    rw::set<float>(self, 0x518, 151.0f);    // item height
    call(g::IScrollProtocol_setEnabledScrollBar, {me, 1});
    Addr reader = call(g::GUIReader_getInstance, {});
    self->panel = call(g::GUIReader_widgetFromJsonFile, {reader, S_PanelJson});
    call(sfn::Ref_retain, {self->panel});
    vcall(self->panel, SLOT_setTouchEnabled, {self->panel, 0});
    Addr panel = self->panel;
    uint32_t setPosition = ld32(ld32(panel) + node::setPosition);
    float width = ldf(vcall(panel, node::getContentSize, {panel}));
    {
        Frame f;
        Addr pos = f.alloc(8);
        call(sfn::Vec2_ctor, {pos, fbits(width * 0.5f), 0});
        call(setPosition, {panel, pos});
        call(sfn::Vec2_dtor, {pos});
    }
    return call(g::BaseScrollView_int_init, {me});
}
RW_FUNCTION(0x5BAF50, "AlbumChallengeScrollView::init", AlbumChallengeScrollView::init);

// A row: an empty widget of the item size holding a clone of the panel template, centred horizontally.
Addr Self::getLayoutPanel(Self *self, int32_t) {
    Addr row = call(g::Widget_create, {});
    Addr content = call(g::Widget_clone, {self->panel});
    vcall(row, node::setAnchorPoint, {row, ld32(GOT_ANCHOR_MIDDLE)});
    vcall(row, node::addChild, {row, content});
    uint32_t setContentSize = ld32(ld32(row) + node::setContentSize);
    {
        Frame f;
        Addr size = f.alloc(8);
        call(sfn::Size_ctor, {size, rw::get<uint32_t>(self, 0x514), rw::get<uint32_t>(self, 0x518)});
        call(setContentSize, {row, size});
    }
    vcall(row, node::setPositionX, {row, fbits(rw::get<float>(self, 0x514) * 0.5f)});
    return row;
}
RW_FUNCTION(0x5BB044, "AlbumChallengeScrollView::getLayoutPanel", AlbumChallengeScrollView::getLayoutPanel);

Addr Self::createRow(Self *self, uint32_t index, int32_t column) {
    if (itemCount(self) <= index) {
        call(g::throwOutOfRange, {});
        return 0;
    }
    Addr row = getLayoutPanel(self, int32_t(index));
    Addr me = rw::guest_addr(self);
    vcall(me, SLOT_changeRow, {me, row, index, uint32_t(column)});
    return row;
}
RW_FUNCTION(0x5BB10C, "AlbumChallengeScrollView::createRow", AlbumChallengeScrollView::createRow);

// Fill a row for challenge `index`: mission text, reward (an incentive image from setItemImageData, the other
// reward kinds hidden) and the receive button. Returns 1 when the row has no "Txt_Mission" text, else 0.
uint32_t Self::changeRow(Self *self, Addr row, uint32_t index, int32_t) {
    if (itemCount(self) <= index) {
        call(g::throwOutOfRange, {});
        return 0;
    }
    if (ld32(self->itemIDs.begin + 4 * index) == 0) return 0;
    Addr mission = findNode(row, S_Txt_Mission, 11);
    if (!mission) return 1;
    Frame f;
    Addr image = newStr(f, S_empty, 0);   // the reward's image path
    Addr name = newStr(f, S_empty, 0);    // its name
    call(g::SceneUtil_setItemImageData, {name, image, 1, 1, 0});
    if (Addr dummy = findNode(row, S_Dummy_Panel, 11)) vcall(dummy, SLOT_setTouchEnabled, {dummy, 0});
    setVisible(row, 1);
    Addr clearMask = findNode(row, S_Clear_Mask, 10);
    Addr clear = findNode(row, S_Clear, 5);
    if (clearMask && clear) {
        setVisible(clearMask, 0);
        setVisible(clear, 0);
    }
    call(g::SceneUtil_setShadow, {mission});
    setTextId(mission, TEXT_MISSION);
    if (Addr reward = findNode(row, S_Txt_Reward, 10)) {
        call(g::SceneUtil_setShadow, {reward});
        setRewardCount(reward, name);
    }
    Addr area = findNode(row, S_Area_Reward, 11);
    Addr set = findNode(area, S_Set_Reward, 10);
    if (!set) {
        Addr json = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_RewardSetJson});
        uint32_t addChildZName = ld32(ld32(area) + node::addChildZName);
        Addr copy = call(g::Widget_clone, {json});
        withStr(S_Set_Reward, 10, [&](Addr s) { call(addChildZName, {area, copy, 0, s}); });
        set = findNode(area, S_Set_Reward, 10);
    }
    Addr luxBoard = findNode(set, S_LuxBoard, 8);
    Addr incentive = findNode(set, S_Incentive, 9);
    Addr medal = findNode(set, S_Medal, 5);
    Addr kb = findNode(set, S_KB, 2);
    Addr iconBase = findNode(set, S_Icon_Base, 9);
    Addr gender = findNode(set, S_Gender, 6);
    Addr avatar = findNode(set, S_Icon_Avatar, 11);
    Addr stamp = findNode(set, S_Stamp, 5);
    Addr stars = findNode(set, S_Star_Area, 9);
    for (Addr n : {stars, luxBoard, incentive, medal, kb, iconBase, gender, avatar, stamp}) setVisible(n, 0);
    call(g::ImageView_loadTexture, {incentive, image, 0});
    setVisible(incentive, 1);
    Addr button = findNode(row, S_Button_Receive, 14);
    if (button) {
        if (Addr label = findNode(row, S_Txt_Receive, 11)) {
            call(g::SceneUtil_setShadow, {label});
            setTextId(label, TEXT_MISSION);
        }
    }
    setVisible(button, 0);
    addTouch(button, VT_rowTouch, {row, button, rw::guest_addr(self)});
    strFree(name);
    strFree(image);
    return 0;
}
RW_FUNCTION(0x5BB164, "AlbumChallengeScrollView::changeRow", AlbumChallengeScrollView::changeRow);

void Self::registItemTouchEvent(Self *, Addr, int32_t) {}
RW_FUNCTION(0x5BBB7C, "AlbumChallengeScrollView::registItemTouchEvent", AlbumChallengeScrollView::registItemTouchEvent);

Self *Self::setEnabledTapItem(Self *self, uint32_t enabled) {
    self->tapEnabled = uint8_t(enabled);
    return self;
}
RW_FUNCTION(0x5BBB80, "AlbumChallengeScrollView::setEnabledTapItem", AlbumChallengeScrollView::setEnabledTapItem);

// ------------------------------------------------------------------------------------------------ reward popup
// The "reward received" popup: a modal shadow on popup layer 2 holding the Album_Challenge_Receive layout, the
// reward set (createRewardLayer) with the incentive image, and an OK button that closes it ($_1).
void Self::openAlbumChallengeGetRewardPopup(Addr) {
    call(g::AudioPlaySystemSEPopUp, {});
    Addr layer = call(g::SceneManager_getPopupLayer2, {call(fn::SceneManager_getInstance, {})});
    Addr shadow = call(g::SceneUtil_createShadowSprite, {});
    uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
    vcall(shadow, node::setOpacity, {shadow, 0});
    call(fn::Node_runAction, {shadow, call(g::FadeTo_create, {ld32(ld32(GOT_IN_OUT_TIME)), opacity})});
    vcall(layer, node::addChild, {layer, shadow});
    Addr popup = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_ReceiveJson});
    vcall(shadow, node::addChild, {shadow, popup});
    {
        uint32_t setAnchor = ld32(ld32(popup) + node::setAnchorPoint);
        Frame f;
        Addr anchor = f.alloc(8);
        call(sfn::Vec2_ctor, {anchor, fbits(0.5f), fbits(0.5f)});
        call(setAnchor, {popup, anchor});
        call(sfn::Vec2_dtor, {anchor});
    }
    uint32_t setPosition = ld32(ld32(popup) + node::setPosition);
    call(setPosition, {popup, vcall(shadow, node::getPosition, {shadow})});
    Frame f;
    Addr image = newStr(f, S_empty, 0);
    Addr name = newStr(f, S_empty, 0);
    call(g::SceneUtil_setItemImageData, {name, image, 1, 1, 0});
    Addr title = findNode(popup, S_Txt_Title, 9);
    call(g::SceneUtil_setShadow, {title});
    setTextId(title, TEXT_REWARD_TITLE);
    Addr reward = findNode(popup, S_Txt_Reward, 10);
    call(g::SceneUtil_setShadow, {reward});
    setRewardCount(reward, name);
    Addr area = findNode(popup, S_Area_Reward, 11);
    Addr medalArea = findNode(popup, S_Medal_Area, 10);
    Addr rankBoard = findNode(popup, S_RankTitle_Board, 15);
    Addr kb = findNode(popup, S_KB, 2);
    Addr stamp = findNode(popup, S_Stamp, 5);
    Addr luxBoard = findNode(popup, S_LuxBoard, 8);
    Addr rankName = findNode(popup, S_RankTitle_Name, 14);
    call(g::SceneUtil_setShadow, {rankName});
    setVisible(medalArea, 0);
    setVisible(rankBoard, 0);
    setVisible(kb, 0);
    setVisible(stamp, 0);
    setVisible(luxBoard, 0);
    // createRewardLayer is called with whatever the last setVisible returned in r0 as `this` (unused)
    uint32_t r0 = vcall(rankName, node::setVisible, {rankName, 0});
    Addr set = createRewardLayer(r0);
    Addr incentive = findNode(set, S_Incentive, 9);
    call(g::ImageView_loadTexture, {incentive, image, 0});
    setVisible(incentive, 1);
    vcall(area, node::addChild, {area, set});
    Addr ok = findNode(popup, S_Button_OK, 9);
    addTouch(ok, VT_popupOk, {popup, layer, shadow});
    titleLabel(ok, S_Txt_OK, 6, TEXT_OK);
    withLambda(VT_popupOpened, {popup}, [&](Addr done) {
        Addr anim = call(g::SceneUtil_createOpenPopupAnimation, {popup, done});
        call(fn::Node_runAction, {popup, call(g::EaseSineOut_create, {anim})});
    });
    strFree(name);
    strFree(image);
}
RW_FUNCTION(0x5BBB8C, "AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup",
            AlbumChallengeScrollView::openAlbumChallengeGetRewardPopup);

// The DeckEdit_DrawMedal layout for user medal `userMedalId`: large image, rarity stars and the skill icon(s).
Addr Self::createDrawMedal(Addr, uint64_t userMedalId, int32_t rare) {
    // the original frame (pushed registers + locals): getMedalData takes the medal record by value at [sp + 8]
    // and reads its never-initialised bytes 0x2C..0x30 from the stack
    guest::Frame frame(0x148);
    Addr sp = frame.addr();
    Addr mgr = call(g::MedalDataManager_getInstance, {});
    Addr record = sp + 0xE0;
    call(g::aeabi_memset, {record, 0x2C, 0});
    call(g::aeabi_memset, {record + 0x30, 0x18, 0});
    *rw::frame_ptr<uint32_t>(sp) = 0;                               // medalId 0
    for (uint32_t k = 0; k < 72; k += 4) *rw::frame_ptr<uint32_t>(sp + 8 + k) = ld32(record + k);
    Addr data = sp + 0xD0;                                          // shared_ptr<UserMedalData>
    rw::call_exact(g::MedalDataManager_getMedalData,
                   {data, mgr, uint32_t(userMedalId), uint32_t(userMedalId >> 32)});
    Addr drawMedal = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_DrawMedalJson});
    Addr medalImage = findNode(drawMedal, S_Medal, 5);
    // large medal image (the shared_ptr passed by value)
    {
        Addr mgr2 = call(g::MedalDataManager_getInstance, {});
        Addr copy = sp + 0xB0;
        st32(copy, ld32(data));
        uint32_t ctrl = ld32(data + 4);
        st32(copy + 4, ctrl);
        if (ctrl) guest::addRef(ctrl);
        Addr path = call(g::MedalDataManager_getMedalLargeImagePath, {mgr2, copy});
        Addr s = sp + 0xB8;
        strInitC(s, path);
        call(g::ImageView_loadTexture, {medalImage, s, 0});
        strFree(s);
        if (ld32(copy + 4)) guest::release(ld32(copy + 4));
    }
    // rarity (master::Medal position 0x470), read twice
    auto readRare = [&](Addr dst) {
        Addr sb = ld32(ld32(data) + 0x58);
        rw::call(g::SafeBuffer_read, {sb, dst, ld32(sb + 0xC) + 0x470, 4, 0x470});
    };
    readRare(sp + 0xD8);
    Addr stars = findNode(drawMedal, S_Star_Area, 9);
    readRare(sp + 0xDC);
    call(g::SceneUtil_updateMedalRarePanel,
         {stars, ld32(sp + 0xDC), 1, fbits(0.75f), 0, ld32(sp + 0xD8), uint32_t(rare)});
    Addr icon = findNode(drawMedal, S_Icon_Skill, 10);
    if (!icon) icon = findNode(drawMedal, S_Icon_Skil, 9);
    setVisible(icon, 0);
    vcall(icon, node::setLocalZOrder, {icon, 1});
    Addr medal = ld32(data);
    if (medal && int32_t(ld32(medal + 0x48)) >= 1) {
        uint32_t offset = 0x38;
        int32_t k = 0;
        do {
            uint32_t lo = ld32(medal + offset), hi = ld32(medal + offset + 4);
            if (lo | hi) {
                Addr skillMgr = call(g::SkillDataManager_getInstance, {});
                Addr skill = sp + 0x84;
                // the UserSkill::Skill record by value (zeroed) and the bool follow on the stack
                call(g::SkillDataManager_getData, {skill, skillMgr, lo, hi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
                uint32_t obj = ld32(skill), ctrl = ld32(skill + 4);
                st32(skill, 0);
                st32(skill + 4, 0);
                Addr skillMgr2 = call(g::SkillDataManager_getInstance, {});
                Addr copy = sp + 0x70;
                st32(copy, obj);
                st32(copy + 4, ctrl);
                if (ctrl) guest::addRef(ctrl);
                Addr path = call(g::SkillDataManager_getImagePath, {skillMgr2, copy});
                Addr s = sp + 0x78;
                strInitC(s, path);
                call(g::ImageView_loadTexture, {icon, s, 0});
                strFree(s);
                if (ld32(copy + 4)) guest::release(ld32(copy + 4));
                if (ctrl) guest::release(ctrl);
            }
            offset += 8;
            k++;
            medal = ld32(data);
        } while (k < int32_t(ld32(medal + 0x48)));
        setVisible(icon, 1);
    }
    if (ld32(data + 4)) guest::release(ld32(data + 4));
    return drawMedal;
}
RW_FUNCTION(0x5BC390, "AlbumChallengeScrollView::createDrawMedal", AlbumChallengeScrollView::createDrawMedal);

// The Album_Reward_Set layout with every reward kind hidden.
Addr Self::createRewardLayer(Addr) {
    Addr set = call(g::GUIReader_widgetFromJsonFile, {call(g::GUIReader_getInstance, {}), S_RewardSetJson});
    Addr luxBoard = findNode(set, S_LuxBoard, 8);
    Addr incentive = findNode(set, S_Incentive, 9);
    Addr medal = findNode(set, S_Medal, 5);
    Addr kb = findNode(set, S_KB, 2);
    Addr iconBase = findNode(set, S_Icon_Base, 9);
    Addr gender = findNode(set, S_Gender, 6);
    Addr avatar = findNode(set, S_Icon_Avatar, 11);
    Addr stamp = findNode(set, S_Stamp, 5);
    Addr stars = findNode(set, S_Star_Area, 9);
    for (Addr n : {stars, luxBoard, incentive, medal, kb, iconBase, gender, avatar, stamp}) setVisible(n, 0);
    return set;
}
RW_FUNCTION(0x5BC7C4, "AlbumChallengeScrollView::createRewardLayer", AlbumChallengeScrollView::createRewardLayer);
