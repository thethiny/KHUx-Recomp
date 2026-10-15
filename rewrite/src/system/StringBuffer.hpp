// StringBuffer: a fixed 1 KB printf-style text buffer the game builds file names, URLs and labels in.
#pragma once

#include "SystemCommon.hpp"

struct StringBuffer {
    const uint8_t text[1024];             // +0 NUL-terminated
    rw::Field<int32_t> length;      // +1024

    static constexpr uint32_t CAPACITY = 1024;

    // StringBuffer(fmt, ...)
    static StringBuffer *construct(StringBuffer *self, uint32_t fmt);
    static StringBuffer *vfset(StringBuffer *self, uint32_t fmt, uint32_t va);
    static StringBuffer *set(StringBuffer *self, uint32_t str);
    // Append str (truncated to the capacity).
    static StringBuffer *concat(StringBuffer *self, uint32_t str);
    static StringBuffer *fset(StringBuffer *self, uint32_t fmt);
    static StringBuffer *fconcat(StringBuffer *self, uint32_t fmt);
    static StringBuffer *vfconcat(StringBuffer *self, uint32_t fmt, uint32_t va);
    // text[index] = c
    static void replace(StringBuffer *self, uint32_t index, uint32_t c);
};
RW_LAYOUT(StringBuffer, length, 1024);
RW_SIZE(StringBuffer, 1028);
