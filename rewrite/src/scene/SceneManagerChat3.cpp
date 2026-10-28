// SceneManager: the chat stamp list popup (talk stamps, then medal stamps, paged with two arrow buttons).
#include "SceneManager.hpp"
#include "SceneUi.hpp"

using namespace scene;

namespace {
constexpr uint32_t STAMP_MIGRATING = 0x498;      // uint8 (flag498): the stamp list popup is open
constexpr uint32_t STAMP_LIST = 0x4A8;           // ChatStampListScrollView* (created once, kept)

// ChatStampListScrollView
constexpr uint32_t LIST_ON_SELECT = 0x640;       // std::function<void(bool)>
constexpr uint32_t LIST_ON_PAGE = 0x560;         // std::function<void()>
constexpr uint32_t LIST_SLOT_34C = 0x34C;        // vtable byte offset, called with false after the layout
// ChatStampListScrollView::ChatStamp (28 bytes)
constexpr uint32_t STAMP_ROW_SIZE = 0x1C;
constexpr uint32_t ROW_ID = 0x00, ROW_FLAG = 0x04, ROW_IMAGE = 0x08, ROW_IS_MEDAL = 0x14, ROW_STAMP_ID = 0x18;

// APIManager: user stamps (object at +0x7C; its SafeBuffer at +0x38 holds the stamp ids, count at +0x20)
constexpr uint32_t API_USER_STAMPS = 0x7C;
constexpr uint32_t STAMPS_BUFFER = 0x38, BUFFER_COUNT = 0x20;
// master::Stamp row (SafeBuffer)
constexpr uint32_t STAMP_KIND = 4, STAMP_NUMBER = 8, STAMP_ENABLED = 0x1C;
constexpr int32_t STAMP_KIND_MEDAL = 3;

constexpr uint32_t ArmatureDataManager_getInstance = rwlib::cocostudio::ArmatureDataManager::getInstance;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = rwlib::cocostudio::ArmatureDataManager::addArmatureFileInfo;
constexpr uint32_t Armature_create = rwlib::cocostudio::Armature::create;
constexpr uint32_t ARMATURE_GET_ANIMATION = 0x298;     // vtable byte offset
constexpr uint32_t ANIMATION_PLAY = 0x68;              // ArmatureAnimation::play(name, durationTo, loop)
constexpr uint32_t ChatStampListScrollView_create = 0x55B9D9;
constexpr uint32_t ChatStampListScrollView_resetScrollPosition = 0x55C609;
constexpr uint32_t ChatStampListScrollView_setStampList = 0x55C4D5;
constexpr uint32_t BaseStageScrollView_setIndex = 0x59D5A9, BaseStageScrollView_updateLayout = 0x59D3F5;
constexpr uint32_t FunctionB_swap = 0x490179, FunctionV_swap = 0x4EE86D;
constexpr uint32_t TouchBeganFunction_swap = 0x483E4D, TouchFunction_swap = 0x4F3699;
constexpr uint32_t Stamp_create = 0x7919E5;                     // master::Stamp::create(shared_ptr out, int id)
constexpr uint32_t IntVector_pushSlow = 0x47D709, IntVector_destroy = 0x478961;
constexpr uint32_t SortTalkStamps = 0x630415, SortMedalStamps = 0x62FA31;    // std::sort instantiations (begin, end)
constexpr uint32_t ToString = 0xC95451;                         // std::to_string(int) (sret)
constexpr uint32_t StampVector_pushSlow = 0x630181, StampVector_copy = 0x6190F1, StampVector_destroy = 0x55B939;
constexpr uint32_t ImageButton_create = 0x531239;               // (string by value, bool)
constexpr uint32_t ImageButton_setTargetView = 0x5318C1;        // (button, node, float scale)
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t EventListenerTouchOneByOne_create = rwlib::cocos2d::EventListenerTouchOneByOne::create, EventListenerTouchOneByOne_setSwallowTouches = rwlib::cocos2d::EventListenerTouchOneByOne::setSwallowTouches;
constexpr uint32_t EventDispatcher_addEventListenerWithSceneGraphPriority = rwlib::cocos2d::EventDispatcher::addEventListenerWithSceneGraphPriority;
constexpr uint32_t Widget_setBright = rwlib::cocos2d::ui::Widget::setBright;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base, Size_ctor = rwlib::cocos2d::Size::ctor_base__ff;
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;

// lambdas
constexpr uint32_t VT_Stamp_Select = 0xF5FF28;   // $_47 [send button, send text] (void(bool))
constexpr uint32_t VT_Stamp_Page = 0xF5FEFC;     // $_48 [this]
constexpr uint32_t VT_Stamp_Left = 0xF5FED0;     // $_51 [this]
constexpr uint32_t VT_Stamp_Right = 0xF5FEA4;    // $_52 [this]
constexpr uint32_t VT_Stamp_TouchBegan = 0xF5FE78;  // $_53 []
constexpr uint32_t VT_Stamp_TouchEnded = 0xF5FE4C;  // $_54 (heap 0x14): the popup bounds (xMax, xMin, yMax, yMin)
constexpr uint32_t VT_Stamp_CloseTouch = 0xF5FE20;  // $_55 []
constexpr uint32_t VT_Stamp_SendTouch = 0xF5FDC8;   // $_56 (heap 0x30)
constexpr uint32_t VT_Stamp_CanClose = 0xF5FD9C;    // $_57 [close button]
constexpr uint32_t VT_Stamp_OnClose = 0xF5FD44;     // $_58 (heap 0x20)

constexpr uint32_t STR_Empty = 0xDF8A1A;
constexpr uint32_t STR_ChatStampJson = 0xE03391;    // "cocostudio/publish/Chat_Stamp.json"
constexpr uint32_t STR_ArrowAnimJson = 0xE00B10;    // "cocostudio/publish/ArrowAnim.ExportJson"
constexpr uint32_t STR_ArrowLeft = 0xE00B38, STR_ArrowAnim = 0xE00B43, STR_Left = 0xE033B4;
constexpr uint32_t STR_ArrowRight = 0xE00B4D, STR_Right = 0xE00B59, STR_Base = 0xDFEF5A;
constexpr uint32_t STR_ButtonSend = 0xE00637, STR_TxtSend = 0xE0064F, STR_Area = 0xDF9378;
constexpr uint32_t STR_TalkStampPath = 0xDFED50;    // "img/stamp/talkstamp_"
constexpr uint32_t STR_MedalPath = 0xDFED36;        // "img/medal/Medal_L_"
constexpr uint32_t STR_Png = 0xDFED65, STR_CloseButton = 0xDF8D2F;
constexpr int32_t TEXT_SEND = 104200001;

uint32_t mgrAddr(SceneManager *self) { return rw::guest_addr(self); }

// vector<int>::push_back(value), inlined (`tmp` holds the value for the slow path)
void pushInt(uint32_t vec, uint32_t tmp, uint32_t value) {
    st32(tmp, value);
    uint32_t end = ld32(vec + 4);
    if (end < ld32(vec + 8)) {
        uint32_t at = 0;
        if (end) {
            st32(end, value);
            at = ld32(vec + 4);
        }
        st32(vec + 4, at + 4);
    } else {
        call(IntVector_pushSlow, {vec, tmp});
    }
}

// vector<ChatStamp>::push_back(row), inlined: the bytes after the flag at +0x14 come along (stack garbage)
void pushStamp(uint32_t vec, uint32_t row) {
    uint32_t end = ld32(vec + 4);
    if (end == ld32(vec + 8)) {
        call(StampVector_pushSlow, {vec, row});
        return;
    }
    uint32_t at = 0;
    if (end) {
        st32(end + ROW_ID, ld32(row + ROW_ID));
        st8(end + ROW_FLAG, ld8(row + ROW_FLAG));
        uint32_t s = row + ROW_IMAGE, d = end + ROW_IMAGE;
        if (strIsLong(s)) {
            strInit(d, ld32(s + 8), ld32(s + 4));
        } else {
            st32(d, ld32(s));
            st32(d + 4, ld32(s + 4));
            st32(d + 8, ld32(s + 8));
        }
        uint32_t w0 = ld32(row + ROW_IS_MEDAL), w1 = ld32(row + ROW_STAMP_ID);
        st8(end + 0x18, uint8_t(w1));
        st8(end + 0x14, uint8_t(w0));
        st8(end + 0x1B, uint8_t(w1 >> 24));
        st8(end + 0x1A, uint8_t(w1 >> 16));
        st8(end + 0x19, uint8_t(w1 >> 8));
        st8(end + 0x17, uint8_t(w0 >> 24));
        st8(end + 0x16, uint8_t(w0 >> 16));
        st8(end + 0x15, uint8_t(w0 >> 8));
        at = ld32(vec + 4);
    }
    st32(vec + 4, at + STAMP_ROW_SIZE);
}

// the inlined vector<int>::clear arithmetic (end moves back to begin)
void clearInts(uint32_t vec) {
    uint32_t begin = ld32(vec), end = ld32(vec + 4);
    if (end != begin) st32(vec + 4, end + (~(end - 4 - begin) & 0xFFFFFFFCu));
}

// int at `index` of a SafeBuffer
int32_t safeInt(uint32_t sb, uint32_t index) {
    Frame f;
    return int32_t(ld32(safeBytes(f, sb, index, 4)));
}

// the user's stamps whose master row is enabled and whose kind is (medal == true: 3, else: not 3)
void collectStamps(uint32_t stamps, uint32_t count, uint32_t ints, bool medal) {
    Frame f;
    uint32_t tmp = f.alloc(4);
    for (uint32_t i = 0, off = 0; i < count; i++, off += 4) {
        int32_t id = safeInt(ld32(stamps + STAMPS_BUFFER), off);
        Frame g;
        uint32_t row = masterRow(g, Stamp_create, uint32_t(id));
        uint32_t sb = ld32(row);
        if (sb && safeInt(sb, STAMP_ENABLED) && (safeInt(ld32(row), STAMP_KIND) == STAMP_KIND_MEDAL) == medal)
            pushInt(ints, tmp, uint32_t(safeInt(ld32(stamps + STAMPS_BUFFER), off)));
        sharedRelease(row);
    }
}

// `prefix` + stamp number + ".png" into the std::string at `out`
void stampImagePath(uint32_t out, uint32_t prefix, uint32_t prefixLen, uint32_t stampRow) {
    strInit(out, prefix, prefixLen);
    int32_t number = safeInt(stampRow, STAMP_NUMBER);
    Frame f;
    uint32_t digits = f.alloc(12);
    call(ToString, {digits, uint32_t(number)});
    call(fn::string_append, {out, strData(digits), strSize(digits)});
    strFree(digits);
    call(fn::string_append, {out, STR_Png, 4});
}


// an armature "ArrowAnim" playing `name` under `arrow`, moved by (50, 50); pos = the Vec2 kept for both arrows
uint32_t addArrowAnim(uint32_t arrow, uint32_t name, uint32_t nameLen, uint32_t pos, bool construct) {
    uint32_t armature;
    {
        Frame f;
        uint32_t s = newStr(f, STR_ArrowAnim, 9);
        armature = call(Armature_create, {s});
        strFree(s);
    }
    uint32_t anim = vcall(armature, ARMATURE_GET_ANIMATION, {armature});
    {
        uint32_t play = ld32(ld32(anim) + ANIMATION_PLAY);
        Frame f;
        uint32_t s = newStr(f, name, nameLen);
        call(play, {anim, s, 0xFFFFFFFFu, 0xFFFFFFFFu});
        strFree(s);
    }
    uint32_t p = vcall(armature, node::getPosition, {armature});
    if (construct) {
        call(fn::Vec2_ctor_copy, {pos, p});
    } else {
        st32(pos, ld32(p));
        st32(pos + 4, ld32(p + 4));
    }
    stf(pos, ldf(pos) + 50.0f);
    stf(pos + 4, ldf(pos + 4) + 50.0f);
    vcall(armature, node::setPosition, {armature, pos});
    vcall(arrow, node::addChild, {arrow, armature});
    return armature;
}

// an ImageButton over the arrow, added to the arrow's first child, calling `vtable`{this} when touched
void addArrowButton(SceneManager *self, uint32_t arrow, uint32_t vtable, bool resize, uint32_t otherArrow) {
    uint32_t children = vcall(arrow, node::getChildren, {arrow});
    uint32_t holder = ld32(ld32(children));
    uint32_t button;
    {
        Frame f;
        uint32_t s = newStr(f, STR_Empty, 0);
        button = call(ImageButton_create, {s, 1});
        strFree(s);
    }
    if (resize) {
        for (uint32_t a : {arrow, otherArrow}) {
            uint32_t setSize = ld32(ld32(a) + node::setContentSize);
            Frame f;
            uint32_t size = f.alloc(8);
            call(Size_ctor, {size, fbits(100.0f), fbits(100.0f)});
            call(setSize, {a, size});
            vcall(a, node::setAnchorPoint, {a, VEC2_ANCHOR_MIDDLE});
        }
    }
    call(ImageButton_setTargetView, {button, arrow, fbits(1.0f)});
    vcall(holder, node::addChild, {holder, button});
    Frame f;
    uint32_t cb = newFunction(f, vtable, {mgrAddr(self)});
    call(ImageButton_addTouchEventListener, {button, cb});
    functionDestroy(cb);
}
}  // namespace

// The stamp list popup: Chat_Stamp.json over a shadow on popup layer 2, arrows (animated) to page, the
// ChatStampListScrollView with the user's talk stamps then medal stamps, a send button, and touch outside closes.
void SceneManager::openChatStampListPopup(SceneManager *self) {
    const uint32_t m = mgrAddr(self);
    // the original's frame: its rows and the send lambda's captures copy stack bytes it never wrote to the heap
    Frame frame;
    const uint32_t base = frame.allocExact(0x360);
    const uint32_t medalRow = base + 0xB8, talkRow = base + 0x100;
    const uint32_t ints = base + 0x150, rows = base + 0x15C, arrowPos = base + 0x1D0;
    const uint32_t sendCaptures = base + 0x23C;

    call(fn::AudioPlaySystemSEPopUp, {});
    uint32_t layer = getInstance()->popupLayer2;
    uint32_t shadow = SceneUtil::createShadowSprite();
    uint32_t opacity = vcall(shadow, node::getOpacity, {shadow});
    vcall(shadow, node::setOpacity, {shadow, 0});
    call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(0.1f), opacity})});
    vcall(layer, node::addChildZ, {layer, shadow, 2});
    st8(m + STAMP_MIGRATING, 1);
    uint32_t popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), STR_ChatStampJson});

    // centre the popup on the visible area
    vcall(popup, node::setAnchorPoint, {popup, VEC2_ANCHOR_MIDDLE});
    {
        uint32_t setPos = ld32(ld32(popup) + node::setPosition);
        float px = bitsf(vcall(popup, node::getPositionX, {popup}));
        float w = ldf(vcall(popup, node::getContentSize, {popup}));
        float margin = SceneUtil::getSideMargin();
        float py = bitsf(vcall(popup, node::getPositionY, {popup}));
        uint32_t size = vcall(popup, node::getContentSize, {popup});
        float x = margin + (px + w * 0.5f);
        float y = py + ldf(size + 4) * 0.5f;
        float yy = SceneUtil::getSideHeightMargin() + y;
        Frame f;
        uint32_t v = f.alloc(8);
        call(Vec2_ctor, {v, fbits(x), fbits(yy)});
        call(setPos, {popup, v});
        call(Vec2_dtor, {v});
    }
    vcall(shadow, node::addChildZ, {shadow, popup, 3});

    // the paging arrows
    {
        uint32_t adm = call(ArmatureDataManager_getInstance, {});
        Frame f;
        uint32_t s = newStr(f, STR_ArrowAnimJson, 0x27);
        call(ArmatureDataManager_addArmatureFileInfo, {adm, s});
        strFree(s);
    }
    uint32_t arrowLeft = findNode(popup, STR_ArrowLeft, 10);
    addArrowAnim(arrowLeft, STR_Left, 4, arrowPos, true);
    uint32_t arrowRight = findNode(popup, STR_ArrowRight, 11);
    addArrowAnim(arrowRight, STR_Right, 5, arrowPos, false);

    // the popup's bounds, 15% / 20% larger: touches outside close it
    float xMin, xMax, yMin, yMax;
    {
        float x = ldf(vcall(popup, node::getPosition, {popup}));
        xMin = x + ldf(vcall(popup, node::getContentSize, {popup})) * -0.5f * 1.15f;
        x = ldf(vcall(popup, node::getPosition, {popup}));
        xMax = x + ldf(vcall(popup, node::getContentSize, {popup})) * 0.5f * 1.15f;
        float y = ldf(vcall(popup, node::getPosition, {popup}) + 4);
        yMin = y + ldf(vcall(popup, node::getContentSize, {popup}) + 4) * -0.5f * 1.2f;
        y = ldf(vcall(popup, node::getPosition, {popup}) + 4);
        yMax = y + ldf(vcall(popup, node::getContentSize, {popup}) + 4) * 0.5f * 1.2f;
    }
    uint32_t panel = findNode(popup, STR_Base, 4);
    vcall(panel, node::setOpacity, {panel, 255});
    uint32_t sendButton = findNode(popup, STR_ButtonSend, 11);
    call(Widget_setBright, {sendButton, 0});
    uint32_t sendText = findNode(popup, STR_TxtSend, 8);
    SceneUtil::setShadow(sendText);
    setTextId(sendText, TEXT_SEND);
    SceneUtil::setButtonEnabled(sendButton, sendText, 0, 0);
    uint32_t area = findNode(popup, STR_Area, 4);
    for (uint32_t k = 0; k < 6; k++) st32(ints + 4 * k, 0);      // ints, rows: empty vectors

    // the list view (created on the first open)
    if (!ld32(m + STAMP_LIST)) {
        uint32_t list = call(ChatStampListScrollView_create, {});
        st32(m + STAMP_LIST, list);
        uint32_t setSize = ld32(ld32(list) + node::setContentSize);
        call(setSize, {list, vcall(area, node::getContentSize, {area})});
        Frame f;
        uint32_t pos = f.alloc(8);
        call(fn::Vec2_ctor_copy, {pos, vcall(ld32(m + STAMP_LIST), node::getPosition, {ld32(m + STAMP_LIST)})});
        vcall(ld32(m + STAMP_LIST), node::setPosition, {ld32(m + STAMP_LIST), pos});
        vcall(area, node::addChild, {area, ld32(m + STAMP_LIST)});
        {
            uint32_t target = ld32(m + STAMP_LIST) + LIST_ON_SELECT;
            Frame g;
            uint32_t cb = newFunction(g, VT_Stamp_Select, {sendButton, sendText});
            call(FunctionB_swap, {cb, target});
            functionDestroy(cb);
        }
        {
            uint32_t target = ld32(m + STAMP_LIST) + LIST_ON_PAGE;
            Frame g;
            uint32_t cb = newFunction(g, VT_Stamp_Page, {m});
            call(FunctionV_swap, {cb, target});
            functionDestroy(cb);
        }
        call(Vec2_dtor, {pos});
    }
    call(ChatStampListScrollView_resetScrollPosition, {ld32(m + STAMP_LIST)});

    // talk stamps (kind != 3), sorted
    uint32_t stamps = ld32(call(fn::APIManager_getInstance, {}) + API_USER_STAMPS);
    uint32_t count = ld32(ld32(stamps + STAMPS_BUFFER) + BUFFER_COUNT);
    collectStamps(stamps, count, ints, false);
    call(SortTalkStamps, {ld32(ints), ld32(ints + 4)});
    if (ld32(ints + 4) != ld32(ints)) {
        for (uint32_t i = 0; i < ((ld32(ints + 4) - ld32(ints)) >> 2); i++) {
            Frame f;
            uint32_t row = masterRow(f, Stamp_create, ld32(ld32(ints) + 4 * i));
            uint32_t path = f.alloc(12);
            stampImagePath(path, STR_TalkStampPath, 0x14, ld32(row));
            if (((ld32(ints + 4) - ld32(ints)) >> 2) <= i) call(ThrowOutOfRange, {ints});
            st32(talkRow + ROW_ID, ld32(ld32(ints) + 4 * i));
            st8(talkRow + ROW_FLAG, 1);
            strCopy(talkRow + ROW_IMAGE, path);
            st8(talkRow + ROW_IS_MEDAL, 0);
            if (((ld32(ints + 4) - ld32(ints)) >> 2) <= i) call(ThrowOutOfRange, {ints});
            st32(talkRow + ROW_STAMP_ID, ld32(ld32(ints) + 4 * i));
            pushStamp(rows, talkRow);
            strFree(talkRow + ROW_IMAGE);
            strFree(path);
            sharedRelease(row);
        }
        clearInts(ints);
    }

    // medal stamps (kind 3), sorted
    collectStamps(stamps, count, ints, true);
    call(SortMedalStamps, {ld32(ints), ld32(ints + 4)});
    if (ld32(ints + 4) != ld32(ints)) {
        for (uint32_t i = 0; i < ((ld32(ints + 4) - ld32(ints)) >> 2); i++) {
            Frame f;
            uint32_t row = masterRow(f, Stamp_create, ld32(ld32(ints) + 4 * i));
            uint32_t path = f.alloc(12);
            stampImagePath(path, STR_MedalPath, 0x12, ld32(row));
            st32(medalRow + ROW_ID, count + i);
            st8(medalRow + ROW_FLAG, 1);
            strCopy(medalRow + ROW_IMAGE, path);
            st8(medalRow + ROW_IS_MEDAL, 1);
            if (((ld32(ints + 4) - ld32(ints)) >> 2) <= i) call(ThrowOutOfRange, {ints});
            st32(medalRow + ROW_STAMP_ID, ld32(ld32(ints) + 4 * i));
            pushStamp(rows, medalRow);
            strFree(medalRow + ROW_IMAGE);
            strFree(path);
            sharedRelease(row);
        }
    }
    call(ChatStampListScrollView_setStampList, {ld32(m + STAMP_LIST), rows});
    call(BaseStageScrollView_setIndex, {ld32(m + STAMP_LIST), 0});
    call(BaseStageScrollView_updateLayout, {ld32(m + STAMP_LIST), 0});
    vcall(ld32(m + STAMP_LIST), LIST_SLOT_34C, {ld32(m + STAMP_LIST), 0});

    // the arrow buttons
    addArrowButton(self, arrowLeft, VT_Stamp_Left, true, arrowRight);
    addArrowButton(self, arrowRight, VT_Stamp_Right, false, 0);

    // touches outside the popup close it
    uint32_t closeButton = findNode(popup, STR_CloseButton, 12);
    uint32_t dispatcher = ld32(director() + 0x84);
    uint32_t listener = call(EventListenerTouchOneByOne_create, {});
    call(EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    {
        Frame f;
        uint32_t began = newFunction(f, VT_Stamp_TouchBegan, {});
        call(TouchBeganFunction_swap, {began, listener + 0x50});
        functionDestroy(began);
        uint32_t ended = f.alloc(20);
        st32(ended + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x14});
        if (heap) {
            st32(heap, VT_Stamp_TouchEnded);
            stf(heap + 4, xMax);
            stf(heap + 8, xMin);
            stf(heap + 12, yMax);
            stf(heap + 16, yMin);
        }
        st32(ended + 16, heap);
        call(TouchFunction_swap, {ended, listener + 0x80});
        functionDestroy(ended);
    }
    call(EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, shadow});
    addTouch(closeButton, VT_Stamp_CloseTouch, {});

    // send: $_56 holds {send, this, close, a copy of the rows, this, close, send} (+ the stack bytes after)
    st32(sendCaptures, sendButton);
    st32(sendCaptures + 4, m);
    st32(sendCaptures + 8, closeButton);
    call(StampVector_copy, {sendCaptures + 0xC, rows});
    st32(sendCaptures + 0x18, m);
    st32(sendCaptures + 0x1C, closeButton);
    st32(sendCaptures + 0x20, sendButton);
    {
        Frame f;
        uint32_t fo = f.alloc(20);
        st32(fo + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x30});
        if (heap) {
            st32(heap, VT_Stamp_SendTouch);
            for (uint32_t k = 0; k < 3; k++) st32(heap + 4 + 4 * k, ld32(sendCaptures + 4 * k));
            for (uint32_t k = 0; k < 3; k++) st32(heap + 0x10 + 4 * k, ld32(sendCaptures + 0xC + 4 * k));
            for (uint32_t k = 0; k < 3; k++) st32(sendCaptures + 0xC + 4 * k, 0);    // the vector moved
            for (uint32_t k = 0; k < 4; k++) st32(heap + 0x1C + 4 * k, ld32(sendCaptures + 0x18 + 4 * k));
            st8(heap + 0x2C, ld8(sendCaptures + 0x28));
        }
        st32(fo + 16, heap);
        call(fn::Widget_addTouchEventListener, {sendButton, fo});
        functionDestroy(fo);
    }
    call(StampVector_destroy, {sendCaptures + 0xC});
    {
        Frame f;
        uint32_t none = newEmptyFunction(f);
        call(fn::Node_runAction, {popup, SceneUtil::createOpenPopupAnimation(popup, none)});
        functionDestroy(none);
    }
    {
        SceneManager *mgr = getInstance();
        Frame f;
        uint32_t canClose = newFunction(f, VT_Stamp_CanClose, {closeButton});
        uint32_t onClose = f.alloc(20);
        st32(onClose + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x20});
        if (heap) {
            const uint32_t words[7] = {sendButton, closeButton, m, popup, area, layer, shadow};
            st32(heap, VT_Stamp_OnClose);
            for (uint32_t k = 0; k < 7; k++) st32(heap + 4 + 4 * k, words[k]);
        }
        st32(onClose + 16, heap);
        call(fn::SceneManager_openPopup, {mgrAddr(mgr), canClose, onClose, 1});
        functionDestroy(onClose);
        functionDestroy(canClose);
    }
    call(IntVector_destroy, {ints});
    call(StampVector_destroy, {rows});
    call(Vec2_dtor, {arrowPos});
}
RW_FUNCTION(0x617C00, "SceneManager::openChatStampListPopup", SceneManager::openChatStampListPopup);
