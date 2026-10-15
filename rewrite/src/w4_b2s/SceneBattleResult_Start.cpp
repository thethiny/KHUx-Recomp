// SceneBattleResult::startResult: set up the result dialog of a finished quest (stage, raid or colosseum), open it
// and collect the extra dialogs that follow (new keyblades, level up, new Lux rank, user title).
//
// The original keeps a 0x2420 byte frame (battle record arrays of 8000 / 800 bytes, prize tables, function
// objects); it is reproduced here with the same offsets (guest::Frame) and its calls keep the callee frames where
// they were (call_exact).
#include "SceneBattleResult.hpp"

#include "B2sCommon.hpp"

using namespace b2s;

namespace {
// ---- guest functions
constexpr Addr BattleResultDialog_create = 0x57DA8B;
constexpr Addr BattleResultDialog_setCloseEvent = 0x582B81;
constexpr Addr BattleResultDialog_openStageResult = 0x57E04D;
constexpr Addr BattleResultDialog_openRaidResult = 0x57FAB5;       // (dialog, Node*, TimeStatus)
constexpr Addr BattleResultDialog_openColosseumResult = 0x580D7D;
constexpr Addr StageClear_Property_assign = 0x683DB9;              // StageClear::Property::operator=
constexpr Addr EvStage_create = 0x782D2D;
constexpr Addr Stage_create = 0x78E4E9;
constexpr Addr StageUtil_getEnemyDropPrizes = 0x763C1D;            // (MapPrizes::Items*, int, int, PlayStyle)
constexpr Addr StageUtil_getTreasurePrizes = 0x763DF9;
constexpr Addr audio_Manager_getInstance = 0x5BECFD;
constexpr Addr audio_Manager_load = 0x5BEEF5;
constexpr Addr VectorInt_pushBackSlow = 0x47D709;                  // std::vector<int>::__push_back_slow_path
constexpr Addr VectorInt_dtor = 0x478961;                          // std::__vector_base<int>::~__vector_base
constexpr Addr Array3_at = 0x5149A1;                               // std::array<int, 3>::at(index)
constexpr Addr AddResultReward = 0x684099;                         // sub_684098(holder, a, b, c, flag)
constexpr Addr SceneBattleResult_addResultRewardKeyBlade = 0x683E65;

// lambda __func vtables (closeEvent callbacks of the three result kinds)
constexpr Addr VT_closeStage = 0xF64550, VT_closeRaid = 0xF64524, VT_closeColosseum = 0xF644F8;

constexpr uint32_t SOUND_LEVEL_UP = 509000050;

// ---- frame offsets (the original's sp offsets)
constexpr uint32_t F_L = 0x34;                 // the local record: [0] this, then the working vectors
constexpr uint32_t F_VEC_B_TMP3 = 0x38;        //   value slots of vector B pushes (3, 2, 1)
constexpr uint32_t F_VEC_B_TMP2 = 0x3C;
constexpr uint32_t F_VEC_B_TMP1 = 0x40;
constexpr uint32_t F_VEC_B = 0x44;             //   std::vector<int> B: begin, end, cap
constexpr uint32_t F_VEC_A_TMP = 0x50;         //   value slot of vector A pushes
constexpr uint32_t F_VEC_A = 0x54;             //   std::vector<int> A: begin, end, cap
constexpr uint32_t F_STAGE = 0x60;             //   shared_ptr<master::Stage>: object, control block
constexpr uint32_t F_EVSTAGE = 0x68;           //   shared_ptr<master::EvStage>
constexpr uint32_t F_TEMP = 0x70;              //   scratch ints
constexpr uint32_t F_LEVEL_COUNT = 0x74;
constexpr uint32_t F_BYTE = 0x7B;
constexpr uint32_t F_VALUE = 0x7C;             //   scratch value read from SafeBuffers
constexpr uint32_t F_STAGE_ID = 0x80;
constexpr uint32_t F_ARR_C = 0x84, F_ARR_B = 0x90, F_ARR_A = 0x9C;       // std::array<int, 3> reads (loop)
constexpr uint32_t F_ARR_ST_C = 0xA8, F_ARR_ST_B = 0xB4, F_ARR_ST_A = 0xC0;   // (stage rows)
constexpr uint32_t F_DROPS = 0xCC;             // three ints: enemy cut-ins
constexpr uint32_t F_TREASURE_PRIZES = 0xD8;   // 4 x 16: MapPrizes::Items
constexpr uint32_t F_TREASURE_REC = 0x118;     // 100 x 8 bytes: (stage, area) pairs of opened treasures
constexpr uint32_t F_ENEMY_PRIZES = 0x438;     // 4 x 16
constexpr uint32_t F_ENEMY_REC = 0x478;        // 1000 x 8 bytes: (stage, area) pairs of defeated enemies
constexpr uint32_t F_FN_RAID = 0x23D0, F_FN_STAGE = 0x23E8, F_FN_COLOSSEUM = 0x23B8;

// the 4 prize slots {1, 0, 0, 0 (byte)} of a MapPrizes::Items table
void initPrizes(Addr items) {
    for (uint32_t i = 0; i < 4; i++) {
        const Addr e = items + i * 16;
        st32(e, 1);
        st32(e + 4, 0);
        st32(e + 8, 0);
        st8(e + 12, 0);
    }
}

// std::upper_bound over a sorted int range, with the original's halving loop (signed compares)
Addr upperBound(Addr first, Addr last, int32_t value) {
    Addr it = first;
    int32_t len = int32_t(last - first) >> 2;
    while (len != 0) {
        const int32_t half = int32_t(uint32_t(len) + (uint32_t(len) >> 31)) >> 1;
        const Addr mid = it + uint32_t(half) * 4;
        if (int32_t(ld32(mid)) > value) {
            len = half;
        } else {
            it = mid + 4;
            len = len - 1 - half;
        }
    }
    return it;
}

// v.push_back(value) where the value slot `tmp` lives in the frame; `slow` forces the out-of-line path
void pushBackInt(Addr vec, Addr tmp, uint32_t value, bool slow) {
    const uint32_t end = ld32(vec + 4), cap = ld32(vec + 8);
    if (slow || end >= cap) {
        cx(VectorInt_pushBackSlow, {vec, tmp});
    } else {
        uint32_t base = 0;
        if (end != 0) {
            st32(end, value);
            base = ld32(vec + 4);
        }
        st32(vec + 4, base + 4);
    }
}

// the reward record `holder`: (record, a, b, c, flag)
void addReward(Addr holder, uint32_t a, uint32_t b, uint32_t c, uint32_t flag) {
    cx(AddResultReward, {holder, a, b, c, flag});
}
}  // namespace

void SceneBattleResult::startResult(SceneBattleResult *self) {
    guest::Frame F(0x2420);
    const Addr me = rw::guest_addr(self);
    const Addr L = F.addr(F_L);
    const Addr dialogSlot = me + 0x2C8;
    auto fa = [&](uint32_t off) { return F.addr(off); };
    auto fw = [&](uint32_t off) -> uint32_t & { return *F.at<uint32_t>(off); };
    auto sm = [&]() { return cx(SCENEMANAGER_GETINSTANCE, {}); };
    // SafeBuffer::read(sb, out, data + offset, n, offset)
    auto readSb = [&](Addr sb, Addr out, uint32_t offset, uint32_t n) {
        cx(SAFEBUFFER_READ, {sb, out, ld32(sb + 0xC) + offset, n, offset});
    };
    // clearData = (obj, ctrl) of the clear response, taken from a field pair of `src`
    auto setClearData = [&](Addr src, uint32_t objOff, uint32_t ctrlOff) {
        const uint32_t obj = ld32(src + objOff);
        const uint32_t ctrl = ld32(src + ctrlOff);
        sharedAddRef(ctrl);
        st32(me + 0x2B4, obj);
        const uint32_t old = ld32(me + 0x2B8);
        st32(me + 0x2B8, ctrl);
        sharedRelease(old);
    };
    // dialog->setCloseEvent(lambda(this)); destroyed afterwards
    auto setCloseEvent = [&](Addr fn, Addr vtable) {
        st32(fn + 16, fn);
        st32(fn, vtable);
        st32(fn + 4, me);
        cx(BattleResultDialog_setCloseEvent, {ld32(dialogSlot), fn});
        fnDestroy(fn);
    };
    auto popupLayer = [&]() { return cx(SCENEMANAGER_GETPOPUPLAYER, {sm()}); };

    st32(dialogSlot, cx(BattleResultDialog_create, {}));
    const uint32_t kind = self->resultKind;
    bool tail = false;           // the common ending (clear data compare, level up sound) runs
    Addr tailApi = 0;

    if (kind == 0) {
        // ---- stage result
        const Addr api = cx(APIMANAGER_GETINSTANCE, {});
        cx(StageClear_Property_assign, {me + 0x28C, ld32(api + 0x144) + 0x54});
        setClearData(ld32(api + 0x144), 0x3C, 0x40);
        st8(me + 0x2BC, 1);
        {
            const Addr clear = ld32(api + 0x144);
            readSb(clear, fa(F_VALUE), 0, 1);          // read(clear, out, data + 0, 1, 0)
        }
        if (ld8(fa(F_VALUE)) != 0) {
            st8(sm() + 0x2F6, 1);
            bool eventStage = ld32(sm() + 0x28C) == 2;
            if (!eventStage) eventStage = ld32(sm() + 0x28C) == 3;
            if (eventStage) {
                st8(sm() + 0x2F6, 0);
                if (ld32(sm() + 0x2F0) != 0) {
                    cx(EvStage_create, {fa(F_EVSTAGE), ld32(sm() + 0x2F0)});
                    const Addr ev = ld32(fa(F_EVSTAGE));
                    if (ev != 0) {
                        readSb(ev, fa(F_TEMP), 0x28, 4);
                        if (ld32(fa(F_TEMP)) == 3) st8(sm() + 0x2F6, 1);
                    }
                    sharedRelease(ld32(fa(F_EVSTAGE + 4)));
                }
            }
        }
        {
            const Addr api2 = cx(APIMANAGER_GETINSTANCE, {});
            const Addr sb = ld32(api2 + 0x8C);
            readSb(sb, fa(F_VALUE), 0x20, 4);
            const int32_t a = int32_t(ld32(fa(F_VALUE)));
            readSb(ld32(me + 0x28C), fa(F_VALUE), 0x20, 4);
            const int32_t b = int32_t(ld32(fa(F_VALUE)));
            if (a < b) st8(sm() + 0x2F5, 1);
        }
        setCloseEvent(fa(F_FN_STAGE), VT_closeStage);
        {
            const Addr dialog = ld32(dialogSlot);
            cx(BattleResultDialog_openStageResult, {dialog, popupLayer()});
        }
        tail = true;
        tailApi = cx(APIMANAGER_GETINSTANCE, {});
    } else if (kind == 1) {
        // ---- raid result
        const Addr api = cx(APIMANAGER_GETINSTANCE, {});
        cx(StageClear_Property_assign, {me + 0x28C, ld32(api + 0x180) + 0x20});
        setClearData(ld32(api + 0x180), 0x10, 0x14);
        setCloseEvent(fa(F_FN_RAID), VT_closeRaid);
        {
            const Addr dialog = ld32(dialogSlot);
            const Addr layer = popupLayer();
            const Addr raid = ld32(api + 0x180);
            cx(BattleResultDialog_openRaidResult, {dialog, layer, ld32(raid)});
        }
        tail = true;
        tailApi = cx(APIMANAGER_GETINSTANCE, {});
    } else if (kind == 2) {
        // ---- colosseum result
        const Addr app = cx(APPSTATE_GETINSTANCE, {});
        readSb(ld32(app + 0x20), fa(F_VALUE), 8, 4);
        bool hasResult = false;
        if (ld32(fa(F_VALUE)) != 0) {
            const Addr api = cx(APIMANAGER_GETINSTANCE, {});
            cx(StageClear_Property_assign, {me + 0x28C, ld32(api + 0x268) + 0x34});
            setClearData(ld32(api + 0x268), 0x24, 0x28);
            st8(me + 0x2BD, 1);
            hasResult = true;
        }
        setCloseEvent(fa(F_FN_COLOSSEUM), VT_closeColosseum);
        {
            const Addr dialog = ld32(dialogSlot);
            cx(BattleResultDialog_openColosseumResult, {dialog, popupLayer()});
        }
        const Addr api = cx(APIMANAGER_GETINSTANCE, {});
        if (hasResult) {
            tail = true;
            tailApi = api;
        }
    } else {
        cx(APIMANAGER_GETINSTANCE, {});
    }

    bool v35 = false;
    if (tail) {
        // ---- compare the Lux rank in the clear data with the user's: level up sound
        readSb(ld32(me + 0x294), fa(F_VALUE), 8, 4);
        const int32_t fromRank = int32_t(ld32(fa(F_VALUE)));
        readSb(ld32(tailApi + 0x94), fa(F_VALUE), 8, 4);
        const int32_t toRank = int32_t(ld32(fa(F_VALUE)));
        v35 = true;
        if (fromRank < toRank) {
            st8(me + 0x2BE, 1);
            cx(audio_Manager_load, {cx(audio_Manager_getInstance, {}), SOUND_LEVEL_UP});
        }
    }

    {
        const Addr api = cx(APIMANAGER_GETINSTANCE, {});
        if (ld32(api + 0x30C) != ld32(api + 0x308)) st8(me + 0x2BF, 1);
    }
    if (!v35) return;
    if (ld32(ld32(me + 0x2B4) + 0x20) != 0) st8(me + 0x2C0, 1);
    if (ld8(me + 0x2BC) == 0) return;

    // ---- stage / raid cleared: build the keyblade reward list ------------------------------------------------
    // the old list is destroyed (three strings per entry, last first)
    {
        const Addr first = ld32(me + 0x2CC);
        for (;;) {
            const Addr end = ld32(me + 0x2D0);
            if (end == first) break;
            st32(me + 0x2D0, end - 0x44);
            if (ld8(end - 0xC) & 1) cx(OPERATOR_DELETE, {ld32(end - 4)});
            if (ld8(end - 0x34) & 1) cx(OPERATOR_DELETE, {ld32(end - 0x2C)});
            if (ld8(end - 0x40) & 1) cx(OPERATOR_DELETE, {ld32(end - 0x38)});
        }
    }
    st32(me + 0x2C4, 0);
    const Addr app = cx(APPSTATE_GETINSTANCE, {});
    const Addr userData = ld32(app + 0x20);
    const Addr userDataCtrl = ld32(app + 0x24);
    sharedAddRef(userDataCtrl);
    const Addr recEnd1 = F.addr(F_ENEMY_REC + 0x1F40);
    {
        const Addr base = ld32(userData + 0xC);
        cx(SAFEBUFFER_READ, {userData, fa(F_ENEMY_REC), base + 0x1F78, 0x1F40, 0x1F78});
    }
    // enemies defeated: every record (stage, area) that is not -1 adds the drop prizes
    for (Addr cur = fa(F_ENEMY_REC); cur != recEnd1; cur += 8) {
        if (ld32(cur) == 0xFFFFFFFFu) continue;
        initPrizes(fa(F_ENEMY_PRIZES));
        initPrizes(fa(F_ENEMY_PRIZES));
        cx(StageUtil_getEnemyDropPrizes, {fa(F_ENEMY_PRIZES), ld32(cur), ld32(cur + 4), 0});
        for (uint32_t j = 0; j < 4; j++) {
            const Addr prize = fa(F_ENEMY_PRIZES) + j * 16 + 8;
            if (ld32(prize) == 0) continue;
            const uint32_t id = ld32(cur + 4);
            const Addr end = ld32(me + 0x2D0);
            Addr hit = 0;
            if (ld32(me + 0x2CC) != end) {
                Addr q = ld32(me + 0x2CC) + 0x1C;
                for (;;) {
                    if (ld32(q) == id && ld32(q + 8) == j) { hit = q; break; }
                    q += 0x44;
                    if (q - end == 0x1C) break;
                }
            }
            if (hit) {
                st32(hit + 0xC, ld32(hit + 0xC) + 1);
                st32(hit + 0x14, ld32(hit + 0x14) + ld32(prize));
            } else {
                cx(SceneBattleResult_addResultRewardKeyBlade, {me, id, j});
            }
        }
    }
    // treasures opened: the same with the treasure prizes
    {
        const Addr base = ld32(userData + 0xC);
        cx(SAFEBUFFER_READ, {userData, fa(F_TREASURE_REC), base + 0x3EB8, 0x320, 0x3EB8});
    }
    for (Addr cur = fa(F_TREASURE_REC); cur != fa(F_ENEMY_PRIZES); cur += 8) {
        if (ld32(cur) == 0xFFFFFFFFu) continue;
        initPrizes(fa(F_TREASURE_PRIZES));
        initPrizes(fa(F_TREASURE_PRIZES));
        cx(StageUtil_getTreasurePrizes, {fa(F_TREASURE_PRIZES), ld32(cur), ld32(cur + 4), 0});
        for (uint32_t j = 0; j < 4; j++) {
            const Addr prize = fa(F_TREASURE_PRIZES) + j * 16 + 8;
            if (ld32(prize) == 0) continue;
            const uint32_t id = ld32(cur + 4);
            const Addr end = ld32(me + 0x2D0);
            Addr hit = 0;
            if (ld32(me + 0x2CC) != end) {
                Addr q = ld32(me + 0x2CC) + 0x1C;
                for (;;) {
                    if (ld32(q) == id && ld32(q + 8) == j) { hit = q; break; }
                    q += 0x44;
                    if (q - end == 0x1C) break;
                }
            }
            if (hit) {
                st32(hit + 0xC, ld32(hit + 0xC) + 1);
                st32(hit + 0x14, ld32(hit + 0x14) + ld32(prize));
            } else {
                cx(SceneBattleResult_addResultRewardKeyBlade, {me, id, j});
            }
        }
    }

    // ---- the stage row and the cut-ins of the stage
    {
        const Addr api = cx(APIMANAGER_GETINSTANCE, {});
        cx(SAFEBUFFER_READ, {api + 0x33C, fa(F_STAGE_ID), ld32(api + 0x348) + 4, 4, 4});
    }
    cx(Stage_create, {fa(F_STAGE), ld32(fa(F_STAGE_ID))});
    fw(F_VEC_A) = 0;
    fw(F_VEC_A + 4) = 0;
    fw(F_VEC_A + 8) = 0;
    {
        const Addr base = ld32(userData + 0xC);
        cx(SAFEBUFFER_READ, {userData, fa(F_DROPS), base + 0x4228, 0xC, 0x4228});
    }
    for (uint32_t k = 0; k < 3; k++) {
        const uint32_t value = ld32(fa(F_DROPS) + 4 * k);
        if (value == 0xFFFFFFFFu) break;
        fw(F_VEC_A_TMP) = value;
        pushBackInt(fa(F_VEC_A), fa(F_VEC_A_TMP), value, false);
    }

    // ---- which of the three rank rewards (1, 2, 3) the stage has and the record has not yet
    const Addr api = cx(APIMANAGER_GETINSTANCE, {});
    const Addr rankData = ld32(api + 0x13C);
    uint32_t loopIndex = 0;                      // [sp+0x20]
    fw(F_VEC_B) = 0;
    fw(F_VEC_B + 4) = 0;
    fw(F_VEC_B + 8) = 0;
    Addr rBegin = ld32(rankData + 0x4C), rEnd = ld32(rankData + 0x50);
    {
        // value 1
        Addr it = upperBound(rBegin, rEnd, 0);
        const bool skip = it != rEnd && int32_t(ld32(it)) < 2;
        if (!skip) {
            const Addr a = upperBound(ld32(fa(F_VEC_A)), ld32(fa(F_VEC_A + 4)), 0);
            if (a != ld32(fa(F_VEC_A + 4)) && !(int32_t(ld32(a)) > 1)) {
                fw(F_VEC_B_TMP1) = 1;
                cx(VectorInt_pushBackSlow, {fa(F_VEC_B), fa(F_VEC_B_TMP1)});
                rBegin = ld32(rankData + 0x4C);
                rEnd = ld32(rankData + 0x50);
            }
        }
    }
    {
        // value 2
        Addr it = upperBound(rBegin, rEnd, 1);
        const bool skip = it != rEnd && int32_t(ld32(it)) < 3;
        if (!skip) {
            const Addr a = upperBound(ld32(fa(F_VEC_A)), ld32(fa(F_VEC_A + 4)), 1);
            if (a != ld32(fa(F_VEC_A + 4)) && !(int32_t(ld32(a)) > 2)) {
                fw(F_VEC_B_TMP2) = 2;
                pushBackInt(fa(F_VEC_B), fa(F_VEC_B_TMP2), 2, false);
            }
        }
    }
    rBegin = ld32(rankData + 0x4C);
    rEnd = ld32(rankData + 0x50);
    {
        // value 3
        Addr it = upperBound(rBegin, rEnd, 2);
        const bool skip = it != rEnd && int32_t(ld32(it)) < 4;
        if (!skip) {
            const Addr a = upperBound(ld32(fa(F_VEC_A)), ld32(fa(F_VEC_A + 4)), 2);
            if (a != ld32(fa(F_VEC_A + 4)) && !(int32_t(ld32(a)) > 3)) {
                fw(F_VEC_B_TMP3) = 3;
                pushBackInt(fa(F_VEC_B), fa(F_VEC_B_TMP3), 3, false);
            }
        }
    }

    // ---- the stage's reward rows for those ranks
    fw(F_L) = me;
    {
        const Addr bEnd = ld32(fa(F_VEC_B + 4));
        for (Addr q = ld32(fa(F_VEC_B)); q != bEnd; q += 4) {
            const uint32_t rank = ld32(q) - 1;
            Addr stage = ld32(fa(F_STAGE));
            readSb(stage, fa(F_ARR_ST_A), 0x3A4, 0xC);
            const uint32_t a = ld32(cx(Array3_at, {fa(F_ARR_ST_A), rank}));
            stage = ld32(fa(F_STAGE));
            readSb(stage, fa(F_ARR_ST_B), 0x3B0, 0xC);
            const uint32_t b = ld32(cx(Array3_at, {fa(F_ARR_ST_B), rank}));
            stage = ld32(fa(F_STAGE));
            readSb(stage, fa(F_ARR_ST_C), 0x3E0, 0xC);
            const uint32_t c = ld32(cx(Array3_at, {fa(F_ARR_ST_C), rank}));
            addReward(L, a, b, c, 1);
        }
    }
    // ---- the first-clear reward rows, when the stage was cleared for the first time
    {
        const Addr api2 = cx(APIMANAGER_GETINSTANCE, {});
        const Addr clear = ld32(api2 + 0x144);
        cx(SAFEBUFFER_READ, {clear, fa(F_BYTE), ld32(clear + 0xC), 1, loopIndex});
    }
    if (ld8(fa(F_BYTE)) != 0) {
        for (;; loopIndex++) {
            Addr stage = ld32(fa(F_STAGE));
            readSb(stage, fa(F_LEVEL_COUNT), 0x190, 4);
            if (int32_t(loopIndex) >= int32_t(ld32(fa(F_LEVEL_COUNT)))) break;
            stage = ld32(fa(F_STAGE));
            readSb(stage, fa(F_ARR_A), 0x194, 0xC);
            const uint32_t a = ld32(cx(Array3_at, {fa(F_ARR_A), loopIndex}));
            stage = ld32(fa(F_STAGE));
            readSb(stage, fa(F_ARR_B), 0x1A0, 0xC);
            const uint32_t b = ld32(cx(Array3_at, {fa(F_ARR_B), loopIndex}));
            stage = ld32(fa(F_STAGE));
            readSb(stage, fa(F_ARR_C), 0x1D0, 0xC);
            const uint32_t c = ld32(cx(Array3_at, {fa(F_ARR_C), loopIndex}));
            addReward(L, a, b, c, 0);
        }
    }
    if (ld32(me + 0x2D0) != ld32(me + 0x2CC)) st8(me + 0x2C1, 1);
    cx(VectorInt_dtor, {fa(F_VEC_B)});
    cx(VectorInt_dtor, {fa(F_VEC_A)});
    sharedRelease(ld32(fa(F_STAGE + 4)));
    sharedRelease(userDataCtrl);
}

RW_FUNCTION(0x683264, "SceneBattleResult::startResult()", SceneBattleResult::startResult);
