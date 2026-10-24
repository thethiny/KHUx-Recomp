// SphereUI: the Avatar Board grid (5 rows x 9 columns of spheres). Each cell has a kind (a masu id 1..23, or a path
// piece 1001..1006 / 2001..2006, or 1000 = the start), a container node, a SphereBlock and an open status
// (0 closed, 1 open, 2 can be opened). The masu records (23 x 7 words) come from master::SphereMasu: cost, reward
// type, flag, reward id, -, skill id, count. check_*_route / check_*_status walk the paths from the start cell and
// mark which spheres are reachable; setOpenCursor puts the cursor on the best sphere that can be opened.
#include "../scene/SceneCommon.hpp"

using namespace scene;

namespace {
namespace a {
constexpr uint32_t STRLEN = 0x40E08C;
constexpr uint32_t AEABI_MEMSET = 0x40E7A0;               // __aeabi_memset(dst, n, c)
constexpr uint32_t Ref_ctor = rwlib::cocos2d::Ref::ctor_base, Ref_dtor = rwlib::cocos2d::Ref::dtor_base, Ref_retain = rwlib::cocos2d::Ref::retain, Ref_release = rwlib::cocos2d::Ref::release;
constexpr uint32_t Ref_autorelease = rwlib::cocos2d::Ref::autorelease;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t SceneUtil_searchNodeByName = 0x45A9F5;
constexpr uint32_t SceneUtil_setButtonEnabled = 0x468625;
constexpr uint32_t StringBuffer_ctor = 0x49140D;          // (buf, fmt, ...)
constexpr uint32_t ArmatureDataManager_getInstance = rwlib::cocostudio::ArmatureDataManager::getInstance;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = rwlib::cocostudio::ArmatureDataManager::addArmatureFileInfo;
constexpr uint32_t Armature_create = rwlib::cocostudio::Armature::create;
constexpr uint32_t ConnectingSphere_create = 0x793A65;
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t SphereBlock_create = 0x7936E1;         // (kind, skill, type, cost)
constexpr uint32_t Node_stopAllActions = rwlib::cocos2d::Node::stopAllActions;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t audio_getInstance = 0x5BECFD, audio_load = 0x5BEEF5, audio_getLayer = 0x5BF33D;
constexpr uint32_t Layer_playSE = 0x5BF785;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t Sphere_create = 0x78CF21, SphereArray_create = 0x78D675, SphereMasu_create = 0x78DD09;
constexpr uint32_t Medal_create = 0x785871, Skill_create = 0x78BDE9, Material_create = 0x7852E5;
constexpr uint32_t Title_create = 0x78FA8D, Keyblade_create = 0x783F9D, AvatarParts_create = 0x778DE1;
constexpr uint32_t openPopUpIncentiveComp = 0x795CFD;
}  // namespace a

constexpr uint32_t VTABLE = 0xFAC4FC;
constexpr uint32_t SIZE = 0x5AC;
// fields
constexpr uint32_t F_SPHERE_DATA = 0x14;      // ConnectingSphere* (retained)
constexpr uint32_t F_SPHERE = 0x1C;           // shared_ptr<master::Sphere>
constexpr uint32_t F_ARRAY = 0x24;            // shared_ptr<master::SphereArray>
constexpr uint32_t F_MASU = 0x2C;             // shared_ptr<master::SphereMasu>
constexpr uint32_t F_KIND = 0x2B8;            // int[5][9]
constexpr uint32_t F_CURSOR = 0x36C;          // cocostudio::Armature* (retained)
constexpr uint32_t F_NODE = 0x370;            // Node*[5][9] (the sphere containers)
constexpr uint32_t F_BLOCK = 0x424;           // SphereBlock*[5][9]
constexpr uint32_t F_STATUS = 0x4D8;          // int[5][9]
constexpr uint32_t F_START_COL = 0x58C, F_START_ROW = 0x590;
constexpr uint32_t F_TXT_POINT = 0x598, F_TXT_TYPE = 0x59C, F_TXT_NAME = 0x5A0;
constexpr uint32_t F_RELEASE_BUTTON = 0x5A4;  // the incentive popup's release button (when open)
constexpr uint32_t F_MODE = 0x5A8;
// SphereBlock fields
constexpr uint32_t B_NEW = 0x270, B_STATUS = 0x274, B_SELECTED = 0x278, B_ARMATURE = 0x280;
constexpr uint32_t B_ICON_A = 0x288, B_ICON_B = 0x28C, B_ICON_C = 0x290;
// a masu record (k = 1..23): this + 0x18 + 0x1C * k
inline uint32_t masu(uint32_t self, uint32_t k) { return self + 0x18 + 0x1C * k; }
constexpr uint32_t M_COST = 0x00, M_TYPE = 0x04, M_ID = 0x0C, M_SKILL = 0x14, M_COUNT = 0x18;
constexpr uint32_t CURSOR_TAG = 0x4545;
constexpr uint32_t SE_OPEN = 509000180;

// guest strings
constexpr uint32_t S_ExportJsonFmt = 0xE01EED, S_SphereAnimation = 0xE09B6A, S_TxtRelease = 0xDF9CD9;
constexpr uint32_t S_PercentD = 0xDF86DE, S_TwoSpaces = 0xE09B98, S_SpCursor = 0xE09B9B;
// text tables: reward type -> TextData id, medal rarity -> TextData id
constexpr uint32_t TYPE_TEXTS = 0xEAC1A0, RARITY_TEXTS = 0xEAC200;
// the armature name tables (pointers)
constexpr uint32_t ARMATURES_A = 0xF6EC9C, ARMATURES_B = 0xF6ECFC, ARMATURES_C = 0xF6ED14;

inline uint32_t cell(uint32_t self, uint32_t base, int32_t row, int32_t col) {
    return self + base + uint32_t(36 * row) + uint32_t(4 * col);
}

void sbRead(uint32_t sb, uint32_t dst, uint32_t offset, uint32_t len) {
    rw::call(a::SafeBuffer_read, {sb, dst, ld32(sb + 0xC) + offset, len, offset});
}

void setTextC(uint32_t text, uint32_t chars) {
    Frame f;
    const uint32_t s = f.alloc(12);
    strInit(s, chars, call(a::STRLEN, {chars}));
    call(fn::Text_setString, {text, s});
    strFree(s);
}

void setTextData(uint32_t text, uint32_t id) {
    Frame f;
    const uint32_t td = f.alloc(8);
    call(fn::TextData_get, {td, 0, id});
    setTextC(text, ld32(ld32(td) + 4));
    sharedRelease(td);
}

// text = StringBuffer(TextData(id), count)
void setTextFormat(uint32_t text, uint32_t id, uint32_t count) {
    Frame f;
    const uint32_t td = f.alloc(8);
    call(fn::TextData_get, {td, 0, id});
    const uint32_t buf = f.alloc(0x404);
    call(a::StringBuffer_ctor, {buf, ld32(ld32(td) + 4), count});
    setTextC(text, buf);
    sharedRelease(td);
}

// text = the name field (len bytes at offset) of master::X(id)
void setTextMaster(uint32_t text, uint32_t create, uint32_t id, uint32_t offset, uint32_t len) {
    Frame f;
    const uint32_t m = f.alloc(8);
    call(create, {m, id});
    const uint32_t buf = f.alloc(0x84);
    sbRead(ld32(m), buf, offset, len);
    setTextC(text, buf);
    sharedRelease(m);
}

// out = std::move(tmp) for a shared_ptr member (the inlined move-assignment)
void moveShared(uint32_t member, uint32_t tmp) {
    const uint32_t obj = ld32(tmp), ctrl = ld32(tmp + 4);
    st32(tmp, 0);
    st32(tmp + 4, 0);
    st32(member, obj);
    const uint32_t old = ld32(member + 4);
    st32(member + 4, ctrl);
    if (old) {
        call(fn::shared_release, {old});
        if (ld32(tmp + 4)) call(fn::shared_release, {ld32(tmp + 4)});
    }
}

void playOpenSE() {
    Frame f;
    const uint32_t layer = f.alloc(8);
    call(a::audio_getLayer, {layer, call(a::audio_getInstance, {}), 0});
    call(a::Layer_playSE, {ld32(layer), SE_OPEN, 0, 0});
    sharedRelease(layer);
}

void addArmatureFile(uint32_t manager, uint32_t path) {
    Frame f;
    const uint32_t s = f.alloc(12);
    strInit(s, path, call(a::STRLEN, {path}));
    call(a::ArmatureDataManager_addArmatureFileInfo, {manager, s});
    strFree(s);
}

void addArmatureNamed(uint32_t manager, uint32_t name) {
    Frame f;
    const uint32_t buf = f.alloc(0x404);
    call(a::StringBuffer_ctor, {buf, S_ExportJsonFmt, name});
    addArmatureFile(manager, buf);
}

// the type / name texts of the reward on masu k (shared by update_status and setOpenCursor)
void showReward(uint32_t self, uint32_t k) {
    const uint32_t rec = masu(self, k);
    const uint32_t skill = ld32(rec + M_SKILL), count = ld32(rec + M_COUNT), type = ld32(rec + M_TYPE);
    const uint32_t id = ld32(rec + M_ID);
    if (type == 3) {
        Frame f;
        const uint32_t m = f.alloc(8), rarity = f.alloc(4);
        call(a::Medal_create, {m, id});
        sbRead(ld32(m), rarity, 0x454, 4);
        const uint32_t r = ld32(rarity);
        sharedRelease(m);
        setTextData(ld32(self + F_TXT_TYPE), ld32(RARITY_TEXTS + 4 * r));
    } else {
        setTextData(ld32(self + F_TXT_TYPE), ld32(TYPE_TEXTS + 4 * type));
    }
    const uint32_t name = ld32(self + F_TXT_NAME);
    switch (type) {
    case 0: setTextMaster(name, a::Skill_create, skill, 0xC, 0x41); break;
    case 3: setTextMaster(name, a::Medal_create, id, 0x10, 0x41); break;
    case 5: setTextMaster(name, a::Material_create, id, 0xC, 0x41); break;
    case 7: setTextMaster(name, a::Title_create, id, 8, 0x41); break;
    case 12: setTextFormat(name, 103400069, count); break;
    case 13: setTextMaster(name, a::Keyblade_create, id, 4, 0x41); break;
    case 18: case 19: case 20: case 23: setTextMaster(name, a::AvatarParts_create, id, 4, 0x81); break;
    case 21: setTextFormat(name, 103400070, count); break;
    case 22: setTextFormat(name, 103400071, count); break;
    default: break;
    }
}

// the incentive popup's release button: enabled unless an incentive can be opened
void updateReleaseButton(uint32_t self);
}  // namespace

struct SphereUI {
    static uint32_t create();
    static uint32_t construct(uint32_t self);
    static uint32_t destruct(uint32_t self);
    static uint32_t init(uint32_t self);
    static void loadBoard(uint32_t self, uint32_t id);
    static void createBoard(uint32_t self, uint32_t id);
    static void createSphere(uint32_t self, int32_t row, int32_t col, int32_t kind);
    static void updateStatus(uint32_t self, uint32_t dt);
    static void setOpenCursor(uint32_t self);
    static uint32_t setCursor(uint32_t self, int32_t row, int32_t col, uint32_t playSe);
    static uint32_t isOpenIncentive(uint32_t self, uint32_t playSe);
    static uint32_t checkUpRoute(uint32_t self, int32_t col, int32_t row, uint32_t flag);
    static uint32_t checkDownRoute(uint32_t self, int32_t col, int32_t row, uint32_t flag);
    static uint32_t checkRightRoute(uint32_t self, int32_t col, int32_t row, uint32_t flag);
    static uint32_t checkLeftRoute(uint32_t self, int32_t col, int32_t row, uint32_t flag);
    static uint32_t checkUpStatus(uint32_t self, int32_t col, int32_t row, uint32_t flag);
    static uint32_t checkDownStatus(uint32_t self, int32_t col, int32_t row, uint32_t flag);
    static uint32_t checkRightStatus(uint32_t self, int32_t col, int32_t row, uint32_t flag);
    static uint32_t checkLeftStatus(uint32_t self, int32_t col, int32_t row, uint32_t flag);
};

namespace {
void updateReleaseButton(uint32_t self) {
    const uint32_t button = self + F_RELEASE_BUTTON;
    if (!ld32(button)) return;
    const uint32_t open = SphereUI::isOpenIncentive(self, 0);
    Frame f;
    const uint32_t s = newStr(f, S_TxtRelease, 0xB);
    const uint32_t text = call(a::SceneUtil_searchNodeByName, {ld32(button), s});
    strFree(s);
    call(a::SceneUtil_setButtonEnabled, {ld32(button), text, open ? 0u : 1u, 0});
}
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
uint32_t SphereUI::create() {
    const uint32_t me = call(fn::operator_new, {SIZE});
    construct(me);
    if (vcall(me, 0x08, {me}) == 1) {
        call(a::Ref_autorelease, {me});
        return me;
    }
    vcall(me, 0x04, {me});
    return 0;
}

uint32_t SphereUI::construct(uint32_t self) {
    call(a::Ref_ctor, {self});
    st32(self, VTABLE);
    st32(self + F_CURSOR, 0);
    st8(self + 0x594, 0);
    st32(self + F_RELEASE_BUTTON, 0);
    call(a::AEABI_MEMSET, {self + 0x14, 0x20, 0});
    st32(self + F_MODE, 1);
    for (int32_t row = 0; row < 5; row++)
        for (int32_t col = 0; col < 9; col++) {
            st32(cell(self, F_KIND, row, col), 0);
            st32(cell(self, F_BLOCK, row, col), 0);
            st32(cell(self, F_STATUS, row, col), 0);
        }
    return self;
}

uint32_t SphereUI::destruct(uint32_t self) {
    st32(self, VTABLE);
    if (ld32(self + F_CURSOR)) {
        call(a::Ref_release, {ld32(self + F_CURSOR)});
        st32(self + F_CURSOR, 0);
    }
    if (ld32(self + F_SPHERE_DATA)) {
        call(a::Ref_release, {ld32(self + F_SPHERE_DATA)});
        st32(self + F_SPHERE_DATA, 0);
    }
    for (uint32_t ctrl : {0x30u, 0x28u, 0x20u})
        if (ld32(self + ctrl)) call(fn::shared_release, {ld32(self + ctrl)});
    call(a::Ref_dtor, {self});
    return self;
}

// loads the board's armatures, creates the open-state model (ConnectingSphere) and picks the board mode from the
// scene manager's state
uint32_t SphereUI::init(uint32_t self) {
    call(a::audio_load, {call(a::audio_getInstance, {}), 509100420});
    for (uint32_t i = 0; i < 23; i++)
        if ((0xFC30A9u >> i) & 1)
            addArmatureNamed(call(a::ArmatureDataManager_getInstance, {}), ld32(ARMATURES_A + 4 * i));
    uint32_t manager = call(a::ArmatureDataManager_getInstance, {});
    for (uint32_t k = 0; k < 6; k++) {
        addArmatureNamed(manager, ld32(ARMATURES_B + 4 * k));
        addArmatureNamed(call(a::ArmatureDataManager_getInstance, {}), ld32(ARMATURES_C + 4 * k));
        manager = call(a::ArmatureDataManager_getInstance, {});
    }
    addArmatureFile(manager, S_SphereAnimation);
    const uint32_t data = call(a::ConnectingSphere_create, {});
    st32(self + F_SPHERE_DATA, data);
    call(a::Ref_retain, {data});
    const uint32_t state = ld32(call(a::SceneManager_getInstance, {}) + 0x28C);
    uint32_t mode;
    if (state - 2 < 2) mode = 4;
    else if (state == 4) mode = 5;
    else if (state == 5) mode = 3;
    else mode = 1;
    st32(self + F_MODE, mode);
    return 1;
}

// the board's master data: the kind grid (rows from the SphereArray's five 9-int arrays, bottom up) and the masu
// records
void SphereUI::loadBoard(uint32_t self, uint32_t id) {
    Frame f;
    const uint32_t tmp = f.alloc(8), v = f.alloc(4);
    call(a::Sphere_create, {tmp, id});
    moveShared(self + F_SPHERE, tmp);
    sbRead(ld32(self + F_SPHERE), v, 0x90, 4);
    call(a::SphereArray_create, {tmp, ld32(v)});
    moveShared(self + F_ARRAY, tmp);
    sbRead(ld32(self + F_SPHERE), v, 0x94, 4);
    call(a::SphereMasu_create, {tmp, ld32(v)});
    moveShared(self + F_MASU, tmp);

    const uint32_t row = f.alloc(0x24);
    static const uint32_t rowOffsets[5] = {0x94, 0x70, 0x4C, 0x28, 4};
    for (uint32_t col = 0; col < 9; col++)
        for (uint32_t r = 0; r < 5; r++) {
            sbRead(ld32(self + F_ARRAY), row, rowOffsets[r], 0x24);
            st32(self + F_KIND + 0x24 * r + 4 * col, ld32(row + 4 * col));
        }
    const uint32_t list = f.alloc(0x5C);
    static const uint32_t fieldOffsets[7] = {8, 0x64, 0xC0, 0x11C, 0x178, 0x1D4, 0x230};
    for (uint32_t i = 0; i < 23; i++) {
        const uint32_t rec = masu(self, i + 1);
        for (uint32_t w = 0; w < 7; w++) {
            sbRead(ld32(self + F_MASU), list, fieldOffsets[w], 0x5C);
            st32(rec + 4 * w, ld32(list + 4 * i));
        }
        if (ld32(rec + M_TYPE) == 3 && ld32(rec + 8)) st32(rec + M_TYPE, 0);
    }
}

void SphereUI::createBoard(uint32_t self, uint32_t id) {
    loadBoard(self, id);
    for (int32_t row = 0; row < 5; row++)
        for (int32_t col = 0; col < 9; col++) {
            int32_t kind = int32_t(ld32(cell(self, F_KIND, row, col)));
            if (kind < 1) continue;
            if (kind <= 998) kind = int32_t(ld32(self + 0x1C * uint32_t(kind) + 0x1C));   // the masu's reward type
            createSphere(self, row, col, kind);
        }
    Frame f;
    const uint32_t buf = f.alloc(0x404);
    call(a::StringBuffer_ctor, {buf, S_PercentD, 0});
    setTextC(ld32(self + F_TXT_POINT), buf);
    call(a::StringBuffer_ctor, {buf, S_TwoSpaces});
    setTextC(ld32(self + F_TXT_TYPE), buf);
    call(a::StringBuffer_ctor, {buf, S_TwoSpaces});
    setTextC(ld32(self + F_TXT_NAME), buf);
    addArmatureNamed(call(a::ArmatureDataManager_getInstance, {}), S_SpCursor);
    {
        const uint32_t s = newStr(f, S_SpCursor, 10);
        st32(self + F_CURSOR, call(a::Armature_create, {s}));
        strFree(s);
    }
    call(a::Ref_retain, {ld32(self + F_CURSOR)});
    const uint32_t w = ld32(vcall(ld32(self + F_NODE), node::getContentSize, {ld32(self + F_NODE)}));
    const uint32_t h = ld32(vcall(ld32(self + F_NODE), node::getContentSize, {ld32(self + F_NODE)}) + 4);
    const uint32_t pos = f.alloc(8);
    call(a::Vec2_ctor, {pos, fbits(bitsf(w) * 0.5f), fbits(bitsf(h) * 0.5f)});
    const uint32_t cursor = ld32(self + F_CURSOR);
    vcall(cursor, node::setPosition, {cursor, pos});
    call(a::Vec2_dtor, {pos});
    const uint32_t anim = vcall(ld32(self + F_CURSOR), 0x298, {ld32(self + F_CURSOR)});   // getAnimation
    vcall(anim, 0x70, {anim, 0, 0xFFFFFFFF, 0xFFFFFFFF});                                   // playWithIndex(0, -1, -1)
    updateStatus(self, 0);
    setOpenCursor(self);
}

// one sphere: a SphereBlock in the cell's container (path pieces alternate two looks by checkerboard parity)
void SphereUI::createSphere(uint32_t self, int32_t row, int32_t col, int32_t kind) {
    const int32_t parity = (col + row) % 2;
    const int32_t look = kind <= 1000 ? parity : parity + 2;
    const uint32_t c = self + uint32_t(36 * row) + uint32_t(4 * col);
    const uint32_t container = ld32(c + F_NODE);
    const uint32_t w = ld32(vcall(container, node::getContentSize, {container}));
    const uint32_t h = ld32(vcall(ld32(c + F_NODE), node::getContentSize, {ld32(c + F_NODE)}) + 4);
    if (look == 2 || look == 3) {
        st32(c + F_BLOCK, call(a::SphereBlock_create, {uint32_t(kind - 1001), 0, uint32_t(look), 0}));
    } else if (look == 0) {
        if (kind == 1000) {
            st32(c + F_BLOCK, call(a::SphereBlock_create, {0, 0, 4, 0}));
            st32(self + F_START_COL, uint32_t(col));
            st32(self + F_START_ROW, uint32_t(row));
        } else {
            const uint32_t k = ld32(c + F_KIND);
            st32(c + F_BLOCK, call(a::SphereBlock_create, {uint32_t(kind), ld32(self + 0x1C * k + 0x30), 0,
                                                            ld32(self + 0x1C * k + 0x18)}));
            // already opened (listed in the open-state model): open, icons shown
            for (uint32_t i = 10; i < 33; i++) {
                if (ld32(ld32(self + F_SPHERE_DATA) + 4 * i) != k) continue;
                st32(c + F_STATUS, 1);
                const uint32_t b = ld32(c + F_BLOCK);
                vcall(ld32(b + B_ICON_A), node::setVisible, {ld32(b + B_ICON_A), 1});
                vcall(ld32(ld32(c + F_BLOCK) + B_ICON_B), node::setVisible, {ld32(ld32(c + F_BLOCK) + B_ICON_B), 0});
                vcall(ld32(ld32(c + F_BLOCK) + B_ICON_C), node::setVisible, {ld32(ld32(c + F_BLOCK) + B_ICON_C), 0});
            }
        }
    }
    const uint32_t block = ld32(c + F_BLOCK);
    call(a::Node_stopAllActions, {block});
    vcall(block, 0x214, {block, 255});                   // setOpacity
    vcall(block, node::setVisible, {block, 1});
    const uint32_t x = fbits(bitsf(w) * 0.5f), y = fbits(bitsf(h) * 0.5f);
    Frame f;
    const uint32_t pos = f.alloc(8);
    call(a::Vec2_ctor, {pos, x, y});
    const uint32_t b2 = ld32(c + F_BLOCK);
    vcall(b2, node::setPosition, {b2, pos});
    call(a::Vec2_dtor, {pos});
    const uint32_t parent = ld32(c + F_NODE);
    vcall(parent, node::addChildZTag, {parent, ld32(c + F_BLOCK), 10, uint32_t(col + row * 10 + 1000)});
}

// after an open: the newly opened sphere gets the cursor and its reward texts; then the reachable spheres are
// recomputed from the start, their animations follow their status, and when every reachable reward is open the
// "complete" popup opens once
void SphereUI::updateStatus(uint32_t self, uint32_t dt) {
    (void)dt;
    bool anyNew = false;
    for (int32_t row = 0; row < 5; row++)
        for (int32_t col = 0; col < 9; col++) {
            const uint32_t b = ld32(cell(self, F_BLOCK, row, col));
            if (b) anyNew = anyNew || ld8(b + B_NEW) != 0;
        }
    if (anyNew) {
        bool first = true;
        for (int32_t row = 0; row < 5; row++)
            for (int32_t col = 0; col < 9; col++) {
                const uint32_t bc = cell(self, F_BLOCK, row, col);
                if (!ld32(bc)) continue;
                if (!ld8(ld32(bc) + B_NEW) || !first) {
                    st8(ld32(bc) + B_SELECTED, 0);
                } else {
                    showReward(self, ld32(cell(self, F_KIND, row, col)));
                    st8(ld32(bc) + B_SELECTED, 1);
                    setCursor(self, row, col, 1);
                    first = false;
                    const uint32_t cursor = ld32(self + F_CURSOR);
                    if (cursor && !vcall(cursor, node::isVisible, {cursor}))
                        vcall(ld32(self + F_CURSOR), node::setVisible, {ld32(self + F_CURSOR), 1});
                }
                st8(ld32(bc) + B_NEW, 0);
            }
        updateReleaseButton(self);
    }
    checkUpRoute(self, int32_t(ld32(self + F_START_COL)), int32_t(ld32(self + F_START_ROW)), 1);
    checkDownRoute(self, int32_t(ld32(self + F_START_COL)), int32_t(ld32(self + F_START_ROW)), 1);
    checkRightRoute(self, int32_t(ld32(self + F_START_COL)), int32_t(ld32(self + F_START_ROW)), 1);
    checkLeftRoute(self, int32_t(ld32(self + F_START_COL)), int32_t(ld32(self + F_START_ROW)), 1);
    for (int32_t row = 0; row < 5; row++)
        for (int32_t col = 0; col < 9; col++) {
            const uint32_t bc = cell(self, F_BLOCK, row, col);
            const uint32_t b = ld32(bc);
            if (!b) continue;
            const uint32_t status = ld32(bc + (F_STATUS - F_BLOCK));
            if (status == ld32(b + B_STATUS)) continue;
            st32(b + B_STATUS, status);
            const uint32_t s = ld32(ld32(bc) + B_STATUS);
            if (s > 2) continue;
            const uint32_t arm = ld32(ld32(bc) + B_ARMATURE);
            const uint32_t anim = vcall(arm, 0x298, {arm});           // getAnimation
            vcall(anim, 0x70, {anim, s, 0xFFFFFFFF, 0xFFFFFFFF});    // playWithIndex(status, -1, -1)
        }
    if (!ld8(ld32(self + F_SPHERE_DATA) + 0x90)) return;
    for (int32_t row = 0; row < 5; row++)
        for (int32_t col = 0; col < 9; col++) {
            if (!ld32(cell(self, F_BLOCK, row, col))) continue;
            if (int32_t(ld32(cell(self, F_KIND, row, col))) > 999) continue;
            if (ld32(cell(self, F_STATUS, row, col)) == 1) continue;
            return;                                              // a reward still closed
        }
    call(a::openPopUpIncentiveComp, {self});
    st8(ld32(self + F_SPHERE_DATA) + 0x90, 0);
}

// the cursor goes to the sphere that can be opened (status 2) with the best reward: by reward type, then by the
// lowest cost
void SphereUI::setOpenCursor(uint32_t self) {
    int32_t bestRow = -1, bestCol = -1;
    for (int32_t row = 0; row < 5; row++)
        for (int32_t col = 0; col < 9; col++) {
            if (ld32(cell(self, F_STATUS, row, col)) != 2) continue;
            const int32_t kind = int32_t(ld32(cell(self, F_KIND, row, col)));
            if (kind > 23) continue;
            const uint32_t rec = self + 0x34 + 0x1C * uint32_t(kind - 1);
            const int32_t cost = int32_t(ld32(rec));
            if (cost < 0) continue;
            bool take;
            if (bestCol == -1) {
                take = true;
            } else {
                const uint32_t typeNew = ld32(rec + 4);
                const uint32_t best = self + 0x34 + 0x1C * (ld32(cell(self, F_KIND, bestRow, bestCol)) - 1);
                const uint32_t typeBest = ld32(best + 4);
                const auto cheaper = [&] { return int32_t(ld32(best)) > cost; };
                if (typeBest == typeNew) {
                    take = cheaper();
                } else if (typeBest > 23) {
                    take = false;
                } else {
                    switch (typeBest) {
                    case 0: case 3: take = typeNew <= 22 && ((1u << typeNew) & 0x601000); break;
                    case 5: take = typeNew <= 22 && ((1u << typeNew) & 0x601009); break;
                    case 7: take = typeNew <= 22 && ((1u << typeNew) & 0x601029); break;
                    case 12: take = typeNew == 21; break;
                    case 13: take = typeNew > 23 || !((1u << typeNew) & 0x9C0000); break;
                    case 18: case 19: case 20: case 23:
                        take = typeNew > 23 || !((1u << typeNew) & 0x9C0000) || cheaper();
                        break;
                    case 22: take = typeNew == 12 || typeNew == 21; break;
                    default: take = false; break;
                    }
                }
            }
            if (take) {
                bestRow = row;
                bestCol = col;
            }
        }
    if (bestRow != -1 && bestCol != -1 && ld32(cell(self, F_BLOCK, bestRow, bestCol))) {
        showReward(self, ld32(cell(self, F_KIND, bestRow, bestCol)));
        st8(ld32(cell(self, F_BLOCK, bestRow, bestCol)) + B_SELECTED, 1);
        setCursor(self, bestRow, bestCol, 0);
    }
    vcall(ld32(self + F_CURSOR), node::setVisible, {ld32(self + F_CURSOR), 1});
    updateReleaseButton(self);
}

uint32_t SphereUI::setCursor(uint32_t self, int32_t row, int32_t col, uint32_t playSe) {
    for (int32_t r = 0; r < 5; r++)
        for (int32_t c = 0; c < 9; c++) {
            const uint32_t n = ld32(cell(self, F_NODE, r, c));
            if (n) vcall(n, 0x130, {n, CURSOR_TAG, 1});           // removeChildByTag(tag, cleanup)
        }
    if (playSe == 1) call(a::AudioPlaySystemSEDecide, {});
    const uint32_t n = ld32(cell(self, F_NODE, row, col));
    vcall(n, node::addChildZTag, {n, ld32(self + F_CURSOR), 20, CURSOR_TAG});
    return 1;
}

// the selected sphere: 3 = can be opened (enough points), 0 = not enough points, 1 = already open, 2 = closed;
// 4 = none selected
uint32_t SphereUI::isOpenIncentive(uint32_t self, uint32_t playSe) {
    for (int32_t row = 0; row < 5; row++)
        for (int32_t col = 0; col < 9; col++) {
            const uint32_t b = ld32(cell(self, F_BLOCK, row, col));
            if (!b || !ld8(b + B_SELECTED)) continue;
            const uint32_t status = ld32(cell(self, F_STATUS, row, col));
            if (status == 2) {
                const uint32_t k = ld32(cell(self, F_KIND, row, col));
                if (int32_t(ld32(ld32(self + F_SPHERE_DATA) + 0x88)) >= int32_t(ld32(self + 0x1C * k + 0x18))) {
                    call(a::AudioPlaySystemSEDecide, {});
                    return 0;
                }
                playOpenSE();
                return 3;
            }
            if (status == 1) { playOpenSE(); return 1; }
            if (status == 0) { playOpenSE(); return 2; }
        }
    if (playSe == 1) playOpenSE();
    return 4;
}

// ------------------------------------------------------------------------------------------------ path walking
namespace {
using Check = uint32_t (*)(uint32_t, int32_t, int32_t, uint32_t);
// a route step into cell (row, col): which status check its piece continues with
uint32_t route(uint32_t self, int32_t col, int32_t row, uint32_t flag, uint32_t kindA, Check a, uint32_t kindB, Check b,
               uint32_t kindC, Check c) {
    const uint32_t kind = ld32(cell(self, F_KIND, row, col));
    uint32_t r;
    if (kind == kindA) r = a(self, col, row, flag);
    else if (kind == kindB) r = b(self, col, row, flag);
    else if (kind == kindC) r = c(self, col, row, flag);
    else return 0;
    st32(cell(self, F_STATUS, row, col), r);
    return r;
}

// a status step into cell (row, col): a path piece continues the walk (or ends it), a sphere takes a status
// (2 = can be opened when reached from an open one) and the walk goes on from it in three directions
uint32_t status(uint32_t self, int32_t col, int32_t row, uint32_t flag, const uint32_t pieces[6], Check onPiece[6],
                Check next[3]) {
    const uint32_t kind = ld32(cell(self, F_KIND, row, col));
    for (int k = 0; k < 6; k++) {
        if (kind != pieces[k]) continue;
        if (!onPiece[k]) return 0;
        const uint32_t r = onPiece[k](self, col, row, flag);
        st32(cell(self, F_STATUS, row, col), r);
        return r;
    }
    const uint32_t st = cell(self, F_STATUS, row, col);
    uint32_t s = ld32(st);
    if (!s && (flag ^ 1) == 0) {
        s = 2;
        st32(st, 2);
    }
    next[0](self, col, row, s == 1);
    next[1](self, col, row, ld32(st) == 1);
    next[2](self, col, row, ld32(st) == 1);
    return ld32(st);
}
}  // namespace

uint32_t SphereUI::checkUpRoute(uint32_t self, int32_t col, int32_t row, uint32_t flag) {
    if (row < 1) return 0;
    return route(self, col, row - 1, flag, 1006, checkRightStatus, 1005, checkLeftStatus, 1001, checkUpStatus);
}
uint32_t SphereUI::checkDownRoute(uint32_t self, int32_t col, int32_t row, uint32_t flag) {
    if (row > 4) return 0;
    return route(self, col, row + 1, flag, 1004, checkRightStatus, 1003, checkLeftStatus, 1001, checkDownStatus);
}
uint32_t SphereUI::checkRightRoute(uint32_t self, int32_t col, int32_t row, uint32_t flag) {
    if (col > 8) return 0;
    return route(self, col + 1, row, flag, 1005, checkDownStatus, 1003, checkUpStatus, 1002, checkRightStatus);
}
uint32_t SphereUI::checkLeftRoute(uint32_t self, int32_t col, int32_t row, uint32_t flag) {
    if (col < 1) return 0;
    return route(self, col - 1, row, flag, 1006, checkDownStatus, 1004, checkUpStatus, 1002, checkLeftStatus);
}

uint32_t SphereUI::checkUpStatus(uint32_t self, int32_t col, int32_t row, uint32_t flag) {
    if (row < 1) return 0;
    static const uint32_t pieces[6] = {1001, 2001, 1005, 2005, 1006, 2006};
    static Check on[6] = {checkUpRoute, checkUpRoute, checkLeftRoute, checkLeftRoute, checkRightRoute, checkRightRoute};
    static Check next[3] = {checkUpRoute, checkRightRoute, checkLeftRoute};
    // 1002..1004 end the walk
    const uint32_t kind = ld32(cell(self, F_KIND, row - 1, col));
    if (kind >= 1002 && kind <= 1004) return 0;
    return status(self, col, row - 1, flag, pieces, on, next);
}
uint32_t SphereUI::checkDownStatus(uint32_t self, int32_t col, int32_t row, uint32_t flag) {
    if (row > 4) return 0;
    static const uint32_t pieces[6] = {1001, 2001, 1003, 2003, 1004, 2004};
    static Check on[6] = {checkDownRoute, checkDownRoute, checkLeftRoute, checkLeftRoute, checkRightRoute,
                          checkRightRoute};
    static Check next[3] = {checkDownRoute, checkRightRoute, checkLeftRoute};
    const uint32_t kind = ld32(cell(self, F_KIND, row + 1, col));
    if (kind == 1002 || kind == 1005 || kind == 1006) return 0;
    return status(self, col, row + 1, flag, pieces, on, next);
}
uint32_t SphereUI::checkLeftStatus(uint32_t self, int32_t col, int32_t row, uint32_t flag) {
    if (col < 1) return 0;
    static const uint32_t pieces[6] = {1002, 2002, 1004, 2004, 1006, 2006};
    static Check on[6] = {checkLeftRoute, checkLeftRoute, checkUpRoute, checkUpRoute, checkDownRoute, checkDownRoute};
    static Check next[3] = {checkUpRoute, checkDownRoute, checkLeftRoute};
    const uint32_t kind = ld32(cell(self, F_KIND, row, col - 1));
    if (kind == 1001 || kind == 1003 || kind == 1005) return 0;
    return status(self, col - 1, row, flag, pieces, on, next);
}
uint32_t SphereUI::checkRightStatus(uint32_t self, int32_t col, int32_t row, uint32_t flag) {
    if (col > 8) return 0;
    static const uint32_t pieces[6] = {1002, 2002, 1003, 2003, 1005, 2005};
    static Check on[6] = {checkRightRoute, checkRightRoute, checkUpRoute, checkUpRoute, checkDownRoute, checkDownRoute};
    static Check next[3] = {checkUpRoute, checkDownRoute, checkRightRoute};
    const uint32_t kind = ld32(cell(self, F_KIND, row, col + 1));
    if (kind == 1001 || kind == 1004 || kind == 1006) return 0;
    return status(self, col + 1, row, flag, pieces, on, next);
}

RW_FUNCTION(0x6BF038, "SphereUI::create", SphereUI::create);
RW_FUNCTION(0x793288, "SphereUI::SphereUI", SphereUI::construct);
RW_FUNCTION(0x793318, "SphereUI::~SphereUI", SphereUI::destruct);
RW_FUNCTION(0x7937B4, "SphereUI::init", SphereUI::init);
RW_FUNCTION(0x793B18, "SphereUI::load_board", SphereUI::loadBoard);
RW_FUNCTION(0x793E38, "SphereUI::create_board", SphereUI::createBoard);
RW_FUNCTION(0x79349C, "SphereUI::create_sphere", SphereUI::createSphere);
RW_FUNCTION(0x7941CC, "SphereUI::update_status", SphereUI::updateStatus);
RW_FUNCTION(0x794C0C, "SphereUI::setOpenCursor", SphereUI::setOpenCursor);
RW_FUNCTION(0x7955EC, "SphereUI::set_cursol", SphereUI::setCursor);
RW_FUNCTION(0x79597C, "SphereUI::is_open_incentive", SphereUI::isOpenIncentive);
RW_FUNCTION(0x795B3C, "SphereUI::check_up_route", SphereUI::checkUpRoute);
RW_FUNCTION(0x795BA0, "SphereUI::check_down_route", SphereUI::checkDownRoute);
RW_FUNCTION(0x795C04, "SphereUI::check_right_route", SphereUI::checkRightRoute);
RW_FUNCTION(0x795C68, "SphereUI::check_left_route", SphereUI::checkLeftRoute);
RW_FUNCTION(0x795D30, "SphereUI::check_up_status", SphereUI::checkUpStatus);
RW_FUNCTION(0x7960AC, "SphereUI::check_down_status", SphereUI::checkDownStatus);
RW_FUNCTION(0x795E4C, "SphereUI::check_left_status", SphereUI::checkLeftStatus);
RW_FUNCTION(0x795F7C, "SphereUI::check_right_status", SphereUI::checkRightStatus);
