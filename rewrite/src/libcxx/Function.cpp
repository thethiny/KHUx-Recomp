// std::function<R(Args...)> members instantiated over the game's signatures. libc++'s std::function holds a
// 16-byte buffer and __f_ (+16), a pointer to the callable (__base: vtable slots __clone() 2, __clone(__base*) 3,
// destroy 4, destroy_deallocate 5, operator() 6). A small callable lives in the buffer (__f_ == this), a big one
// on the heap. Copy, move, assignment and swap are the same code for every signature; operator() spills its
// by-value arguments to the stack and passes their addresses (__base::operator() takes them by reference).
#include "libcxx/Libcxx.hpp"

using libcxx::copyWords;
using rw::load;
using rw::save;

namespace {

constexpr uint32_t F = 16;   // offset of __f_

uint32_t vcallBase(uint32_t base, uint32_t slot, std::initializer_list<uint32_t> args) {
    return rw::vcall(rw::host_ptr<uint8_t>(base), slot, args);
}
void cloneInto(uint32_t base, uint32_t dst) { vcallBase(base, 3, {base, dst}); }
void destroyBase(uint32_t base) { vcallBase(base, 4, {base}); }

// operator() on an empty std::function: throw std::bad_function_call
void throwBadFunctionCall() {
    const uint32_t e = rw::call(0xC98131, {4});            // __cxa_allocate_exception
    save<uint32_t>(e, 0xF7E01C);                            // vtable for std::bad_function_call
    rw::call(0xC983D5, {e, 0xF7E008, 0xC99C5D});           // __cxa_throw(e, &typeid(bad_function_call), ~exception)
}

// function(const function&)
uint32_t copyConstruct(uint32_t self, uint32_t other) {
    const uint32_t f = load<uint32_t>(other + F);
    if (!f) save<uint32_t>(self + F, 0);
    else if (f == other) {
        save<uint32_t>(self + F, self);
        cloneInto(load<uint32_t>(other + F), self);
    } else {
        save<uint32_t>(self + F, vcallBase(f, 2, {f}));   // __clone() on the heap
    }
    return self;
}

// function(function&&): a heap callable changes owner, one in the buffer is cloned (the source keeps its own)
uint32_t moveConstruct(uint32_t self, uint32_t other) {
    const uint32_t f = load<uint32_t>(other + F);
    if (!f) save<uint32_t>(self + F, 0);
    else if (f == other) {
        save<uint32_t>(self + F, self);
        cloneInto(load<uint32_t>(other + F), self);
    } else {
        save<uint32_t>(self + F, f);
        save<uint32_t>(other + F, 0);
    }
    return self;
}

// operator=(function&&): destroy the own callable, then take the other's as the move constructor does
uint32_t moveAssign(uint32_t self, uint32_t other) {
    const uint32_t own = load<uint32_t>(self + F);
    if (own == self) destroyBase(own);
    else if (own) vcallBase(own, 5, {own});                 // destroy_deallocate
    save<uint32_t>(self + F, 0);
    const uint32_t f = load<uint32_t>(other + F);
    if (!f) save<uint32_t>(self + F, 0);
    else if (f == other) {
        save<uint32_t>(self + F, self);
        cloneInto(load<uint32_t>(other + F), self);
    } else {
        save<uint32_t>(self + F, f);
        save<uint32_t>(other + F, 0);
    }
    return self;
}

// swap(function&): buffers are swapped through a temporary buffer on the stack
void swap(uint32_t a, uint32_t b) {
    const uint32_t fb = load<uint32_t>(b + F), fa = load<uint32_t>(a + F);
    if (fa == a) {
        if (fb == b) {
            guest::Frame frame(16);
            const uint32_t tmp = frame.addr();
            cloneInto(fa, tmp);
            destroyBase(load<uint32_t>(a + F));
            save<uint32_t>(a + F, 0);
            cloneInto(load<uint32_t>(b + F), fa);
            destroyBase(load<uint32_t>(b + F));
            save<uint32_t>(b + F, 0);
            save<uint32_t>(a + F, a);
            cloneInto(tmp, fb);
            destroyBase(tmp);
            save<uint32_t>(b + F, b);
        } else {
            cloneInto(fa, b);
            destroyBase(load<uint32_t>(a + F));
            save<uint32_t>(a + F, load<uint32_t>(b + F));
            save<uint32_t>(b + F, b);
        }
    } else if (fb == b) {
        cloneInto(b, a);
        destroyBase(load<uint32_t>(b + F));
        save<uint32_t>(b + F, load<uint32_t>(a + F));
        save<uint32_t>(a + F, a);
    } else {
        save<uint32_t>(a + F, fb);
        save<uint32_t>(b + F, fa);
    }
}

// operator=(const function&): function(other).swap(*this), through the instance's own copy constructor and swap
template <uint32_t Copy, uint32_t Swap> uint32_t copyAssign(uint32_t self, uint32_t other) {
    guest::Frame frame(0x18);
    const uint32_t tmp = frame.addr();
    rw::call(Copy, {tmp, other});
    rw::call(Swap, {tmp, self});
    libcxx::destroyFunction(tmp);
    return self;
}

// ---------------------------------------------------------------------------------------------- operator()
// The callable, or bad_function_call.
uint32_t target(uint32_t self) {
    const uint32_t f = load<uint32_t>(self + F);
    if (!f) throwBadFunctionCall();
    return f;
}

// one argument passed by reference (const shared_ptr&, const T&): forwarded
uint32_t callRef(uint32_t self, uint32_t ref) {
    const uint32_t f = target(self);
    return vcallBase(f, 6, {f, ref});
}

// one argument by value (an enum, a pointer): spilled, its address passed
uint32_t callValue(uint32_t self, uint32_t value) {
    const uint32_t f = target(self);
    guest::Frame frame(8);
    *frame.at<uint32_t>(0) = value;
    return vcallBase(f, 6, {f, frame.addr()});
}

// ... a void one (the stack guard check leaves r0 = 0)
void callValueVoid(uint32_t self, uint32_t value) { callValue(self, value); }

// bool(int, shared_ptr<const master::EvCampaign>&): the int spilled, the reference forwarded
uint32_t callIntRef(uint32_t self, uint32_t value, uint32_t ref) {
    const uint32_t f = target(self);
    guest::Frame frame(8);
    *frame.at<uint32_t>(0) = value;
    return vcallBase(f, 6, {f, frame.addr(), ref});
}

// Client::StatusCode(StatusCode, const shared_ptr<Ret>&, const Document&)
uint32_t callStatusRefRef(uint32_t self, uint32_t status, uint32_t ret, uint32_t doc) {
    const uint32_t f = target(self);
    guest::Frame frame(8);
    *frame.at<uint32_t>(0) = status;
    return vcallBase(f, 6, {f, frame.addr(), ret, doc});
}

// two by-value words (iap::Result + a pointer)
void callTwoValues(uint32_t self, uint32_t a, uint32_t b) {
    const uint32_t f = target(self);
    guest::Frame frame(8);
    *frame.at<uint32_t>(4) = a;
    *frame.at<uint32_t>(0) = b;
    vcallBase(f, 6, {f, frame.addr(4), frame.addr(0)});
}

// void(bool, const unordered_map*)
void callBoolPointer(uint32_t self, uint32_t flag, uint32_t p) {
    const uint32_t f = target(self);
    guest::Frame frame(8);
    *frame.at<uint8_t>(7) = uint8_t(flag);
    *frame.at<uint32_t>(0) = p;
    vcallBase(f, 6, {f, frame.addr(7), frame.addr(0)});
}

// void(const vector<int>&, const unordered_map*, bool)
void callRefPointerBool(uint32_t self, uint32_t ref, uint32_t p, uint32_t flag) {
    const uint32_t f = target(self);
    guest::Frame frame(8);
    *frame.at<uint32_t>(4) = p;
    *frame.at<uint8_t>(3) = uint8_t(flag);
    vcallBase(f, 6, {f, ref, frame.addr(4), frame.addr(3)});
}

// void(bool, FacebookManager::ErrorCode, string): the string (by value) is already a reference
void callFacebook3(uint32_t self, uint32_t flag, uint32_t code, uint32_t str) {
    const uint32_t f = target(self);
    guest::Frame frame(8);
    *frame.at<uint8_t>(7) = uint8_t(flag);
    *frame.at<uint32_t>(0) = code;
    vcallBase(f, 6, {f, frame.addr(7), frame.addr(0), str});
}

// void(bool, ErrorCode, string, string / vector<string>): the 4th argument (a reference) on the stack
void callFacebook4(uint32_t self, uint32_t flag, uint32_t code, uint32_t str, uint32_t fourth) {
    const uint32_t f = target(self);
    guest::Frame frame(8);
    *frame.at<uint8_t>(7) = uint8_t(flag);
    *frame.at<uint32_t>(0) = code;
    vcallBase(f, 6, {f, frame.addr(7), frame.addr(0), str, fourth});
}

// void(ChatManager::StatusCode, unsigned, unsigned, const vector<Block>&)
void callChatBlocks(uint32_t self, uint32_t status, uint32_t a, uint32_t b, uint32_t blocks) {
    const uint32_t f = target(self);
    guest::Frame frame(12);
    *frame.at<uint32_t>(8) = status;
    *frame.at<uint32_t>(4) = a;
    *frame.at<uint32_t>(0) = b;
    vcallBase(f, 6, {f, frame.addr(8), frame.addr(4), frame.addr(0), blocks});
}

// void(ChatManager::StatusCode, const vector<Message>&, unsigned, unsigned, unsigned)
void callChatMessages(uint32_t self, uint32_t status, uint32_t messages, uint32_t a, uint32_t b, uint32_t c) {
    const uint32_t f = target(self);
    guest::Frame frame(16);
    *frame.at<uint32_t>(12) = status;
    *frame.at<uint32_t>(8) = a;
    *frame.at<uint32_t>(4) = b;
    *frame.at<uint32_t>(0) = c;
    vcallBase(f, 6, {f, frame.addr(12), messages, frame.addr(8), frame.addr(4), frame.addr(0)});
}

// void(Avatar::InstanceInfo&, int, int, const char*)
void callInstanceInfo(uint32_t self, uint32_t info, uint32_t a, uint32_t b, uint32_t text) {
    const uint32_t f = target(self);
    guest::Frame frame(12);
    *frame.at<uint32_t>(8) = a;
    *frame.at<uint32_t>(4) = b;
    *frame.at<uint32_t>(0) = text;
    vcallBase(f, 6, {f, info, frame.addr(8), frame.addr(4), frame.addr(0)});
}

// void(SceneUtil::MedalInfoButtonType, bool, unsigned long long)
void callMedalInfoButton(uint32_t self, uint32_t type, uint32_t flag, uint64_t id) {
    const uint32_t f = target(self);
    guest::Frame frame(16);
    *frame.at<uint32_t>(12) = type;
    *frame.at<uint8_t>(11) = uint8_t(flag);
    *frame.at<uint64_t>(0) = id;
    vcallBase(f, 6, {f, frame.addr(12), frame.addr(11), frame.addr(0)});
}

// StageStart::Treasure(unsigned): the result is returned through r0 (sret), the function is r1
void callTreasure(uint32_t result, uint32_t self, uint32_t value) {
    const uint32_t f = target(self);
    guest::Frame frame(8);
    *frame.at<uint32_t>(0) = value;
    vcallBase(f, 6, {result, f, frame.addr()});
}

// shared_ptr<const master::Base>(const rapidjson::Value&): sret, the reference forwarded
void callMasterParser(uint32_t result, uint32_t self, uint32_t value) {
    const uint32_t f = target(self);
    vcallBase(f, 6, {result, f, value});
}

// ---------------------------------------------------------------------------------------------- assignment from callables
// operator=(F&&) for an F stored in place: function(F).swap(*this) - the temporary built on the stack (its __func:
// vtable + init(tmp)), swapped with the instance's own swap, then destroyed
template <uint32_t Swap, class Init> uint32_t assignInPlace(uint32_t self, Init init) {
    guest::Frame frame(0x18);
    const uint32_t tmp = frame.addr();
    init(tmp);
    rw::call(Swap, {tmp, self});
    libcxx::destroyFunction(tmp);
    return self;
}

// operator=(__bind<R (C::*)(A...), C*, _1, _2>&&): the 3-word bind in a buffer-held __func (Vtable)
template <uint32_t Vtable, uint32_t Swap> uint32_t assignSmallBind(uint32_t self, uint32_t bind) {
    return assignInPlace<Swap>(self, [&](uint32_t tmp) {
        save<uint32_t>(tmp, Vtable);
        copyWords(tmp + 4, bind, 12);
        save<uint32_t>(tmp + F, tmp);
    });
}

// operator=(R (*)()): a function pointer (null: an empty function) in a buffer-held __func (Vtable)
template <uint32_t Vtable, uint32_t Swap> uint32_t assignFunctionPointer(uint32_t self, uint32_t fp) {
    return assignInPlace<Swap>(self, [&](uint32_t tmp) {
        save<uint32_t>(tmp + F, 0);
        if (fp) {
            save<uint32_t>(tmp + F, tmp);
            save<uint32_t>(tmp, Vtable);
            save<uint32_t>(tmp + 4, fp);
        }
    });
}

// function<void(MedalInfoButtonType, bool, unsigned long long)>(function<void(MedalInfoButtonType, bool, int)>):
// the wrapped function moved into a heap __func (0x20 bytes, the function at 8)
constexpr uint32_t MedalInfoIntFunction_move = 0x487A01;   // function<void(MedalInfoButtonType, bool, int)>(&&)
uint32_t medalInfoFromIntFunction(uint32_t self, uint32_t other) {
    save<uint32_t>(self + F, 0);
    const uint32_t p = guest::newObject(0x20);
    if (p) {
        save<uint32_t>(p, 0xF7E0A8);
        rw::call(MedalInfoIntFunction_move, {p + 8, other});
    }
    save<uint32_t>(self + F, p);
    return self;
}

// ... operator=(function<void(MedalInfoButtonType, bool, int)>&): function(function(other)).swap(*this)
uint32_t medalInfoAssignIntFunction(uint32_t self, uint32_t other) {
    guest::Frame frame(0x38);
    const uint32_t copy = frame.addr(), tmp = frame.addr(0x18);
    rw::call(0x48765D, {copy, other});           // function<void(MedalInfoButtonType, bool, int)>(const&)
    rw::call(0x487691, {tmp, copy, 0});          // the converting constructor above
    rw::call(0x4876D9, {tmp, self});             // swap
    libcxx::destroyFunction(tmp);
    libcxx::destroyFunction(copy);
    return self;
}

// ---------------------------------------------------------------------------------------------- from a __bind
// function(__bind<R (C::*)(A...), C*, ...>): the bound object (member function pointer, object, a bound value)
// passed by value in r1-r3 and one stack word, stored in a heap __func
template <uint32_t Vtable> uint32_t fromSmallBind(uint32_t self, uint32_t fn, uint32_t adj, uint32_t object, uint32_t bound) {
    save<uint32_t>(self + F, 0);
    const uint32_t p = guest::newObject(0x14);
    if (p) {
        save<uint32_t>(p, Vtable);
        save<uint32_t>(p + 4, fn);
        save<uint32_t>(p + 8, adj);
        save<uint32_t>(p + 0xC, object);
        save<uint32_t>(p + 0x10, bound);
    }
    save<uint32_t>(self + F, p);
    return self;
}

// function(__bind<void (api::Client::*)(const Request&, const shared_ptr<Result>&), Client*, Request, _1>&&):
// the Request (a shared_ptr, a string, a std::function and two flags) is moved into the heap __func
uint32_t fromRequestBind(uint32_t self, uint32_t bind) {
    save<uint32_t>(self + F, 0);
    const uint32_t p = guest::newObject(0x50);
    if (p) {
        save<uint32_t>(p, 0xF7F64C);   // vtable for __func<__bind<... Client::Request ...>>
        const uint32_t fn = load<uint32_t>(bind), adj = load<uint32_t>(bind + 4);
        save<uint32_t>(p + 0xC, adj);
        save<uint32_t>(p + 8, fn);
        save<uint32_t>(p + 0x10, load<uint32_t>(bind + 8));        // Client*
        save<uint32_t>(p + 0x18, load<uint32_t>(bind + 0x10));     // shared_ptr (moved)
        save<uint32_t>(p + 0x1C, load<uint32_t>(bind + 0x14));
        save<uint32_t>(bind + 0x10, 0);
        save<uint32_t>(bind + 0x14, 0);
        const uint32_t s0 = load<uint32_t>(bind + 0x18);           // string (moved)
        save<uint32_t>(p + 0x24, load<uint32_t>(bind + 0x1C));
        save<uint32_t>(p + 0x28, load<uint32_t>(bind + 0x20));
        save<uint32_t>(bind + 0x1C, 0);
        save<uint32_t>(bind + 0x20, 0);
        save<uint32_t>(p + 0x20, s0);
        rw::call(0x4B9723, {p + 0x30, bind + 0x28});                // the std::function, moved
        const uint8_t b0 = load<uint8_t>(bind + 0x40), b1 = load<uint8_t>(bind + 0x41);
        save<uint8_t>(p + 0x49, b1);
        save<uint8_t>(p + 0x48, b0);
        save<uint32_t>(p + 8, load<uint32_t>(bind));
        save<uint32_t>(p + 0xC, load<uint32_t>(bind + 4));
    }
    save<uint32_t>(self + F, p);
    return self;
}

// function(__bind<void (DownloadManager::*)(unsigned, const function<bool(...)>&, long, HttpClient*, HttpResponse*),
// DownloadManager*, unsigned&, function<...>, long&, _1, _2>&&)
uint32_t fromDownloadBind(uint32_t self, uint32_t bind) {
    save<uint32_t>(self + F, 0);
    const uint32_t p = guest::newObject(0x38);
    if (p) {
        save<uint32_t>(p, 0xF7FA88);   // vtable for __func<__bind<... DownloadManager ...>>
        const uint32_t fn = load<uint32_t>(bind), adj = load<uint32_t>(bind + 4);
        save<uint32_t>(p + 0xC, adj);
        save<uint32_t>(p + 8, fn);
        save<uint32_t>(p + 0x10, load<uint32_t>(bind + 8));
        save<uint32_t>(p + 0x14, load<uint32_t>(bind + 0xC));
        rw::call(0x4DFC27, {p + 0x18, bind + 0x10});                // the std::function, moved
        save<uint32_t>(p + 0x30, load<uint32_t>(bind + 0x28));
        const uint32_t fn2 = load<uint32_t>(bind), adj2 = load<uint32_t>(bind + 4);
        save<uint32_t>(p + 0xC, adj2);
        save<uint32_t>(p + 8, fn2);
    }
    save<uint32_t>(self + F, p);
    return self;
}

}  // namespace

// copy / move construction, assignment, swap
RW_FUNCTION(0x4683FE, "std::function<void(MedalInfoButtonType, bool)>::function(const&)", copyConstruct);
RW_FUNCTION(0x4B7328, "std::function<StatusCode(StatusCode, const shared_ptr<Ret>&, const Document&)>::function(const&)", copyConstruct);
RW_FUNCTION(0x4DB7AC, "std::function<void(ChatManager::StatusCode, unsigned, unsigned, const vector<Block>&)>::function(const&)", copyConstruct);
RW_FUNCTION(0x4DBC56, "std::function<void(ChatManager::StatusCode, const vector<Message>&, unsigned, unsigned, unsigned)>::function(const&)", copyConstruct);
RW_FUNCTION(0x4DBFEC, "std::function<void(ChatManager::StatusCode)>::function(const&)", copyConstruct);
RW_FUNCTION(0x4DFBF4, "std::function<bool(const shared_ptr<bg::Data>&)>::function(const&)", copyConstruct);
RW_FUNCTION(0x4E4DB4, "std::function<void(bool, FacebookManager::ErrorCode, string)>::function(const&)", copyConstruct);
RW_FUNCTION(0x4E5038, "std::function<void(bool, FacebookManager::ErrorCode, string, vector<string>)>::function(const&)", copyConstruct);
RW_FUNCTION(0x4E541C, "std::function<void(bool, FacebookManager::ErrorCode, string, string)>::function(const&)", copyConstruct);
RW_FUNCTION(0x589770, "std::function<void(const vector<int>&, const unordered_map<string, iap::ProductInfo>*, bool)>::function(const&)", copyConstruct);
RW_FUNCTION(0x58996C, "std::function<void(bool, const unordered_map<string, iap::ProductInfo>*)>::function(const&)", copyConstruct);
RW_FUNCTION(0x5C8B14, "std::function<bool(LWF::LWF*)>::function(const&)", copyConstruct);
RW_FUNCTION(0x47B5C4, "std::function<void(RenderTexture*, const string&)>::function(&&)", moveConstruct);
RW_FUNCTION(0x47BC68, "std::function<void(bool)>::function(&&)", moveConstruct);
RW_FUNCTION(0x47F904, "std::function<void()>::function(&&)", moveConstruct);
RW_FUNCTION(0x481F0C, "std::function<void(Node*)>::function(&&)", moveConstruct);
RW_FUNCTION(0x488968, "std::function<void(MedalInfoButtonType, bool)>::function(&&)", moveConstruct);
RW_FUNCTION(0x48B1E8, "std::function<void(bool, bool)>::function(&&)", moveConstruct);
RW_FUNCTION(0x543044, "std::function<void(int)>::function(&&)", moveConstruct);
RW_FUNCTION(0x5AAE3C, "std::function<Layer*()>::function(&&)", moveConstruct);
RW_FUNCTION(0x5BE2FC, "std::function<void(bool, const char*)>::function(&&)", moveConstruct);
RW_FUNCTION(0x474424, "std::function<bool(int, shared_ptr<const master::EvCampaign>&)>::operator=(&&)", moveAssign);
RW_FUNCTION(0x8DDEFE, "std::function<void(void*)>::operator=(&&)", moveAssign);
RW_FUNCTION(0x4821B4, "std::function<void(WebView*, string)>::swap", swap);
RW_FUNCTION(0x4876D8, "std::function<void(MedalInfoButtonType, bool, unsigned long long)>::swap", swap);
RW_FUNCTION(0x5F74E8, "std::function<void(BattleMarker*)>::swap", swap);
RW_FUNCTION(0x606758, "std::function<void(GestureRecognizer*, const Vec2&, unsigned)>::swap", swap);
RW_FUNCTION(0x764738, "std::function<StageStart::Treasure(unsigned)>::swap", swap);
RW_FUNCTION(0x490108, "std::function<void(bool)>::operator=(const&)", (copyAssign<0x45C84D, 0x490179>));
RW_FUNCTION(0x4917C8, "std::function<void*(unsigned)>::operator=(const&)", (copyAssign<0x491765, 0x491E5D>));
RW_FUNCTION(0x491838, "std::function<void(void*)>::operator=(const&)", (copyAssign<0x491797, 0x491F59>));
RW_FUNCTION(0x497DA0, "std::function<void(const shared_ptr<RESTClient::Result>&)>::operator=(const&)", (copyAssign<0x49BC31, 0x49BC65>));
RW_FUNCTION(0x49D278, "std::function<int(double, double, double, double)>::operator=(const&)", (copyAssign<0x49D2E9, 0x49D31D>));
RW_FUNCTION(0x49D418, "std::function<void(HttpClient*, HttpResponse*)>::operator=(const&)", (copyAssign<0x49D489, 0x49D4BD>));
RW_FUNCTION(0x4E176C, "std::function<void(iap::Result, const unordered_map<string, iap::ProductInfo>*)>::operator=(const&)", (copyAssign<0x4E23E1, 0x4E2415>));
RW_FUNCTION(0x4E2750, "std::function<void(const char*)>::operator=(const&)", (copyAssign<0x4E292D, 0x4E2961>));
RW_FUNCTION(0x4E3884, "std::function<void(bool, FacebookManager::ErrorCode, string)>::operator=(const&)", (copyAssign<0x4E4DB5, 0x4E65E1>));
RW_FUNCTION(0x4E3F78, "std::function<void(bool, FacebookManager::ErrorCode, string, vector<string>)>::operator=(const&)", (copyAssign<0x4E5039, 0x4E64E5>));
RW_FUNCTION(0x4E47F0, "std::function<void(bool, FacebookManager::ErrorCode, string, string)>::operator=(const&)", (copyAssign<0x4E541D, 0x4E63E9>));
RW_FUNCTION(0x4EB9CC, "std::function<void()>::operator=(const&)", (copyAssign<0x4683CD, 0x4EE86D>));
RW_FUNCTION(0x5315C0, "std::function<void(Ref*, Widget::TouchEventType)>::operator=(const&)", (copyAssign<0x531A69, 0x531A9D>));
RW_FUNCTION(0x53327C, "std::function<bool()>::operator=(const&)", (copyAssign<0x46A3B5, 0x47D0F1>));
RW_FUNCTION(0x5BDC18, "std::function<void(bool, const char*)>::operator=(const&)", (copyAssign<0x5BD96D, 0x5BE335>));
RW_FUNCTION(0x5C9BE4, "std::function<void(const string&, LWF::Movie*, LWF::Button*)>::operator=(const&)", (copyAssign<0x5C9C55, 0x5C9C89>));
RW_FUNCTION(0x5CB688, "std::function<unsigned(const shared_ptr<AvatarIcon>&)>::operator=(const&)", (copyAssign<0x5CB6F9, 0x5CB72D>));
RW_FUNCTION(0x607440, "std::function<void(Vec2&)>::operator=(const&)", (copyAssign<0x6074B1, 0x6074E5>));
RW_FUNCTION(0x6075E0, "std::function<void(function<void(bool)>)>::operator=(const&)", (copyAssign<0x607651, 0x607685>));
RW_FUNCTION(0x607780, "std::function<bool(unsigned)>::operator=(const&)", (copyAssign<0x6077F1, 0x607825>));
RW_FUNCTION(0x607920, "std::function<bool(const Vec2&, const Vec2&)>::operator=(const&)", (copyAssign<0x607991, 0x6079C5>));
RW_FUNCTION(0x607AC0, "std::function<bool(float, float)>::operator=(const&)", (copyAssign<0x607B31, 0x607B65>));
RW_FUNCTION(0x612E28, "std::function<Layer*()>::operator=(const&)", (copyAssign<0x5AAC65, 0x636F15>));
RW_FUNCTION(0x6F6094, "std::function<void(const string&, LWF::TextRenderer*)>::operator=(const&)", (copyAssign<0x6F6105, 0x6F6139>));
RW_FUNCTION(0x74D910, "std::function<void(EnemyActor*, bool)>::operator=(const&)", (copyAssign<0x74DB7B, 0x74DBAD>));
RW_FUNCTION(0x7535CC, "std::function<bool(EnemyActor*)>::operator=(const&)", (copyAssign<0x75363D, 0x753671>));
RW_FUNCTION(0x76F034, "std::function<unsigned(const shared_ptr<const SafeBuffer>&)>::operator=(const&)", (copyAssign<0x76F0A5, 0x76F0D9>));

// operator()
RW_FUNCTION(0x49911C, "std::function<void(const shared_ptr<RESTClient::Result>&)>::operator()", callRef);
RW_FUNCTION(0x4DF5E8, "std::function<bool(const shared_ptr<bg::Data>&)>::operator()", callRef);
RW_FUNCTION(0x5CADBC, "std::function<unsigned(const shared_ptr<AvatarIcon>&)>::operator()", callRef);
RW_FUNCTION(0x5E00C0, "std::function<bool(const MedalDeck::MedalInfo&)>::operator()", callRef);
RW_FUNCTION(0x5E03B4, "std::function<void(const shared_ptr<const master::Medal>&)>::operator()", callRef);
RW_FUNCTION(0x76E754, "std::function<unsigned(const shared_ptr<const SafeBuffer>&)>::operator()", callRef);
RW_FUNCTION(0x4DBF6C, "std::function<void(ChatManager::StatusCode)>::operator()", callValueVoid);
RW_FUNCTION(0x5D6108, "std::function<void(BattleMarker*)>::operator()", callValueVoid);
RW_FUNCTION(0x74DEDC, "std::function<bool(EnemyActor*)>::operator()", callValue);
RW_FUNCTION(0x474474, "std::function<bool(int, shared_ptr<const master::EvCampaign>&)>::operator()", callIntRef);
RW_FUNCTION(0x4B9140, "std::function<StatusCode(StatusCode, const shared_ptr<Ret>&, const Document&)>::operator()", callStatusRefRef);
RW_FUNCTION(0x4E1920, "std::function<void(iap::Result, const iap::Purchase*)>::operator()", callTwoValues);
RW_FUNCTION(0x4E189C, "std::function<void(iap::Result, const unordered_map<string, iap::ProductInfo>*)>::operator()", callTwoValues);
RW_FUNCTION(0x5899A0, "std::function<void(bool, const unordered_map<string, iap::ProductInfo>*)>::operator()", callBoolPointer);
RW_FUNCTION(0x58E310, "std::function<void(const vector<int>&, const unordered_map<string, iap::ProductInfo>*, bool)>::operator()", callRefPointerBool);
RW_FUNCTION(0x4E33B8, "std::function<void(bool, FacebookManager::ErrorCode, string)>::operator()", callFacebook3);
RW_FUNCTION(0x4E3324, "std::function<void(bool, FacebookManager::ErrorCode, string, vector<string>)>::operator()", callFacebook4);
RW_FUNCTION(0x4E3440, "std::function<void(bool, FacebookManager::ErrorCode, string, string)>::operator()", callFacebook4);
RW_FUNCTION(0x4DB574, "std::function<void(ChatManager::StatusCode, unsigned, unsigned, const vector<Block>&)>::operator()", callChatBlocks);
RW_FUNCTION(0x4DBB28, "std::function<void(ChatManager::StatusCode, const vector<Message>&, unsigned, unsigned, unsigned)>::operator()", callChatMessages);
RW_FUNCTION(0x5C7184, "std::function<void(Avatar::InstanceInfo&, int, int, const char*)>::operator()", callInstanceInfo);
RW_FUNCTION(0x52F8C0, "std::function<void(MedalInfoButtonType, bool, unsigned long long)>::operator()", callMedalInfoButton);
RW_FUNCTION(0x764080, "std::function<StageStart::Treasure(unsigned)>::operator()", callTreasure);
RW_FUNCTION(0x76DB44, "std::function<shared_ptr<const master::Base>(const rapidjson::Value&)>::operator()", callMasterParser);

// from a __bind
RW_FUNCTION(0x4B81B4, "std::function<void(const shared_ptr<RESTClient::Result>&)>::function(__bind<Client::*(Action, ...)>)", fromSmallBind<0xF7F6C8>);
RW_FUNCTION(0x4C553C, "std::function<StatusCode(StatusCode, const shared_ptr<Ret>&, const Document&)>::function(__bind<APIManager::*>)", fromSmallBind<0xF7F76C>);
RW_FUNCTION(0x4B9010, "std::function<void(const shared_ptr<RESTClient::Result>&)>::function(__bind<Client::*(const Request&, ...)>&&)", fromRequestBind);
RW_FUNCTION(0x4DF584, "std::function<void(HttpClient*, HttpResponse*)>::function(__bind<DownloadManager::*>&&)", fromDownloadBind);

// ---------------------------------------------------------------------------------------------- more instances
RW_FUNCTION(0x45C84C, "std::function<void(bool)>::function(const&)", copyConstruct);
RW_FUNCTION(0x48765C, "std::function<void(MedalInfoButtonType, bool, int)>::function(const&)", copyConstruct);
RW_FUNCTION(0x530E0E, "std::function<void(LWF::Movie*)>::function(const&)", copyConstruct);
RW_FUNCTION(0x487A00, "std::function<void(MedalInfoButtonType, bool, int)>::function(&&)", moveConstruct);
RW_FUNCTION(0x483E4C, "std::function<bool(Touch*, Event*)>::swap", swap);
RW_FUNCTION(0x4F3698, "std::function<void(Touch*, Event*)>::swap", swap);
RW_FUNCTION(0x636F14, "std::function<Layer*()>::swap", swap);
RW_FUNCTION(0x81509C, "std::function<bool(LWF::LWF*)>::swap", swap);
RW_FUNCTION(0x814AE0, "std::function<bool(LWF::LWF*)>::operator=(const&)", (copyAssign<0x5C8B15, 0x81509D>));
RW_FUNCTION(0x54A064, "std::function<Widget*(int)>::operator()", callValue);
RW_FUNCTION(0x7C8298, "std::function<void(Ref*, void*)>::operator()", callTwoValues);
RW_FUNCTION(0x5FC9C4, "std::function<bool(Touch*, Event*)>::operator=(__bind<bool (SceneActionMap::*)(Touch*, Event*), SceneActionMap*, _1, _2>&&)", (assignSmallBind<0xF973AC, 0x483E4D>));
RW_FUNCTION(0x5FCA48, "std::function<void(Touch*, Event*)>::operator=(__bind<void (SceneActionMap::*)(Touch*, Event*), SceneActionMap*, _1, _2>&&)", (assignSmallBind<0xF97360, 0x4F3699>));
RW_FUNCTION(0x60CE4C, "std::function<Layer*()>::operator=(Layer* (*)())", (assignFunctionPointer<0xF7E130, 0x636F15>));
RW_FUNCTION(0x487690, "std::function<void(MedalInfoButtonType, bool, unsigned long long)>::function(function<void(MedalInfoButtonType, bool, int)>)", medalInfoFromIntFunction);
RW_FUNCTION(0x46B05C, "std::function<void(MedalInfoButtonType, bool, unsigned long long)>::operator=(function<void(MedalInfoButtonType, bool, int)>&)", medalInfoAssignIntFunction);
