// Master data tables (namespace master): one row of a table is a SafeBuffer holding the row's fields at fixed
// data positions. Every table class has the same five generated functions besides its parse():
//   getXxxIds(out)   the IDs of the table (MasterDataManager::getIDs)
//   create(out, id)  the row as shared_ptr<const X>: from the MasterDataManager cache, else decoded from the
//                    table file and cached (the row is a local class create(int)::impl inside a make_shared block)
//   getHash()        the table's schema hash string
//   getID()          the row's ID (the first field)
//   ~X() (deleting)  SafeBuffer destructor + operator delete
// TableFunctions<T> implements them once; a table T provides TABLE, CREATE_BLOCK_VTABLE, ROW_VTABLE and HASH.
#pragma once

#include "core/Guest.hpp"
#include "core/SafeBuffer.hpp"

namespace master {

// MasterDataManager functions the tables call (rewritten in core/MasterDataManager.cpp; called through the guest
// so the master module stays independent of that file)
constexpr uint32_t MDM_GET_INSTANCE = 0x76C091;   // MasterDataManager::getInstance()
constexpr uint32_t MDM_FIND = 0x76CE05;           // MasterDataManager::find(out shared_ptr, mgr, table, id)
constexpr uint32_t MDM_INSERT = 0x76CEA9;         // MasterDataManager::insert(mgr, table, id, const shared_ptr&)
constexpr uint32_t MDM_GET_IDS = 0x76D091;        // MasterDataManager::getIDs(out std::set<int>, mgr, table)
constexpr uint32_t MDM_GET_DATA = 0x76D0A1;       // MasterDataManager::getData(out shared_ptr<bg::Data>, mgr, table, id)

// rapidjson (used by the parse functions)
constexpr uint32_t JSON_MEMBER = 0x4B1BB5;        // GenericValue::operator[](const char*) -> GenericValue&
constexpr uint32_t STRNCPY = 0x410E4C;            // strncpy (import)

// A row: a SafeBuffer whose vtable is the table's create(int)::impl vtable.
struct Row : SafeBuffer {
    // The generic create(): out = shared_ptr<const Row> for (table, id), or null when the table has no such row.
    static void create(guest::SharedPtr *out, int32_t id, uint32_t table, uint32_t blockVtable, uint32_t rowVtable);
    // The row ID (data position 0).
    static int32_t getID(const Row *self) { return self->get32(0); }
};

template <class T>
struct TableFunctions {
    // out: std::set<int> (returned by value)
    static void getIds(uint32_t out) {
        uint32_t mgr = rw::call(MDM_GET_INSTANCE, {});
        rw::call(MDM_GET_IDS, {out, mgr, T::TABLE});
    }
    static void create(guest::SharedPtr *out, int32_t id) {
        Row::create(out, id, T::TABLE, T::CREATE_BLOCK_VTABLE, T::ROW_VTABLE);
    }
    static uint32_t getHash() { return T::HASH; }
    static int32_t getID(const Row *self) { return Row::getID(self); }
    static void destructDelete(Row *self) { SafeBuffer::destructDelete(self); }
};

}  // namespace master
