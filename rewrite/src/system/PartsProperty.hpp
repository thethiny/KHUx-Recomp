// PartsProperty::Property: an enemy part's obfuscated state (a SafeBuffer whose first 64 * 12 bytes are the
// part's BuffChange slots), implementing IBuffProtocol at +32 like StageState.
#pragma once

#include "core/SafeBuffer.hpp"
#include "BuffStatus.hpp"

namespace parts {

struct Property : SafeBuffer {
    rw::Field<uint32_t> statusVtable;   // +32 IBuffProtocol base (getBlankBuff is called on this + 32)

    // BuffChange getBuff(int slot) const: decode slot `slot` into *out.
    static void getBuff(const uint8_t *out, const Property *self, int32_t slot);
    // Store a buff in the first empty slot.
    static void addBuff(Property *self, const uint8_t *change);
    static void setBuff(Property *self, int32_t slot, const uint8_t *change);
    static void deletingDestroy(Property *self);
};
RW_LAYOUT(Property, statusVtable, 32);

}  // namespace parts
