// ChatManager (chat entry polling), CarouselDeck (deck medal carousel layout), RaidBossResultPopUp.
#include "../core/Guest.hpp"
#include "../scene/SceneCommon.hpp"

namespace w4b2 {
using namespace scene;

constexpr uint32_t API_MANAGER = 0x4BA8C9;
constexpr uint32_t DATE_SET_NOW = 0x492739;

// ============================================================================================ ChatManager
// singleton at 0x1003928: +0xC, rapidjson stack +0x10, +0x28, +0x2C, per action {Date +0x30, interval +0x38} x 8
struct ChatManager {
    static uint32_t getInstance() {
        const uint32_t guard = 0x10039B8, inst = 0x1003928;
        if (guest::staticInitBegin(guard)) {
            call(0x4DB18F, {inst});
            call(guest::CXA_ATEXIT, {ld32(0xFD64DC), inst, ld32(0xFD604C)});
            guest::staticInitDone(guard);
        }
        return inst;
    }
    static uint32_t construct(uint32_t self) {
        st32(self + 0xC, 0);
        call(0x49A659, {self + 0x10, 0, 0x400});  // rapidjson Stack(allocator, 1024)
        st32(self + 0x28, 0);
        st32(self + 0x2C, 0);
        for (uint32_t i = 0; i != 0x60; i += 12) {
            uint32_t e = self + i;
            st8(e + 0x37, uint8_t(ld8(e + 0x37) & 0xFD));
            st32(e + 0x38, 0);
            call(DATE_SET_NOW, {e + 0x30});
        }
        return self;
    }
    // remembers the server's "__request_interval_order__" for the action
    static uint32_t updateInterval(uint32_t self, uint32_t action, uint32_t doc) {
        uint32_t type = ld32(doc + 0xC);
        if (type != 3) return type;
        uint32_t v = call(0x4B1BB5, {doc, 0xDFA33F});  // doc["__request_interval_order__"]
        if ((ld8(v + 0xD) & 4) == 0) return v;
        uint32_t e = self + action * 12;
        uint32_t interval = ld32(v);
        uint32_t r = call(DATE_SET_NOW, {e + 0x30});
        st32(e + 0x38, interval);
        return r;
    }
    // GET <server>/user {getChatEntry: 1, getBlockEntry: 1} unless polled too recently (then the callback gets
    // status 3 at once)
    static uint32_t getUser(uint32_t self, uint32_t callback) {
        Frame f;
        int32_t since = int32_t(call(0x4922D5, {self + 0x30}));  // Date::calculateIntervalSinceNow
        if (since > 0 || rw::load<int32_t>(self + 0x38) > -since) {
            if (ld32(callback + 0x10) == 0) return 0;
            uint32_t blocks = f.alloc(12);
            st32(blocks, 0);
            st32(blocks + 4, 0);
            st32(blocks + 8, 0);
            call(0x4DB575, {callback, 3, 0, 0, blocks});  // callback(Status 3, 0, 0, {})
            call(0x4DB601, {blocks});                     // vector<Block> dtor
            return 0;
        }
        uint32_t user = ld32(call(API_MANAGER, {}) + 0x7C);
        uint32_t host = (ld8(user + 0x2C) & 1) ? ld32(user + 0x34) : user + 0x2D;
        uint32_t url = f.alloc(0x420);   // StringBuffer: 1 KiB of text, then its length word at +0x400
        call(0x49140D, {url, 0xDFC192, host});  // StringBuffer("%s/user", host)
        uint32_t headers = f.alloc(12);
        call(0x4DB699, {headers, 1});  // createHeaders(true)
        uint32_t doc = f.alloc(8);
        call(0x4B9345, {doc});  // make_shared<Document>
        uint32_t d = ld32(doc);
        uint32_t alloc = ld32(d + 0x10);
        uint32_t obj = 0;
        if (d != 0) {
            st32(d, 0);
            st32(d + 4, 0);
            st32(d + 8, 0);
            st32(d + 0xC, 3);  // SetObject
            obj = ld32(doc);
        }
        call(0x499801, {obj, 0xDFC19A, 1, alloc});        // AddMember("getChatEntry", 1)
        call(0x499801, {ld32(doc), 0xDFC1A7, 1, alloc});  // AddMember("getBlockEntry", 1)
        uint32_t client = call(0x497AE9, {});  // RESTClient::getInstance
        // [this, callback] lambda in a std::function<void(const shared_ptr<Result>&)> (heap stored)
        uint32_t cap = f.alloc(0x20);  // this, 0, then a copy of the callback (std::function at +8)
        st32(cap, self);
        st32(cap + 4, 0);
        call(0x4DB7AD, {cap + 8, callback});
        uint32_t fn = f.alloc(20);
        st32(fn + 16, 0);
        uint32_t func = call(fn::operator_new, {0x28});
        if (func != 0) {
            st32(func, 0xF5583C);
            st32(func + 8, ld32(cap));
            st32(func + 0xC, ld32(cap + 4));
            uint32_t src = ld32(cap + 8 + 0x10);
            if (src == 0) {
                st32(func + 0x20, 0);
            } else if (src == cap + 8) {
                st32(func + 0x20, func + 0x10);
                vcall(src, 0xC, {src, func + 0x10});  // __clone(dst)
            } else {
                st32(func + 0x20, vcall(src, 8, {src}));  // __clone()
            }
        }
        st32(fn + 16, func);
        uint32_t cipher = f.alloc(8), conn = f.alloc(8);
        st32(cipher, 0);
        st32(cipher + 4, 0);
        st32(conn, 0);
        st32(conn + 4, 0);
        uint32_t empty = f.alloc(12);
        strInit(empty, 0xDF8A1A, 0);
        uint32_t result = f.alloc(8);
        call(0x497FE5, {result, client, url, 0, headers, doc, fn, cipher, 0, conn, empty});  // RESTClient::request
        if (ld32(result + 4) != 0) call(fn::shared_release, {ld32(result + 4)});
        strFree(empty);
        if (ld32(conn + 4) != 0) call(fn::shared_release, {ld32(conn + 4)});
        if (ld32(cipher + 4) != 0) call(fn::shared_release, {ld32(cipher + 4)});
        functionDestroy(fn);
        functionDestroy(cap + 8);
        if (ld32(doc + 4) != 0) call(fn::shared_release, {ld32(doc + 4)});
        call(0x47363F, {headers});  // vector<string> dtor
        return 1;
    }
};
RW_FUNCTION(0x4DB0E4, "ChatManager::getInstance", ChatManager::getInstance);
RW_FUNCTION(0x4DB18E, "ChatManager::ChatManager", ChatManager::construct);
RW_FUNCTION(0x4DB224, "ChatManager::updateInterval", ChatManager::updateInterval);
RW_FUNCTION(0x4DB270, "ChatManager::getUser", ChatManager::getUser);

// ============================================================================================ CarouselDeck
// medal panels +0x278[], ability panels +0x29C[], three slots: holders +0x2C4[3] / +0x2E8[3], info +0x2F4[],
// "more" button +0x30C, counter atlas +0x2E0, separator +0x2C0, medal count +0x354, medal buttons +0x358[],
// friend medal button +0x36C, front button +0x370, total +0x374, first shown +0x378, medal ids +0x37C[]
struct CarouselDeck {
    static uint32_t addFriendMedalOntoDeck(uint32_t self, int32_t medal, uint32_t isFriend) {
        uint32_t btn = call(0x5CF9A1, {self, uint32_t(medal)});  // createMadalButton
        if (btn == 0) return 0;
        st32(self + 0x36C, btn);
        call(rwlib::cocos2d::Ref::retain, {btn});  // Ref::retain
        int32_t m = int32_t(call(0xCD6009, {uint32_t(medal), 10}));  // __modsi3
        int32_t id = (m == 4 || m == 2) ? medal - 1 : medal;
        uint32_t c = ld32(self + 0x354);
        st32(self + c * 4 + 0x37C, uint32_t(id));
        c = ld32(self + 0x354);
        call(0x5CFB8D, {self, ld32(self + c * 4 + 0x2F4), ld32(self + c * 4 + 0x278), uint32_t(medal)});  // setAbilityInfo
        c = ld32(self + 0x354);
        call(0x5D017D, {self, ld32(self + c * 4 + 0x29C), uint32_t(medal), 1, isFriend});  // setDeckMedalInfoPanel
        st32(self + 0x374, ld32(self + 0x374) + 1);
        return 0x374;
    }

    // lays the deck medals (from the first shown one) into the three carousel slots
    static uint32_t setDeckMedal(uint32_t self, uint32_t) {
        Frame f;
        for (uint32_t k = 0; k < 3; k++) {
            uint32_t o = ld32(self + 0x2E8 + k * 4);
            vcall(o, 0x138, {o});     // removeAllChildren
            o = ld32(self + 0x2E8 + k * 4);
            vcall(o, 0xB0, {o, 1});   // setVisible(true)
        }
        const uint32_t count = self + 0x354, friendBtn = self + 0x36C, more = self + 0x30C;
        if (ld32(count) != 0) {
            for (uint32_t i = 0; i < ld32(count); i++) {
                uint32_t o = ld32(self + 0x358 + i * 4);
                vcall(o, 0x29C, {o, 0});
                o = ld32(self + 0x358 + i * 4);
                if (vcall(o, 0x11C, {o}) != 0) {  // getParent
                    o = ld32(self + 0x358 + i * 4);
                    vcall(o, 0x124, {o});  // removeFromParent
                }
            }
        }
        uint32_t fr = ld32(friendBtn);
        if (fr != 0) {
            vcall(fr, 0x29C, {fr, 0});
            fr = ld32(friendBtn);
            if (vcall(fr, 0x11C, {fr}) != 0) {
                fr = ld32(friendBtn);
                vcall(fr, 0x124, {fr});
            }
        }
        uint32_t m = ld32(more);
        if (vcall(m, 0x11C, {m}) != 0) {
            m = ld32(more);
            vcall(m, 0x124, {m});
        }
        m = ld32(more);
        uint32_t lwf = vcall(m, 0x298, {m});
        vcall(lwf, 0x80, {lwf, 0});
        st32(self + 0x370, 0);
        uint32_t sep = ld32(self + 0x2C0);
        vcall(sep, 0xB0, {sep, 0});
        uint32_t idx = ld32(self + 0x378);
        uint32_t k = 0;
        auto next = [&]() {
            int32_t total = rw::load<int32_t>(self + 0x374);
            int32_t n = int32_t(idx) + 1;
            idx = total > n ? uint32_t(n) : 0;
        };
        for (;;) {
            uint32_t cnt = ld32(count);
            bool front = false;  // slot 0 got its content: k jumps to 1
            if (idx < cnt) {
                uint32_t h = ld32(self + 0x2C4 + k * 4);
                vcall(h, 0xF0, {h, ld32(self + 0x358 + idx * 4)});
                h = ld32(self + 0x2E8 + k * 4);
                vcall(h, 0xF0, {h, ld32(self + 0x2F4 + idx * 4)});
                if (k == 0) {
                    uint32_t btn = ld32(self + 0x358 + idx * 4);
                    st32(self + 0x370, btn);
                    vcall(btn, 0x29C, {btn, 1});
                    front = true;
                }
            } else if (idx == cnt && ld32(friendBtn) != 0) {
                uint32_t h = ld32(self + 0x2C4 + k * 4);
                vcall(h, 0xF0, {h, ld32(friendBtn)});
                h = ld32(self + 0x2E8 + k * 4);
                vcall(h, 0xF0, {h, ld32(self + 0x2F4 + idx * 4)});
                if (k == 0) {
                    uint32_t b = ld32(friendBtn);
                    st32(self + 0x370, b);
                    vcall(b, 0x29C, {b, 1});
                    uint32_t s = ld32(self + 0x2C0);
                    vcall(s, 0xB0, {s, 1});
                    front = true;
                }
            } else {
                m = ld32(more);
                if (vcall(m, 0x11C, {m}) == 0) {
                    uint32_t h = ld32(self + 0x2C4 + k * 4);
                    vcall(h, 0xF0, {h, ld32(more)});
                    if (k == 0) {
                        m = ld32(more);
                        uint32_t l = vcall(m, 0x298, {m});
                        vcall(l, 0x7C, {l, 0});
                        front = true;
                    }
                }
            }
            next();
            if (front) {
                k = 1;
                continue;
            }
            k++;
            if (k == 3) break;
        }
        uint32_t box = ld32(self + 0x2D4);
        uint32_t cnt = ld32(count);
        int32_t first = rw::load<int32_t>(self + 0x378);
        uint32_t prev = uint32_t(first - 1);
        bool skip = (first >= 1 && prev < cnt) || (prev == cnt && ld32(friendBtn) != 0);
        if (!skip) {
            m = ld32(more);
            if (vcall(m, 0x11C, {m}) == 0) {
                vcall(box, 0xF0, {box, ld32(more)});
                m = ld32(more);
                uint32_t l = vcall(m, 0x298, {m});
                vcall(l, 0x80, {l, 0});
            }
        }
        int32_t total = rw::load<int32_t>(self + 0x374);
        int32_t shown = rw::load<int32_t>(self + 0x378);
        if (total >= shown) {
            uint32_t text = ld32(0xF5D168 + uint32_t(total + ~shown) * 4);
            uint32_t atlas = ld32(self + 0x2E0);
            uint32_t s = f.alloc(12);
            strInitC(s, text);
            call(rwlib::cocos2d::ui::TextAtlas::setString, {atlas, s});  // TextAtlas::setString
            strFree(s);
        }
        return 0;
    }
};
RW_FUNCTION(0x5D073C, "CarouselDeck::addFriendMedalOntoDeck", CarouselDeck::addFriendMedalOntoDeck);
RW_FUNCTION(0x5D17A0, "CarouselDeck::setDeckMedal", CarouselDeck::setDeckMedal);

// ============================================================================================ RaidBossResultPopUp
struct RaidBossResultPopUp {
    // fetches the members of raid `index` (APIManager raids, 176-byte records) and shows its result popup;
    // with no reward ids, just runs the callback
    static uint32_t popUpRaidBossResult(uint32_t index, uint32_t rewards, uint32_t callback) {
        Frame f;
        if (ld32(rewards + 4) == ld32(rewards)) {
            if (ld32(callback + 0x10) != 0) call(0x470E69, {callback});  // callback()
            return 0;
        }
        uint32_t raids = ld32(call(API_MANAGER, {}) + 0x168);
        uint32_t begin = ld32(raids);
        uint32_t n = uint32_t(int32_t(ld32(raids + 4) - begin) >> 4) * 0xBA2E8BA3u;
        if (n <= index) {
            call(0x47CCE1, {});  // vector::at out of range
            return 0;
        }
        uint32_t e = begin + index * 176;
        uint32_t idLo = ld32(e), idHi = ld32(e + 4), field8 = ld32(e + 8), state = ld32(e + 0x2C);
        uint32_t field1c = ld32(e + 0x1C);
        uint32_t sm = call(0x60C26D, {});  // SceneManager::getInstance
        uint32_t empty = newEmptyFunction(f);
        st8(sm + 0x4CC, 1);
        call(0x490109, {sm + 0x548, empty});  // function<void(bool)>::operator=
        functionDestroy(empty);
        call(0x60D169, {call(0x60C26D, {}), 1});  // enableLoadingIndicator(true)
        // the lambda's captures: three words, vector<int> rewards, std::function callback, bool cleared
        uint32_t a = f.alloc(0x40);
        st32(a, field1c);
        st32(a + 4, field8);
        st32(a + 8, index);
        call(0x4AF707, {a + 0xC, rewards});  // vector<int> copy
        call(fn::function_vv_copy, {a + 0x18, callback});
        uint8_t cleared = state == 2 ? 1 : 0;
        st8(a + 0x30, cleared);
        uint32_t am = call(API_MANAGER, {});
        uint32_t b = f.alloc(0x40);
        st32(b, ld32(a));
        st32(b + 4, ld32(a + 4));
        st32(b + 8, ld32(a + 8));
        call(0x4AF707, {b + 0xC, a + 0xC});
        uint32_t src = ld32(a + 0x18 + 0x10);
        if (src == 0) {
            st32(b + 0x18 + 0x10, 0);
        } else if (src == a + 0x18) {
            st32(b + 0x18 + 0x10, b + 0x18);
            vcall(src, 0xC, {src, b + 0x18});
        } else {
            st32(b + 0x18 + 0x10, vcall(src, 8, {src}));
        }
        st8(b + 0x30, ld8(a + 0x30));
        uint32_t fn = f.alloc(20);
        st32(fn + 16, 0);
        uint32_t func = call(fn::operator_new, {0x40});
        if (func != 0) {
            st32(func, 0xF5C7C8);
            st32(func + 8, ld32(b));
            st32(func + 0xC, ld32(b + 4));
            st32(func + 0x10, ld32(b + 8));
            call(0x4AF707, {func + 0x14, b + 0xC});
            call(fn::function_vv_move, {func + 0x20, b + 0x18});
            st8(func + 0x38, ld8(b + 0x30));
        }
        st32(fn + 16, func);
        call(0x4CC3D5, {am, 0, idLo, idHi, fn});  // APIManager::executeRaidMemberGet(id, callback)
        functionDestroy(fn);
        functionDestroy(b + 0x18);
        call(0x478961, {b + 0xC});  // vector<int> dtor
        functionDestroy(a + 0x18);
        call(0x478961, {a + 0xC});
        return 0;
    }
};
RW_FUNCTION(0x5B6DA8, "RaidBossResultPopUp::popUpRaidBossResult", RaidBossResultPopUp::popUpRaidBossResult);

}  // namespace w4b2
