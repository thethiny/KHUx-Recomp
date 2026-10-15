#include "GLBufferedNode.hpp"

#include "../core/Guest.hpp"

namespace {
namespace gl {
// OpenGL ES imports (PLT stubs, as the lifted code calls them)
constexpr uint32_t DeleteBuffers = 0x43667C;   // glDeleteBuffers(n, names)
constexpr uint32_t GenBuffers = 0x4366AC;      // glGenBuffers(n, names)
constexpr uint32_t BindBuffer = 0x4366B8;      // glBindBuffer(target, name)
constexpr uint32_t BufferData = 0x4366C4;      // glBufferData(target, size, data, usage)
constexpr uint32_t BufferSubData = 0x436EA4;   // glBufferSubData(target, offset, size, data)
constexpr uint32_t ARRAY_BUFFER = 0x8892;
constexpr uint32_t ELEMENT_ARRAY_BUFFER = 0x8893;
constexpr uint32_t DYNAMIC_DRAW = 0x88E8;
}  // namespace gl

// Upload `size` bytes to the buffer (name, capacity): grow it (delete + generate + glBufferData) when too small,
// else overwrite its start with glBufferSubData.
void upload(rw::Field<uint32_t> &name, rw::Field<uint32_t> &capacity, uint32_t target, uint32_t data,
            uint32_t size) {
    uint32_t current = name;
    if (capacity.get() >= size) {
        rw::call(gl::BindBuffer, {target, current});
        rw::call(gl::BufferSubData, {target, 0, size, data});
        return;
    }
    if (current != 0) rw::call(gl::DeleteBuffers, {1, rw::guest_addr(&name)});
    rw::call(gl::GenBuffers, {1, rw::guest_addr(&name)});
    capacity = size;
    rw::call(gl::BindBuffer, {target, name.get()});
    rw::call(gl::BufferData, {target, size, data, gl::DYNAMIC_DRAW});
}
}  // namespace

GLBufferedNode *GLBufferedNode::construct(GLBufferedNode *self) {
    self->vtable = VTABLE;
    for (int k = 0; k < 4; k++) {
        self->bufferObject[k] = 0;
        self->bufferSize[k] = 0;
        self->indexBufferObject[k] = 0;
        self->indexBufferSize[k] = 0;
    }
    return self;
}
RW_FUNCTION(0x882154, "GLBufferedNode::GLBufferedNode", GLBufferedNode::construct);

// Deletes every buffer that was ever allocated (non-zero capacity). OpenGL calls: not shadow-run.
GLBufferedNode *GLBufferedNode::destruct(GLBufferedNode *self) {
    self->vtable = VTABLE;
    for (int k = 0; k < 4; k++) {
        if (self->bufferSize[k] != 0) rw::call(gl::DeleteBuffers, {1, rw::guest_addr(&self->bufferObject[k])});
        if (self->indexBufferSize[k] != 0)
            rw::call(gl::DeleteBuffers, {1, rw::guest_addr(&self->indexBufferObject[k])});
    }
    return self;
}
RW_FUNCTION_NOVERIFY(0x882188, "GLBufferedNode::~GLBufferedNode", GLBufferedNode::destruct);

void GLBufferedNode::destructDelete(GLBufferedNode *self) {
    destruct(self);
    guest::deleteObject(rw::guest_addr(self));
}
RW_FUNCTION_NOVERIFY(0x8821D8, "GLBufferedNode::~GLBufferedNode(deleting)", GLBufferedNode::destructDelete);

void GLBufferedNode::setGLBufferData(GLBufferedNode *self, uint32_t buf, uint32_t bufSize, int32_t slot) {
    upload(self->bufferObject[slot], self->bufferSize[slot], gl::ARRAY_BUFFER, buf, bufSize);
}
RW_FUNCTION_NOVERIFY(0x8821E4, "GLBufferedNode::setGLBufferData", GLBufferedNode::setGLBufferData);

void GLBufferedNode::setGLIndexData(GLBufferedNode *self, uint32_t buf, uint32_t bufSize, int32_t slot) {
    upload(self->indexBufferObject[slot], self->indexBufferSize[slot], gl::ELEMENT_ARRAY_BUFFER, buf, bufSize);
}
RW_FUNCTION_NOVERIFY(0x882264, "GLBufferedNode::setGLIndexData", GLBufferedNode::setGLIndexData);
