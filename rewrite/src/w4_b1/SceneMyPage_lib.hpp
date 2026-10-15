// SceneMyPage rewrites: member offsets, guest addresses, lambda vtables and helpers shared by its files.
#pragma once

#include "SceneMyPage.hpp"

namespace mypage {

using namespace w4b1;
using Self = SceneMyPage;

inline Self *S(uint32_t a) { return rw::host_ptr<Self>(a); }
inline void addRef(uint32_t ctrl) { if (ctrl) guest::addRef(ctrl); }
inline void release(uint32_t ctrl) { if (ctrl) guest::release(ctrl); }
inline uint32_t f32(uint32_t self, uint32_t off) { return ld32(self + off); }

// member offsets
constexpr uint32_t OFF_SHARED_CTL = 0x2C4;        // shared_ptr control block (object at +0x2C0)
constexpr uint32_t OFF_IDS = 0x2C8;               // vector<uint64>
constexpr uint32_t OFF_ACTORS_A = 0x2D4, OFF_ACTORS_B = 0x2E0;   // cocos2d::Vector<AvatarActor*>
constexpr uint32_t OFF_HAS_PARTY = 0x2F8;         // uint8: the user is in a party (user detail byte 0x10)
constexpr uint32_t OFF_POINTS = 0x304;            // vector<Vec2>
constexpr uint32_t OFF_ROUTES = 0x310;            // vector<vector<uint>>
constexpr uint32_t OFF_ROUTE = 0x31C;             // vector<uint>
constexpr uint32_t OFF_VEC_328 = 0x328, OFF_BALOON_SHOWN = 0x334;  // vector<bool>
constexpr uint32_t OFF_BALOONS = 0x340;           // cocos2d::Vector<ui::Widget*>
constexpr uint32_t OFF_ACTORS = 0x34C;            // cocos2d::Vector<AvatarActor*>
constexpr uint32_t OFF_MOTIONS = 0x358;           // vector<Avatar::Motion>
constexpr uint32_t OFF_SCALE_BASE = 0x364, OFF_SCALE = 0x368, OFF_WALK_UNIT = 0x36C;   // floats
constexpr uint32_t OFF_CAMERA = 0x378;            // Vec2
constexpr uint32_t OFF_IDS2 = 0x390;              // vector<uint64>
constexpr uint32_t OFF_COLUMNS = 0x39C;           // int
constexpr uint32_t OFF_SPEED_FAST = 0x3B0, OFF_SPEED_MID = 0x3B4, OFF_SPEED_SLOW = 0x3B8;   // floats
constexpr uint32_t OFF_IMAGES = 0x3D4;            // std::map<int, cocos2d::Image*>
constexpr uint32_t OFF_DOWNLOADS = 0x3E0;         // std::map<int, unsigned> (download handles)
constexpr uint32_t OFF_INTS = 0x3EC;              // vector<int>
constexpr uint32_t OFF_EXPIRE_SECONDS = 0x40C;    // int: Misc 701 days in seconds
constexpr uint32_t OFF_DOWNLOADER = 0x414;        // ImageDownloader*
constexpr uint32_t OFF_SOLO_X = 0x418, OFF_RANK_X = 0x41C, OFF_SOLO_X_OUT = 0x420, OFF_RANK_X_OUT = 0x424;

// lambda vtables (__func + 8)
constexpr uint32_t VT_init_0 = 0xF61E9C, VT_init_1 = 0xF61E44, VT_init_2 = 0xF61DEC, VT_init_3 = 0xF61D94;
constexpr uint32_t VT_init_4 = 0xF61D3C, VT_init_5 = 0xF61CE4, VT_init_6 = 0xF61C8C, VT_init_7 = 0xF61C60;
constexpr uint32_t VT_init_8 = 0xF61C34, VT_init_9 = 0xF61C08, VT_init_10 = 0xF61BDC, VT_init_11 = 0xF61BB0;
constexpr uint32_t VT_init_12 = 0xF61B84, VT_init_13 = 0xF61B58, VT_init_14 = 0xF61B2C;

// own methods (Thumb addresses)
constexpr uint32_t M_initField = 0x63B621, M_initMovePoints = 0x63BDDD, M_initLeftButtons = 0x63CA19;
constexpr uint32_t M_initRightButtons = 0x63D339, M_updateInfoIcon = 0x63DB19, M_initPartyMembers = 0x63DDF1;
constexpr uint32_t M_initBannerArea = 0x63E421, M_cancelShopHandle = 0x639A4D;

// guest functions
constexpr uint32_t Misc_create = 0x786D5D;
constexpr uint32_t SafeBuffer_read = 0x45A269;            // (sb, dst, src, len, offset)
constexpr uint32_t ImageDownloader_ctor = 0x5BCF7D, ImageDownloader_dtor = 0x5BCFA9;
constexpr uint32_t DownloadManager_getInstance = 0x4DECB1, DownloadManager_cancel = 0x4DF62D;
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t SceneManager_setCanTouchHeader = 0x60E86D;
constexpr uint32_t SceneManager_setOnBackKeyCallback = 0x613E5B;
constexpr uint32_t SceneManager_showHeader = 0x6131ED;
constexpr uint32_t SceneManager_changeTickerMessage = 0x610B39;
constexpr uint32_t SceneManager_removeBackground = 0x611691;
constexpr uint32_t SceneManager_setOnChangeProfile = 0x62160F;
constexpr uint32_t SceneUtil_createNodeWithSceneFile = 0x45AA49;
constexpr uint32_t SceneUtil_getSideHeightMargin = 0x45AF59;
constexpr uint32_t SceneUtil_setBlackOutline = 0x45BE95;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t Widget_create = 0x825875;
constexpr uint32_t Widget_addTouchEventListener = 0x826CCF;
constexpr uint32_t ImageView_create = 0x8394C1;
constexpr uint32_t Size_ctor = 0x8DF027;                  // Size(float, float)
constexpr uint32_t GUIReader_getInstance = 0x7BF1E5, GUIReader_widgetFromJsonFile = 0x7BF9FD;
constexpr uint32_t WidgetVector_pushBack = 0x4F6B5D;
constexpr uint32_t BoolVector_pushBack = 0x62161D;        // std::vector<bool>::push_back(const bool&)
constexpr uint32_t ImageButton_create = 0x531239;         // (std::string, bool)
constexpr uint32_t ImageButton_setTargetView = 0x5318C1;  // (btn, node, float)
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t ImageMap_destroy = 0x652511;           // std::__tree<int, Image*>::destroy(node)
constexpr uint32_t UIntMap_destroy = 0x5B42A7;            // std::__tree<int, unsigned>::destroy(node)
constexpr uint32_t IntVector_baseDtor = 0x478961, U64Vector_baseDtor = 0x468499;
constexpr uint32_t MotionVector_baseDtor = 0x485A39, ActorVector_dtor = 0x485A63, WidgetVector_dtor = 0x485A8F;
constexpr uint32_t UIntVector_baseDtor = 0x485ABB, UIntVectorVector_baseDtor = 0x485AE5, Vec2Vector_baseDtor = 0x485B15;

// strings
constexpr uint32_t S_EMPTY = 0xDF8A1A;

// SafeBuffer::read of one byte / word at `offset` of a SafeBuffer object
inline uint32_t sbRead(uint32_t sb, uint32_t offset, uint32_t len) {
    Frame f;
    uint32_t out = f.alloc(4);
    st32(out, 0);
    call(SafeBuffer_read, {sb, out, ld32(sb + 0xC) + offset, len, offset});
    return len == 1 ? ld8(out) : ld32(out);
}

// widget->addTouchEventListener(inline lambda {captures})
inline void addTouchLambda(uint32_t widget, uint32_t vtable, std::initializer_list<uint32_t> caps) {
    Frame f;
    uint32_t fo = newFunction(f, vtable, caps);
    call(Widget_addTouchEventListener, {widget, fo});
    functionDestroy(fo);
}

// in-order walk of a libc++ __tree from begin to the end node (`end` = &root)
inline uint32_t treeNext(uint32_t node) {
    uint32_t r = ld32(node + 4);
    if (r) {
        while (ld32(r)) r = ld32(r);
        return r;
    }
    uint32_t n = node;
    uint32_t p;
    do {
        p = ld32(n + 8);
        if (ld32(p) == n) return p;
        n = p;
    } while (true);
}
}  // namespace mypage
