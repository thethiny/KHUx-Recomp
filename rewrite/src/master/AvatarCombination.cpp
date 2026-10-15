#include "AvatarCombination.hpp"
#include "ParseUtilD.hpp"

namespace master {

using AvatarCombinationFunctions = TableFunctions<AvatarCombination>;
RW_FUNCTION(0x7788f8, "master::AvatarCombination::getAvatarCombinationIds", AvatarCombinationFunctions::getIds);
RW_FUNCTION(0x778914, "master::AvatarCombination::create", AvatarCombinationFunctions::create);
RW_FUNCTION(0x778c80, "master::AvatarCombination::getHash", AvatarCombinationFunctions::getHash);
RW_FUNCTION(0x778d6c, "master::AvatarCombination::getID", AvatarCombinationFunctions::getID);
RW_FUNCTION(0x778db8, "master::AvatarCombination::~AvatarCombination (deleting)", AvatarCombinationFunctions::destructDelete);

void AvatarCombination::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_d::Kind;
    static constexpr parse_d::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)[, bytes per string]}
        {0xe07dbd, Kind::Int, AVATAR_COMBINATION_ID, 0x4, 0x24},            // "avatarCombinationId"
        {0xe07dd1, Kind::Int, COMBINATION_TYPE, 0x4, 0x20},                 // "combinationType"
        {0xdfb6e2, Kind::Int, AVATAR_PARTS_ID, 0x4, 0x1c},                  // "avatarPartsId"
        {0xdfaf4d, Kind::Int, STATUS, 0x4, 0x18},                           // "status"
    };
    parse_d::parseRow(out, json, {0x40, 0x10, PARSE_VTABLE, 0x778c2d}, FIELDS);
}
RW_FUNCTION(0x778aa8, "master::AvatarCombination::parse", AvatarCombination::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
