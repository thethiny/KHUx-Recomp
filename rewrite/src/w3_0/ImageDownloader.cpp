#include "ImageDownloader.hpp"

#include "../scene/SceneCommon.hpp"

using namespace scene;

namespace {
constexpr uint32_t STRLEN = 0x40E08C;                      // strlen (import)
constexpr uint32_t MEMCMP = 0x40EF2C;                      // memcmp (import)
constexpr uint32_t STRING_ASSIGN = 0x48D811;               // std::string::assign(const char*, size_t)
constexpr uint32_t STRING_RESERVE = 0x48D705;              // std::string::reserve(size_t)
constexpr uint32_t STRING_COMPARE = 0x47CBD5;              // std::string::compare(const char*)
constexpr uint32_t STRING_VECTOR_DTOR = 0x47363F;          // std::__vector_base<std::string>::~__vector_base
constexpr uint32_t STRING_VECTOR_PUSH_BACK_SLOW = 0x484465;  // vector<string>::__push_back_slow_path(string&&)
constexpr uint32_t STRINGBUFFER_CTOR = 0x49140D;           // StringBuffer::StringBuffer(fmt, ...)
constexpr uint32_t REF_RELEASE = rwlib::cocos2d::Ref::release;                 // cocos2d::Ref::release()
constexpr uint32_t TEXTURE2D_CTOR = rwlib::cocos2d::Texture2D::ctor_base;              // cocos2d::Texture2D::Texture2D()
constexpr uint32_t TEXTURE2D_INIT_WITH_IMAGE = rwlib::cocos2d::Texture2D::initWithImage;   // Texture2D::initWithImage(Image*)
constexpr uint32_t IMAGEVIEW_LOAD_TEXTURE = rwlib::cocos2d::ui::ImageView::loadTexture__PNS_9Texture2DE;      // ui::ImageView::loadTexture(Texture2D*)
constexpr uint32_t DOWNLOAD_MANAGER_GET_INSTANCE = 0x4DECB1;
constexpr uint32_t DOWNLOAD_MANAGER_START = 0x4DED65;      // start(url, const function<bool(const shared_ptr<bg::Data>&)>&, timeout, name)
constexpr uint32_t DOWNLOAD_MANAGER_CANCEL = 0x4DF62D;     // cancel(unsigned handle)
constexpr uint32_t SCENEUTIL_IS_LOGGED_IN_FACEBOOK = 0x4697E9;
constexpr uint32_t FACEBOOK_GET_INSTANCE = 0x4E5585;       // BGFacebook::FacebookManager::getInstance()
constexpr uint32_t FACEBOOK_IS_CONNECTING = 0x4E56B1;
constexpr uint32_t FACEBOOK_GET_PROFILE_IMAGE_URL = 0x4E5783;  // (const string& id, function<void(bool, ..., string, string)>)
constexpr uint32_t SCENE_MANAGER_GET_INSTANCE = 0x60C26D;
constexpr uint32_t SCENE_MANAGER_ADD_FACE_IMAGE_URL = 0x621C29;  // (const string& id, const string& url)
// the maps' libc++ __tree helpers
constexpr uint32_t IMAGES_FIND = 0x5BEB55;                 // map<string, Image*>::find
constexpr uint32_t IMAGES_AT = 0x5BD3D1;                   // map<string, Image*>::operator[]
constexpr uint32_t IMAGES_DESTROY = 0x5BEC6D;              // __tree<string, Image*>::destroy(node)
constexpr uint32_t HANDLES_AT = 0x5BD455;                  // map<string, unsigned>::operator[]
constexpr uint32_t HANDLES_DESTROY = 0x5BE57F;             // __tree<string, unsigned>::destroy(node)
constexpr uint32_t CALLBACKS_AT = 0x5BD60D;                // map<string, function>::operator[] (const key&)
constexpr uint32_t CALLBACKS_AT_MOVE = 0x5BDB8D;           // map<string, function>::operator[] (key&&)
constexpr uint32_t CALLBACKS_FIND = 0x5BE1E5;              // map<string, function>::find
constexpr uint32_t CALLBACKS_ERASE = 0x5BE16D;             // map<string, function>::erase(iterator)
constexpr uint32_t CALLBACKS_DESTROY = 0x5BECA7;           // __tree<string, function>::destroy(node)
// std::function<void(bool, const char*)>
constexpr uint32_t CALLBACK_COPY = 0x5BD96D;               // copy constructor
constexpr uint32_t CALLBACK_MOVE = 0x5BE2FD;               // move constructor
constexpr uint32_t CALLBACK_CALL = 0x5BDB05;               // operator()(bool, const char*)
constexpr uint32_t CALLBACK_ASSIGN = 0x5BDC19;             // operator=(const function&)

constexpr uint32_t STR_FACEBOOK_PROFILE_IMAGE = 0xE00CE6;  // "facebook_profile_image_%s"
constexpr uint32_t LOAD_LAMBDA_VTABLE = 0xF5CAB4;          // __func<loadImage::$_0> + 8 (DownloadManager callback)
constexpr uint32_t PROFILE_LAMBDA_VTABLE = 0xF5CA88;       // __func<getProfileImageUrl::$_1> + 8

// $_0 / $_1 captures (std::function heap blocks)
constexpr uint32_t PROFILE_CALLBACK = 8;   // $_1: std::function<void(bool, const char*)> callback
constexpr uint32_t PROFILE_ID = 0x20;      // $_1: std::string facebookId
constexpr uint32_t PROFILE_OWNER = 0x2C;   // $_1: ImageDownloader*

// in-order successor of a libc++ __tree node
uint32_t treeNext(uint32_t node) {
    uint32_t right = ld32(node + 4);
    if (right) {
        node = right;
        while (ld32(node)) node = ld32(node);
        return node;
    }
    uint32_t parent;
    for (;;) {
        parent = ld32(node + 8);
        bool left = ld32(parent) == node;
        node = parent;
        if (left) return parent;
    }
}

// std::string copy constructor as libc++ inlines it (a long string is re-initialised, a short one copied)
void copyString(uint32_t dst, uint32_t src) {
    if (ld8(src) & 1) {
        strInit(dst, ld32(src + 8), ld32(src + 4));
    } else {
        st32(dst, ld32(src));
        st32(dst + 4, ld32(src + 4));
        st32(dst + 8, ld32(src + 8));
    }
}
// move the three words of src into dst and leave src empty
void moveWords(uint32_t dst, uint32_t src) {
    uint32_t w0 = ld32(src), w1 = ld32(src + 4), w2 = ld32(src + 8);
    st32(dst, w0);
    st32(dst + 4, w1);
    st32(dst + 8, w2);
    st32(src, 0);
    st32(src + 4, 0);
    st32(src + 8, 0);
}
// std::function copy constructor as inlined: an empty source stays empty, an in-place functor is cloned into the
// destination's buffer (__clone(dst), vtable +0xC), a heap functor is cloned (__clone(), vtable +8)
void copyFunctionInline(uint32_t dst, uint32_t src) {
    uint32_t f = ld32(src + 16);
    if (f == 0) {
        st32(dst + 16, 0);
    } else if (f == src) {
        st32(dst + 16, dst);
        vcall(src, 0xC, {src, dst});
    } else {
        st32(dst + 16, vcall(f, 8, {f}));
    }
}
}  // namespace

ImageDownloader *ImageDownloader::construct(ImageDownloader *self) {
    self->imagesRoot = 0;
    self->imagesSize = 0;
    self->imagesBegin = rw::guest_addr(&self->imagesRoot);
    self->handlesRoot = 0;
    self->handlesSize = 0;
    self->handlesBegin = rw::guest_addr(&self->handlesRoot);
    self->callbacksRoot = 0;
    self->callbacksSize = 0;
    self->facebookIds.cap = 0;
    self->facebookIds.end = 0;
    self->facebookIds.begin = 0;
    self->defaultTimeout = 0;
    self->callbacksBegin = rw::guest_addr(&self->callbacksRoot);
    return self;
}
RW_FUNCTION(0x5BCF7C, "ImageDownloader::ImageDownloader", ImageDownloader::construct);

ImageDownloader *ImageDownloader::destruct(ImageDownloader *self) {
    const uint32_t me = rw::guest_addr(self);
    cancelAllImageHandle(self);
    const uint32_t end = rw::guest_addr(&self->imagesRoot);
    for (uint32_t node = self->imagesBegin; node != end; node = treeNext(node)) {
        uint32_t image = ld32(node + NODE_VALUE);
        if (image) call(REF_RELEASE, {image});
    }
    call(IMAGES_DESTROY, {me, self->imagesRoot});
    self->imagesBegin = end;
    self->imagesRoot = 0;
    self->imagesSize = 0;
    call(CALLBACKS_DESTROY, {rw::guest_addr(&self->callbacksBegin), self->callbacksRoot});
    call(STRING_VECTOR_DTOR, {rw::guest_addr(&self->facebookIds)});
    call(HANDLES_DESTROY, {rw::guest_addr(&self->handlesBegin), self->handlesRoot});
    call(IMAGES_DESTROY, {me, self->imagesRoot});
    return self;
}
RW_FUNCTION(0x5BCFA8, "ImageDownloader::~ImageDownloader", ImageDownloader::destruct);

// Cancel every running download and forget the handles.
void ImageDownloader::cancelAllImageHandle(ImageDownloader *self) {
    const uint32_t end = rw::guest_addr(&self->handlesRoot);
    for (uint32_t node = self->handlesBegin; node != end; node = treeNext(node)) {
        uint32_t manager = call(DOWNLOAD_MANAGER_GET_INSTANCE, {});
        call(DOWNLOAD_MANAGER_CANCEL, {manager, ld32(node + NODE_VALUE)});
    }
    call(HANDLES_DESTROY, {rw::guest_addr(&self->handlesBegin), self->handlesRoot});
    self->handlesBegin = end;
    self->handlesRoot = 0;
    self->handlesSize = 0;
}
RW_FUNCTION(0x5BD06C, "ImageDownloader::cancelAllImageHandle", ImageDownloader::cancelAllImageHandle);

uint32_t ImageDownloader::loadImageDefault(ImageDownloader *self, uint32_t view, uint32_t url, uint32_t name) {
    return call(0x5BD0DD, {rw::guest_addr(self), view, url, name, uint32_t(self->defaultTimeout)});   // loadImage(..., long)
}
RW_FUNCTION(0x5BD0CA, "ImageDownloader::loadImage(ImageView*, const char*, const char*)", ImageDownloader::loadImageDefault);

// Show the image at `url` in `view`: from the cache when it was downloaded already (returns 1), else start a
// download named "facebook_profile_image_<name>" whose callback ($_0) fills the cache and the view (returns 0).
uint32_t ImageDownloader::loadImage(ImageDownloader *self, uint32_t view, uint32_t url, uint32_t name,
                                    int32_t timeout) {
    const uint32_t me = rw::guest_addr(self);
    Frame f;
    const uint32_t key = f.alloc(12), lookup = f.alloc(12);
    if (timeout < 0) timeout = self->defaultTimeout;
    strInit(key, url, call(STRLEN, {url}));
    strInit(lookup, url, call(STRLEN, {url}));
    uint32_t node = call(IMAGES_FIND, {me, lookup});
    strFree(lookup);
    uint32_t result;
    if (node != rw::guest_addr(&self->imagesRoot)) {
        result = 1;
        uint32_t image = ld32(node + NODE_VALUE);
        if (image) {
            uint32_t texture = call(fn::operator_new, {0x40});
            call(TEXTURE2D_CTOR, {texture});
            if (call(TEXTURE2D_INIT_WITH_IMAGE, {texture, ld32(node + NODE_VALUE)}) == 1)
                call(IMAGEVIEW_LOAD_TEXTURE, {view, texture});
            call(REF_RELEASE, {texture});
        }
    } else {
        // the lambda's captures {this, url, view}, built on the stack and copied once more like the original
        const uint32_t captures = f.alloc(20), moved = f.alloc(20);
        const uint32_t callback = f.alloc(20);     // std::function<bool(const shared_ptr<bg::Data>&)>
        const uint32_t fileName = f.alloc(0x410);  // StringBuffer (its characters first)
        st32(captures, me);
        copyString(captures + 4, key);
        st32(captures + 16, view);
        call(STRINGBUFFER_CTOR, {fileName, STR_FACEBOOK_PROFILE_IMAGE, name});
        call(STRING_ASSIGN, {key, fileName, call(STRLEN, {fileName})});
        st32(call(IMAGES_AT, {me, key}), 0);   // reserve the name: no image yet
        uint32_t manager = call(DOWNLOAD_MANAGER_GET_INSTANCE, {});
        st32(moved, ld32(captures));
        copyString(moved + 4, captures + 4);
        st32(moved + 16, view);
        st32(callback + 16, 0);
        uint32_t functor = call(fn::operator_new, {0x18});
        if (functor) {
            st32(functor, LOAD_LAMBDA_VTABLE);
            st32(functor + 4, ld32(moved));
            moveWords(functor + 8, moved + 4);
            st32(functor + 0x14, view);
        }
        st32(callback + 16, functor);
        uint32_t handle = call(DOWNLOAD_MANAGER_START, {manager, url, callback, uint32_t(timeout), fileName});
        st32(call(HANDLES_AT, {rw::guest_addr(&self->handlesBegin), key}), handle);
        functionDestroy(callback);
        strFree(moved + 4);
        strFree(captures + 4);
        result = 0;
    }
    strFree(key);
    return result;
}
RW_FUNCTION(0x5BD0DC, "ImageDownloader::loadImage(ImageView*, const char*, const char*, long)", ImageDownloader::loadImage);

// Ask Facebook for the profile image URL of `facebookId`; the answer goes to $_1 (below), then to `callback`.
void ImageDownloader::getProfileImageUrl(ImageDownloader *self, uint32_t facebookId, uint32_t callback) {
    Frame f;
    const uint32_t id = f.alloc(12), idArg = f.alloc(12);
    const uint32_t captures = f.alloc(0x28);   // $_1 captures: callback (+0), id (+0x18), this (+0x24)
    const uint32_t moved = f.alloc(0x28);
    const uint32_t handler = f.alloc(20);      // std::function<void(bool, ..., string, string)>
    strInit(id, facebookId, call(STRLEN, {facebookId}));
    call(CALLBACK_COPY, {captures, callback});
    copyString(captures + 0x18, id);
    st32(captures + 0x24, rw::guest_addr(self));
    uint32_t facebook = call(FACEBOOK_GET_INSTANCE, {});
    strInit(idArg, facebookId, call(STRLEN, {facebookId}));
    copyFunctionInline(moved, captures);
    copyString(moved + 0x18, captures + 0x18);
    st32(moved + 0x24, ld32(captures + 0x24));
    st32(handler + 16, 0);
    uint32_t functor = call(fn::operator_new, {0x30});
    if (functor) {
        st32(functor, PROFILE_LAMBDA_VTABLE);
        call(CALLBACK_MOVE, {functor + PROFILE_CALLBACK, moved});
        moveWords(functor + PROFILE_ID, moved + 0x18);
        st32(functor + PROFILE_OWNER, ld32(moved + 0x24));
    }
    st32(handler + 16, functor);
    call(FACEBOOK_GET_PROFILE_IMAGE_URL, {facebook, idArg, handler});
    functionDestroy(handler);
    strFree(moved + 0x18);
    functionDestroy(moved);
    strFree(idArg);
    strFree(captures + 0x18);
    functionDestroy(captures);
    strFree(id);
}
RW_FUNCTION(0x5BD6C0, "ImageDownloader::getProfileImageUrl", ImageDownloader::getProfileImageUrl);

// Queue a Facebook ID for its profile image (once) and remember its callback; not logged in: callback(false, null).
void ImageDownloader::addFacebookID(ImageDownloader *self, uint32_t facebookId, uint32_t callback) {
    if (!call(SCENEUTIL_IS_LOGGED_IN_FACEBOOK, {})) {
        call(CALLBACK_CALL, {callback, 0, 0});
        return;
    }
    uint32_t it = self->facebookIds.begin;
    uint32_t end = self->facebookIds.end;
    while (it != end) {
        if (call(STRING_COMPARE, {it, facebookId}) == 0) break;
        it += 12;
    }
    if (it != self->facebookIds.end) return;   // already queued
    Frame f;
    const uint32_t id = f.alloc(12), key = f.alloc(12);
    strInit(id, facebookId, call(STRLEN, {facebookId}));
    uint32_t last = self->facebookIds.end;
    if (last < self->facebookIds.cap) {
        uint32_t next = 0;
        if (last) {
            moveWords(last, id);
            next = self->facebookIds.end;
        }
        self->facebookIds.end = next + 12;
    } else {
        call(STRING_VECTOR_PUSH_BACK_SLOW, {rw::guest_addr(&self->facebookIds), id});
    }
    strFree(id);
    strInit(key, facebookId, call(STRLEN, {facebookId}));
    uint32_t slot = call(CALLBACKS_AT_MOVE, {rw::guest_addr(&self->callbacksBegin), key});
    call(CALLBACK_ASSIGN, {slot, callback});
    strFree(key);
}
RW_FUNCTION(0x5BD9A0, "ImageDownloader::addFacebookID", ImageDownloader::addFacebookID);

// Every frame: when logged in to Facebook and idle, request the profile image URL of the first queued ID.
void ImageDownloader::update(ImageDownloader *self) {
    if (call(SCENEUTIL_IS_LOGGED_IN_FACEBOOK, {}) != 1) return;
    if (call(FACEBOOK_IS_CONNECTING, {call(FACEBOOK_GET_INSTANCE, {})})) return;
    uint32_t front = self->facebookIds.begin;
    if (self->facebookIds.end == front) return;
    Frame f;
    const uint32_t key = f.alloc(12), callback = f.alloc(20);
    uint32_t id = strData(front);
    strInit(key, id, call(STRLEN, {id}));
    uint32_t slot = call(CALLBACKS_AT, {rw::guest_addr(&self->callbacksBegin), key});
    strFree(key);
    id = strData(front);
    call(CALLBACK_COPY, {callback, slot});
    getProfileImageUrl(self, id, callback);
    functionDestroy(callback);
}
RW_FUNCTION(0x5BD4D8, "ImageDownloader::update", ImageDownloader::update);

// ------------------------------------------------------------------------------------------------ lambdas
namespace {
// getProfileImageUrl::$_1 __clone(): a copy of the captures in a new block
uint32_t profileLambdaClone(uint32_t self) {
    uint32_t copy = call(fn::operator_new, {0x30});
    if (copy) {
        st32(copy, PROFILE_LAMBDA_VTABLE);
        call(CALLBACK_COPY, {copy + PROFILE_CALLBACK, self + PROFILE_CALLBACK});
        copyString(copy + PROFILE_ID, self + PROFILE_ID);
        st32(copy + PROFILE_OWNER, ld32(self + PROFILE_OWNER));
    }
    return copy;
}
// getProfileImageUrl::$_1 __clone(__base*): the same into a given block
void profileLambdaCloneInto(uint32_t self, uint32_t copy) {
    if (!copy) return;
    st32(copy, PROFILE_LAMBDA_VTABLE);
    call(CALLBACK_COPY, {copy + PROFILE_CALLBACK, self + PROFILE_CALLBACK});
    copyString(copy + PROFILE_ID, self + PROFILE_ID);
    st32(copy + PROFILE_OWNER, ld32(self + PROFILE_OWNER));
}

// getProfileImageUrl::$_1 operator()(bool ok, <unused>, std::string, std::string url): on success remember the
// face image URL (SceneManager), hand it to the callback, and drop the ID from the queue and the callbacks;
// on failure callback(false, null).
void profileLambdaCall(uint32_t self, const uint8_t *ok, uint32_t, uint32_t message, uint32_t urlArg) {
    Frame f;
    const uint32_t url = f.alloc(12);
    // the strings are taken by value: `message` is moved into a temporary that is only destroyed
    uint32_t messageData = ld32(message + 8);
    uint8_t messageFlags = ld8(message);
    st32(message, 0);
    st32(message + 4, 0);
    st32(message + 8, 0);
    moveWords(url, urlArg);
    const uint32_t callback = self + PROFILE_CALLBACK;
    if (!*ok) {
        call(CALLBACK_CALL, {callback, 0, 0});
    } else {
        auto *owner = rw::host_ptr<ImageDownloader>(ld32(self + PROFILE_OWNER));
        const uint32_t id = self + PROFILE_ID;
        call(SCENE_MANAGER_ADD_FACE_IMAGE_URL, {call(SCENE_MANAGER_GET_INSTANCE, {}), id, url});
        call(CALLBACK_CALL, {callback, 1, strIsLong(url) ? ld32(url + 8) : (url | 1)});
        // facebookIds.erase(find(facebookIds, id))
        const uint32_t begin = owner->facebookIds.begin;
        uint32_t end = owner->facebookIds.end;
        uint32_t it = begin;
        if (it != end) {
            const bool idLong = ld8(id) & 1;
            const uint32_t idData = idLong ? ld32(id + 8) : id + 1;
            const uint32_t idSize = idLong ? ld32(id + 4) : uint32_t(ld8(id) >> 1);
            for (; it != end; it += 12) {
                const bool isLong = ld8(it) & 1;
                const uint32_t size = isLong ? ld32(it + 4) : uint32_t(ld8(it) >> 1);
                if (size != idSize) continue;
                if (isLong) {
                    if (call(MEMCMP, {ld32(it + 8), idData, idSize}) == 0) break;
                    continue;
                }
                uint32_t k = 0;
                while (k < idSize && ld8(it + 1 + k) == ld8(idData + k)) k++;
                if (k == idSize) break;
            }
        }
        if (it != end) {
            const uint32_t offset = it - begin;
            const uint32_t index = uint32_t(int32_t(offset) >> 2) * 0xAAAAAAABu;   // offset / 12
            uint32_t pos = begin + (offset & ~3u);
            if (pos + 12 != end) {
                // move the following strings down by one (string move assignment, inlined)
                uint32_t p = begin + index * 12 + 8;   // points at the destination's data word
                const uint32_t stop = 12 - end;
                do {
                    const uint32_t dst = p - 8;
                    if (ld8(dst) & 1) {
                        st8(ld32(p), 0);
                        st32(p - 4, 0);
                    } else {
                        st8(dst + 1, 0);
                        st8(dst, 0);
                    }
                    call(STRING_RESERVE, {dst, 0});
                    moveWords(dst, p + 4);
                    p += 12;
                } while (p + stop != 8);
                pos = p - 8;
                end = owner->facebookIds.end;
            }
            // destroy the strings past the new end
            while (end != pos) {
                uint32_t last = end - 12;
                owner->facebookIds.end = last;
                if (ld8(end - 12) & 1) {
                    guest::deleteObject(ld32(end - 4));
                    last = owner->facebookIds.end;
                }
                end = last;
            }
        }
        const uint32_t callbacks = rw::guest_addr(&owner->callbacksBegin);
        uint32_t node = call(CALLBACKS_FIND, {callbacks, id});
        if (node != rw::guest_addr(&owner->callbacksRoot)) call(CALLBACKS_ERASE, {callbacks, node});
    }
    strFree(url);
    if (messageFlags & 1) guest::deleteObject(messageData);
}
}  // namespace
RW_FUNCTION(0x5BDD24, "ImageDownloader::getProfileImageUrl $_1 __func::__clone()", profileLambdaClone);
RW_FUNCTION(0x5BDDCC, "ImageDownloader::getProfileImageUrl $_1 __func::__clone(__base*)", profileLambdaCloneInto);
RW_FUNCTION(0x5BDECC, "ImageDownloader::getProfileImageUrl $_1 __func::operator()", profileLambdaCall);
