// StartDeckEditDialog: the support character list.
#include "StartDeckEditDialog_lib.hpp"

using namespace sded;

// (sub_5765C4) the support user `index` in the "Plus" panel (SceneUtil::changePartnerAvatarPanel)
void StartDeckEditDialog::showSupportUser(Addr captures, int32_t index) {
    uint32_t self = ld32(captures);
    uint32_t sb = ld32(api() + API_SUPPORTS);
    uint32_t user[74];
    safeWords(sb, uint32_t(index) * 0x128, user, 74);
    uint32_t plus = find(S(self)->root, S_PLUS, 4);
    uint32_t avatar = find(plus, S_AVATAR, 6);
    vcall(plus, VS_setTouchEnabled, {plus, 1});
    vcall(avatar, VS_setTouchEnabled, {avatar, 1});
    // (Widget*, StageSupportList::SupportUser by value (8-aligned: r2:r3 + 0x120 stack bytes), AvatarIconCache*)
    uint32_t cache = S(self)->avatarIconCache;
    call(SceneUtil_changePartnerAvatarPanel,
         {plus, 0, user[0], user[1], user[2], user[3], user[4], user[5], user[6], user[7], user[8], user[9], user[10],
          user[11], user[12], user[13], user[14], user[15], user[16], user[17], user[18], user[19], user[20], user[21],
          user[22], user[23], user[24], user[25], user[26], user[27], user[28], user[29], user[30], user[31], user[32],
          user[33], user[34], user[35], user[36], user[37], user[38], user[39], user[40], user[41], user[42], user[43],
          user[44], user[45], user[46], user[47], user[48], user[49], user[50], user[51], user[52], user[53], user[54],
          user[55], user[56], user[57], user[58], user[59], user[60], user[61], user[62], user[63], user[64], user[65],
          user[66], user[67], user[68], user[69], user[70], user[71], user[72], user[73], cache});
}
RW_FUNCTION(0x5765C4, "StartDeckEditDialog::initSupport::showSupportUser (sub_5765C4)", StartDeckEditDialog::showSupportUser);

// Switches the dialog to the support list: deck paging off, the list (created once) filled with the support
// users, the current support selected ($_15 / $_16 handle selection), Button_Close enabled after the tutorial.
void StartDeckEditDialog::initSupport(Addr self) {
    Self *s = S(self);
    call(BaseSlideView_setSlideEnabled, {s->deckSlide, 0});
    vcall(s->leftUI, VS_setVisible, {s->leftUI, 0});
    vcall(s->rightUI, VS_setVisible, {s->rightUI, 0});
    uint32_t panelKB = find(ld32(s->deckSlide + 0x588), S_PANEL_KB, 8);
    vcall(panelKB, VS_setVisible, {panelKB, 0});
    Frame f;
    uint32_t caps = f.alloc(4);
    st32(caps, self);
    uint32_t support = find(s->root, S_SUPPORT, 7);
    uint32_t area = find(support, S_AREA, 4);
    uint32_t z = vcall(area, VS_getLocalZOrder, {area});
    vcall(area, VS_setZOrder10, {area, z + 2});
    uint32_t ids = f.alloc(12);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    uint32_t a = api();
    uint32_t listField = a + API_SUPPORTS;
    uint32_t count = ld32(ld32(listField) + 0x20);
    uint32_t selected = 0;
    if (count) {
        uint32_t off = 0, sel = 0, i = 0;
        uint32_t tmp = f.alloc(8);
        do {
            if (i == 0) {
                uint32_t first[2];
                safeWords(ld32(listField), 0, first, 2);
                uint32_t sm = sceneManager();
                st32(sm + SM_SUPPORT_ID + 4, first[1]);
                st32(sm + SM_SUPPORT_ID, first[0]);
                showSupportUser(caps, 0);
            }
            uint32_t id[2];
            safeWords(ld32(listField), off, id, 2);
            st32(tmp, id[0]);
            st32(tmp + 4, id[1]);
            uint32_t end = ld32(ids + 4);
            if (end == ld32(ids + 8)) {
                call(U64Vector_pushBackSlow, {ids, tmp});
            } else {
                uint32_t v = 0;
                if (end) {
                    st32(end, id[0]);
                    st32(end + 4, id[1]);
                    v = ld32(ids + 4);
                }
                st32(ids + 4, v + 8);
            }
            uint32_t sm = sceneManager();
            uint32_t diff = (id[1] ^ ld32(sm + SM_SUPPORT_ID + 4)) | (id[0] ^ ld32(sm + SM_SUPPORT_ID));
            uint32_t r = diff ? sel : i;
            off += 0x128;
            i++;
            sel = r;
        } while (i < count);
        selected = sel;
    }
    uint32_t plus = find(s->root, S_PLUS, 4);
    uint32_t first = ld32(ld32(vcall(plus, VS_getChildren, {plus})));
    uint32_t firstX = vcall(first, VS_getPositionX, {first});
    uint32_t scroll = s->supportScroll;
    uint32_t existing = scroll;
    if (!scroll) {
        s->supportScroll = call(SupportScroll_create, {});
        uint32_t a2 = api();
        uint32_t slide = s->deckSlide;
        uint32_t idsB = ld32(slide + 0x578), idsE = ld32(slide + 0x57C);
        uint32_t idx = ld32(slide + 0x5AC);
        uint32_t kbList = ld32(a2 + API_KEYBLADES);
        if (!(uint32_t(int32_t(idsE - idsB) >> 2) > idx)) {
            call(wl::VECTOR_AT_THROW, {slide + 0x578});
            return;
        }
        uint32_t w[4];
        safeWords(kbList, ld32(idsB + idx * 4) * 0x58, w, 4);
        st32(s->supportScroll + 0x7B8, w[3]);
        scroll = s->supportScroll;
    }
    vcall(scroll, VS_setContentSize, {scroll, vcall(area, VS_getContentSize, {area})});
    call(IScrollProtocol_setIndex, {s->supportScroll, selected});
    call(BaseScrollViewU64_setItemIDs, {s->supportScroll, ids});
    {
        uint32_t sv = s->supportScroll;
        uint32_t sm = sceneManager();
        call(SupportScroll_setSelectedID, {sv, SM_SUPPORT_ID, ld32(sm + SM_SUPPORT_ID), ld32(sm + SM_SUPPORT_ID + 4)});
    }
    {
        uint32_t sv = s->supportScroll;
        Frame g;
        uint32_t fo = g.alloc(24);
        st32(fo + 16, 0);
        uint32_t heap = call(fn::operator_new, {0x14});
        if (heap) {
            st32(heap, VT_support_15);
            st32(heap + 4, first);
            st32(heap + 8, firstX);
            st32(heap + 0xC, self);
            st32(heap + 0x10, self);
        }
        st32(fo + 16, heap);
        call(FunctionVI_swap, {fo, sv + 0x638});
        functionDestroy(fo);
    }
    {
        uint32_t sv = s->supportScroll;
        Frame g;
        uint32_t fo = inlineFn(g, VT_support_16, {self});
        call(FunctionVI_swap, {fo, sv + 0x650});
        functionDestroy(fo);
    }
    if (!existing) vcall(area, VS_addChild, {area, s->supportScroll});
    uint32_t close = find(support, S_BUTTON_CLOSE, 12);
    int32_t phase = int32_t(call(SceneUtil_getNexTutorialphase, {}));
    vcall(close, VS_setTouchEnabled, {close, phase > 994 ? 1u : 0u});
    {
        Frame g;
        uint32_t fo = inlineFn(g, VT_support_17, {self, close});
        call(scene::fn::Widget_addTouchEventListener, {close, fo});
        functionDestroy(fo);
    }
    call(U64Vector_baseDtor, {ids});
}
RW_FUNCTION(0x576018, "StartDeckEditDialog::initSupport", StartDeckEditDialog::initSupport);

// The support list and the "Plus" panel slide in from the right; the deck arrows fade out; the dialog close button
// is off while the list is open (a popup layer: $_19 / $_20).
void StartDeckEditDialog::supportScrollRunInAnimation(Addr self) {
    Self *s = S(self);
    s->supportOpen = 1;
    uint32_t support = find(s->root, S_SUPPORT, 7);
    vcall(support, VS_setVisible, {support, 1});
    {
        Frame f;
        uint32_t empty = newEmptyFunction(f);
        uint32_t anim = call(SceneUtil_createRightInAnimation, {support, 0, 0, empty});
        call(wl::Node_runAction, {support, call(EaseSineOut_create, {anim})});
        functionDestroy(empty);
    }
    uint32_t win = find(support, S_SUPPORT_WIN, 11);
    vcall(win, VS_setTouchEnabled, {win, 1});
    uint32_t plus = find(s->root, S_PLUS, 4);
    vcall(plus, VS_setVisible, {plus, 1});
    {
        Frame f;
        uint32_t empty = newEmptyFunction(f);
        uint32_t anim = call(SceneUtil_createRightInAnimation, {plus, 0, 0, empty});
        call(wl::Node_runAction, {plus, call(EaseSineOut_create, {anim})});
        functionDestroy(empty);
    }
    uint32_t close = find(support, S_BUTTON_CLOSE, 12);
    uint32_t left = s->leftUI;
    call(wl::Node_runAction, {left, call(FadeOut_create, {fb(0.1f)})});
    uint32_t right = s->rightUI;
    call(wl::Node_runAction, {right, call(FadeOut_create, {fb(0.1f)})});
    vcall(s->closeButton, VS_setTouchEnabled, {s->closeButton, 0});
    vcall(s->closeButton, VS_setVisible, {s->closeButton, 0});
    uint32_t sm = sceneManager();
    Frame f;
    uint32_t canClose = inlineFn(f, VT_runIn_19, {close});
    uint32_t onClose = inlineFn(f, VT_runIn_20, {self});
    call(SceneManager_openPopup, {sm, canClose, onClose, 1});
    functionDestroy(onClose);
    functionDestroy(canClose);
}
RW_FUNCTION(0x57684C, "StartDeckEditDialog::supportScrollRunInAnimation", StartDeckEditDialog::supportScrollRunInAnimation);

namespace {
// a heap lambda {vtable, node, Vec2 pos (copied), extra word at +0x10 when size is 0x14}
uint32_t runOutLambda(uint32_t fo, uint32_t vtable, uint32_t size, uint32_t node, uint32_t pos, uint32_t extra) {
    st32(fo + 16, 0);
    uint32_t heap = call(fn::operator_new, {size});
    if (heap) {
        st32(heap, vtable);
        st32(heap + 4, node);
        call(scene::fn::Vec2_ctor_copy, {heap + 8, pos});
        if (size == 0x14) st32(heap + 0x10, extra);
        else st32(heap + 4, node);
    }
    st32(fo + 16, heap);
    return heap;
}
}  // namespace

// The support list and "Plus" slide out ($_21 / $_22 hide them and put them back), the deck arrows fade in.
void StartDeckEditDialog::supportScrollRunOutAnimation(Addr self) {
    Self *s = S(self);
    uint32_t support = find(s->root, S_SUPPORT, 7);
    Frame f;
    uint32_t sPos = f.alloc(8);
    call(scene::fn::Vec2_ctor_copy, {sPos, vcall(support, VS_getPosition, {support})});
    // the lambda capture {support, Vec2, self} built on the stack, then copied for the std::function
    uint32_t capA = f.alloc(16), capB = f.alloc(16);
    st32(capA, support);
    call(scene::fn::Vec2_ctor_copy, {capA + 4, sPos});
    st32(capA + 0xC, self);
    st32(capB, ld32(capA));
    call(scene::fn::Vec2_ctor_copy, {capB + 4, capA + 4});
    st32(capB + 0xC, ld32(capA + 0xC));
    {
        uint32_t fo = f.alloc(24);
        runOutLambda(fo, VT_runOut_21, 0x14, ld32(capB), capB + 4, ld32(capB + 0xC));
        uint32_t anim = call(SceneUtil_createRightOutAnimation, {support, 0, 0, fo});
        call(wl::Node_runAction, {support, call(EaseSineOut_create, {anim})});
        functionDestroy(fo);
    }
    call(wl::Vec2_dtor, {capB + 4});
    uint32_t win = find(support, S_SUPPORT_WIN, 11);
    vcall(win, VS_setTouchEnabled, {win, 0});
    uint32_t plus = find(s->root, S_PLUS, 4);
    uint32_t pPos = f.alloc(8);
    call(scene::fn::Vec2_ctor_copy, {pPos, vcall(plus, VS_getPosition, {plus})});
    uint32_t capC = f.alloc(16), capD = f.alloc(16);
    st32(capC, plus);
    call(scene::fn::Vec2_ctor_copy, {capC + 4, pPos});
    st32(capD, ld32(capC));
    call(scene::fn::Vec2_ctor_copy, {capD + 4, capC + 4});
    st32(capD, ld32(capC));
    {
        uint32_t fo = f.alloc(24);
        runOutLambda(fo, VT_runOut_22, 0x10, ld32(capD), capD + 4, 0);
        uint32_t anim = call(SceneUtil_createRightOutAnimation, {plus, 0, 0, fo});
        call(wl::Node_runAction, {plus, call(EaseSineOut_create, {anim})});
        functionDestroy(fo);
    }
    call(wl::Vec2_dtor, {capD + 4});
    uint32_t left = s->leftUI;
    call(wl::Node_runAction, {left, call(FadeIn_create, {fb(0.1f)})});
    uint32_t right = s->rightUI;
    call(wl::Node_runAction, {right, call(FadeIn_create, {fb(0.1f)})});
    vcall(s->closeButton, VS_setTouchEnabled, {s->closeButton, 1});
    vcall(s->closeButton, VS_setVisible, {s->closeButton, 1});
    call(wl::Vec2_dtor, {capC + 4});
    call(wl::Vec2_dtor, {pPos});
    call(wl::Vec2_dtor, {capA + 4});
    call(wl::Vec2_dtor, {sPos});
}
RW_FUNCTION(0x576BD8, "StartDeckEditDialog::supportScrollRunOutAnimation", StartDeckEditDialog::supportScrollRunOutAnimation);
