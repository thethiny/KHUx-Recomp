// DeckMedalView::updateLayout(bool playEffect): draw the view from its medal and deck slot.
//
// In order: the medal picture with its marks; the slot header of the normal layout (frame, attribute and
// direction icons, the bonus multipliers, or the "slot opens at rank N" text of an empty slot); the skill icon;
// the info row, whose value depends on showType (sell price, attack, defense, level, cost, rarity stars, fusion
// circle); the bonus of a medal matching the slot; the fusion / medal-kind row; which of the blinking plates are
// shown; and the type-match effect (with its sound and pop when playEffect is set).
//
// The original keeps everything in one 17 KB frame. This rewrite runs on a frame of the same size because one
// frame address reaches the heap: the LWF movie-event map built on the stack (its root node's parent pointer is the
// map's own end node).
#include "DeckMedalView.hpp"

#include "MedalDataManager.hpp"
#include "../master/Burst.hpp"
#include "../master/Medal.hpp"
#include "../net/api/User.hpp"
#include "../net/api/UserSupport.hpp"

using namespace scene;
namespace api = hole::network::api;

namespace {
using UserMedal = MedalDataManager::UserMedalData;

constexpr uint32_t FRAME_SIZE = 0x43C0;                   // push {r4-r7, lr} + sub sp, #0x43AC
// the LWF movie-event map and what goes into it, at their original frame offsets
constexpr uint32_t OFF_HANDLER_MAP = 0xEC;                // std::map<string, function<void(LWF::Movie*)>>
constexpr uint32_t OFF_MOVIE_NAME = 0xF8;                 // std::string "_root"
constexpr uint32_t OFF_HANDLER = 0x740;                   // pair<const string, function<...>> (0x28 bytes)

// ---- callees
constexpr uint32_t Node_stopAllActions = 0x893815;
constexpr uint32_t String_assign = 0x48D811;              // std::string::assign(const char*, size_t)
constexpr uint32_t Memcpy = 0x40E164;                     // __aeabi_memcpy (import)
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalSmallImagePath = 0x76B17D;   // (mgr, shared_ptr<UserMedalData>)
constexpr uint32_t MedalDataManager_getMedalLargeImagePath = 0x76B1E5;
constexpr uint32_t MedalDataManager_isFavoriteMedal = 0x76B249;
constexpr uint32_t SkillDataManager_getInstance = 0x76BA0D;
constexpr uint32_t SkillDataManager_getData = 0x76BA59;   // (out, mgr, u64 id, UserSkill::Skill by value, bool)
constexpr uint32_t SkillDataManager_getImagePath = 0x76BE8D;
constexpr uint32_t AppState_getNewUserMedalIDs = 0x4EB1BD;    // -> vector<u64>*
constexpr uint32_t Burst_create = 0x77A491;               // master::Burst::create(shared_ptr out, int id)
constexpr uint32_t LWFNode_create = 0x7EDF31;             // (const char* path, void*)
constexpr uint32_t LWF_AddMovieEventHandler = 0x808645;   // (lwf, const string& movie, const map& handlers)
constexpr uint32_t HandlerMap_insert = 0x530869;          // map::__insert_unique(hint, const pair&)
constexpr uint32_t HandlerMap_destroy = 0x530811;         // __tree::destroy(node)
constexpr uint32_t Vec2_ctorXY = 0x8E4B9D;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t ScaleTo_create = 0x86A8CD;             // (float duration, float scale)
constexpr uint32_t Sequence_create = 0x867999;            // (action, ..., NULL)
constexpr uint32_t SceneUtil_updateMixCircle = 0x465C85;  // (circle, icon, plate, shared_ptr medal, 7 flags)
constexpr uint32_t AudioManager_getInstance = 0x5BECFD;
constexpr uint32_t AudioManager_getLayer = 0x5BF33D;      // (shared_ptr<Layer> out, mgr, int layer)
constexpr uint32_t AudioLayer_playSE = 0x5BF785;          // (layer, ResourceID, int, int)

constexpr uint32_t AUDIO_LAYER_SE = 10000;
constexpr uint32_t SE_TYPE_MATCH = 0x1E56BA6C;
constexpr uint32_t SE_TYPE_UNMATCH = 0x1E56BA62;

constexpr uint32_t APIManager_userSupport = 0x114;        // shared_ptr<UserSupport>: the player's support medal
constexpr uint32_t APIManager_userDetail = 0x94;          // shared_ptr<UserDetail>
constexpr uint32_t Director_animationInterval = 0xA8;     // double
constexpr uint32_t LWFNode_lwf = 0x3E0;                   // LWF::LWF*

// ---- virtual slots
constexpr uint32_t VSLOT_setTouchEnabled = 0x29C;         // ui::Widget

// ---- updateLayout's $_0 (DeckMedalView.cpp): enterFrame handler [LWF node, medal image, view, type]
constexpr uint32_t VT_TypeMatchMovie = 0xF57388 + 8;

// ---- text table ids
constexpr int32_t TEXT_SLOT_OPENS_AT_RANK = 101200024;    // "%d" format
constexpr int32_t TEXT_SLOT_LOCKED = 101200034;
constexpr int32_t TEXT_LEVEL_LABEL = 101200006;
constexpr int32_t TEXT_COST_LABEL = 101200009;
constexpr int32_t TEXT_NO_EFFECT = 101200037;
constexpr int32_t TEXT_IN_DECK = 101210004;
constexpr int32_t TEXT_KIND_A = 101210001;                // medal kinds 2, 4 (enhance), 5 and 8
constexpr int32_t TEXT_KIND_3 = 101210002;
constexpr int32_t TEXT_KIND_6 = 101210003;

// ---- strings (address, length)
struct Lit { uint32_t addr, len; };
constexpr Lit S_Medal{0xDFD05F, 5}, S_Icon_New{0xDFDD8A, 8}, S_Icon_Lock{0xDFDD93, 9};
constexpr Lit S_Area_OpenInfo{0xDFDD7C, 13}, S_Txt_OpenInfo{0xDFDF07, 12};
constexpr Lit S_MedalBase{0xDFDD9D, 9}, S_TYpe{0xDFDDA7, 4}, S_Slot{0xDFDDAC, 4}, S_SlotNum{0xDFDDB1, 7};
constexpr Lit S_Dummy_Mag{0xDFDDB9, 9}, S_Dummy_Mag2{0xDFDD63, 10}, S_base_Type{0xDFDDC3, 9};
constexpr Lit S_Txt_Mag_Label{0xDFDDCD, 13}, S_Icon_Type{0xDFDDE4, 9};
constexpr Lit S_Icon_Skill{0xDF8C40, 10}, S_Icon_Skil{0xDF8F81, 9};
constexpr Lit S_Dummy{0xDFDF14, 5}, S_Icon_1{0xDFDF1A, 6}, S_Txt_1A{0xDFDF21, 6}, S_Txt_1B{0xDFDF28, 6};
constexpr Lit S_Icon_2{0xDFDF2F, 6}, S_Txt_2{0xDFDF36, 5}, S_avatarCoinPanel{0xDFDF3C, 15};
constexpr Lit S_Icon_3{0xDFDF4C, 6}, S_Txt_3{0xDFDF53, 5}, S_Icon_4{0xDFDF59, 6}, S_Icon_Arrow{0xDFDF60, 10};
constexpr Lit S_Txt_4{0xDFDF6B, 5}, S_Star_Area{0xDF8C22, 9}, S_Star_Area2{0xDFDF71, 10};
constexpr Lit S_Dummy_Guilt_1{0xDFDD6E, 13}, S_Area_MixCircle{0xDF8B8A, 14};
constexpr Lit S_Area_Guilt_Icon{0xDF8B99, 15}, S_Area_Guilt_Plate{0xDF8BA9, 16};
constexpr Lit S_Panel_S{0xDFDF7C, 7}, S_Panel_M{0xDFDF84, 7}, S_Panel_L{0xDFDF8C, 7}, S_Panel_XL{0xDFDF94, 8};
constexpr Lit S_Area_Point{0xDF9FB4, 10}, S_TypeMatchEffect{0xDFE0FE, 15};
constexpr Lit S_root{0xDFE0ED, 5}, S_enterFrame{0xDFE0F3, 10};

constexpr Lit IMG_Medal_L_Blank{0xDFDEEE, 24};
constexpr Lit IMG_Medal_UL{0xDFDDEE, 33}, IMG_Medal_RL{0xDFDE2D, 33}, IMG_Medal_L{0xDFDE6C, 32};
constexpr Lit IMG_Mark_Uplight{0xDFDE10, 28}, IMG_Mark_Reverse{0xDFDE4F, 28};
constexpr Lit IMG_Deck_Medal[3] = {{0xDFDEA9, 22}, {0xDFDEC0, 22}, {0xDFDED7, 22}};
constexpr Lit IMG_KeyBlade_type[3] = {{0xDF88C6, 39}, {0xDF88EE, 39}, {0xDF8916, 39}};
constexpr Lit IMG_Minfo_Def01{0xDFE019, 22}, IMG_Minfo_Def02{0xDFE078, 22}, IMG_Minfo_Def03{0xDFE061, 22};
constexpr Lit IMG_Minfo_Def04A{0xDFDFD1, 23}, IMG_Minfo_Def04B{0xDFDFE9, 23}, IMG_Minfo_Def04C{0xDFE001, 23};
constexpr Lit IMG_Minfo_Def09{0xDFE0A6, 22}, IMG_Minfo_Def13{0xDFE08F, 22};
constexpr uint32_t FMT_Slot_Num = 0xDFDE8D;               // "img/ui/Deck_Slot_Num_%d.png"
constexpr uint32_t FMT_Arrow_U = 0xDFDF9D;                // "img/ui/Deck_Arrow_U%d.png"
constexpr uint32_t FMT_Arrow_R = 0xDFDFB7;                // "img/ui/Deck_Arrow_R%d.png"
constexpr uint32_t FMT_Ratio = 0xDFDDDB;                  // "×%01.2f"
constexpr uint32_t FMT_Int = 0xDF86DE;                    // "%d"
constexpr uint32_t FMT_TypeMatchLwf = 0xDFE0BD;           // "lwf/medal/medaltype/MedalType%d/MedalType%d.lwf"
constexpr uint32_t JSON_MedalInfo2 = 0xDFE030;            // "cocostudio/publish/DeckEdit_Deck_MedalInfo2.json"

// ------------------------------------------------------------------------------------------ helpers
inline uint32_t find(uint32_t root, Lit name) { return findNode(root, name.addr, name.len); }
inline void setVisible(uint32_t n, uint32_t v) { vcall(n, node::setVisible, {n, v}); }
inline void setOpacity(uint32_t n, uint32_t v) { vcall(n, node::setOpacity, {n, v}); }
inline void showWithOpacity(uint32_t n, uint32_t opacity) {
    setVisible(n, 1);
    setOpacity(n, opacity);
}
inline void loadTexture(uint32_t image, Lit path) {
    Frame f;
    uint32_t s = newStr(f, path.addr, path.len);
    call(fn::ImageView_loadTexture, {image, s, 0});
    strFree(s);
}
inline void loadTextureC(uint32_t image, uint32_t path) {
    Frame f;
    uint32_t s = newStr(f, path);
    call(fn::ImageView_loadTexture, {image, s, 0});
    strFree(s);
}
// text = "×1.50" for a multiplier stored x10000
inline void setRatioText(uint32_t text, int32_t x10000) {
    Frame f;
    double ratio = double(float(x10000) / 10000.0f);
    uint64_t bits;
    std::memcpy(&bits, &ratio, 8);
    setTextC(text, stringBuffer(f, {FMT_Ratio, uint32_t(bits), uint32_t(bits >> 32)}));
}
inline void setIntText(uint32_t text, int32_t v) {
    Frame f;
    setTextC(text, stringBuffer(f, {FMT_Int, uint32_t(v)}));
}
inline void setSize(uint32_t n, float w, float h) {
    Frame f;
    vcall(n, node::setContentSize, {n, newVec2(f, w, h)});
}
// The info-row layout: the text `width` x 20, at half the icon's width plus dx.
inline void placeText(uint32_t text, uint32_t icon, float width, float dx) {
    setSize(text, width, 20.0f);
    float iconWidth = ldf(vcall(icon, node::getContentSize, {icon}));
    vcall(text, node::setPositionX, {text, fbits(iconWidth * 0.5f + dx)});
}
// a by-value shared_ptr<UserMedalData> argument (the caller releases the copy after the call)
inline uint32_t medalCopy(Frame &f, const DeckMedalView *self) {
    uint32_t sp = f.alloc(8);
    st32(sp, self->medal);
    st32(sp + 4, self->medalCtl);
    if (self->medalCtl) call(fn::shared_add, {self->medalCtl});
    return sp;
}
inline void releaseIf(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
inline void stopPlate(rw::Field<uint32_t> &plate) {
    if (plate) {
        call(Node_stopAllActions, {plate});
        plate = 0;
    }
}
inline uint32_t medalRow(const UserMedal *m) { return m->medal.obj; }   // master::Medal (a SafeBuffer)

// the info panels of both layouts
struct InfoNodes {
    uint32_t dummy;
    uint32_t icon1, txt1A, txt1B;            // the showType value
    uint32_t icon2, txt2;                    // fusion / medal-kind row
    uint32_t icon3, txt3;                    // attribute bonus
    uint32_t icon4, arrow, txt4;             // attribute + direction bonus
    uint32_t starArea, guilt;
    uint32_t panelS, panelM, panelXL;
};

// ------------------------------------------------------------------------------------------ medal picture
// The medal picture, the frame of the player's support medal, the "new" mark and the favourite lock.
void drawMedalImage(DeckMedalView *self, uint32_t image, uint32_t iconNew, uint32_t iconLock) {
    Frame f;
    uint32_t path = f.alloc(12);
    st32(path, 0);
    st32(path + 4, 0);
    st32(path + 8, 0);
    uint32_t mgr = call(MedalDataManager_getInstance, {});
    uint32_t copy = medalCopy(f, self);
    uint32_t file = call(self->isSmall ? MedalDataManager_getMedalSmallImagePath
                                       : MedalDataManager_getMedalLargeImagePath, {mgr, copy});
    call(String_assign, {path, file, host_strlen(file)});
    releaseIf(ld32(copy + 4));
    call(fn::ImageView_loadTexture, {image, path, 0});
    setVisible(image, 1);

    uint32_t apiMgr = call(fn::APIManager_getInstance, {});
    uint32_t supportCtl = ld32(apiMgr + APIManager_userSupport + 4);
    uint32_t support = ld32(apiMgr + APIManager_userSupport);
    if (supportCtl) call(fn::shared_add, {supportCtl});
    const UserMedal *m = rw::host_ptr<UserMedal>(self->medal);
    uint32_t idLo = m->userMedalIdLo, idHi = m->userMedalIdHi;
    uint32_t supportId = safeBytes(f, support, api::UserSupport::USER_MEDAL_ID, 8);
    uint32_t small = self->isSmall != 0;
    if (ld32(supportId) == idLo && ld32(supportId + 4) == idHi)
        SceneUtil::showSupportMedalEffect(image, small);
    else
        SceneUtil::hideSupportMedalEffect(image, small);

    uint32_t ids = call(AppState_getNewUserMedalIDs, {call(fn::AppState_getInstance, {})});
    uint32_t it = ld32(ids), end = ld32(ids + 4);
    if (it != end) {
        uint32_t lo = ld32(self->medal), hi = ld32(self->medal + 4);
        for (; it != end; it += 8)
            if (ld32(it) == lo && ld32(it + 4) == hi) break;
    }
    setVisible(iconNew, it != end ? 1 : 0);

    uint32_t favMgr = call(MedalDataManager_getInstance, {});
    uint32_t favCopy = medalCopy(f, self);
    setVisible(iconLock, call(MedalDataManager_isFavoriteMedal, {favMgr, favCopy}));
    releaseIf(ld32(favCopy + 4));
    if (self->darklightMag && self->magnification >= 10001) setVisible(self->darklightMag, 0);
    releaseIf(supportCtl);
    strFree(path);
}

// ------------------------------------------------------------------------------------------ slot header
// The normal layout's header: the slot's frame (by direction), its number, its attribute icons and the bonus
// multipliers - or, for an empty slot (slotIndex -1), the blank frame and the "opens at rank" text.
void drawSlotHeader(DeckMedalView *self, uint32_t s, uint32_t openInfo, bool hasMedal) {
    uint32_t medalBase = find(s, S_MedalBase);
    uint32_t typeIcon = find(s, S_TYpe);
    uint32_t slot = find(s, S_Slot);
    uint32_t slotNum = find(s, S_SlotNum);
    uint32_t mag = find(s, S_Dummy_Mag);
    uint32_t magBase = find(mag, S_base_Type);
    uint32_t magLabel = find(mag, S_Txt_Mag_Label);
    SceneUtil::setShadow(magLabel);
    setRatioText(magLabel, self->attributeBonus);
    uint32_t mag2 = find(s, S_Dummy_Mag2);
    find(mag2, S_base_Type);
    uint32_t mag2Label = find(mag2, S_Txt_Mag_Label);
    SceneUtil::setShadow(mag2Label);
    uint32_t mag2Icon = find(mag2, S_Icon_Type);
    setVisible(mag2, 0);
    uint32_t keybladeType = find(s, S_Icon_Type);

    if (self->slotIndex == -1) {
        for (uint32_t n : {typeIcon, slot, slotNum, magBase, keybladeType, magLabel}) setVisible(n, 0);
        if (!self->field_3BA) {
            setVisible(medalBase, 0);
            return;
        }
        setVisible(medalBase, 1);
        loadTexture(medalBase, IMG_Medal_L_Blank);
        setVisible(openInfo, 1);
        uint32_t text = find(openInfo, S_Txt_OpenInfo);
        SceneUtil::setShadow(text);
        if (self->slotOpenRank >= 1) {
            Frame f;
            uint32_t entry = textData(f, TEXT_SLOT_OPENS_AT_RANK);
            setTextC(text, stringBuffer(f, {textDataStr(entry), uint32_t(self->slotOpenRank)}));
            sharedRelease(entry);
        } else {
            setTextId(text, TEXT_SLOT_LOCKED);
        }
        return;
    }

    for (uint32_t n : {typeIcon, slot, slotNum, magBase, keybladeType, magLabel}) setVisible(n, 1);
    switch (self->slotDirection) {
    case 1:
    case 2:
        loadTexture(medalBase, self->slotDirection == 1 ? IMG_Medal_UL : IMG_Medal_RL);
        setRatioText(mag2Label, self->magnification);
        // (the direction bonus shows only without a real medal)
        if (!hasMedal) setVisible(mag2, 1);
        loadTexture(mag2Icon, self->slotDirection == 1 ? IMG_Mark_Uplight : IMG_Mark_Reverse);
        break;
    case 3:
        loadTexture(medalBase, IMG_Medal_L);
        break;
    default:
        break;
    }
    {
        Frame f;
        loadTextureC(slotNum, stringBuffer(f, {FMT_Slot_Num, uint32_t(self->slotIndex + 1)}));
    }
    switch (self->slotAttribute) {
    case 1:
    case 2:
    case 3:
        loadTexture(typeIcon, IMG_Deck_Medal[self->slotAttribute - 1]);
        loadTexture(keybladeType, IMG_KeyBlade_type[self->slotAttribute - 1]);
        break;
    case 4:
        for (uint32_t n : {typeIcon, magBase, keybladeType, magLabel}) setVisible(n, 0);
        break;
    default:
        break;
    }
}

// ------------------------------------------------------------------------------------------ skill icon
// shared_ptr<UserSkillData> of a skill id: from the skill list given to setData (the skill's record) when there is
// one - falling back to the empty skill when the id is not in it - else by id alone.
void skillData(DeckMedalView *self, uint32_t idLo, uint32_t idHi, uint32_t &obj, uint32_t &ctl) {
    Frame f;
    uint32_t out = f.alloc(8);
    uint32_t list = self->skills;
    if (!list) {
        uint32_t mgr = call(SkillDataManager_getInstance, {});
        call(SkillDataManager_getData, {out, mgr, idLo, idHi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
        obj = ld32(out);
        ctl = ld32(out + 4);
        return;
    }
    ctl = 0;
    for (uint32_t rec = ld32(list), end = ld32(list + 4); rec != end; rec += 0x28) {
        if (ld32(rec) != idLo || ld32(rec + 4) != idHi) continue;
        uint32_t w[10];
        for (uint32_t k = 0; k < 10; ++k) w[k] = ld32(rec + 4 * k);
        uint32_t mgr = call(SkillDataManager_getInstance, {});
        call(SkillDataManager_getData, {out, mgr, 0, 0, w[0], w[1], w[2], w[3], w[4], w[5], w[6], w[7], w[8], w[9], 0});
        obj = ld32(out);
        ctl = ld32(out + 4);
        if (obj) return;
        break;
    }
    uint32_t mgr = call(SkillDataManager_getInstance, {});
    call(SkillDataManager_getData, {out, mgr, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
    obj = ld32(out);
    releaseIf(ctl);
    ctl = ld32(out + 4);
}

// The skill icon shows the last of the medal's skills (when it has any).
void drawSkillIcon(DeckMedalView *self, uint32_t s) {
    uint32_t icon = find(s, S_Icon_Skill);
    if (!icon) icon = find(s, S_Icon_Skil);
    setVisible(icon, 0);
    vcall(icon, node::setLocalZOrder, {icon, 1});
    if (!self->medal) return;
    const UserMedal *m = rw::host_ptr<UserMedal>(self->medal);
    if (int32_t(m->skillCount) < 1) return;
    for (int32_t i = 0; i < int32_t(m->skillCount); ++i) {
        uint32_t id = self->medal + 0x38 + 8 * uint32_t(i);   // userSkillIds (the count is not capped at 2)
        uint32_t idLo = ld32(id), idHi = ld32(id + 4);
        if (!idLo && !idHi) continue;
        uint32_t obj, ctl;
        skillData(self, idLo, idHi, obj, ctl);
        uint32_t mgr = call(SkillDataManager_getInstance, {});
        Frame f;
        uint32_t copy = f.alloc(8);
        st32(copy, obj);
        st32(copy + 4, ctl);
        if (ctl) call(fn::shared_add, {ctl});
        uint32_t file = call(SkillDataManager_getImagePath, {mgr, copy});
        loadTextureC(icon, file);
        releaseIf(ld32(copy + 4));
        releaseIf(ctl);
    }
    setVisible(icon, 1);
}

// ------------------------------------------------------------------------------------------ info panels
// Find the info panels, reset them (hidden, shadows, the fixed text sizes and positions).
InfoNodes findInfoNodes(uint32_t s) {
    InfoNodes n;
    n.dummy = find(s, S_Dummy);
    setVisible(n.dummy, 1);
    n.icon1 = find(n.dummy, S_Icon_1);
    setVisible(n.icon1, 0);
    n.txt1A = find(n.icon1, S_Txt_1A);
    SceneUtil::setShadow(n.txt1A);
    n.txt1B = find(n.icon1, S_Txt_1B);
    float height = ldf(vcall(n.txt1B, node::getContentSize, {n.txt1B}) + 4);
    setSize(n.txt1B, 50.0f, height);
    SceneUtil::setShadow(n.txt1B);
    n.icon2 = find(n.dummy, S_Icon_2);
    setVisible(n.icon2, 0);
    n.txt2 = find(n.icon2, S_Txt_2);
    SceneUtil::setStatusChangeColor(n.txt2);
    SceneUtil::setShadow(n.txt2);
    uint32_t coin = find(s, S_avatarCoinPanel);
    if (coin) setVisible(coin, 0);
    n.icon3 = find(n.dummy, S_Icon_3);
    setVisible(n.icon3, 0);
    n.txt3 = find(n.dummy, S_Txt_3);
    SceneUtil::setShadow(n.txt3);
    float icon1Width = ldf(vcall(n.icon1, node::getContentSize, {n.icon1}));
    vcall(n.txt3, node::setPositionX, {n.txt3, fbits(icon1Width * 0.5f + -30.0f)});
    n.icon4 = find(n.dummy, S_Icon_4);
    setVisible(n.icon4, 0);
    n.arrow = find(n.dummy, S_Icon_Arrow);
    n.txt4 = find(n.dummy, S_Txt_4);
    SceneUtil::setShadow(n.txt4);
    n.starArea = find(s, S_Star_Area);
    vcall(n.starArea, VSLOT_setTouchEnabled, {n.starArea, 0});
    setVisible(n.starArea, 0);
    uint32_t stars2 = find(s, S_Star_Area2);
    vcall(stars2, VSLOT_setTouchEnabled, {stars2, 0});
    setVisible(stars2, 0);
    n.guilt = find(s, S_Dummy_Guilt_1);
    vcall(n.guilt, VSLOT_setTouchEnabled, {n.guilt, 0});
    setVisible(n.guilt, 0);
    n.panelS = find(s, S_Panel_S);
    setVisible(n.panelS, 0);
    n.panelM = find(s, S_Panel_M);
    setVisible(n.panelM, 0);
    return n;
}

// ------------------------------------------------------------------------------------------ slot bonus
// A medal whose attribute matches the slot: the bonus icon and multiplier (attribute only, or attribute and
// direction with the direction arrow). Returns the multiplier the attack value uses; `type` gets the medal type
// of the effect.
int32_t drawSlotBonus(DeckMedalView *self, const InfoNodes &n, int32_t attribute, int32_t direction,
                      int32_t &type) {
    self->typeMatched = 1;
    self->bonusIcon = n.icon3;
    self->bonusPanel = n.panelS;
    if (self->slotDirection == 3 || direction != self->slotDirection) {
        int32_t bonus = self->attributeBonus;
        type = 1;
        if (attribute == 1 || attribute == 2 || attribute == 3) {
            loadTexture(n.icon3, attribute == 1 ? IMG_Minfo_Def04A : attribute == 2 ? IMG_Minfo_Def04B
                                                                                  : IMG_Minfo_Def04C);
            type = attribute;
        }
        setRatioText(n.txt3, bonus);
        return bonus;
    }
    int32_t bonus = self->magnification;
    self->bonusIcon = n.icon4;
    self->bonusPanel = n.panelXL;
    type = attribute;
    if (direction == 1 || direction == 2) {
        Frame f;
        loadTextureC(n.arrow, stringBuffer(f, {direction == 1 ? FMT_Arrow_U : FMT_Arrow_R, uint32_t(attribute)}));
    }
    setRatioText(n.txt4, bonus);
    return bonus;
}

// ------------------------------------------------------------------------------------------ info row
// showType 1, sell price view: the sphere point panel of a medal whose burst is maxed.
void drawCoinPanel(DeckMedalView *self, uint32_t s, const UserMedal *m) {
    Frame f;
    uint32_t burst = masterRow(f, Burst_create, uint32_t(safe32(medalRow(m), master::Medal::BURST_ID)));
    uint32_t row = ld32(burst);
    if (row) {
        int32_t maxCount = safe32(row, master::Burst::MAX_ENHANCE_COUNT);
        if (maxCount >= 1 && m->burstUpperNumber == maxCount) {
            self->coinPanel = find(s, S_avatarCoinPanel);
            if (!self->coinPanel) {
                uint32_t reader = call(fn::GUIReader_getInstance, {});
                self->coinPanel = call(fn::GUIReader_widgetFromJsonFile, {reader, JSON_MedalInfo2});
                uint32_t area = find(s, S_Area_Point);
                uint32_t panel = self->coinPanel;
                Frame nf;
                uint32_t name = newStr(nf, S_avatarCoinPanel.addr, S_avatarCoinPanel.len);
                vcall(area, node::addChildZName, {area, panel, 0, name});
                strFree(name);
            }
            setVisible(self->coinPanel, 1);
            uint32_t text = find(self->coinPanel, S_Txt_4);
            setIntText(text, safe32(medalRow(m), master::Medal::SELL_SPHERE_POINT));
        }
    }
    sharedRelease(burst);
}

// The value the info row shows (showType), and which of its nodes blink as status plates.
void drawInfoRow(DeckMedalView *self, uint32_t s, const InfoNodes &n, int32_t attackBonus) {
    const UserMedal *m = rw::host_ptr<UserMedal>(self->medal);
    switch (uint32_t(self->showType)) {
    case 0:
        self->statusIcon = 0;
        self->statusPanel = 0;
        return;
    case 1: {                                   // sell price
        loadTexture(n.icon1, IMG_Minfo_Def01);
        placeText(n.txt1A, n.icon1, 77.0f, 33.0f);
        {
            Frame f;
            uint32_t money = f.alloc(12);
            SceneUtil::getMoneyStr(money, uint64_t(int64_t(m->sellPrice)));
            call(fn::Text_setString, {n.txt1A, money});
            strFree(money);
        }
        SceneUtil::setNormalColor(n.txt1A);
        setVisible(n.txt1B, 0);
        self->statusIcon = n.icon1;
        self->statusPanel = n.panelM;
        if (self->showCoinPanel) drawCoinPanel(self, s, m);
        return;
    }
    case 2: {                                   // attack (with the slot bonus when the attribute matches)
        loadTexture(n.icon1, IMG_Minfo_Def03);
        int32_t attack = m->attack;
        placeText(n.txt1A, n.icon1, 49.0f, 33.0f);
        int32_t value = attack;
        if (self->typeMatched) {
            SceneUtil::setStatusChangeColor(n.txt1A);
            value = int32_t(int64_t(attack) * int64_t(attackBonus) / 10000);
        } else {
            SceneUtil::setNormalColor(n.txt1A);
        }
        setIntText(n.txt1A, value);
        break;
    }
    case 3:                                     // defense
        loadTexture(n.icon1, IMG_Minfo_Def02);
        placeText(n.txt1A, n.icon1, 49.0f, 33.0f);
        setIntText(n.txt1A, m->defense);
        SceneUtil::setNormalColor(n.txt1A);
        break;
    case 4:                                     // level
        loadTexture(n.icon1, IMG_Minfo_Def13);
        placeText(n.txt1A, n.icon1, 37.0f, 33.0f);
        setIntText(n.txt1A, m->level);
        SceneUtil::setNormalColor(n.txt1A);
        setVisible(n.txt1B, 1);
        placeText(n.txt1B, n.icon1, 30.0f, -33.0f);
        setTextId(n.txt1B, TEXT_LEVEL_LABEL);
        self->statusIcon = n.icon1;
        self->statusPanel = n.panelS;
        return;
    case 5:                                     // deck cost
        loadTexture(n.icon1, IMG_Minfo_Def13);
        placeText(n.txt1A, n.icon1, 25.0f, 37.0f);
        setIntText(n.txt1A, int32_t(uint32_t(safe32(medalRow(m), master::Medal::COST)) - uint32_t(m->upperCost)));
        SceneUtil::setNormalColor(n.txt1A);
        setVisible(n.txt1B, 1);
        placeText(n.txt1B, n.icon1, 50.0f, -37.0f);
        setTextId(n.txt1B, TEXT_COST_LABEL);
        self->statusIcon = n.icon1;
        self->statusPanel = n.panelS;
        return;
    case 6:                                     // rarity stars
        SceneUtil::updateMedalRarePanel(n.starArea, safe32(medalRow(m), master::Medal::RARE), 1, bitsf(0x3F19999A),
                                        1, 0, 0);
        self->statusIcon = n.starArea;
        self->statusPanel = 0;
        return;
    case 7: {                                   // fusion circle
        uint32_t circle = find(n.guilt, S_Area_MixCircle);
        vcall(circle, node::setPositionX, {circle, rw::get<uint32_t>(self, 0x384)});
        uint32_t icon = find(n.guilt, S_Area_Guilt_Icon);
        if (icon) setVisible(icon, 0);
        uint32_t plate = find(n.guilt, S_Area_Guilt_Plate);
        if (plate) setVisible(plate, 0);
        uint32_t apiMgr = call(fn::APIManager_getInstance, {});
        Frame f;
        uint32_t copy = medalCopy(f, self);
        uint32_t detail = ld32(apiMgr + APIManager_userDetail);
        uint8_t isGuilt = ld8(safeBytes(f, detail, api::UserDetail::IS_GUILT, 1));
        call(SceneUtil_updateMixCircle, {circle, icon, plate, copy, isGuilt ? 1u : 0u, 0, 1, 1, 0, 0, 1});
        releaseIf(ld32(copy + 4));
        self->statusIcon = n.guilt;
        self->statusPanel = n.panelM;
        return;
    }
    default:
        return;
    }
    // attack and defense
    setVisible(n.txt1B, 0);
    self->statusIcon = n.icon1;
    self->statusPanel = n.panelS;
}

// ------------------------------------------------------------------------------------------ fusion / kind row
void drawMixRow(DeckMedalView *self, const InfoNodes &n, Lit icon, float width, int32_t textId, uint32_t panel) {
    loadTexture(n.icon2, icon);
    placeText(n.txt2, n.icon2, width, 0.0f);
    setTextId(n.txt2, textId);
    self->mixIcon = n.icon2;
    self->mixPanel = panel;
}

// The second row: "in deck" (or picked in the fusion picker), "no effect", or what the medal kind does.
void drawKindRow(DeckMedalView *self, const InfoNodes &n) {
    const UserMedal *m = rw::host_ptr<UserMedal>(self->medal);
    self->mixIcon = 0;
    uint8_t inDeck = self->mixMode ? self->mixSelected : m->inDeck;
    if (inDeck) {
        drawMixRow(self, n, IMG_Minfo_Def13, 100.0f, TEXT_IN_DECK, n.panelM);
    } else if (self->noEffect) {
        drawMixRow(self, n, IMG_Minfo_Def13, 100.0f, TEXT_NO_EFFECT, n.panelM);
        return;
    }
    int32_t kind = safe32(medalRow(m), master::Medal::TYPE);
    switch (kind) {
    case 2:
    case 5:
        drawMixRow(self, n, IMG_Minfo_Def13, 100.0f, TEXT_KIND_A, n.panelS);
        break;
    case 3:
        drawMixRow(self, n, IMG_Minfo_Def09, 60.0f, TEXT_KIND_3, n.panelS);
        break;
    case 4:                                     // enhancement medals: one row each for attack / defense
        if (safe32(medalRow(m), master::Medal::ENHANCE_ATTACK) >= 1)
            drawMixRow(self, n, IMG_Minfo_Def13, 100.0f, TEXT_KIND_A, n.panelS);
        if (safe32(medalRow(m), master::Medal::ENHANCE_DEFENSE) >= 1)
            drawMixRow(self, n, IMG_Minfo_Def13, 100.0f, TEXT_KIND_A, n.panelS);
        break;
    case 6:
        drawMixRow(self, n, IMG_Minfo_Def13, 80.0f, TEXT_KIND_6, n.panelS);
        break;
    case 8:
        drawMixRow(self, n, IMG_Minfo_Def13, 50.0f, TEXT_KIND_A, n.panelS);
        break;
    default:
        break;
    }
}

// ------------------------------------------------------------------------------------------ plates
// Show the plates DeckMedalViewManager blinks, at their starting opacity.
void showPlates(DeckMedalView *self) {
    if (self->isSmall) {
        setVisible(self->largePanel, 1);
        if (self->isShowMixInfo && self->mixIcon) {
            showWithOpacity(self->mixIcon, 0);
            showWithOpacity(self->mixPanel, 0);
            showWithOpacity(self->statusIcon, 0);
            if (self->statusPanel) showWithOpacity(self->statusPanel, 0);
            if (self->coinPanel) showWithOpacity(self->coinPanel, 0);
        } else if (self->statusIcon) {
            showWithOpacity(self->statusIcon, 0xFF);
            if (self->statusPanel) showWithOpacity(self->statusPanel, 0);
            if (self->coinPanel) showWithOpacity(self->coinPanel, 0);
        }
        return;
    }
    setVisible(self->largePanel, 0);
    bool status = self->field_38A && self->statusIcon;
    if (self->typeMatched) {
        showWithOpacity(self->bonusIcon, 0xFF);
        showWithOpacity(self->bonusPanel, 0xFF);
        if (!status) return;
        showWithOpacity(self->statusIcon, 0);
        if (self->statusPanel) showWithOpacity(self->statusPanel, 0);
        if (self->coinPanel) showWithOpacity(self->coinPanel, 0);
    } else if (status) {
        showWithOpacity(self->statusIcon, 0xFF);
        if (self->statusPanel) showWithOpacity(self->statusPanel, 0xFF);
        if (self->coinPanel) showWithOpacity(self->coinPanel, 0xFF);
    }
}

// ------------------------------------------------------------------------------------------ type-match effect
void removeTypeMatchEffect(uint32_t s) {
    uint32_t effect = find(s, S_TypeMatchEffect);
    if (effect) vcall(effect, node::removeFromParent, {effect});
}

// The type-match pop: the medal type's LWF over the medal picture (its enterFrame handler, $_0, fades it out and
// starts the looping effect), the sound, and the bonus icon scaling up and back.
void playTypeMatch(DeckMedalView *self, uint32_t base, uint32_t image, int32_t type) {
    self->typeMatchPlaying = 1;
    uint32_t lwf;
    {
        Frame f;
        lwf = call(LWFNode_create, {stringBuffer(f, {FMT_TypeMatchLwf, uint32_t(type), uint32_t(type)}), 0});
    }
    {
        Frame f;
        float width = ldf(vcall(image, node::getContentSize, {image}));
        uint32_t size = vcall(image, node::getContentSize, {image});
        float x = width * 0.5f;
        float y = ldf(size + 4) * 0.5f;
        uint32_t pos = f.alloc(8);
        call(Vec2_ctorXY, {pos, fbits(x), fbits(y)});
        vcall(lwf, node::setPosition, {lwf, pos});
        call(Vec2_dtor, {pos});
    }
    float scale = bitsf(vcall(image, node::getScale, {image}));
    vcall(lwf, node::setScale, {lwf, fbits(1.0f / scale)});
    vcall(image, node::addChild, {image, lwf});
    {
        Frame f;
        uint32_t layer = f.alloc(8);
        call(AudioManager_getLayer, {layer, call(AudioManager_getInstance, {}), AUDIO_LAYER_SE});
        call(AudioLayer_playSE, {ld32(layer), SE_TYPE_MATCH, 0, 0});
        sharedRelease(layer);
    }
    uint32_t dir = call(fn::Director_getInstance, {});
    double interval;
    std::memcpy(&interval, rw::host_ptr<uint8_t>(dir + Director_animationInterval), 8);
    call(Node_stopAllActions, {self->bonusIcon});
    uint32_t icon = self->bonusIcon;
    uint32_t up = call(ScaleTo_create, {fbits(float(interval * 4.0)), fbits(1.5f)});
    uint32_t back = call(ScaleTo_create, {fbits(float(interval * 9.0)), fbits(1.0f)});
    call(fn::Node_runAction, {icon, call(Sequence_create, {up, back, 0})});

    // lwf->AddMovieEventHandler("_root", {{"enterFrame", $_0}})
    uint32_t movieLwf = ld32(lwf + LWFNode_lwf);
    uint32_t name = base + OFF_MOVIE_NAME;
    strInit(name, S_root.addr, S_root.len);
    uint32_t handler = base + OFF_HANDLER;
    st8(handler, uint8_t(S_enterFrame.len << 1));
    call(Memcpy, {handler + 1, S_enterFrame.addr, S_enterFrame.len});
    st8(handler + 1 + S_enterFrame.len, 0);
    uint32_t function = handler + 0x10;
    st32(function + 16, 0);
    uint32_t callable = call(fn::operator_new, {0x14});
    if (callable) {
        st32(callable, VT_TypeMatchMovie);
        st32(callable + 4, lwf);
        st32(callable + 8, image);
        st32(callable + 12, rw::guest_addr(self));
        st32(callable + 16, uint32_t(type));
    }
    st32(function + 16, callable);
    uint32_t map = base + OFF_HANDLER_MAP, endNode = map + 4;
    st32(endNode, 0);
    st32(map + 8, 0);
    st32(map, endNode);
    call(HandlerMap_insert, {map, endNode, handler});
    call(LWF_AddMovieEventHandler, {movieLwf, name, map});
    call(HandlerMap_destroy, {map, ld32(endNode)});
    functionDestroy(function);
    strFree(handler);
    strFree(name);
}
}  // namespace

// ------------------------------------------------------------------------------------------ updateLayout
void DeckMedalView::updateLayout(DeckMedalView *self, uint32_t playEffect) {
    Frame top;
    const uint32_t base = top.allocExact(FRAME_SIZE);     // the original frame (see the file comment)
    const uint32_t s = rw::guest_addr(self);
    stopPlate(self->bonusIcon);
    stopPlate(self->statusIcon);
    stopPlate(self->mixIcon);

    uint32_t image = find(s, S_Medal);
    if (!image) return;
    uint32_t iconNew = find(s, S_Icon_New);
    vcall(iconNew, node::setLocalZOrder, {iconNew, 1});
    uint32_t iconLock = find(s, S_Icon_Lock);
    vcall(iconLock, node::setLocalZOrder, {iconLock, 1});
    setVisible(iconLock, 0);
    uint32_t openInfo = find(s, S_Area_OpenInfo);
    setVisible(openInfo, 0);

    // medal id 1 stands for "no medal"
    auto hasMedal = [&] { return self->medal && safe32(medalRow(rw::host_ptr<UserMedal>(self->medal)), 0) != 1; };
    if (hasMedal()) {
        drawMedalImage(self, image, iconNew, iconLock);
    } else {
        setVisible(image, 0);
        setVisible(iconNew, 0);
        if (self->darklightMag && self->magnification >= 10001) setVisible(self->darklightMag, 1);
    }
    if (!self->isSmall) drawSlotHeader(self, s, openInfo, hasMedal());
    drawSkillIcon(self, s);

    InfoNodes n = findInfoNodes(s);
    uint32_t largePanel = find(s, S_Panel_L);
    setVisible(largePanel, 0);
    n.panelXL = find(s, S_Panel_XL);
    setVisible(n.panelXL, 0);
    self->largePanel = largePanel;
    self->typeMatched = 0;

    int32_t type = 1;
    if (!hasMedal()) {
        setVisible(n.dummy, 0);
    } else {
        const UserMedal *m = rw::host_ptr<UserMedal>(self->medal);
        int32_t attribute = safe32(medalRow(m), master::Medal::ATTRIBUTE);
        int32_t direction = safe32(medalRow(m), master::Medal::DARKLIGHT);
        int32_t attackBonus = 10000;
        if (!self->isSmall && attribute == self->slotAttribute && attribute != 4) {
            attackBonus = drawSlotBonus(self, n, attribute, direction, type);
        } else {
            self->bonusIcon = 0;
            self->bonusPanel = 0;
        }
        self->coinPanel = 0;
        drawInfoRow(self, s, n, attackBonus);
        drawKindRow(self, n);
    }
    showPlates(self);

    if (playEffect == 1) {
        if (self->typeMatched) {
            playTypeMatch(self, base, image, type);
        } else {
            Frame f;
            uint32_t layer = f.alloc(8);
            call(AudioManager_getLayer, {layer, call(AudioManager_getInstance, {}), AUDIO_LAYER_SE});
            call(AudioLayer_playSE, {ld32(layer), SE_TYPE_UNMATCH, 0, 0});
            sharedRelease(layer);
            removeTypeMatchEffect(s);
        }
    } else if (self->typeMatched) {
        showTypeMatchEffect(self, type);
    } else {
        removeTypeMatchEffect(s);
    }
}
RW_FUNCTION(0x5276D8, "DeckMedalView::updateLayout", DeckMedalView::updateLayout);
