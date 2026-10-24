// SceneDrawRaise::createDrawMedal: the medal card of a drawn medal (DeckEdit_DrawMedal / DrawMedal2 layout):
// large image, rarity stars, skill icon, and the blinking "status bonus" plate with its LWF effect. Returns the
// cocostudio widget. The original never reads `this`.
//
// The original frame (0x188 bytes) is kept: getMedalData / SkillDataManager::getData / updateMedalRarePanel take
// stack arguments and the medal record's word at +0x2C is never initialised.
#include "SceneDrawRaise.hpp"

#include "core/SafeBuffer.hpp"

using namespace scene;

namespace {
constexpr uint32_t SceneUtil_searchNodeByName = 0x45A9F5;
constexpr uint32_t SceneUtil_updateMedalRarePanel = 0x4619F5;  // (panel, rare, align, float scale, [sp] bool small,
                                                               //  [sp+4] maxStars, [sp+8] zOrder)
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;
constexpr uint32_t MedalDataManager_getMedalLargeImagePath = 0x76B1E5;   // (mgr, shared_ptr by value) -> char*
constexpr uint32_t SkillDataManager_getInstance = 0x76BA0D;
constexpr uint32_t SkillDataManager_getData = 0x76BA59;        // (sret, mgr, u64 id, [sp] Skill (40 bytes), bool)
constexpr uint32_t SkillDataManager_getImagePath = 0x76BE8D;   // (mgr, shared_ptr by value) -> char*
constexpr uint32_t ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE;           // (image, const string&, TextureResType)
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;
constexpr uint32_t RepeatForever_create = rwlib::cocos2d::RepeatForever::create;

constexpr uint32_t MEDAL_RARE = 0x470;
constexpr uint32_t MEDAL_BONUS = 0x4B8;                        // master::Medal: non-zero = status bonus plate
constexpr uint32_t USERMEDAL_MASTER = 0x58;
constexpr uint32_t USERMEDAL_SKILL_IDS = 0x38;                 // u64 per skill
constexpr uint32_t USERMEDAL_SKILL_COUNT = 0x48;
constexpr uint32_t MEDAL_RECORD_SIZE = 0x48;

constexpr uint32_t STR_DrawMedal2 = 0xE04CD6;                  // "cocostudio/publish/DeckEdit_DrawMedal2_ver120.json"
constexpr uint32_t STR_DrawMedal = 0xE04D09;                   // "cocostudio/publish/DeckEdit_DrawMedal_ver120.json"
constexpr uint32_t STR_Dummy_Medal = 0xDF8C16;
constexpr uint32_t STR_Medal = 0xDFD05F;
constexpr uint32_t STR_Star_Area = 0xDF8C22;
constexpr uint32_t STR_Icon_Skill = 0xDF8C40;
constexpr uint32_t STR_Icon_Skil = 0xDF8F81;                   // the misspelt name of the small layout
constexpr uint32_t STR_Bonus_Plate = 0xE04D3B;
constexpr uint32_t STR_StatusBonusLwf = 0xE04D47;              // "lwf/raisedraw/demo/Draw_StatusBonus/Draw_StatusBonus.lwf"
constexpr uint32_t STR_Area_Point_BonusAnm = 0xE04D80;

inline void fst32(uint32_t a, uint32_t v) { *rw::frame_ptr<uint32_t>(a) = v; }
inline void releaseIf(uint32_t ctrl) {
    if (ctrl) call(fn::shared_release, {ctrl});
}
uint32_t findAt(uint32_t root, uint32_t tmp, uint32_t text, uint32_t len) {
    strInit(tmp, text, len);
    uint32_t node = call(SceneUtil_searchNodeByName, {root, tmp});
    strFree(tmp);
    return node;
}
// image->loadTexture(path) with the temporary string at `tmp`
void loadTexture(uint32_t image, uint32_t tmp, uint32_t path) {
    strInit(tmp, path, host_strlen(path));
    call(ImageView_loadTexture, {image, tmp, 0});
    strFree(tmp);
}
int32_t medalValue(uint32_t sp, uint32_t index) {
    return safe32(ld32(ld32(sp + 0x10C) + USERMEDAL_MASTER), index);
}
}  // namespace

namespace draw_raise_medal {
uint32_t createDrawMedal(uint32_t, uint32_t, uint32_t idLo, uint32_t idHi, uint32_t large, int32_t zOrder) {
    Frame f;
    const uint32_t sp = f.allocExact(0x188);

    uint32_t mgr = call(MedalDataManager_getInstance, {});
    const uint32_t local = sp + 0x120;
    for (uint32_t k = 0; k < MEDAL_RECORD_SIZE; k += 4)
        if (k != 0x2C) fst32(local + k, 0);
    fst32(sp, 0);
    for (uint32_t k = 0; k < MEDAL_RECORD_SIZE; k += 4) fst32(sp + 8 + k, ld32(local + k));
    rw::call_exact(MedalDataManager_getMedalData, {sp + 0x10C, mgr, idLo, idHi});

    uint32_t widget;
    {
        uint32_t reader = call(fn::GUIReader_getInstance, {});
        widget = call(fn::GUIReader_widgetFromJsonFile, {reader, large == 1 ? STR_DrawMedal2 : STR_DrawMedal});
        uint32_t image = large == 1 ? findAt(widget, sp + 0x100, STR_Dummy_Medal, 0xB)
                                    : findAt(widget, sp + 0xE0, STR_Medal, 5);
        uint32_t medalMgr = call(MedalDataManager_getInstance, {});
        uint32_t copy = large == 1 ? sp + 0xEC : sp + 0xCC;   // the shared_ptr passed by value
        fst32(copy, ld32(sp + 0x10C));
        uint32_t ctrl = ld32(sp + 0x110);
        fst32(copy + 4, ctrl);
        if (ctrl) call(fn::shared_add, {ctrl});
        uint32_t path = call(MedalDataManager_getMedalLargeImagePath, {medalMgr, copy});
        loadTexture(image, large == 1 ? sp + 0xF4 : sp + 0xD4, path);
        releaseIf(ld32(copy + 4));
    }

    // rarity stars
    int32_t rare = medalValue(sp, MEDAL_RARE);
    uint32_t stars = findAt(widget, sp + 0xC0, STR_Star_Area, 9);
    int32_t rare2 = medalValue(sp, MEDAL_RARE);
    fst32(sp, 0);
    fst32(sp + 4, uint32_t(rare));
    fst32(sp + 8, uint32_t(zOrder));
    uint32_t scale = large ? 0x3F800000u : 0x3F400000u;     // 1.0 / 0.75
    rw::call_exact(SceneUtil_updateMedalRarePanel, {stars, uint32_t(rare2), 1, scale});

    // skill icon: the image of the medal's last non-zero skill
    uint32_t icon = findAt(widget, sp + 0xB4, STR_Icon_Skill, 0xA);
    if (!icon) icon = findAt(widget, sp + 0xA8, STR_Icon_Skil, 9);
    vcall(icon, node::setVisible, {icon, 0});
    vcall(icon, node::setLocalZOrder, {icon, 1});
    uint32_t medal = ld32(sp + 0x10C);
    if (medal && int32_t(ld32(medal + USERMEDAL_SKILL_COUNT)) >= 1) {
        uint32_t offset = USERMEDAL_SKILL_IDS;
        for (int32_t i = 0;;) {
            uint32_t lo = ld32(medal + offset), hi = ld32(medal + offset + 4);
            if (lo | hi) {
                uint32_t skillMgr = call(SkillDataManager_getInstance, {});
                for (uint32_t k = 0; k < 0x2C; k += 4) fst32(sp + k, 0);   // Skill record + bool, all zero
                rw::call_exact(SkillDataManager_getData, {sp + 0xA0, skillMgr, lo, hi});
                uint32_t skill = ld32(sp + 0xA0), skillCtrl = ld32(sp + 0xA4);
                fst32(sp + 0xA0, 0);
                fst32(sp + 0xA4, 0);
                uint32_t skillMgr2 = call(SkillDataManager_getInstance, {});
                fst32(sp + 0x8C, skill);
                fst32(sp + 0x90, skillCtrl);
                if (skillCtrl) call(fn::shared_add, {skillCtrl});
                uint32_t path = call(SkillDataManager_getImagePath, {skillMgr2, sp + 0x8C});
                loadTexture(icon, sp + 0x94, path);
                releaseIf(ld32(sp + 0x90));
                releaseIf(skillCtrl);
            }
            offset += 8;
            ++i;
            medal = ld32(sp + 0x10C);
            if (!(i < int32_t(ld32(medal + USERMEDAL_SKILL_COUNT)))) break;
        }
        vcall(icon, node::setVisible, {icon, 1});
    }

    // status bonus plate: blinks forever, with the bonus LWF effect
    uint32_t plate = findAt(widget, sp + 0x80, STR_Bonus_Plate, 0xB);
    int32_t bonus = medalValue(sp, MEDAL_BONUS);
    if (bonus) {
        vcall(plate, node::setVisible, {plate, 1});
        uint32_t d1 = call(fn::DelayTime_create, {0x3F2A7EFA});   // 0.666
        uint32_t f1 = call(fn::FadeTo_create, {0x3EAA7EFA, 0x33});  // 0.333
        uint32_t f2 = call(fn::FadeTo_create, {0x3EAA7EFA, 0xFF});
        uint32_t d2 = call(fn::DelayTime_create, {0x3F2A7EFA});
        fst32(sp, 0);
        uint32_t seq = uint32_t(rw::call_exact(fn::Sequence_create, {d1, f1, f2, d2}));
        call(fn::Node_runAction, {plate, call(RepeatForever_create, {seq})});
        uint32_t lwf = call(AudioLWFNode_create, {STR_StatusBonusLwf, 0});
        uint32_t area = findAt(widget, sp + 0x74, STR_Area_Point_BonusAnm, 0x13);
        vcall(area, node::addChild, {area, lwf});
    } else {
        vcall(plate, node::setVisible, {plate, 0});
    }
    releaseIf(ld32(sp + 0x110));
    return widget;
}
}  // namespace draw_raise_medal
RW_FUNCTION(0x6914CC, "SceneDrawRaise::createDrawMedal", draw_raise_medal::createDrawMedal);
