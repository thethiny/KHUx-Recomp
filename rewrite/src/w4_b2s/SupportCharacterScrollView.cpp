// SupportCharacterScrollView: constructor, destructor, create, init, createRow, changeRow, registItemTouchEvent.
#include "SupportCharacterScrollView.hpp"

#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
using Addr = uint32_t;

// ---- guest functions
constexpr Addr Layout_ctor = 0x828671;                  // cocos2d::ui::Layout::Layout()
constexpr Addr aeabi_memset = 0x40E7A0;                 // __aeabi_memset(dst, n, c)
constexpr Addr memcpy_import = 0x40E164;                // memcpy / __aeabi_memcpy(dst, src, n)
constexpr Addr strlen_import = 0x40E08C;
constexpr Addr Ref_retain = 0x8FD0B5;
constexpr Addr Ref_release = 0x8FD0BD;
constexpr Addr Ref_autorelease = 0x8FD0D5;
constexpr Addr AvatarIconCache_dtor = 0x5CA8FD;
constexpr Addr VectorBase_u64_dtor = 0x468499;          // std::__vector_base<unsigned long long>::~__vector_base
constexpr Addr IScrollProtocol_dtor = 0x4F57E1;
constexpr Addr IScrollProtocol_setEnabledScrollBar = 0x4F6FE9;
constexpr Addr IScrollProtocol_setScrollType = 0x4F70B5;
constexpr Addr BaseScrollView_u64_init = 0x4FC969;      // BaseScrollView<unsigned long long>::init
constexpr Addr mulsf3 = 0xCD6E14;                       // __mulsf3
constexpr Addr Widget_create = 0x825875;
constexpr Addr Widget_clone = 0x8272B9;
constexpr Addr Size_ctor = 0x8DF027;                    // cocos2d::Size::Size(out, w, h)
constexpr Addr GUIReader_getInstance = 0x7BF1E5;
constexpr Addr GUIReader_widgetFromJsonFile = 0x7BF9FD;
constexpr Addr ArmatureDataManager_getInstance = 0x7AC9B5;
constexpr Addr ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;
constexpr Addr Armature_create = 0x79F3C7;              // cocostudio::Armature::create(const std::string&)
constexpr Addr VectorThrowOutOfRange = 0x47CCE1;        // std::__vector_base_common::__throw_out_of_range
constexpr Addr Director_getInstance = 0x8EE519;
constexpr Addr ListenerOneByOne_create = 0x8FACA5;
constexpr Addr ListenerOneByOne_setSwallowTouches = 0x8FAC99;
constexpr Addr FunctionBool_swap = 0x483E4D;            // std::function<bool(Touch*, Event*)>::swap
constexpr Addr FunctionVoid_swap = 0x4F3699;            // std::function<void(Touch*, Event*)>::swap
constexpr Addr CopyTouchCaptures = 0x504821;            // sub_504820(listener function slot, 3 captured words)
constexpr Addr EventDispatcher_addListenerSceneGraph = 0x8F3649;
constexpr Addr APIManager_getInstance = 0x4BA8C9;
constexpr Addr HashTable_find_u64 = 0x490275;           // unordered_map<unsigned long long, int>::find
constexpr Addr SafeBuffer_read = 0x45A269;              // SafeBuffer::read(out, src, n, offset)
constexpr Addr changePartnerPanel = 0x46BE75;           // SceneUtil::changePartnerPanel(widget, row, int)
constexpr Addr updateMixCircle = 0x465C85;              // SceneUtil::updateMixCircle(...)
constexpr Addr MedalDataManager_getInstance = 0x76AD69;
constexpr Addr MedalDataManager_getMedalData = 0x76ADB5;        // (sret, mgr, u64 id, int, UserMedal::Medal by value)
constexpr Addr MedalDataManager_getMedalSmallImagePath = 0x76B17D;
constexpr Addr SkillDataManager_getInstance = 0x76BA0D;
constexpr Addr SkillDataManager_getData = 0x76BA59;             // (sret, mgr, u64 id, UserSkill::Skill by value, bool)
constexpr Addr SkillDataManager_getImagePath = 0x76BE8D;
constexpr Addr SkillVector_dtor = 0x46BE37;             // std::__vector_base<UserSkill::Skill>::~__vector_base
constexpr Addr SkillVector_pushBackSlow = 0x487311;     // std::vector<UserSkill::Skill>::__push_back_slow_path
constexpr Addr ImageButton_create = 0x531239;           // ImageButton::create(std::string, bool)
constexpr Addr ImageButton_addTouchEventListener = 0x5315B1;
constexpr Addr ImageButton_setTargetView = 0x5318C1;    // (button, node, float)

// GOT entries (read at run time like the original)
constexpr Addr GOT_ownVtables = 0xFD65B4;               // vtable block of SupportCharacterScrollView (+8, +0x360, +0x374)
constexpr Addr GOT_baseVtables = 0xFD65A4;              // vtable block of its base (IScrollProtocol side)
constexpr Addr GOT_ANCHOR_MIDDLE = 0xFD6054;            // &cocos2d::Vec2::ANCHOR_MIDDLE

// lambda __func vtables (std::function callables created here)
constexpr Addr VT_touchBegan = 0xF55EC4;                // registItemTouchEvent::$_1 (this, index)
constexpr Addr VT_touchLambda2 = 0xF55E98;              // registItemTouchEvent::$_2 (this, index, this, index, this)
constexpr Addr VT_medalTouch = 0xF55EF0;                // changeRow lambda: this + a copy of SupportUser (0x12C bytes)

// ---- string literals (rodata)
constexpr Addr STR_StartDeckEdit2_Support = 0xDFD1C6;   // "cocostudio/publish/StartDeckEdit2_Support_ver120.json"
constexpr Addr STR_mainPanel = 0xDFD1FC;
constexpr Addr STR_CursorAnimExport = 0xDFD206;         // "cocostudio/publish/Cursor_Anim_Partner.ExportJson"
constexpr Addr STR_Area_MixCircle = 0xDF8B8A;
constexpr Addr STR_Area_Guilt_Icon = 0xDF8B99;
constexpr Addr STR_Area_Guilt_Plate = 0xDF8BA9;
constexpr Addr STR_Cursor_Anim_Partner = 0xDFD238;
constexpr Addr STR_Animation1 = 0xDFD0AF;
constexpr Addr STR_Area_Point = 0xDF9FB4;
constexpr Addr STR_SelectedAnimation = 0xDFD24C;
constexpr Addr STR_Select = 0xDF901C;
constexpr Addr STR_Medal = 0xDFD05F;
constexpr Addr STR_imageButton = 0xDF8F6D;
constexpr Addr STR_Empty = 0xDF8A1A;
constexpr Addr STR_Icon_Skil = 0xDF8F81;

// ---- Node / ProtectedNode virtual offsets used here (byte offsets into the vtable)
constexpr uint32_t V_setContentSize = 0xA8;
constexpr uint32_t V_setAnchorPoint = 0x9C;
constexpr uint32_t V_setVisible = 0xB0;
constexpr uint32_t V_addChildZName = 0xFC;
constexpr uint32_t V_Armature_getAnimation = 0x298;
constexpr uint32_t V_Animation_play = 0x68;
constexpr uint32_t V_this_init = 0x248;
constexpr uint32_t V_this_fillRow = 0x340;

// the temporary std::string of the original: built from a literal, destroyed (heap buffer freed) after use
Addr searchByName(Addr root, Addr literal, uint32_t len) { return findNode(root, literal, len); }

// std::function destroy as the compiler inlines it
void destroyFunction(Addr fn) { functionDestroy(fn); }
}  // namespace

// ---------------------------------------------------------------------------------------------------------------
SupportCharacterScrollView *SupportCharacterScrollView::construct(SupportCharacterScrollView *self) {
    const Addr me = rw::guest_addr(self);
    rw::call(Layout_ctor, {me});
    self->f4FC = rw::bitsf(0x3DCCCCCD);
    self->f500 = rw::bitsf(0x3E99999A);
    self->f504 = 0;
    rw::save<uint32_t>(me + 0x508, 0);
    self->f50C = 0;
    self->f510 = rw::bitsf(0x43480000);
    rw::call(aeabi_memset, {me + 0x514, 0x24, 0});
    self->f538 = 1;
    self->f53C = 1;
    rw::save<uint32_t>(me + 0x590, 0);
    self->f594 = 0;
    self->f5C4 = 0;
    self->f5D8 = 0;
    self->f5F0 = 0;
    self->f608 = 0;
    self->f620 = 0;
    rw::call(aeabi_memset, {me + 0x540, 0x12, 0});
    rw::call(aeabi_memset, {me + 0x554, 0x12, 0});
    rw::call(aeabi_memset, {me + 0x568, 0x25, 0});
    rw::call(aeabi_memset, {me + 0x598, 0x2A, 0});
    rw::save<uint32_t>(me + 0x628 - 0x0, 0);   // userIds.begin (+0x628 = 0xC5 << 3)
    self->userIds.end = 0;
    rw::save<uint32_t>(me + 0x630, 0);         // userIds.cap
    const Addr vt = ld32(GOT_ownVtables);
    self->vtable = vt + 8;
    self->vtable2 = vt + 0x360;
    self->vtable3 = vt + 0x374;
    rw::save<uint32_t>(me + 0x648, 0);         // onRowFunctionA: empty
    rw::save<uint32_t>(me + 0x660, 0);         // onRowFunctionB: empty
    self->f66C = 0;
    self->f670 = 0;
    self->f674 = 0;
    self->f678 = 0;
    self->f67C = 0;
    self->f680 = 0;
    self->f684 = 0;
    self->flags7A7 = uint8_t(self->flags7A7 & 0xFD);
    self->avatarIconCache = rw::Ptr<uint32_t>{0};   // +0x7BC
    self->f7B8 = 0;
    self->selfUserIdHi = 0;
    self->selfUserIdLo = 0;
    return self;
}

// ---------------------------------------------------------------------------------------------------------------
SupportCharacterScrollView *SupportCharacterScrollView::destruct(SupportCharacterScrollView *self) {
    const Addr me = rw::guest_addr(self);
    const Addr vt = ld32(GOT_ownVtables);
    self->vtable = vt + 8;
    self->vtable2 = vt + 0x360;
    self->vtable3 = vt + 0x374;
    if (self->itemTemplate) rw::call(Ref_release, {self->itemTemplate.addr});
    const Addr cache = self->avatarIconCache.addr;
    if (cache) {
        const Addr r = rw::call(AvatarIconCache_dtor, {cache});
        rw::call(guest::OPERATOR_DELETE, {r});         // the destructor returns this in r0
    }
    self->avatarIconCache = rw::Ptr<uint32_t>{0};
    destroyFunction(me + 0x650);
    destroyFunction(me + 0x638);
    const Addr base = ld32(GOT_baseVtables);
    self->vtable = base + 8;
    self->vtable2 = base + 0x360;
    self->vtable3 = base + 0x374;
    rw::call(VectorBase_u64_dtor, {me + 0x628});
    rw::call(IScrollProtocol_dtor, {me});
    return self;
}

// ---------------------------------------------------------------------------------------------------------------
SupportCharacterScrollView *SupportCharacterScrollView::create() {
    SupportCharacterScrollView *obj = rw::host_ptr<SupportCharacterScrollView>(guest::newObject(0x7C0));
    construct(obj);
    const Addr me = rw::guest_addr(obj);
    if (vcall(me, V_this_init, {me}) == 1) {
        rw::call(Ref_autorelease, {me});
        return obj;
    }
    vcall(me, 4, {me});         // virtual slot 1: the deleting destructor
    return nullptr;
}

// ---------------------------------------------------------------------------------------------------------------
uint32_t SupportCharacterScrollView::init(SupportCharacterScrollView *self) {
    const Addr me = rw::guest_addr(self);
    self->rowWidth = rw::bitsf(0x43A50000);      // 330
    self->rowHeight = rw::bitsf(0x42DC0000);     // 110
    rw::call(IScrollProtocol_setEnabledScrollBar, {me, 1});
    rw::call(IScrollProtocol_setScrollType, {me, 0});
    self->halfRowWidth = rw::bitsf(rw::call(mulsf3, {rw::fbits(self->rowWidth), 0x3F000000}));
    self->f574 = rw::bitsf(0xC1200000);          // -10

    const Addr widget = rw::call(Widget_create, {});
    self->itemTemplate = rw::Ptr<uint32_t>{widget};
    const Addr setContentSize = ld32(ld32(widget) + V_setContentSize);
    {
        Frame f;
        const Addr size = f.alloc(8);
        rw::call(Size_ctor, {size, rw::fbits(self->rowWidth), rw::fbits(self->rowHeight)});
        rw::call(setContentSize, {widget, size});
    }
    {
        const Addr tpl = self->itemTemplate.addr;
        const Addr setAnchor = ld32(ld32(tpl) + V_setAnchorPoint);
        rw::call(setAnchor, {tpl, ld32(GOT_ANCHOR_MIDDLE)});
    }
    rw::call(Ref_retain, {self->itemTemplate.addr});

    const Addr reader = rw::call(GUIReader_getInstance, {});
    const Addr root = rw::call(GUIReader_widgetFromJsonFile, {reader, STR_StartDeckEdit2_Support});
    {
        const Addr tpl = self->itemTemplate.addr;
        const Addr addChild = ld32(ld32(tpl) + V_addChildZName);
        Frame f;
        const Addr name = newStr(f, STR_mainPanel, 9);
        rw::call(addChild, {tpl, root, 0, name});
        strFree(name);
    }
    {
        const Addr mgr = rw::call(ArmatureDataManager_getInstance, {});
        Frame f;
        const Addr file = newStr(f, STR_CursorAnimExport, 0x31);
        rw::call(ArmatureDataManager_addArmatureFileInfo, {mgr, file});
        strFree(file);
    }
    const Addr mixCircle = searchByName(root, STR_Area_MixCircle, 0xE);
    vcall(mixCircle, V_setVisible, {mixCircle, 0});
    const Addr guiltIcon = searchByName(root, STR_Area_Guilt_Icon, 0xF);
    if (guiltIcon) vcall(guiltIcon, V_setVisible, {guiltIcon, 0});
    const Addr guiltPlate = searchByName(root, STR_Area_Guilt_Plate, 0x10);
    if (guiltPlate) vcall(guiltPlate, V_setVisible, {guiltPlate, 0});
    return rw::call(BaseScrollView_u64_init, {me});
}

// ---------------------------------------------------------------------------------------------------------------
uint32_t SupportCharacterScrollView::createRow(SupportCharacterScrollView *self, uint32_t index, uint32_t arg) {
    const Addr me = rw::guest_addr(self);
    const uint32_t count = uint32_t(int32_t(self->userIds.end - self->userIds.begin) >> 3);
    if (!(count > index)) {
        rw::call(VectorThrowOutOfRange, {me + 0x628});      // std::vector::at: throws, never returns
        return 0;
    }
    const Addr row = rw::call(Widget_clone, {self->itemTemplate.addr});
    Addr armature;
    {
        Frame f;
        const Addr name = newStr(f, STR_Cursor_Anim_Partner, 0x13);
        armature = rw::call(Armature_create, {name});
        strFree(name);
    }
    const Addr animation = vcall(armature, V_Armature_getAnimation, {armature});
    {
        Frame f;
        const Addr name = newStr(f, STR_Animation1, 0xA);
        // ArmatureAnimation::play(name, durationTo = -1, loop = -1)
        vcall(animation, V_Animation_play, {animation, name, 0xFFFFFFFFu, 0xFFFFFFFFu});
        strFree(name);
    }
    const Addr point = searchByName(row, STR_Area_Point, 0xA);
    {
        Frame f;
        const Addr name = newStr(f, STR_SelectedAnimation, 0x11);
        vcall(point, V_addChildZName, {point, armature, 0, name});
        strFree(name);
    }
    vcall(me, V_this_fillRow, {me, row, index, arg});
    return row;
}

// ---------------------------------------------------------------------------------------------------------------
// Refill a row widget with the support user at `index`. The original keeps a 0x530 byte frame with a 0x128 byte
// SupportUser copy in it (read from the support list SafeBuffer) and a std::function lambda that captures a copy
// of that struct; the frame is reproduced exactly (the lambda also copies one stale stack word).
uint32_t SupportCharacterScrollView::changeRow(SupportCharacterScrollView *self, uint32_t widget, uint32_t index,
                                               uint32_t /*unused*/) {
    const Addr me = rw::guest_addr(self);
    guest::Frame F(0x530);                  // push {r4-r7, lr} + sub sp, #0x51C
    auto fa = [&](uint32_t off) { return F.addr(off); };
    auto fw = [&](uint32_t off) -> uint32_t & { return *F.at<uint32_t>(off); };
    const Addr S = fa(0x2C0);               // SupportUser copy
    const Addr idLoc = fa(0x170);           // the user id (64 bit) for the map lookup

    const uint32_t count = uint32_t(int32_t(self->userIds.end - self->userIds.begin) >> 3);
    if (!(count > index)) {
        rw::call(VectorThrowOutOfRange, {me + 0x628});
        return 0;
    }
    const Addr userId = self->userIds.begin + index * 8;       // &userIds[index]
    const Addr areaPoint = searchByName(widget, STR_Area_Point, 0xA);
    if (areaPoint == 0) return 1;

    // ---- find the user's row in the support list and copy it (SafeBuffer) into S
    const Addr select = searchByName(widget, STR_Select, 6);
    vcall(select, V_setVisible, {select, 0});
    const Addr api = rw::call(APIManager_getInstance, {});
    fw(0x170) = ld32(userId);
    fw(0x174) = ld32(userId + 4);
    const Addr supportList = ld32(api + 0x134);
    const Addr node = rw::call(HashTable_find_u64, {supportList + 0x24, idLoc});
    uint32_t row = 0xFFFFFFFFu;
    if (node) row = ld32(node + 0x10);
    rw::call(changePartnerPanel, {widget, row, self->f7B8});
    {
        const Addr sb = ld32(api + 0x134);
        const uint32_t offset = row * 0x128;
        rw::call(SafeBuffer_read, {sb, S, ld32(sb + 0xC) + offset, 0x128, offset});
    }

    // ---- medal icon of the user's partner medal
    const Addr medalMgr = rw::call(MedalDataManager_getInstance, {});
    {
        // getMedalData(sret = shared_ptr at +0x150, mgr, id 0, 0, UserMedal::Medal by value = S + 0x48 (0x48 bytes))
        auto w = [&](uint32_t k) { return ld32(S + 0x48 + 4 * k); };
        rw::call(MedalDataManager_getMedalData,
                 {fa(0x150), medalMgr, 0, 0, 0, 0, w(0), w(1), w(2), w(3), w(4), w(5), w(6), w(7), w(8), w(9), w(10),
                  w(11), w(12), w(13), w(14), w(15), w(16), w(17)});
    }
    const Addr medalNode = searchByName(widget, STR_Medal, 5);
    const Addr medalMgr2 = rw::call(MedalDataManager_getInstance, {});
    fw(0x130) = fw(0x150);                      // the shared_ptr<UserMedalData> argument, by value
    fw(0x134) = fw(0x154);
    if (fw(0x134)) rw::call(guest::SHARED_ADD_REF, {fw(0x134)});
    {
        const Addr path = rw::call(MedalDataManager_getMedalSmallImagePath, {medalMgr2, fa(0x130)});
        const uint32_t len = rw::call(strlen_import, {path});
        Frame f;
        const Addr s = newStr(f, path, len);
        rw::call(fn::ImageView_loadTexture, {medalNode, s, 0});
        strFree(s);
    }
    if (fw(0x134)) rw::call(guest::SHARED_RELEASE, {fw(0x134)});

    // ---- the transparent touch button over the medal (created once)
    Addr button = searchByName(medalNode, STR_imageButton, 0xB);
    if (button == 0) {
        {
            Frame f;
            const Addr empty = newStr(f, STR_Empty, 0);
            button = rw::call(ImageButton_create, {empty, 0});
            strFree(empty);
        }
        rw::call(ImageButton_setTargetView, {button, medalNode, 0x3F800000});
        const Addr addChild = ld32(ld32(medalNode) + V_addChildZName);
        Frame f;
        const Addr name = newStr(f, STR_imageButton, 0xB);
        rw::call(addChild, {medalNode, button, 0, name});
        strFree(name);
    }
    st32(button + 0x2B8, 0x41A00000);                          // 20.0f
    // lambda capturing this + (stale word, SupportUser): std::function at +0x2A8 (callable slot at +0x2B8)
    rw::call(memcpy_import, {fa(0x180), S, 0x128});
    rw::call(memcpy_import, {fa(0x3E8), fa(0x17C), 0x12C});
    fw(0x2B8) = 0;
    const Addr lambda = rw::call(guest::OPERATOR_NEW, {0x138});
    if (lambda) {
        st32(lambda, VT_medalTouch);
        st32(lambda + 8, me);
        rw::call(memcpy_import, {lambda + 0xC, fa(0x3E8), 0x12C});
    }
    fw(0x2B8) = lambda;
    rw::call(ImageButton_addTouchEventListener, {button, fa(0x2A8)});
    destroyFunction(fa(0x2A8));

    // the "this is me" marker
    {
        const bool mine = (ld32(userId + 4) ^ self->selfUserIdHi) == 0 && (ld32(userId) ^ self->selfUserIdLo) == 0;
        vcall(areaPoint, V_setVisible, {areaPoint, mine ? 1u : 0u});
    }

    // ---- skill icon: the first of the medal's skills that the user has
    const Addr icon = searchByName(widget, STR_Icon_Skil, 9);
    vcall(icon, V_setVisible, {icon, 0});
    const Addr medalData = fw(0x150);
    if (medalData != 0 && int32_t(ld32(medalData + 0x48)) >= 1) {
        for (int32_t k = 0;;) {
            const Addr m = fw(0x150);
            const uint32_t idLo = ld32(m + 0x38 + uint32_t(k) * 8);
            const uint32_t idHi = ld32(m + 0x3C + uint32_t(k) * 8);
            if ((idLo | idHi) != 0) {
                // copy the user's skills (S + 0x98, count at S + 0x90, 0x28 bytes each) into a std::vector at +0xF4
                fw(0xF4) = 0;
                fw(0xF8) = 0;
                fw(0xFC) = 0;
                const uint32_t n = fw(0x350);
                Addr src = fa(0x358);
                for (uint32_t i = 0; i < n; i++, src += 0x28) {
                    const uint32_t end = fw(0xF8), cap = fw(0xFC);
                    if (end == cap) {
                        rw::call(SkillVector_pushBackSlow, {fa(0xF4), src});
                    } else {
                        if (end != 0) {
                            for (uint32_t b = 0; b < 0x28; b += 4) st32(end + b, ld32(src + b));
                        }
                        fw(0xF8) = end + 0x28;
                    }
                }
                // look the id up in the copy
                Addr found = 0;
                const Addr vbegin = fw(0xF4), vend = fw(0xF8);
                if (vbegin != vend) {
                    Addr e = vbegin;
                    do {
                        if ((ld32(e + 4) ^ idHi) == 0 && (ld32(e) ^ idLo) == 0) { found = e; break; }
                        e += 0x28;
                    } while (e != vend);
                }
                Addr obj = 0, ctrl = 0;       // shared_ptr<UserSkillData> (r5, r7)
                bool haveObj = false;
                if (found) {
                    const Addr skillMgr = rw::call(SkillDataManager_getInstance, {});
                    // getData(sret = +0xEC, mgr, id 0, Skill by value (the element, id words from the lookup), false)
                    rw::call(SkillDataManager_getData,
                             {fa(0xEC), skillMgr, 0, 0, idLo, idHi, ld32(found + 8), ld32(found + 0xC),
                              ld32(found + 0x10), ld32(found + 0x14), ld32(found + 0x18), ld32(found + 0x1C),
                              ld32(found + 0x20), ld32(found + 0x24), 0});
                    obj = fw(0xEC);
                    ctrl = fw(0xF0);
                    fw(0xEC) = 0;
                    fw(0xF0) = 0;
                    haveObj = obj != 0;
                }
                if (!haveObj) {
                    // nothing found (or no data): an empty Skill gives the "unknown" skill data
                    const Addr skillMgr = rw::call(SkillDataManager_getInstance, {});
                    rw::call(SkillDataManager_getData, {fa(0xE4), skillMgr, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0});
                    const Addr obj2 = fw(0xE4);
                    const Addr ctrl2 = fw(0xE8);
                    fw(0xE4) = 0;
                    fw(0xE8) = 0;
                    if (ctrl != 0) {
                        rw::call(guest::SHARED_RELEASE, {ctrl});
                        if (fw(0xE8) != 0) rw::call(guest::SHARED_RELEASE, {fw(0xE8)});
                    }
                    obj = obj2;
                    ctrl = ctrl2;
                }
                // show the skill icon
                const Addr skillMgr2 = rw::call(SkillDataManager_getInstance, {});
                fw(0xD0) = obj;
                fw(0xD4) = ctrl;
                if (ctrl) rw::call(guest::SHARED_ADD_REF, {ctrl});
                {
                    const Addr path = rw::call(SkillDataManager_getImagePath, {skillMgr2, fa(0xD0)});
                    const uint32_t len = rw::call(strlen_import, {path});
                    Frame f;
                    const Addr s = newStr(f, path, len);
                    rw::call(fn::ImageView_loadTexture, {icon, s, 0});
                    strFree(s);
                }
                if (fw(0xD4)) rw::call(guest::SHARED_RELEASE, {fw(0xD4)});
                rw::call(SkillVector_dtor, {fa(0xF4)});
                if (ctrl) rw::call(guest::SHARED_RELEASE, {ctrl});
            }
            k++;
            if (k >= int32_t(ld32(fw(0x150) + 0x48))) break;
        }
        vcall(icon, V_setVisible, {icon, 1});
    }

    // ---- the mix circle and the guild widgets
    const Addr mixCircle = searchByName(widget, STR_Area_MixCircle, 0xE);
    vcall(mixCircle, V_setVisible, {mixCircle, 0});
    const Addr guiltIcon = searchByName(widget, STR_Area_Guilt_Icon, 0xF);
    if (guiltIcon) vcall(guiltIcon, V_setVisible, {guiltIcon, 0});
    const Addr guiltPlate = searchByName(widget, STR_Area_Guilt_Plate, 0x10);
    if (guiltPlate) vcall(guiltPlate, V_setVisible, {guiltPlate, 0});
    const uint32_t flag45 = rw::load<uint8_t>(S + 0x45);
    vcall(mixCircle, V_setVisible, {mixCircle, 1});
    fw(0xA4) = fw(0x150);                          // shared_ptr<UserMedalData> copy for the call
    fw(0xA8) = fw(0x154);
    if (fw(0xA8)) rw::call(guest::SHARED_ADD_REF, {fw(0xA8)});
    rw::call(updateMixCircle, {mixCircle, guiltIcon, guiltPlate, fa(0xA4), flag45 != 0 ? 1u : 0u, 0, 1, 0, 0, 1, 0});
    if (fw(0xA8)) rw::call(guest::SHARED_RELEASE, {fw(0xA8)});
    if (fw(0x154)) rw::call(guest::SHARED_RELEASE, {fw(0x154)});
    return 0;
}

// ---------------------------------------------------------------------------------------------------------------
void SupportCharacterScrollView::registItemTouchEvent(SupportCharacterScrollView *self, uint32_t /*widget*/,
                                                      int32_t index) {
    const Addr me = rw::guest_addr(self);
    Frame f;
    const Addr dispatcher = ld32(rw::call(Director_getInstance, {}) + 0x84);
    const Addr listener = rw::call(ListenerOneByOne_create, {});
    rw::call(ListenerOneByOne_setSwallowTouches, {listener, 0});

    // onTouchBegan: std::function<bool(Touch*, Event*)> holding lambda $_1 (this, index), stored inline
    const Addr fn1 = f.alloc(20);
    st32(fn1 + 16, fn1);
    st32(fn1, VT_touchBegan);
    st32(fn1 + 8, uint32_t(index));
    st32(fn1 + 4, me);
    rw::call(FunctionBool_swap, {fn1, listener + 0x50});
    destroyFunction(fn1);

    // the three words the later listener slots copy: (this, index, this)
    const Addr captures = f.alloc(12);
    st32(captures, me);
    st32(captures + 4, uint32_t(index));
    st32(captures + 8, me);
    st32(fn1 + 16, 0);
    // onTouchMoved: std::function<void(Touch*, Event*)> holding lambda $_2 (five words), on the heap
    Addr heap = rw::call(guest::OPERATOR_NEW, {0x18});
    if (heap) {
        st32(heap, VT_touchLambda2);
        st32(heap + 4, me);
        st32(heap + 8, uint32_t(index));
        st32(heap + 0xC, me);
        st32(heap + 0x10, uint32_t(index));
        st32(heap + 0x14, me);
    }
    st32(fn1 + 16, heap);
    rw::call(FunctionVoid_swap, {fn1, listener + 0x68});
    destroyFunction(fn1);
    rw::call(CopyTouchCaptures, {listener + 0x80, captures});
    rw::call(CopyTouchCaptures, {listener + 0x98, captures});
    rw::call(EventDispatcher_addListenerSceneGraph, {dispatcher, listener, me});
}

RW_FUNCTION(0x5036DC, "SupportCharacterScrollView::SupportCharacterScrollView()", SupportCharacterScrollView::construct);
RW_FUNCTION(0x5034BC, "SupportCharacterScrollView::~SupportCharacterScrollView()", SupportCharacterScrollView::destruct);
RW_FUNCTION(0x503688, "SupportCharacterScrollView::create()", SupportCharacterScrollView::create);
RW_FUNCTION(0x503874, "SupportCharacterScrollView::init()", SupportCharacterScrollView::init);
RW_FUNCTION(0x503B64, "SupportCharacterScrollView::createRow(int, int)", SupportCharacterScrollView::createRow);
RW_FUNCTION(0x503D5C, "SupportCharacterScrollView::changeRow(Widget*, int, int)", SupportCharacterScrollView::changeRow);
RW_FUNCTION(0x5046E0, "SupportCharacterScrollView::registItemTouchEvent(Widget*, int)", SupportCharacterScrollView::registItemTouchEvent);
