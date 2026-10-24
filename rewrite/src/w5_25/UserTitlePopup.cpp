// SceneUtil::openUserTitlePopup(titles, index, onClose): "You got a title!" (BattleResult_Title_V2): the title's
// plate image and name, its description, OK ($_56 closes it, then the next title / onClose) and the open
// animation ($_57). With no title at `index` it calls onClose right away.
#include "../master/Title.hpp"
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SceneManager_getPopupLayer = 0x613E69;
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_createUserTitleImagePath = 0x46ED99;   // (string out, int label)
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t SceneUtil_createOpenPopupAnimation = 0x45BC05;
constexpr uint32_t Title_create = 0x78FA8D;                         // master::Title::create(shared_ptr out, id)
constexpr uint32_t Widget_addTouchEventListener = rwlib::cocos2d::ui::Widget::addTouchEventListener;
constexpr uint32_t EaseSineOut_create = rwlib::cocos2d::EaseSineOut::create;
constexpr uint32_t audio_Manager_getInstance = 0x5BECFD, audio_Manager_load = 0x5BEEF5;
}  // namespace g

constexpr uint32_t S_TitleJson = 0xDF956E;   // "cocostudio/publish/BattleResult_Title_V2.json"
constexpr uint32_t S_Txt = 0xDF94D1, S_RankTitle_Board = 0xDF940E, S_RankTitle_Name = 0xDF941E;
constexpr uint32_t S_Txt_Title_Label = 0xDF9554, S_Button_Ok = 0xDF9564, S_Txt_OK = 0xDF869F;
constexpr int32_t TXT_TITLE_GOT = 103010008, TXT_OK = 100100001;
constexpr uint32_t SE_TITLE = 0x1E584184;
constexpr uint32_t VT_Ok = 0xF546C0;         // $_56 {popup, layer, shadow, shared_ptr titles, index, onClose}
constexpr uint32_t VT_Opened = 0xF54694;     // $_57 {popup}
constexpr uint32_t TITLES_COUNT = 0x20;      // the UserTitle SafeBuffer: count; records of 16 bytes (+8 title id)

// text->setString(c string copy), then the text shadow
void setShadowedText(uint32_t text, uint32_t cstr) {
    setTextC(text, cstr);
    call(g::SceneUtil_setShadow, {text});
}

struct UserTitlePopup {
    static void open(uint32_t titles, int32_t index, uint32_t onClose) {
        uint32_t sb = ld32(titles);
        if (!sb || !(int32_t(ld32(sb + TITLES_COUNT)) > index)) {
            if (ld32(onClose + 16)) call(fn::function_vv_call, {onClose});
            return;
        }
        uint32_t layer = call(g::SceneManager_getPopupLayer, {sceneManager()});
        uint32_t shadow = call(g::SceneUtil_createShadowSprite, {});
        vcall(shadow, node::setOpacity, {shadow, 0});
        vcall(layer, node::addChild, {layer, shadow});
        uint32_t popup = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_TitleJson});
        centerPopup(popup);
        vcall(layer, node::addChild, {layer, popup});
        Frame f;
        int32_t titleId = safe32(sb, 16 * uint32_t(index) + 8);
        uint32_t title = masterRow(f, g::Title_create, uint32_t(titleId));
        uint32_t row = ld32(title);
        titleLabel(popup, S_Txt, 3, TXT_TITLE_GOT);
        uint32_t board = findNode(popup, S_RankTitle_Board, 15);
        {
            Frame t;
            uint32_t path = t.alloc(12);
            call(g::SceneUtil_createUserTitleImagePath, {path, uint32_t(safe32(row, master::Title::LABEL))});
            uint32_t copy = newStr(t, strData(path));
            call(fn::ImageView_loadTexture, {board, copy, 0});
            strFree(copy);
            strFree(path);
        }
        uint32_t name = findNode(board, S_RankTitle_Name, 14);
        setShadowedText(name, safeBytes(f, row, master::Title::TITLE, 0x41));
        uint32_t description = findNode(popup, S_Txt_Title_Label, 15);
        setShadowedText(description, safeBytes(f, row, master::Title::DESCRIPTION, 0x101));
        uint32_t ok = findNode(popup, S_Button_Ok, 9);
        titleLabel(ok, S_Txt_OK, 6, TXT_OK);
        {
            Frame t;
            uint32_t ctrl = ld32(titles + 4);          // the lambda object's copy of `titles`
            if (ctrl) call(fn::shared_add, {ctrl});
            uint32_t copy = t.alloc(20);
            call(fn::function_vv_copy, {copy, onClose});
            uint32_t cb = newEmptyFunction(t);
            uint32_t heap = call(fn::operator_new, {0x38});
            if (heap) {
                st32(heap, VT_Ok);
                st32(heap + 8, popup);
                st32(heap + 0xC, layer);
                st32(heap + 0x10, shadow);
                st32(heap + 0x14, sb);
                st32(heap + 0x18, ctrl);
                if (ctrl) call(fn::shared_add, {ctrl});
                st32(heap + 0x1C, uint32_t(index));
                call(fn::function_vv_move, {heap + 0x20, copy});
            }
            st32(cb + 16, heap);
            call(g::Widget_addTouchEventListener, {ok, cb});
            functionDestroy(cb);
            functionDestroy(copy);
            if (ctrl) call(fn::shared_release, {ctrl});
        }
        call(g::audio_Manager_load, {call(g::audio_Manager_getInstance, {}), SE_TITLE});
        {
            Frame t;
            uint32_t opened = newFunction(t, VT_Opened, {popup});
            uint32_t anim = call(g::SceneUtil_createOpenPopupAnimation, {popup, opened});
            call(fn::Node_runAction, {popup, call(g::EaseSineOut_create, {anim})});
            functionDestroy(opened);
        }
        sharedRelease(title);
    }
};
}  // namespace

RW_FUNCTION(0x470724, "SceneUtil::openUserTitlePopup(const shared_ptr<UserTitle>&, int, function<void()>)",
            UserTitlePopup::open);
