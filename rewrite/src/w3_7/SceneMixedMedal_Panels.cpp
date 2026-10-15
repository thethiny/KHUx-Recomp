// SceneMixedMedal: building the panels - the medal list (right page), the base medal panel, the material slots, the
// skill overwrite panel and the campaign banner.
#include "SceneMixedMedal.hpp"

#include "../core/SafeBuffer.hpp"
#include "../scene_social/SocialCommon.hpp"

using namespace social;
using Self = SceneMixedMedal;
namespace F = smm::fn;

namespace {

// std::function vtables (__func<lambda> + 8)
constexpr Addr VT_select_onTapItem = 0xF655FC;     // MiniMedalScrollView::onTapItem (void(u64, int))
constexpr Addr VT_select_getDropTarget = 0xF655D0; // MiniMedalScrollView::getDropTarget (int(u64, Vec2, float*))
constexpr Addr VT_select_onDropItem = 0xF655A4;    // MiniMedalScrollView::onDropItem (void(int, u64))
constexpr Addr VT_select_sortChanged = 0xF65578;   // IMedalSortFilter::registSortButton callback
constexpr Addr VT_base_evoButton = 0xF65C58;       // base medal: tap on the medal picture
constexpr Addr VT_material_touchBegan = 0xF65DE4;  // material area: onTouchBegan (captures this, 0x6C)
constexpr Addr VT_material_touchMoved = 0xF65DB8;  // material area: onTouchMoved
constexpr Addr VT_skill_mixButton = 0xF65C2C;      // "skill_mix_button"
constexpr Addr setTouchEndHandler = 0x699999;      // sub_699998(std::function<void(Touch*,Event*)>*, {this, 0x6C})

// MiniMedalScrollView std::function members
constexpr uint32_t LIST_onTapItem = 0x638, LIST_onDropItem = 0x650, LIST_getDropTarget = 0x668;
// EventListenerTouchOneByOne std::function members
constexpr uint32_t TOUCH_onBegan = 0x50, TOUCH_onMoved = 0x68, TOUCH_onEnded = 0x80, TOUCH_onCancelled = 0x98;

// APIManager members
constexpr uint32_t API_userData = 0x94;            // shared_ptr<SafeBuffer> user data (+0x34: medal slots)
constexpr uint32_t API_userMedals = 0xD4;          // shared_ptr<SafeBuffer> user medal list (72-byte records)
constexpr uint32_t USERDATA_maxMedal = 0x34;
constexpr uint32_t MEDAL_RECORD = 0x48;
constexpr uint32_t SAFEBUFFER_count = 0x20;        // the list's record count

// master::Medal (SafeBuffer) fields
constexpr uint32_t MEDAL_no = 0x0C;
constexpr uint32_t MEDAL_name = 0x10;              // char[0x41]
constexpr uint32_t MEDAL_rare = 0x470;
constexpr uint32_t MEDAL_guiltGroup = 0x478;
constexpr uint32_t MEDAL_maxLevel = 0x47C;
constexpr uint32_t MEDAL_burstType = 0x4A8;        // 1: has a burst
constexpr uint32_t MEDAL_burstId = 0x4AC;
constexpr uint32_t MEDAL_burstUpIds = 0x4B0;       // int[2]: medal ids that raise the burst

constexpr uint32_t SDM_skillCountOff = 0x48;       // UserMedalData::skillCount
constexpr uint32_t SDM_skillIdsOff = 0x38;         // UserMedalData::userSkillIds (u64[2])

// SceneUtil::ATLASNUM_KERNING_PIXEL (float)
constexpr Addr ATLASNUM_KERNING_PIXEL = 0xDEC3DC;

// strings
constexpr Addr S_Win_under = 0xE03EF1;
constexpr Addr S_Close_Button = 0xDF8D2F;
constexpr Addr S_Scroll_Area = 0xDF9B59;
constexpr Addr S_Txt_Possession = 0xE04023;
constexpr Addr S_Txt_Cost_Label = 0xE04032;
constexpr Addr S_Txt_Slash_Label = 0xE04041;
constexpr Addr S_Txt_CostMax_Label = 0xE04051;
constexpr Addr S_fmt_d = 0xDF86DE;                 // "%d"
constexpr Addr S_fmt_s = 0xDF8821;                 // "%s"
constexpr Addr S_Number = 0xDF8A28;
constexpr Addr S_Number_Label = 0xDF8A2F;
constexpr Addr S_NameLabel = 0xDF8A3C;
constexpr Addr S_LV = 0xDF8A6E;
constexpr Addr S_LV_Now = 0xDF8A71;
constexpr Addr S_Slash = 0xDF8A78;
constexpr Addr S_LV_Max = 0xDF8A7E;
constexpr Addr S_Txt_NextLV = 0xE04F5B;
constexpr Addr S_Dummy_Medal = 0xDF8C16;
constexpr Addr S_Star_Area = 0xDF8C22;
constexpr Addr S_imageButton = 0xDF8F6D;
constexpr Addr S_Medal_Evo_But = 0xE04F66;
constexpr Addr S_Icon_Skill = 0xDF8C40;
constexpr Addr S_Icon_Skil = 0xDF8F81;
constexpr Addr S_Base_Material = 0xE04F1E;
constexpr Addr S_Area = 0xDF9378;
constexpr Addr S_Area_Anim = 0xE04F2C;
constexpr Addr S_CursorAnim = 0xE04F36;            // "Cursor_Anim_MdalMix_ver103"
constexpr Addr S_Animation1 = 0xDFD0AF;
constexpr Addr S_AtlasLabel_EXP = 0xDF8CCD;
constexpr Addr S_Announce = 0xDFF8BD;
constexpr Addr S_Txt_Overwrite = 0xE04F74;
constexpr Addr S_Base_Skill = 0xE04F82;
constexpr Addr S_Txt_SuccessRate = 0xE04F8D;
constexpr Addr S_Button_Overwrite = 0xE04F9D;
constexpr Addr S_skill_mix_button = 0xE04FAE;
constexpr Addr S_Area_Anim_d = 0xE04FBF;           // "Area_Anim%d"
constexpr Addr S_Base_d = 0xE04FCB;                // "Base%d"

// the original frames of the functions that pass a zeroed UserMedal::Medal by value (its word +0x2C is never
// initialised: the stale stack word at that address is part of the argument)
constexpr uint32_t PUSHED = 20;
constexpr uint32_t FRAME_openMedalSelectPanel = 0x9E4, HOLE_openMedalSelectPanel = 0x948 + 0x2C;
constexpr uint32_t FRAME_updateBaseMedal = 0x1244, HOLE_updateBaseMedal = 0x11F0 + 0x2C;

void setShadow(Addr text) { call(F::SceneUtil_setShadow, {text}); }
void setNormalColor(Addr text) { call(F::SceneUtil_setNormalColor, {text}); }
void release(Addr ctrl) {
    if (ctrl) call(F::shared_release, {ctrl});
}
int32_t sb32(Addr sb, uint32_t index) { return rw::host_ptr<SafeBuffer>(sb)->get32(index); }
// the master::Medal row (a SafeBuffer) of a UserMedalData
Addr medalRow(Addr data) { return ld32(data + 0x58); }

// Read the stale word of the original frame's zeroed medal record (before any call reuses that stack area).
uint32_t staleRecordWord(uint32_t frame, uint32_t hole) { return ld32(rw::cpu->r[13] - PUSHED - frame + hole); }

// MedalDataManager::getMedalData(out, manager, id, 0, UserMedal::Medal{} by value)
void getMedalData(Addr out, Addr mgr, uint32_t lo, uint32_t hi, uint32_t stale) {
    call(F::MedalDataManager_getMedalData,
         {out, mgr, lo, hi, 0, 0,                                   // medalId, padding to the 8-aligned record
          0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, stale, 0, 0, 0, 0, 0, 0});   // the 72-byte record (+0x2C stale)
}

// SkillDataManager::getData(out, manager, id, UserSkill::Skill{} by value, false)
void getSkillData(Addr out, Addr mgr, uint32_t lo, uint32_t hi) {
    call(F::SkillDataManager_getData, {out, mgr, lo, hi, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
}

// MiniMedalScrollView::setDisableMedalID(list, id, disable, used)
void setDisableMedalID(Addr list, uint32_t lo, uint32_t hi, uint32_t disable, uint32_t used) {
    call(F::MiniMedalScrollView_setDisableMedalID, {list, 0, lo, hi, disable, used});
}

// text->setString(StringBuffer(fmt, value))
void setTextFormat(Addr text, Addr fmt, uint32_t value) {
    Frame f;
    Addr buf = stringBuffer(f, {fmt, value});
    setTextC(text, buf);
}

// libc++ vector<T*>::push_back(v) inlined: in place when there is room, else __push_back_slow_path(const T&)
void pushPointer(Addr vec, uint32_t value, Addr slowPath) {
    Addr end = ld32(vec + 4);
    if (end != ld32(vec + 8)) {
        Addr base = 0;
        if (end) {
            st32(end, value);
            base = ld32(vec + 4);
        }
        st32(vec + 4, base + 4);
    } else {
        Frame f;
        Addr tmp = f.alloc(4);
        st32(tmp, value);
        call(slowPath, {vec, tmp});
    }
}
// vector<unsigned long long>::push_back(int widened) inlined (slow path: __push_back_slow_path(T&&))
void pushU64(Addr vec, int32_t value) {
    uint32_t lo = uint32_t(value), hi = uint32_t(value >> 31);
    Addr end = ld32(vec + 4);
    if (end < ld32(vec + 8)) {
        Addr base = 0;
        if (end) {
            st32(end, lo);
            st32(end + 4, hi);
            base = ld32(vec + 4);
        }
        st32(vec + 4, base + 8);
    } else {
        Frame f;
        Addr tmp = f.alloc(8);
        st32(tmp, lo);
        st32(tmp + 4, hi);
        call(F::vector_u64_push_slow_move, {vec, tmp});
    }
}
// vector<shared_ptr<T>>::push_back(const shared_ptr&) inlined
void pushShared(Addr vec, Addr sp, Addr slowPath) {
    Addr end = ld32(vec + 4);
    if (end == ld32(vec + 8)) {
        call(slowPath, {vec, sp});
        return;
    }
    if (end) {
        st32(end, ld32(sp));
        Addr ctrl = ld32(sp + 4);
        st32(end + 4, ctrl);
        if (ctrl) call(F::shared_add, {ctrl});
    }
    st32(vec + 4, ld32(vec + 4) + 8);
}
// vector<shared_ptr<T>>::clear() inlined: destroy from the back
void clearShared(Addr vec) {
    Addr begin = ld32(vec), end = ld32(vec + 4);
    while (end != begin) {
        Addr last = end - 8;
        st32(vec + 4, last);
        Addr ctrl = ld32(end - 4);
        end = last;
        if (ctrl) {
            call(F::shared_release, {ctrl});
            end = ld32(vec + 4);
        }
    }
}
// target->swap(std::function holding an inline lambda)
void swapInto(Addr target, Addr vtable, std::initializer_list<uint32_t> captures, Addr swap) {
    withLambda(vtable, captures, [&](Addr f) { call(swap, {f, target}); });
}
// ArmatureAnimation::play(name, -1, -1) on a new "Cursor_Anim_MdalMix_ver103" armature added to `parent`
void addCursorAnimation(Addr parent) {
    Addr armature = withStr(S_CursorAnim, 26, [&](Addr s) { return call(F::Armature_create, {s}); });
    vcall(parent, node::addChild, {parent, armature});
    Addr animation = vcall(armature, 0x298, {armature});      // Armature::getAnimation
    withStr(S_Animation1, 10, [&](Addr s) { vcall(animation, 0x68, {animation, s, 0xFFFFFFFFu, 0xFFFFFFFFu}); });
}

}  // namespace

// ------------------------------------------------------------------------------------------------ medal list
void Self::initMedalSelectPanel(SceneMixedMedal *self) {
    Addr me = rw::guest_addr(self);
    Addr winUnder = search(self->centerUIRight, S_Win_under, 9);
    Addr close = search(self->centerUIRight, S_Close_Button, 12);
    vcall(close, node::setVisible, {close, 0});
    Addr scrollArea = search(winUnder, S_Scroll_Area, 11);
    {
        // tutorial 117 (the medal list), its close function kept by AppState
        Addr app = call(F::AppState_getInstance, {});
        Frame f;
        Addr none = newEmptyFunction(f);
        Addr popup = f.alloc(20);
        call(F::SceneUtil_openTutorialPopup, {popup, 117, none, scrollArea, 0, 0, 0});
        Addr old = f.alloc(20);
        call(F::AppState_setTutorialCloseFunc, {old, app, popup, 117});
        functionDestroy(old);
        functionDestroy(popup);
        functionDestroy(none);
    }

    self->medalList = call(F::MiniMedalScrollView_create, {});
    {
        Addr list = self->medalList;
        Addr setContentSize = ld32(ld32(list) + node::setContentSize);
        Addr size = vcall(scrollArea, node::getContentSize, {scrollArea});
        call(setContentSize, {list, size});
    }
    call(F::MiniMedalScrollView_setIsShowMixInfo, {self->medalList, 1});
    call(F::MiniMedalScrollView_changeShowType, {self->medalList, 6});
    swapInto(self->medalList + LIST_onTapItem, VT_select_onTapItem, {me}, F::function_vyi_swap);
    swapInto(self->medalList + LIST_getDropTarget, VT_select_getDropTarget, {me}, F::function_iyVecPf_swap);
    swapInto(self->medalList + LIST_onDropItem, VT_select_onDropItem, {me}, F::function_viy_swap);
    vcall(scrollArea, node::addChild, {scrollArea, self->medalList});
    titleLabel(winUnder, S_Txt_Possession, 14, 101200021);

    Frame f;
    Addr origin = f.alloc(8), size = f.alloc(8);
    call(F::Director_getVisibleOrigin, {origin, call(F::Director_getInstance, {})});
    call(F::Director_getVisibleSize, {size, call(F::Director_getInstance, {})});
    {
        // the drop target rectangle: the visible area above the list
        Addr list = self->medalList;
        float ox = ldf(origin), oy = ldf(origin + 4);
        float h = ldf(vcall(list, node::getContentSize, {list}) + 4);
        Addr rect = f.alloc(16);
        call(F::Rect_ctorXYWH, {rect, fbits(ox), fbits(oy + h), ld32(size), ld32(size + 4)});
        call(F::MiniMedalScrollView_setTargetRect, {list, rect});
        call(F::Vec2_dtor, {rect});
    }
    Addr world = f.alloc(8);
    {
        // the list's rectangle in world space
        Addr list = self->medalList;
        Addr parent = vcall(list, node::getParent, {list});
        Addr pos = vcall(self->medalList, node::getPosition, {self->medalList});
        call(F::Node_convertToWorldSpace, {world, parent, pos});
        float x = ldf(origin) + ldf(world);
        float y = ldf(origin + 4) + ldf(world + 4);
        float w = ldf(vcall(self->medalList, node::getContentSize, {self->medalList}));
        float h = ldf(vcall(self->medalList, node::getContentSize, {self->medalList}) + 4);
        Addr rect = f.alloc(16);
        call(F::Rect_ctorXYWH, {rect, fbits(x), fbits(y), fbits(w), fbits(h)});
        call(F::Rect_assign, {me + 0x370, rect});
        call(F::Vec2_dtor, {rect});
    }
    withLambda(VT_select_sortChanged, {me}, [&](Addr cb) {
        call(F::IMedalSortFilter_registSortButton, {me + 0x288, winUnder, 3, cb});
    });
    {
        Addr state = f.alloc(8);
        vcall(me, smm::vslot::getSortFilter, {state, me});
        uint32_t showType = call(F::IMedalSortFilter_getShowType, {me + 0x288, state, 0});
        release(ld32(state + 4));
        call(F::MiniMedalScrollView_changeShowType, {self->medalList, showType});
    }
    call(F::Vec2_dtor, {world});
    call(F::Vec2_dtor, {origin});
}
RW_FUNCTION(0x696814, "SceneMixedMedal::initMedalSelectPanel", SceneMixedMedal::initMedalSelectPanel);

// Fill the medal list with every owned medal (sorted / filtered), mark the ones that raise the base medal's
// burst, and show "owned / max" medals.
void Self::openMedalSelectPanel(SceneMixedMedal *self) {
    Addr me = rw::guest_addr(self);
    const uint32_t stale = staleRecordWord(FRAME_openMedalSelectPanel, HOLE_openMedalSelectPanel);
    call(F::MiniMedalScrollView_clearDisableMedals, {self->medalList});
    Addr winUnder = search(self->centerUIRight, S_Win_under, 9);

    Frame f;
    Addr burstUpIds = f.alloc(12);                            // std::vector<unsigned long long>
    st32(burstUpIds, 0);
    st32(burstUpIds + 4, 0);
    st32(burstUpIds + 8, 0);
    Addr mgr = call(F::SceneManager_getInstance, {});
    uint32_t baseHi = ld32(mgr + 0x2A4), baseLo = ld32(mgr + 0x2A0);
    clearShared(me + 0x28C);

    Addr api = call(F::APIManager_getInstance, {});
    Addr listCtrl = ld32(api + API_userMedals + 4);
    Addr list = ld32(api + API_userMedals);
    if (listCtrl) call(F::shared_add, {listCtrl});
    Addr medal = f.alloc(8);
    if (ld32(list + SAFEBUFFER_count) != 0) {
        uint32_t offset = 0;
        for (uint32_t i = 0;;) {
            uint32_t lo = uint32_t(sb32(list, offset)), hi = uint32_t(sb32(list, offset + 4));
            Addr mdm = call(F::MedalDataManager_getInstance, {});
            getMedalData(medal, mdm, lo, hi, stale);
            pushShared(me + 0x28C, medal, F::vector_userMedal_push_slow);
            if (lo == baseLo && hi == baseHi) {
                // the base medal: its own medal id and the two burst-up medal ids
                Addr row = medalRow(ld32(medal));
                pushU64(burstUpIds, sb32(row, 0));
                pushU64(burstUpIds, sb32(row, MEDAL_burstUpIds));
                pushU64(burstUpIds, sb32(row, MEDAL_burstUpIds + 4));
            }
            release(ld32(medal + 4));
            offset += MEDAL_RECORD;
            i++;
            if (!(i < ld32(list + SAFEBUFFER_count))) break;
        }
    }
    call(F::IMedalSortFilter_runSortFilter, {me + 0x288, 3});

    // the list shows the filtered medals' ids
    Addr ids = f.alloc(12);
    st32(ids, 0);
    st32(ids + 4, 0);
    st32(ids + 8, 0);
    {
        Addr begin = self->sortFilter.filtered.begin;
        if (self->sortFilter.filtered.end != begin) {
            uint32_t idx = 0;
            for (;;) {
                Addr data = ld32(begin + 8 * idx);
                Addr end = ld32(ids + 4);
                if (end == ld32(ids + 8)) {
                    call(F::vector_u64_push_slow_copy, {ids, data});
                } else {
                    Addr base = 0;
                    if (end) {
                        st32(end, ld32(data));
                        st32(end + 4, ld32(data + 4));
                        base = ld32(ids + 4);
                    }
                    st32(ids + 4, base + 8);
                }
                begin = self->sortFilter.filtered.begin;
                idx++;
                if (!(idx < uint32_t(int32_t(self->sortFilter.filtered.end - begin) >> 3))) break;
            }
        }
    }
    vcall(self->medalList, node::setVisible, {self->medalList, 1});
    self->medalListIndex = 0;
    call(F::IScrollProtocol_setIndex, {self->medalList, 0});
    call(F::MiniMedalScrollView_setItemIDs, {self->medalList, ids});
    call(F::MiniMedalScrollView_setBurstUpMedalIDs, {self->medalList, burstUpIds});

    // "owned / max" medals
    Addr api2 = call(F::APIManager_getInstance, {});
    Addr userCtrl = ld32(api2 + API_userData + 4);
    Addr user = ld32(api2 + API_userData);
    if (userCtrl) call(F::shared_add, {userCtrl});
    Addr cost = search(winUnder, S_Txt_Cost_Label, 14);
    setShadow(cost);
    int32_t owned = int32_t(self->sortFilter.medals.end - self->sortFilter.medals.begin) >> 3;
    setTextFormat(cost, S_fmt_d, uint32_t(owned));
    if (sb32(user, USERDATA_maxMedal) >= owned) call(F::SceneUtil_setNormalColor, {cost});
    else call(F::SceneUtil_setWarningColor, {cost});
    setShadow(search(winUnder, S_Txt_Slash_Label, 15));
    Addr max = search(winUnder, S_Txt_CostMax_Label, 17);
    setShadow(max);
    setTextFormat(max, S_fmt_d, uint32_t(sb32(user, USERDATA_maxMedal)));
    release(userCtrl);
    call(F::vector_u64_dtor, {ids});
    release(listCtrl);
    call(F::vector_u64_dtor, {burstUpIds});
}
RW_FUNCTION(0x696D98, "SceneMixedMedal::openMedalSelectPanel", SceneMixedMedal::openMedalSelectPanel);

// ------------------------------------------------------------------------------------------------ base medal
void Self::initBaseMedalPanel(SceneMixedMedal *self) {
    Addr number = search(self->centerUILeft1, S_Number, 6);
    setShadow(number);
    setTextId(number, 103600003);
    setShadow(search(self->centerUILeft1, S_Number_Label, 12));
    setShadow(search(self->centerUILeft1, S_NameLabel, 9));
    Addr lv = search(self->centerUILeft1, S_LV, 2);
    setShadow(lv);
    setTextId(lv, 100200002);
    Addr lvNow = search(lv, S_LV_Now, 6);
    setShadow(lvNow);
    setNormalColor(lvNow);
    setShadow(search(lv, S_Slash, 5));
    setShadow(search(lv, S_LV_Max, 6));
    Addr next = search(self->centerUILeft1, S_Txt_NextLV, 10);
    setShadow(next);
    setTextId(next, 100200012);
}
RW_FUNCTION(0x6973CC, "SceneMixedMedal::initBaseMedalPanel", SceneMixedMedal::initBaseMedalPanel);

void Self::updateBaseMedal(SceneMixedMedal *self, uint64_t userMedalId) {
    Addr me = rw::guest_addr(self);
    const uint32_t stale = staleRecordWord(FRAME_updateBaseMedal, HOLE_updateBaseMedal);
    // the previous base medal becomes pickable again
    if (Addr old = self->baseMedal.obj) setDisableMedalID(self->medalList, ld32(old), ld32(old + 4), 0, 0);

    Frame f;
    {
        Addr mdm = call(F::MedalDataManager_getInstance, {});
        Addr got = f.alloc(8);
        getMedalData(got, mdm, uint32_t(userMedalId), uint32_t(userMedalId >> 32), stale);
        // baseMedal = std::move(got)
        Addr obj = ld32(got), ctrl = ld32(got + 4);
        st32(got, 0);
        st32(got + 4, 0);
        self->baseMedal.obj = obj;
        Addr oldCtrl = self->baseMedal.ctrl;
        self->baseMedal.ctrl = ctrl;
        if (oldCtrl) {
            call(F::shared_release, {oldCtrl});
            release(ld32(got + 4));
        }
    }
    {
        Addr base = self->baseMedal.obj;
        setDisableMedalID(self->medalList, ld32(base), ld32(base + 4), 1, 0);
    }
    if (sb32(medalRow(self->baseMedal.obj), MEDAL_burstType) == 1) {
        int32_t burstId = sb32(medalRow(self->baseMedal.obj), MEDAL_burstId);
        Addr burst = f.alloc(8);
        call(F::master_Burst_create, {burst, uint32_t(burstId)});
        // baseBurst = std::move(burst)
        Addr obj = ld32(burst), ctrl = ld32(burst + 4);
        st32(burst, 0);
        st32(burst + 4, 0);
        self->baseBurst.obj = obj;
        Addr oldCtrl = self->baseBurst.ctrl;
        self->baseBurst.ctrl = ctrl;
        if (oldCtrl) {
            call(F::shared_release, {oldCtrl});
            release(ld32(burst + 4));
        }
        int32_t guiltGroup = sb32(medalRow(self->baseMedal.obj), MEDAL_guiltGroup);
        getGuiltFactorValueRange(guiltGroup, rw::host_ptr<uint32_t>(me + 0x32C));
    }
    // the materials stay greyed out in the list
    {
        Addr list = self->medalList;
        Addr ids = self->materialIds.begin;
        if (self->materialIds.end != ids) {
            uint32_t k = 0;
            for (uint32_t off = 0;; off += 8) {
                setDisableMedalID(list, ld32(ids + off), ld32(ids + off + 4), 1, 0);
                ids = self->materialIds.begin;
                k++;
                list = self->medalList;
                if (!(k < uint32_t(int32_t(self->materialIds.end - ids) >> 3))) break;
            }
        }
        call(F::IScrollProtocol_updateLayout, {list, 0});
    }

    // the medal picture
    Addr dummy = search(self->centerUILeft2, S_Dummy_Medal, 11);
    {
        Addr mdm = call(F::MedalDataManager_getInstance, {});
        Addr copy = f.alloc(8);
        st32(copy, self->baseMedal.obj);
        Addr ctrl = self->baseMedal.ctrl;
        st32(copy + 4, ctrl);
        if (ctrl) call(F::shared_add, {ctrl});
        Addr path = call(F::MedalDataManager_getMedalLargeImagePath, {mdm, copy});
        withStr(path, host_strlen(path), [&](Addr s) { call(F::ImageView_loadTexture, {dummy, s, 0}); });
        release(ld32(copy + 4));
    }
    Addr stars = search(self->centerUILeft2, S_Star_Area, 9);
    int32_t rare = sb32(medalRow(self->baseMedal.obj), MEDAL_rare);
    call(F::SceneUtil_updateMedalRarePanel, {stars, uint32_t(rare), 1, fbits(0.6f), 0, 0, 0});
    if (!search(dummy, S_imageButton, 11)) {
        // a tap on the picture opens the medal details
        Addr button = withStr(S_Medal_Evo_But, 13, [&](Addr s) { return call(F::ImageButton_create, {s, 0}); });
        call(F::ImageButton_setTargetView, {button, dummy, fbits(1.0f)});
        Addr addChildNamed = ld32(ld32(dummy) + node::addChildZName);
        withStr(S_imageButton, 11, [&](Addr s) { call(addChildNamed, {dummy, button, 0, s}); });
        withLambda(VT_base_evoButton, {me}, [&](Addr cb) { call(F::ImageButton_addTouchEventListener, {button, cb}); });
    }
    Addr icon = search(self->centerUILeft2, S_Icon_Skill, 10);
    if (!icon) icon = search(self->centerUILeft2, S_Icon_Skil, 9);
    vcall(icon, node::setVisible, {icon, 0});
    Addr base = self->baseMedal.obj;
    if (base && int32_t(ld32(base + SDM_skillCountOff)) >= 1) {
        // the icon of the medal's skill
        uint32_t off = SDM_skillIdsOff;
        for (int32_t k = 0;; k++, off += 8) {
            uint32_t lo = ld32(base + off), hi = ld32(base + off + 4);
            if (lo | hi) {
                Addr skm = call(F::SkillDataManager_getInstance, {});
                Addr skill = f.alloc(8);
                getSkillData(skill, skm, lo, hi);
                Addr obj = ld32(skill), ctrl = ld32(skill + 4);
                st32(skill, 0);
                st32(skill + 4, 0);
                Addr skm2 = call(F::SkillDataManager_getInstance, {});
                Addr copy = f.alloc(8);
                st32(copy, obj);
                st32(copy + 4, ctrl);
                if (ctrl) call(F::shared_add, {ctrl});
                Addr path = call(F::SkillDataManager_getImagePath, {skm2, copy});
                withStr(path, host_strlen(path), [&](Addr s) { call(F::ImageView_loadTexture, {icon, s, 0}); });
                release(ld32(copy + 4));
                release(ctrl);
            }
            base = self->baseMedal.obj;
            if (!(k + 1 < int32_t(ld32(base + SDM_skillCountOff)))) break;
        }
        vcall(icon, node::setVisible, {icon, 1});
    }

    Addr row = 0;
    Addr numberLabel = search(self->centerUILeft1, S_Number_Label, 12);
    row = medalRow(self->baseMedal.obj);
    setTextFormat(numberLabel, S_fmt_d, uint32_t(sb32(row, MEDAL_no)));
    Addr nameLabel = search(self->centerUILeft1, S_NameLabel, 9);
    {
        Frame nf;
        Addr name = safeBytes(nf, medalRow(self->baseMedal.obj), MEDAL_name, 0x41);
        Addr buf = stringBuffer(nf, {S_fmt_s, name});
        setTextC(nameLabel, buf);
    }
    Addr lv = search(self->centerUILeft1, S_LV, 2);
    Addr lvNow = search(lv, S_LV_Now, 6);
    setNormalColor(lvNow);
    setTextFormat(lvNow, S_fmt_d, ld32(self->baseMedal.obj + 8));
    Addr lvMax = search(lv, S_LV_Max, 6);
    setTextFormat(lvMax, S_fmt_d, uint32_t(sb32(medalRow(self->baseMedal.obj), MEDAL_maxLevel)));
}
RW_FUNCTION(0x697834, "SceneMixedMedal::updateBaseMedal", SceneMixedMedal::updateBaseMedal);

// ------------------------------------------------------------------------------------------------ materials
void Self::initMaterialPanel(SceneMixedMedal *self) {
    Addr me = rw::guest_addr(self);
    Addr baseMaterial = search(self->centerUILeft1, S_Base_Material, 13);
    self->materialArea = search(baseMaterial, S_Area, 4);
    vcall(self->materialArea, 0x29C, {self->materialArea, 0});   // ui::Widget::setTouchEnabled(false)
    self->materialAreaAnim = search(baseMaterial, S_Area_Anim, 9);
    addCursorAnimation(self->materialAreaAnim);

    // touches on the material slots (dragging a material back out, tapping one)
    Addr listener = call(F::EventListenerTouchOneByOne_create, {});
    call(F::EventListenerTouchOneByOne_setSwallowTouches, {listener, 0});
    swapInto(listener + TOUCH_onBegan, VT_material_touchBegan, {me, 0x6C}, F::function_touchBool_swap);
    swapInto(listener + TOUCH_onMoved, VT_material_touchMoved, {me}, F::function_touchVoid_swap);
    {
        Frame f;
        Addr captures = f.alloc(8);
        st32(captures, me);
        st32(captures + 4, 0x6C);
        call(setTouchEndHandler, {listener + TOUCH_onEnded, captures});
        call(setTouchEndHandler, {listener + TOUCH_onCancelled, captures});
    }
    Addr dispatcher = ld32(call(F::Director_getInstance, {}) + 0x84);
    call(F::EventDispatcher_addEventListenerWithSceneGraphPriority, {dispatcher, listener, self->materialArea});

    initSkillMixPanel(self);
    self->expLabel = search(self->centerUILeft1, S_AtlasLabel_EXP, 14);
    call(F::SceneUtil_setAdditionalKerningForAtlasLabel,
         {self->expLabel, uint32_t(f2i(ldf(ATLASNUM_KERNING_PIXEL)))});
}
RW_FUNCTION(0x6982B0, "SceneMixedMedal::initMaterialPanel", SceneMixedMedal::initMaterialPanel);

// The running fusion campaigns (master::EvCampaign kinds 11..17): experience / cost / skill rates, and the banner.
void Self::initCampaign(SceneMixedMedal *self) {
    Addr me = rw::guest_addr(self);
    clearShared(me + 0x3A0);
    Addr api = call(F::APIManager_getInstance, {});
    Addr ids = ld32(api + 0x2FC);
    if (int32_t(ld32(api + 0x300) - ids) >= 1) {
        Frame f;
        Addr campaign = f.alloc(8);
        int32_t count = int32_t(ld32(api + 0x300) - ids) >> 2;
        for (int32_t i = 0;;) {
            if (!(uint32_t(count) > uint32_t(i))) {
                call(F::vector_out_of_range, {api + 0x2FC});
                return;
            }
            call(F::master_EvCampaign_create, {campaign, ld32(ids + 4 * uint32_t(i))});
            Addr row = ld32(campaign);
            if (row) {
                int32_t kind = sb32(row, 4);
                bool keep = true;
                switch (kind) {
                case 11: self->expRate = sb32(row, 8); break;
                case 12: case 13: case 17: break;
                case 14: self->costRate = sb32(row, 8); break;
                case 16: self->skillRate = sb32(row, 8); break;
                default: keep = false; break;   // 15 and the others: not a fusion campaign
                }
                if (keep) pushShared(me + 0x3A0, campaign, F::vector_evCampaign_push_slow);
            }
            release(ld32(campaign + 4));
            ids = ld32(api + 0x2FC);
            count = int32_t(ld32(api + 0x300) - ids) >> 2;
            i++;
            if (i >= count) break;
        }
    }
    Addr announce = search(self->root, S_Announce, 8);
    Addr view = call(F::CampaignAnimationView_create, {announce, me + 0x3A0});
    vcall(announce, node::addChild, {announce, view});
}
RW_FUNCTION(0x6991B0, "SceneMixedMedal::initCampaign", SceneMixedMedal::initCampaign);

// ------------------------------------------------------------------------------------------------ skill overwrite
void Self::initSkillMixPanel(SceneMixedMedal *self) {
    Addr me = rw::guest_addr(self);
    self->skillMixTitle = titleLabel(self->centerUILeft1, S_Txt_Overwrite, 13, 101300020);
    self->baseSkill = search(self->centerUILeft1, S_Base_Skill, 10);
    self->successRateText = search(self->centerUILeft1, S_Txt_SuccessRate, 15);
    setShadow(self->successRateText);
    Addr buttonArea = search(self->centerUILeft1, S_Button_Overwrite, 16);
    self->skillMixButton = withStr(S_skill_mix_button, 16, [&](Addr s) { return call(F::ImageButton_create, {s, 0}); });
    call(F::ImageButton_setTargetImageView, {self->skillMixButton, buttonArea});
    call(F::ImageButton_setSelectEnabled, {self->skillMixButton, 1});
    vcall(buttonArea, node::addChild, {buttonArea, self->skillMixButton});
    self->skillMixButtonText = search(buttonArea, S_Txt_Overwrite, 13);
    setShadow(self->skillMixButtonText);
    {
        Addr button = self->skillMixButton;
        withLambda(VT_skill_mixButton, {me}, [&](Addr cb) { call(F::ImageButton_addTouchEventListener, {button, cb}); });
    }
    // slots 2..5: the cursor animation and the panel of each material
    for (uint32_t k = 2;; k++) {
        Addr animArea;
        {
            Frame f;
            Addr name = stringBuffer(f, {S_Area_Anim_d, k});
            animArea = withStr(name, host_strlen(name), [&](Addr s) {
                return call(F::SceneUtil_searchNodeByName, {self->centerUILeft1, s});
            });
        }
        pushPointer(me + 0x2D8, animArea, F::vector_widget_push_slow);
        addCursorAnimation(animArea);
        Addr panel;
        {
            Frame f;
            Addr name = stringBuffer(f, {S_Base_d, k});
            panel = withStr(name, host_strlen(name), [&](Addr s) {
                return call(F::SceneUtil_searchNodeByName, {self->centerUILeft1, s});
            });
        }
        pushPointer(me + 0x2E4, panel, F::vector_imageView_push_slow);
        if (!(int32_t(k) - 1 < 4)) break;
    }
    setEnabledSkillMix(self, 0);
}
RW_FUNCTION(0x699A14, "SceneMixedMedal::initSkillMixPanel", SceneMixedMedal::initSkillMixPanel);
