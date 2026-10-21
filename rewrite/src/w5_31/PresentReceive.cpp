// Presents > Collect (one row): PresentBoxScrollView::changeRow $_0 (the row's Collect button: /user/present/
// receive with the present's id, or the medal-box-full dialog for a medal), popUpGetPresent ("Received <name>
// (x n)", or "could not be received"), its dialog lambda $_4 (the row removed, the refresh callback, onDone) and
// removeItem.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t PresentMap_insertHint = 0x5105FD;        // __tree<ull, Present>::__insert_unique(hint, const pair&)
constexpr uint32_t PresentMap_at = 0x50DC75;                // map<ull, Present>::operator[](const ull&)
constexpr uint32_t PresentMap_destroy = 0x4B5381;           // __tree<ull, Present>::destroy(node)
constexpr uint32_t SceneUtil_setItemImageData = 0x46C71D;   // (string& name, string& image, type, id, bool)
constexpr uint32_t SceneUtil_openOkDialog4Line = 0x45C775;  // (const string&, function<void(bool)>, const char*)
constexpr uint32_t SceneUtil_isOverMedalSlot = 0x46A085;
constexpr uint32_t SceneUtil_openMedalOverDialog = 0x460251; // (int, int, const function&)
constexpr uint32_t StringBuffer_ctor = 0x49140D;
constexpr uint32_t FunctionB_call = 0x51022D;               // std::function<bool()>::operator()
constexpr uint32_t SM_enableLoadingIndicator = 0x60D169;
constexpr uint32_t APIManager_executeUserPresentReceive = 0x4CD081;  // (api, const unordered_set<ull>&, fn<void(bool)>)
constexpr uint32_t U64Set_insertUnique = 0x482649, U64Set_dtor = 0x482619;
constexpr uint32_t U64Vector_pushBackSlow = 0x4ECB6D, U64Vector_dtor = 0x468499;
constexpr uint32_t BaseScrollViewULL_setItemIDs = 0x4FD231;
constexpr uint32_t IScrollProtocol_resetScroll = 0x4F73ED;
constexpr uint32_t STRLEN = 0x40E08C;
}  // namespace g

constexpr uint32_t APIMANAGER_PRESENTS = 0x104;   // map<unsigned long long, UserPresent::Present>*
// UserPresent::Present (a map value): 7 words (+8 item type, +0xC item id, +0x10 count), +0x1C text (string),
// +0x28 received Date (8 bytes) and +0x30 a byte: the present can be received
constexpr uint32_t PRESENT_TYPE = 0x08, PRESENT_ITEM = 0x0C, PRESENT_COUNT = 0x10, PRESENT_TEXT = 0x1C,
                   PRESENT_RECEIVED = 0x28, PRESENT_OK = 0x30, PRESENT_SIZE = 0x34;
// PresentBoxScrollView
constexpr uint32_t F_IDS = 0x628;                 // vector<ull>
constexpr uint32_t F_CAN_TOUCH = 0x668;           // std::function<bool()>
constexpr uint32_t F_ON_REFRESH = 0x698;          // std::function<void()>
constexpr uint32_t ITEM_MEDAL = 3;
constexpr uint32_t VT_GotDialog = 0xF56368;       // popUpGetPresent $_4 {this, -, id, onDone} (heap 0x30)
constexpr uint32_t VT_Received = 0xF56470;        // changeRow $_0's request callback {this, -, id} (heap 0x18)
constexpr uint32_t VT_MedalOver = 0xF564C8;       // changeRow $_0's medal-box-full dialog callback {this}
constexpr int32_t TXT_GOT_ONE = 103500015, TXT_GOT_N = 103500016, TXT_NOT_RECEIVED = 103500017;
constexpr uint32_t S_Empty = 0xDF8A1A;

void copyString(uint32_t dst, uint32_t src) {
    if (ld8(src) & 1) {
        strInit(dst, ld32(src + 8), ld32(src + 4));
    } else {
        for (uint32_t k = 0; k < 12; k += 4) st32(dst + k, ld32(src + k));
    }
}
}  // namespace

struct PresentReceive {
    // "Received <name>" (x n), or "could not be received"; the dialog's lambda carries {this, id, onDone}.
    // Kept in the original frame (0x948): the lambda's second word is the never-set stack word at +0xCC.
    static uint32_t popUpGetPresent(uint32_t self, uint32_t, uint32_t lo, uint32_t hi, uint32_t onDone) {
        constexpr uint32_t EPILOGUE_R0 = 0x934;
        guest::Frame fr(0x948);
        const uint32_t key = fr.addr(0x88);
        st32(key, lo);
        st32(key + 4, hi);
        Frame f;
        // a copy of the user's present map
        const uint32_t userMap = ld32(call(fn::APIManager_getInstance, {}) + APIMANAGER_PRESENTS);
        const uint32_t local = f.alloc(12);
        st32(local + 4, 0);
        st32(local + 8, 0);
        st32(local, local + 4);
        for (uint32_t node = ld32(userMap); node != userMap + 4;) {
            call(g::PresentMap_insertHint, {local, local + 4, node + 0x10});
            uint32_t r = ld32(node + 4);
            if (r) {
                node = r;
                while (ld32(node)) node = ld32(node);
            } else {
                uint32_t parent;
                while (ld32(parent = ld32(node + 8)) != node) node = parent;
                node = parent;
            }
        }
        const uint32_t entry = call(g::PresentMap_at, {local, key});
        const uint32_t present = f.alloc(PRESENT_SIZE);
        for (uint32_t k = 0; k < PRESENT_TEXT; k += 4) st32(present + k, ld32(entry + k));
        copyString(present + PRESENT_TEXT, entry + PRESENT_TEXT);
        for (uint32_t k = 0; k < 9; k++) st8(present + PRESENT_RECEIVED + k, ld8(entry + PRESENT_RECEIVED + k));
        const uint32_t name = f.alloc(12), image = f.alloc(12);
        for (uint32_t k = 0; k < 12; k += 4) {
            st32(name + k, 0);
            st32(image + k, 0);
        }
        call(g::SceneUtil_setItemImageData, {name, image, ld32(present + PRESENT_TYPE), ld32(present + PRESENT_ITEM), 0});
        if (ld8(present + PRESENT_OK)) {
            const uint32_t message = newStr(f, S_Empty, 0);
            const int32_t count = int32_t(ld32(present + PRESENT_COUNT));
            {
                const uint32_t td = textData(f, count > 1 ? TXT_GOT_N : TXT_GOT_ONE);
                const uint32_t buf = f.alloc(0x404);
                if (count > 1) call(g::StringBuffer_ctor, {buf, textDataStr(td), strData(name), uint32_t(count)});
                else call(g::StringBuffer_ctor, {buf, textDataStr(td), strData(name)});
                call(fn::string_assign, {message, buf, call(g::STRLEN, {buf})});
                sharedRelease(td);
            }
            const uint32_t text = newStr(f, strData(message));
            const uint32_t captures = fr.addr(0xC8);          // {this, (unset), id}
            st32(captures, self);
            st32(captures + 8, lo);
            st32(captures + 12, hi);
            const uint32_t done = fr.addr(0xD8);
            call(fn::function_vv_copy, {done, onDone});
            const uint32_t cb = fr.addr(0xF0);
            const uint32_t obj = heapFunction(cb, VT_GotDialog, 0x30, 0x18, fn::function_vv_move, done);
            if (obj)
                for (uint32_t k = 0; k < 16; k += 4) st32(obj + 8 + k, ld32(captures + k));
            call(g::SceneUtil_openOkDialog4Line, {text, cb, 0});
            functionDestroy(cb);
            functionDestroy(done);
            strFree(text);
            strFree(message);
        } else {
            const uint32_t td = textData(f, TXT_NOT_RECEIVED);
            const uint32_t text = newStr(f, textDataStr(td));
            const uint32_t none = newEmptyFunction(f);
            call(g::SceneUtil_openOkDialog4Line, {text, none, 0});
            functionDestroy(none);
            strFree(text);
            sharedRelease(td);
        }
        strFree(name);
        strFree(image);
        strFree(present + PRESENT_TEXT);
        call(g::PresentMap_destroy, {local, ld32(local + 4)});
        return EPILOGUE_R0;
    }

    // the list without present `id` (the old ids cleared as vector::clear compiles), scrolled back
    static void removeItem(uint32_t self, uint32_t, uint32_t lo, uint32_t hi) {
        Frame f;
        const uint32_t ids = f.alloc(12);
        st32(ids, 0);
        st32(ids + 4, 0);
        st32(ids + 8, 0);
        uint32_t begin = ld32(self + F_IDS);
        if (ld32(self + F_IDS + 4) != begin) {
            uint32_t end = 0;
            for (uint32_t i = 0; i < (ld32(self + F_IDS + 4) - ld32(self + F_IDS)) >> 3; i++) {
                const uint32_t at = begin + 8 * i;
                const uint32_t idLo = ld32(at), idHi = ld32(at + 4);
                if (idLo != lo || idHi != hi) {
                    const uint32_t tail = ld32(ids + 4);
                    if (tail == ld32(ids + 8)) {
                        call(g::U64Vector_pushBackSlow, {ids, at});
                    } else {
                        uint32_t next = 0;
                        if (tail) {
                            st32(tail, idLo);
                            st32(tail + 4, idHi);
                            next = ld32(ids + 4);
                        }
                        st32(ids + 4, next + 8);
                    }
                }
                begin = ld32(self + F_IDS);
                end = ld32(self + F_IDS + 4);
            }
            if (end != begin) st32(self + F_IDS + 4, end + (~(end - 8 - begin) & ~7u));
        }
        call(g::BaseScrollViewULL_setItemIDs, {self, ids});
        call(g::IScrollProtocol_resetScroll, {self});
        call(g::U64Vector_dtor, {ids});
    }

    // $_4 (bool ok): the row removed, the list's refresh callback, then onDone
    static void gotDialogClosed(uint32_t functor, uint32_t okRef) {
        if (!ld8(okRef)) return;
        const uint32_t self = ld32(functor + 8);
        removeItem(self, 0, ld32(functor + 16), ld32(functor + 20));
        call(g::IScrollProtocol_resetScroll, {self});
        if (ld32(self + F_ON_REFRESH + 16)) call(fn::function_vv_call, {self + F_ON_REFRESH});
        if (ld32(functor + 0x18 + 16)) call(fn::function_vv_call, {functor + 0x18});
    }

    // changeRow $_0 {-, button, this, -, -, -, item type, ..., present id at +0x48}: Collect - a medal with the
    // medal box full: the box-full dialog; else /user/present/receive with the id
    static void collectTouched(uint32_t functor, uint32_t /*sender*/, uint32_t typeRef) {
        if (ld32(typeRef) != 2) return;
        const uint32_t button = ld32(functor + 8), self = ld32(functor + 12);
        if (button && vcall(button, node::isVisible, {button}) != 1) return;
        if (ld32(self + F_CAN_TOUCH + 16) && call(g::FunctionB_call, {self + F_CAN_TOUCH}) != 1) return;
        Frame f;
        if (ld32(functor + 24) == ITEM_MEDAL && call(g::SceneUtil_isOverMedalSlot, {1, self}) == 1) {
            const uint32_t cb = newFunction(f, VT_MedalOver, {self});
            call(g::SceneUtil_openMedalOverDialog, {1, 4, cb});
            functionDestroy(cb);
            return;
        }
        const uint32_t lo = ld32(functor + 0x48), hi = ld32(functor + 0x4C);
        const uint32_t ids = f.alloc(20);                 // unordered_set<ull>: buckets, count, first, size, 1.0f
        for (uint32_t k = 0; k < 16; k += 4) st32(ids + k, 0);
        stf(ids + 16, 1.0f);
        const uint32_t inserted = f.alloc(8);
        call(g::U64Set_insertUnique, {inserted, ids, functor + 0x48});
        call(g::SM_enableLoadingIndicator, {sceneManager(), 1});
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t cb = f.alloc(20);
        st32(cb + 16, 0);
        const uint32_t obj = call(fn::operator_new, {0x18});
        if (obj) {
            st32(obj, VT_Received);
            st32(obj + 8, self);
            st32(obj + 12, 0);
            st32(obj + 16, lo);
            st32(obj + 20, hi);
        }
        st32(cb + 16, obj);
        call(g::APIManager_executeUserPresentReceive, {api, ids, cb});
        functionDestroy(cb);
        call(g::U64Set_dtor, {ids});
    }
};

RW_FUNCTION(0x50DEE8, "PresentBoxScrollView::popUpGetPresent", PresentReceive::popUpGetPresent);
RW_FUNCTION(0x50E664, "PresentBoxScrollView::removeItem", PresentReceive::removeItem);
RW_FUNCTION(0x50F6C0, "PresentBoxScrollView::popUpGetPresent::$_4 operator()", PresentReceive::gotDialogClosed);
RW_FUNCTION(0x510044, "PresentBoxScrollView::changeRow::$_0 operator() (Collect)", PresentReceive::collectTouched);
