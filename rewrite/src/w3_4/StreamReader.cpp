// StreamReader: the SQEX streaming-audio file reader (an IStreamReader over an SQEX::File). Layout: vtable +0,
// second vtable +4 (the SQEX::Object base), file +8, a buffer allocated with SQEX::Memory +12.
#include "core/Guest.hpp"

struct StreamReader {
    rw::Field<uint32_t> vtable;        // +0
    rw::Field<uint32_t> vtable2;       // +4
    rw::Field<uint32_t> file;          // +8  SQEX::File*
    rw::Field<uint32_t> buffer;        // +12 SQEX::Memory block (freed by the destructor)

    static StreamReader *destructor(StreamReader *self);
    static StreamReader *deletingDestructor(StreamReader *self);
    static uint32_t ReadRequest(StreamReader *self, const uint32_t *param);
};
RW_LAYOUT(StreamReader, buffer, 12);

namespace {
constexpr uint32_t MEMORY_FREE = 0xCE43C0;      // SQEX::Memory::Free(void*) (ARM)
constexpr uint32_t FILE_SEEK = 0xCE9B8C;        // SQEX::File::Seek(file, int offset, int whence) (ARM)
constexpr uint32_t FILE_READ = 0xCE9B58;        // SQEX::File::Read(file, void* dst, unsigned size) (ARM)
constexpr uint32_t VT_STREAM_READER = 0xFD5A00; // _ZTV12StreamReader
constexpr uint32_t VT_ISTREAM_READER = 0xFD59D8;   // _ZTV13IStreamReader
constexpr uint32_t VT_SQEX_OBJECT = 0xFD59B8;   // _ZTVN4SQEX6ObjectE
}  // namespace

StreamReader *StreamReader::destructor(StreamReader *self) {
    uint32_t buffer = self->buffer;
    self->vtable = VT_STREAM_READER + 8;
    self->vtable2 = VT_STREAM_READER + 0x1C;
    if (buffer) rw::call(MEMORY_FREE, {buffer});
    // base destructors: SQEX::Object, IStreamReader
    self->vtable2 = VT_SQEX_OBJECT + 8;
    self->vtable = VT_ISTREAM_READER + 8;
    return self;
}
RW_FUNCTION(0xCDB820, "StreamReader::~StreamReader()", StreamReader::destructor);

StreamReader *StreamReader::deletingDestructor(StreamReader *self) {
    destructor(self);
    rw::call(MEMORY_FREE, {rw::guest_addr(self)});
    return self;
}
RW_FUNCTION(0xCDB888, "StreamReader::~StreamReader() (deleting)", StreamReader::deletingDestructor);

// tagSTREAMING_READ_REQUEST_PARAM: dst +0, size +4, file offset +8, two counters +12 / +16. Declines (0) while
// the first counter is over half the second; otherwise seeks and reads (returns File::Read's result).
uint32_t StreamReader::ReadRequest(StreamReader *self, const uint32_t *param) {
    float ratio = float(int32_t(param[3])) / float(int32_t(param[4]));
    if (!(ratio <= 0.5f)) return 0;
    rw::call(FILE_SEEK, {self->file, param[2], 0});
    return rw::call(FILE_READ, {self->file, param[0], param[1]});
}
RW_FUNCTION(0xCDB948, "StreamReader::ReadRequest(const tagSTREAMING_READ_REQUEST_PARAM*)", StreamReader::ReadRequest);
