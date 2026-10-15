// GLBufferedNode: a mixin owning up to 4 OpenGL vertex buffers and 4 index buffers (cocos2d-x's
// extension/GUI/CCControlExtension's GLBufferedNode). A buffer is reallocated only when the new data is larger
// than its current capacity; otherwise the data is uploaded with glBufferSubData.
#pragma once

#include "rw/rw.hpp"

struct GLBufferedNode {
    rw::Field<uint32_t> vtable;              // +0
    rw::Field<uint32_t> bufferObject[4];     // +4  GL vertex buffer names
    rw::Field<uint32_t> bufferSize[4];       // +20 their capacities (bytes)
    rw::Field<uint32_t> indexBufferObject[4];  // +36 GL index buffer names
    rw::Field<uint32_t> indexBufferSize[4];  // +52 their capacities (bytes)

    static constexpr uint32_t VTABLE = 0xFBCD80;  // _ZTV14GLBufferedNode + 8

    static GLBufferedNode *construct(GLBufferedNode *self);
    static GLBufferedNode *destruct(GLBufferedNode *self);
    static void destructDelete(GLBufferedNode *self);
    static void setGLBufferData(GLBufferedNode *self, uint32_t buf, uint32_t bufSize, int32_t slot);
    static void setGLIndexData(GLBufferedNode *self, uint32_t buf, uint32_t bufSize, int32_t slot);
};
RW_LAYOUT(GLBufferedNode, bufferObject, 4);
RW_LAYOUT(GLBufferedNode, bufferSize, 20);
RW_LAYOUT(GLBufferedNode, indexBufferObject, 36);
RW_LAYOUT(GLBufferedNode, indexBufferSize, 52);
RW_SIZE(GLBufferedNode, 68);
