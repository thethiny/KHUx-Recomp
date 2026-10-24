// SceneLoginBonus::initLoginBonus (IDA split it into chunks; one function here): the stamp board's texts - titles,
// today's continuous bonus ("Got ..."), the total day counter, the total bonus and the next total bonus - then one
// cell per continuous day (its incentive count and icon, and the stamp LWF, stopped on its last frame, on the days
// already passed), then the total panel (its day text, count and icon) or, without a total bonus, no total panel.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t SceneUtil_setShadow = 0x45BD15;
constexpr uint32_t StringAssign = 0x48D811;                // std::string::assign(const char*, size_t)
constexpr uint32_t throw_out_of_range = 0x47CCE1;
constexpr uint32_t TextAtlas_setString = rwlib::cocos2d::ui::TextAtlas::setString;
constexpr uint32_t ImageView_loadTextureTex = rwlib::cocos2d::ui::ImageView::loadTexture__PNS_9Texture2DE;    // ImageView::loadTexture(Texture2D*)
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;         // (const char* path, void*)
constexpr uint32_t LWFMovie_GotoAndStop = rwlib::LWF::Movie::GotoAndStop;
constexpr uint32_t MovieHandlerMap_insertUnique = 0x530869, MovieHandlerMap_destroy = 0x530811;
constexpr uint32_t LWF_AddMovieEventHandler = rwlib::LWF::LWF::AddMovieEventHandler;
constexpr uint32_t memcpy = 0x40E164;
constexpr uint32_t getIncentiveName = 0x6F90E5, getIncentiveNextName = 0x6F9689, getIncentiveId = 0x6F9B51;
constexpr uint32_t loadAvatarSetTexture = 0x6F9CD9, loadAvatarTexture = 0x6F9DA5, loadMedalTexture = 0x6F9E71;
constexpr uint32_t loadKeyBladeTexture = 0x6F9F3D, loadMaterialTexture = 0x6FA009, loadIncentiveTexture = 0x6FA0D5;
}  // namespace g

constexpr uint32_t F_ROOT = 0x28C, F_CONTINUOUS = 0x290, F_TOTAL = 0x29C, F_NEXT = 0x2A8;   // vector<shared_ptr>
constexpr uint32_t F_TODAY = 0x2C8, F_CELLS = 0x2CC, F_TOTAL_DAYS = 0x2D0;
// master::LoginBonus row: incentive type, id, count, day
constexpr uint32_t ROW_TYPE = 0x920, ROW_ID = 0x924, ROW_COUNT = 0x934, ROW_DAY = 0x90C;
constexpr uint32_t LWFNODE_LWF = 0x3E0;
constexpr uint32_t V_setScaleXY = 0x48;
constexpr uint32_t VT_StampEnterFrame = 0xF69F38;          // {movie shared_ptr}: stops the stamp on its last frame
constexpr uint32_t S_CenterUI = 0xE02FBA, S_Title1 = 0xE0636F, S_Title2 = 0xE06376, S_ContGet = 0xE0637D;
constexpr uint32_t S_TotalDay = 0xE06390, S_AtlasLabel = 0xE063A4, S_TotalDay2 = 0xE063AF;
constexpr uint32_t S_TotalGet = 0xE063C4, S_TotalNext = 0xE063D8, S_FmtCont = 0xE063ED, S_FmtContNum = 0xE063F4;
constexpr uint32_t S_PanelCont = 0xE06408, S_FmtPanelCont = 0xE06413, S_StampLwf = 0xE06420, S_Stamp = 0xE06455;
constexpr uint32_t S_TotalLabel = 0xE05596, S_TotalNum = 0xE0645B, S_Total = 0xE0646E, S_PanelTotal = 0xE06474;
constexpr uint32_t S_AreaPoint = 0xDF9FB4, S_Fmt04d = 0xDFBFAE, S_FmtSD = 0xDFD5F0, S_EnterFrame = 0xDFE0F3;

inline uint32_t find(uint32_t root, uint32_t name, uint32_t len) { return findNode(root, name, len); }
inline uint32_t findC(uint32_t root, uint32_t name) { return findNode(root, name, host_strlen(name)); }
inline void shadow(uint32_t text) { call(g::SceneUtil_setShadow, {text}); }
inline void hide(uint32_t n) { vcall(n, node::setVisible, {n, 0}); }
inline void addRef(uint32_t ctl) {
    if (ctl) call(fn::shared_add, {ctl});
}
inline void release(uint32_t ctl) {
    if (ctl) call(fn::shared_release, {ctl});
}
int32_t rowInt(Frame &f, uint32_t sb, uint32_t off) {
    const uint32_t v = f.alloc(4);
    rw::call(g::SafeBuffer_read, {sb, v, ld32(sb + 0xC) + off, 4, off});
    return int32_t(ld32(v));
}
// a string (libc++ std::string object) into a Text
void setTextString(uint32_t text, uint32_t str) {
    const uint32_t c = (ld8(str) & 1) ? ld32(str + 8) : str + 1;
    setTextC(text, c);
}
// element `i` of a vector<shared_ptr<...>> (copied: object, control block with a reference)
void element(uint32_t vec, uint32_t i, uint32_t &obj, uint32_t &ctl) {
    if (((ld32(vec + 4) - ld32(vec)) >> 3) <= i) call(g::throw_out_of_range, {vec});
    const uint32_t e = ld32(vec) + 8 * i;
    obj = ld32(e);
    ctl = ld32(e + 4);
    addRef(ctl);
}
// the incentive icon of `kind` (its loader and scale)
void incentiveIcon(uint32_t self, uint32_t image, int32_t kind, uint32_t id) {
    uint32_t loader;
    float scale;
    switch (kind) {
    case 2: case 4: case 6: case 7: case 11: case 15: case 16: case 17: case 26:
        loader = g::loadIncentiveTexture; scale = 1.0f; break;
    case 3: loader = g::loadMedalTexture; scale = 1.79f; break;
    case 5: loader = g::loadMaterialTexture; scale = 1.0f; break;
    case 13: loader = g::loadKeyBladeTexture; scale = 1.36f; break;
    case 18: loader = g::loadAvatarSetTexture; scale = 1.12f; break;
    case 19: case 20: loader = g::loadAvatarTexture; scale = 1.12f; break;
    default: return;
    }
    const uint32_t tex = call(loader, {self, id});
    call(g::ImageView_loadTextureTex, {image, tex});
    vcall(image, V_setScaleXY, {image, fbits(scale), fbits(scale)});
}
}  // namespace

struct LoginBonusInit {
    static void init(uint32_t self);
};

void LoginBonusInit::init(uint32_t self) {
    Frame f;
    const uint32_t cu = find(ld32(self + F_ROOT), S_CenterUI, 8);
    const int32_t today = int32_t(ld32(self + F_TODAY));
    uint32_t cont, contCtl;
    element(self + F_CONTINUOUS, uint32_t(today > 0 ? today - 1 : today), cont, contCtl);
    const uint32_t contId = f.alloc(4);
    st32(contId, 0);
    {
        const uint32_t t = find(cu, S_Title1, 6);
        shadow(t);
        setTextId(t, 104500001);
    }
    {
        const uint32_t t = find(cu, S_Title2, 6);
        shadow(t);
        setTextId(t, 104500002);
    }
    {
        const uint32_t t = find(cu, S_ContGet, 0x12);
        if (cont && int32_t(ld32(self + F_TODAY)) >= 1) {
            shadow(t);
            const int32_t type = rowInt(f, cont, ROW_TYPE), id = rowInt(f, cont, ROW_ID);
            const int32_t count = rowInt(f, cont, ROW_COUNT);
            Frame g2;
            const uint32_t s = g2.alloc(12);
            call(g::getIncentiveName, {s, contId, uint32_t(type), uint32_t(id), uint32_t(count), contId});
            setTextString(t, s);
            strFree(s);
        } else {
            hide(t);
        }
    }
    {
        const uint32_t t = find(cu, S_TotalDay, 0x13);
        shadow(t);
        setTextId(t, 104500005);
    }
    {
        const uint32_t atlas = find(cu, S_AtlasLabel, 0xA);
        Frame g2;
        const uint32_t buf = stringBuffer(g2, {S_Fmt04d, ld32(self + F_TOTAL_DAYS)});
        const uint32_t s = newStr(g2, buf, host_strlen(buf));
        call(g::TextAtlas_setString, {atlas, s});
        strFree(s);
    }
    {
        const uint32_t t = find(cu, S_TotalDay2, 0x14);
        shadow(t);
        setTextId(t, 104500008);
    }
    uint32_t total, totalCtl;
    {
        const uint32_t vec = self + F_TOTAL;
        if (ld32(vec) == ld32(vec + 4)) call(g::throw_out_of_range, {vec});
        total = ld32(ld32(vec));
        totalCtl = ld32(ld32(vec) + 4);
        addRef(totalCtl);
    }
    const uint32_t totalId = f.alloc(4);
    st32(totalId, 0);
    {
        const uint32_t t = find(cu, S_TotalGet, 0x13);
        if (total) {
            shadow(t);
            const int32_t type = rowInt(f, total, ROW_TYPE), id = rowInt(f, total, ROW_ID);
            const int32_t count = rowInt(f, total, ROW_COUNT);
            Frame g2;
            const uint32_t s = g2.alloc(12);
            call(g::getIncentiveName, {s, totalId, uint32_t(type), uint32_t(id), uint32_t(count), totalId});
            setTextString(t, s);
            strFree(s);
        } else {
            hide(t);
        }
    }
    uint32_t next, nextCtl;
    {
        const uint32_t vec = self + F_NEXT;
        if (ld32(vec) == ld32(vec + 4)) call(g::throw_out_of_range, {vec});
        next = ld32(ld32(vec));
        nextCtl = ld32(ld32(vec) + 4);
        addRef(nextCtl);
    }
    {
        const uint32_t t = find(cu, S_TotalNext, 0x14);
        if (next) {
            shadow(t);
            const int32_t type = rowInt(f, next, ROW_TYPE), id = rowInt(f, next, ROW_ID);
            const int32_t count = rowInt(f, next, ROW_COUNT), day = rowInt(f, next, ROW_DAY);
            Frame g2;
            const uint32_t s = g2.alloc(12);
            call(g::getIncentiveNextName, {s, 0, uint32_t(type), uint32_t(id), uint32_t(count), uint32_t(day)});
            setTextString(t, s);
            strFree(s);
        } else {
            hide(t);
        }
    }

    // ---------------------------------------------------------------- the continuous days
    for (int32_t i = 0; i < int32_t(ld32(self + F_CELLS)); ++i) {
        const int32_t day = i + 1;
        Frame g2;
        const uint32_t icon = findC(cu, stringBuffer(g2, {S_FmtCont, uint32_t(day)}));
        const uint32_t numText = findC(cu, stringBuffer(g2, {S_FmtContNum, uint32_t(day)}));
        uint32_t row, rowCtl;
        element(self + F_CONTINUOUS, uint32_t(i), row, rowCtl);
        shadow(numText);
        {
            Frame g3;
            const uint32_t td = textData(g3, 100400009);
            const int32_t count = rowInt(g3, row, ROW_COUNT);
            setTextC(numText, stringBuffer(g3, {S_FmtSD, textDataStr(td), uint32_t(count)}));
            sharedRelease(td);
        }
        const uint32_t id = g2.alloc(4);
        st32(id, 0);
        {
            const int32_t type = rowInt(g2, row, ROW_TYPE), rid = rowInt(g2, row, ROW_ID);
            call(g::getIncentiveId, {0, uint32_t(type), uint32_t(rid), id});
        }
        if (ld32(id)) incentiveIcon(self, icon, rowInt(g2, row, ROW_TYPE), ld32(id));
        if (day < int32_t(ld32(self + F_TODAY))) {
            const uint32_t name = newStr(g2, S_PanelCont, 0xA);
            if (i >= 1) {
                const uint32_t buf = stringBuffer(g2, {S_FmtPanelCont, uint32_t(day)});
                call(g::StringAssign, {name, buf, host_strlen(buf)});
            }
            const uint32_t nameC = (ld8(name) & 1) ? ld32(name + 8) : name + 1;
            const uint32_t panel = findC(cu, nameC);
            const uint32_t area = find(panel, S_AreaPoint, 0xA);
            const uint32_t lwfNode = call(g::AudioLWFNode_create, {S_StampLwf, 0});
            vcall(area, node::addChild, {area, lwfNode});
            if (day < int32_t(ld32(self + F_TODAY))) {
                const uint32_t lwf = ld32(lwfNode + LWFNODE_LWF);
                const uint32_t movie = ld32(lwf + 0x18);
                call(g::LWFMovie_GotoAndStop, {movie, ld32(movie + 0x98) - 1});
                const uint32_t lwfPtr = ld32(lwfNode + LWFNODE_LWF);
                const uint32_t stamp = newStr(g2, S_Stamp, 5);
                const uint32_t root = ld32(lwf + 0x18), rootCtl = ld32(lwf + 0x1C);
                addRef(rootCtl);
                // pair<const string, function<void(Movie*)>> {"enterFrame", {root movie}}
                const uint32_t pair = g2.alloc(0x28);
                st8(pair, 20);
                call(g::memcpy, {pair + 1, S_EnterFrame, 10});
                st8(pair + 11, 0);
                st32(pair + 0x20, pair + 0x10);
                st32(pair + 0x10, VT_StampEnterFrame);
                st32(pair + 0x14, root);
                st32(pair + 0x18, rootCtl);
                const uint32_t map = g2.alloc(12);
                st32(map, map + 4);
                st32(map + 4, 0);
                st32(map + 8, 0);
                call(g::MovieHandlerMap_insertUnique, {map, map + 4, pair});
                call(g::LWF_AddMovieEventHandler, {lwfPtr, stamp, map});
                call(g::MovieHandlerMap_destroy, {map, ld32(map + 4)});
                functionDestroy(pair + 0x10);
                strFree(pair);
                strFree(stamp);
            }
            strFree(name);
        }
        release(rowCtl);
    }

    // ---------------------------------------------------------------- the total panel
    if (total && rowInt(f, total, 0) != 0) {
        {
            const uint32_t t = find(cu, S_TotalLabel, 0xF);
            shadow(t);
            Frame g2;
            const uint32_t td = textData(g2, 104500007);
            const uint32_t fmt = textDataStr(td);
            const int32_t day = rowInt(g2, total, ROW_DAY);
            setTextC(t, stringBuffer(g2, {fmt, uint32_t(day)}));
            sharedRelease(td);
        }
        {
            const uint32_t t = find(cu, S_TotalNum, 0x12);
            shadow(t);
            Frame g2;
            const uint32_t td = textData(g2, 100400009);
            const uint32_t text = textDataStr(td);
            const int32_t count = rowInt(g2, total, ROW_COUNT);
            setTextC(t, stringBuffer(g2, {S_FmtSD, text, uint32_t(count)}));
            sharedRelease(td);
        }
        const uint32_t image = find(cu, S_Total, 5);
        if (ld32(totalId)) incentiveIcon(self, image, rowInt(f, total, ROW_TYPE), ld32(totalId));
    } else {
        hide(find(cu, S_PanelTotal, 0xB));
    }
    release(nextCtl);
    release(totalCtl);
    release(contCtl);
}

RW_FUNCTION(0x6F710C, "SceneLoginBonus::initLoginBonus", LoginBonusInit::init);
