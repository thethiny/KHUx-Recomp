// AdvStageScrollView: the event and colosseum rows of the quest list (stage frame, title box, colosseum info with
// its slide view of rounds). All five are static in practice: r0 is the row widget, not the scroll view.
#include "AdvStageScrollView.hpp"

using namespace scene;

namespace {
namespace lib {
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t Stage_create = 0x78E4E9;               // master::Stage::create(shared_ptr out, id)
constexpr uint32_t EvStage_create = 0x782D2D;             // master::EvStage::create(shared_ptr out, id)
constexpr uint32_t ColosseumStage_create = 0x77CF95;      // master::ColosseumStage::create(shared_ptr out, id)
constexpr uint32_t Colosseum_create = 0x77C4E9;           // master::Colosseum::create(shared_ptr out, id)
constexpr uint32_t throwOutOfRange = 0x47CCE1;            // std::__vector_base_common::__throw_out_of_range
constexpr uint32_t IntVector_assign = 0x47D5BD;           // vector<int>::assign(int* first, int* last)
constexpr uint32_t IntVector_dtor = 0x478961;             // std::__vector_base<int>::~__vector_base
constexpr uint32_t IntVector_copy = 0x4AF707;             // vector<int>::vector(const vector&)
constexpr uint32_t IntVector_pushBackSlowRef = 0x4D6CC1;  // vector<int>::__push_back_slow_path(const int&)
constexpr uint32_t IntVector_pushBackSlowMove = 0x47D709; // vector<int>::__push_back_slow_path(int&&)
constexpr uint32_t StageVector_copy = 0x54E489;           // vector<Colosseum::Stage>::vector(const vector&)
constexpr uint32_t StageVector_dtor = 0x4B3A95;           // __vector_base<Colosseum::Stage>::~__vector_base
constexpr uint32_t ImageView_loadTexture = 0x839731;      // (image, const string&, TextureResType)
constexpr uint32_t Text_setString = 0x83A2DD;
constexpr uint32_t TextAtlas_setString = 0x83AED7;
constexpr uint32_t setBossTypeImage = 0x46C4C5;           // SceneUtil::setBossTypeImage(ImageView*, int, bool)
constexpr uint32_t setSubMisstion = 0x5691E1;             // AdvStageScrollView::setSubMisstion(Widget*, int, const vector<int>&)
constexpr uint32_t getCampaignPoint = 0x4741BD;           // SceneUtil::getCampaignPoint(int, int, int, const vector<int>&, int, bool)
constexpr uint32_t pointToString = 0x506B61;              // AdvStageScrollView::pointToString(string out, int, int, bool)
constexpr uint32_t Date_makeLocalDate = 0x4927CD;         // (Date out, const Date*)
constexpr uint32_t TextData_get = 0x4928AD;               // (shared_ptr out, Type, id)
constexpr uint32_t TextData_format = 0x492929;            // (string out, Type, id, ...)
constexpr uint32_t FileUtils_getInstance = 0xA3F44D;
constexpr uint32_t Data_getBytes = 0x8ED90B;
constexpr uint32_t Data_dtor = 0x8ED89B;
constexpr uint32_t getInt16LE = 0x75991D;                 // StageManager::getInteger16bitFromLittleEndianBytes
constexpr uint32_t getInt32LE = 0x75992B;                 // StageManager::getInteger32bitFromLittleEndianBytes
constexpr uint32_t divsi3 = 0xCD5E8D;                     // __divsi3
constexpr uint32_t BaseSlideView_create = 0x534E79;
constexpr uint32_t BaseSlideView_setScreenWidth = 0x526595;   // (view, float)
constexpr uint32_t BaseSlideView_setItemIDs = 0x5260AD;       // (view, vector<int> by value, bool)
constexpr uint32_t BaseSlideView_setIndex = 0x526141;
constexpr uint32_t BaseSlideView_updateLayout = 0x525EB5;
constexpr uint32_t FunctionWidgetInt_swap = 0x54B1C5;         // std::function<Widget*(int)>::swap
constexpr uint32_t FunctionWidgetWidgetIntBool_swap = 0x547159; // std::function<Widget*(Widget*, int, bool)>::swap
constexpr uint32_t setArrowButton = 0x56E1CD;            // sub_56E1CC(root, const char* name, bool visible, const function<void()>&)
}  // namespace lib

// GOT slots of AdvStageScrollView's icon path formats (const char* globals)
constexpr uint32_t GOT_EVENT_ICON_PASS = 0xFD667C;
constexpr uint32_t GOT_COLOSSEUM_ICON_PASS = 0xFD6680;

constexpr uint32_t API_EVENT_LIST = 300;        // APIManager + 300: event list (vector of 56-byte events)
constexpr uint32_t API_COLOSSEUM = 588;         // APIManager + 588: colosseum data (+0: colosseum stage id,
                                                //   +8: vector<Colosseum::Stage>)
constexpr uint32_t API_CAMPAIGNS = 764;         // APIManager + 764: vector<int> of running campaigns

// virtual slots
constexpr uint32_t VS_SLIDE_31C = 0x31C;        // BaseSlideView virtual taking a bool (called with true)

// lambda vtables (__func of setColosseumInfo's lambdas)
constexpr uint32_t VT_setColosseumInfo_6 = 0xF59514;    // createItem (no captures)
constexpr uint32_t VT_setColosseumInfo_heap = 0xF594E8; // updateItem (76-byte heap lambda)
constexpr uint32_t VT_setColosseumInfo_9 = 0xF594BC;    // LeftUI (slide)
constexpr uint32_t VT_setColosseumInfo_10 = 0xF59490;   // RightUI (slide)
constexpr uint32_t VT_setColosseumInfo_11 = 0xF59464;   // Left_arrow (slide)
constexpr uint32_t VT_setColosseumInfo_12 = 0xF59438;   // Right_arrow (slide)
constexpr uint32_t VT_setColosseumInfo_13 = 0xF5940C;   // Point_Left_arrow (slide)
constexpr uint32_t VT_setColosseumInfo_14 = 0xF593E0;   // Point_Right_arrow (slide)

// strings
constexpr uint32_t S_EVENT_BANNER = 0xDFF691;           // "Event_Banner"
constexpr uint32_t S_TXT_EV_GRADE_LABEL = 0xDFF3E2;     // "Txt_EV_Grade_Label"
constexpr uint32_t S_ATLAS_LABEL_AP = 0xDFD382;         // "AtlasLabel_AP"
constexpr uint32_t S_TXT_PERIOD_LABEL = 0xDFF6C2;       // "Txt_Period_Label"
constexpr uint32_t S_FMT_SS = 0xDF8BBA;                 // "%s%s"
constexpr uint32_t S_TXT_CLASS_LABEL = 0xDFF6D3;        // "Txt_Class_Label"
constexpr uint32_t S_FMT_S_D = 0xDFF6E3;                // "%s %d"
constexpr uint32_t S_TXT_LUX_BEST = 0xDFF65D;           // "Txt_LuxBestRecord_Label"
constexpr uint32_t S_FMT_S_D_D = 0xDFF6E9;              // "%s %d/%d"
constexpr uint32_t S_FMT_S_S = 0xDFDAC7;                // "%s %s"
constexpr uint32_t S_NO_RECORD = 0xDFE932;              // the "no record" placeholder
constexpr uint32_t S_PANEL = 0xDFD420;                  // "Panel"
constexpr uint32_t S_FMT_D = 0xDF86DE;                  // "%d"
constexpr uint32_t S_KEYBLADE_TYPE = 0xDF8A58;          // "KeyBlade_type"
constexpr uint32_t S_NEW = 0xDFD177;                    // "New"
constexpr uint32_t S_TXT_NEW = 0xDFF3D5;                // "Txt_New"
constexpr uint32_t S_TXT_ROUND = 0xDFE918;              // "Txt_Round"
constexpr uint32_t S_TXT_ROUND_NUM = 0xDFF3F5;          // "Txt_Round_Num_Label"
constexpr uint32_t S_CL_STG = 0xDFF598;                 // "stage/cl_stg%05d.bin"
constexpr uint32_t S_CL_STG_PART = 0xDFF5AD;            // "stage/cl_stg%05d_%02d.bin"
constexpr uint32_t S_AREA = 0xDF9378;                   // "Area"
constexpr uint32_t S_COLOSSEUM_SLIDE = 0xDFF5C7;        // "ColosseumSlideView"
constexpr uint32_t S_LEFT_UI = 0xDF85F0;                // "LeftUI"
constexpr uint32_t S_RIGHT_UI = 0xDF85F7;               // "RightUI"
constexpr uint32_t S_LEFT_ARROW = 0xDFF5DA;             // "Left_arrow"
constexpr uint32_t S_RIGHT_ARROW = 0xDFF5E5;            // "Right_arrow"
constexpr uint32_t S_POINT_LEFT_ARROW = 0xDFF5F1;       // "Point_Left_arrow"
constexpr uint32_t S_POINT_RIGHT_ARROW = 0xDFF602;      // "Point_Right_arrow"
constexpr uint32_t S_CLEAR = 0xDFD17B;                  // "Clear"

uint32_t api() { return call(lib::APIManager_getInstance, {}); }
void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }
uint16_t ld16(uint32_t a) { return rw::load<uint16_t>(a); }
// a std::string copy of another one's characters (the original rebuilds it with strlen)
uint32_t strCopy(Frame &f, uint32_t s) { return newStr(f, strData(s)); }

// AtlasLabel_AP of a row: the campaign point bonus of a stage
void setCampaignPointLabel(uint32_t widget, uint32_t point) {
    Frame f;
    uint32_t label = findNode(widget, S_ATLAS_LABEL_AP, 0xD);
    uint32_t str = f.alloc(12);
    call(lib::pointToString, {str, point, 0, 0});
    uint32_t tmp = strCopy(f, str);
    call(lib::TextAtlas_setString, {label, tmp});
    strFree(tmp);
    strFree(str);
}

// Colosseum::Stage (16 bytes): class id, round, stage id, campaign kind
struct ColosseumStageInfo {
    uint32_t classId, round, stageId, kind;
};
}  // namespace

namespace AdvStageScrollView_event {

// ------------------------------------------------------------------------------------------------ event row
// The event stage frame: icon, grade, boss type, sub missions and the "New" badge. The badge goes to the first
// stage of an event whose EvStage type is 3, when the previous stage of that event list is the event's last
// cleared stage (or the event has none).
uint32_t setEventStageFrame(uint32_t widget, int32_t stageId) {
    Frame f;
    uint32_t entryHead = f.alloc(12);   // the matched stage entry's first three words (a stack copy)
    uint32_t missions = f.alloc(12);    // vector<int> of the matched entry
    st32(missions, 0);
    st32(missions + 4, 0);
    st32(missions + 8, 0);
    uint32_t ev = f.alloc(8);           // shared_ptr<EvStage>
    uint32_t list = ld32(api() + API_EVENT_LIST);
    uint32_t newStage = 0;              // the stage that gets the "New" badge
    uint32_t afterCleared = 0;          // the previous stage was the event's last cleared one
    uint32_t lb = ld32(list), le = ld32(list + 4);
    if (int32_t(le - lb) >= 1) {
        uint32_t count = uint32_t(int32_t(le - lb) >> 3) * 0xB6DB6DB7u;   // / 56
        newStage = 0;
        uint32_t i = 0;
        for (;;) {
            if (!(count > i)) {
                call(lib::throwOutOfRange, {list});
                return 0;
            }
            uint32_t entry = lb + 56 * i;
            uint32_t sb = ld32(entry);
            int32_t bytes = int32_t(ld32(entry + 4) - sb);
            if (bytes >= 1) {
                uint32_t count2 = uint32_t(bytes >> 3) * 0xAAAAAAABu;   // / 24
                uint32_t off = 0, j = 0;
                afterCleared = 0;
                for (;;) {
                    if (count2 <= j) {
                        call(lib::throwOutOfRange, {entry});
                        return 0;
                    }
                    uint32_t elem = sb + off;
                    uint32_t elemStage = ld32(elem);
                    if (elemStage == uint32_t(stageId)) {
                        st32(entryHead, ld32(elem));
                        st32(entryHead + 4, ld32(elem + 4));
                        st32(entryHead + 8, ld32(elem + 8));
                        if (elem != entryHead) call(lib::IntVector_assign, {missions, ld32(elem + 12), ld32(elem + 16)});
                        call(lib::EvStage_create, {ev, ld32(entry + 12)});
                        if (newStage == 0) {
                            newStage = 0;
                            uint32_t evRow = ld32(ev);
                            if (evRow && safe32(evRow, 40) == 3) {
                                if ((afterCleared & 1) || ld32(entry + 0x30) == 0) newStage = uint32_t(stageId);
                                else newStage = 0;
                                afterCleared = 0;
                            }
                        }
                        sharedRelease(ev);
                    } else if ((((newStage != 0 ? 1u : 0u) | afterCleared) & 1) == 0) {
                        uint32_t lastCleared = ld32(entry + 0x30);
                        newStage = 0;
                        afterCleared = (lastCleared == elemStage && lastCleared != 0) ? 1 : 0;
                    }
                    sb = ld32(entry);
                    count2 = uint32_t(int32_t(ld32(entry + 4) - sb) >> 3) * 0xAAAAAAABu;
                    off += 24;
                    ++j;
                    if (int32_t(j) >= int32_t(count2)) break;
                }
                lb = ld32(list);
                le = ld32(list + 4);
            }
            count = uint32_t(int32_t(le - lb) >> 3) * 0xB6DB6DB7u;
            ++i;
            if (int32_t(i) >= int32_t(count)) break;
        }
    }

    uint32_t stage = masterRow(f, lib::Stage_create, uint32_t(stageId));
    uint32_t row = ld32(stage);
    // event icon
    uint32_t panel = findNode(widget, S_PANEL, 5);
    uint32_t iconFmt = ld32(ld32(GOT_EVENT_ICON_PASS));
    int32_t iconA = safe32(row, 276);
    int32_t iconB = safe32(row, 280);
    {
        Frame t;
        uint32_t buf = stringBuffer(t, {iconFmt, uint32_t(iconA), uint32_t(iconB)});
        uint32_t s = newStr(t, buf);
        call(lib::ImageView_loadTexture, {panel, s, 0});
        strFree(s);
    }
    // grade (hidden)
    uint32_t grade = findNode(widget, S_TXT_EV_GRADE_LABEL, 0x12);
    SceneUtil::setShadow(grade);
    int32_t gradeValue = safe32(row, 4);
    {
        Frame t;
        uint32_t buf = stringBuffer(t, {S_FMT_D, uint32_t(gradeValue)});
        uint32_t s = newStr(t, buf);
        call(lib::Text_setString, {grade, s});
        strFree(s);
    }
    setVisible(grade, 0);
    // boss type and sub missions
    uint32_t bossType = findNode(widget, S_KEYBLADE_TYPE, 0xD);
    call(lib::setBossTypeImage, {bossType, uint32_t(stageId), 0});
    int32_t missionGroup = safe32(row, 480);
    call(lib::setSubMisstion, {widget, uint32_t(missionGroup), missions});
    // "New" badge
    uint32_t badge = findNode(widget, S_NEW, 3);
    if (newStage == uint32_t(stageId)) {
        setVisible(badge, 1);
        uint32_t text = findNode(badge, S_TXT_NEW, 7);
        SceneUtil::setShadow(text);
        setTextC(text, S_NEW);
    } else {
        setVisible(badge, 0);
    }
    uint32_t text = findNode(badge, S_TXT_NEW, 7);
    SceneUtil::setShadow(text);
    setTextC(text, S_NEW);
    sharedRelease(stage);
    call(lib::IntVector_dtor, {missions});
    return 2316;   // r0 = the frame size the epilogue added to sp
}

// ------------------------------------------------------------------------------------------------ colosseum row
// The colosseum stage frame of round `index`: icon, "Round" label and the round number (or a placeholder when the
// class of this stage has no record).
uint32_t setColosseumStageFrame(uint32_t widget, uint32_t index) {
    Frame f;
    uint32_t stages = f.alloc(12);
    call(lib::StageVector_copy, {stages, ld32(api() + API_COLOSSEUM) + 8});
    uint32_t b = ld32(stages), e = ld32(stages + 4);
    if (!(uint32_t(int32_t(e - b) >> 4) > index)) {
        call(lib::throwOutOfRange, {stages});
        return 0;
    }
    uint32_t info = b + index * 16;
    uint32_t classId = ld32(info);
    uint32_t round = ld32(info + 4);
    uint32_t cs = f.alloc(8);
    call(lib::ColosseumStage_create, {cs, ld32(ld32(api() + API_COLOSSEUM))});
    // the colosseum (class) entry of this stage: its record
    int32_t id = safe32(ld32(cs), 28);
    uint32_t record = 0;
    uint32_t col = f.alloc(8);
    uint32_t found;
    do {
        if (id > safe32(ld32(cs), 32)) break;
        call(lib::Colosseum_create, {col, uint32_t(id)});
        found = 0;
        if (classId == uint32_t(safe32(ld32(col), 8))) {
            record = uint32_t(safe32(ld32(col), 16));
            found = 1;
        }
        sharedRelease(col);
        ++id;
    } while (found != 1);

    uint32_t panel = findNode(widget, S_PANEL, 5);
    {
        Frame t;
        uint32_t s = newStr(t, ld32(ld32(GOT_COLOSSEUM_ICON_PASS)));
        call(lib::ImageView_loadTexture, {panel, s, 0});
        strFree(s);
    }
    uint32_t roundLabel = findNode(widget, S_TXT_ROUND, 9);
    SceneUtil::setShadow(roundLabel);
    {
        Frame t;
        uint32_t td = textData(t, 103300008);
        setTextC(roundLabel, textDataStr(td));
        sharedRelease(td);
    }
    uint32_t roundNum = findNode(widget, S_TXT_ROUND_NUM, 0x13);
    SceneUtil::setShadow(roundNum);
    if (record) {
        Frame t;
        uint32_t buf = stringBuffer(t, {S_FMT_D, round});
        setTextC(roundNum, buf);
    } else {
        Frame t;
        uint32_t td = textData(t, 103300024);
        setTextC(roundNum, textDataStr(td));
        sharedRelease(td);
    }
    sharedRelease(cs);
    call(lib::StageVector_dtor, {stages});
    return 0;   // r0 = canary - canary
}

// ------------------------------------------------------------------------------------------------ titles
// The event title box of a row: banner, stage name, campaign point bonus.
uint32_t setEventTitle(uint32_t widget, int32_t stageId) {
    if (stageId == 0) return 0;
    Frame f;
    uint32_t stage = masterRow(f, lib::Stage_create, uint32_t(stageId));
    uint32_t banner = findNode(widget, S_EVENT_BANNER, 0xC);
    setVisible(banner, 1);
    uint32_t labelName = newStr(f, S_TXT_EV_GRADE_LABEL, 0x12);
    uint32_t nameBytes = safeBytes(f, ld32(stage), 8, 0x81);   // the stage name (128 chars + 0)
    uint32_t name = newStr(f, nameBytes);
    uint32_t search = strCopy(f, labelName);
    uint32_t label = SceneUtil::searchNodeByName(widget, search);
    strFree(search);
    SceneUtil::setShadow(label);
    uint32_t tmp = strCopy(f, name);
    call(lib::Text_setString, {label, tmp});
    strFree(tmp);
    strFree(name);
    strFree(labelName);
    uint32_t campaigns = api() + API_CAMPAIGNS;
    int32_t kind = safe32(ld32(stage), 268);
    int32_t base = safe32(ld32(stage), 284);
    uint32_t point = call(lib::getCampaignPoint, {uint32_t(kind), uint32_t(base), uint32_t(stageId), campaigns, 7, 0});
    setCampaignPointLabel(widget, point);
    sharedRelease(stage);
    return 0;   // r0 = canary - canary
}

// The colosseum title box of round `index`: period, class, best record, campaign point bonus.
uint32_t setColosseumTitle(uint32_t widget, uint32_t index) {
    Frame f;
    uint32_t cs = f.alloc(8);
    call(lib::ColosseumStage_create, {cs, ld32(ld32(api() + API_COLOSSEUM))});
    // period: "<label><start date> - <end date>"
    uint32_t period = findNode(widget, S_TXT_PERIOD_LABEL, 0x10);
    SceneUtil::setShadow(period);
    uint32_t start = safeBytes(f, ld32(cs), 4, 8);
    uint32_t startLocal = f.alloc(8);
    call(lib::Date_makeLocalDate, {startLocal, start});
    uint32_t end = safeBytes(f, ld32(cs), 12, 8);
    uint32_t endLocal = f.alloc(8);
    call(lib::Date_makeLocalDate, {endLocal, end});
    {
        Frame t;
        uint32_t td = textData(t, 103900011);
        uint32_t prefix = textDataStr(td);
        // Date: year (16 bits), month, day
        uint32_t y1 = ld16(startLocal), md1 = ld16(startLocal + 2);
        uint32_t y2 = ld16(endLocal), md2 = ld16(endLocal + 2);
        uint32_t range = t.alloc(12);
        call(lib::TextData_format, {range, 0, 103901038, y1, md1 & 0xFF, md1 >> 8, y2, md2 & 0xFF, md2 >> 8});
        uint32_t buf = stringBuffer(t, {S_FMT_SS, prefix, strData(range)});
        setTextC(period, buf);
        strFree(range);
        sharedRelease(td);
    }
    // this round's stage
    uint32_t stages = f.alloc(12);
    call(lib::StageVector_copy, {stages, ld32(api() + API_COLOSSEUM) + 8});
    uint32_t b = ld32(stages), e = ld32(stages + 4);
    if (!(uint32_t(int32_t(e - b) >> 4) > index)) {
        call(lib::throwOutOfRange, {stages});
        return 4444;
    }
    uint32_t infoAddr = b + index * 16;
    ColosseumStageInfo info{ld32(infoAddr), ld32(infoAddr + 4), ld32(infoAddr + 8), ld32(infoAddr + 12)};
    int32_t id = safe32(ld32(cs), 28);
    uint32_t record = 0;
    uint32_t col = f.alloc(8);
    uint32_t found;
    do {
        if (id > safe32(ld32(cs), 32)) break;
        call(lib::Colosseum_create, {col, uint32_t(id)});
        found = 0;
        if (info.classId == uint32_t(safe32(ld32(col), 8))) {
            record = uint32_t(safe32(ld32(col), 16));
            found = 1;
        }
        sharedRelease(col);
        ++id;
    } while (found != 1);
    // class
    uint32_t classLabel = findNode(widget, S_TXT_CLASS_LABEL, 0xF);
    SceneUtil::setShadow(classLabel);
    {
        Frame t;
        uint32_t td = textData(t, 103300007);
        uint32_t buf = stringBuffer(t, {S_FMT_S_D, textDataStr(td), info.classId});
        setTextC(classLabel, buf);
        sharedRelease(td);
    }
    // best record "<label> round/record" or the placeholder
    uint32_t best = findNode(widget, S_TXT_LUX_BEST, 0x17);
    SceneUtil::setShadow(best);
    if (record) {
        Frame t;
        uint32_t td = textData(t, 103300008);
        uint32_t buf = stringBuffer(t, {S_FMT_S_D_D, textDataStr(td), info.round, record});
        setTextC(best, buf);
        sharedRelease(td);
    } else {
        Frame t;
        uint32_t td = textData(t, 103300008);
        uint32_t buf = stringBuffer(t, {S_FMT_S_S, textDataStr(td), S_NO_RECORD});
        setTextC(best, buf);
        sharedRelease(td);
    }
    // campaign point bonus
    uint32_t stage = masterRow(f, lib::Stage_create, info.stageId);
    uint32_t apiMgr = api();
    int32_t base = safe32(ld32(stage), 284);
    uint32_t point = call(lib::getCampaignPoint,
                          {info.kind, uint32_t(base), info.stageId, apiMgr + API_CAMPAIGNS, 7, 0});
    setCampaignPointLabel(widget, point);
    sharedRelease(stage);
    call(lib::StageVector_dtor, {stages});
    sharedRelease(cs);
    return 4444;   // r0 = the frame size the epilogue added to sp
}

// ------------------------------------------------------------------------------------------------ colosseum info
// The colosseum info box: reads the stage's battle files (cl_stgNNNNN.bin, then part _PP), collects the waves'
// enemies / rounds / kinds, and fills the "ColosseumSlideView" (made on first use) with one page per 6 waves,
// plus the arrow buttons that scroll it.
//
// The original builds its updateItem lambda from a stack block one word of which (+0x74) it never writes: the
// stale word lands in the heap lambda. So the whole original frame is reproduced (locals at their offsets from
// the original sp) and that word read where it lies.
uint32_t setColosseumInfo(uint32_t widget, uint32_t stage) {
    Frame fr;
    const uint32_t S = fr.allocExact(20 + 0xA84);   // pushed registers + sub sp: S = the original sp

    // the stage files
    uint32_t fu = call(lib::FileUtils_getInstance, {});
    uint32_t getData = ld32(ld32(fu) + 0x10);       // FileUtils::getDataFromFile(Data out, this, const string&)
    call(fn::StringBuffer_ctor, {S + 0x644, S_CL_STG, ld32(stage + 8)});
    strInitC(S + 0x13C, S + 0x644);
    call(getData, {S + 0x148, fu, S + 0x13C});
    strFree(S + 0x13C);
    uint32_t part = call(lib::getInt16LE, {call(lib::Data_getBytes, {S + 0x148}) + 10});
    fu = call(lib::FileUtils_getInstance, {});
    getData = ld32(ld32(fu) + 0x10);
    call(fn::StringBuffer_ctor, {S + 0x240, S_CL_STG_PART, ld32(stage + 8), part});
    strInitC(S + 0x128, S + 0x240);
    call(getData, {S + 0x134, fu, S + 0x128});
    strFree(S + 0x128);

    // the wave offset of this round: waves of the classes before this one, plus the rounds before it
    const uint32_t cs = S + 0x120;
    call(lib::ColosseumStage_create, {cs, ld32(ld32(api() + API_COLOSSEUM))});
    int32_t id = safe32(ld32(cs), 28);
    uint32_t acc = 0, waveOffset = 0;
    const uint32_t col = S + 0x118;
    for (;;) {
        if (id > safe32(ld32(cs), 32)) break;
        call(lib::Colosseum_create, {col, uint32_t(id)});
        uint32_t done;
        if (ld32(stage) == uint32_t(safe32(ld32(col), 8))) {
            uint32_t round = ld32(stage + 4);
            int32_t perStep = safe32(ld32(cs), 0x54);
            int32_t step = safe32(ld32(cs), 0x58);
            uint32_t q = call(lib::divsi3, {round - 1, uint32_t(perStep)});
            waveOffset = uint32_t(step) * q + acc;
            done = 1;
        } else {
            acc += uint32_t(safe32(ld32(col), 16));
            done = 0;
        }
        sharedRelease(col);
        ++id;
        if (done == 1) break;
    }

    // waves of the part file
    const uint32_t enemies = S + 0x10C, rounds = S + 0x100, kinds = S + 0xF4;   // vector<int> each
    for (uint32_t k = 0; k < 12; k += 4) {
        st32(enemies + k, 0);
        st32(rounds + k, 0);
        st32(kinds + k, 0);
    }
    uint32_t header = call(lib::getInt32LE, {call(lib::Data_getBytes, {S + 0x134}) + 0x24});
    uint32_t tableOff = header + 0x34;
    uint32_t bytes = call(lib::Data_getBytes, {S + 0x134});
    uint32_t skip = call(lib::getInt32LE, {bytes + tableOff});
    int32_t waves = int32_t(call(lib::getInt32LE, {header + bytes + 0x38}));
    if (waves >= 1) {
        uint32_t p = header + 0x44 * skip + bytes + 0x44;
        for (int32_t w = 0; w < waves; ++w) {
            uint32_t enemy = call(lib::getInt32LE, {p + 4});
            st32(S + 0xF0, enemy);
            if (enemy == 0) {
                enemy = call(lib::getInt32LE, {p});
                st32(S + 0xF0, enemy);
            }
            uint32_t round = call(lib::getInt32LE, {p + 0xC});
            uint32_t kind = call(lib::getInt32LE, {p + 0x14});
            st32(S + 0xEC, kind);
            // enemies.push_back(enemy)
            uint32_t end = ld32(enemies + 4);
            if (end == ld32(enemies + 8)) {
                call(lib::IntVector_pushBackSlowRef, {enemies, S + 0xF0});
            } else {
                uint32_t e = 0;
                if (end) {
                    st32(end, enemy);
                    e = ld32(enemies + 4);
                }
                st32(enemies + 4, e + 4);
            }
            // rounds.push_back(round + waveOffset)
            uint32_t r = round + waveOffset;
            st32(S + 0xE8, r);
            end = ld32(rounds + 4);
            if (end >= ld32(rounds + 8)) {
                call(lib::IntVector_pushBackSlowMove, {rounds, S + 0xE8});
            } else {
                uint32_t e = 0;
                if (end) {
                    st32(end, r);
                    e = ld32(rounds + 4);
                }
                st32(rounds + 4, e + 4);
            }
            // kinds.push_back(kind)
            end = ld32(kinds + 4);
            if (end == ld32(kinds + 8)) {
                call(lib::IntVector_pushBackSlowRef, {kinds, S + 0xEC});
            } else {
                uint32_t e = 0;
                if (end) {
                    st32(end, kind);
                    e = ld32(kinds + 4);
                }
                st32(kinds + 4, e + 4);
            }
            p += 0x20;
        }
    }

    // the slide view
    uint32_t panel = findNode(widget, S_PANEL, 5);
    uint32_t area = findNode(widget, S_AREA, 4);
    uint32_t slide = findNode(area, S_COLOSSEUM_SLIDE, 0x12);
    if (!slide) {
        slide = call(lib::BaseSlideView_create, {});
        uint32_t size = vcall(panel, node::getContentSize, {panel});
        call(lib::BaseSlideView_setScreenWidth, {slide, ld32(size)});
        uint32_t addChild = ld32(ld32(area) + node::addChildZName);
        strInit(S + 0xB8, S_COLOSSEUM_SLIDE, 0x12);
        call(addChild, {area, slide, 0, S + 0xB8});
        strFree(S + 0xB8);
    }
    // createItem callback (slide + 1280)
    const uint32_t createFn = S + 0xA48;
    st32(createFn + 16, createFn);
    st32(createFn, VT_setColosseumInfo_6);
    call(lib::FunctionWidgetInt_swap, {createFn, slide + 1280});
    functionDestroy(createFn);

    // updateItem callback (slide + 1304): captures slide, the stage info, the file values and the three vectors
    const uint32_t cap = S + 0x70;
    st32(cap, slide);
    // cap + 4: never written (stale stack word)
    for (uint32_t k = 0; k < 16; k += 4) st32(cap + 8 + k, ld32(stage + k));
    st32(cap + 0x18, part);
    st32(cap + 0x1C, tableOff);
    st32(cap + 0x20, skip);
    call(lib::IntVector_copy, {cap + 0x24, kinds});
    call(lib::IntVector_copy, {cap + 0x30, enemies});
    call(lib::IntVector_copy, {cap + 0x3C, rounds});
    const uint32_t lam = S + 0x168;   // the lambda, moved
    for (uint32_t k = 0; k < 0x24; k += 4) st32(lam + k, ld32(cap + k));
    for (uint32_t k = 0x24; k < 0x48; k += 4) {
        st32(lam + k, ld32(cap + k));
        st32(cap + k, 0);
    }
    const uint32_t updateFn = S + 0xA60;
    st32(updateFn + 16, 0);
    uint32_t heap = call(fn::operator_new, {0x4C});
    if (heap) {
        st32(heap, VT_setColosseumInfo_heap);
        for (uint32_t k = 0; k < 0x24; k += 4) st32(heap + 4 + k, ld32(lam + k));
        for (uint32_t k = 0x24; k < 0x48; k += 4) {
            st32(heap + 4 + k, ld32(lam + k));
            st32(lam + k, 0);
        }
    }
    st32(updateFn + 16, heap);
    call(lib::FunctionWidgetWidgetIntBool_swap, {updateFn, slide + 1304});
    functionDestroy(updateFn);
    call(lib::IntVector_dtor, {lam + 0x3C});
    call(lib::IntVector_dtor, {lam + 0x30});
    call(lib::IntVector_dtor, {lam + 0x24});
    call(lib::IntVector_dtor, {cap + 0x3C});
    call(lib::IntVector_dtor, {cap + 0x30});
    call(lib::IntVector_dtor, {cap + 0x24});

    // pages: 0 .. waves-skip/6 (one id per page)
    const uint32_t pageVal = S + 0x60, pages = S + 0x64;
    st32(pageVal, 0);
    st32(pages, 0);
    st32(pages + 4, 0);
    st32(pages + 8, 0);
    int32_t lastPage = int32_t(call(lib::divsi3, {skip, 6}));
    if (!(int32_t(skip) < int32_t(~4u))) {
        uint32_t n = 1;
        uint32_t end = 0, capEnd = 0;
        for (;;) {
            uint32_t page = n - 1;
            if (end == capEnd) {
                call(lib::IntVector_pushBackSlowRef, {pages, pageVal});
            } else {
                uint32_t e = 0;
                if (end) {
                    st32(end, page);
                    e = ld32(pages + 4);
                }
                st32(pages + 4, e + 4);
            }
            st32(pageVal, n);
            if (!(int32_t(page) < lastPage)) break;
            ++n;
            end = ld32(pages + 4);
            capEnd = ld32(pages + 8);
        }
    }
    uint32_t setSize = ld32(ld32(slide) + node::setContentSize);
    uint32_t areaSize = vcall(area, node::getContentSize, {area});
    call(setSize, {slide, areaSize});
    call(lib::IntVector_copy, {S + 0x54, pages});
    call(lib::BaseSlideView_setItemIDs, {slide, S + 0x54, 1});
    call(lib::IntVector_dtor, {S + 0x54});
    vcall(slide, VS_SLIDE_31C, {slide, 1});
    call(lib::BaseSlideView_setIndex, {slide, 0});
    call(lib::BaseSlideView_updateLayout, {slide, 1});

    // arrows (shown with more than one page), each with a lambda capturing the slide view
    uint32_t pageCount = uint32_t(int32_t(ld32(pages + 4) - ld32(pages)) >> 2);
    uint32_t multi = pageCount > 1 ? 1 : 0;
    auto arrow = [&](uint32_t root, uint32_t name, uint32_t fo, uint32_t vtable) {
        st32(fo + 16, fo);
        st32(fo, vtable);
        st32(fo + 4, slide);
        call(lib::setArrowButton, {root, name, multi, fo});
        functionDestroy(fo);
    };
    arrow(widget, S_LEFT_UI, S + 0x228, VT_setColosseumInfo_9);
    arrow(widget, S_RIGHT_UI, S + 0x210, VT_setColosseumInfo_10);
    uint32_t parent = vcall(widget, node::getParent, {widget});
    arrow(parent, S_LEFT_ARROW, S + 0x1F8, VT_setColosseumInfo_11);
    parent = vcall(widget, node::getParent, {widget});
    arrow(parent, S_RIGHT_ARROW, S + 0x1E0, VT_setColosseumInfo_12);
    arrow(widget, S_POINT_LEFT_ARROW, S + 0x1C8, VT_setColosseumInfo_13);
    arrow(widget, S_POINT_RIGHT_ARROW, S + 0x1B0, VT_setColosseumInfo_14);

    uint32_t clear = findNode(widget, S_CLEAR, 5);
    if (clear) setVisible(clear, 0);

    call(lib::IntVector_dtor, {pages});
    call(lib::IntVector_dtor, {kinds});
    call(lib::IntVector_dtor, {rounds});
    call(lib::IntVector_dtor, {enemies});
    sharedRelease(cs);
    call(lib::Data_dtor, {S + 0x134});
    call(lib::Data_dtor, {S + 0x148});
    return 2692;   // r0 = the frame size the epilogue added to sp
}

}  // namespace AdvStageScrollView_event

RW_FUNCTION(0x5698AC, "AdvStageScrollView::setEventStageFrame(Widget*, int)", AdvStageScrollView_event::setEventStageFrame);
RW_FUNCTION(0x569EAC, "AdvStageScrollView::setColosseumStageFrame(Widget*, int)",
            AdvStageScrollView_event::setColosseumStageFrame);
RW_FUNCTION(0x56EDBC, "AdvStageScrollView::setEventTitle(Widget*, int)", AdvStageScrollView_event::setEventTitle);
RW_FUNCTION(0x56F428, "AdvStageScrollView::setColosseumTitle(Widget*, int)", AdvStageScrollView_event::setColosseumTitle);
RW_FUNCTION(0x56D65C, "AdvStageScrollView::setColosseumInfo(Widget*, const Colosseum::Stage&)",
            AdvStageScrollView_event::setColosseumInfo);
