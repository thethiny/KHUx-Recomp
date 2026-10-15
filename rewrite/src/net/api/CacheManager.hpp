// hole::network::CacheManager: the on-disk cache of downloaded files. A URL maps to a file in the writable path
// whose name is the URL's bytes XORed with an LCG key stream (seed 0x6D5379, x = x * 214013 + 2531011 before
// each byte), written as lowercase hex.
#pragma once

#include "Common.hpp"

namespace hole::network {

struct CacheManager {
    // the singleton (an empty object in .bss)
    static constexpr uint32_t INSTANCE = 0x10038D0;

    static uint32_t getInstance();
    // *out = FileUtils::getWritablePath() + hex(url ^ keystream)   (out: a std::string, built in place)
    static uint32_t convertURLToCacheFilePath(api::StdString *out, uint32_t self, const api::StdString *url);
    // *out = the cached file's bytes as a shared_ptr<bg::Data> when it is `size` bytes long and its md5 (a lowercase
    // hex std::string) equals *md5; else an empty shared_ptr
    static uint32_t validate(api::SharedPtr *out, uint32_t self, const api::StdString *url, uint32_t size,
                             const api::StdString *md5);
    // remove() of the cache file
    static uint32_t removeCache(uint32_t self, const api::StdString *url);
};

}  // namespace hole::network
