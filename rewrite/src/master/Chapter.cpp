#include "Chapter.hpp"
#include "ParseUtilA.hpp"

namespace master {

using ChapterFunctions = TableFunctions<Chapter>;
RW_FUNCTION(0x77c058, "master::Chapter::getChapterIds", ChapterFunctions::getIds);
RW_FUNCTION(0x77c074, "master::Chapter::create", ChapterFunctions::create);
RW_FUNCTION(0x77c388, "master::Chapter::getHash", ChapterFunctions::getHash);
RW_FUNCTION(0x77c474, "master::Chapter::getID", ChapterFunctions::getID);
RW_FUNCTION(0x77c4c0, "master::Chapter::~Chapter (deleting)", ChapterFunctions::destructDelete);

void Chapter::parse(guest::SharedPtr *out, uint32_t json) {
    using parse_a::Kind;
    static constexpr parse_a::Field FIELDS[] = {
        // {JSON key literal, kind, data position, bytes, stack buffer (frame offset)}
        {0xe08488, Kind::Int, CHAPTER_ID, 0x4, 0x18},                 // "chapterId"
        {0xdfade6, Kind::String, NAME, 0x101, 0x1c},                  // "name"
    };
    parse_a::parseRow(out, json, {0x138, 0x108, PARSE_VTABLE, 0x77c335}, FIELDS);
}
RW_FUNCTION(0x77c208, "master::Chapter::parse", Chapter::parse);   // SafeBuffer(size, true): clock-seeded key

}  // namespace master
