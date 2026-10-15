// ImageDownloader: downloads remote images (profile pictures) and shows them in ui::ImageViews. Images already
// downloaded are kept by URL; running downloads are kept by name so they can be cancelled. Facebook profile
// pictures are looked up by Facebook ID (a queue of IDs; one request at a time from update()).
#pragma once

#include "core/Guest.hpp"

struct ImageDownloader {
    // std::map<std::string url, cocos2d::Image*> (libc++ __tree: begin node, root (= end node's left), size);
    // a node's value (Image*) is at node + 0x1C
    rw::Field<uint32_t> imagesBegin;     // +0x00
    rw::Field<uint32_t> imagesRoot;      // +0x04 (its address is the end node)
    rw::Field<uint32_t> imagesSize;      // +0x08
    // std::map<std::string name, unsigned DownloadManager handle>; value at node + 0x1C
    rw::Field<uint32_t> handlesBegin;    // +0x0C
    rw::Field<uint32_t> handlesRoot;     // +0x10
    rw::Field<uint32_t> handlesSize;     // +0x14
    rw::Field<int32_t> defaultTimeout;   // +0x18 loadImage's timeout when the caller passes a negative one
    // std::vector<std::string>: Facebook IDs waiting for their profile image URL
    guest::Vector facebookIds;           // +0x1C
    // std::map<std::string facebookId, std::function<void(bool, const char* url)>> callbacks
    rw::Field<uint32_t> callbacksBegin;  // +0x28
    rw::Field<uint32_t> callbacksRoot;   // +0x2C
    rw::Field<uint32_t> callbacksSize;   // +0x30

    static constexpr uint32_t NODE_VALUE = 0x1C;   // offset of the mapped value in a map node (key string at +0x10)

    static ImageDownloader *construct(ImageDownloader *self);
    static ImageDownloader *destruct(ImageDownloader *self);
    static void cancelAllImageHandle(ImageDownloader *self);
    static uint32_t loadImageDefault(ImageDownloader *self, uint32_t view, uint32_t url, uint32_t name);
    static uint32_t loadImage(ImageDownloader *self, uint32_t view, uint32_t url, uint32_t name, int32_t timeout);
    static void getProfileImageUrl(ImageDownloader *self, uint32_t facebookId, uint32_t callback);
    static void addFacebookID(ImageDownloader *self, uint32_t facebookId, uint32_t callback);
    static void update(ImageDownloader *self);
};
RW_LAYOUT(ImageDownloader, handlesBegin, 0x0C);
RW_LAYOUT(ImageDownloader, defaultTimeout, 0x18);
RW_LAYOUT(ImageDownloader, facebookIds, 0x1C);
RW_LAYOUT(ImageDownloader, callbacksBegin, 0x28);
RW_SIZE(ImageDownloader, 0x34);
