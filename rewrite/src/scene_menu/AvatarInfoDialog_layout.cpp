// AvatarInfoDialog::createLayout: builds the Player Information page once the profile arrived - copies the
// profile out of the API cache, sets up the four party action buttons (A..D) and the invitation button with their
// texts and touch lambdas, and fills the slide view (two pages, arrows on both sides).
#include "AvatarInfoDialog.hpp"

using namespace scene;
using aid::sbRead;
using aid::S_Empty;

namespace {
namespace c {
constexpr uint32_t STRLEN = 0x40E08C;
constexpr uint32_t MEMCPY = 0x40E164;
constexpr uint32_t BaseSlideView_create = 0x534E79;
constexpr uint32_t BaseSlideView_setItemIDs = 0x5260AD;      // (vector<int> by value, bool)
constexpr uint32_t Director_getVisibleSize = rwlib::cocos2d::Director::getVisibleSize;       // (Size* out, Director*)
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;   // (Node*, function<void()>)
constexpr uint32_t SceneUtil_searchNodeByName = 0x45A9F5;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_setButtonEnabled = 0x468625;    // (Button*, Text*, bool enabled, bool)
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t String_assignCopy = 0x498A1D;             // sub_498A1C: std::string::operator=(const string&)
constexpr uint32_t String_assign = 0x48D811;                 // sub_48D810: std::string::assign(const char*, n)
constexpr uint32_t U64Vector_assign = 0x4D7461;              // vector<unsigned long long>::assign(first, last)
constexpr uint32_t SkillVector_pushBackSlow = 0x487311;      // vector<UserSkill::Skill>::__push_back_slow_path
constexpr uint32_t IntVector_pushBackSlow = 0x4D6CC1;
constexpr uint32_t IntVector_copy = 0x4AF707;
constexpr uint32_t IntVector_dtor = 0x478961;
constexpr uint32_t TimezoneSet_insertUnique = 0x54CE39;      // (pair out, tree, const Timezone&)
constexpr uint32_t TimezoneSet_assignMulti = 0x54CC0D;       // (tree, first, last)
constexpr uint32_t TimezoneSet_destroy = 0x54CD73;           // (tree, node)
constexpr uint32_t Widget_addTouchEventListener = rwlib::cocos2d::ui::Widget::addTouchEventListener;
constexpr uint32_t WidgetFactory_swap = 0x54B1C5;            // function<Widget*(int)>::swap
constexpr uint32_t WidgetUpdater_swap = 0x547159;            // function<Widget*(Widget*, int, bool)>::swap
constexpr uint32_t FunctionVB_swap = 0x490179;               // function<void(bool)>::swap
constexpr uint32_t ImageButton_setTargetAnchorPoint = 0x5318CF;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff;
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
}  // namespace c

// guest string literals
constexpr uint32_t S_UnderUI = 0xDFE20F, S_ButtonA = 0xDFE217, S_ButtonB = 0xDFE220, S_ButtonC = 0xDFE229,
                   S_ButtonD = 0xDFE232, S_ButtonInvitation = 0xDFDC26, S_TxtA = 0xDFE23B, S_TxtB = 0xDFE241,
                   S_TxtC = 0xDFE247, S_TxtD = 0xDFE24D, S_Txt = 0xDF94D1,
                   S_LeftUI = 0xDF85F0, S_RightUI = 0xDF85F7;

// the touch lambdas' __func vtables
constexpr uint32_t VT_OPEN_DONE = 0xF5890C;        // open animation done: (view)
constexpr uint32_t VT_BUTTON_A = 0xF588E0, VT_BUTTON_B = 0xF58830, VT_BUTTON_C = 0xF587D8, VT_BUTTON_D = 0xF587AC;
constexpr uint32_t VT_INVITATION = 0xF586D0;
constexpr uint32_t VT_PAGE_FACTORY = 0xF586A4;     // function<Widget*(int)>: (this)
constexpr uint32_t VT_PAGE_UPDATER = 0xF58494;     // function<Widget*(Widget*, int, bool)>
constexpr uint32_t VT_ON_CLOSE_SUB = 0xF58468;     // function<void(bool)>: (this, view)
constexpr uint32_t VT_LEFT_ARROW = 0xF5843C, VT_RIGHT_ARROW = 0xF58410;

// texts
constexpr int32_t TXT_INVITE = 102100026;
constexpr int32_t TXT_SELF_A = 102100003, TXT_SELF_B = 102100004, TXT_SELF_C = 102100005, TXT_SELF_D = 102100006;
constexpr int32_t TXT_NO_PARTY_D = 103800035, TXT_OTHER_B = 103800038, TXT_OTHER_C_FRIEND = 103800048,
                  TXT_OTHER_C = 103800047, TXT_OTHER_D = 103800046;

// AvatarInfoDialog profile fields (between isOtherUser and pendingPopup)
constexpr uint32_t F_USER = 0x300;          // shared_ptr<UserData> (+0x00 of the API cache)
constexpr uint32_t F_PARTY = 0x310;         // shared_ptr<SafeBuffer> party (+0x08)
constexpr uint32_t F_SKILLS_SB = 0x320;     // shared_ptr<SafeBuffer> skills (+0x20)
constexpr uint32_t F_SKILLS = 0x298;        // vector<UserSkill::Skill>
constexpr uint32_t F_RECORD = 0x330;        // 0x58-byte record (deck / friend)
constexpr uint32_t F_NAME = 0x2CC, F_COMMENT = 0x2D8;
constexpr uint32_t F_PARTY_ID = 0x2E4, F_PARTY_15 = 0x2E8, F_PARTY_18 = 0x2EC, F_PARTY_ROLE = 0x2F0;
constexpr uint32_t F_TIMEZONES = 0x2F4;     // std::set<APIManager::Timezone>
constexpr uint32_t F_FRIEND_IDS = 0x3E8;    // vector<unsigned long long>
constexpr uint32_t F_IS_FRIEND = 0x2CA;

inline uint32_t api() { return call(c::APIManager_getInstance, {}); }

// dst = src for a std::shared_ptr, as inlined: retain the new, store, release the old
void copyShared(uint32_t dst, uint32_t src) {
    const uint32_t obj = ld32(src), ctrl = ld32(src + 4);
    if (ctrl) call(fn::shared_add, {ctrl});
    st32(dst, obj);
    const uint32_t old = ld32(dst + 4);
    st32(dst + 4, ctrl);
    if (old) call(fn::shared_release, {old});
}

uint32_t findChild(uint32_t parent, uint32_t name, uint32_t len) {
    Frame f;
    const uint32_t s = newStr(f, name, len);
    const uint32_t node = call(c::SceneUtil_searchNodeByName, {parent, s});
    strFree(s);
    return node;
}

void setTextLiteral(uint32_t text, uint32_t lit, uint32_t len) {
    Frame f;
    const uint32_t s = newStr(f, lit, len);
    call(fn::Text_setString, {text, s});
    strFree(s);
}

void setTextData(uint32_t text, int32_t id) {
    Frame f;
    const uint32_t td = f.alloc(8);
    call(fn::TextData_get, {td, 0, uint32_t(id)});
    const uint32_t chars = ld32(ld32(td) + 4);
    const uint32_t s = f.alloc(12);
    strInit(s, chars, call(c::STRLEN, {chars}));
    call(fn::Text_setString, {text, s});
    strFree(s);
    sharedRelease(td);
}

void showNode(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }

// one arrow (an ImageButton over the first child of LeftUI / RightUI) that flips the slide view
void addArrow(uint32_t root, uint32_t name, uint32_t len, uint32_t vtable, uint32_t view) {
    const uint32_t ui = findChild(root, name, len);
    const uint32_t target = ld32(ld32(vcall(ui, node::getChildren, {ui})));
    Frame f;
    const uint32_t empty = newStr(f, S_Empty, 0);
    const uint32_t button = call(aid::ImageButton_create, {empty, 1});
    strFree(empty);
    call(aid::ImageButton_setTargetView, {button, target, fbits(1.0f)});
    const uint32_t anchor = f.alloc(8);
    call(c::Vec2_ctor, {anchor, fbits(-0.5f), fbits(-0.5f)});
    call(c::ImageButton_setTargetAnchorPoint, {button, anchor});
    call(c::Vec2_dtor, {anchor});
    vcall(target, node::addChildZ, {target, button, 10});
    const uint32_t onTouch = newFunction(f, vtable, {view});
    call(aid::ImageButton_addTouchEventListener, {button, onTouch});
    functionDestroy(onTouch);
}

// LeftUI / RightUI {view}(sender, type): touch began -> the page sound, the slide view turns ($_10 / $_11)
constexpr uint32_t SE_PAGE = 509000170;
constexpr uint32_t BaseSlideView_changeNextPage = 0x526245, BaseSlideView_changePreviousPage = 0x5263ED;
template <uint32_t Turn> void arrowTouch(uint32_t self, uint32_t, uint32_t typep) {
    if (ld32(typep) != 0) return;
    playLayerSE(SE_PAGE);
    call(Turn, {ld32(self + 4)});
}

// the dialog's sub-popup closed {dialog, view}(-): the cancel sound, the buttons under the page locked, the slide
// view's touch on ($_9)
void subPopupClosed(uint32_t self) {
    constexpr uint32_t AudioPlaySystemSECancel = 0x5C0D81, VSLOT_SLIDE_TOUCH = 0x31C;
    const uint32_t dialog = ld32(self + 4);
    call(AudioPlaySystemSECancel, {});
    AvatarInfoDialog::setTouchEnabledUnderUIButtons(rw::host_ptr<AvatarInfoDialog>(dialog), 0);
    const uint32_t view = ld32(self + 8);
    vcall(view, VSLOT_SLIDE_TOUCH, {view, 1});
}
}  // namespace

void createLayout(AvatarInfoDialog *self, uint32_t showActions) {
    const uint32_t me = rw::guest_addr(self);
    Frame f;
    vcall(self->root, node::setVisible, {self->root, 1});
    const uint32_t view = call(c::BaseSlideView_create, {});
    vcall(view, 0x31C, {view, 1});
    const uint32_t size = f.alloc(8);
    call(c::Director_getVisibleSize, {size, call(fn::Director_getInstance, {})});
    vcall(view, node::setContentSize, {view, size});
    {
        const uint32_t root = self->root;
        const uint32_t done = newFunction(f, VT_OPEN_DONE, {view});
        const uint32_t anim = call(c::SceneUtil_createOpenPopupAnimation, {root, done});
        call(fn::Node_runAction, {root, call(c::EaseSineOut_create, {anim})});
        functionDestroy(done);
    }

    // the profile from the API cache (user, party, ... shared_ptrs, the player's record)
    const uint32_t holder = api() + 0x294;
    copyShared(me + F_USER, ld32(holder) + 0x00);
    copyShared(me + 0x308, ld32(holder) + 0x10);
    copyShared(me + F_PARTY, ld32(holder) + 0x08);
    copyShared(me + 0x318, ld32(holder) + 0x18);
    copyShared(me + F_SKILLS_SB, ld32(holder) + 0x20);
    copyShared(me + 0x328, ld32(holder) + 0x28);
    {
        const uint32_t p = ld32(holder);
        for (uint32_t k = 0; k < 0x10; k += 4) st32(me + 0x388 + k, ld32(p + 0x230 + k));
        for (uint32_t k = 0; k < 0x10; k += 4) st32(me + 0x398 + k, ld32(p + 0x240 + k));
    }
    const uint32_t p = ld32(holder);
    for (uint32_t k = 0; k < 0x10; k += 4) st32(me + 0x3A8 + k, ld32(p + 0x250 + k));
    call(c::String_assignCopy, {me + 0x3B8, p + 0x260});
    st32(me + 0x3C8, ld32(p + 0x270));
    st32(me + 0x3C4, ld32(p + 0x26C));
    call(c::String_assignCopy, {me + 0x3CC, p + 0x274});
    for (uint32_t k = 0; k < 0x10; k += 4) st32(me + 0x3D8 + k, ld32(p + 0x280 + k));
    if (me + 0x3A8 != p + 0x250) call(c::U64Vector_assign, {me + F_FRIEND_IDS, ld32(p + 0x290), ld32(p + 0x294)});
    call(c::MEMCPY, {me + 0x3F4, p + 0x29C, 0x14});
    call(c::String_assignCopy, {me + 0x408, p + 0x2B0});

    const uint32_t record = f.alloc(0x30);       // the shown player's 0x30-byte friend record (another player)
    if (self->isOtherUser) {
        sbRead(ld32(p + 0x28), record, 0, 0x30);
        call(c::MEMCPY, {me + F_RECORD, ld32(holder) + 0x30, 0x58});
        // the skills
        const uint32_t skill = f.alloc(0x28);
        uint32_t sb = ld32(me + F_SKILLS_SB);
        if (ld32(sb + 0x20)) {
            uint32_t off = 0;
            for (uint32_t k = 0; k < ld32(ld32(me + F_SKILLS_SB) + 0x20); k++, off += 0x28) {
                sb = ld32(me + F_SKILLS_SB);
                sbRead(sb, skill, off, 0x28);
                const uint32_t end = ld32(me + F_SKILLS + 4);
                if (end == ld32(me + F_SKILLS + 8)) {
                    call(c::SkillVector_pushBackSlow, {me + F_SKILLS, skill});
                } else {
                    if (end) for (uint32_t w = 0; w < 0x28; w += 4) st32(end + w, ld32(skill + w));
                    st32(me + F_SKILLS + 4, ld32(me + F_SKILLS + 4) + 0x28);
                }
            }
        }
        // is the shown player in the viewer's friend list
        st8(me + F_IS_FRIEND, 0);
        const uint32_t begin = ld32(me + F_FRIEND_IDS), endIds = ld32(me + F_FRIEND_IDS + 4);
        if (endIds != begin) {
            uint32_t n = 0;
            for (uint32_t e = begin;; e += 8) {
                n++;
                if (ld32(e) == ld32(me + 0x2C0) && ld32(e + 4) == ld32(me + 0x2C4)) {
                    st8(me + F_IS_FRIEND, 1);
                    break;
                }
                if (n >= uint32_t(int32_t(endIds - begin) >> 3)) break;
            }
        }
    } else {
        // the viewer: the friend record whose id matches the party's, and the first deck flagged in use
        const uint32_t tmp = f.alloc(0x30);
        sbRead(ld32(me + F_PARTY), tmp, 0x3C, 4);
        const uint32_t id = ld32(tmp);
        const uint32_t friends = api() + 0xC4;
        uint32_t off = 0;
        for (uint32_t k = 0; k < ld32(ld32(friends) + 0x20); k++, off += 0x30) {
            sbRead(ld32(friends), tmp, off, 0x30);
            if (ld32(tmp) == id) for (uint32_t w = 0; w < 0x30; w += 4) rw::frame_ptr<uint32_t>(record)[w / 4] = ld32(tmp + w);
        }
        const uint32_t decks = api() + 0xEC;
        const uint32_t n = ld32(ld32(decks) + 0x20);
        const uint32_t deck = f.alloc(0x58);
        off = 0;
        for (uint32_t k = 0; k < n; k++, off += 0x58) {
            sbRead(ld32(decks), deck, off, 0x58);
            if (ld8(deck + 0x4C)) {
                call(c::MEMCPY, {me + F_RECORD, deck, 0x58});
                break;
            }
        }
    }

    // name, comment, party fields
    {
        const uint32_t name = f.alloc(0x24);
        sbRead(ld32(me + F_USER), name, 0x14, 0x21);
        call(c::String_assign, {me + F_NAME, name, call(c::STRLEN, {name})});
        const uint32_t comment = f.alloc(0x104);
        sbRead(ld32(me + F_USER), comment, 0x3C, 0x101);
        call(c::String_assign, {me + F_COMMENT, comment, call(c::STRLEN, {comment})});
        const uint32_t v = f.alloc(4);
        sbRead(ld32(me + F_PARTY), v, 0x10, 4);
        st32(me + F_PARTY_ID, ld32(v));
        sbRead(ld32(me + F_PARTY), v, 0x14, 4);
        st32(me + F_PARTY_15, ld32(v));
        sbRead(ld32(me + F_PARTY), v, 0x18, 4);
        st32(me + F_PARTY_18, ld32(v));
        sbRead(ld32(me + F_PARTY), v, 0x24, 4);
        if (ld32(v) <= 3) st32(me + F_PARTY_ROLE, ld32(v));
    }

    // the party's active time zones (a std::set<Timezone> built locally, then assigned)
    {
        const uint32_t tree = f.alloc(12);          // begin node, root (the end node's left), size
        st32(tree + 4, 0);
        st32(tree + 8, 0);
        st32(tree, tree + 4);
        const uint32_t zones = f.alloc(0x18), count = f.alloc(4), value = f.alloc(4), result = f.alloc(8);
        sbRead(ld32(me + F_PARTY), zones, 0x58, 0x18);
        sbRead(ld32(me + F_PARTY), count, 0x20, 4);
        const int32_t n = int32_t(ld32(count));
        for (int32_t k = 0; k < n; k++)
            for (uint32_t z = 1; z <= 6; z++)
                if (ld32(zones + 4 * uint32_t(k)) == z) {
                    st32(value, z);
                    call(c::TimezoneSet_insertUnique, {result, tree, value});
                }
        if (me + F_TIMEZONES != tree) call(c::TimezoneSet_assignMulti, {me + F_TIMEZONES, ld32(tree), tree + 4});

        // the party action buttons and their touch lambdas
        const uint32_t underUi = findChild(self->root, S_UnderUI, 7);
        const uint32_t a = findChild(underUi, S_ButtonA, 8);
        const uint32_t b = findChild(underUi, S_ButtonB, 8);
        const uint32_t cc = findChild(underUi, S_ButtonC, 8);
        const uint32_t d = findChild(underUi, S_ButtonD, 8);
        const uint32_t inv = findChild(underUi, S_ButtonInvitation, 0x11);
        const uint32_t fo = f.alloc(20);
        heapLambda(fo, 0x38, VT_BUTTON_A, {me, a, b, cc, d, inv, a, b, cc, d, inv, me, view});
        call(c::Widget_addTouchEventListener, {a, fo});
        functionDestroy(fo);
        heapLambda(fo, 0x54, VT_BUTTON_B, {a, b, cc, d, inv, me, a, b, cc, d, inv, me, view, a, b, cc, d, inv, me, view});
        call(c::Widget_addTouchEventListener, {b, fo});
        functionDestroy(fo);
        heapLambda(fo, 0x54, VT_BUTTON_C, {a, b, cc, d, inv, me, a, b, cc, d, inv, me, view, a, b, cc, d, inv, me, view});
        call(c::Widget_addTouchEventListener, {cc, fo});
        functionDestroy(fo);
        heapLambda(fo, 0x38, VT_BUTTON_D, {a, b, cc, d, inv, me, a, b, cc, d, inv, me, view});
        call(c::Widget_addTouchEventListener, {d, fo});
        functionDestroy(fo);

        const uint32_t txtA = findChild(underUi, S_TxtA, 5);
        const uint32_t txtB = findChild(underUi, S_TxtB, 5);
        const uint32_t txtC = findChild(underUi, S_TxtC, 5);
        const uint32_t txtD = findChild(underUi, S_TxtD, 5);
        const uint32_t txtInv = findChild(inv, S_Txt, 3);
        for (uint32_t t : {txtA, txtB, txtC, txtD, txtInv}) call(c::SceneUtil_setShadow, {t});
        setTextData(txtInv, TXT_INVITE);

        bool keepButtons = showActions != 0;
        if (self->isOtherUser) {
            // the viewer's party settings, the shown player's party, the viewer's own party id
            const uint32_t viewer = api();
            const uint32_t settings = api() + 0x84;
            uint32_t invited = 0, flag2 = 0;
            if (ld32(settings)) {
                const uint32_t b1 = f.alloc(4);
                sbRead(ld32(settings), b1, 0x11, 1);
                const uint32_t v1 = ld8(b1);
                sbRead(ld32(settings), b1 + 1, 0x12, 1);
                invited = v1 != 0;
                flag2 = ld8(b1 + 1) != 0;
            }
            const uint32_t q = f.alloc(0x10);
            sbRead(ld32(me + F_PARTY), q, 0x28, 8);
            const uint32_t partyLo = ld32(q), partyHi = ld32(q + 4);
            sbRead(ld32(me + F_PARTY), q + 8, 0x28, 8);
            const uint32_t shownLo = ld32(q + 8), shownHi = ld32(q + 12);
            sbRead(ld32(settings), q, 8, 8);
            const uint32_t ownLo = ld32(q), ownHi = ld32(q + 4);
            // (no party buffer at all counts as a different kind too)
            uint32_t samePartyKind = 0;
            if (ld32(viewer + 0x94)) {
                sbRead(ld32(me + F_PARTY), q, 0x30, 4);
                const uint32_t mine = ld32(q);
                sbRead(ld32(viewer + 0x94), q + 4, 0x30, 4);
                samePartyKind = mine == ld32(q + 4);
            }
            if (!samePartyKind) keepButtons = false;
            const bool sameParty = shownLo == ownLo && shownHi == ownHi;
            if (!(partyLo | partyHi)) {
                // the shown player has no party
                showNode(a, 0);
                showNode(b, 0);
                showNode(cc, 0);
                showNode(d, 0);
                setTextLiteral(txtA, S_Empty, 0);
                setTextLiteral(txtB, S_Empty, 0);
                setTextLiteral(txtC, S_Empty, 0);
                setTextData(txtD, TXT_NO_PARTY_D);
                if (!keepButtons || sameParty) keepButtons = false;
            } else {
                showNode(a, 0);
                showNode(b, invited);
                showNode(cc, flag2 | invited);
                showNode(d, flag2 | invited);
                setTextLiteral(txtA, S_Empty, 0);
                setTextData(txtB, TXT_OTHER_B);
                setTextData(txtC, ld8(me + F_IS_FRIEND) ? TXT_OTHER_C_FRIEND : TXT_OTHER_C);
                setTextData(txtD, TXT_OTHER_D);
                if (!sameParty) keepButtons = false;
            }
        } else {
            setTextData(txtA, TXT_SELF_A);
            setTextData(txtB, TXT_SELF_B);
            setTextData(txtC, TXT_SELF_C);
            setTextData(txtD, TXT_SELF_D);
        }
        if (!keepButtons) {
            showNode(a, 0);
            showNode(b, 0);
            showNode(cc, 0);
            showNode(d, 0);
        }

        // the invitation button: only on another player's page with no action button shown
        if (!self->isOtherUser || vcall(a, node::isVisible, {a}) || vcall(b, node::isVisible, {b}) ||
            vcall(cc, node::isVisible, {cc}) || vcall(d, node::isVisible, {d})) {
            showNode(inv, 0);
            vcall(inv, 0x29C, {inv, 0});                // ui::Widget::setTouchEnabled(false)
        } else {
            const uint32_t a1 = api();
            const uint32_t settingsCtrl = ld32(a1 + 0x88), settings = ld32(a1 + 0x84);
            if (settingsCtrl) call(fn::shared_add, {settingsCtrl});
            const uint32_t a2 = api();
            const uint32_t partyCtrl = ld32(a2 + 0x200), party = ld32(a2 + 0x1FC);
            if (partyCtrl) call(fn::shared_add, {partyCtrl});
            const uint32_t canInvite = f.alloc(4);
            sbRead(settings, canInvite, 0x10, 1);
            if (party && ld8(canInvite)) call(c::SceneUtil_setButtonEnabled, {inv, txtInv, 1, 0});
            else call(c::SceneUtil_setButtonEnabled, {inv, txtInv, 0, 0});
            heapLambda(fo, 0x38, VT_INVITATION, {a, b, cc, d, inv, me, a, b, cc, d, inv, me, view});
            call(c::Widget_addTouchEventListener, {inv, fo});
            functionDestroy(fo);
            if (partyCtrl) call(fn::shared_release, {partyCtrl});
            if (settingsCtrl) call(fn::shared_release, {settingsCtrl});
        }

        // the slide view's page factory and page updater, the dialog's sub-popup close callback
        const uint32_t factory = newFunction(f, VT_PAGE_FACTORY, {me});
        call(c::WidgetFactory_swap, {factory, view + 0x500});
        functionDestroy(factory);
        const uint32_t updater = f.alloc(20);
        heapLambda(updater, 0x5C, VT_PAGE_UPDATER, {view, me, view, a, b, cc, d, inv, me, view});
        if (const uint32_t obj = ld32(updater + 16))
            for (uint32_t w = 0; w < 0x30; w += 4) st32(obj + 0x2C + w, ld32(record + w));
        call(c::WidgetUpdater_swap, {updater, view + 0x518});
        functionDestroy(updater);
        const uint32_t onClose = newFunction(f, VT_ON_CLOSE_SUB, {me, view});
        call(c::FunctionVB_swap, {onClose, me + 0x458});
        functionDestroy(onClose);

        // two pages: item ids 0 and 1 (pushed one by one, as the original's loop does)
        const uint32_t ids = f.alloc(16);          // value, begin, end, capacity
        for (uint32_t k = 0; k < 16; k += 4) st32(ids + k, 0);
        for (uint32_t page = 1;; page++) {
            const uint32_t end = ld32(ids + 8);
            if (end == ld32(ids + 12)) {
                call(c::IntVector_pushBackSlow, {ids + 4, ids});
            } else {
                uint32_t e = 0;
                if (end) {
                    st32(end, page - 1);
                    e = ld32(ids + 8);
                }
                st32(ids + 8, e + 4);
            }
            st32(ids, page);
            if (int32_t(page) > 1) break;
        }
        const uint32_t copy = f.alloc(12);
        call(c::IntVector_copy, {copy, ids + 4});
        call(c::BaseSlideView_setItemIDs, {view, copy, 0});
        call(c::IntVector_dtor, {copy});
        vcall(self->root, node::addChild, {self->root, view});
        addArrow(self->root, S_LeftUI, 6, VT_LEFT_ARROW, view);
        addArrow(self->root, S_RightUI, 7, VT_RIGHT_ARROW, view);
        call(c::IntVector_dtor, {ids + 4});
        call(c::TimezoneSet_destroy, {tree, ld32(tree + 4)});
    }
}

RW_FUNCTION(0x5332EC, "AvatarInfoDialog::createLayout", createLayout);
RW_FUNCTION(0x546EA8, "AvatarInfoDialog::createLayout::$_11 operator() (RightUI)", arrowTouch<BaseSlideView_changeNextPage>);
RW_FUNCTION(0x546FD0, "AvatarInfoDialog::createLayout::$_10 operator() (LeftUI)", arrowTouch<BaseSlideView_changePreviousPage>);
RW_FUNCTION(0x5470FE, "AvatarInfoDialog::createLayout::$_9 operator() (sub-popup closed)", subPopupClosed);
