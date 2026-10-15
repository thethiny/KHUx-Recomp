#include "MasterTable.hpp"

namespace master {

void Row::create(guest::SharedPtr *out, int32_t id, uint32_t table, uint32_t blockVtable, uint32_t rowVtable) {
    uint32_t mgr = rw::call(MDM_GET_INSTANCE, {});
    guest::Frame f(0x20);
    auto *cached = f.at<guest::SharedPtr>(0x10);   // shared_ptr<const SafeBuffer> from the cache
    auto *file = f.at<guest::SharedPtr>(0x08);     // shared_ptr<bg::Data>: the row's bytes in the table file
    auto *made = f.at<guest::SharedPtr>(0x00);     // the new row, as handed to the cache
    rw::call(MDM_FIND, {f.addr(0x10), mgr, table, uint32_t(id)});
    if (cached->obj) {
        out->obj = cached->obj;
        out->ctrl = cached->ctrl;
        if (cached->ctrl) guest::addRef(cached->ctrl);
    } else {
        rw::call(MDM_GET_DATA, {f.addr(0x08), mgr, table, uint32_t(id)});
        if (file->obj) {
            // make_shared block: vtable, owners - 1, weak owners - 1, then the row object
            uint32_t block = guest::newObject(0x2C);
            uint32_t row;
            if (block) {
                rw::save<uint32_t>(block + 4, 0);
                rw::save<uint32_t>(block + 8, 0);
                rw::save<uint32_t>(block, blockVtable);
                row = block + 12;
                SafeBuffer::constructFromData(rw::host_ptr<SafeBuffer>(row), file);
                rw::save<uint32_t>(row, rowVtable);
                made->obj = row;
                made->ctrl = block;
                guest::addRef(block);
            } else {
                row = 12;
                made->obj = row;
                made->ctrl = 0;
            }
            rw::call(MDM_INSERT, {mgr, table, uint32_t(id), f.addr(0x00)});
            if (made->ctrl) guest::release(made->ctrl);
            out->obj = row;
            out->ctrl = block;
            if (block) {
                guest::addRef(block);
                guest::release(block);
            }
        } else {
            out->obj = 0;
            out->ctrl = 0;
        }
        if (file->ctrl) guest::release(file->ctrl);
    }
    if (cached->ctrl) guest::release(cached->ctrl);
}

}  // namespace master
