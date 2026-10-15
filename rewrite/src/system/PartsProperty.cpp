#include "PartsProperty.hpp"
#include "IBuffProtocol.hpp"

namespace parts {

void Property::getBuff(const uint8_t *out, const Property *self, int32_t slot) {
    rw::store32(out, 0);
    rw::store32(out + 4, 0);
    rw::store32(out + 8, 0);
    uint32_t index = 12u * uint32_t(slot);
    SafeBuffer::read(self, out, self->data.get() + index, 12, index);
}
RW_FUNCTION(0x5F66CE, "PartsProperty::Property::getBuff", Property::getBuff);

void Property::addBuff(Property *self, const uint8_t *change) {
    auto *buffs = reinterpret_cast<IBuffProtocol *>(reinterpret_cast<uint8_t *>(self) + 32);
    uint32_t index = 12u * uint32_t(IBuffProtocol::getBlankBuff(buffs));
    SafeBuffer::write(self, self->data.get() + index, change, 12, index);
}
RW_FUNCTION(0x5F672C, "PartsProperty::Property::addBuff", Property::addBuff);

void Property::setBuff(Property *self, int32_t slot, const uint8_t *change) {
    uint32_t index = 12u * uint32_t(slot);
    SafeBuffer::write(self, self->data.get() + index, change, 12, index);
}
RW_FUNCTION(0x5F678C, "PartsProperty::Property::setBuff", Property::setBuff);

void Property::deletingDestroy(Property *self) {
    guest::deleteObject(rw::guest_addr(SafeBuffer::destruct(self)));
}
RW_FUNCTION(0x5F704C, "PartsProperty::Property::~Property (deleting)", Property::deletingDestroy);

}  // namespace parts
