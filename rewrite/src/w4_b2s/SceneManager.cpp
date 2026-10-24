// SceneManager::openPartyNewPopup(std::function<void()> callback): the "new party member" popup
// (PopupNormal_Party_JoinNew): shows the first party user that has data (name, comment, join date), hides itself
// behind the notices the party data asks for (flags at +0x4BC..+0x4C0), and calls `callback` at once when there is
// nothing to show.
//
// The original keeps a 0x460 byte frame whose bytes matter: the copied "opacity word" at +0x1D4 holds one written
// byte and three stale ones that end up in heap lambdas. The frame is reproduced with the original offsets.
#include "B2sCommon.hpp"

using namespace b2s;

namespace {
constexpr Addr FunctionVoid_copy = 0x4683CD;           // std::function<void()>::function(const function&)
constexpr Addr FunctionVoid_move = 0x47F905;           // std::function<void()>::function(function&&)
constexpr Addr FunctionVoid_call = 0x470E69;           // std::function<void()>::operator()() const
constexpr Addr VectorThrowOutOfRange = 0x47CCE1;
constexpr Addr HashTable_find_u64 = 0x490275;          // unordered_map<unsigned long long, int>::find
constexpr Addr Date_makeLocalDate = 0x4927CD;          // Date::makeLocalDate() const (sret, 6 bytes)
constexpr Addr TextData_get = 0x4928AD;                // TextData::get(shared_ptr out, Type, id)
constexpr Addr TextData_format = 0x492929;             // TextData::format(string out, Type, id, ...)
constexpr Addr SceneUtil_createShadowSprite = 0x45AD69;
constexpr Addr SceneUtil_getSideMargin = 0x45AD0D;
constexpr Addr SceneUtil_getSideHeightMargin = 0x45AF59;
constexpr Addr SceneUtil_setShadow = 0x45BD15;
constexpr Addr SceneUtil_openOkDialog4Line = 0x45C775; // (const std::string&, function<void(bool)>, const char*)
constexpr Addr GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance;
constexpr Addr GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr Addr Widget_addTouchEventListener = rwlib::cocos2d::ui::Widget::addTouchEventListener;
constexpr Addr Text_setString = rwlib::cocos2d::ui::Text::setString;
constexpr Addr Vec2_ctorXY = rwlib::cocos2d::Vec2::ctor_base__ff;
constexpr Addr Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr Addr SceneManager_openPopup = 0x61524D;      // (mgr, function<bool()>, function<void(bool)>, bool)
constexpr Addr SceneManager_closeCurrentPopup = 0x6216A1;   // (mgr, bool)
constexpr Addr SceneManager_openPartyBecomePopup = 0x620DE5;   // (ignored, function<void(bool)>)
constexpr Addr PartyNewPopup_showNextNotice = 0x620D11;        // sub_620D10(W): closure body, W = {popup, shadow, opacity word}

constexpr Addr GOT_ANCHOR_MIDDLE = 0xFD6054;

// lambda __func vtables created here
constexpr Addr VT_okTouch = 0xF5F454;                  // heap lambda of the OK button (0x40 bytes)
constexpr Addr VT_popupCondition = 0xF5F428;           // function<bool()> given to openPopup (no captures)
constexpr Addr VT_popupDone = 0xF5F3FC;                // function<void(bool)> given to openPopup (0x30 bytes)
constexpr Addr VT_noticeSkill = 0xF5F29C;              // ok dialog callbacks (0x18 bytes each)
constexpr Addr VT_noticeBecome = 0xF5F1EC;
constexpr Addr VT_noticeB3 = 0xF5F194;
constexpr Addr VT_noticeB2 = 0xF5F168;

// ---- string literals
constexpr Addr STR_JoinNewJson = 0xE03567;             // "cocostudio/publish/PopupNormal_Party_JoinNew.json"
constexpr Addr STR_Txt_Wording = 0xDF862F;             // 11
constexpr Addr STR_Txt_JoinDate = 0xE03599;            // 12
constexpr Addr STR_Txt_Name_Label = 0xDFDBB8;          // 14
constexpr Addr STR_Txt_Comment = 0xDF9EF7;             // 11
constexpr Addr STR_Txt_Comment_Label = 0xDFE1CC;       // 17
constexpr Addr STR_Txt_OK = 0xDF869F;                  // 6
constexpr Addr STR_Button_OK = 0xDF8688;               // 9

// ---- TextData ids
constexpr uint32_t TEXT_COMMENT = 0x62FDCEC;
constexpr uint32_t TEXT_OK = 0x5F767A1;
constexpr uint32_t TEXT_NAME = 0x630C722;
constexpr uint32_t TEXT_DATE_FORMAT = 0x630CB0B;
constexpr uint32_t TEXT_NOTICE0 = 0x62FDCFE;
constexpr uint32_t TEXT_NOTICE3 = 0x630C721;
constexpr uint32_t TEXT_NOTICE2 = 0x62FDCFD;

// SceneManager members
constexpr uint32_t SM_LAYER = 0x3E8;                   // the layer the popup goes to
constexpr uint32_t SM_FLAG_B3 = 0x4BC, SM_FLAG_HAS_MEMBERS = 0x4BD, SM_FLAG_B0 = 0x4BE, SM_FLAG_B1 = 0x4BF,
                   SM_FLAG_B2 = 0x4C0, SM_MEMBER_INDEX = 0x4C4;
// APIManager members
constexpr uint32_t API_PARTY = 0x1EC;                  // shared_ptr<UserParty>: object, control block (+0x1F0)
constexpr uint32_t API_USERS = 0x1FC;                  // shared_ptr<...>: the party members' user rows (SafeBuffer), +0x200 ctrl

// ---- frame offsets (the original's sp offsets)
constexpr uint32_t F_SIZE = 0x460;
constexpr uint32_t F_W = 0x1CC;                // {popup, shadow, opacity word}
constexpr uint32_t F_B = 0x40C;                // copy of W (+0), layer/shadow/popup (+0xC), callback copy at +0x1C
constexpr uint32_t F_CB = 0x428;               // std::function<void()> copy of the callback
constexpr uint32_t F_C1 = 0x290;               // layer, shadow, popup; and the function D after it
constexpr uint32_t F_D = 0x2A0;                // function copy of the callback (callable at +0x10)
constexpr uint32_t F_C2 = 0x238;
constexpr uint32_t F_H = 0x248;                // function copy of D
constexpr uint32_t F_K = 0x260;                // function<void(bool)> (heap lambda)
constexpr uint32_t F_EA = 0x278;               // function<bool()> (inline lambda)
constexpr uint32_t F_FL = 0x2C8;               // function of the OK button listener
constexpr uint32_t F_C0 = 0x2BC;               // popup, shadow, opacity word again
constexpr uint32_t F_ROW = 0x2E0;              // SupportUser row (0x128 bytes)

// std::function copy constructor as the compiler inlines it (dst must be an uninitialised function object)
void fnCopy(Addr dst, Addr src) {
    const Addr p = ld32(src + 16);
    if (p == 0) {
        st32(dst + 16, 0);
    } else if (p == src) {
        st32(dst + 16, dst);
        vcx(p, 0x0C, {p, dst});                // __clone(dst)
    } else {
        st32(dst + 16, vcx(p, 0x08, {p}));     // __clone() -> heap copy
    }
}
}  // namespace

void openPartyNewPopup(uint32_t self, uint32_t callback) {
    guest::Frame F(F_SIZE);
    const Addr me = self;
    auto fa = [&](uint32_t off) { return F.addr(off); };
    auto sm = [&]() { return cx(SCENEMANAGER_GETINSTANCE, {}); };
    auto strlenOf = [&](Addr s) { return cx(STRLEN, {s}); };
    // setString(node, c string) through a temporary std::string in the frame
    auto setText = [&](Addr node, Addr tmp, Addr text) {
        strInit(tmp, text, strlenOf(text));
        cx(Text_setString, {node, tmp});
        strFree(tmp);
    };

    // ---- the party data
    const Addr api = cx(APIMANAGER_GETINSTANCE, {});
    const uint32_t partyCtrl = ld32(api + API_PARTY + 4);
    const Addr party = ld32(api + API_PARTY);
    sharedAddRef(partyCtrl);
    if (party == 0) {
        if (ld32(callback + 0x10) != 0) cx(FunctionVoid_call, {callback});
        sharedRelease(partyCtrl);
        return;
    }
    const uint32_t b0 = ld8(party), b1 = ld8(party + 1), b2 = ld8(party + 2), b3 = ld8(party + 3);
    st8(me + SM_FLAG_B3, uint8_t(b3));
    st8(me + SM_FLAG_B0, uint8_t(b0));
    st8(me + SM_FLAG_B1, uint8_t(b1));
    st8(me + SM_FLAG_B2, uint8_t(b2));
    const bool hasMembers = ld32(party + 8) != ld32(party + 4);
    st8(me + SM_FLAG_HAS_MEMBERS, hasMembers ? 1 : 0);
    const bool nothing = !hasMembers && b3 == 0;
    if ((b0 | b1 | b2) == 0 && nothing) {
        // no member and no notice: just run the callback
        if (ld32(callback + 0x10) != 0) cx(FunctionVoid_call, {callback});
        sharedRelease(partyCtrl);
        return;
    }
    st32(me + SM_MEMBER_INDEX, 0);

    // ---- the popup on the SceneManager's layer, over a modal shadow
    const Addr api2 = cx(APIMANAGER_GETINSTANCE, {});
    const uint32_t usersCtrl = ld32(api2 + API_USERS + 4);
    const Addr users = ld32(api2 + API_USERS);
    sharedAddRef(usersCtrl);
    const Addr layer = ld32(sm() + SM_LAYER);
    const Addr shadow = cx(SceneUtil_createShadowSprite, {});
    const uint32_t opacity = vcx(shadow, 0x20C, {shadow});            // getOpacity
    vcx(shadow, slot::setOpacity, {shadow, 0});
    vcx(layer, slot::addChildZ, {layer, shadow, 0});
    const Addr reader = cx(GUIReader_getInstance, {});
    const Addr popup = cx(GUIReader_widgetFromJsonFile, {reader, STR_JoinNewJson});
    vcx(popup, slot::setAnchorPoint, {popup, ld32(GOT_ANCHOR_MIDDLE)});
    {
        const Addr setPosition = ld32(ld32(popup) + slot::setPosition);
        const uint32_t posX = vcx(popup, slot::getPositionX, {popup});
        const Addr sizeA = vcx(popup, slot::getContentSize, {popup});
        const uint32_t width = ld32(sizeA);
        const uint32_t sideWidth = cx(SceneUtil_getSideMargin, {});
        const uint32_t halfW = cx(FMUL, {width, 0x3F000000});
        const uint32_t sumX = cx(FADD, {posX, halfW});
        const uint32_t x = cx(FADD, {sideWidth, sumX});
        const uint32_t posY = vcx(popup, slot::getPositionY, {popup});
        const Addr sizeB = vcx(popup, slot::getContentSize, {popup});
        const uint32_t height = ld32(sizeB + 4);
        const uint32_t sideHeight = cx(SceneUtil_getSideHeightMargin, {});
        const uint32_t halfH = cx(FMUL, {height, 0x3F000000});
        const uint32_t sumY = cx(FADD, {posY, halfH});
        const uint32_t y = cx(FADD, {sideHeight, sumY});
        cx(Vec2_ctorXY, {fa(0x188), x, y});
        cx(setPosition, {popup, fa(0x188)});
        cx(Vec2_dtor, {fa(0x188)});
    }
    vcx(layer, slot::addChildZ, {layer, popup, 1});
    const Addr B = fa(F_B);
    st32(B + 0xC, layer);
    st32(B + 0x10, shadow);
    st32(B + 0x14, popup);
    cx(FunctionVoid_copy, {fa(F_CB), callback});
    // W = {popup, shadow, opacity byte (the other three bytes of the word are stale)}
    st32(fa(F_W), popup);
    st32(fa(F_W) + 4, shadow);
    st8(fa(F_W) + 8, uint8_t(opacity));

    // ---- the texts
    const Addr wording = findNode(popup, fa(0x17C), STR_Txt_Wording, 0xB);
    cx(SceneUtil_setShadow, {wording});
    const Addr joinDate = findNode(popup, fa(0x170), STR_Txt_JoinDate, 0xC);
    cx(SceneUtil_setShadow, {joinDate});
    const Addr nameLabel = findNode(popup, fa(0x164), STR_Txt_Name_Label, 0xE);
    cx(SceneUtil_setShadow, {nameLabel});
    const Addr comment = findNode(popup, fa(0x158), STR_Txt_Comment, 0xB);
    cx(SceneUtil_setShadow, {comment});
    const Addr commentLabel = findNode(popup, fa(0x14C), STR_Txt_Comment_Label, 0x11);
    cx(SceneUtil_setShadow, {commentLabel});
    const Addr okText = findNode(popup, fa(0x140), STR_Txt_OK, 6);
    cx(SceneUtil_setShadow, {okText});

    sharedAddRef(partyCtrl);
    sharedAddRef(usersCtrl);
    st32(B, ld32(fa(F_W)));
    st32(B + 4, ld32(fa(F_W) + 4));
    st32(B + 8, ld32(fa(F_W) + 8));

    // the comment's text and the OK button's text
    cx(TextData_get, {fa(0x12C), 0, TEXT_COMMENT});
    setText(comment, fa(0x134), ld32(ld32(fa(0x12C)) + 4));
    sharedRelease(ld32(fa(0x130)));
    cx(TextData_get, {fa(0x118), 0, TEXT_OK});
    setText(okText, fa(0x120), ld32(ld32(fa(0x118)) + 4));
    sharedRelease(ld32(fa(0x11C)));

    // ---- the first party member that has data
    st8(me + SM_FLAG_HAS_MEMBERS, 0);
    st32(me + SM_MEMBER_INDEX, 0);
    {
        Addr first = ld32(party + 4), last = ld32(party + 8);
        if (last != first) {
            const Addr hash = users + 0x24;                    // userId -> row
            uint32_t index = 0;
            for (;;) {
                const uint32_t count = uint32_t(int32_t(last - first) >> 4) * 0xAAAAAAABu;   // entries of 0x30 bytes
                if (!(count > index)) {
                    cx(VectorThrowOutOfRange, {party + 4});
                    return;
                }
                const Addr entry = first + index * 0x30;
                // the entry: a std::string (name), 16 bytes (user id, ...), a std::string (comment)
                if (ld8(entry) & 1) {
                    strInit(fa(0x198), ld32(entry + 8), ld32(entry + 4));
                } else {
                    st32(fa(0x198), ld32(entry));
                    st32(fa(0x19C), ld32(entry + 4));
                    st32(fa(0x1A0), ld32(entry + 8));
                }
                for (uint32_t k = 0; k < 4; k++) st32(fa(0x1A8) + 4 * k, ld32(entry + 0x10 + 4 * k));
                if (ld8(entry + 0x20) & 1) {
                    strInit(fa(0x1B8), ld32(entry + 0x28), ld32(entry + 0x24));
                } else {
                    st32(fa(0x1B8), ld32(entry + 0x20));
                    st32(fa(0x1BC), ld32(entry + 0x24));
                    st32(fa(0x1C0), ld32(entry + 0x28));
                }
                st32(fa(0x194), ld32(fa(0x1AC)));              // the 64 bit user id as the lookup key
                st32(fa(0x190), ld32(fa(0x1A8)));
                const Addr node = cx(HashTable_find_u64, {hash, fa(0x190)});
                int32_t row = -1;
                if (node != 0) row = int32_t(ld32(node + 0x10));
                if (node == 0 || row < 0) {
                    // no data for this member: next one
                    if (ld8(fa(0x1B8)) & 1) cx(OPERATOR_DELETE, {ld32(fa(0x1C0))});
                    if (ld8(fa(0x198)) & 1) cx(OPERATOR_DELETE, {ld32(fa(0x1A0))});
                    index = ld32(me + SM_MEMBER_INDEX) + 1;
                    st32(me + SM_MEMBER_INDEX, index);
                    first = ld32(party + 4);
                    last = ld32(party + 8);
                    if (index >= uint32_t(int32_t(last - first) >> 4) * 0xAAAAAAABu) break;
                    continue;
                }
                // found: the user's row (0x128 bytes, SafeBuffer) fills the texts
                const uint32_t offset = uint32_t(row) * 0x128;
                cx(SAFEBUFFER_READ, {users, fa(F_ROW), ld32(users + 0xC) + offset, 0x128, offset});
                cx(TextData_get, {fa(0x104), 0, TEXT_NAME});
                setText(wording, fa(0x10C), ld32(ld32(fa(0x104)) + 4));
                sharedRelease(ld32(fa(0x108)));
                // the join date: Date -> local date -> formatted text
                cx(Date_makeLocalDate, {fa(0xFC), fa(0x1B0)});
                const uint32_t h0 = ld16(fa(0xFC)), h1 = ld16(fa(0xFE)), h2 = ld16(fa(0x100));
                cx(TextData_format, {fa(0xE4), 0, TEXT_DATE_FORMAT, h0, h1 & 0xFF, h1 >> 8, h2 & 0xFF, h2 >> 8});
                const Addr formatted = (ld8(fa(0xE4)) & 1) ? ld32(fa(0xE4) + 8) : (fa(0xE4) | 1);
                strInit(fa(0xF0), formatted, strlenOf(formatted));
                cx(Text_setString, {joinDate, fa(0xF0)});
                strFree(fa(0xF0));
                strFree(fa(0xE4));
                // the user's name (a char array inside the row)
                {
                    const Addr nameChars = fa(F_ROW) + 0xC;
                    strInit(fa(0xD8), nameChars, strlenOf(nameChars));
                    cx(Text_setString, {nameLabel, fa(0xD8)});
                    strFree(fa(0xD8));
                }
                cx(Text_setString, {commentLabel, fa(0x1B8)});
                st32(me + SM_MEMBER_INDEX, ld32(me + SM_MEMBER_INDEX) + 1);
                st8(me + SM_FLAG_HAS_MEMBERS, 1);
                if (ld8(fa(0x1B8)) & 1) cx(OPERATOR_DELETE, {ld32(fa(0x1C0))});
                if (ld8(fa(0x198)) & 1) cx(OPERATOR_DELETE, {ld32(fa(0x1A0))});
                break;
            }
        }
    }

    // ---- the OK button: touch listener (heap lambda holding the widgets and the shared data)
    const Addr okButton = findNode(popup, fa(0xCC), STR_Button_OK, 9);
    sharedAddRef(partyCtrl);
    sharedAddRef(usersCtrl);
    st32(fa(F_C0), ld32(B));
    st32(fa(F_C0) + 4, ld32(B + 4));
    st32(fa(F_C0) + 8, ld32(B + 8));
    st32(fa(F_FL) + 0x10, 0);
    uint32_t spareCtrl1 = partyCtrl, spareCtrl2 = usersCtrl;       // released below when the lambda was not made
    const Addr okLambda = cx(OPERATOR_NEW, {0x40});
    if (okLambda) {
        st32(okLambda, VT_okTouch);
        st32(okLambda + 4, popup);
        st32(okLambda + 8, me);
        st32(okLambda + 0x10, party);
        st32(okLambda + 0x14, partyCtrl);
        st32(okLambda + 0x18, users);
        st32(okLambda + 0x1C, usersCtrl);
        st32(okLambda + 0x20, wording);
        st32(okLambda + 0x24, joinDate);
        st32(okLambda + 0x28, nameLabel);
        st32(okLambda + 0x2C, commentLabel);
        st32(okLambda + 0x30, ld32(fa(F_C0)));
        st32(okLambda + 0x34, ld32(fa(F_C0) + 4));
        st32(okLambda + 0x38, ld32(fa(F_C0) + 8));
        st32(okLambda + 0x3C, shadow);
        spareCtrl1 = 0;
        spareCtrl2 = 0;
    }
    st32(fa(F_FL) + 0x10, okLambda);
    cx(Widget_addTouchEventListener, {okButton, fa(F_FL)});
    fnDestroy(fa(F_FL));
    sharedRelease(spareCtrl2);
    sharedRelease(spareCtrl1);

    // ---- open the popup with a copy of the callback (the popup's done handler runs it)
    st32(fa(F_C1), ld32(B + 0xC));
    st32(fa(F_C1) + 4, ld32(B + 0x10));
    st32(fa(F_C1) + 8, ld32(B + 0x14));
    fnCopy(fa(F_D), fa(F_CB));
    const Addr mgr = sm();
    st32(fa(F_EA) + 0x10, fa(F_EA));
    st32(fa(F_EA), VT_popupCondition);
    st32(fa(F_C2), ld32(fa(F_C1)));
    st32(fa(F_C2) + 4, ld32(fa(F_C1) + 4));
    st32(fa(F_C2) + 8, ld32(fa(F_C1) + 8));
    fnCopy(fa(F_H), fa(F_D));
    st32(fa(F_K) + 0x10, 0);
    const Addr doneLambda = cx(OPERATOR_NEW, {0x30});
    if (doneLambda) {
        st32(doneLambda, VT_popupDone);
        st32(doneLambda + 8, ld32(fa(F_C2)));
        st32(doneLambda + 0xC, ld32(fa(F_C2) + 4));
        st32(doneLambda + 0x10, ld32(fa(F_C2) + 8));
        cx(FunctionVoid_move, {doneLambda + 0x18, fa(F_H)});
    }
    st32(fa(F_K) + 0x10, doneLambda);
    cx(SceneManager_openPopup, {mgr, fa(F_EA), fa(F_K), 0});
    fnDestroy(fa(F_K));
    fnDestroy(fa(F_H));
    fnDestroy(fa(F_EA));

    // ---- which notice follows
    const uint32_t noticeWord = ld32(fa(F_W) + 8);
    auto okDialog = [&](Addr textBuf, Addr textTmp, uint32_t textId, Addr fn, Addr vtable) {
        cx(TextData_get, {textBuf, 0, textId});
        const Addr text = ld32(ld32(textBuf) + 4);
        strInit(textTmp, text, strlenOf(text));
        st32(fn + 0x10, 0);
        const Addr lambda = cx(OPERATOR_NEW, {0x18});
        if (lambda) {
            st32(lambda, vtable);
            st32(lambda + 4, me);
            st32(lambda + 8, popup);
            st32(lambda + 0xC, shadow);
            st32(lambda + 0x10, noticeWord);
        }
        st32(fn + 0x10, lambda);
        cx(SceneUtil_openOkDialog4Line, {textTmp, fn, 0});
        fnDestroy(fn);
        strFree(textTmp);
        sharedRelease(ld32(textBuf + 4));
    };
    if (ld8(me + SM_FLAG_B3) == 0 && ld8(me + SM_FLAG_B0) == 0 && ld8(me + SM_FLAG_B1) == 0 &&
        ld8(me + SM_FLAG_B2) == 0) {
        if (ld8(me + SM_FLAG_HAS_MEMBERS) != 0) cx(PartyNewPopup_showNextNotice, {fa(F_W)});
        else cx(SceneManager_closeCurrentPopup, {sm(), 1});
    } else {
        vcx(popup, slot::setVisible, {popup, 0});
        if (ld8(me + SM_FLAG_B0) != 0) {
            // the textual notice 0
            cx(TextData_get, {fa(0xB8), 0, TEXT_NOTICE0});
            const Addr text = ld32(ld32(fa(0xB8)) + 4);
            strInit(fa(0xC0), text, strlenOf(text));
            const Addr fn = fa(0x220);
            st32(fn + 0x10, 0);
            const Addr lambda = cx(OPERATOR_NEW, {0x18});
            if (lambda) {
                st32(lambda, VT_noticeSkill);
                st32(lambda + 4, me);
                st32(lambda + 8, popup);
                st32(lambda + 0xC, shadow);
                st32(lambda + 0x10, noticeWord);
            }
            st32(fn + 0x10, lambda);
            cx(SceneUtil_openOkDialog4Line, {fa(0xC0), fn, 0});
            fnDestroy(fn);
            strFree(fa(0xC0));
            sharedRelease(ld32(fa(0xBC)));
        } else if (ld8(me + SM_FLAG_B1) != 0) {
            // become a party member
            const Addr fn = fa(0x208);
            st32(fn + 0x10, 0);
            const Addr lambda = cx(OPERATOR_NEW, {0x18});
            if (lambda) {
                st32(lambda, VT_noticeBecome);
                st32(lambda + 4, me);
                st32(lambda + 8, popup);
                st32(lambda + 0xC, shadow);
                st32(lambda + 0x10, noticeWord);
            }
            st32(fn + 0x10, lambda);
            cx(SceneManager_openPartyBecomePopup, {lambda, fn});     // r0 is the new lambda (ignored by the callee)
            fnDestroy(fn);
        } else if (ld8(me + SM_FLAG_B3) != 0) {
            okDialog(fa(0xA4), fa(0xAC), TEXT_NOTICE3, fa(0x1F0), VT_noticeB3);
        } else if (ld8(me + SM_FLAG_B2) != 0) {
            okDialog(fa(0x90), fa(0x98), TEXT_NOTICE2, fa(0x1D8), VT_noticeB2);
        } else {
            cx(SceneManager_closeCurrentPopup, {sm(), 1});
        }
    }

    // ---- leave: destroy the copies, release the shared data (every add_ref taken above)
    fnDestroy(fa(F_D));
    sharedRelease(usersCtrl);
    sharedRelease(partyCtrl);
    fnDestroy(fa(F_CB));
    sharedRelease(usersCtrl);
    sharedRelease(partyCtrl);
}

RW_FUNCTION(0x61FC20, "SceneManager::openPartyNewPopup(std::function<void()>)", openPartyNewPopup);
