// SceneMyPage: the banner area (a BaseSlideView of the notice banners with dot navigation and arrows).
#include "SceneMyPage_lib.hpp"

using namespace mypage;

namespace {
constexpr uint32_t BaseSlideView_create = 0x534E79, BaseSlideView_setScreenWidth = 0x526595;
constexpr uint32_t BaseSlideView_setIndex = 0x526141, BaseSlideView_setItemIDs = 0x5260AD;
constexpr uint32_t BaseSlideView_setSlideEnabled = 0x526111, BaseSlideView_updateLayout = 0x525EB5;
constexpr uint32_t IntVector_pushSlow = 0x4D6CC1, IntVector_copy = 0x4AF707;
constexpr uint32_t Sprite_createFile = 0x8A637D;          // Sprite::create(const string&)
constexpr uint32_t FunctionWidgetInt_swap = 0x54B1C5, FunctionWidgetW_swap = 0x547159, FunctionVV_swap = 0x4EE86D;
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5, ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;
constexpr uint32_t Armature_create = 0x79F3C7;
constexpr uint32_t ImageButton_setTargetAnchorPoint = 0x5318CF;
constexpr uint32_t VT_banner_37 = 0xF61344, VT_banner_38 = 0xF61268, VT_banner_39 = 0xF6123C;
constexpr uint32_t VT_banner_40 = 0xF61210, VT_banner_41 = 0xF611E4;
constexpr uint32_t S_AREA_BANNER = 0xE038EF, S_TXT_LIMIT = 0xDF9EED, S_AREA_DOT = 0xE038FB;
constexpr uint32_t S_DOT_PNG = 0xE03909, S_DOT_NAME = 0xE0391E;   // img/ui/Dot_Navi1.png, dot_navi%d
constexpr uint32_t S_ARROW_LEFT = 0xE03929, S_ARROW_RIGHT = 0xE0393F;
constexpr uint32_t S_ARROW_JSON = 0xE00B10, S_ARROW_ANIM = 0xE00B43, S_LEFT = 0xE033B4, S_RIGHT = 0xE00B59;
constexpr uint32_t OFF_SLIDE = 0x400, OFF_BANNER_IDS = 0x3EC, OFF_SLIDE_TIME = 0x3F8;

// std::function slot `slot` of the slide view = inline lambda (swap with a temporary, destroyed after)
void setSlideFunction(uint32_t slide, uint32_t slot, uint32_t swap, uint32_t vt, std::initializer_list<uint32_t> caps) {
    Frame f;
    uint32_t fo = newFunction(f, vt, caps);
    call(swap, {fo, slide + slot});
    functionDestroy(fo);
}

// an ArrowAnim armature playing `move` on `area`, with an invisible button over it ($_40 / $_41)
void arrow(uint32_t self, uint32_t area, uint32_t move, uint32_t moveLen, uint32_t vt) {
    uint32_t arm;
    {
        Frame f;
        uint32_t name = newStr(f, S_ARROW_ANIM, 9);
        arm = call(Armature_create, {name});
        strFree(name);
    }
    uint32_t anim = vcall(arm, 0x298, {arm});   // getAnimation
    {
        Frame f;
        uint32_t name = newStr(f, move, moveLen);
        vcall(anim, 0x68, {anim, name, 0xFFFFFFFFu, 0xFFFFFFFFu});   // play(name, -1, -1)
        strFree(name);
    }
    vcall(area, node::addChild, {area, arm});
    Frame f;
    uint32_t empty = newStr(f, S_EMPTY, 0);
    uint32_t button = call(ImageButton_create, {empty, 1});
    strFree(empty);
    call(ImageButton_setTargetView, {button, arm, fb(1.0f)});
    uint32_t anchor = f.alloc(8);
    call(wl::Vec2_ctorXY, {anchor, fb(-0.5f), fb(-0.5f)});
    call(ImageButton_setTargetAnchorPoint, {button, anchor});
    call(wl::Vec2_dtor, {anchor});
    vcall(arm, node::addChild, {arm, button});
    uint32_t fo = newFunction(f, vt, {self});
    call(ImageButton_addTouchEventListener, {button, fo});
    functionDestroy(fo);
}
}  // namespace

void SceneMyPage_initBannerArea(uint32_t self) {
    uint32_t slide = call(BaseSlideView_create, {});
    st32(self + OFF_SLIDE, slide);
    vcall(slide, 0x31C, {slide, 1});
    call(smain::lib::EventListenerTouchOneByOne_setSwallowTouches, {ld32(ld32(self + OFF_SLIDE) + 0x5C4), 1});
    uint32_t area = findNode(ld32(self + 0x290), S_AREA_BANNER, 11);
    {
        uint32_t s = ld32(self + OFF_SLIDE);
        vcall(s, node::setContentSize, {s, vcall(area, node::getContentSize, {area})});
        uint32_t s2 = ld32(self + OFF_SLIDE);
        call(BaseSlideView_setScreenWidth, {s2, ld32(vcall(area, node::getContentSize, {area}))});
    }
    // banner ids 0 .. count - 1 (the clear and push_back inlined)
    const uint32_t ids = self + OFF_BANNER_IDS;
    {
        uint32_t b = ld32(ids), e = ld32(ids + 4);
        if (e != b) st32(ids + 4, e + (~3u & ~((e - 4) - b)));
    }
    uint32_t info = ld32(call(APIManager_getInstance, {}) + 0x314);
    {
        Frame f;
        uint32_t value = f.alloc(4);
        st32(value, 0);
        if (ld32(info + 0x10) != ld32(info + 0xC)) {
            uint32_t i = 1;
            bool more;
            do {
                uint32_t end = ld32(ids + 4);
                if (end != ld32(ids + 8)) {
                    uint32_t e = 0;
                    if (end) {
                        st32(end, i - 1);
                        e = ld32(ids + 4);
                    }
                    st32(ids + 4, e + 4);
                } else {
                    call(IntVector_pushSlow, {ids, value});
                }
                st32(value, i);
                uint32_t count = uint32_t(int32_t(ld32(info + 0x10) - ld32(info + 0xC)) >> 4);
                more = i < count;
                i++;
            } while (more);
        }
    }
    uint32_t limit = findNode(ld32(self + 0x290), S_TXT_LIMIT, 9);
    uint32_t n = uint32_t(int32_t(ld32(ids + 4) - ld32(ids)) >> 2);
    if (n - 1 > 6) {
        vcall(area, node::setVisible, {area, 0});
        vcall(limit, node::setVisible, {limit, 0});
        return;
    }
    static const float spacingByCount[7] = {24.0f, 24.0f, 22.0f, 20.0f, 20.0f, 18.0f, 15.0f};
    const float spacing = spacingByCount[n - 1];
    uint32_t dots = findNode(ld32(self + 0x290), S_AREA_DOT, 13);
    if (ld32(ids + 4) != ld32(ids)) {
        float dotW = 0.0f, startX = 0.0f;
        uint32_t i = 0;
        do {
            uint32_t dot;
            {
                Frame f;
                uint32_t path = newStr(f, S_DOT_PNG, 20);
                dot = call(Sprite_createFile, {path});
                strFree(path);
            }
            {
                Frame f;
                uint32_t buf = stringBuffer(f, {S_DOT_NAME, i + 1});
                uint32_t name = newStr(f, buf);
                vcall(dots, node::addChildZName, {dots, dot, 0, name});
                strFree(name);
            }
            if (i == 0) {
                dotW = ldf(vcall(dot, node::getContentSize, {dot}));
                uint32_t bytes = ld32(ids + 4) - ld32(ids);
                uint32_t areaSize = vcall(dots, node::getContentSize, {dots});
                uint32_t cnt = uint32_t(int32_t(bytes) >> 2);
                float total = dotW * float(cnt);
                float gaps = spacing * float(cnt - 1);
                startX = (ldf(areaSize) - (total + gaps)) * 0.5f;
            }
            float x = startX + float(int32_t(i)) * (spacing + dotW);
            vcall(dot, node::setPositionX, {dot, fb(x)});
            i++;
        } while (i < uint32_t(int32_t(ld32(ids + 4) - ld32(ids)) >> 2));
    }
    // the slide view's item factory ($_37), item update ($_38) and page change ($_39)
    setSlideFunction(ld32(self + OFF_SLIDE), 0x500, FunctionWidgetInt_swap, VT_banner_37, {area});
    setSlideFunction(ld32(self + OFF_SLIDE), 0x518, FunctionWidgetW_swap, VT_banner_38, {self, dots, limit});
    setSlideFunction(ld32(self + OFF_SLIDE), 0x560, FunctionVV_swap, VT_banner_39, {self});
    call(BaseSlideView_setIndex, {ld32(self + OFF_SLIDE), 0});
    {
        uint32_t s = ld32(self + OFF_SLIDE);
        Frame f;
        uint32_t copy = f.alloc(12);
        call(IntVector_copy, {copy, ids});
        call(BaseSlideView_setItemIDs, {s, copy, 0});
        call(IntVector_baseDtor, {copy});
    }
    call(BaseSlideView_setSlideEnabled, {ld32(self + OFF_SLIDE), 1});
    call(smain::lib::EventListenerTouchOneByOne_setSwallowTouches, {ld32(ld32(self + OFF_SLIDE) + 0x5C4), 1});
    call(BaseSlideView_updateLayout, {ld32(self + OFF_SLIDE), 1});
    vcall(area, node::addChild, {area, ld32(self + OFF_SLIDE)});
    if (uint32_t(int32_t(ld32(ids + 4) - ld32(ids)) >> 2) < 2) return;

    // more than one banner: the arrows, and the auto slide time (Misc 700, in 1/10000 s)
    uint32_t left = findNode(ld32(self + 0x290), S_ARROW_LEFT, 21);
    vcall(left, node::setVisible, {left, 1});
    {
        uint32_t adm = call(ArmatureDataManager_getInstance, {});
        Frame f;
        uint32_t json = newStr(f, S_ARROW_JSON, 39);
        call(ArmatureDataManager_addArmatureFileInfo, {adm, json});
        strFree(json);
    }
    arrow(self, left, S_LEFT, 4, VT_banner_40);
    uint32_t right = findNode(ld32(self + 0x290), S_ARROW_RIGHT, 22);
    vcall(right, node::setVisible, {right, 1});
    arrow(self, right, S_RIGHT, 5, VT_banner_41);
    Frame f;
    uint32_t misc = masterRow(f, Misc_create, 700);
    int32_t ms = scene::safe32(ld32(misc), 4);
    release(ld32(misc + 4));
    stf(self + OFF_SLIDE_TIME, float(ms) / 10000.0f);
}

RW_FUNCTION(0x63E420, "SceneMyPage::initBannerArea", SceneMyPage_initBannerArea);
