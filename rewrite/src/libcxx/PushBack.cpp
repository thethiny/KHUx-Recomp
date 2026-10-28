// std::vector<T>::__push_back_slow_path(x) for the game's element types: the reallocating push_back. Trivially
// relocatable elements are moved with one memcpy (libcxx::pushBackRelocating), the others through a __split_buffer
// and the out-of-line __swap_out_circular_buffer (libcxx::pushBackViaSplitBuffer). Each instance supplies the
// element's copy / move constructor, inlined by the compiler - the stores below.
#include "libcxx/Libcxx.hpp"

using guest::Vector;
using libcxx::copyBytes;
using libcxx::copyShared;
using libcxx::copyString;
using libcxx::copyWords;
using libcxx::moveShared;
using libcxx::moveTriple;
using libcxx::pushBackRelocating;
using libcxx::pushBackRelocatingFreeInline;
using libcxx::pushBackViaSplitBuffer;
using rw::load;
using rw::save;

namespace {

// ---------------------------------------------------------------------------------------------- relocating

// elements copied word by word (ints, pointers, floats, small structs)
template <uint32_t Size, uint32_t SplitDtor> void pushBackWords(Vector *self, uint32_t x) {
    pushBackRelocating<Size>(self, SplitDtor, [&](uint32_t slot) { copyWords(slot, x, Size); });
}

// char / signed char (the split buffer's destructor inlined)
void pushBackByte(Vector *self, const uint8_t *x) {
    pushBackRelocatingFreeInline<1>(self, [&](uint32_t slot) { save<uint8_t>(slot, *x); });
}

// Date: four 16-bit fields, copied from the last
void pushBackDate(Vector *self, uint32_t x) {
    pushBackRelocating<8>(self, 0x482B35, [&](uint32_t slot) {
        const uint16_t a = load<uint16_t>(x), b = load<uint16_t>(x + 2), c = load<uint16_t>(x + 4), d = load<uint16_t>(x + 6);
        save<uint16_t>(slot + 6, d);
        save<uint16_t>(slot + 4, c);
        save<uint16_t>(slot + 2, b);
        save<uint16_t>(slot, a);
    });
}

// larger trivially copyable structs: memcpy
template <uint32_t Size, uint32_t SplitDtor> void pushBackMemcpy(Vector *self, uint32_t x) {
    pushBackRelocating<Size>(self, SplitDtor, [&](uint32_t slot) { copyBytes(slot, x, Size); });
}

// ---------------------------------------------------------------------------------------------- via split buffer


template <uint32_t Size, uint32_t SwapOut, uint32_t Dtor, class Construct>
void grow(Vector *self, Construct construct) {
    pushBackViaSplitBuffer<Size>(self, SwapOut, Dtor, construct);
}

void pushBackStringMove(Vector *self, uint32_t x) {
    grow<12, 0x484575, 0x4845D9>(self, [&](uint32_t slot) { moveTriple(slot, x); });
}
void pushBackStringCopy(Vector *self, uint32_t x) {
    grow<12, 0x484575, 0x4845D9>(self, [&](uint32_t slot) { copyString(slot, x); });
}

// std::vector<std::shared_ptr<T>>
template <uint32_t SwapOut, uint32_t Dtor> void pushBackSharedCopy(Vector *self, uint32_t x) {
    grow<8, SwapOut, Dtor>(self, [&](uint32_t slot) { copyShared(slot, x); });
}
template <uint32_t SwapOut, uint32_t Dtor> void pushBackSharedMove(Vector *self, uint32_t x) {
    grow<8, SwapOut, Dtor>(self, [&](uint32_t slot) { moveShared(slot, x); });
}

// std::vector<std::vector<T>> (moved)
template <uint32_t SwapOut, uint32_t Dtor> void pushBackVectorMove(Vector *self, uint32_t x) {
    grow<12, SwapOut, Dtor>(self, [&](uint32_t slot) { moveTriple(slot, x); });
}

// elements with an out-of-line copy / move constructor: called
template <uint32_t Size, uint32_t SwapOut, uint32_t Dtor, uint32_t Ctor> void pushBackCtor(Vector *self, uint32_t x) {
    grow<Size, SwapOut, Dtor>(self, [&](uint32_t slot) { rw::call(Ctor, {slot, x}); });
}

// hole::network::api::UserLink::Info (0x10): a byte, a string at 4 (the byte is stored again after the string)
void pushBackUserLinkInfo(Vector *self, uint32_t x) {
    grow<0x10, 0x4B1EE1, 0x4B1F55>(self, [&](uint32_t slot) {
        save<uint8_t>(slot, load<uint8_t>(x));
        copyString(slot + 4, x + 4);
        save<uint8_t>(slot, load<uint8_t>(x));
    });
}

// MypageInfo::AdminText (0x14): two words, a string at 8
void pushBackAdminText(Vector *self, uint32_t x) {
    grow<0x14, 0x4B2291, 0x4B2305>(self, [&](uint32_t slot) {
        copyWords(slot, x, 8);
        copyString(slot + 8, x + 8);
    });
}

// UnionRanking::Ranker (0x70): shared_ptrs at 0 and 8, the rest plain
void pushBackRanker(Vector *self, uint32_t x) {
    grow<0x70, 0x4B2601, 0x4B2683>(self, [&](uint32_t slot) {
        copyShared(slot, x);
        copyShared(slot + 8, x + 8);
        copyBytes(slot + 0x10, x + 0x10, 0x60);
    });
}

// ColosseumRanking::User (0x30): shared_ptrs at 0, 8, 0x10, then six plain words
void pushBackColosseumUser(Vector *self, uint32_t x) {
    grow<0x30, 0x4B29B1, 0x4B2A69>(self, [&](uint32_t slot) {
        copyShared(slot, x);
        copyShared(slot + 8, x + 8);
        copyShared(slot + 0x10, x + 0x10);
        copyWords(slot + 0x18, x + 0x18, 0x10);
        const uint32_t a = load<uint32_t>(x + 0x28), b = load<uint32_t>(x + 0x2C);
        save<uint32_t>(slot + 0x2C, b);
        save<uint32_t>(slot + 0x28, a);
    });
}

// LuxRanking::User (0x20): shared_ptrs at 0, 8, 0x10, two plain words
void pushBackLuxUser(Vector *self, uint32_t x) {
    grow<0x20, 0x4B30E5, 0x4B3185>(self, [&](uint32_t slot) {
        copyShared(slot, x);
        copyShared(slot + 8, x + 8);
        copyShared(slot + 0x10, x + 0x10);
        copyWords(slot + 0x18, x + 0x18, 8);
    });
}

constexpr uint32_t Party_copy = 0x4B2E63;   // hole::network::api::Party::Party(const Party&)

// PartyRanking::Info (0x78): a Party, two words
void pushBackPartyRankingInfo(Vector *self, uint32_t x) {
    grow<0x78, 0x4B2C59, 0x4B2DED>(self, [&](uint32_t slot) {
        rw::call(Party_copy, {slot, x});
        const uint32_t a = load<uint32_t>(x + 0x70), b = load<uint32_t>(x + 0x74);
        save<uint32_t>(slot + 0x70, a);
        save<uint32_t>(slot + 0x74, b);
    });
}

// PartyApplicationList::PartyEx (0x198): a Party, the rest plain
void pushBackPartyEx(Vector *self, uint32_t x) {
    grow<0x198, 0x4B3FCD, 0x4B4151>(self, [&](uint32_t slot) {
        rw::call(Party_copy, {slot, x});
        copyBytes(slot + 0x70, x + 0x70, 0x128);
    });
}

// PartyInviteList::Offer / PartyApplication::Applicant (0x38): 4 words, a string at 0x10, 7 words
template <uint32_t SwapOut, uint32_t Dtor> void pushBackOffer(Vector *self, uint32_t x) {
    grow<0x38, SwapOut, Dtor>(self, [&](uint32_t slot) {
        copyWords(slot, x, 0x10);
        copyString(slot + 0x10, x + 0x10);
        for (uint32_t off : {0x28u, 0x24u, 0x20u, 0x1Cu}) save<uint32_t>(slot + off, load<uint32_t>(x + off));
        copyWords(slot + 0x2C, x + 0x2C, 12);
    });
}

// PartyMemberList::Detail (0x98): plain up to 0x7C, strings at 0x7C and 0x88
void pushBackMemberDetail(Vector *self, uint32_t x) {
    grow<0x98, 0x4B48A5, 0x4B493F>(self, [&](uint32_t slot) {
        copyBytes(slot, x, 0x7C);
        copyString(slot + 0x7C, x + 0x7C);
        copyString(slot + 0x88, x + 0x88);
    });
}

// PartyNotice::NewcomerUser (0x30): a string at 0, 4 words at 0x10, a string at 0x20
void pushBackNewcomer(Vector *self, uint32_t x) {
    grow<0x30, 0x4B4CE5, 0x4B4D97>(self, [&](uint32_t slot) {
        copyString(slot, x);
        copyWords(slot + 0x10, x + 0x10, 0x10);
        copyString(slot + 0x20, x + 0x20);
    });
}

// RaidStart::Player (0x48): 13 words, a string at 0x34, 2 words
void pushBackRaidPlayer(Vector *self, uint32_t x) {
    grow<0x48, 0x4B5545, 0x4B5639>(self, [&](uint32_t slot) {
        copyWords(slot, x, 0x34);
        copyString(slot + 0x34, x + 0x34);
        copyWords(slot + 0x40, x + 0x40, 8);
    });
}

constexpr uint32_t StoryVector_copy = 0x4B5BC9;   // vector<Stage::Story>(const vector&)
constexpr uint32_t IntVector_copy = 0x4AF707;     // vector<int>(const vector&)

// StageEvent::Event (0x38) / Stage::Event (0x30): the stories (vector) at 0, a word, a byte, a string at 0x14,
// 4 words at 0x20 (StageEvent::Event: one more word and byte)
template <uint32_t Size, uint32_t SwapOut, uint32_t Dtor> void pushBackStageEvent(Vector *self, uint32_t x) {
    grow<Size, SwapOut, Dtor>(self, [&](uint32_t slot) {
        rw::call(StoryVector_copy, {slot, x});
        save<uint8_t>(slot + 0x10, load<uint8_t>(x + 0x10));
        save<uint32_t>(slot + 0xC, load<uint32_t>(x + 0xC));
        copyString(slot + 0x14, x + 0x14);
        copyWords(slot + 0x20, x + 0x20, 0x10);
        if constexpr (Size == 0x38) {
            save<uint8_t>(slot + 0x34, load<uint8_t>(x + 0x34));
            save<uint32_t>(slot + 0x30, load<uint32_t>(x + 0x30));
        }
    });
}

// Stage::Story (0x18): 3 words, a vector<int> at 0xC
void pushBackStory(Vector *self, uint32_t x) {
    grow<0x18, 0x4B5DA9, 0x4B5E2B>(self, [&](uint32_t slot) {
        copyWords(slot, x, 12);
        rw::call(IntVector_copy, {slot + 0xC, x + 0xC});
    });
}

// RaidReward151101::Reward (0xB0): plain up to 0x98, shared_ptrs at 0x98, 0xA0, 0xA8
void pushBackRaidReward151101(Vector *self, uint32_t x) {
    grow<0xB0, 0x4B62B9, 0x4B634F>(self, [&](uint32_t slot) {
        copyBytes(slot, x, 0x98);
        copyShared(slot + 0x98, x + 0x98);
        copyShared(slot + 0xA0, x + 0xA0);
        copyShared(slot + 0xA8, x + 0xA8);
    });
}

// RaidMember::Cooperator (0x50): 2 words, a string at 8, 14 words
void pushBackCooperator(Vector *self, uint32_t x) {
    grow<0x50, 0x4B6BA1, 0x4B6C9D>(self, [&](uint32_t slot) {
        copyWords(slot, x, 8);
        copyString(slot + 8, x + 8);
        for (uint32_t base : {0x14u, 0x24u, 0x34u})
            for (uint32_t k = 12;; k -= 4) {
                save<uint32_t>(slot + base + k, load<uint32_t>(x + base + k));
                if (!k) break;
            }
        save<uint32_t>(slot + 0x44, load<uint32_t>(x + 0x44));
        save<uint32_t>(slot + 0x48, load<uint32_t>(x + 0x48));
    });
}

// RaidBase (0xF0): plain up to 0xA8 (copied twice by the compiled code), a string at 0xB0, 0xF0 - 0xBC plain
void pushBackRaidBase(Vector *self, uint32_t x) {
    grow<0xF0, 0x4D7301, 0x4D7419>(self, [&](uint32_t slot) {
        copyBytes(slot, x, 0xA8);
        const uint32_t a = load<uint32_t>(x + 0xA8);
        save<uint32_t>(slot + 0xAC, load<uint32_t>(x + 0xAC));
        save<uint32_t>(slot + 0xA8, a);
        copyString(slot + 0xB0, x + 0xB0);
        for (uint32_t base : {0xBCu, 0xCCu, 0xDCu})
            for (uint32_t k = 12;; k -= 4) {
                save<uint32_t>(slot + base + k, load<uint32_t>(x + base + k));
                if (!k) break;
            }
        save<uint32_t>(slot + 0xEC, load<uint32_t>(x + 0xEC));
        copyBytes(slot, x, 0xA8);
    });
}

// RaidSubdueScrollView::SubdueData (0x50), moved: 4 words, a string at 0x10 (moved), 13 words
void pushBackSubdueData(Vector *self, uint32_t x) {
    grow<0x50, 0x5B99A1, 0x5B9AAD>(self, [&](uint32_t slot) {
        copyWords(slot, x, 0x10);
        moveTriple(slot + 0x10, x + 0x10);
        copyWords(slot + 0x1C, x + 0x1C, 0x34);
    });
}

// Avatar::InstanceInfo (0x464), moved: the SafeBuffer base (moved, then Avatar::Style's vtable), a string at 0x20
// (moved), 45 parts of 0x18 bytes (a byte, a shared_ptr, a string - both moved)
void pushBackInstanceInfo(Vector *self, uint32_t x) {
    grow<0x464, 0x5C94D1, 0x5C9531>(self, [&](uint32_t slot) {
        const uint32_t obj = rw::call(0x45A319, {slot, x});   // SafeBuffer::SafeBuffer(SafeBuffer&&)
        save<uint32_t>(obj, 0xF7E554);                          // vtable for Avatar::Style
        moveTriple(slot + 0x20, x + 0x20);
        for (uint32_t off = 0x2C; off < 0x464; off += 0x18) {
            const uint8_t kind = load<uint8_t>(x + off);
            moveShared(slot + off + 4, x + off + 4);
            const uint32_t a = load<uint32_t>(x + off + 0xC);
            save<uint32_t>(slot + off + 0x14, load<uint32_t>(x + off + 0x14));
            save<uint32_t>(slot + off + 0x10, load<uint32_t>(x + off + 0x10));
            save<uint32_t>(slot + off + 0xC, a);
            save<uint32_t>(x + off + 0x14, 0);
            save<uint32_t>(x + off + 0x10, 0);
            save<uint32_t>(x + off + 0xC, 0);
            save<uint8_t>(slot + off, kind);
        }
    });
}

// ChatBlockListScrollView::ChatBlock (0x1C): 3 words, a half word, a string at 0x10
void pushBackChatBlock(Vector *self, uint32_t x) {
    grow<0x1C, 0x627E11, 0x627E95>(self, [&](uint32_t slot) {
        copyWords(slot, x, 12);
        save<uint16_t>(slot + 0xC, load<uint16_t>(x + 0xC));
        copyString(slot + 0x10, x + 0x10);
    });
}

// ChatStampListScrollView::ChatStamp (0x1C): a word, a byte, a string at 8, 8 bytes (stored from the last)
void pushBackChatStamp(Vector *self, uint32_t x) {
    grow<0x1C, 0x6302F5, 0x6303CF>(self, [&](uint32_t slot) {
        save<uint8_t>(slot + 4, load<uint8_t>(x + 4));
        save<uint32_t>(slot, load<uint32_t>(x));
        copyString(slot + 8, x + 8);
        for (uint32_t k = 0x1B; k >= 0x14; k--) save<uint8_t>(slot + k, load<uint8_t>(x + k));
    });
}

// ChatScrollView::ChatMessage (0x40): 4 words and a byte, strings at 0x14 and 0x2C, 12 plain bytes at 0x20, a byte
void pushBackChatMessage(Vector *self, uint32_t x) {
    grow<0x40, 0x630E3D, 0x630F29>(self, [&](uint32_t slot) {
        copyWords(slot, x, 0x10);
        save<uint8_t>(slot + 0x10, load<uint8_t>(x + 0x10));
        copyString(slot + 0x14, x + 0x14);
        copyBytes(slot + 0x20, x + 0x20, 0xC);
        copyString(slot + 0x2C, x + 0x2C);
        save<uint8_t>(slot + 0x38, load<uint8_t>(x + 0x38));
    });
}

}  // namespace

// relocating (memcpy)
RW_FUNCTION(0x47D708, "std::vector<int>::__push_back_slow_path<int>", (pushBackWords<4, 0x47D819>));
RW_FUNCTION(0x4D6CC0, "std::vector<int>::__push_back_slow_path<const int&>", (pushBackWords<4, 0x47D819>));
RW_FUNCTION(0x4ED6F0, "std::vector<unsigned int>::__push_back_slow_path<unsigned int>", (pushBackWords<4, 0x4ED801>));
RW_FUNCTION(0x4F3350, "std::vector<long>::__push_back_slow_path<long>", (pushBackWords<4, 0x4F3461>));
RW_FUNCTION(0x4F9FCC, "std::vector<float>::__push_back_slow_path<float>", (pushBackWords<4, 0x4FA0DD>));
RW_FUNCTION(0x4FA108, "std::vector<float>::__push_back_slow_path<const float&>", (pushBackWords<4, 0x4FA0DD>));
RW_FUNCTION(0x4F8404, "std::vector<cocos2d::ui::Widget*>::__push_back_slow_path", (pushBackWords<4, 0x4F8515>));
RW_FUNCTION(0x526818, "std::vector<BaseSlideView::UpdateTarget>::__push_back_slow_path", (pushBackWords<4, 0x526929>));
RW_FUNCTION(0x5306D4, "std::vector<DeckMedalView*>::__push_back_slow_path", (pushBackWords<4, 0x5307E5>));
RW_FUNCTION(0x544D14, "std::vector<APIManager::Timezone>::__push_back_slow_path", (pushBackWords<4, 0x544605>));
RW_FUNCTION(0x5F94AC, "std::vector<AvatarActor*>::__push_back_slow_path", (pushBackWords<4, 0x5F95BD>));
RW_FUNCTION(0x60C10C, "std::vector<cocos2d::LWFNode*>::__push_back_slow_path", (pushBackWords<4, 0x60C21D>));
RW_FUNCTION(0x64C430, "std::vector<Avatar::Motion>::__push_back_slow_path", (pushBackWords<4, 0x64C541>));
RW_FUNCTION(0x6A5D9C, "std::vector<cocos2d::ui::ImageView*>::__push_back_slow_path", (pushBackWords<4, 0x6A5EAD>));
RW_FUNCTION(0x6C5B58, "std::vector<Avatar::Expression>::__push_back_slow_path<const Expression&>", (pushBackWords<4, 0x6C5C69>));
RW_FUNCTION(0x6C5C94, "std::vector<Avatar::Expression>::__push_back_slow_path<Expression>", (pushBackWords<4, 0x6C5C69>));
RW_FUNCTION(0x748C70, "std::vector<IBuffProtocol*>::__push_back_slow_path", (pushBackWords<4, 0x748D81>));
RW_FUNCTION(0x748DAC, "std::vector<IStatusProtocol*>::__push_back_slow_path", (pushBackWords<4, 0x748EBD>));
RW_FUNCTION(0x7531E0, "std::vector<EnemyActor*>::__push_back_slow_path", (pushBackWords<4, 0x7532F1>));
RW_FUNCTION(0x75CCCC, "std::vector<MapPlacementObject*>::__push_back_slow_path", (pushBackWords<4, 0x75CDDD>));
RW_FUNCTION(0x75CE08, "std::vector<MapNode*>::__push_back_slow_path", (pushBackWords<4, 0x5DB1A1>));
RW_FUNCTION(0x75D728, "std::vector<MapNonPlayableCharacter::Info*>::__push_back_slow_path", (pushBackWords<4, 0x75D839>));
RW_FUNCTION(0x75D864, "std::vector<MapMaterial::Info*>::__push_back_slow_path", (pushBackWords<4, 0x75D975>));
RW_FUNCTION(0x75D9A0, "std::vector<MapTreasure::Info*>::__push_back_slow_path", (pushBackWords<4, 0x75DAB1>));
RW_FUNCTION(0x75DADC, "std::vector<MapTransferInfo*>::__push_back_slow_path", (pushBackWords<4, 0x75DBED>));
RW_FUNCTION(0x75DC18, "std::vector<MapEnemyTeamInfo*>::__push_back_slow_path", (pushBackWords<4, 0x75DD29>));
RW_FUNCTION(0x5DB090, "std::vector<MapNode*>::__push_back_slow_path<MapNode* const&>", (pushBackWords<4, 0x5DB1A1>));
RW_FUNCTION(0x5DBC44, "std::vector<MapPrizes*>::__push_back_slow_path<MapPrizes* const&>", (pushBackWords<4, 0x5DBD55>));
RW_FUNCTION(0x5DEE60, "std::vector<MapPrize*>::__push_back_slow_path<MapPrize* const&>", (pushBackWords<4, 0x5DEF71>));
RW_FUNCTION(0x70C6D8, "std::vector<ImageButton*>::__push_back_slow_path<ImageButton* const&>", (pushBackWords<4, 0x70C7E9>));
RW_FUNCTION(0x758594, "std::vector<MapGateMarker*>::__push_back_slow_path<MapGateMarker* const&>", (pushBackWords<4, 0x7586A5>));
RW_FUNCTION(0x7586D0, "std::vector<MapNonPlayableCharacter*>::__push_back_slow_path<MapNonPlayableCharacter* const&>", (pushBackWords<4, 0x7587E1>));
RW_FUNCTION(0x75880C, "std::vector<MapTreasure*>::__push_back_slow_path<MapTreasure* const&>", (pushBackWords<4, 0x75891D>));
RW_FUNCTION(0x758948, "std::vector<MapMaterial*>::__push_back_slow_path<MapMaterial* const&>", (pushBackWords<4, 0x758A59>));
RW_FUNCTION(0x758A84, "std::vector<MapFieldLayer*>::__push_back_slow_path<MapFieldLayer* const&>", (pushBackWords<4, 0x758B95>));
RW_FUNCTION(0x7C9260, "std::vector<cocos2d::Component*>::__push_back_slow_path<Component* const&>", (pushBackWords<4, 0x7C9235>));
RW_FUNCTION(0x4B33F8, "std::vector<unsigned long long>::__push_back_slow_path<unsigned long long>", (pushBackWords<8, 0x4B350D>));
RW_FUNCTION(0x4ECB6C, "std::vector<unsigned long long>::__push_back_slow_path<const unsigned long long&>", (pushBackWords<8, 0x4B350D>));
RW_FUNCTION(0x4B38E4, "std::vector<hole::network::api::ColosseumClear::ClearColosseum>::__push_back_slow_path", (pushBackWords<8, 0x4B39F9>));
RW_FUNCTION(0x4F82C0, "std::vector<IScrollProtocol::UpdateData>::__push_back_slow_path", (pushBackWords<8, 0x4F83D5>));
RW_FUNCTION(0x59DC54, "std::vector<BaseStageScrollView::UpdateData>::__push_back_slow_path", (pushBackWords<8, 0x59DD69>));
RW_FUNCTION(0x4B2358, "std::vector<hole::network::api::RankingReward::Result>::__push_back_slow_path", (pushBackWords<0xC, 0x4B249D>));
RW_FUNCTION(0x4B3C48, "std::vector<hole::network::api::UserShop::Benefit>::__push_back_slow_path", (pushBackWords<0xC, 0x4B3D8D>));
RW_FUNCTION(0x4B2014, "std::vector<hole::network::api::MypageInfo::OpenBanner>::__push_back_slow_path", (pushBackWords<0x10, 0x4B2129>));
RW_FUNCTION(0x4B26C8, "std::vector<hole::network::api::UnionRanking::Info>::__push_back_slow_path", (pushBackWords<0x10, 0x4B27DD>));
RW_FUNCTION(0x4B3AC4, "std::vector<hole::network::api::Colosseum::Stage>::__push_back_slow_path", (pushBackWords<0x10, 0x4B3BD9>));
RW_FUNCTION(0x4D4B98, "std::vector<hole::network::api::UserTitle::Title>::__push_back_slow_path", (pushBackWords<0x10, 0x4D4CAD>));
RW_FUNCTION(0x4D5108, "std::vector<hole::network::api::UserMaterial::Material>::__push_back_slow_path", (pushBackWords<0x10, 0x4D521D>));
RW_FUNCTION(0x583CA8, "std::vector<BattleResultDialog::LevelUpSkill>::__push_back_slow_path", (pushBackWords<0x10, 0x583DBD>));
RW_FUNCTION(0x7645F4, "std::vector<StageUtil::SymbolEnemyListInfo>::__push_back_slow_path", (pushBackWords<0x10, 0x764709>));
RW_FUNCTION(0x4D4CDC, "std::vector<hole::network::api::UserAvatarParts::Part>::__push_back_slow_path", (pushBackWords<0x18, 0x4D4E21>));
RW_FUNCTION(0x51B78C, "std::vector<MyRecordScrollView::RecordData>::__push_back_slow_path", (pushBackWords<0x18, 0x51B8D1>));
RW_FUNCTION(0x487310, "std::vector<hole::network::api::UserSkill::Skill>::__push_back_slow_path<const Skill&>", (pushBackWords<0x28, 0x487459>));
RW_FUNCTION(0x4B00A4, "std::vector<hole::network::api::UserSkill::Skill>::__push_back_slow_path<Skill>", (pushBackWords<0x28, 0x487459>));
RW_FUNCTION(0x70CB94, "std::vector<hole::network::api::UserAvatar::Coordinate>::__push_back_slow_path", (pushBackWords<0x30, 0x70CCDD>));
RW_FUNCTION(0x4D4E58, "std::vector<hole::network::api::UserMedal::Medal>::__push_back_slow_path", (pushBackMemcpy<0x48, 0x4D4F95>));
RW_FUNCTION(0x4D494C, "std::vector<hole::network::api::UserKeyblade::Keyblade>::__push_back_slow_path<Keyblade>", (pushBackMemcpy<0x58, 0x4D4A89>));
RW_FUNCTION(0x4D4FCC, "std::vector<hole::network::api::UserKeyblade::Keyblade>::__push_back_slow_path<const Keyblade&>", (pushBackMemcpy<0x58, 0x4D4A89>));
RW_FUNCTION(0x494044, "std::vector<char>::__push_back_slow_path<const char&>", pushBackByte);
RW_FUNCTION(0x4940EC, "std::vector<char>::__push_back_slow_path<char>", pushBackByte);
RW_FUNCTION(0x6A5CF4, "std::vector<signed char>::__push_back_slow_path<signed char>", pushBackByte);
RW_FUNCTION(0x482A18, "std::vector<Date>::__push_back_slow_path<const Date&>", pushBackDate);

// via a split buffer
RW_FUNCTION(0x484464, "std::vector<std::string>::__push_back_slow_path<std::string>", pushBackStringMove);
RW_FUNCTION(0x48D890, "std::vector<std::string>::__push_back_slow_path<const std::string&>", pushBackStringCopy);
RW_FUNCTION(0x487A38, "std::vector<shared_ptr<MedalDataManager::UserMedalData>>::__push_back_slow_path", (pushBackSharedCopy<0x487B31, 0x487B7B>));
RW_FUNCTION(0x599228, "std::vector<shared_ptr<const master::EvCampaign>>::__push_back_slow_path", (pushBackSharedCopy<0x599321, 0x59936B>));
RW_FUNCTION(0x6BC884, "std::vector<shared_ptr<const master::Sphere>>::__push_back_slow_path", (pushBackSharedCopy<0x6BC97D, 0x6BC9C7>));
RW_FUNCTION(0x6FB494, "std::vector<shared_ptr<const master::LoginBonus>>::__push_back_slow_path<const&>", (pushBackSharedCopy<0x6FB40D, 0x6FB457>));
RW_FUNCTION(0x70B8D0, "std::vector<shared_ptr<const master::AvatarParts>>::__push_back_slow_path", (pushBackSharedCopy<0x70B9C9, 0x70BA13>));
RW_FUNCTION(0x6FB314, "std::vector<shared_ptr<const master::LoginBonus>>::__push_back_slow_path<&&>", (pushBackSharedMove<0x6FB40D, 0x6FB457>));
RW_FUNCTION(0x4946D4, "std::vector<std::vector<char>>::__push_back_slow_path<vector<char>>", (pushBackVectorMove<0x4947E5, 0x494851>));
RW_FUNCTION(0x6B5C68, "std::vector<std::vector<shared_ptr<UserMedalData>>>::__push_back_slow_path", (pushBackVectorMove<0x6B5D79, 0x6B5DE5>));
RW_FUNCTION(0x70DCC4, "std::vector<std::vector<int>>::__push_back_slow_path<vector<int>>", (pushBackVectorMove<0x70DDD5, 0x70DE41>));
RW_FUNCTION(0x4B35B0, "std::vector<hole::network::api::GachaList::Gacha>::__push_back_slow_path", (pushBackCtor<0x48, 0x4B36BD, 0x4B3779, 0x4B37E5>));
RW_FUNCTION(0x4B6430, "std::vector<hole::network::api::RaidReward::Reward>::__push_back_slow_path", (pushBackCtor<0x148, 0x4B6541, 0x4B6759, 0x4B67DD>));
RW_FUNCTION(0x4DDF04, "std::vector<ChatManager::Message>::__push_back_slow_path", (pushBackCtor<0x4C, 0x4DE011, 0x4DE171, 0x4DE205>));
RW_FUNCTION(0x4DE87C, "std::vector<ChatManager::Block>::__push_back_slow_path", (pushBackCtor<0x44, 0x4DE989, 0x4DEAC9, 0x4DEB5F>));
RW_FUNCTION(0x51517C, "std::vector<BattleResultScrollView::ResutData>::__push_back_slow_path", (pushBackCtor<0x44, 0x515289, 0x51537B, 0x5153E7>));
RW_FUNCTION(0x5B2908, "std::vector<DrawScrollView::GachaData>::__push_back_slow_path", (pushBackCtor<0x40, 0x5B2A0D, 0x5B2B27, 0x5AE2CD>));
RW_FUNCTION(0x622B64, "std::vector<function<void(bool)>>::__push_back_slow_path<const&>", (pushBackCtor<0x18, 0x622C71, 0x622CBB, 0x45C84D>));
RW_FUNCTION(0x65E98C, "std::vector<function<void(bool)>>::__push_back_slow_path<&&>", (pushBackCtor<0x18, 0x622C71, 0x622CBB, 0x47BC69>));
RW_FUNCTION(0x622D00, "std::vector<function<bool()>>::__push_back_slow_path", (pushBackCtor<0x18, 0x622E0D, 0x622E57, 0x46A3B5>));
RW_FUNCTION(0x64A064, "std::vector<cocos2d::Vec2>::__push_back_slow_path<const Vec2&>", (pushBackCtor<8, 0x64A161, 0x64A1AB, 0x8E4C03>));
RW_FUNCTION(0x64DFA8, "std::vector<cocos2d::Vec2>::__push_back_slow_path<Vec2>", (pushBackCtor<8, 0x64A161, 0x64A1AB, 0x8E4C03>));
RW_FUNCTION(0x64DE08, "std::vector<std::vector<unsigned int>>::__push_back_slow_path<const&>", (pushBackCtor<0xC, 0x64DF15, 0x64DF81, 0x641E91>));
RW_FUNCTION(0x4B1DC0, "std::vector<hole::network::api::UserLink::Info>::__push_back_slow_path", pushBackUserLinkInfo);
RW_FUNCTION(0x4B2158, "std::vector<hole::network::api::MypageInfo::AdminText>::__push_back_slow_path", pushBackAdminText);
RW_FUNCTION(0x4B24D4, "std::vector<hole::network::api::UnionRanking::Ranker>::__push_back_slow_path", pushBackRanker);
RW_FUNCTION(0x4B2868, "std::vector<hole::network::api::ColosseumRanking::User>::__push_back_slow_path", pushBackColosseumUser);
RW_FUNCTION(0x4B2FD0, "std::vector<hole::network::api::LuxRanking::User>::__push_back_slow_path", pushBackLuxUser);
RW_FUNCTION(0x4B2B38, "std::vector<hole::network::api::PartyRanking::Info>::__push_back_slow_path", pushBackPartyRankingInfo);
RW_FUNCTION(0x4B3EA8, "std::vector<hole::network::api::PartyApplicationList::PartyEx>::__push_back_slow_path", pushBackPartyEx);
RW_FUNCTION(0x4B41D8, "std::vector<hole::network::api::PartyInviteList::Offer>::__push_back_slow_path", (pushBackOffer<0x4B433D, 0x4B440D>));
RW_FUNCTION(0x4B449C, "std::vector<hole::network::api::PartyApplication::Applicant>::__push_back_slow_path", (pushBackOffer<0x4B4601, 0x4B46D1>));
RW_FUNCTION(0x4B4718, "std::vector<hole::network::api::PartyMemberList::Detail>::__push_back_slow_path", pushBackMemberDetail);
RW_FUNCTION(0x4B4B5C, "std::vector<hole::network::api::PartyNotice::NewcomerUser>::__push_back_slow_path", pushBackNewcomer);
RW_FUNCTION(0x4B53F4, "std::vector<hole::network::api::RaidStart::Player>::__push_back_slow_path", pushBackRaidPlayer);
RW_FUNCTION(0x4B5924, "std::vector<hole::network::api::StageEvent::Event>::__push_back_slow_path", (pushBackStageEvent<0x38, 0x4B5A95, 0x4B5B81>));
RW_FUNCTION(0x4B5EA8, "std::vector<hole::network::api::Stage::Event>::__push_back_slow_path", (pushBackStageEvent<0x30, 0x4B600D, 0x4B60D1>));
RW_FUNCTION(0x4B5C8C, "std::vector<hole::network::api::Stage::Story>::__push_back_slow_path", pushBackStory);
RW_FUNCTION(0x4B6174, "std::vector<hole::network::api::RaidReward151101::Reward>::__push_back_slow_path", pushBackRaidReward151101);
RW_FUNCTION(0x4B6A24, "std::vector<hole::network::api::RaidMember::Cooperator>::__push_back_slow_path", pushBackCooperator);
RW_FUNCTION(0x4D7128, "std::vector<hole::network::api::RaidBase>::__push_back_slow_path", pushBackRaidBase);
RW_FUNCTION(0x5B9868, "std::vector<RaidSubdueScrollView::SubdueData>::__push_back_slow_path", pushBackSubdueData);
RW_FUNCTION(0x5C9340, "std::vector<Avatar::InstanceInfo>::__push_back_slow_path", pushBackInstanceInfo);
RW_FUNCTION(0x627CD0, "std::vector<ChatBlockListScrollView::ChatBlock>::__push_back_slow_path", pushBackChatBlock);
RW_FUNCTION(0x630180, "std::vector<ChatStampListScrollView::ChatStamp>::__push_back_slow_path", pushBackChatStamp);
RW_FUNCTION(0x630CBC, "std::vector<ChatScrollView::ChatMessage>::__push_back_slow_path", pushBackChatMessage);
