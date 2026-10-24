// AdvStageScrollView: the story list rows (stage frame, "coming soon / over" frame, the story title panel) and the
// arrow-button helper of the colosseum info box. All of these are static in practice: r0 is the row widget.
#include "AdvStageScrollView.hpp"

using namespace scene;

namespace adv_story {
namespace lib {
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t StoryVector_copy = 0x4B5BC9;       // std::vector<hole::network::api::Stage::Story>(const vector&)
constexpr uint32_t StoryVector_dtor = 0x4B58EF;       // std::__vector_base<Stage::Story>::~__vector_base
constexpr uint32_t IntVector_dtor = 0x478961;         // std::__vector_base<int>::~__vector_base
constexpr uint32_t StageUtil_findStory = 0x764101;    // StageUtil::findStory(int stageId, int kind) -> Story*
constexpr uint32_t Stage_create = 0x78E4E9;           // master::Stage::create(shared_ptr out, id)
constexpr uint32_t ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE;  // (image, const string&, TextureResType)
constexpr uint32_t Text_setString = rwlib::cocos2d::ui::Text::setString;
constexpr uint32_t TextAtlas_setString = rwlib::cocos2d::ui::TextAtlas::setString;
constexpr uint32_t setSubMisstion = 0x5691E1;         // AdvStageScrollView::setSubMisstion(widget, int, const vector<int>&)
constexpr uint32_t pointToString = 0x506B61;          // AdvStageScrollView::pointToString(string out, int, int, bool)
constexpr uint32_t ImageButton_create = 0x531239;     // (std::string by value, bool)
constexpr uint32_t ImageButton_setTargetView = 0x5318C1;        // (button, Node*, float scale)
constexpr uint32_t ImageButton_setTargetAnchorPoint = 0x5318CF; // (button, const Vec2&)
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;  // (button, std::function by value)
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff;              // Vec2(this, x, y)
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t FunctionVoid_copy = 0x4683CD;      // std::function<void()>(const function&)
constexpr uint32_t FunctionVoid_move = 0x47F905;      // std::function<void()>(function&&)
}  // namespace lib

// GOT slots of the AdvStageScrollView static path tables (const char* arrays; element 0 is a format / path)
constexpr uint32_t GOT_STAGE_ICON_PASS = 0xFD6678;
constexpr uint32_t GOT_OVER_ICON_PASS = 0xFD6684;
constexpr uint32_t GOT_COMING_SOON_ICON_PASS = 0xFD6688;
constexpr uint32_t GOT_STAGE_WORLD_ICON_PASS = 0xFD6690;

// __func vtable of the arrow button's touch lambda {bool visible; std::function<void()> callback}
constexpr uint32_t VT_ARROW_TOUCH_LAMBDA = 0xF59540;

// strings
constexpr uint32_t S_PANEL = 0xDFD420;               // "Panel"
constexpr uint32_t S_TXT_STAGE_NUM_LABEL = 0xDFF3BB; // "Txt_Stage_Num_Label"
constexpr uint32_t S_FMT_D = 0xDF86DE;               // "%d"
constexpr uint32_t S_FMT_S = 0xDF8821;               // "%s"
constexpr uint32_t S_KEYBLADE_TYPE = 0xDF8A58;       // "KeyBlade_type"
constexpr uint32_t S_STORY = 0xDFF3CF;               // "Story"
constexpr uint32_t S_NEW = 0xDFD177;                 // "New"
constexpr uint32_t S_TXT_NEW = 0xDFF3D5;             // "Txt_New"
constexpr uint32_t S_BALL = 0xDFF3DD;                // "Ball"
constexpr uint32_t S_FACEBOOK_PHOTO_PANEL = 0xDF8E5A;// "FaceBook_PhotoPanel"
constexpr uint32_t S_WORLD_SELECT_ICO = 0xDFF614;    // "world_select_ico"
constexpr uint32_t S_TXT_WORLD_LABEL = 0xDFF625;     // "Txt_World_Label"
constexpr uint32_t S_PLATE2 = 0xDFF635;              // "Plate2"
constexpr uint32_t S_TXT2 = 0xDFF63C;                // "Txt2"
constexpr uint32_t S_TXT_STORY_LABEL = 0xDFF641;     // "Txt_Story_Label"
constexpr uint32_t S_PLATE1 = 0xDFF651;              // "Plate1"
constexpr uint32_t S_TXT1 = 0xDFF658;                // "Txt1"
constexpr uint32_t S_TXT_LUX_BEST = 0xDFF65D;        // "Txt_LuxBestRecord_Label"
constexpr uint32_t S_TXT_LUX_BEST_NUM = 0xDFF675;    // "Txt_LuxBestRecord_Num_Label"
constexpr uint32_t S_ATLAS_AP = 0xDFD382;            // "AtlasLabel_AP"
constexpr uint32_t S_IMAGE_BUTTON_FMT = 0xDFF854;    // "%s_ImageButton"
constexpr uint32_t S_EMPTY = 0xDF8A1A;               // ""

// text ids
constexpr int32_t TEXT_TXT2 = 102900021;
constexpr int32_t TEXT_TXT1_FMT = 102900020;         // format taking the stage number
constexpr int32_t TEXT_LUX_BEST = 102900011;

constexpr uint32_t API_CURRENT_STAGE = 292;          // APIManager + 292: Story list object (+12: the current stage id)
constexpr uint32_t API_CAMPAIGN_IDS = 764;           // APIManager + 764: std::vector<int> (campaigns)

// master::Stage row offsets used here
constexpr uint32_t STAGE_NUMBER = 4;
constexpr uint32_t STAGE_STORY_NAME = 8;             // char[129]
constexpr uint32_t STAGE_WORLD_NAME = 137;           // char[129]
constexpr uint32_t STAGE_WORLD_ICON = 276;
constexpr uint32_t STAGE_ICON_SUB = 280;
constexpr uint32_t STAGE_STORY_KEY = 284;
constexpr uint32_t STAGE_HAS_STORY_B = 292;
constexpr uint32_t STAGE_HAS_STORY_A = 336;
constexpr uint32_t STAGE_SUB_MISSION = 480;

void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }

// a std::string built from a NUL-terminated guest buffer
uint32_t strFromC(Frame &f, uint32_t cstr) { return newStr(f, cstr); }

// the first element of a static const char* table reached through its GOT slot
uint32_t gotTable0(uint32_t got) { return ld32(ld32(got)); }

// node = searchNodeByName(root, literal); free; node->setOpacity(255) (the name is freed after the call)
void setOpaque(uint32_t root, uint32_t name, uint32_t len) {
    Frame f;
    uint32_t s = newStr(f, name, len);
    uint32_t node = SceneUtil::searchNodeByName(root, s);
    vcall(node, node::setOpacity, {node, 255});
    strFree(s);
}

// ------------------------------------------------------------------------------------------------ setStoryTitle
// sub_56ECB0: the lambda of setStoryTitle capturing the panel by reference:
//   [&](const string &name, const string &text) { Text *t = searchNodeByName(panel, name); setShadow(t);
//                                                 t->setString(text); }
// Returns the stack-canary difference (0).
uint32_t setLabelLambda(const uint32_t *capture, uint32_t name, uint32_t text) {
    uint32_t panel = *capture;
    uint32_t label;
    {
        Frame f;
        uint32_t s = strFromC(f, strData(name));
        label = SceneUtil::searchNodeByName(panel, s);
        strFree(s);
    }
    SceneUtil::setShadow(label);
    Frame f;
    uint32_t s = strFromC(f, strData(text));
    call(lib::Text_setString, {label, s});
    strFree(s);
    return 0;
}

// The story panel of a stage: world icon and name, story name, stage number, best Lux record and the campaign
// AP rate. Static: (panel widget, stage id). Returns the frame size left in r0 by the epilogue (3692).
uint32_t setStoryTitle(uint32_t panel, int32_t stageId) {
    if (stageId == 0) return 3692;
    Frame f;
    uint32_t row = masterRow(f, lib::Stage_create, uint32_t(stageId));
    uint32_t sb = ld32(row);

    // world icon
    uint32_t icon = findNode(panel, S_WORLD_SELECT_ICO, 16);
    {
        Frame g;
        uint32_t fmt = gotTable0(GOT_STAGE_WORLD_ICON_PASS);
        uint32_t buf = stringBuffer(g, {fmt, uint32_t(safe32(sb, STAGE_WORLD_ICON))});
        uint32_t s = strFromC(g, buf);
        call(lib::ImageView_loadTexture, {icon, s, 0});
        strFree(s);
    }

    uint32_t cap = f.alloc(4);   // the lambda: captures the panel
    st32(cap, panel);

    // world name
    {
        Frame g;
        uint32_t name = newStr(g, S_TXT_WORLD_LABEL, 15);
        uint32_t chars = safeBytes(g, sb, STAGE_WORLD_NAME, 0x81);
        uint32_t text = strFromC(g, chars);
        setLabelLambda(rw::host_ptr<uint32_t>(cap), name, text);
        strFree(text);
        strFree(name);
    }
    setOpaque(panel, S_PLATE2, 6);
    {
        Frame g;
        uint32_t name = newStr(g, S_TXT2, 4);
        uint32_t td = textData(g, TEXT_TXT2);
        uint32_t text = strFromC(g, textDataStr(td));
        setLabelLambda(rw::host_ptr<uint32_t>(cap), name, text);
        strFree(text);
        sharedRelease(td);
        strFree(name);
    }
    // story name
    {
        Frame g;
        uint32_t name = newStr(g, S_TXT_STORY_LABEL, 15);
        uint32_t chars = safeBytes(g, sb, STAGE_STORY_NAME, 0x81);
        uint32_t text = strFromC(g, chars);
        setLabelLambda(rw::host_ptr<uint32_t>(cap), name, text);
        strFree(text);
        strFree(name);
    }
    setOpaque(panel, S_PLATE1, 6);
    // stage number ("... %d ...")
    {
        Frame g;
        uint32_t name = newStr(g, S_TXT1, 4);
        uint32_t td = textData(g, TEXT_TXT1_FMT);
        uint32_t fmt = textDataStr(td);
        uint32_t buf = stringBuffer(g, {fmt, uint32_t(safe32(sb, STAGE_NUMBER))});
        uint32_t text = strFromC(g, buf);
        setLabelLambda(rw::host_ptr<uint32_t>(cap), name, text);
        strFree(text);
        sharedRelease(td);
        strFree(name);
    }
    uint32_t story = call(lib::StageUtil_findStory, {uint32_t(stageId), uint32_t(safe32(sb, STAGE_STORY_KEY))});
    // best Lux record
    {
        Frame g;
        uint32_t name = newStr(g, S_TXT_LUX_BEST, 0x17);
        uint32_t td = textData(g, TEXT_LUX_BEST);
        uint32_t text = strFromC(g, textDataStr(td));
        setLabelLambda(rw::host_ptr<uint32_t>(cap), name, text);
        strFree(text);
        sharedRelease(td);
        strFree(name);
    }
    {
        Frame g;
        uint32_t name = newStr(g, S_TXT_LUX_BEST_NUM, 0x1B);
        uint32_t money = g.alloc(12);
        SceneUtil::getMoneyStr(money, uint64_t(int64_t(int32_t(ld32(story + 8)))));
        uint32_t buf = stringBuffer(g, {S_FMT_S, strData(money)});
        uint32_t text = strFromC(g, buf);
        setLabelLambda(rw::host_ptr<uint32_t>(cap), name, text);
        strFree(text);
        strFree(money);
        strFree(name);
    }
    // campaign AP rate
    uint32_t api = call(lib::APIManager_getInstance, {});
    int32_t base = int32_t(ld32(story + 4));
    int32_t point = SceneUtil::getCampaignPoint(base, safe32(sb, STAGE_STORY_KEY), stageId, api + API_CAMPAIGN_IDS, 7,
                                                0);
    uint32_t atlas = findNode(panel, S_ATLAS_AP, 13);
    {
        Frame g;
        uint32_t pts = g.alloc(12);
        call(lib::pointToString, {pts, uint32_t(point), 0, 0});
        uint32_t s = strFromC(g, strData(pts));
        call(lib::TextAtlas_setString, {atlas, s});
        strFree(s);
        strFree(pts);
    }
    sharedRelease(row);
    return 3692;
}

// ------------------------------------------------------------------------------------------------ stage frames
// A story stage row: stage icon, number, boss type, story mark, sub missions and the "New" mark on the current
// stage. Static: (row widget, stage id). Returns the frame size left in r0 by the epilogue (2284).
uint32_t setStoryStageFrame(uint32_t widget, int32_t stageId) {
    uint32_t currentStage = ld32(ld32(call(lib::APIManager_getInstance, {}) + API_CURRENT_STAGE) + 12);
    Frame f;
    uint32_t stories = f.alloc(12);   // an unused copy of the story list
    call(lib::StoryVector_copy, {stories, ld32(call(lib::APIManager_getInstance, {}) + API_CURRENT_STAGE)});
    uint32_t story = call(lib::StageUtil_findStory, {uint32_t(stageId), 1});
    uint32_t row = masterRow(f, lib::Stage_create, uint32_t(stageId));
    uint32_t sb = ld32(row);

    uint32_t panel = findNode(widget, S_PANEL, 5);
    {
        Frame g;
        uint32_t fmt = gotTable0(GOT_STAGE_ICON_PASS);
        int32_t a = safe32(sb, STAGE_WORLD_ICON);
        int32_t b = safe32(sb, STAGE_ICON_SUB);
        uint32_t buf = stringBuffer(g, {fmt, uint32_t(a), uint32_t(b)});
        uint32_t s = strFromC(g, buf);
        call(lib::ImageView_loadTexture, {panel, s, 0});
        strFree(s);
    }

    uint32_t num = findNode(widget, S_TXT_STAGE_NUM_LABEL, 0x13);
    SceneUtil::setShadow(num);
    {
        Frame g;
        uint32_t buf = stringBuffer(g, {S_FMT_D, uint32_t(safe32(sb, STAGE_NUMBER))});
        uint32_t s = strFromC(g, buf);
        call(lib::Text_setString, {num, s});
        strFree(s);
    }
    setVisible(num, 1);

    uint32_t keyblade = findNode(widget, S_KEYBLADE_TYPE, 0xD);
    SceneUtil::setBossTypeImage(keyblade, stageId, 0);
    setVisible(keyblade, 1);

    uint32_t hasStory = 1;
    if (safe32(sb, STAGE_HAS_STORY_A) == 0) hasStory = safe32(sb, STAGE_HAS_STORY_B) != 0 ? 1 : 0;
    uint32_t storyNode = findNode(widget, S_STORY, 5);
    setVisible(storyNode, hasStory);

    call(lib::setSubMisstion, {widget, uint32_t(safe32(sb, STAGE_SUB_MISSION)), story + 12});

    uint32_t newMark = findNode(widget, S_NEW, 3);
    if (currentStage == uint32_t(stageId)) {
        setVisible(newMark, 1);
        uint32_t text = findNode(newMark, S_TXT_NEW, 7);
        SceneUtil::setShadow(text);
        Frame g;
        uint32_t s = newStr(g, S_NEW, 3);
        call(lib::Text_setString, {text, s});
        strFree(s);
    } else {
        setVisible(newMark, 0);
    }
    setVisible(findNode(widget, S_BALL, 4), 0);
    setVisible(findNode(widget, S_FACEBOOK_PHOTO_PANEL, 0x13), 0);
    sharedRelease(row);
    call(lib::StoryVector_dtor, {stories});
    return 2284;
}

// The row past the last story stage: the "over" icon (more to come) or "coming soon", everything else hidden.
// Static: (row widget, over). Returns the stack-canary difference (0).
uint32_t setOverStageFrame(uint32_t widget, int32_t over) {
    uint32_t panel = findNode(widget, S_PANEL, 5);
    if (panel) {
        Frame g;
        uint32_t path = gotTable0(over ? GOT_OVER_ICON_PASS : GOT_COMING_SOON_ICON_PASS);
        uint32_t s = strFromC(g, path);
        call(lib::ImageView_loadTexture, {panel, s, 0});
        strFree(s);
    }
    Frame f;
    uint32_t none = f.alloc(12);   // empty std::vector<int>
    st32(none, 0);
    st32(none + 4, 0);
    st32(none + 8, 0);
    call(lib::setSubMisstion, {widget, 0, none});
    static const struct { uint32_t name, len; } hidden[] = {
        {S_TXT_STAGE_NUM_LABEL, 0x13}, {S_KEYBLADE_TYPE, 0xD}, {S_STORY, 5},
        {S_NEW, 3},                    {S_BALL, 4},            {S_FACEBOOK_PHOTO_PANEL, 0x13},
    };
    for (const auto &h : hidden) {
        uint32_t node = findNode(widget, h.name, h.len);
        if (node) setVisible(node, 0);
    }
    call(lib::IntVector_dtor, {none});
    return 0;
}

// ------------------------------------------------------------------------------------------------ colosseum
// sub_56E1CC: a helper of setColosseumInfo (LeftUI / RightUI / Left_arrow / Right_arrow / Point_*_arrow): shows
// or hides the node `name` under `root`; makes sure its first child carries an ImageButton "<name>_ImageButton"
// (created over the child on first use) and sets its touch listener to a lambda {visible, callback}.
// Returns the frame size left in r0 by the epilogue (2228).
uint32_t setArrowButton(uint32_t root, uint32_t name, uint32_t visible, uint32_t callback) {
    if (root == 0) return 2228;
    uint32_t node;
    {
        Frame f;
        uint32_t s = strFromC(f, name);
        node = SceneUtil::searchNodeByName(root, s);
        strFree(s);
    }
    if (node == 0) return 2228;
    vcall(node, node::setVisible, {node, visible});
    uint32_t target;
    {
        Frame f;
        uint32_t s = strFromC(f, name);
        uint32_t again = SceneUtil::searchNodeByName(root, s);
        uint32_t children = vcall(again, node::getChildren, {again});
        target = ld32(ld32(children));   // getChildren().at(0)
        strFree(s);
    }
    uint32_t button;
    {
        Frame f;
        uint32_t buf = stringBuffer(f, {S_IMAGE_BUTTON_FMT, name});
        uint32_t s = strFromC(f, buf);
        button = SceneUtil::searchNodeByName(target, s);
        strFree(s);
    }
    if (button == 0) {
        {
            Frame f;
            uint32_t s = newStr(f, S_EMPTY, 0);
            button = call(lib::ImageButton_create, {s, 0});
            strFree(s);
        }
        call(lib::ImageButton_setTargetView, {button, target, fbits(1.0f)});
        {
            Frame f;
            uint32_t v = f.alloc(8);
            call(lib::Vec2_ctor, {v, fbits(-0.5f), fbits(-0.5f)});
            call(lib::ImageButton_setTargetAnchorPoint, {button, v});
            call(lib::Vec2_dtor, {v});
        }
        uint32_t addChild = ld32(ld32(target) + node::addChildZName);   // read before the name is built
        Frame f;
        uint32_t buf = stringBuffer(f, {S_IMAGE_BUTTON_FMT, name});
        uint32_t s = strFromC(f, buf);
        call(addChild, {target, button, 0, s});
        strFree(s);
    }
    Frame f;
    // the stack lambda {bool visible; std::function<void()> callback (at +8)}
    uint32_t lambda = f.alloc(28);
    st8(lambda, uint8_t(visible));
    uint32_t fnCopy = lambda + 8;
    call(lib::FunctionVoid_copy, {fnCopy, callback});
    // std::function<void(Ref*, TouchEventType)> holding the lambda on the heap
    uint32_t listener = f.alloc(20);
    st32(listener + 16, 0);
    uint32_t heap = call(guest::OPERATOR_NEW, {0x28});
    if (heap) {
        st32(heap, VT_ARROW_TOUCH_LAMBDA);
        st8(heap + 8, ld8(lambda));
        call(lib::FunctionVoid_move, {heap + 16, fnCopy});
        st8(heap + 8, ld8(lambda));
    }
    st32(listener + 16, heap);
    call(lib::ImageButton_addTouchEventListener, {button, listener});
    functionDestroy(listener);
    functionDestroy(fnCopy);
    return 2228;
}
}  // namespace adv_story

RW_FUNCTION(0x568C88, "AdvStageScrollView::setStoryStageFrame", adv_story::setStoryStageFrame);
RW_FUNCTION(0x56A2CC, "AdvStageScrollView::setOverStageFrame", adv_story::setOverStageFrame);
RW_FUNCTION(0x56E558, "AdvStageScrollView::setStoryTitle", adv_story::setStoryTitle);
RW_FUNCTION(0x56ECB0, "AdvStageScrollView::setStoryTitle (label lambda)", adv_story::setLabelLambda);
RW_FUNCTION(0x56E1CC, "AdvStageScrollView::setColosseumInfo (arrow button helper)", adv_story::setArrowButton);
