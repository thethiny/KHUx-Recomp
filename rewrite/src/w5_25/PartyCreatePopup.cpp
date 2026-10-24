// ScenePartyCreate::openPopUpCreateParty(): the party settings popup (PartyTop_Edit), new or editing the party
// the API holds (its flag byte +0x10; name +0x30, play style +0x3C, join type +0x40, message +0x44). The name field
// starts as "<player name><suffix>" and becomes an EditBox (max Misc 806), the message field a ui::Text plus an
// EditBox (max Misc 807); three play-style and three join-type checkboxes (default 2); OK ($_28) / Cancel. Players
// aged 12 or under (userDetail +0) get the fixed name / message plates and no edit boxes. Slides in from the left.
#include "../scene/SceneUi.hpp"
#include "../scene/SceneUtil.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_getSideMargin = 0x45AD0D, SceneUtil_getSideHeightMargin = 0x45AF59;
constexpr uint32_t SceneUtil_createLeftInAnimation = 0x45B5C5;   // (node, float, float, function<void()>)
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t String_assign = 0x498A1D;                     // std::string::operator=(const string&)
constexpr uint32_t Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;
constexpr uint32_t CheckBox_setSelectedState = rwlib::cocos2d::ui::CheckBox::setSelectedState;
constexpr uint32_t Director_getInstance = rwlib::cocos2d::Director::getInstance;
constexpr uint32_t EventDispatcher_removeEventListenersForTarget = rwlib::cocos2d::EventDispatcher::removeEventListenersForTarget;
constexpr uint32_t ScenePartyCreate_showPlayStyle = 0x6D3FB3;    // sub_6D3FB2({this, box1, box2, box3}, style)
constexpr uint32_t ScenePartyCreate_showJoinType = 0x6D4071;     // sub_6D4070({this, box1, box2, box3}, type)
}  // namespace g

constexpr uint32_t S_Json = 0xE05AFA;            // "cocostudio/publish/PartyTop_Edit.json"
constexpr uint32_t S_shadowCreateParty = 0xE05AE8, S_s_s = 0xDF8BBA;
constexpr uint32_t S_DFPSoGei = 0xDF9F4D, S_Calligraphy = 0xDF9F1F;
constexpr uint32_t S_Txt_Limit2 = 0xDFE418, S_Txt_Limit3 = 0xE05B20, S_TextField_Name = 0xDFF282,
                   S_TextField_Message = 0xE05AD6;
constexpr uint32_t S_Txt_OK = 0xDF869F, S_Button_Ok = 0xDF9564, S_Txt_Cancel = 0xDF86A6,
                   S_Button_Close = 0xDF8692, S_Txt_Party = 0xDF8DC4;
constexpr uint32_t S_Plate_PlayStyle = 0xE05B2B, S_Txt_PlayStyle = 0xE0008C, S_Txt_PlayStyle_1 = 0xE05B3B,
                   S_Txt_PlayStyle_2 = 0xE05B4B, S_Txt_PlayStyle_3 = 0xE05B5B;
constexpr uint32_t S_CheckBox_PlayStyle_1 = 0xE05B6B, S_CheckBox_PlayStyle_2 = 0xE05B80,
                   S_CheckBox_PlayStyle_3 = 0xE05B95;
constexpr uint32_t S_Plate_joinType = 0xE05BAA, S_Txt_joinType_1 = 0xE05BB9, S_Txt_joinType_2 = 0xE05BC8,
                   S_Txt_joinType_3 = 0xE05BD7;
constexpr uint32_t S_CheckBox_joinType_1 = 0xE05BE6, S_CheckBox_joinType_2 = 0xE05BFA,
                   S_CheckBox_joinType_3 = 0xE05C0E;
constexpr uint32_t S_Txt_Message = 0xE05C22, S_Base_Name = 0xE05C2E, S_Base_Name_U13 = 0xE05C38,
                   S_Txt_name = 0xE05C46, S_Base_Message = 0xE05C4F, S_Base_Message_U13 = 0xE05C5C;
constexpr int32_t TXT_NAME_SUFFIX = 103800039, TXT_MESSAGE = 103800057, TXT_LIMIT = 100100037;
constexpr int32_t TXT_OK = 100100001, TXT_CANCEL = 100100002, TXT_PARTY = 103800006;
constexpr int32_t TXT_PLAY_STYLE = 102400003, TXT_PLAY_1 = 102400006, TXT_PLAY_2 = 102400005, TXT_PLAY_3 = 102400004;
constexpr int32_t TXT_JOIN_TYPE = 103800019, TXT_JOIN_1 = 103800020, TXT_JOIN_2 = 103800021, TXT_JOIN_3 = 103800049;
constexpr int32_t TXT_INFO = 103800044;
constexpr int32_t MISC_NAME_MAX = 806, MISC_MESSAGE_MAX = 807;

constexpr uint32_t VT_Ok = 0xF67D30;              // 0x20 {this, layer, shadow, popup, this, nameBox, messageBox}
constexpr uint32_t VT_Cancel = 0xF67D04;          // no captures
constexpr uint32_t VT_CanClose = 0xF67CD8;        // {close}
constexpr uint32_t VT_Closed = 0xF67C28;          // 0x18 {this, layer, shadow, popup, this}
constexpr uint32_t VT_Play[3] = {0xF67BFC, 0xF67BD0, 0xF67BA4};   // 0x14 {this, box1, box2, box3}
constexpr uint32_t VT_Join[3] = {0xF67B78, 0xF67B4C, 0xF67B20};

// APIManager
constexpr uint32_t API_USER_DATA = 0x74, API_PARTY = 0x84, API_USER_DETAIL = 0x1D4;
constexpr uint32_t UD_NAME = 0x14, UD_NAME_SIZE = 0x21;
constexpr uint32_t PARTY_EXISTS = 0x10, PARTY_NAME = 0x30, PARTY_STYLE = 0x3C, PARTY_JOIN = 0x40, PARTY_MESSAGE = 0x44;
constexpr uint32_t AGE_U13 = 12;
// ScenePartyCreate
constexpr uint32_t PC_DELEGATE = 0x288, PC_EDIT_BOX = 0x29C, PC_EDIT_LABEL = 0x2A0, PC_EDITING = 0x2A4,
                   PC_MESSAGE_TEXT = 0x2A8;

constexpr uint32_t V_ADD_CHILD_TAG_NAME = 0xFC, V_SET_VISIBLE = 0xB0, V_GET_PARENT = 0x11C,
                   V_REMOVE_CHILD = 0x12C, V_GET_PLACE_HOLDER = 0x150;
constexpr uint32_t DIRECTOR_EVENT_DISPATCHER = 0x84;

void release(uint32_t ctrl) {
    if (ctrl) call(fn::shared_release, {ctrl});
}
uint32_t shadowed(uint32_t text) {
    call(g::SceneUtil_setShadow, {text});
    return text;
}
int32_t miscValue(Frame &f, uint32_t &row, int32_t id) {
    row = masterRow(f, fn::Misc_create, uint32_t(id));
    return safe32(ld32(row), 4);
}
void addCaptures(uint32_t widget, uint32_t vt, uint32_t size, std::initializer_list<uint32_t> words) {
    Frame f;
    uint32_t cb = newEmptyFunction(f);
    uint32_t heap = call(fn::operator_new, {size});
    if (heap) {
        st32(heap, vt);
        uint32_t k = 4;
        for (uint32_t w : words) { st32(heap + k, w); k += 4; }
    }
    st32(cb + 16, heap);
    call(fn::Widget_addTouchEventListener, {widget, cb});
    functionDestroy(cb);
}
void removeListeners(uint32_t target) {
    uint32_t dispatcher = ld32(call(g::Director_getInstance, {}) + DIRECTOR_EVENT_DISPATCHER);
    call(g::EventDispatcher_removeEventListenersForTarget, {dispatcher, target, 0});
}

struct PartyCreatePopup {
    static void open(uint32_t self) {
        uint32_t api = call(fn::APIManager_getInstance, {});
        uint32_t party = ld32(api + API_PARTY), partyCtrl = ld32(api + API_PARTY + 4);
        if (partyCtrl) call(fn::shared_add, {partyCtrl});
        api = call(fn::APIManager_getInstance, {});
        Frame f;
        bool editing = false;
        if (party) editing = ld8(safeBytes(f, party, PARTY_EXISTS, 1)) != 0;

        call(fn::AudioPlaySystemSEPopUp, {});
        uint32_t layer = call(fn::SceneManager_getPopupLayer, {sceneManager()});
        uint32_t shadow = call(g::SceneUtil_createShadowSprite, {});
        uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
        vcall(shadow, node::setOpacity, {shadow, 0});
        call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(0.1f), opacity})});
        {
            Frame t;
            uint32_t name = newStr(t, S_shadowCreateParty, 17);
            vcall(layer, V_ADD_CHILD_TAG_NAME, {layer, shadow, 0, name});
            strFree(name);
        }
        uint32_t popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_Json});
        vcall(layer, node::addChild, {layer, popup});
        {
            float x = bitsf(vcall(popup, node::getPositionX, {popup}));
            float m = bitsf(call(g::SceneUtil_getSideMargin, {}));
            vcall(popup, node::setPositionX, {popup, fbits(x + m)});
            float y = bitsf(vcall(popup, node::getPositionY, {popup}));
            float h = bitsf(call(g::SceneUtil_getSideHeightMargin, {}));
            vcall(popup, node::setPositionY, {popup, fbits(y + h)});
        }

        // the name: the player's name and the suffix, or the party's
        uint32_t name = f.alloc(12), message = f.alloc(12);
        {
            Frame t;
            uint32_t player = safeBytes(t, ld32(api + API_USER_DATA), UD_NAME, UD_NAME_SIZE);
            uint32_t suffix = textData(t, TXT_NAME_SUFFIX);
            uint32_t buf = stringBuffer(t, {S_s_s, player, textDataStr(suffix)});
            strInitC(name, buf);
            sharedRelease(suffix);
        }
        if (editing) call(g::String_assign, {name, party + PARTY_NAME});
        {
            Frame t;
            uint32_t text = textData(t, TXT_MESSAGE);
            strInitC(message, textDataStr(text));
            sharedRelease(text);
        }
        if (editing) call(g::String_assign, {message, party + PARTY_MESSAGE});

        uint32_t limit2 = shadowed(findNode(popup, S_Txt_Limit2, 10));
        uint32_t limit3 = shadowed(findNode(popup, S_Txt_Limit3, 10));
        uint32_t nameField = findNode(popup, S_TextField_Name, 14);
        uint32_t nameLayer = f.alloc(4), messageLayer = f.alloc(4);
        {
            Frame t;
            uint32_t text = textData(t, TXT_LIMIT);
            uint32_t row;
            int32_t max = miscValue(t, row, MISC_NAME_MAX);
            setTextC(limit3, stringBuffer(t, {textDataStr(text), uint32_t(max)}));
            sharedRelease(row);
            sharedRelease(text);
        }
        st32(nameLayer, 0);
        st8(self + PC_EDITING, 0);
        uint32_t nameBox;
        {
            Frame t;
            uint32_t placeholder = t.alloc(12);
            vcall(nameField, V_GET_PLACE_HOLDER, {placeholder, nameField});
            uint32_t size = t.alloc(8);
            call(g::Size_ctor, {size, fbits(510.0f), fbits(74.0f)});
            uint32_t row;
            int32_t max = miscValue(t, row, MISC_NAME_MAX);
            nameBox = SceneUtil::textFieldToEditBox(self + PC_EDIT_BOX, self + PC_EDIT_LABEL, nameField, placeholder,
                                                    strData(name), S_DFPSoGei, size, self + PC_DELEGATE,
                                                    self + PC_EDITING, nameLayer, uint32_t(max));
            sharedRelease(row);
            strFree(placeholder);
        }
        uint32_t messageField = findNode(popup, S_TextField_Message, 17);
        {
            Frame t;
            uint32_t text = textData(t, TXT_LIMIT);
            uint32_t row;
            int32_t max = miscValue(t, row, MISC_MESSAGE_MAX);
            setTextC(limit2, stringBuffer(t, {textDataStr(text), uint32_t(max)}));
            sharedRelease(row);
            sharedRelease(text);
        }
        st32(self + PC_MESSAGE_TEXT, SceneUtil::textFieldToUiText(messageField, strData(message), S_Calligraphy));
        st32(messageLayer, 0);
        uint32_t messageBox;
        {
            Frame t;
            uint32_t placeholder = t.alloc(12);
            vcall(messageField, V_GET_PLACE_HOLDER, {placeholder, messageField});
            uint32_t size = t.alloc(8);
            call(g::Size_ctor, {size, fbits(412.0f), fbits(148.0f)});
            uint32_t row;
            int32_t max = miscValue(t, row, MISC_MESSAGE_MAX);
            messageBox = SceneUtil::textFieldToEditBox(self + PC_EDIT_BOX, self + PC_EDIT_LABEL, messageField,
                                                       placeholder, strData(message), S_Calligraphy, size,
                                                       self + PC_DELEGATE, self + PC_EDITING, messageLayer,
                                                       uint32_t(max));
            sharedRelease(row);
            strFree(placeholder);
        }
        vcall(messageBox, V_SET_VISIBLE, {messageBox, 0});

        uint32_t okText = findNode(popup, S_Txt_OK, 6);
        setTextId(okText, TXT_OK);
        shadowed(okText);
        uint32_t ok = findNode(popup, S_Button_Ok, 9);
        addCaptures(ok, VT_Ok, 0x20, {self, layer, shadow, popup, self, nameBox, messageBox});
        uint32_t cancelText = findNode(popup, S_Txt_Cancel, 10);
        setTextId(cancelText, TXT_CANCEL);
        shadowed(cancelText);
        uint32_t close = findNode(popup, S_Button_Close, 12);
        addTouch(close, VT_Cancel, {});
        {
            uint32_t mgr = sceneManager();
            Frame t;
            uint32_t canClose = newFunction(t, VT_CanClose, {close});
            uint32_t closed = newEmptyFunction(t);
            uint32_t heap = call(fn::operator_new, {0x18});
            if (heap) {
                st32(heap, VT_Closed);
                st32(heap + 4, self);
                st32(heap + 8, layer);
                st32(heap + 0xC, shadow);
                st32(heap + 0x10, popup);
                st32(heap + 0x14, self);
            }
            st32(closed + 16, heap);
            call(fn::SceneManager_openPopup, {mgr, canClose, closed, 1});
            functionDestroy(closed);
            functionDestroy(canClose);
        }

        api = call(fn::APIManager_getInstance, {});
        uint32_t detail = ld32(api + API_USER_DETAIL), detailCtrl = ld32(api + API_USER_DETAIL + 4);
        if (detailCtrl) call(fn::shared_add, {detailCtrl});
        uint32_t partyText = findNode(popup, S_Txt_Party, 9);
        setTextId(partyText, TXT_PARTY);
        shadowed(partyText);
        choices(self, popup, editing ? party : 0, PARTY_STYLE, S_Plate_PlayStyle, 15,
                {S_Txt_PlayStyle_1, S_Txt_PlayStyle_2, S_Txt_PlayStyle_3}, 15,
                {TXT_PLAY_STYLE, TXT_PLAY_1, TXT_PLAY_2, TXT_PLAY_3},
                {S_CheckBox_PlayStyle_1, S_CheckBox_PlayStyle_2, S_CheckBox_PlayStyle_3}, 20,
                g::ScenePartyCreate_showPlayStyle, VT_Play);
        choices(self, popup, editing ? party : 0, PARTY_JOIN, S_Plate_joinType, 14,
                {S_Txt_joinType_1, S_Txt_joinType_2, S_Txt_joinType_3}, 14,
                {TXT_JOIN_TYPE, TXT_JOIN_1, TXT_JOIN_2, TXT_JOIN_3},
                {S_CheckBox_joinType_1, S_CheckBox_joinType_2, S_CheckBox_joinType_3}, 19,
                g::ScenePartyCreate_showJoinType, VT_Join);
        uint32_t info = findNode(popup, S_Txt_Message, 11);
        setTextId(info, TXT_INFO);
        shadowed(info);

        if (detail && safe32(detail, 0) <= int32_t(AGE_U13)) {
            uint32_t baseName = findNode(popup, S_Base_Name, 9);
            uint32_t baseNameU13 = findNode(popup, S_Base_Name_U13, 13);
            uint32_t fixedName = findNode(baseNameU13, S_Txt_name, 8);
            call(fn::Text_setString, {fixedName, name});
            vcall(baseName, V_SET_VISIBLE, {baseName, 0});
            vcall(baseNameU13, V_SET_VISIBLE, {baseNameU13, 1});
            removeListeners(ld32(nameLayer));
            uint32_t baseMessage = findNode(popup, S_Base_Message, 12);
            uint32_t baseMessageU13 = findNode(popup, S_Base_Message_U13, 16);
            uint32_t fixedMessage = findNode(baseMessageU13, S_Txt_Message, 11);
            call(fn::Text_setString, {fixedMessage, message});
            vcall(baseMessage, V_SET_VISIBLE, {baseMessage, 0});
            vcall(baseMessageU13, V_SET_VISIBLE, {baseMessageU13, 1});
            vcall(limit2, V_SET_VISIBLE, {limit2, 0});
            vcall(limit3, V_SET_VISIBLE, {limit3, 0});
            uint32_t text = ld32(self + PC_MESSAGE_TEXT);
            if (text) {
                uint32_t parent = vcall(text, V_GET_PARENT, {text});
                vcall(parent, V_REMOVE_CHILD, {parent, ld32(self + PC_MESSAGE_TEXT), 1});
                st32(self + PC_MESSAGE_TEXT, 0);
            }
            removeListeners(ld32(messageLayer));
        }
        {
            Frame t;
            uint32_t none = newEmptyFunction(t);
            uint32_t anim = call(g::SceneUtil_createLeftInAnimation, {popup, 0, 0, none});
            call(fn::Node_runAction, {popup, call(g::EaseSineOut_create, {anim})});
            functionDestroy(none);
        }
        release(detailCtrl);
        strFree(message);
        strFree(name);
        release(partyCtrl);
    }

    // a plate of three checkboxes: the labels, the current choice (1..3, else 2) and the three touch lambdas
    static void choices(uint32_t self, uint32_t popup, uint32_t party, uint32_t field, uint32_t plateName,
                        uint32_t plateLen, std::initializer_list<uint32_t> labels, uint32_t labelLen,
                        std::initializer_list<int32_t> texts, std::initializer_list<uint32_t> boxNames,
                        uint32_t boxLen, uint32_t show, const uint32_t (&vts)[3]) {
        uint32_t plate = findNode(popup, plateName, plateLen);
        uint32_t text[4];
        text[0] = findNode(plate, S_Txt_PlayStyle, 13);
        uint32_t k = 1;
        for (uint32_t l : labels) text[k++] = findNode(popup, l, labelLen);
        k = 0;
        for (int32_t id : texts) { setTextId(text[k], id); k++; }
        for (uint32_t t : text) call(g::SceneUtil_setShadow, {t});
        uint32_t box[3];
        k = 0;
        for (uint32_t b : boxNames) box[k++] = findNode(popup, b, boxLen);
        Frame f;
        uint32_t group = f.alloc(16);
        st32(group, self);
        st32(group + 4, box[0]);
        st32(group + 8, box[1]);
        st32(group + 12, box[2]);
        int32_t choice = 2;
        if (party) {
            choice = int32_t(ld32(party + field));
            if (uint32_t(choice - 1) >= 3) choice = 2;
        }
        call(show, {group, uint32_t(choice)});
        // 3 is the first box, 1 the last
        uint32_t selected = choice == 3 ? box[0] : choice == 1 ? box[2] : box[1];
        call(g::CheckBox_setSelectedState, {selected, 1});
        for (uint32_t i = 0; i < 3; i++) addCaptures(box[i], vts[i], 0x14, {self, box[0], box[1], box[2]});
    }
};
}  // namespace

RW_FUNCTION(0x6D2320, "ScenePartyCreate::openPopUpCreateParty()", PartyCreatePopup::open);
