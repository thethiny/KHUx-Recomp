// APIManager::verifyResponse: the per-action handlers (APIManager_verify*.cpp) and what they share.
//
// verifyResponse(action, response) runs once per server reply. For each action it parses the parts of the reply
// the action carries (hole::network::api::X::parse into a shared_ptr<X>), keeps the parsed objects in the
// manager (the shared_ptr members), and applies item updates to the cached user data (updateMedal, updateSkill,
// ...). It returns false when a part the action needs is missing or malformed: onRespond then reports a server
// error. Actions without a body to check return true.
//
// The original is one function with a 6.6 KB stack frame: every case keeps its parsed shared_ptrs, records and
// id sets in its own slots of that frame. The handlers run on a frame of the same size and use the original slot
// offsets, since frame addresses end up in heap memory (an unordered_set's first-node anchor in its buckets).
#pragma once

#include "APIManager.hpp"

namespace apiv {

using namespace apim;

constexpr uint32_t FRAME_SIZE = 0x19D0;   // push {r4-r7, lr} + sub sp, #0x19BC

// The running check: the original frame, the manager and the response document (a rapidjson::Value).
struct Ctx {
    const Frame &F;
    APIManager *self;
    uint32_t json;
};

// ------------------------------------------------------------------------------------------ callees
constexpr uint32_t JSON_MEMBER = 0x4B1BB5;          // GenericValue::operator[](const char*)
constexpr uint32_t SAFEBUFFER_READ = 0x45A269;      // SafeBuffer::read(dst, src, n, index)
constexpr uint32_t STRING_ASSIGN = 0x498A1D;        // std::string::operator=(const std::string&)
constexpr uint32_t STRING_ASSIGN_CHARS = 0x48D811;  // std::string::assign(const char*, size_t)
constexpr uint32_t STRLEN = 0x40E08C;               // strlen (import)
constexpr uint32_t MEMCPY = 0x40E164;               // __aeabi_memcpy (import)
constexpr uint32_t TRACKING_SEND_ONCE = 0x4E04E5;   // TrackingSDKUtil::sendEventOnlyOnce(const char*, const char*)
constexpr uint32_t TRACKING_SEND = 0x4E0519;        // TrackingSDKUtil::sendEvent(const char*, const char*)
constexpr uint32_t APPSTATE_SAVE = 0x4E8DC9;        // AppState::save
constexpr uint32_t APPSTATE_SET_POP_BENEFIT_STONE = 0x4EBFD7;
constexpr uint32_t APPSTATE_ADD_NEW_AVATAR_BOARD_IDS = 0x4EBED5;
constexpr uint32_t MASTER_MEDAL_CREATE = 0x785871;  // master::Medal::create(int) -> shared_ptr
constexpr uint32_t MASTER_STAGE_CREATE = 0x78E4E9;  // master::Stage::create(int) -> shared_ptr
constexpr uint32_t MASTER_DRAW_MEDAL_TYPE_CREATE = 0x77DCE1;

// std::unordered_set<unsigned long long>
constexpr uint32_t ID_SET_DTOR = 0x482619;          // __hash_table<ull>::~__hash_table
constexpr uint32_t ID_SET_INSERT_COPY = 0x482649;   // __insert_unique(const ull&)
constexpr uint32_t ID_SET_INSERT = 0x4D6E1D;        // __insert_unique<ull>(ull&&)
constexpr uint32_t ID_SET_FIND = 0x4D5461;          // find<ull>(const ull&)
// std::vector<int>
constexpr uint32_t INT_VECTOR_DTOR = 0x478961;      // __vector_base<int>::~__vector_base
constexpr uint32_t INT_VECTOR_DEALLOCATE = 0x47D6B1;
constexpr uint32_t INT_VECTOR_PUSH_SLOW = 0x47D709; // __push_back_slow_path<int>(int&&)
constexpr uint32_t INT_VECTOR_PUSH_SLOW_COPY = 0x4D6CC1; // __push_back_slow_path<const int&>
constexpr uint32_t U64_VECTOR_ASSIGN = 0x4D7461;    // vector<ull>::assign(ull*, ull*)
constexpr uint32_t U64_VECTOR_DTOR = 0x468499;      // __vector_base<ull>::~__vector_base
constexpr uint32_t VECTOR_THROW_OUT_OF_RANGE = 0x47CCE1; // vector::at's __throw_out_of_range
// std::vector<PartyApplicationList::PartyEx> (408-byte elements)
constexpr uint32_t PARTY_EX_VECTOR_DTOR = 0x4B3DD1;
constexpr uint32_t PARTY_EX_VECTOR_DEALLOCATE = 0x4D5589;
constexpr uint32_t PARTY_EX_VECTOR_PUSH_SLOW = 0x4B3EA9; // __push_back_slow_path<const PartyEx&>
constexpr uint32_t PARTY_COPY = 0x4B2E63;           // Party::Party(const Party&) -> this
constexpr uint32_t PARTY_INFO_MAKE_SHARED = 0x4D5619; // std::make_shared<APIManager::PartyInfo>()
constexpr uint32_t USER_SHOP_ADD_BENEFIT = 0x4C4FB9;  // UserShop::addBenefit(const Benefit&)

// the parsers: X::parse(shared_ptr<X>* out, const Value& response[, ...])
constexpr uint32_t LOGIN_PARSE = 0x49DFD9;
constexpr uint32_t LOGIN_BONUS_PARSE = 0x49E271;
constexpr uint32_t USER_PARSE = 0x49E449;           // (out, response, bool required, const char* key)
constexpr uint32_t USER_POINT_PARSE = 0x49E8A9;
constexpr uint32_t USER_DETAIL_PARSE = 0x49EE1D;
constexpr uint32_t STAGE_RESUMPTION_PARSE = 0x49F4B1;
constexpr uint32_t USER_AVATAR_PARSE = 0x49F82D;
constexpr uint32_t USER_AVATAR_ALL_PARSE = 0x49F95D;
constexpr uint32_t USER_AVATAR_PARTS_PARSE = 0x49FC0D;
constexpr uint32_t USER_STONE_PARSE = 0x49FE71;
constexpr uint32_t USER_MEDAL_PARSE = 0x4A029D;
constexpr uint32_t USER_SKILL_PARSE = 0x4A0781;
constexpr uint32_t MATERIAL_INIT = 0x4A09E5;        // UserMaterial::Material::init(const Value&) -> bool
constexpr uint32_t USER_MATERIAL_PARSE = 0x4A0A71;
constexpr uint32_t KEYBLADE_INIT = 0x4A0CBD;        // UserKeyblade::Keyblade::init(const Value&) -> bool
constexpr uint32_t USER_KEYBLADE_PARSE = 0x4A0F3D;
constexpr uint32_t USER_ALBUM_PARSE = 0x4A121D;
constexpr uint32_t USER_ALBUM_CHALLENGE_PARSE = 0x4A1455;
constexpr uint32_t USER_ALBUM_CHALLENGE_UPDATE = 0x4A1591; // UserAlbumChallenge::updateChallenges(const set&)
constexpr uint32_t USER_TITLE_PARSE = 0x4A1689;
constexpr uint32_t USER_SUPPORT_PARSE = 0x4A18D5;
constexpr uint32_t USER_MEDAL_ENHANCE_PARSE = 0x4A1CF9;
constexpr uint32_t USER_MEDAL_EVOLVE_PARSE = 0x4A1F61;
constexpr uint32_t RAID_BASE_INIT = 0x4A2425;
constexpr uint32_t RAID_PARSE = 0x4A2579;
constexpr uint32_t RAID_MEMBER_PARSE = 0x4A26E9;
constexpr uint32_t RAID_REWARD_PARSE = 0x4A2CE1;
constexpr uint32_t RAID_REWARD_151101_PARSE = 0x4A325D;
constexpr uint32_t STAGE_PARSE = 0x4A38CD;
constexpr uint32_t STAGE_160310_PARSE = 0x4A3D3D;
constexpr uint32_t STAGE_EVENT_PARSE = 0x4A3F61;
constexpr uint32_t SUPPORT_USER_INIT = 0x4A41CD;
constexpr uint32_t STAGE_SUPPORT_LIST_PARSE = 0x4A457D; // (out, response, const char* key)
constexpr uint32_t STAGE_START_PARSE = 0x4A4A61;
constexpr uint32_t STAGE_CLEAR_PARSE = 0x4A4ED5;
constexpr uint32_t RAID_START_PARSE = 0x4A533D;
constexpr uint32_t RAID_CLEAR_PARSE = 0x4A58A9;
constexpr uint32_t SPHERE_INIT = 0x4A5AED;
constexpr uint32_t USER_SPHERE_PARSE = 0x4A5CB9;    // (out, response, bool)
constexpr uint32_t USER_SPHERE_SET = 0x4A5F35;      // UserSphere::setSphere(const Sphere&)
constexpr uint32_t USER_SPHERE_REMOVE = 0x4A5FD1;   // UserSphere::removeSphere(int)
constexpr uint32_t USER_PRESENT_PARSE = 0x4A61E5;
constexpr uint32_t USER_PRESENT_SET_RECEIVED = 0x4A649D; // UserPresent::setReceived(ull)
constexpr uint32_t USER_BIRTHDAY_PARSE = 0x4A64FD;
constexpr uint32_t SYSTEM_NEED_URL_PARSE = 0x4A6659;
constexpr uint32_t SYSTEM_COPPA_PARSE = 0x4A69FD;
constexpr uint32_t SYSTEM_INFORMATION_LIST_PARSE = 0x4A6CD1;
constexpr uint32_t SYSTEM_INFORMATION_LIST_151203_PARSE = 0x4A6DBD;
constexpr uint32_t SYSTEM_INVITECODE_PARSE = 0x4A6F4D;
constexpr uint32_t PAYMENT_INFO_PARSE = 0x4A707D;
constexpr uint32_t PARTY_INIT = 0x4A723D;
constexpr uint32_t USER_PARTY_PARSE = 0x4A7585;
constexpr uint32_t PARTY_CREATE_PARSE = 0x4A7919;
constexpr uint32_t PARTY_NOTICE_PARSE = 0x4A7AC9;
constexpr uint32_t PARTY_LOGIN_PARSE = 0x4A7DD5;
constexpr uint32_t PARTY_MEMBER_LIST_PARSE = 0x4A8041; // (out, response, const char* key)
constexpr uint32_t PARTY_APPLICATION_PARSE = 0x4A8521;  // (out, response, Type)
constexpr uint32_t PARTY_EX_INIT = 0x4A8685;
constexpr uint32_t PARTY_APPLICATION_LIST_PARSE = 0x4A86F5;
constexpr uint32_t PARTY_APPLICANT_LIST_PARSE = 0x4A8AF1;
constexpr uint32_t PARTY_INVITE_OFFER_PARSE = 0x4A8D85;
constexpr uint32_t OFFER_INIT = 0x4A8E31;
constexpr uint32_t PARTY_INVITE_LIST_PARSE = 0x4A8F95;
constexpr uint32_t PARTY_INVITE_LIST_SET_OFFER = 0x4A9229;
constexpr uint32_t PARTY_OFFER_LIST_PARSE = 0x4A9265;
constexpr uint32_t PARTY_OFFER_LIST_SET_PARTY = 0x4A9669;
constexpr uint32_t PURCHASE_PREPARE_PARSE = 0x4A96B9; // (out, response, const char* key)
constexpr uint32_t USER_SHOP_PARSE = 0x4A9845;
constexpr uint32_t COLOSSEUM_PARSE = 0x4A9B35;      // (out, response, bool)
constexpr uint32_t COLOSSEUM_START_PARSE = 0x4A9E09;
constexpr uint32_t COLOSSEUM_CLEAR_PARSE = 0x4AA081;
constexpr uint32_t GACHA_LIST_PARSE = 0x4AA595;
constexpr uint32_t GACHA_DRAW_PARSE = 0x4AA7B1;
constexpr uint32_t TUTORIAL_PROGRESS_PARSE = 0x4AAAD9;
constexpr uint32_t TUTORIAL_STATUS_PARSE = 0x4AAD01;
constexpr uint32_t TUTORIAL_CLEAR_PARSE = 0x4AAE05;
constexpr uint32_t USER_PROFILE_PARSE = 0x4AB9A5;
constexpr uint32_t LUX_RANKING_PARSE = 0x4ABFE1;    // (out, response, const char* key)
constexpr uint32_t PARTY_RANKING_PARSE = 0x4AC37D;  // (out, response, const char* key)
constexpr uint32_t COLOSSEUM_RANKING_PARSE = 0x4AC855; // (out, response, const char* key)
constexpr uint32_t UNION_RANKING_PARSE = 0x4ACD31;
constexpr uint32_t RANKING_REWARD_PARSE = 0x4AD04D;
constexpr uint32_t USER_CHAT_PARSE = 0x4AD3DD;
constexpr uint32_t MYPAGE_INFO_PARSE = 0x4AD8F9;
constexpr uint32_t USER_OPTION_PARSE = 0x4ADC6D;
constexpr uint32_t LINK_INFO_INIT = 0x4ADDA1;       // UserLink::Info::init
constexpr uint32_t USER_LINK_PARSE = 0x4ADE19;

// APIManager's own (direct calls)
constexpr uint32_t UPDATE_COORDINATE = 0x4C406D;
constexpr uint32_t UPDATE_KEYBLADE = 0x4C40E1;
constexpr uint32_t UPDATE_MEDAL = 0x4C42F5;
constexpr uint32_t UPDATE_SKILL = 0x4C4499;
constexpr uint32_t UPDATE_MATERIAL = 0x4C4671;
constexpr uint32_t REMOVE_MEDALS = 0x4C47DD;
constexpr uint32_t REMOVE_SKILLS = 0x4C4905;
constexpr uint32_t UPDATE_TITLE = 0x4C4A21;
constexpr uint32_t UPDATE_FAVORITE_KEYBLADE = 0x4C4BAD;
constexpr uint32_t UPDATE_AVATAR_PART = 0x4C4CD1;

// rapidjson GenericValue: the flags word at +12 (type in the low byte) and the flag bits checked here
constexpr uint32_t VALUE_FLAGS = 12;
constexpr uint32_t JSON_FLAG_BOOL = 0x100;
constexpr uint32_t JSON_FLAG_INT = 0x400;
constexpr uint32_t JSON_FLAG_UINT64 = 0x2000;
constexpr uint32_t JSON_FLAG_STRING = 0x100000;
constexpr uint32_t JSON_TRUE = 0x102;               // kTrueFlag
constexpr float ONE_F = 1.0f;

// ------------------------------------------------------------------------------------------ manager members
// The shared_ptr members verifyResponse fills (offsets into APIManager; the header names the ones other code
// reads).
namespace off {
constexpr uint32_t LOGIN = 0x04C;
constexpr uint32_t TUTORIAL_PROGRESS = 0x054;
constexpr uint32_t TUTORIAL_CLEAR = 0x05C;
constexpr uint32_t TUTORIAL_STATUS = 0x064;
constexpr uint32_t LOGIN_BONUS = 0x06C;
constexpr uint32_t USER = 0x074;
constexpr uint32_t USER_CHAT = 0x07C;
constexpr uint32_t USER_PARTY = 0x084;
constexpr uint32_t USER_POINT = 0x08C;
constexpr uint32_t USER_DETAIL = 0x094;
constexpr uint32_t STAGE_RESUMPTION = 0x09C;
constexpr uint32_t USER_STONE = 0x0A4;
constexpr uint32_t USER_SHOP = 0x0AC;
constexpr uint32_t USER_SPHERE = 0x0B4;
constexpr uint32_t USER_AVATAR = 0x0BC;
constexpr uint32_t USER_AVATAR_ALL = 0x0C4;
constexpr uint32_t USER_AVATAR_PARTS = 0x0CC;
constexpr uint32_t USER_MEDAL = 0x0D4;
constexpr uint32_t USER_SKILL = 0x0DC;
constexpr uint32_t USER_MATERIAL = 0x0E4;
constexpr uint32_t USER_KEYBLADE = 0x0EC;
constexpr uint32_t USER_ALBUM = 0x0F4;
constexpr uint32_t USER_ALBUM_CHALLENGE = 0x0FC;
constexpr uint32_t USER_PRESENT = 0x104;
constexpr uint32_t USER_TITLE = 0x10C;
constexpr uint32_t USER_SUPPORT = 0x114;
constexpr uint32_t STAGE = 0x11C;
constexpr uint32_t STAGE_160310 = 0x124;
constexpr uint32_t STAGE_EVENT = 0x12C;
constexpr uint32_t SUPPORT_LIST = 0x134;            // StageSupportList: stage/support/list, raid/start, party/invite/search
constexpr uint32_t STAGE_START = 0x13C;
constexpr uint32_t STAGE_CLEAR = 0x144;
constexpr uint32_t RAID = 0x14C;
constexpr uint32_t RAIDS = 0x154;                   // std::vector<RaidBase> (240 bytes each)
constexpr uint32_t RAID_REWARD = 0x160;
constexpr uint32_t RAID_REWARD_151101 = 0x168;
constexpr uint32_t RAID_MEMBER = 0x170;
constexpr uint32_t RAID_START = 0x178;
constexpr uint32_t RAID_CLEAR = 0x180;
constexpr uint32_t KEYBLADE_ENHANCE_TITLE = 0x188;  // the titles keyblade/enhance returned
constexpr uint32_t USER_MEDAL_ENHANCE = 0x190;
constexpr uint32_t USER_MEDAL_EVOLVE = 0x198;
constexpr uint32_t USER_BIRTHDAY = 0x1A0;
constexpr uint32_t SYSTEM_NEED_URL = 0x1A8;
constexpr uint32_t SYSTEM_COPPA = 0x1B0;
constexpr uint32_t SYSTEM_INFORMATION_LIST = 0x1B8;
constexpr uint32_t SYSTEM_INFORMATION_LIST_151203 = 0x1C0;
constexpr uint32_t INFORMATION_BODY = 0x1C8;        // std::string
constexpr uint32_t PAYMENT_INFO = 0x1D4;
constexpr uint32_t PARTY_CREATE = 0x1DC;
constexpr uint32_t OTHER_PARTY = 0x1E4;             // shared_ptr<APIManager::PartyInfo>: party (another party)
constexpr uint32_t PARTY_NOTICE = 0x1EC;
constexpr uint32_t PARTY_LOGIN = 0x1F4;
constexpr uint32_t PARTY_MEMBER_LIST = 0x1FC;
constexpr uint32_t PARTY_SEARCH_RESULTS = 0x204;    // std::vector<PartyApplicationList::PartyEx>
constexpr uint32_t PARTY_APPLICATION = 0x210;
constexpr uint32_t PARTY_APPLICATION_LIST = 0x218;
constexpr uint32_t PARTY_APPLICANT_LIST = 0x220;
constexpr uint32_t PARTY_INVITE_OFFER = 0x228;
constexpr uint32_t PARTY_INVITE_LIST = 0x230;
constexpr uint32_t PARTY_OFFER_LIST = 0x238;
constexpr uint32_t PURCHASE_PREPARE = 0x240;
constexpr uint32_t RECEIPT_STATUS = 0x248;          // u32: purchase/finish receiptStatus
constexpr uint32_t COLOSSEUM = 0x24C;
constexpr uint32_t COLOSSEUM_REWARD_RANK_IDS = 0x254; // std::vector<int>
constexpr uint32_t COLOSSEUM_START = 0x260;
constexpr uint32_t COLOSSEUM_CLEAR = 0x268;
constexpr uint32_t ACTIVE_CAMPAIGN_IDS = 0x270;     // std::vector<int>
constexpr uint32_t SYSTEM_INVITECODE = 0x27C;
constexpr uint32_t GACHA_LIST = 0x284;
constexpr uint32_t GACHA_DRAW = 0x28C;
constexpr uint32_t USER_PROFILE = 0x294;
constexpr uint32_t LUX_RANKING_WEEKLY = 0x29C;
constexpr uint32_t LUX_RANKING_MONTHLY = 0x2A4;
constexpr uint32_t LUX_RANKING_WEEKLY_OWN = 0x2AC;
constexpr uint32_t LUX_RANKING_MONTHLY_OWN = 0x2B4;
constexpr uint32_t LUX_RANKING = 0x2BC;
constexpr uint32_t LUX_RANKING_OWN = 0x2C4;
constexpr uint32_t PARTY_RANKING = 0x2CC;
constexpr uint32_t PARTY_RANKING_OWN = 0x2D4;
constexpr uint32_t COLOSSEUM_RANKING = 0x2DC;
constexpr uint32_t COLOSSEUM_RANKING_OWN = 0x2E4;
constexpr uint32_t UNION_RANKING = 0x2EC;           // ranking/parade
constexpr uint32_t RANKING_REWARD = 0x2F4;
constexpr uint32_t CAMPAIGN_IDS = 0x2FC;            // std::vector<int>
constexpr uint32_t SPHERE_CHECK_BOARD_IDS = 0x308;  // std::vector<int>: boards user/sphere/check reported
constexpr uint32_t MYPAGE_INFO = 0x314;
constexpr uint32_t USER_OPTION = 0x31C;
constexpr uint32_t USER_DETAIL_OTHER = 0x324;       // user/detail (another user's)
constexpr uint32_t LINK_MEMBERS = 0x32C;            // PartyMemberList (linkUserList)
constexpr uint32_t USER_LINK = 0x334;               // UserLink: unordered_map<PlatformType, Info> at +0
constexpr uint32_t STAGE_REQUEST = 0x33C;           // SafeBuffer: action, stage id, support user id, keyblade id
constexpr uint32_t PIN_CODE = 0x360;                // std::string: campaign/pincode avatarSync.pinCode
constexpr uint32_t EVENT_CATEGORY = 0x041;          // u8: the stage/event request's eventCategory
constexpr uint32_t AVATAR_SYNC_SRC = 0x35C;         // u8: campaign pincode avatarSync.srcAccept
constexpr uint32_t AVATAR_SYNC_DEST = 0x35D;        // u8: ... destAccept

constexpr uint32_t MYPAGE_PRESENT_COUNT = 0x18;     // MypageInfo: unreceived presents
}  // namespace off

// ------------------------------------------------------------------------------------------ helpers
inline StdSharedPtr &member(APIManager *self, uint32_t offset) {
    return *reinterpret_cast<StdSharedPtr *>(reinterpret_cast<uint8_t *>(self) + offset);
}

// X::parse into the frame slot (a shared_ptr<X>); returns the parsed object (null: missing or malformed)
inline uint32_t parse(const Ctx &c, uint32_t fn, uint32_t slot) {
    c.F.call(fn, {c.F.at(slot), c.json});
    return c.F.word(slot);
}
inline uint32_t parse(const Ctx &c, uint32_t fn, uint32_t slot, uint32_t arg) {
    c.F.call(fn, {c.F.at(slot), c.json, arg});
    return c.F.word(slot);
}
inline uint32_t parse(const Ctx &c, uint32_t fn, uint32_t slot, uint32_t arg, uint32_t arg2) {
    c.F.call(fn, {c.F.at(slot), c.json, arg, arg2});
    return c.F.word(slot);
}

// manager member = the frame slot's shared_ptr (operator=)
inline void keep(const Ctx &c, uint32_t slot, uint32_t offset) {
    assignShared(c.F, member(c.self, offset), c.F.word(slot), c.F.word(slot + 4));
}
// the frame slot's shared_ptr goes out of scope
inline void drop(const Ctx &c, uint32_t slot) { releaseIfSet(c.F, c.F.word(slot + 4)); }

// The common case: parse, keep it when it parsed, drop the frame's reference. 1 = parsed.
inline uint32_t parseAndKeep(const Ctx &c, uint32_t fn, uint32_t slot, uint32_t offset) {
    uint32_t ok = 0;
    if (parse(c, fn, slot)) {
        keep(c, slot, offset);
        ok = 1;
    }
    drop(c, slot);
    return ok;
}

// response[name] / value[name]
inline uint32_t jsonMember(const Ctx &c, uint32_t value, uint32_t name) { return c.F.call(JSON_MEMBER, {value, name}); }
// the flags word (an object / array is exactly JSON_OBJECT / JSON_ARRAY)
inline uint32_t jsonFlags(uint32_t value) { return load32(value + VALUE_FLAGS); }
inline uint32_t jsonArraySize(uint32_t array) { return load32(array + 4); }
inline uint32_t jsonArrayAt(uint32_t array, uint32_t k) { return load32(array) + 16 * k; }

inline void sendEventOnce(const Ctx &c, uint32_t name) { c.F.call(TRACKING_SEND_ONCE, {name, 0}); }
inline void saveAppState(const Ctx &c) { c.F.call(APPSTATE_SAVE, {c.F.call(APPSTATE_GET_INSTANCE, {})}); }

// The records of a parsed user list (UserRecordList: SafeBuffer, count at +0x20), each decoded into the frame
// slot and handed to the manager's update method: update(self, record).
// listRef: the guest word holding the list pointer (re-read for every record, like the original).
inline void applyRecordsAt(const Ctx &c, uint32_t listRef, uint32_t recordSize, uint32_t recordSlot, uint32_t update) {
    const uint32_t count = load32(load32(listRef) + 0x20);
    for (uint32_t k = 0, index = 0; k < count; k++, index += recordSize) {
        const uint32_t list = load32(listRef);
        c.F.call(SAFEBUFFER_READ, {list, c.F.at(recordSlot), load32(list + 12) + index, recordSize, index});
        c.F.call(update, {addr(c.self), c.F.at(recordSlot)});
    }
}
// the list in a frame slot (a parsed shared_ptr)
inline void applyRecords(const Ctx &c, uint32_t listSlot, uint32_t recordSize, uint32_t recordSlot, uint32_t update) {
    applyRecordsAt(c, c.F.at(listSlot), recordSize, recordSlot, update);
}

// frame byte &= mask: the original's bitfield initializations of its locals (frame stores need no logging)
inline void frameAnd8(const Ctx &c, uint32_t slot, uint8_t mask) {
    uint8_t *p = rw::frame_ptr<uint8_t>(c.F.at(slot));
    *p = uint8_t(*p & mask);
}

// std::string::operator=(const std::string&) as inlined: assign(chars, size) unless it is the same string
inline void assignString(const Ctx &c, uint32_t dst, uint32_t src) {
    if (dst == src) return;
    c.F.call(STRING_ASSIGN_CHARS, {dst, cstr(src), strSize(src)});
}
// libc++ ~string(): the long form frees its buffer
inline void destroyString(const Ctx &c, uint32_t s) {
    if (load8(s) & 1) c.F.call(OPERATOR_DELETE, {load32(s + 8)});
}

// A list member (shared_ptr<UserRecordList>) of a parsed result held while its records are applied: the
// original copies the shared_ptr (a reference more) and drops the copies when done.
struct HeldList {
    uint32_t list, ctrl;
};
inline HeldList holdList(const Ctx &c, uint32_t member) {
    HeldList h{load32(member), load32(member + 4)};
    if (h.ctrl) addShared(c.F, h.ctrl);
    return h;
}
inline void applyHeld(const Ctx &c, const HeldList &h, uint32_t recordSize, uint32_t recordSlot, uint32_t update) {
    const uint32_t count = load32(h.list + 0x20);
    for (uint32_t k = 0, index = 0; k < count; k++, index += recordSize) {
        c.F.call(SAFEBUFFER_READ, {h.list, c.F.at(recordSlot), load32(h.list + 12) + index, recordSize, index});
        c.F.call(update, {addr(c.self), c.F.at(recordSlot)});
    }
}
inline void releaseHeld(const Ctx &c, const HeldList &h) { releaseIfSet(c.F, h.ctrl); }

// manager member = std::move(the frame slot's shared_ptr) when it parsed; the frame slot is emptied either way
// (the moved-from temporary releases what it still holds). 1 = parsed.
inline uint32_t moveParsed(const Ctx &c, uint32_t slot, uint32_t offset) {
    const uint32_t ptr = c.F.word(slot), ctrl = c.F.word(slot + 4);
    c.F.setWord(slot, 0);
    c.F.setWord(slot + 4, 0);
    if (!ptr) {
        releaseIfSet(c.F, ctrl);
        return 0;
    }
    StdSharedPtr &dst = member(c.self, offset);
    dst.ptr = ptr;
    const uint32_t old = dst.ctrl;
    dst.ctrl = ctrl;
    releaseIfSet(c.F, old);
    return 1;
}

// response[name], an array of ints, replaces a std::vector<int> member (built in the frame vector at vecSlot,
// each value through the frame word valueSlot). false: not an array or an element is not an int (nothing kept).
inline bool readIntList(const Ctx &c, uint32_t name, uint32_t vecSlot, uint32_t valueSlot, StdVector &dst) {
    const uint32_t vec = c.F.at(vecSlot);
    c.F.setWord(vecSlot + 0, 0);
    c.F.setWord(vecSlot + 4, 0);
    c.F.setWord(vecSlot + 8, 0);
    bool ok = false;
    const uint32_t array = jsonMember(c, c.json, name);
    if (jsonFlags(array) == JSON_ARRAY) {
        ok = true;
        const uint32_t n = jsonArraySize(array);
        for (uint32_t k = 0; k < n; k++) {
            const uint32_t e = jsonArrayAt(array, k);
            if (!(jsonFlags(e) & JSON_FLAG_INT)) {
                ok = false;
                break;
            }
            c.F.setWord(valueSlot, load32(e));
            pushBackCopy(c.F, vec, c.F.at(valueSlot), 4, INT_VECTOR_PUSH_SLOW, true);
        }
        if (ok) {
            c.F.call(INT_VECTOR_DEALLOCATE, {addr(&dst)});
            dst.begin = c.F.word(vecSlot + 0);
            dst.end = c.F.word(vecSlot + 4);
            dst.cap = c.F.word(vecSlot + 8);
            c.F.setWord(vecSlot + 0, 0);
            c.F.setWord(vecSlot + 4, 0);
            c.F.setWord(vecSlot + 8, 0);
        }
    }
    c.F.call(INT_VECTOR_DTOR, {vec});
    return ok;
}

// A clear reply's new user detail against the cached one: crossing level 100 / 200 sends the AvatarLevel events
// (the levels, data position 0, decoded into the two frame words).
constexpr uint32_t EV_AVATAR_LEVEL_100 = 0xDFC0CD;  // "AvatarLevel100"
constexpr uint32_t EV_AVATAR_LEVEL_200 = 0xDFC0DC;  // "AvatarLevel200"
inline void sendLevelEvents(const Ctx &c, uint32_t newDetailSlot, uint32_t oldLevelSlot, uint32_t newLevelSlot) {
    const uint32_t old = member(c.self, off::USER_DETAIL).ptr;
    if (!old || !c.F.word(newDetailSlot)) return;
    c.F.call(SAFEBUFFER_READ, {old, c.F.at(oldLevelSlot), load32(old + 12), 4, 0});
    const int32_t before = int32_t(c.F.word(oldLevelSlot));
    const uint32_t detail = c.F.word(newDetailSlot);
    c.F.call(SAFEBUFFER_READ, {detail, c.F.at(newLevelSlot), load32(detail + 12), 4, 0});
    const int32_t after = int32_t(c.F.word(newLevelSlot));
    if (before <= 99 && after >= 100) sendEventOnce(c, EV_AVATAR_LEVEL_100);
    if (before <= 199 && after >= 200) sendEventOnce(c, EV_AVATAR_LEVEL_200);
}

// the tracking event names more than one handler sends
constexpr uint32_t EV_JOIN_PARTY = 0xDFC10D;        // "JoinParty"
// UserParty: bool at data position 0x10 = the player just joined a party; u64 at 8 = the party id
constexpr uint32_t PARTY_JOINED = 0x10;
constexpr uint32_t PARTY_ID = 0x08;

// A parsed UserParty saying the player just joined sends "JoinParty" (the flag decoded into the frame byte).
inline void sendJoinParty(const Ctx &c, uint32_t party, uint32_t byteSlot) {
    c.F.call(SAFEBUFFER_READ, {party, c.F.at(byteSlot), load32(party + 12) + PARTY_JOINED, 1, PARTY_JOINED});
    if (load8(c.F.at(byteSlot))) sendEventOnce(c, EV_JOIN_PARTY);
}

// record sizes of the user lists
constexpr uint32_t COORDINATE_SIZE = 0x30;
constexpr uint32_t KEYBLADE_SIZE = 0x58;
constexpr uint32_t MEDAL_SIZE = 0x48;
constexpr uint32_t SKILL_SIZE = 0x28;
constexpr uint32_t MATERIAL_SIZE = 0x10;
constexpr uint32_t AVATAR_PART_SIZE = 0x18;
constexpr uint32_t TITLE_SIZE = 0x10;

// std::unordered_set<unsigned long long> constructed in the frame: no buckets, no nodes, max_load_factor 1.0f
inline void newIdSet(const Ctx &c, uint32_t slot) {
    c.F.setWord(slot + 0, 0);
    c.F.setWord(slot + 4, 0);
    c.F.setWord(slot + 8, 0);
    c.F.setWord(slot + 12, 0);
    c.F.setWord(slot + 16, rw::fbits(ONE_F));
}
inline void deleteIdSet(const Ctx &c, uint32_t slot) { c.F.call(ID_SET_DTOR, {c.F.at(slot)}); }

// response[name], an array of uint64 ids, into the set (each id goes through the frame word valueSlot;
// __insert_unique's result pair lands at resultSlot). false: not an array, or an element is not a uint64 (the
// ids before it stay in the set).
inline bool readIdSet(const Ctx &c, uint32_t name, uint32_t setSlot, uint32_t valueSlot, uint32_t resultSlot) {
    const uint32_t array = jsonMember(c, c.json, name);
    if (jsonFlags(array) != JSON_ARRAY) return false;
    const uint32_t n = jsonArraySize(array);
    for (uint32_t k = 0; k < n; k++) {
        const uint32_t e = jsonArrayAt(array, k);
        if (!(jsonFlags(e) & JSON_FLAG_UINT64)) return false;
        c.F.setWord(valueSlot, load32(e));
        c.F.setWord(valueSlot + 4, load32(e + 4));
        c.F.call(ID_SET_INSERT, {c.F.at(resultSlot), c.F.at(setSlot), c.F.at(valueSlot)});
    }
    return true;
}

// ------------------------------------------------------------------------------------------ the handlers
// One per action (or group of actions sharing a body); the return value is verifyResponse's.
uint32_t verifyUser(const Ctx &c);                    // 1 user
uint32_t verifyUserDetailOther(const Ctx &c);         // 3 user/detail
uint32_t verifyUserProfile(const Ctx &c);             // 4 user/profile
uint32_t verifyUserChat(const Ctx &c);                // 5
uint32_t verifyUserStone(const Ctx &c);               // 6
uint32_t verifyUserShop(const Ctx &c);                // 7
uint32_t verifyUserBirthday(const Ctx &c);            // 8, 40
uint32_t verifyUserSphere(const Ctx &c);              // 9
uint32_t verifyUserKeyblade(const Ctx &c);            // 10
uint32_t verifyUserSkill(const Ctx &c);               // 11
uint32_t verifyUserMedal(const Ctx &c);               // 13
uint32_t verifyUserMaterial(const Ctx &c);            // 14
uint32_t verifyUserAlbum(const Ctx &c);               // 15
uint32_t verifyUserTitle(const Ctx &c);               // 16
uint32_t verifyUserSupport(const Ctx &c);             // 17, 45
uint32_t verifyUserPresent(const Ctx &c);             // 18
uint32_t verifyUserAvatar(const Ctx &c);              // 19
uint32_t verifyUserAvatarAll(const Ctx &c);           // 20
uint32_t verifyUserAvatarParts(const Ctx &c);         // 21
uint32_t verifyPaymentInfo(const Ctx &c);             // 22
uint32_t verifySystemNeedUrl(const Ctx &c);           // 23
uint32_t verifySystemCoppa(const Ctx &c);             // 24
uint32_t verifySystemMaster(const Ctx &c);            // 25
uint32_t verifySystemResource(const Ctx &c);          // 26
uint32_t verifySystemResourceEv(const Ctx &c);        // 27
uint32_t verifySystemInformationList(const Ctx &c);   // 28
uint32_t verifySystemInformationList151203(const Ctx &c); // 29
uint32_t verifySystemInformationDetail(const Ctx &c); // 30
uint32_t verifySystemSerialcode(const Ctx &c);        // 31
uint32_t verifySystemSerialcodeCampaign(const Ctx &c);// 32
uint32_t verifySystemInvitecode(const Ctx &c);        // 33
uint32_t verifyUserUpdate(const Ctx &c);              // 36, 37
uint32_t verifyUserDetail(const Ctx &c);              // 38, 39
uint32_t verifyUserAvatarUpdate(const Ctx &c);        // 41
uint32_t verifyUserSphereUpdate(const Ctx &c);        // 42
uint32_t verifyUserKeybladeUpdate(const Ctx &c);      // 43
uint32_t verifyUserMedalLock(const Ctx &c);           // 44
uint32_t verifyMedalSkillUpdate(const Ctx &c);        // 46
uint32_t verifyUserPresentReceive(const Ctx &c);      // 47
uint32_t verifyUserMaterialSell(const Ctx &c);        // 48
uint32_t verifyUserMedalSell(const Ctx &c);           // 49
uint32_t verifySkillSell(const Ctx &c);               // 50
uint32_t verifyKeybladeBurstMax(const Ctx &c);        // 51
uint32_t verifyKeybladeEnhance(const Ctx &c);         // 52
uint32_t verifyUserMedalEnhance(const Ctx &c);        // 53
uint32_t verifyUserMedalEvolve(const Ctx &c);         // 54
uint32_t verifySkillEvolve(const Ctx &c);             // 55
uint32_t verifyMypage(const Ctx &c);                  // 56
uint32_t verifyUserSphereCheck(const Ctx &c);         // 57
uint32_t verifyLogin(const Ctx &c);                   // 59, 61
uint32_t verifyLoginBonus(const Ctx &c);              // 60
uint32_t verifyTutorialProgress(const Ctx &c);        // 62
uint32_t verifyTutorialClear(const Ctx &c);           // 63
uint32_t verifyTutorialStatus(const Ctx &c);          // 64, 65
uint32_t verifyShopMedalMax(const Ctx &c);            // 66
uint32_t verifyShopApRecover(const Ctx &c);           // 67
uint32_t verifyUserSphereBuy(const Ctx &c);           // 68
uint32_t verifyUnionChange(const Ctx &c);             // 69
uint32_t verifyGachaList(const Ctx &c);               // 70
uint32_t verifyGachaDraw(const Ctx &c);               // 71
uint32_t verifyPurchasePrepare(const Ctx &c);         // 72
uint32_t verifyPurchaseFinish(const Ctx &c);          // 73
uint32_t verifyPartyCreate(const Ctx &c);             // 74
uint32_t verifyPartyUpdate(const Ctx &c);             // 75, 79
uint32_t verifyPartyNotice(const Ctx &c);             // 76
uint32_t verifyPartyLogin(const Ctx &c);              // 77
uint32_t verifyParty(const Ctx &c);                   // 78
uint32_t verifyPartyMemberList(const Ctx &c);         // 80
uint32_t verifyPartySearchActive(const Ctx &c);       // 81
uint32_t verifyPartyApplication(const Ctx &c);        // 82
uint32_t verifyPartyApplicationList(const Ctx &c);    // 83
uint32_t verifyPartyApplicationCancel(const Ctx &c);  // 84
uint32_t verifyPartyApplicantList(const Ctx &c);      // 85
uint32_t verifyPartyApplicantAgree(const Ctx &c);     // 86
uint32_t verifyPartyApplicantReject(const Ctx &c);    // 87
uint32_t verifyPartyLeaderChange(const Ctx &c);       // 88
uint32_t verifyPartyAdmin(const Ctx &c);              // 89, 90, 91
uint32_t verifyPartyInviteSearch(const Ctx &c);       // 92
uint32_t verifyPartyInviteOffer(const Ctx &c);        // 93
uint32_t verifyPartyInviteList(const Ctx &c);         // 94
uint32_t verifyPartyInviteCancel(const Ctx &c);       // 95
uint32_t verifyPartyOfferList(const Ctx &c);          // 96
uint32_t verifyPartyOfferAgree(const Ctx &c);         // 97
uint32_t verifyPartyOfferReject(const Ctx &c);        // 98
uint32_t verifyStage(const Ctx &c);                   // 99
uint32_t verifyStage160310(const Ctx &c);             // 100
uint32_t verifyStageEvent(const Ctx &c);              // 101
uint32_t verifyStageSupportList(const Ctx &c);        // 102
uint32_t verifyStageStart(const Ctx &c);              // 103
uint32_t verifyRetire(const Ctx &c);                  // 104, 117
uint32_t verifyStageContinue(const Ctx &c);           // 105
uint32_t verifyStageClear(const Ctx &c);              // 106
uint32_t verifyRaid(const Ctx &c);                    // 107
uint32_t verifyRaidList(const Ctx &c);                // 108
uint32_t verifyRaidReward(const Ctx &c);              // 109
uint32_t verifyRaidReward151101(const Ctx &c);        // 110
uint32_t verifyRaidMember(const Ctx &c);              // 111
uint32_t verifyRaidStart(const Ctx &c);               // 112
uint32_t verifyRaidClear(const Ctx &c);               // 113
uint32_t verifyColosseum(const Ctx &c);               // 114
uint32_t verifyColosseumRewardList(const Ctx &c);     // 115
uint32_t verifyColosseumStart(const Ctx &c);          // 116
uint32_t verifyColosseumClear(const Ctx &c);          // 118
uint32_t verifyRankingWeeklyUser(const Ctx &c);       // 119
uint32_t verifyRankingMonthlyUser(const Ctx &c);      // 120
uint32_t verifyRankingWeeklyUserOwn(const Ctx &c);    // 121
uint32_t verifyRankingMonthlyUserOwn(const Ctx &c);   // 122
uint32_t verifyRankingLux(const Ctx &c);              // 123
uint32_t verifyRankingLuxOwn(const Ctx &c);           // 124
uint32_t verifyRankingParty(const Ctx &c);            // 125
uint32_t verifyRankingPartyOwn(const Ctx &c);         // 126
uint32_t verifyRankingColosseum(const Ctx &c);        // 127
uint32_t verifyRankingColosseumOwn(const Ctx &c);     // 128
uint32_t verifyRankingParade(const Ctx &c);           // 129
uint32_t verifyRankingRewardCheck(const Ctx &c);      // 130
uint32_t verifyCampaign(const Ctx &c);                // 131
uint32_t verifyCampaignPincodeGet(const Ctx &c);      // 132, 136
uint32_t verifyCampaignPincodeCreate(const Ctx &c);   // 133
uint32_t verifyUserOption(const Ctx &c);              // 137, 138
uint32_t verifyLinkMembers(const Ctx &c);             // 139
uint32_t verifyUserLink(const Ctx &c);                // 140
uint32_t verifyUserLinkUpdate(const Ctx &c);          // 141
uint32_t verifyUserLinkDelete(const Ctx &c);          // 142
uint32_t verifyUserAlbumChallengeUpdate(const Ctx &c);// 144

}  // namespace apiv
