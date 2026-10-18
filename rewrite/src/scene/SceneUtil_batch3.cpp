// SceneUtil: lambda bodies and std::function slots that run in game but were left after batch 2 (batch 3, named from
// their __func vtables' target_type). The callPrepareMypageAPIs chain: each step's __func holds the two callbacks
// of SceneUtil::callPrepareMypageAPIs (std::function<void(bool)> at +8 and +0x20; 0x38 bytes) and, when the
// previous request succeeded, makes the next request with the next step as its callback.
#include "SceneUi.hpp"

using namespace scene;

namespace {

constexpr uint32_t SM_loadingBlocked = 0x4CC, SM_onFinishLoading = 0x548;
constexpr uint32_t FunctionB_assign = 0x490109;               // std::function<void(bool)>::operator=(const function&)
constexpr uint32_t APIManager_executeCampaignGet = 0x4D346D;  // (api, const std::function<void(bool)>&)
constexpr uint32_t APIManager_executeRaidList = 0x4CC009;
constexpr uint32_t APIManager_executePartyGet = 0x4C69BD;     // (api, uint64 user id, const function&)
constexpr uint32_t APIManager_executeMypageInfoGet = 0x4D35B1;

// the steps' __func vtables ($_66 is made by callPrepareMypageAPIs itself; $_N's body makes $_N-1)
constexpr uint32_t VT_66 = 0xF54668, VT_65 = 0xF553FC, VT_63 = 0xF552C8, VT_62 = 0xF552F4, VT_61 = 0xF55320,
                   VT_60 = 0xF5534C;
constexpr uint32_t VT_64 = 0xF5529C;
constexpr uint32_t CB_A = 8, CB_B = 0x20, STEP_SIZE = 0x38;
constexpr uint32_t APIManager_executeUserSphereCheck = 0x4CCBC1;   // (api, SphereLockType, const function&)
constexpr uint32_t APIManager_executePartyNoticeUpdate = 0x4CED11; // (api, const function&)
constexpr uint32_t TutorialMisc_create = 0x777BB5;                 // (shared_ptr* out, id)
constexpr uint32_t PrepareMypage_partyNotice = 0x48D9E1;           // sub_48D9E0(callbacks, bool)
constexpr uint32_t API_USER_RANK = 0x94;                           // APIManager: SafeBuffer, the Lux rank at +0x40
// SceneUtil text look (sub_4744F4)
constexpr uint32_t Color3B_ctor = 0x9062E7, Color4B_ctor = 0x9064D3, Size_ctor = 0x8DF027;
constexpr uint32_t Text_enableShadow = 0x83A9B1;                   // (text, const Color4B&, const Size&, blur)
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t GOT_COLOR_WHITE = 0xFD6060;                     // &Color3B::WHITE
constexpr uint32_t V_setTextColor = 0x22C;

// one step: on success unblock loading (no "finished loading" callback), then `request(next)` with the next
// step's __func holding copies of both callbacks
template <class F> void chainStepFrom(uint32_t callbacks, uint32_t nextVtable, F request);
template <class F> void chainStep(uint32_t functor, uint32_t okPtr, uint32_t nextVtable, F request) {
    if (!ld8(okPtr)) return;
    chainStepFrom(functor + CB_A, nextVtable, request);
}
// the same from the two callbacks themselves (A, then B 0x18 bytes on)
template <class F> void chainStepFrom(uint32_t callbacks, uint32_t nextVtable, F request) {
    {
        const uint32_t mgr = call(fn::SceneManager_getInstance, {});
        Frame f;
        const uint32_t empty = newEmptyFunction(f);
        st8(mgr + SM_loadingBlocked, 1);
        call(FunctionB_assign, {mgr + SM_onFinishLoading, empty});
        functionDestroy(empty);
    }
    const uint32_t api = call(fn::APIManager_getInstance, {});
    Frame f;
    const uint32_t a = f.alloc(24), b = f.alloc(24), next = f.alloc(24);
    call(fn::function_vb_copy, {a, callbacks});
    call(fn::function_vb_copy, {b, callbacks + (CB_B - CB_A)});
    st32(next + 16, 0);
    const uint32_t obj = call(fn::operator_new, {STEP_SIZE});
    if (obj) {
        st32(obj, nextVtable);
        call(fn::function_vb_move, {obj + CB_A, a});
        call(fn::function_vb_move, {obj + CB_B, b});
    }
    st32(next + 16, obj);
    request(api, next);
    functionDestroy(next);
    functionDestroy(b);
    functionDestroy(a);
}

// __clone(): both callbacks copied into a new block
uint32_t cloneStep(uint32_t self, uint32_t vtable) {
    const uint32_t obj = call(fn::operator_new, {STEP_SIZE});
    if (obj) {
        st32(obj, vtable);
        call(fn::function_vb_copy, {obj + CB_A, self + CB_A});
        call(fn::function_vb_copy, {obj + CB_B, self + CB_B});
    }
    return obj;
}

// $_60: Facebook friends (logged in) or straight on
constexpr uint32_t VT_59 = 0xF553A4;
constexpr uint32_t SceneUtil_isLoggedInFacebook = 0x4697E9;
constexpr uint32_t PrepareMypage_skipFriends = 0x48F1A5;              // sub_48F1A4(captures, bool): the next step's body
constexpr uint32_t FacebookManager_getInstance = 0x4E5585;
constexpr uint32_t FacebookManager_getFriends = 0x4E575B;             // (fb, limit, const std::function<...>&)
constexpr uint32_t Misc_create = 0x786D5D;                            // master::Misc::create(shared_ptr* out, id)
constexpr uint32_t MISC_FRIEND_LIMIT = 811;

// the libc++ std::function copy constructor as the compiler inlined it
void copyFunctionInline(uint32_t dst, uint32_t src) {
    const uint32_t p = ld32(src + 16);
    if (!p) {
        st32(dst + 16, 0);
    } else if (p == src) {
        st32(dst + 16, dst);
        vcall(p, 0xC, {p, dst});
    } else {
        st32(dst + 16, vcall(p, 0x8, {p}));
    }
}

constexpr uint32_t APIManager_executeUserGet = 0x4C6081;
constexpr uint32_t APIManager_executePartyMemberList = 0x4C7AA5;      // (api, bool, const function&, platform)
constexpr uint32_t FunctionVB_call = 0x474139;                        // std::function<void(bool)>::operator()(bool)
constexpr uint32_t API_REFRESH = 0x314;                               // APIManager: an object, byte +0x1C: refresh the user
constexpr uint32_t API_USER_INFO = 0x84;                              // shared_ptr (SafeBuffer: byte 0x10 in a party)
constexpr uint32_t API_PARTY_BUSY = 0x1FC;
constexpr uint32_t VT_userRefreshed = 0xF55378;
constexpr uint32_t PrepareMypage_partyMembers = 0x48F385;             // sub_48F384

void blockLoading() {
    const uint32_t mgr = call(fn::SceneManager_getInstance, {});
    Frame f;
    const uint32_t empty = newEmptyFunction(f);
    st8(mgr + SM_loadingBlocked, 1);
    call(FunctionB_assign, {mgr + SM_onFinishLoading, empty});
    functionDestroy(empty);
}
uint8_t safeByte(uint32_t buffer, uint32_t index) {
    const SafeBuffer *sb = safe(buffer);
    return uint8_t(ld8(sb->data.addr + index) ^ SafeBuffer::mask(sb->key, index));
}

// openNewLuxPopup: $_55 [?, popup] the reward LWF on the popup's Area_Point; $_54 [?, popup, panel, layer, ?,
// index, done] the close button -> {lambda()#1} [panel, layer, popup, index, done] (the words in that order)
constexpr uint32_t VT_luxClose = 0xF54744;
constexpr uint32_t VT_luxClosed = 0xF54770;
constexpr uint32_t LUX_DONE = 0x18, LUX_SIZE = 0x30;
constexpr uint32_t AudioPlaySystemSEDecide = 0x5C0CF9;
constexpr uint32_t SceneUtil_createClosePopupAnimation = 0x45BC61;   // (Node* (unused), std::function<void()>)
constexpr uint32_t SceneUtil_openNewLuxPopup = 0x46FDC5;             // (index, std::function<void()>)
constexpr uint32_t Node_runAction = 0x894AED;
constexpr uint32_t FadeTo_create = 0x86B3E1;                         // (float duration, uint8 opacity)
constexpr uint32_t LWFNode_create = 0x7EDF31;                        // (const char* path, void*)
constexpr uint32_t audio_Manager_getInstance = 0x5BECFD;
constexpr uint32_t audio_Manager_getLayer = 0x5BF33D;                // (shared_ptr<Layer>* out, mgr, layer)
constexpr uint32_t audio_Layer_playSE = 0x5BF785;                    // (layer, ResourceID, int, int)
constexpr uint32_t SE_LUX_REWARD = 0x1E584184;
constexpr uint32_t API_NEW_LUX = 0x308;                              // APIManager: std::vector<int> begin / end
constexpr uint32_t S_AREA_POINT = 0xDF9FB4;
constexpr uint32_t S_RESULT_REWARD_LWF = 0xDF9FBF;                   // "lwf/battleresult/Result_Reward/Result_Reward.lwf"

// the __clone() of the openNewLuxPopup lambdas: four words, then the captured std::function<void()>
uint32_t cloneLux(uint32_t self, uint32_t vtable) {
    const uint32_t obj = call(fn::operator_new, {LUX_SIZE});
    if (obj) {
        st32(obj, vtable);
        const uint32_t w0 = ld32(self + 8), w1 = ld32(self + 0xC), w2 = ld32(self + 0x10), w3 = ld32(self + 0x14);
        st32(obj + 8, w0);
        st32(obj + 0xC, w1);
        st32(obj + 0x10, w2);
        st32(obj + 0x14, w3);
        call(fn::function_vv_copy, {obj + LUX_DONE, self + LUX_DONE});
    }
    return obj;
}

}  // namespace

struct SceneUtilBatch3 {
    // sub_471200(callbacks, int ok): $_66's body - from the Lux rank of TutorialMisc 8 on the sphere lock check
    // first ($_65 next), else straight to the party notice
    static void prepareSphereCheck(uint32_t callbacks, uint32_t ok) {
        if (ok != 1) return;
        Frame f;
        const uint32_t misc = f.alloc(8);
        call(TutorialMisc_create, {misc, 8});
        if (!ld32(misc) ||
            safe32(ld32(call(fn::APIManager_getInstance, {}) + API_USER_RANK), 0x40) < safe32(ld32(misc), 4)) {
            call(PrepareMypage_partyNotice, {callbacks, 1});
        } else {
            chainStepFrom(callbacks, VT_65, [](uint32_t api, uint32_t next) {
                call(APIManager_executeUserSphereCheck, {api, 1, next});
            });
        }
        sharedRelease(misc);
    }
    // sub_48D9E0(callbacks, int ok): $_65's body - the party notice ($_64 next)
    static void preparePartyNotice(uint32_t callbacks, uint32_t ok) {
        if (ok != 1) return;
        chainStepFrom(callbacks, VT_64, [](uint32_t api, uint32_t next) {
            call(APIManager_executePartyNoticeUpdate, {api, next});
        });
    }

    // sub_4744F4(text, on, light): white with the standard shadow when on, else grey (lighter when `light` is
    // not 1) with a dark shadow
    static void setTextLook(uint32_t text, uint32_t on, uint32_t light) {
        const uint32_t setColor = ld32(ld32(text) + V_setTextColor);
        if (on == 1) {
            call(setColor, {text, ld32(GOT_COLOR_WHITE)});
            call(SceneUtil_setShadow, {text});
            return;
        }
        Frame f;
        const uint32_t color = f.alloc(4), shadow = f.alloc(4), size = f.alloc(8);
        const uint32_t grey = light == 1 ? 0x97 : 0xA7;
        call(Color3B_ctor, {color, grey, grey, grey});
        call(setColor, {text, color});
        call(Color4B_ctor, {shadow, 0x47, 0x47, 0x47, 0x80});
        call(Size_ctor, {size, fbits(2.0f), fbits(-2.0f)});
        call(Text_enableShadow, {text, shadow, size, 6});
    }

    // openNewLuxPopup $_55: the reward sound and the reward LWF over the popup's Area_Point
    static void luxPopupShown(uint32_t functor) {
        {
            Frame f;
            const uint32_t layer = f.alloc(8);
            call(audio_Manager_getLayer, {layer, call(audio_Manager_getInstance, {}), 0});
            call(audio_Layer_playSE, {ld32(layer), SE_LUX_REWARD, 0, 0});
            if (ld32(layer + 4)) call(fn::shared_release, {ld32(layer + 4)});
        }
        const uint32_t point = findNode(ld32(functor + 4), S_AREA_POINT, 10);
        const uint32_t lwf = call(LWFNode_create, {S_RESULT_REWARD_LWF, 0});
        const uint32_t parent = vcall(point, node::getParent, {point});
        vcall(parent, node::addChild, {parent, lwf});
        const uint32_t setPosition = ld32(ld32(lwf) + node::setPosition);
        call(setPosition, {lwf, vcall(point, node::getPosition, {point})});
    }

    // openNewLuxPopup $_54: the close button - shrink the popup and fade the layer; the next popup when closed
    static void luxCloseTouched(uint32_t functor, uint32_t /*sender*/, uint32_t typePtr) {
        if (ld32(typePtr) != 2) return;                      // TouchEventType::ENDED
        call(AudioPlaySystemSEDecide, {});
        const uint32_t popup = ld32(functor + 8);
        const uint32_t panel = ld32(functor + 0xC), layer = ld32(functor + 0x10), index = ld32(functor + 0x14);
        Frame f;
        const uint32_t done = f.alloc(24);
        call(fn::function_vv_copy, {done, functor + LUX_DONE});
        const uint32_t closed = f.alloc(24);
        st32(closed + 16, 0);
        const uint32_t obj = call(fn::operator_new, {LUX_SIZE});
        uint32_t r0 = obj;                                    // the Node* argument is a register leftover (unused)
        if (obj) {
            st32(obj, VT_luxClosed);
            st32(obj + 8, panel);
            st32(obj + 0xC, layer);
            st32(obj + 0x10, popup);
            st32(obj + 0x14, index);
            r0 = call(fn::function_vv_move, {obj + LUX_DONE, done});
        }
        st32(closed + 16, obj);
        call(Node_runAction, {popup, call(SceneUtil_createClosePopupAnimation, {r0, closed})});
        functionDestroy(closed);
        functionDestroy(done);
        call(Node_runAction, {layer, call(FadeTo_create, {fbits(0.1f), 0})});
    }

    // ... closed: remove the popup and the layer, then the next new-Lux popup (or `done` after the last)
    static void luxPopupClosed(uint32_t functor) {
        const uint32_t panel = ld32(functor + 8);
        vcall(panel, node::removeChild, {panel, ld32(functor + 0xC), 1});
        vcall(ld32(functor + 8), node::removeChild, {ld32(functor + 8), ld32(functor + 0x10), 1});
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const int32_t count = int32_t(ld32(api + API_NEW_LUX + 4) - ld32(api + API_NEW_LUX)) >> 2;
        const int32_t next = int32_t(ld32(functor + 0x14)) + 1;
        if (count > next) {
            Frame f;
            const uint32_t done = f.alloc(24);
            call(fn::function_vv_copy, {done, functor + LUX_DONE});
            call(SceneUtil_openNewLuxPopup, {uint32_t(next), done});
            functionDestroy(done);
        } else if (ld32(functor + LUX_DONE + 16)) {
            call(fn::function_vv_call, {functor + LUX_DONE});
        }
    }
    static uint32_t luxCloseClone(uint32_t self) { return cloneLux(self, VT_luxClose); }
    static uint32_t luxClosedClone(uint32_t self) { return cloneLux(self, VT_luxClosed); }
    static void luxDelete(uint32_t self) {
        functionDestroy(self + LUX_DONE);
        call(fn::operator_delete, {self});
    }

    static void prepareCampaign(uint32_t functor, uint32_t okPtr) {         // $_64
        chainStep(functor, okPtr, VT_63, [](uint32_t api, uint32_t next) {
            call(APIManager_executeCampaignGet, {api, next});
        });
    }
    static void prepareRaidList(uint32_t functor, uint32_t okPtr) {         // $_63
        chainStep(functor, okPtr, VT_62, [](uint32_t api, uint32_t next) {
            call(APIManager_executeRaidList, {api, next});
        });
    }
    static void prepareParty(uint32_t functor, uint32_t okPtr) {            // $_62
        chainStep(functor, okPtr, VT_61, [](uint32_t api, uint32_t next) {
            call(APIManager_executePartyGet, {api, 0, 0, 0, next});        // user 0: mine
        });
    }
    static void prepareMypageInfo(uint32_t functor, uint32_t okPtr) {       // $_61
        chainStep(functor, okPtr, VT_60, [](uint32_t api, uint32_t next) {
            call(APIManager_executeMypageInfoGet, {api, next});
        });
    }
    static void prepareFriends(uint32_t functor, uint32_t okPtr) {         // $_60
        if (!ld8(okPtr)) return;
        if (call(SceneUtil_isLoggedInFacebook, {}) != 1) {
            call(PrepareMypage_skipFriends, {functor + CB_A, 1});
            return;
        }
        Frame f;
        const uint32_t a = f.alloc(24), b = f.alloc(24);
        call(fn::function_vb_copy, {a, functor + CB_A});
        call(fn::function_vb_copy, {b, functor + CB_B});
        {
            const uint32_t mgr = call(fn::SceneManager_getInstance, {});
            Frame g;
            const uint32_t empty = newEmptyFunction(g);
            st8(mgr + SM_loadingBlocked, 1);
            call(FunctionB_assign, {mgr + SM_onFinishLoading, empty});
            functionDestroy(empty);
        }
        const uint32_t fb = call(FacebookManager_getInstance, {});
        const uint32_t misc = f.alloc(8);
        call(Misc_create, {misc, MISC_FRIEND_LIMIT});
        const int32_t limit = safe(ld32(misc))->get32(4);
        const uint32_t c = f.alloc(24), d = f.alloc(24), next = f.alloc(24);
        copyFunctionInline(c, a);
        copyFunctionInline(d, b);
        st32(next + 16, 0);
        const uint32_t obj = call(fn::operator_new, {STEP_SIZE});
        if (obj) {
            st32(obj, VT_59);
            call(fn::function_vb_move, {obj + CB_A, c});
            call(fn::function_vb_move, {obj + CB_B, d});
        }
        st32(next + 16, obj);
        call(FacebookManager_getFriends, {fb, uint32_t(limit), next});
        functionDestroy(next);
        functionDestroy(d);
        functionDestroy(c);
        if (ld32(misc + 4)) call(fn::shared_release, {ld32(misc + 4)});
        functionDestroy(b);
        functionDestroy(a);
    }
    // sub_48F1A4(captures [callback A, callback B at +0x18], int ok): after the friends - refresh the user when
    // asked to, else the party members (in a party) or straight on to B
    static void prepareAfterFriends(uint32_t captures, uint32_t ok) {
        if (ok != 1) return;
        if (ld8(ld32(call(fn::APIManager_getInstance, {}) + API_REFRESH) + 0x1C)) {
            blockLoading();
            const uint32_t api = call(fn::APIManager_getInstance, {});
            Frame f;
            const uint32_t a = f.alloc(24), next = f.alloc(24);
            call(fn::function_vb_copy, {a, captures});
            st32(next + 16, 0);
            const uint32_t obj = call(fn::operator_new, {0x20});
            if (obj) {
                st32(obj, VT_userRefreshed);
                call(fn::function_vb_move, {obj + 8, a});
            }
            st32(next + 16, obj);
            call(APIManager_executeUserGet, {api, next});
            functionDestroy(next);
            functionDestroy(a);
            return;
        }
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t ctrl = ld32(api + API_USER_INFO + 4), info = ld32(api + API_USER_INFO);
        if (ctrl) call(fn::shared_add, {ctrl});
        if (info && safeByte(info, 0x10)) {
            call(PrepareMypage_partyMembers, {captures, 1});
        } else if (ld32(captures + 0x18 + 16)) {
            call(FunctionVB_call, {captures + 0x18, 1});
        }
        if (ctrl) call(fn::shared_release, {ctrl});
    }

    // sub_48F384(callback, int ok): in a party (and not busy) the party members, else the callback with true
    static void preparePartyMembers(uint32_t callback, uint32_t ok) {
        if (ok != 1) return;
        const uint32_t api = call(fn::APIManager_getInstance, {});
        const uint32_t ctrl = ld32(api + API_USER_INFO + 4), info = ld32(api + API_USER_INFO);
        if (ctrl) call(fn::shared_add, {ctrl});
        const uint32_t api2 = call(fn::APIManager_getInstance, {});
        if (info && !ld32(api2 + API_PARTY_BUSY) && safeByte(info, 0x10)) {
            blockLoading();
            call(APIManager_executePartyMemberList, {call(fn::APIManager_getInstance, {}), 1, callback, 1});
        } else if (ld32(callback + 16)) {
            call(FunctionVB_call, {callback, 1});
        }
        if (ctrl) call(fn::shared_release, {ctrl});
    }

    static uint32_t clone66(uint32_t self) { return cloneStep(self, VT_66); }
    static uint32_t clone65(uint32_t self) { return cloneStep(self, VT_65); }
    static uint32_t clone63(uint32_t self) { return cloneStep(self, VT_63); }
    static uint32_t clone62(uint32_t self) { return cloneStep(self, VT_62); }
    static uint32_t clone61(uint32_t self) { return cloneStep(self, VT_61); }
    static uint32_t clone60(uint32_t self) { return cloneStep(self, VT_60); }
    // destroy_deallocate(): the callbacks (the second first), then the block
    static void deleteStep(uint32_t self) {
        functionDestroy(self + CB_B);
        functionDestroy(self + CB_A);
        call(fn::operator_delete, {self});
    }
};

using L = SceneUtilBatch3;
RW_FUNCTION(0x48DDD0, "SceneUtil::callPrepareMypageAPIs::$_64::operator()", L::prepareCampaign);
RW_FUNCTION(0x48E1F8, "SceneUtil::callPrepareMypageAPIs::$_63::operator()", L::prepareRaidList);
RW_FUNCTION(0x48E620, "SceneUtil::callPrepareMypageAPIs::$_62::operator()", L::prepareParty);
RW_FUNCTION(0x48EA54, "SceneUtil::callPrepareMypageAPIs::$_61::operator()", L::prepareMypageInfo);
RW_FUNCTION(0x484298, "SceneUtil::callPrepareMypageAPIs::$_66 __clone()", L::clone66);
RW_FUNCTION(0x48FF3C, "SceneUtil::callPrepareMypageAPIs::$_65 __clone()", L::clone65);
RW_FUNCTION(0x48E06C, "SceneUtil::callPrepareMypageAPIs::$_63 __clone()", L::clone63);
RW_FUNCTION(0x48E494, "SceneUtil::callPrepareMypageAPIs::$_62 __clone()", L::clone62);
RW_FUNCTION(0x48E8C8, "SceneUtil::callPrepareMypageAPIs::$_61 __clone()", L::clone61);
RW_FUNCTION(0x48ECF0, "SceneUtil::callPrepareMypageAPIs::$_60 __clone()", L::clone60);
RW_FUNCTION(0x4843DC, "SceneUtil::callPrepareMypageAPIs::$_66 destroy_deallocate()", L::deleteStep);
RW_FUNCTION(0x490080, "SceneUtil::callPrepareMypageAPIs::$_65 destroy_deallocate()", L::deleteStep);
RW_FUNCTION(0x48DD88, "SceneUtil::callPrepareMypageAPIs::$_64 destroy_deallocate()", L::deleteStep);
RW_FUNCTION(0x48E1B0, "SceneUtil::callPrepareMypageAPIs::$_63 destroy_deallocate()", L::deleteStep);
RW_FUNCTION(0x48E5D8, "SceneUtil::callPrepareMypageAPIs::$_62 destroy_deallocate()", L::deleteStep);
RW_FUNCTION(0x48EA0C, "SceneUtil::callPrepareMypageAPIs::$_61 destroy_deallocate()", L::deleteStep);
RW_FUNCTION(0x48EE34, "SceneUtil::callPrepareMypageAPIs::$_60 destroy_deallocate()", L::deleteStep);
RW_FUNCTION(0x484F4C, "SceneUtil::openNewLuxPopup::$_55::operator()", L::luxPopupShown);
RW_FUNCTION(0x485214, "SceneUtil::openNewLuxPopup::$_54::operator()", L::luxCloseTouched);
RW_FUNCTION(0x485500, "SceneUtil::openNewLuxPopup::$_54::{lambda()#1}::operator()", L::luxPopupClosed);
RW_FUNCTION(0x48512C, "SceneUtil::openNewLuxPopup::$_54 __clone()", L::luxCloseClone);
RW_FUNCTION(0x485418, "SceneUtil::openNewLuxPopup::$_54::{lambda()#1} __clone()", L::luxClosedClone);
RW_FUNCTION(0x4851E8, "SceneUtil::openNewLuxPopup::$_54 destroy_deallocate()", L::luxDelete);
RW_FUNCTION(0x4854D4, "SceneUtil::openNewLuxPopup::$_54::{lambda()#1} destroy_deallocate()", L::luxDelete);
RW_FUNCTION(0x48EE7C, "SceneUtil::callPrepareMypageAPIs::$_60::operator()", L::prepareFriends);
RW_FUNCTION(0x48F1A4, "SceneUtil::callPrepareMypageAPIs (sub_48F1A4): after the friends", L::prepareAfterFriends);
RW_FUNCTION(0x48F384, "SceneUtil::callPrepareMypageAPIs (sub_48F384): the party members", L::preparePartyMembers);
RW_FUNCTION(0x471200, "SceneUtil::callPrepareMypageAPIs (sub_471200): the sphere lock check", L::prepareSphereCheck);
RW_FUNCTION(0x48D9E0, "SceneUtil::callPrepareMypageAPIs (sub_48D9E0): the party notice", L::preparePartyNotice);
RW_FUNCTION(0x4744F4, "SceneUtil (sub_4744F4): a text's on / off look", L::setTextLook);
