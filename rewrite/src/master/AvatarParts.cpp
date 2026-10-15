#include "AvatarParts.hpp"
#include "ParseUtilD.hpp"

namespace master {

using AvatarPartsFunctions = TableFunctions<AvatarParts>;
RW_FUNCTION(0x778dc4, "master::AvatarParts::getAvatarPartsIds", AvatarPartsFunctions::getIds);
RW_FUNCTION(0x778de0, "master::AvatarParts::create", AvatarPartsFunctions::create);
RW_FUNCTION(0x7793c4, "master::AvatarParts::getHash", AvatarPartsFunctions::getHash);
RW_FUNCTION(0x7794b0, "master::AvatarParts::getID", AvatarPartsFunctions::getID);
RW_FUNCTION(0x7794fc, "master::AvatarParts::~AvatarParts (deleting)", AvatarPartsFunctions::destructDelete);

void AvatarParts::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_d::Kind;
    static constexpr parse_d::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)[, bytes per string]}
        {0xdfb6e2, Kind::Int, AVATAR_PARTS_ID, 0x4, 0x68},                  // "avatarPartsId"
        {0xdfade6, Kind::String, NAME, 0x81, 0x6c},                         // "name"
        {0xdfb6f0, Kind::Int, PARTS_TYPE, 0x4, 0x64},                       // "partsType"
        {0xdfb761, Kind::Int, GENDER, 0x4, 0x60},                           // "gender"
        {0xe07dd1, Kind::Int, COMBINATION_TYPE, 0x4, 0x5c},                 // "combinationType"
        {0xe07e0a, Kind::Int, COMBINATION_FLAG, 0x4, 0x58},                 // "combinationFlag"
        {0xe07e1a, Kind::Int, POSITION, 0x4, 0x54},                         // "position"
        {0xe07e23, Kind::Int, SET_KIND, 0x4, 0x50},                         // "setKind"
        {0xe07e2b, Kind::Int, FIXED_FLAG, 0x4, 0x4c},                       // "fixedFlag"
        {0xe07e35, Kind::Int, VALID_SET_CLOTH, 0x4, 0x48},                  // "validSetCloth"
        {0xe07e43, Kind::IntArray, SET_CLOTH, 0x14, 0x6c},                  // "setCloth"
        {0xdfaf4d, Kind::Int, STATUS, 0x4, 0x44},                           // "status"
    };
    parse_d::parseRow(out, json, {0x108, 0xc0, PARSE_VTABLE, 0x779371}, FIELDS);
}
RW_FUNCTION(0x778f74, "master::AvatarParts::parse", AvatarParts::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
