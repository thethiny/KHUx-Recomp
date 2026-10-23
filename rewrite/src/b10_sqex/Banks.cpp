// Batch #10: SQEX banks - BankImpl's sound creation with an external id / a 3D layout and its sound list,
// OnMemoryBank's core creators (straight to SoundSystem on the bank's data) and StreamingBank (a file streamed
// through a 32 KB read buffer: every sound gets its own reader, kept with the sound in the bank's stream list).
//
// BankImpl: vtable +0, id +4, sound list {head +8, tail +0xC, count +0x10}, removing u8 +0x14, data +0x18
// (OnMemoryBank). BankImpl list node (0x10): {vtable, prev, next, Sound handle +0xC}.
// StreamingBank (0x34): BankImpl, SQEX::File +0x18, stream list {head +0x24, tail +0x28, count +0x2C}.
// Stream list node (0x10): {vtable, prev, next, entry +0xC}; entry (0xC): {vtable, reader +4, Sound +8}.
// Reader (0x14): {vtable, IStreamReader vtable +4, File* +8, buffer +0xC, buffer size +0x10}.
#include "../w4_b2/Sqex.hpp"

using namespace b2;

namespace sqex {
namespace {

constexpr uint32_t VT_READER_GOT = 0xFD926C;          // StreamingBank's reader (+8 / +0x1C: its two vtables)
constexpr uint32_t VT_STREAM_ENTRY_GOT = 0xFD9270;
constexpr uint32_t VT_STREAM_NODE_GOT = 0xFD9274;
constexpr uint32_t VT_STREAMING_BANK_GOT = 0xFD9278;
constexpr uint32_t VT_SOUND_NODE_GOT = 0xFD9420;      // BankImpl's sound list node
constexpr uint32_t READ_BUFFER_SIZE = 0x8000;

constexpr uint32_t SOUND_DEFAULT_CTOR = 0xCE1328, SOUND_GET_STATUS = 0xCE14FC, SOUND_PRINT_DEBUG_INFO = 0xCE15C0;
constexpr uint32_t BANK_IMPL_CTOR = 0xCE960C, BANK_IMPL_DTOR = 0xCE953C, BANK_IMPL_UPDATE = 0xCE9454;
constexpr uint32_t FILE_CTOR = 0xCE9A44, FILE_DTOR = 0xCE9A94;
// SoundSystem's creators: (Sound*, data, index[, external id]) / (Sound*, reader, buffer, size[, external id])
constexpr uint32_t SS_CREATE_SOUND_EXT = 0xCE0A40, SS_CREATE_LAYOUT = 0xCE0A64, SS_CREATE_LAYOUT_EXT = 0xCE0A84,
                   SS_CREATE_STREAMING = 0xCE0AA4, SS_CREATE_STREAMING_EXT = 0xCE0AC8,
                   SS_CREATE_STREAMING_LAYOUT = 0xCE0AEC, SS_CREATE_STREAMING_LAYOUT_EXT = 0xCE0B10;
constexpr uint32_t S_READ_BUFFER_FAILED = 0xEECFFC;   // "create stream read buffer failed!! heap memory insufficient"
constexpr uint32_t S_BANK_INFO = 0xEEE21C;            // "--bank[%d][num sound:%d]--"

// appends `node` to the list {head, tail, count} at `list` (node prev +4, next +8)
void listAppend(uint32_t list, uint32_t node) {
    if (ld32(list) == 0) {
        st32(list + 4, node);
        st32(list, node);
        st32(node + 8, 0);
        st32(node + 4, 0);
    } else {
        const uint32_t tail = ld32(list + 4);
        st32(node + 4, tail);
        st32(node + 8, 0);
        st32(tail + 8, node);
        st32(list + 4, node);
    }
    st32(list + 8, ld32(list + 8) + 1);
}
// unlinks `node` from that list and counts it out
void listRemove(uint32_t list, uint32_t node) {
    const uint32_t prev = ld32(node + 4), next = ld32(node + 8);
    if (prev != 0) st32(prev + 8, next);
    else st32(list, next);
    if (next != 0) st32(next + 4, prev);
    else st32(list + 4, prev);
    st32(list + 8, ld32(list + 8) - 1);
}
void deleteObject(uint32_t obj) { vcallo(obj, 4, {obj}); }   // the deleting destructor

// BankImpl::Create*: the bank's core creator (vtable slot) fills a Sound; a new list node keeps its handle
template <class... A> uint32_t bankCreate(uint32_t bank, uint32_t slot, A... args) {
    guest::Frame f(8);
    rw::call(SOUND_DEFAULT_CTOR, {f.addr(4)});
    if (vcallo(bank, slot, {bank, f.addr(4), uint32_t(args)...}) != 0) return 0;
    const uint32_t handle = *f.at<uint32_t>(4);
    const uint32_t node = malloc_(0x10, 4);
    st32(node + 4, 0);
    st32(node + 8, 0);
    st32(node + 0xC, 0);
    st32(node, ld32(VT_SOUND_NODE_GOT) + 8);
    rw::call(SOUND_DEFAULT_CTOR, {node + 0xC});
    listAppend(bank + 8, node);
    st32(node + 0xC, handle);
    return handle;
}

// OnMemoryBank: SoundSystem on the bank's data; a negative char result becomes -1, anything else 0
template <class... A> int32_t onMemoryCreate(uint32_t creator, uint32_t bank, uint32_t sound, A... args) {
    return (rw::call(creator, {sound, ld32(bank + 0x18), uint32_t(args)...}) & 0x80) ? -1 : 0;
}

// StreamingBank: a reader over the bank's file with its own read buffer, handed to SoundSystem; on success the
// sound and its reader join the stream list (0), else the reader is deleted (-1)
template <class... A> int32_t streamingCreate(uint32_t creator, uint32_t bank, uint32_t sound, A... extra) {
    const uint32_t reader = malloc_(0x14, 4);
    const uint32_t vt = ld32(VT_READER_GOT);
    st32(reader + 0xC, 0);
    st32(reader + 0x10, READ_BUFFER_SIZE);
    st32(reader, vt + 8);
    st32(reader + 4, vt + 0x1C);
    st32(reader + 8, bank + 0x18);
    const uint32_t buffer = malloc_(READ_BUFFER_SIZE, 4);
    st32(reader + 0xC, buffer);
    if (buffer == 0) log(5, S_READ_BUFFER_FAILED);
    if (int32_t(rw::call(creator, {sound, reader, ld32(reader + 0xC), ld32(reader + 0x10), uint32_t(extra)...})) < 0) {
        deleteObject(reader);
        return -1;
    }
    const uint32_t entry = malloc_(0xC, 4);
    st32(entry + 4, 0);
    st32(entry + 8, 0);
    st32(entry, ld32(VT_STREAM_ENTRY_GOT) + 8);
    rw::call(SOUND_DEFAULT_CTOR, {entry + 8});
    st32(entry + 4, reader);
    st32(entry + 8, ld32(sound));
    const uint32_t node = malloc_(0x10, 4);
    st32(node + 4, 0);
    st32(node + 8, 0);
    st32(node + 0xC, 0);
    st32(node, ld32(VT_STREAM_NODE_GOT) + 8);
    listAppend(bank + 0x24, node);
    st32(node + 0xC, entry);
    return 0;
}

// a stream list node's entry: its reader (if any) and the entry itself deleted
void deleteEntry(uint32_t node) {
    uint32_t entry = ld32(node + 0xC);
    const uint32_t reader = ld32(entry + 4);
    if (reader != 0) {
        deleteObject(reader);
        entry = ld32(node + 0xC);
        if (entry == 0) return;
    }
    deleteObject(entry);
}

}  // namespace

struct Banks {
    static uint32_t createSoundWithExternalId(uint32_t bank, uint32_t index, uint32_t externalId) {
        return bankCreate(bank, 0x1C, index, uint32_t(int32_t(int16_t(externalId))));
    }
    static uint32_t createLayoutSound(uint32_t bank, uint32_t index) { return bankCreate(bank, 0x20, index); }
    static uint32_t createLayoutSoundWithExternalId(uint32_t bank, uint32_t index, uint32_t externalId) {
        return bankCreate(bank, 0x24, index, uint32_t(int32_t(int16_t(externalId))));
    }
    static uint32_t getIsBelongSound(uint32_t bank, uint32_t handle) {
        for (uint32_t node = ld32(bank + 8); node != 0; node = ld32(node + 8))
            if (ld32(node + 0xC) == handle) return 1;
        return 0;
    }
    static void printDebugInfo(uint32_t bank) {
        rw::call(ANDROID_LOG, {4, LOG_TAG, S_BANK_INFO, ld32(bank + 4), ld32(bank + 0x10)});
        guest::Frame f(8);
        for (uint32_t node = ld32(bank + 8); node != 0; node = ld32(node + 8)) {
            *f.at<uint32_t>(4) = ld32(node + 0xC);
            rw::call(SOUND_PRINT_DEBUG_INFO, {f.addr(4)});
        }
    }

    static int32_t onMemoryCreateSoundCoreWithExternalId(uint32_t bank, uint32_t sound, uint32_t index,
                                                         uint32_t externalId) {
        return onMemoryCreate(SS_CREATE_SOUND_EXT, bank, sound, index, externalId);
    }
    static int32_t onMemoryCreateLayoutSoundCore(uint32_t bank, uint32_t sound, uint32_t index) {
        return onMemoryCreate(SS_CREATE_LAYOUT, bank, sound, index);
    }
    static int32_t onMemoryCreateLayoutSoundCoreWithExternalId(uint32_t bank, uint32_t sound, uint32_t index,
                                                               uint32_t externalId) {
        return onMemoryCreate(SS_CREATE_LAYOUT_EXT, bank, sound, index, externalId);
    }

    // (a streaming bank holds one sound: the index is not used)
    static int32_t streamingCreateSoundCore(uint32_t bank, uint32_t sound, uint32_t /*index*/) {
        return streamingCreate(SS_CREATE_STREAMING, bank, sound);
    }
    static int32_t streamingCreateLayoutSoundCore(uint32_t bank, uint32_t sound, uint32_t /*index*/) {
        return streamingCreate(SS_CREATE_STREAMING_LAYOUT, bank, sound);
    }
    static int32_t streamingCreateSoundCoreWithExternalId(uint32_t bank, uint32_t sound, uint32_t /*index*/,
                                                          uint32_t externalId) {
        return streamingCreate(SS_CREATE_STREAMING_EXT, bank, sound, externalId);
    }
    static int32_t streamingCreateLayoutSoundCoreWithExternalId(uint32_t bank, uint32_t sound, uint32_t /*index*/,
                                                                uint32_t externalId) {
        return streamingCreate(SS_CREATE_STREAMING_LAYOUT_EXT, bank, sound, externalId);
    }

    static uint32_t streamingBankCtor(uint32_t bank) {
        rw::call(BANK_IMPL_CTOR, {bank});
        st32(bank, ld32(VT_STREAMING_BANK_GOT) + 8);
        rw::call(FILE_CTOR, {bank + 0x18});
        st32(bank + 0x24, 0);
        st32(bank + 0x28, 0);
        st32(bank + 0x2C, 0);
        return bank;
    }
    static uint32_t streamingBankDtor(uint32_t bank) {
        st32(bank, ld32(VT_STREAMING_BANK_GOT) + 8);
        for (uint32_t node = ld32(bank + 0x24); node != 0;) {
            deleteEntry(node);
            const uint32_t next = ld32(node + 8);
            listRemove(bank + 0x24, node);
            deleteObject(node);
            node = next;
        }
        // the list's own destructor (empty by now)
        for (uint32_t node = ld32(bank + 0x24); node != 0;) {
            const uint32_t next = ld32(node + 8);
            listRemove(bank + 0x24, node);
            deleteObject(node);
            node = next;
        }
        rw::call(FILE_DTOR, {bank + 0x18});
        rw::call(BANK_IMPL_DTOR, {bank});
        return bank;
    }
    // finished streams (status 0) are dropped with their readers, then BankImpl::Update
    static uint32_t streamingBankUpdate(uint32_t bank) {
        for (uint32_t node = ld32(bank + 0x24); node != 0;) {
            if (rw::call(SOUND_GET_STATUS, {ld32(node + 0xC) + 8}) != 0) {
                node = ld32(node + 8);
                continue;
            }
            deleteEntry(node);
            const uint32_t next = ld32(node + 8);
            listRemove(bank + 0x24, node);
            deleteObject(node);
            node = next;
        }
        return rw::call(BANK_IMPL_UPDATE, {bank});
    }
};
RW_FUNCTION(0xCE970C, "SQEX::BankImpl::CreateSoundWithExternalID", Banks::createSoundWithExternalId);
RW_FUNCTION(0xCE97D0, "SQEX::BankImpl::CreateLayoutSound", Banks::createLayoutSound);
RW_FUNCTION(0xCE988C, "SQEX::BankImpl::CreateLayoutSoundWithExternalID", Banks::createLayoutSoundWithExternalId);
RW_FUNCTION(0xCE9994, "SQEX::BankImpl::GetIsBelongSound", Banks::getIsBelongSound);
RW_FUNCTION(0xCE99C4, "SQEX::BankImpl::PrintDebugInfo", Banks::printDebugInfo);
RW_FUNCTION(0xCE9CAC, "SQEX::OnMemoryBank::CreateSoundCoreWithExternalID",
            Banks::onMemoryCreateSoundCoreWithExternalId);
RW_FUNCTION(0xCE9CC8, "SQEX::OnMemoryBank::CreateLayoutSoundCore", Banks::onMemoryCreateLayoutSoundCore);
RW_FUNCTION(0xCE9CE4, "SQEX::OnMemoryBank::CreateLayoutSoundCoreWithExternalID",
            Banks::onMemoryCreateLayoutSoundCoreWithExternalId);
RW_FUNCTION(0xCDBD00, "SQEX::StreamingBank::CreateSoundCore", Banks::streamingCreateSoundCore);
RW_FUNCTION(0xCDBE7C, "SQEX::StreamingBank::CreateLayoutSoundCore", Banks::streamingCreateLayoutSoundCore);
RW_FUNCTION(0xCDBFF8, "SQEX::StreamingBank::CreateSoundCoreWithExternalID",
            Banks::streamingCreateSoundCoreWithExternalId);
RW_FUNCTION(0xCDB9CC, "SQEX::StreamingBank::CreateLayoutSoundCoreWithExternalID",
            Banks::streamingCreateLayoutSoundCoreWithExternalId);
RW_FUNCTION(0xCDC18C, "SQEX::StreamingBank::StreamingBank", Banks::streamingBankCtor);
RW_FUNCTION(0xCDBB60, "SQEX::StreamingBank::~StreamingBank", Banks::streamingBankDtor);
RW_FUNCTION(0xCDC1F4, "SQEX::StreamingBank::Update", Banks::streamingBankUpdate);

}  // namespace sqex
