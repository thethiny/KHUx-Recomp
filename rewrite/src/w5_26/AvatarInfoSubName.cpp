// AvatarInfoDialog::openPopUpSubName: the title popup of the Player Information page (button A). Two title plates
// (the player's main and sub title, picked by Button_AvTitle1 / 2) and the "plate design" (which of the two shows its
// image on the name plate, Button_Plate). The choice is edited in +0x430..+0x43C (copied from the dialog's current
// titles +0x2E4..+0x2EC); sub_536000 fills the plates and the preview from it.
#include "../scene_menu/AvatarInfoDialog.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t FunctionVB_copy = 0x45C84D;              // std::function<void(bool)>(const&)
constexpr uint32_t FunctionVB_move = 0x47BC69;              // std::function<void(bool)>(&&)
constexpr uint32_t SceneManager_openPopup = 0x61524D;       // (sm, function<bool()>, function<void(bool)>, bool)
constexpr uint32_t Vec2_ctor = 0x8E4B9D, Vec2_dtor = 0x8E4C17;
constexpr uint32_t Title_create = 0x78FA8D;                 // master::Title::create(shared_ptr out, id)
constexpr uint32_t SafeBuffer_read = 0x45A269;              // (sb, dst, src, len, offset)
constexpr uint32_t TextData_format = 0x492929;              // (string out, type, id, ...)
constexpr uint32_t String_append = 0x47AC89;                // std::string::append(const char*, size_t)
constexpr uint32_t ImageView_loadTexture = 0x839731;        // (view, string, TextureResType)
constexpr uint32_t STRLEN = 0x40E08C;
}  // namespace g

constexpr uint32_t S_Base = 0xDFE253, S_PreArea = 0xDFE27C, S_PanelBig = 0xDFE285, S_RankTitleBoard = 0xDF940E,
                   S_RankTitleName = 0xDF941E, S_PanelBig2 = 0xDFE2B9, S_TxtAvTitleWin = 0xDFE2EE,
                   S_AvTitleWin = 0xDFE2FD, S_TxtPlateDesign1 = 0xDFE308, S_RankTitleName1 = 0xDFE319,
                   S_TxtPlateDesign2 = 0xDFE329, S_RankTitleName2 = 0xDFE33A, S_ButtonPlate = 0xDFE34A,
                   S_TxtPlate = 0xDFE357, S_CloseButton = 0xDF8D2F, S_TxtAvTitle1 = 0xDFE361,
                   S_ButtonAvTitle1 = 0xDFE36E, S_TxtAvTitle1_2 = 0xDFE37E, S_ButtonAvTitle2 = 0xDFE38D,
                   S_Empty = 0xDF8A1A;
constexpr int32_t TXT_NO_TITLE = 102200011, TXT_PLATE_PREVIEW = 102200012, TXT_SEPARATOR = 102200013,
                  TXT_WINDOW = 102200001, TXT_TITLE1 = 102200002, TXT_TITLE2 = 102200003,
                  TXT_DESIGN1 = 102200005, TXT_DESIGN2 = 102200006, TXT_PLATE = 102200010;

constexpr uint32_t D_TITLES = 0x2E4;                  // AvatarInfoDialog: main title, sub title, plate (3 ints)
constexpr uint32_t D_EDIT_MAIN = 0x430, D_EDIT_SUB = 0x434, D_EDIT_PLATE = 0x438;   // the popup's choice
constexpr uint32_t D_EDIT_SUB_DESIGN = 0x43C;         // the plate shows the sub title's image
constexpr uint32_t TITLE_NAME = 8, TITLE_NAME_LEN = 0x41, TITLE_IMAGE = 0x154;   // master::Title row

constexpr uint32_t V_SET_VISIBLE = 0xB0, V_GET_OPACITY = 0x20C, V_SET_OPACITY = 0x214, V_ADD_CHILD = 0xF0;
constexpr uint32_t V_SET_ANCHOR = 0x9C, V_SET_POSITION = 0x4C, V_GET_X = 0x68, V_GET_Y = 0x70, V_GET_SIZE = 0xAC;

constexpr uint32_t VT_CloseTouch = 0xF583B8;          // Close_Button touch (no captures)
constexpr uint32_t VT_CanClose = 0xF5838C;            // openPopup's function<bool()> {close button}
constexpr uint32_t VT_Closed = 0xF582DC;              // openPopup's result (heap 0x50, see below)
constexpr uint32_t VT_PlateTouch = 0xF582B0;          // Button_Plate / Button_AvTitle1 / 2 (heap 0x28, plates)
constexpr uint32_t VT_Title1Touch = 0xF58258, VT_Title2Touch = 0xF58200;

// text->setString(TextData `id`) then its shadow
void textShadow(uint32_t text, int32_t id) {
    setTextId(text, id);
    SceneUtil::setShadow(text);
}

// a std::function at `fo` holding a heap lambda (0x28 bytes) over the popup's plates
void plateLambda(uint32_t fo, uint32_t vt, const uint32_t (&w)[9]) {
    st32(fo + 16, 0);
    const uint32_t obj = call(fn::operator_new, {0x28});
    if (obj) {
        st32(obj, vt);
        for (uint32_t k = 0; k < 9; k++) st32(obj + 4 + 4 * k, w[k]);
    }
    st32(fo + 16, obj);
}

// one title plate (name text `text`) from title `id`: its name (appended to `names`) and its image id, or "none"
uint32_t titlePlate(uint32_t id, uint32_t text, uint32_t names, bool separator) {
    Frame f;
    if (!id) {
        setTextId(text, TXT_NO_TITLE);
        return 0;
    }
    const uint32_t row = f.alloc(8);
    call(g::Title_create, {row, id});
    const uint32_t sb = ld32(row);
    const uint32_t name = f.alloc(0x44);
    call(g::SafeBuffer_read, {sb, name, ld32(sb + 12) + TITLE_NAME, TITLE_NAME_LEN, TITLE_NAME});
    setTextC(text, name);
    if (separator && (ld8(names) & 1 ? ld32(names + 4) : ld8(names) >> 1)) {
        const uint32_t sep = f.alloc(12);
        call(g::TextData_format, {sep, 0, uint32_t(TXT_SEPARATOR)});
        const bool longSep = ld8(sep) & 1;
        call(g::String_append, {names, longSep ? ld32(sep + 8) : sep + 1, longSep ? ld32(sep + 4) : ld8(sep) >> 1});
        if (longSep) call(fn::operator_delete, {ld32(sep + 8)});
    }
    call(g::String_append, {names, name, call(g::STRLEN, {name})});
    const uint32_t image = f.alloc(4);
    call(g::SafeBuffer_read, {sb, image, ld32(sb + 12) + TITLE_IMAGE, 4, TITLE_IMAGE});
    sharedRelease(row);
    return ld32(image);
}

struct AvatarInfoSubName {
    // sub_536000 {dialog, name1, name2, board, big2, rankName, plateButton, plateText}: the plates and the preview
    static void refresh(uint32_t ctx) {
        const uint32_t dialog = ld32(ctx);
        Frame f;
        const uint32_t names = newStr(f, S_Empty, 0);
        const uint32_t image1 = titlePlate(ld32(dialog + D_EDIT_MAIN), ld32(ctx + 4), names, false);
        const uint32_t image2 = titlePlate(ld32(dialog + D_EDIT_SUB), ld32(ctx + 8), names, true);
        const bool sub = ld32(dialog + D_EDIT_SUB_DESIGN) != 0;
        st32(dialog + D_EDIT_PLATE, ld32(dialog + (sub ? D_EDIT_SUB : D_EDIT_MAIN)));
        const uint32_t image = sub ? image2 : image1;
        const uint32_t board = ld32(ctx + 0xC), big2 = ld32(ctx + 0x10);
        vcall(board, V_SET_VISIBLE, {board, image ? 1u : 0u});
        if (image) {
            const uint32_t path = f.alloc(12);
            SceneUtil::createUserTitleImagePath(path, int32_t(image));
            const uint32_t s = newStr(f, ld8(path) & 1 ? ld32(path + 8) : path + 1);
            call(g::ImageView_loadTexture, {board, s, 0});
            strFree(s);
            strFree(path);
            vcall(big2, V_SET_VISIBLE, {big2, 0});
        } else {
            vcall(big2, V_SET_VISIBLE, {big2, 1});
        }
        call(fn::Text_setString, {ld32(ctx + 0x14), names});
        const uint32_t both = ld32(dialog + D_EDIT_MAIN) ? (ld32(dialog + D_EDIT_SUB) != 0) : 0;
        SceneUtil::setButtonEnabled(ld32(ctx + 0x18), ld32(ctx + 0x1C), both, 0);
        strFree(names);
    }

    static void open(uint32_t dialog, uint32_t onClosed) {
        for (uint32_t k = 0; k < 3; k++) st32(dialog + D_EDIT_MAIN + 4 * k, ld32(dialog + D_TITLES + 4 * k));
        call(fn::AudioPlaySystemSEPopUp, {});
        const uint32_t layer = call(fn::SceneManager_getPopupLayer2, {sceneManager()});
        const uint32_t shadow = SceneUtil::createShadowSprite();
        const uint32_t opacity = vcall(shadow, V_GET_OPACITY, {shadow});
        vcall(shadow, V_SET_OPACITY, {shadow, 0});
        call(fn::Node_runAction, {shadow, call(fn::FadeTo_create, {fbits(0.1f), opacity})});
        vcall(layer, V_ADD_CHILD, {layer, shadow});
        const uint32_t view = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_Base});
        vcall(view, V_SET_ANCHOR, {view, VEC2_ANCHOR_MIDDLE});
        Frame f;
        {
            const float px = bitsf(vcall(view, V_GET_X, {view}));
            const float w = ldf(vcall(view, V_GET_SIZE, {view}));
            const float mx = SceneUtil::getSideMargin();
            const float py = bitsf(vcall(view, V_GET_Y, {view}));
            const float y = py + ldf(vcall(view, V_GET_SIZE, {view}) + 4) * 0.5f;
            const float my = SceneUtil::getSideHeightMargin();
            const uint32_t pos = f.alloc(8);
            call(g::Vec2_ctor, {pos, fbits(mx + (px + w * 0.5f)), fbits(my + y)});
            vcall(view, V_SET_POSITION, {view, pos});
            call(g::Vec2_dtor, {pos});
        }
        vcall(shadow, V_ADD_CHILD, {shadow, view});

        const uint32_t preArea = findNode(view, S_PreArea, 8);
        const uint32_t big = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_PanelBig});
        vcall(preArea, V_ADD_CHILD, {preArea, big});
        const uint32_t board = findNode(big, S_RankTitleBoard, 0xF);
        const uint32_t rankName = findNode(board, S_RankTitleName, 0xE);
        SceneUtil::setUserTitleOutline(rankName);
        const uint32_t big2 = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_PanelBig2});
        vcall(preArea, V_ADD_CHILD, {preArea, big2});
        const uint32_t preview = findNode(big2, S_RankTitleName, 0xE);
        SceneUtil::setUserTitleOutline(preview);
        setTextId(preview, TXT_PLATE_PREVIEW);
        textShadow(findNode(view, S_TxtAvTitleWin, 0xE), TXT_WINDOW);
        const uint32_t win = findNode(view, S_AvTitleWin, 0xA);
        titleLabel(win, S_TxtPlateDesign1, 0x10, TXT_DESIGN1);
        const uint32_t name1 = findNode(win, S_RankTitleName1, 0xF);
        titleLabel(win, S_TxtPlateDesign2, 0x10, TXT_DESIGN2);
        const uint32_t name2 = findNode(win, S_RankTitleName2, 0xF);
        const uint32_t plate = findNode(view, S_ButtonPlate, 0xC);
        const uint32_t plateText = findNode(view, S_TxtPlate, 9);
        textShadow(plateText, TXT_PLATE);
        const uint32_t ctx = f.alloc(0x20);
        const uint32_t plates[8] = {dialog, name1, name2, board, big2, rankName, plate, plateText};
        for (uint32_t k = 0; k < 8; k++) st32(ctx + 4 * k, plates[k]);

        // the close lambda {view, shadow, layer, onClosed}
        const uint32_t closer = f.alloc(0x28);
        st32(closer, view);
        st32(closer + 4, shadow);
        st32(closer + 8, layer);
        call(g::FunctionVB_copy, {closer + 0x10, onClosed});
        const uint32_t close = findNode(view, S_CloseButton, 0xC);
        addTouch(close, VT_CloseTouch, {});
        // the result lambda {close, dialog, onClosed, closer}
        const uint32_t result = f.alloc(0x50);
        st32(result, close);
        st32(result + 4, dialog);
        call(g::FunctionVB_copy, {result + 8, onClosed});
        for (uint32_t k = 0; k < 3; k++) st32(result + 0x20 + 4 * k, ld32(closer + 4 * k));
        functionCopy(result + 0x30, closer + 0x10);
        {
            const uint32_t sm = sceneManager();
            Frame t;
            const uint32_t canClose = newFunction(t, VT_CanClose, {close});
            const uint32_t copy = t.alloc(0x48);   // the by-value copy of the result lambda
            st32(copy, ld32(result));
            st32(copy + 4, ld32(result + 4));
            functionCopy(copy + 8, result + 8);
            for (uint32_t k = 0; k < 3; k++) st32(copy + 0x20 + 4 * k, ld32(result + 0x20 + 4 * k));
            functionCopy(copy + 0x30, result + 0x30);
            const uint32_t closed = t.alloc(20);
            st32(closed + 16, 0);
            const uint32_t obj = call(fn::operator_new, {0x50});
            if (obj) {
                st32(obj, VT_Closed);
                st32(obj + 8, ld32(copy));
                st32(obj + 0xC, ld32(copy + 4));
                call(g::FunctionVB_move, {obj + 0x10, copy + 8});
                for (uint32_t k = 0; k < 3; k++) st32(obj + 0x28 + 4 * k, ld32(copy + 0x20 + 4 * k));
                call(g::FunctionVB_move, {obj + 0x38, copy + 0x30});
            }
            st32(closed + 16, obj);
            call(g::SceneManager_openPopup, {sm, canClose, closed, 1});
            functionDestroy(closed);
            functionDestroy(copy + 0x30);
            functionDestroy(copy + 8);
            functionDestroy(canClose);
        }
        const uint32_t caps[9] = {dialog, dialog, name1, name2, board, big2, rankName, plate, plateText};
        {
            Frame t;
            const uint32_t fo = t.alloc(20);
            plateLambda(fo, VT_PlateTouch, caps);
            call(fn::Widget_addTouchEventListener, {plate, fo});
            functionDestroy(fo);
        }
        textShadow(findNode(view, S_TxtAvTitle1, 0xC), TXT_TITLE1);
        {
            const uint32_t button = findNode(view, S_ButtonAvTitle1, 0xF);
            Frame t;
            const uint32_t fo = t.alloc(20);
            plateLambda(fo, VT_Title1Touch, caps);
            call(fn::Widget_addTouchEventListener, {button, fo});
            functionDestroy(fo);
        }
        textShadow(findNode(view, S_TxtAvTitle1_2, 0xE), TXT_TITLE2);
        {
            const uint32_t button = findNode(view, S_ButtonAvTitle2, 0xF);
            Frame t;
            const uint32_t fo = t.alloc(20);
            plateLambda(fo, VT_Title2Touch, caps);
            call(fn::Widget_addTouchEventListener, {button, fo});
            functionDestroy(fo);
        }
        // the plate shows the sub title's image when only the sub title matches the current plate
        st32(dialog + D_EDIT_SUB_DESIGN, 0);
        const uint32_t current = ld32(dialog + D_EDIT_PLATE);
        if (ld32(dialog + D_EDIT_SUB) && ld32(dialog + D_EDIT_SUB) == current) st32(dialog + D_EDIT_SUB_DESIGN, 1);
        if (ld32(dialog + D_EDIT_MAIN) && ld32(dialog + D_EDIT_MAIN) == current) st32(dialog + D_EDIT_SUB_DESIGN, 0);
        refresh(ctx);
        {
            Frame t;
            const uint32_t none = newEmptyFunction(t);
            call(fn::Node_runAction, {view, SceneUtil::createOpenPopupAnimation(view, none)});
            functionDestroy(none);
        }
        functionDestroy(result + 0x30);
        functionDestroy(result + 8);
        functionDestroy(closer + 0x10);
    }
};
}  // namespace

RW_FUNCTION(0x53506C, "AvatarInfoDialog::openPopUpSubName(function<void(bool)>)", AvatarInfoSubName::open);
RW_FUNCTION(0x536000, "AvatarInfoDialog::openPopUpSubName plates (sub_536000)", AvatarInfoSubName::refresh);
