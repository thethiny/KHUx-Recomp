// Small game classes: IInOutAnimation, LWFUtil, GestureRecognizer, LWFNodeGroup, SceneMovie,
// CampaignAnimationView.
#include "../core/Guest.hpp"
#include "../scene/SceneCommon.hpp"

namespace w4b2 {
using namespace scene;

constexpr uint32_t FN_VV_COPY = 0x4683CD;    // std::function<void()>::function(const function&)
constexpr uint32_t FN_VV_ASSIGN = 0x4EB9CD;  // std::function<void()>::operator=(const function&)
constexpr uint32_t REF_AUTORELEASE = 0x8FD0D5;
constexpr uint32_t NODE_CTOR = 0x892531, NODE_DTOR = 0x8927ED;
constexpr uint32_t AEABI_MEMSET = 0x40E7A0;  // __aeabi_memset(dest, n, c)

// a std::function<void()> holding a [this] lambda (vtable of its __func) on the guest stack
static uint32_t lambdaFunction(Frame &f, uint32_t vtable, uint32_t self) {
    return newFunction(f, vtable, {self});
}

// ============================================================================================ IInOutAnimation
struct IInOutAnimation {
    static uint32_t runOutAnimation(uint32_t self, uint32_t callback) {
        Frame f;
        call(0x60E86D, {call(0x60C26D, {}), 0});  // SceneManager::getInstance()->setCanTouchHeader(false)
        uint32_t fn = ld32(ld32(self) + 0xC);     // virtual runOutAnimationImpl(std::function)
        uint32_t tmp = f.alloc(20);
        call(FN_VV_COPY, {tmp, callback});
        call(fn, {self, tmp});
        functionDestroy(tmp);
        return 0;
    }
};
RW_FUNCTION(0x6F24B4, "IInOutAnimation::runOutAnimation", IInOutAnimation::runOutAnimation);

// ============================================================================================ LWFUtil
struct LWFUtil {
    static uint32_t preloadResources(uint32_t path) {
        Frame f;
        uint32_t cache = call(0x7EFF59, {});  // LWFResourceCache::sharedLWFResourceCache
        uint32_t s = f.alloc(12);
        uint32_t data = f.alloc(8);
        strInitC(s, strData(path));
        call(0x7F0209, {data, cache, s});  // loadLWFData -> shared_ptr<LWFData>
        strFree(s);
        if (ld32(data + 4) != 0) call(fn::shared_release, {ld32(data + 4)});
        return 0;
    }
};
RW_FUNCTION(0x491360, "LWFUtil::preloadResources", LWFUtil::preloadResources);

// ============================================================================================ GestureRecognizer
// enabled +4 (byte), start point +8 (Vec2), listener +0x10, callback std::function +0x18 (callable +0x28)
struct GestureRecognizer {
    static uint32_t construct(uint32_t self, uint32_t node) {
        Frame f;
        st8(self + 4, 0);
        call(0x8E4B95, {self + 8});  // Vec2::Vec2
        st32(self + 0x28, 0);
        uint32_t listener = call(0x8FACA5, {self + 0x18});  // EventListenerTouchOneByOne::create
        st32(self + 0x10, listener);
        uint32_t tmp = f.alloc(20);
        // onTouchBegan / Moved / Ended / Cancelled = [this] lambdas (swapped into the listener)
        const uint32_t vtables[4] = {0xF55918, 0xF558EC, 0xF558C0, 0xF55894};
        const uint32_t fields[4] = {0x50, 0x68, 0x80, 0x98};
        for (int k = 0; k < 4; k++) {
            uint32_t l = ld32(self + 0x10);
            st32(tmp + 16, tmp);
            st32(tmp, vtables[k]);
            st32(tmp + 4, self);
            call(k == 0 ? 0x483E4D : 0x4F3699, {tmp, l + fields[k]});  // std::function::swap
            functionDestroy(tmp);
        }
        uint32_t dispatcher = ld32(call(fn::Director_getInstance, {}) + 0x84);
        call(0x8F3649, {dispatcher, ld32(self + 0x10), node});  // addEventListenerWithSceneGraphPriority
        return self;
    }
    static uint32_t destruct(uint32_t self) {
        uint32_t dispatcher = ld32(call(fn::Director_getInstance, {}) + 0x84);
        call(0x8F2F29, {dispatcher, ld32(self + 0x10)});  // removeEventListener
        functionDestroy(self + 0x18);
        call(0x8E4C17, {self + 8});  // Vec2::~Vec2
        return self;
    }
};
RW_FUNCTION(0x4F348C, "GestureRecognizer::GestureRecognizer", GestureRecognizer::construct);
RW_FUNCTION(0x4F3620, "GestureRecognizer::~GestureRecognizer", GestureRecognizer::destruct);

// ============================================================================================ LWFNodeGroup
// (a cocos2d::Node) on-enter / on-exit callbacks +0x210 / +0x228, LWF names vector<const char*> +0x270,
// current -1 +0x27C, flag +0x284, +0x288
struct LWFNodeGroup {
    static uint32_t create(uint32_t names, uint32_t namesEnd, uint32_t count) {
        uint32_t obj = call(fn::operator_new, {0x290});
        call(0x5D4E15, {obj, names, namesEnd, count});
        call(REF_AUTORELEASE, {obj});
        return obj;
    }
    static uint32_t construct(uint32_t self, uint32_t names, uint32_t namesEnd) {
        Frame f;
        call(NODE_CTOR, {self});
        st32(self, ld32(0xFD67A0) + 8);
        call(0x5D4F75, {self + 0x270, names, namesEnd});  // vector<const char*>(first, last)
        st32(self + 0x27C, 0xFFFFFFFF);
        st8(self + 0x284, 0);
        st32(self + 0x288, 0);
        uint32_t a = lambdaFunction(f, 0xF5D574, self);
        call(FN_VV_ASSIGN, {self + 0x210, a});
        functionDestroy(a);
        uint32_t b = lambdaFunction(f, 0xF5D548, self);
        call(FN_VV_ASSIGN, {self + 0x228, b});
        functionDestroy(b);
        return self;
    }
    static uint32_t preloadResources(uint32_t self) {
        Frame f;
        uint32_t s = f.alloc(12);
        uint32_t end = ld32(self + 0x274);
        for (uint32_t p = ld32(self + 0x270); p != end; p += 4) {
            strInitC(s, ld32(p));
            call(0x491361, {s});  // LWFUtil::preloadResources
            strFree(s);
        }
        return 0;
    }
    static void destroyDelete(uint32_t self) {
        st32(self, ld32(0xFD67A0) + 8);
        call(0x5D4FD7, {self + 0x270});  // __vector_base<const char*>::~__vector_base
        call(NODE_DTOR, {self});
        call(fn::operator_delete, {self});
    }
};
RW_FUNCTION(0x5D4DD0, "LWFNodeGroup::create", LWFNodeGroup::create);
RW_FUNCTION(0x5D4E14, "LWFNodeGroup::LWFNodeGroup", LWFNodeGroup::construct);
RW_FUNCTION(0x5D521C, "LWFNodeGroup::preloadResources", LWFNodeGroup::preloadResources);
RW_FUNCTION(0x5D52FC, "LWFNodeGroup::~LWFNodeGroup (deleting)", LWFNodeGroup::destroyDelete);

// ============================================================================================ SceneMovie
// (a cocos2d::Layer) the opening movie: a VideoPlayer at +0x290
struct SceneMovie {
    static uint32_t create() {
        uint32_t obj = call(fn::operator_new, {0x298});
        call(0x88A7D1, {obj});  // Layer::Layer
        st32(obj, ld32(0xFD68C0) + 8);
        if (call(0x638D89, {obj}) == 1) {  // init
            call(REF_AUTORELEASE, {obj});
            return obj;
        }
        vcall(obj, 4, {obj});
        return 0;
    }
    static void destroyDelete(uint32_t self) {
        st32(self, ld32(0xFD68C0) + 8);
        uint32_t r = call(0x88A88D, {self});  // Layer::~Layer
        call(fn::operator_delete, {r});
    }
    static uint32_t init(uint32_t self) {
        Frame f;
        if (call(0x88A8A1, {self}) != 1) return 0;  // Layer::init
        uint32_t a = lambdaFunction(f, 0xF60AD8, self);
        call(FN_VV_ASSIGN, {self + 0x210, a});
        functionDestroy(a);
        uint32_t b = lambdaFunction(f, 0xF60AAC, self);
        call(FN_VV_ASSIGN, {self + 0x228, b});
        functionDestroy(b);
        call(0x611691, {call(0x60C26D, {})});  // SceneManager::removeBackground
        uint32_t mgr = call(0x60C26D, {});
        uint32_t empty = newEmptyFunction(f);
        call(0x61154D, {mgr, empty});  // SceneManager::hideHeader(std::function<void()>())
        functionDestroy(empty);
        uint32_t size = f.alloc(8);
        call(fn::Director_getVisibleSize, {size, call(fn::Director_getInstance, {})});
        float w = ldf(size), h = ldf(size + 4);
        uint32_t player = call(0x5A358D, {});  // VideoPlayer::create
        st32(self + 0x290, player);
        float sx = w / 960.0f;
        float sy = h / 540.0f;
        float s = bitsf(call(0x41DEE4, {fbits(sx), fbits(sy)}));  // fminf
        float pw = s * 960.0f;
        float ph = s * 540.0f;
        uint32_t setContentSize = ld32(ld32(player) + 0xA8);
        uint32_t sz = f.alloc(8);
        call(0x8DF027, {sz, fbits(pw), fbits(ph)});  // Size::Size
        call(setContentSize, {player, sz});
        uint32_t p = ld32(self + 0x290);
        uint32_t setPosition = ld32(ld32(p) + 0x4C);
        float x = ldf(size) * 0.5f;
        float y = ldf(size + 4) * 0.5f;
        uint32_t pos = f.alloc(8);
        call(0x8E4B9D, {pos, fbits(x), fbits(y)});  // Vec2::Vec2
        call(setPosition, {p, pos});
        call(0x8E4C17, {pos});
        p = ld32(self + 0x290);
        vcall(p, 0x29C, {p, 1});  // setFullScreenEnabled / touch enabled
        uint32_t touch = f.alloc(20);
        st32(touch + 16, touch);
        st32(touch, 0xF60A80);  // [](Ref*, TouchEventType) {}
        st8(touch + 4, 0);
        call(0x826CCF, {ld32(self + 0x290), touch});  // Widget::addTouchEventListener
        functionDestroy(touch);
        p = ld32(self + 0x290);
        uint32_t ev = f.alloc(20);
        st32(ev + 16, ev);
        st32(ev, 0xF60A54);  // [this](Ref*, VideoPlayer::EventType)
        st32(ev + 8, self);
        st32(ev + 4, 0);
        vcall(p, 0x358, {p, ev});  // addEventListener
        functionDestroy(ev);
        vcall(self, 0xF0, {self, ld32(self + 0x290)});  // addChild
        p = ld32(self + 0x290);
        uint32_t name = f.alloc(12);
        strInit(name, 0xE03632, 12);  // "op_movie.mp4"
        vcall(p, 0x31C, {p, name});  // setFileName
        strFree(name);
        p = ld32(self + 0x290);
        vcall(p, 0x32C, {p});  // play
        return 1;
    }
};
RW_FUNCTION(0x638CE0, "SceneMovie::create", SceneMovie::create);
RW_FUNCTION(0x638D64, "SceneMovie::~SceneMovie (deleting)", SceneMovie::destroyDelete);
RW_FUNCTION(0x638D88, "SceneMovie::init", SceneMovie::init);

// ============================================================================================ CampaignAnimationView
// (a cocos2d::Node) current campaign +0x270, campaigns vector<shared_ptr<EvCampaign>> +0x274, panel +0x280,
// announce text +0x284
struct CampaignAnimationView {
    static uint32_t create(uint32_t parent, uint32_t campaigns) {
        uint32_t obj = call(fn::operator_new, {0x288});
        call(AEABI_MEMSET, {obj, 0x288, 0});
        call(NODE_CTOR, {obj});
        st32(obj, ld32(0xFD66D0) + 8);
        call(AEABI_MEMSET, {obj + 0x270, 0x18, 0});
        if (call(0x598B91, {obj, parent, campaigns}) == 1) {
            call(REF_AUTORELEASE, {obj});
            return obj;
        }
        vcall(obj, 4, {obj});
        return 0;
    }
    static uint32_t init(uint32_t self, uint32_t parent, uint32_t campaigns) {
        Frame f;
        if (call(0x892B69, {self}) != 1) return 0;  // Node::init
        st32(self + 0x270, 0xFFFFFFFF);
        if (self + 0x274 != campaigns)
            call(0x599A3D, {self + 0x274, ld32(campaigns), ld32(campaigns + 4)});  // vector::assign
        st32(self + 0x280, parent);
        uint32_t s = f.alloc(12);
        strInit(s, 0xE00401, 12);  // "Txt_Announce"
        st32(self + 0x284, call(0x45A9F5, {parent, s}));  // SceneUtil::searchNodeByName
        strFree(s);
        if (ld32(self + 0x284) == 0) {
            uint32_t reader = call(0x7BF1E5, {});  // GUIReader::getInstance
            uint32_t w = call(0x7BF9FD, {reader, 0xE0040E});  // widgetFromJsonFile(StartDeckEdit2_Announce.json)
            st32(self + 0x280, w);
            uint32_t s2 = f.alloc(12);
            strInit(s2, 0xE00401, 12);
            st32(self + 0x284, call(0x45A9F5, {w, s2}));
            strFree(s2);
            vcall(parent, 0xF0, {parent, ld32(self + 0x280)});  // addChild
        }
        uint32_t panel = ld32(self + 0x280);
        vcall(panel, 0x220, {panel, 1});  // setCascadeOpacityEnabled
        call(0x45BD15, {ld32(self + 0x284)});  // SceneUtil::setShadow
        if (ld32(self + 0x278) == ld32(self + 0x274)) {
            vcall(parent, 0xB0, {parent, 0});  // setVisible(false)
        } else {
            st32(self + 0x270, ld32(self + 0x270) + 1);
            call(0x598D59, {self});  // show
        }
        return 1;
    }
    static uint32_t show(uint32_t self) {
        Frame f;
        call(0x893815, {ld32(self + 0x280)});  // stopAllActions
        uint32_t begin = ld32(self + 0x274);
        uint32_t n = uint32_t(int32_t(ld32(self + 0x278) - begin) >> 3);
        uint32_t idx = ld32(self + 0x270);
        if (n <= idx) {
            idx = 0;
            st32(self + 0x270, 0);
        }
        if (n == 0) return 0;
        if (n <= idx) {
            call(0x47CCE1, {});  // vector::at out of range (throws)
            return 0;
        }
        uint32_t text = ld32(self + 0x284);
        uint32_t campaign = ld32(begin + idx * 8);
        uint32_t buf = f.alloc(0x108);
        call(0x45A269, {campaign, buf, ld32(campaign + 0xC) + 0xC, 0x101, 0xC});  // SafeBuffer::read: the name
        uint32_t s = f.alloc(12);
        strInitC(s, buf);
        call(fn::Text_setString, {text, s});
        strFree(s);
        uint32_t panel = ld32(self + 0x280);
        uint32_t fadeIn = call(0x86B115, {fbits(1.0f)});
        uint32_t delay = call(fn::DelayTime_create, {fbits(0.5f)});
        uint32_t fadeOut = call(0x86B25D, {fbits(1.0f)});
        uint32_t cb = lambdaFunction(f, 0xF5B144, self);  // [this] { next campaign }
        uint32_t callFunc = call(fn::CallFunc_create, {cb});
        uint32_t seq = call(fn::Sequence_create, {fadeIn, delay, fadeOut, callFunc, 0});
        call(fn::Node_runAction, {panel, seq});
        functionDestroy(cb);
        return 0;
    }
    static uint32_t hide(uint32_t self) {
        call(0x893815, {ld32(self + 0x280)});
        uint32_t panel = ld32(self + 0x280);
        uint32_t r = call(fn::Node_runAction, {panel, call(0x86B25D, {fbits(1.0f)})});
        st32(self + 0x270, 0);
        return r;
    }
    static void destroyDelete(uint32_t self) {
        st32(self, ld32(0xFD66D0) + 8);
        call(0x573591, {self + 0x274});  // __vector_base<shared_ptr<EvCampaign>>::~__vector_base
        call(NODE_DTOR, {self});
        call(fn::operator_delete, {self});
    }
    static uint32_t setCampaigns(uint32_t self, uint32_t campaigns) {
        if (self + 0x274 != campaigns)
            call(0x599A3D, {self + 0x274, ld32(campaigns), ld32(campaigns + 4)});
        uint32_t visible = ld32(self + 0x278) != ld32(self + 0x274) ? 1 : 0;
        uint32_t panel = ld32(self + 0x280);
        return vcall(panel, 0xB0, {panel, visible});
    }
};
RW_FUNCTION(0x598AF0, "CampaignAnimationView::create", CampaignAnimationView::create);
RW_FUNCTION(0x598B90, "CampaignAnimationView::init", CampaignAnimationView::init);
RW_FUNCTION(0x598D58, "CampaignAnimationView::show", CampaignAnimationView::show);
RW_FUNCTION(0x598F00, "CampaignAnimationView::hide", CampaignAnimationView::hide);
RW_FUNCTION(0x599C30, "CampaignAnimationView::~CampaignAnimationView (deleting)", CampaignAnimationView::destroyDelete);
RW_FUNCTION(0x7258E8, "CampaignAnimationView::setCampaings", CampaignAnimationView::setCampaigns);

}  // namespace w4b2
