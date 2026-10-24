// Free functions of the game and its cocos2d-x Android glue: app init, JNI entry points, audio helpers, the story
// movie scene selection, and a few non-virtual thunks.
#include "B2Common.hpp"

using namespace b2;

namespace freegame {

constexpr uint32_t SHARED_RELEASE = 0xC8DBD1;    // __shared_weak_count::__release_shared
constexpr uint32_t DIRECTOR_INSTANCE = rwlib::cocos2d::Director::getInstance; // cocos2d::Director::getInstance
constexpr uint32_t STRING_INIT = 0x47CD93;       // std::string::__init(const char*, size_t)
constexpr uint32_t STRLEN = 0x40E08C;
constexpr uint32_t OPERATOR_DELETE = 0xC98F9B;
constexpr uint32_t JNI_STATIC_METHOD = rwlib::cocos2d::JniHelper::getStaticMethodInfo; // JniHelper::getStaticMethodInfo(info, class, method, sig)
constexpr uint32_t CALL_STATIC_VOID = 0x4E0D09;  // _JNIEnv::CallStaticVoidMethod(env, cls, mid, ...)
constexpr uint32_t AUDIO_MANAGER = 0x5BECFD;     // audio::Manager::getInstance
constexpr uint32_t AUDIO_GET_LAYER = 0x5BF33D;   // audio::Manager::getLayer(shared_ptr out, mgr, layer)
constexpr uint32_t AUDIO_PLAY_SE = 0x5BF785;     // audio::Layer::playSE(layer, id, a, b)
constexpr uint32_t API_MANAGER = 0x4BA8C9;       // APIManager::getInstance
constexpr uint32_t SAFEBUFFER_READ = 0x45A269;   // SafeBuffer::read(sb, dst, src, n, index)

// env->functions[slot](env, ...)
static uint32_t jni(uint32_t env, uint32_t byteOffset, std::initializer_list<uint32_t> args) {
    return rw::call(ld32(ld32(env) + byteOffset), args);
}

struct Free {
    static uint32_t appInit() {
        uint32_t app = rw::call(0xC98EF1, {4});      // operator new(4)
        uint32_t r = rw::call(0x79A211, {app});      // AppDelegate::AppDelegate
        uint32_t vm = rw::call(rwlib::cocos2d::JniHelper::getJavaVM, {r});       // JniHelper::getJavaVM
        return rw::call(rwlib::cocos2d::PluginJniHelper::setJavaVM, {vm});             // PluginJniHelper::setJavaVM
    }

    // std::vector<std::string> createHeaders(bool withToken) (sret; r0 = 0)
    static uint32_t createHeaders(uint32_t out, uint32_t withToken) {
        guest::Frame f(0x424);
        st32(out, 0);
        st32(out + 4, 0);
        st32(out + 8, 0);
        if (withToken != 1) return 0;
        uint32_t user = ld32(rw::call(API_MANAGER, {}) + 0x7C);
        uint32_t token = (ld8(user + 0x14) & 1) ? ld32(user + 0x1C) : user + 0x15;
        const uint32_t buf = f.addr(0x1C), tmp = f.addr(0x10);
        rw::call(0x49140D, {buf, 0xDFC2CE, token});  // StringBuffer("X-HTTP-USER-TOKEN: %s", token)
        rw::call(STRING_INIT, {tmp, buf, rw::call(STRLEN, {buf})});
        uint32_t end = ld32(out + 4);
        if (end < ld32(out + 8)) {
            uint32_t e = 0;
            if (end != 0) {
                for (uint32_t k = 0; k < 12; k += 4) st32(end + k, *f.at<uint32_t>(0x10 + k));
                for (uint32_t k = 0; k < 12; k += 4) *f.at<uint32_t>(0x10 + k) = 0;
                e = ld32(out + 4);
            }
            st32(out + 4, e + 12);
        } else {
            rw::call(0x484465, {out, tmp});  // vector<string>::__push_back_slow_path
        }
        if (*f.at<uint8_t>(0x10) & 1) rw::call(OPERATOR_DELETE, {*f.at<uint32_t>(0x18)});
        return 0;
    }

    // non-virtual thunks into StageState (IStatusProtocol bases at +36 / +32): 12-byte records read through the
    // SafeBuffer that holds them
    static uint32_t stageGetBuff(uint32_t out, uint32_t thisAdj, int32_t index) {
        st32(out, 0);
        st32(out + 4, 0);
        st32(out + 8, 0);
        uint32_t data = ld32(thisAdj - 0x18);
        uint32_t off = uint32_t(index) * 12u + 0x426C;
        return rw::call(SAFEBUFFER_READ, {thisAdj - 0x24, out, data + off, 0xC, off});
    }
    static uint32_t stageGetCondition(uint32_t out, uint32_t thisAdj, int32_t type) {
        st32(out, 0);
        st32(out + 4, 0);
        st32(out + 8, 0);
        uint32_t data = ld32(thisAdj - 0x14);
        uint32_t off = uint32_t(type) * 12u + 0x458C;
        return rw::call(SAFEBUFFER_READ, {thisAdj - 0x20, out, data + off, 0xC, off});
    }
    static uint32_t textHandler(uint32_t thisAdj, uint32_t, uint32_t text) {
        uint32_t element = rw::call(0x4F9C81, {thisAdj - 0x270, text});  // CustomTextView::createCustomElement
        return rw::call(rwlib::cocos2d::ui::RichText::pushBackElement, {ld32(thisAdj + 0x28), element});     // RichText::pushBackElement
    }

    // a system SE on audio layer 10000
    static uint32_t playSystemSE(uint32_t id) {
        guest::Frame f(0x10);
        uint32_t mgr = rw::call(AUDIO_MANAGER, {});
        rw::call(AUDIO_GET_LAYER, {f.addr(4), mgr, 10000});
        uint32_t r = rw::call(AUDIO_PLAY_SE, {*f.at<uint32_t>(4), id, 0, 0});
        if (*f.at<uint32_t>(8) != 0) rw::call(SHARED_RELEASE, {*f.at<uint32_t>(8)});
        return r;
    }
    static uint32_t playSystemSEDecide() { return playSystemSE(0x1E583FEA); }
    static uint32_t playSystemSEPopUp() { return playSystemSE(0x1E583FFE); }

    // unlocks and stops every BGM of the game-main stack (a std::deque<pair<ResourceID, int>> at 0x1003C64),
    // then clears it
    static uint32_t clearGameMainBGMStack() {
        const uint32_t D = 0x1003C30;
        uint32_t start = ld32(D + 0x44), mapBegin = ld32(D + 0x38), mapEnd = ld32(D + 0x3C);
        uint32_t block = mapBegin + ((start >> 7) & 0x1FFFFFC);
        uint32_t end = 0, p = 0;
        if (mapEnd != mapBegin) {
            uint32_t e = start + ld32(D + 0x48);
            end = ld32(mapBegin + ((e >> 7) & 0x1FFFFFC)) + (e & 0x1FF) * 8;
            p = ld32(block) + (start & 0x1FF) * 8;
        }
        uint32_t left = end - p;
        while (left != 0) {
            uint32_t id = ld32(p);
            rw::call(0x5BF1ED, {rw::call(AUDIO_MANAGER, {}), id});              // Manager::unlock
            rw::call(0x5BF3AD, {rw::call(AUDIO_MANAGER, {}), ld32(p + 4), 0});  // Manager::stop
            p += 8;
            if (p - ld32(block) == 0x1000) {
                block += 4;
                p = ld32(block);
                left = end - p;
            } else {
                left -= 8;
            }
        }
        return rw::call(0x5C1F05, {D + 0x34});  // deque::clear
    }

    static uint32_t playGameMainSEOneShot(uint32_t id) {
        guest::Frame f(0x1C);
        uint32_t mgr = rw::call(AUDIO_MANAGER, {});
        rw::call(AUDIO_GET_LAYER, {f.addr(0x10), mgr, 0});
        uint32_t r = rw::call(AUDIO_PLAY_SE, {*f.at<uint32_t>(0x10), id, 0, 0});
        if (*f.at<uint32_t>(0x14) != 0) rw::call(SHARED_RELEASE, {*f.at<uint32_t>(0x14)});
        rw::call(0x5BF18D, {rw::call(AUDIO_MANAGER, {}), id});  // Manager::lock
        *f.at<uint32_t>(8) = r;
        *f.at<uint32_t>(0xC) = id;
        rw::call(0x5C1B31, {f.addr(0x10), 0x1003C50, f.addr(8)});  // unordered_map<int,ResourceID>::insert
        return r;
    }

    // The scene that follows a story movie: the stage's scenario (light novel, flash drama or raid) or the map.
    static uint32_t startMovie(int32_t index, uint32_t afterClear) {
        guest::Frame f(0xC4);
        uint32_t ctrl = 0;
        uint32_t mode = ld32(rw::call(0x60C26D, {}) + 0x30C);  // SceneManager::getInstance()->mode
        uint32_t stage, flagged = 0;
        auto createStage = [&](uint32_t questOff, uint32_t slot) -> bool {
            uint32_t quest = ld32(rw::call(API_MANAGER, {}) + questOff);
            if (quest == 0) return false;
            rw::call(0x78E4E9, {f.addr(slot), ld32(quest + 0x30)});  // master::Stage::create(id) -> shared_ptr
            stage = *f.at<uint32_t>(slot);
            ctrl = *f.at<uint32_t>(slot + 4);
            *f.at<uint32_t>(slot) = 0;
            *f.at<uint32_t>(slot + 4) = 0;
            if (questOff == 0x13C && ld8(quest + 0x48) == 1) flagged = 1;
            return true;
        };
        if (mode == 0) {
            if (!createStage(0x13C, 0x3C)) return 0;
        } else if (mode == 2) {
            if (!createStage(0x260, 0x2C)) return 0;
        } else if (mode == 3) {
            if (!createStage(0x178, 0x34)) return 0;
        } else {
            return 0;
        }
        uint32_t layer = 0;
        if (stage != 0) {
            auto read = [&](uint32_t dst, uint32_t off, uint32_t n) {
                rw::call(SAFEBUFFER_READ, {stage, dst, ld32(stage + 0xC) + off, n, off});
            };
            auto myPageOrSelect = [&]() {
                uint32_t v = ld32(rw::call(API_MANAGER, {}) + 0x124);
                uint32_t n = uint32_t(int32_t(ld32(v + 4) - ld32(v)) >> 3) * 0xAAAAAAABu;
                if (n > 2) {
                    uint32_t l = rw::call(0x579EB9, {});  // SceneAdventureSelect::create
                    st8(l + 0x29F, 1);
                    return l;
                }
                return rw::call(0x4857F9, {});  // SceneMyPage::create
            };
            int32_t scenario = 0;
            uint32_t flags;
            bool toMap = false, toMyPage = false;
            if (afterClear == 1) {
                read(f.addr(0x48), 0x150, 4);  // after-clear scenario count
                if (*f.at<int32_t>(0x48) <= index) toMyPage = true;
                else {
                    read(f.addr(0xA8), 0x154, 0x14);
                    scenario = *f.at<int32_t>(0xA8 + uint32_t(index) * 4);
                    read(f.addr(0x94), 0x168, 0x14);
                    flags = f.addr(0x94);
                }
            } else {
                read(f.addr(0x4C), 0x124, 4);
                if (*f.at<int32_t>(0x4C) <= index) toMap = true;
                else {
                    read(f.addr(0x80), 0x128, 0x14);
                    scenario = *f.at<int32_t>(0x80 + uint32_t(index) * 4);
                    read(f.addr(0x6C), 0x13C, 0x14);
                    flags = f.addr(0x6C);
                }
            }
            if (!toMap && !toMyPage) {
                int32_t offset = 0;
                if (ld32(flags + uint32_t(index) * 4) == 1) {
                    uint32_t user = ld32(rw::call(API_MANAGER, {}) + 0x94);
                    rw::call(SAFEBUFFER_READ, {user, f.addr(0x44), ld32(user + 0xC) + 0x30, 4, 0x30});
                    offset = *f.at<int32_t>(0x44) - 1;
                }
                if ((flagged | (scenario == 0 ? 1u : 0u)) != 0) {
                    if (afterClear == 1) toMyPage = true;
                    else toMap = true;
                } else if (scenario <= 999999) {
                    layer = rw::call(0x57A425, {});  // SceneRaidBattle::create
                    rw::call(0x608C6D, {layer, 1, 1, 0, 1, uint32_t(index), afterClear});  // setRaidStage
                } else if (scenario > 1999999) {
                    layer = rw::call(0x6F341D, {1});  // SceneFlashDrama::create(true)
                    rw::call(0x6F3C2D, {layer, uint32_t(offset + scenario), uint32_t(index), afterClear});
                } else {
                    uint32_t v = ld32(rw::call(API_MANAGER, {}) + 0x124);
                    if (v != 0 && uint32_t(int32_t(ld32(v + 4) - ld32(v)) >> 3) * 0xAAAAAAABu <= 2) {
                        // SceneUtil::callPrepareMypageAPIs(std::function<void(bool)>()) with an empty callback
                        *f.at<uint32_t>(0x60) = 0;
                        rw::call(0x46F37D, {f.addr(0x50)});
                        uint32_t fn = *f.at<uint32_t>(0x60);
                        if (fn == f.addr(0x50)) vcallo(fn, 0x10, {fn});
                        else if (fn != 0) vcallo(fn, 0x14, {fn});
                    }
                    layer = rw::call(0x6C11ED, {});  // SceneLight::create
                    st32(layer + 0x29EC, uint32_t(index));
                    st8(layer + 0x29F0, uint8_t(afterClear));
                    rw::call(0x6C10B9, {layer, uint32_t(offset + scenario)});  // SceneLight::load
                    st32(layer + 0x2B0, ld32(ld32(layer + 0x2B4) + 4));
                    st32(layer + 0x2AC, 0);
                    rw::call(0x6C2309, {layer});  // SceneLight::exec_commandCheckAvater
                }
            }
            if (toMyPage) layer = myPageOrSelect();
            else if (toMap) layer = rw::call(0x5FB8AD, {});  // SceneActionMap::createLayer
        }
        if (ctrl != 0) rw::call(SHARED_RELEASE, {ctrl});
        return layer;
    }

    // ------------------------------------------------------------------------------------------ JNI
    static uint32_t webViewDidFinishLoading(uint32_t env, uint32_t, uint32_t index, uint32_t jurl) {
        guest::Frame f(0x1C);
        uint32_t chars = jni(env, 0x2A4, {env, jurl, 0});  // GetStringUTFChars
        rw::call(STRING_INIT, {f.addr(0xC), chars, rw::call(STRLEN, {chars})});
        jni(env, 0x2A8, {env, jurl, chars});              // ReleaseStringUTFChars
        rw::call(rwlib::cocos2d::plugin::WebViewImpl::didFinishLoading, {index, f.addr(0xC)});          // WebViewImpl::didFinishLoading
        if (*f.at<uint8_t>(0xC) & 1) rw::call(OPERATOR_DELETE, {*f.at<uint32_t>(0x14)});
        return 0;
    }
    static uint32_t videoExecuteCallback(uint32_t, uint32_t, uint32_t index, uint32_t event) {
        guest::Frame f(0xC);
        *f.at<uint32_t>(4) = index;
        if (rw::call(0x844551, {0x10064E0, f.addr(4)}) != 0) {  // players.find(index)
            uint32_t player = ld32(rw::call(0x843BC5, {0x10064E0, f.addr(4)}));  // players[index]
            vcallo(player, 0x35C, {player, event});  // VideoPlayer::onPlayEvent
        }
        return 0;
    }
    static uint32_t callVideoNonParameterFun(uint32_t index, uint32_t method) {
        guest::Frame f(0x18);
        if (rw::call(JNI_STATIC_METHOD, {f.addr(8), 0xE0C120, method, 0xDFC3EF}) == 1) {  // Cocos2dxVideoHelper, "(I)V"
            uint32_t env = *f.at<uint32_t>(8), cls = *f.at<uint32_t>(0xC), mid = *f.at<uint32_t>(0x10);
            rw::call(CALL_STATIC_VOID, {env, cls, mid, index});
            jni(*f.at<uint32_t>(8), 0x5C, {*f.at<uint32_t>(8), *f.at<uint32_t>(0xC)});  // DeleteLocalRef(cls)
        }
        return 0;
    }
    static uint32_t setVideoRectJNI(uint32_t index, uint32_t x, uint32_t y, uint32_t w, uint32_t h) {
        guest::Frame f(0x34);
        if (rw::call(JNI_STATIC_METHOD, {f.addr(0x24), 0xE0C120, 0xE0C169, 0xE0B251}) == 1) {  // setVideoRect
            uint32_t env = *f.at<uint32_t>(0x24), cls = *f.at<uint32_t>(0x28), mid = *f.at<uint32_t>(0x2C);
            rw::call(CALL_STATIC_VOID, {env, cls, mid, index, x, y, w, h});
            jni(*f.at<uint32_t>(0x24), 0x5C, {*f.at<uint32_t>(0x24), *f.at<uint32_t>(0x28)});
        }
        return 0;
    }
    static int32_t rendererInit(uint32_t env, uint32_t thiz, int32_t w, int32_t h) {
        guest::Frame f(0x2C);
        uint32_t director = rw::call(DIRECTOR_INSTANCE, {});
        if (ld32(director + 0x9C) != 0) {  // a GL view exists: the context was lost
            rw::call(rwlib::cocos2d::Director::end, {director});  // Director::end
            return -1;
        }
        rw::call(STRING_INIT, {f.addr(0x1C), 0xE132CB, 11});  // "Android app"
        uint32_t view = rw::call(rwlib::cocos2d::GLView::create, {f.addr(0x1C)});   // GLView::create
        if (*f.at<uint8_t>(0x1C) & 1) rw::call(OPERATOR_DELETE, {*f.at<uint32_t>(0x24)});
        uint32_t setFrameSize = ld32(ld32(view) + 0x20);
        uint32_t fw = rw::call(0xCD6D68, {uint32_t(w)});  // __floatsisf
        uint32_t fh = rw::call(0xCD6D68, {uint32_t(h)});
        rw::call(setFrameSize, {view, fw, fh});
        rw::call(rwlib::cocos2d::Director::setOpenGLView, {director, view});  // Director::setOpenGLView
        rw::call(0x459E31, {env, thiz});       // cocos_android_app_init
        rw::call(rwlib::cocos2d::Application::run, {rw::call(rwlib::cocos2d::Application::getInstance, {})});  // Application::getInstance()->run()
        return 0;
    }
    static uint32_t setApkPath(uint32_t, uint32_t, uint32_t jpath) {
        guest::Frame f(0x18);
        rw::call(rwlib::cocos2d::JniHelper::jstring2string, {f.addr(8), jpath});  // JniHelper::jstring2string
        uint32_t g = got(0xFD88B4);             // &g_apkPath
        if (ld8(g) & 1) {
            st8(ld32(g + 8), 0);
            st32(g + 4, 0);
        } else {
            st8(g + 1, 0);
            st8(g, 0);
        }
        rw::call(0x48D705, {g, 0});  // std::string::reserve(0)
        for (uint32_t k = 0; k < 12; k += 4) st32(g + k, *f.at<uint32_t>(8 + k));
        return 0;
    }
    static uint32_t setContext(uint32_t env, uint32_t, uint32_t context, uint32_t assetManager) {
        rw::call(rwlib::cocos2d::JniHelper::setClassLoaderFrom, {context});  // JniHelper::setClassLoaderFrom
        rw::call(rwlib::cocos2d::JniHelper::setApplicationInfoFrom, {context});  // JniHelper::setApplicationInfoFrom
        uint32_t am = rw::call(0x444A04, {env, assetManager});  // AAssetManager_fromJava
        return rw::call(rwlib::cocos2d::FileUtilsAndroid::setassetmanager, {am});  // FileUtilsAndroid::setassetmanager
    }
    static uint32_t setStringForKey(uint32_t key, uint32_t value) {
        guest::Frame f(0x1C);
        if (rw::call(JNI_STATIC_METHOD, {f.addr(0xC), 0xE10FFB, 0xE13613, 0xDFC5B0}) == 1) {
            uint32_t env = *f.at<uint32_t>(0xC);
            uint32_t jkey = jni(env, 0x29C, {env, key});  // NewStringUTF
            env = *f.at<uint32_t>(0xC);
            uint32_t jvalue = jni(env, 0x29C, {env, value});
            rw::call(CALL_STATIC_VOID, {*f.at<uint32_t>(0xC), *f.at<uint32_t>(0x10), *f.at<uint32_t>(0x14), jkey,
                                        jvalue});
            env = *f.at<uint32_t>(0xC);
            jni(env, 0x5C, {env, *f.at<uint32_t>(0x10)});  // DeleteLocalRef
            env = *f.at<uint32_t>(0xC);
            jni(env, 0x5C, {env, jkey});
            env = *f.at<uint32_t>(0xC);
            jni(env, 0x5C, {env, jvalue});
        }
        return 0;
    }
    static uint32_t render() {
        uint32_t d = rw::call(DIRECTOR_INSTANCE, {});
        return vcallo(d, 0x18, {d});  // Director::mainLoop
    }
    // one touch: GLView::handleTouchesBegin / End (1, &id, &x, &y)
    static uint32_t touch(uint32_t slot, uint32_t id, uint32_t x, uint32_t y) {
        guest::Frame f(0x18);
        *f.at<uint32_t>(0x10) = id;
        *f.at<uint32_t>(0xC) = x;
        *f.at<uint32_t>(8) = y;
        uint32_t view = ld32(rw::call(DIRECTOR_INSTANCE, {}) + 0x9C);
        vcallo(view, slot, {view, 1, f.addr(0x10), f.addr(0xC), f.addr(8)});
        return 0;
    }
    static uint32_t touchesBegin(uint32_t, uint32_t, uint32_t id, uint32_t x, uint32_t y) { return touch(0x4C, id, x, y); }
    static uint32_t touchesEnd(uint32_t, uint32_t, uint32_t id, uint32_t x, uint32_t y) { return touch(0x54, id, x, y); }
    static uint32_t touchesMove(uint32_t env, uint32_t, uint32_t jids, uint32_t jxs, uint32_t jys) {
        uint32_t n = jni(env, 0x2AC, {env, jids});  // GetArrayLength
        uint32_t size = (n * 4 + 7) & ~7u;
        guest::Frame f(size * 3 + 8);
        uint32_t ids = f.addr(2 * size), xs = f.addr(size), ys = f.addr(0);
        jni(env, 0x32C, {env, jids, 0, n, ids});  // GetIntArrayRegion
        jni(env, 0x334, {env, jxs, 0, n, xs});    // GetFloatArrayRegion
        jni(env, 0x334, {env, jys, 0, n, ys});
        uint32_t view = ld32(rw::call(DIRECTOR_INSTANCE, {}) + 0x9C);
        vcallo(view, 0x50, {view, n, ids, xs, ys});  // GLView::handleTouchesMove
        return 0;
    }
};
RW_FUNCTION(0x459E30, "cocos_android_app_init", Free::appInit);
RW_FUNCTION(0x4DB698, "createHeaders(bool)", Free::createHeaders);
RW_FUNCTION(0x4F0188, "non-virtual thunk to StageState::getBuff", Free::stageGetBuff);
RW_FUNCTION(0x4F036C, "non-virtual thunk to StageState::getCondition", Free::stageGetCondition);
RW_FUNCTION(0x4F9E14, "non-virtual thunk to CustomTextView::textHandler", Free::textHandler);
RW_FUNCTION(0x5C0CF8, "AudioPlaySystemSEDecide", Free::playSystemSEDecide);
RW_FUNCTION(0x5C0E08, "AudioPlaySystemSEPopUp", Free::playSystemSEPopUp);
RW_FUNCTION(0x5C19B4, "AudioClearGameMainBGMStack", Free::clearGameMainBGMStack);
RW_FUNCTION(0x5C1A78, "AudioPlayGameMainSEOneShot", Free::playGameMainSEOneShot);
RW_FUNCTION(0x6C4A54, "start_movie(int,bool)", Free::startMovie);
RW_FUNCTION(0x81ACA0, "Java_org_cocos2dx_lib_Cocos2dxWebViewHelper_didFinishLoading", Free::webViewDidFinishLoading);
RW_FUNCTION(0x8435B0, "Java_org_cocos2dx_lib_Cocos2dxVideoHelper_nativeExecuteVideoCallback",
            Free::videoExecuteCallback);
RW_FUNCTION(0x8436A0, "callVideoNonParameterFun(int,const char*)", Free::callVideoNonParameterFun);
RW_FUNCTION(0x84372C, "setVideoRectJNI(int,int,int,int,int)", Free::setVideoRectJNI);
RW_FUNCTION(0xA40404, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInit", Free::rendererInit);
RW_FUNCTION(0xA40998, "Java_org_cocos2dx_lib_Cocos2dxHelper_nativeSetApkPath", Free::setApkPath);
RW_FUNCTION(0xA40A30, "Java_org_cocos2dx_lib_Cocos2dxHelper_nativeSetContext", Free::setContext);
RW_FUNCTION(0xA41B28, "setStringForKeyJNI(const char*,const char*)", Free::setStringForKey);
RW_FUNCTION(0xA41BEC, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeRender", Free::render);
RW_FUNCTION(0xA424C0, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesBegin", Free::touchesBegin);
RW_FUNCTION(0xA4251C, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesEnd", Free::touchesEnd);
RW_FUNCTION(0xA42578, "Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesMove", Free::touchesMove);

}  // namespace freegame
