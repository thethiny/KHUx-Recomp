// SceneUtil::setItemImageData(std::string &name, std::string &image, int type, int id, bool alt): the display name
// and the image path of a reward item of `type` (incentive items, medal, material, title, keyblade, avatar parts,
// stamp, sphere ...). The original keeps a 0x2730 byte frame with one buffer per case; the frame size is kept so
// the callees run at the same depth, the buffers share a few offsets.
#include "B2sCommon.hpp"

using namespace b2s;

namespace {
constexpr Addr STRING_ASSIGN = 0x48D811;                 // std::string::assign(const char*, size_t)
constexpr Addr StringBuffer_ctor = 0x49140D;
constexpr Addr TextData_get = 0x4928AD;                  // TextData::get(shared_ptr out, Type, id)
constexpr Addr Medal_create = 0x785871;
constexpr Addr Material_create = 0x7852E5;
constexpr Addr Title_create = 0x78FA8D;
constexpr Addr Keyblade_create = 0x783F9D;
constexpr Addr AvatarParts_create = 0x778DE1;
constexpr Addr Stamp_create = 0x7919E5;
constexpr Addr Sphere_create = 0x78CF21;

constexpr Addr STR_Empty = 0xDF8A1A;
constexpr Addr STR_Default = 0xDF9342;                   // 12 bytes (Japanese placeholder text)
constexpr Addr ICON_02 = 0xDF90A2, ICON_04 = 0xDF90DE, ICON_06 = 0xDF911E, ICON_07 = 0xDF9141, ICON_08 = 0xDF9164,
               ICON_09 = 0xDF9187, ICON_10 = 0xDF91AA, ICON_11 = 0xDF91CD, ICON_12 = 0xDF91F0, ICON_14 = 0xDF9232,
               ICON_15 = 0xDF9255, ICON_16 = 0xDF9278, ICON_17 = 0xDF929B, ICON_26 = 0xDF931F;   // "img/incentive/IncentiveIcon_NN.png" (34)
constexpr Addr FMT_MEDAL = 0xDF90C5;                     // "img/medal/Medal_S_%d.png"
constexpr Addr FMT_MATERIAL = 0xDF9101;                  // "img/material/material_%d.png"
constexpr Addr FMT_KEYBLADE = 0xDF9213;                  // "img/keyBlade/KeyBlade_%06d.png"
constexpr Addr FMT_AVATAR3 = 0xDF92BE;                   // "img/avatarParts/ai%03d.png"
constexpr Addr FMT_AVATAR = 0xDF92D9;                    // "img/avatarParts/ai%d.png"
constexpr Addr FMT_STAMP = 0xDF92F2;                     // "img/stamp/talkstamp_%d.png"
constexpr Addr FMT_SPHERE = 0xDF930D;                    // "img/sphere/%d.png"

constexpr uint32_t F_SIZE = 0x2730;
constexpr uint32_t F_TEXT = 0x24;          // shared_ptr<TextData entry> (obj, ctrl)
constexpr uint32_t F_ROW = 0x34;           // shared_ptr<master row> (obj, ctrl)
constexpr uint32_t F_VALUE = 0xC8;         // an int read from the row
constexpr uint32_t F_FMT = 0x1574;         // StringBuffer (1028 bytes)
constexpr uint32_t F_NAME = 0x1978;        // the row's name field (0x41 / 0x81 bytes)
}  // namespace

void SceneUtil_setItemImageData(uint32_t name, uint32_t image, int32_t type, int32_t id, uint32_t alt) {
    guest::Frame F(F_SIZE);
    auto fa = [&](uint32_t off) { return F.addr(off); };
    auto assign = [&](Addr str, Addr text, uint32_t len) { cx(STRING_ASSIGN, {str, text, len}); };
    // read(row, out, data + offset, n, offset)
    auto readRow = [&](Addr out, uint32_t offset, uint32_t n) {
        const Addr row = ld32(fa(F_ROW));
        cx(SAFEBUFFER_READ, {row, out, ld32(row + 0xC) + offset, n, offset});
    };
    auto releaseRow = [&]() { sharedRelease(ld32(fa(F_ROW + 4))); };
    // name = the row's text field at `offset` (n bytes)
    auto nameFromRow = [&](uint32_t offset, uint32_t n) {
        readRow(fa(F_NAME), offset, n);
        assign(name, fa(F_NAME), cx(STRLEN, {fa(F_NAME)}));
    };
    // image = printf(format, value)
    auto imageFormat = [&](Addr format, uint32_t value) {
        cx(StringBuffer_ctor, {fa(F_FMT), format, value});
        assign(image, fa(F_FMT), cx(STRLEN, {fa(F_FMT)}));
    };
    // name = the text table entry `textId`; image = the incentive icon
    auto incentive = [&](uint32_t textId, Addr icon) {
        cx(TextData_get, {fa(F_TEXT), 0, textId});
        const Addr text = ld32(ld32(fa(F_TEXT)) + 4);
        assign(name, text, cx(STRLEN, {text}));
        sharedRelease(ld32(fa(F_TEXT + 4)));
        assign(image, icon, 34);
    };

    switch (type) {
    case 1:
        assign(name, STR_Empty, 0);
        assign(image, ICON_02, 34);
        return;
    case 2: incentive(100310001, ICON_02); return;
    case 3:
        cx(Medal_create, {fa(F_ROW), uint32_t(id)});
        nameFromRow(0x10, 0x41);
        readRow(fa(F_VALUE), 8, 4);
        imageFormat(FMT_MEDAL, ld32(fa(F_VALUE)));
        releaseRow();
        return;
    case 4: incentive(100300001, ICON_04); return;
    case 5:
        cx(Material_create, {fa(F_ROW), uint32_t(id)});
        nameFromRow(0xC, 0x41);
        readRow(fa(F_VALUE), 0, 4);
        imageFormat(FMT_MATERIAL, ld32(fa(F_VALUE)));
        releaseRow();
        return;
    case 6: incentive(100320001, ICON_06); return;
    case 7:
        cx(Title_create, {fa(F_ROW), uint32_t(id)});
        nameFromRow(8, 0x41);
        assign(image, ICON_07, 34);
        releaseRow();
        return;
    case 8: incentive(103100036, ICON_08); return;
    case 9: incentive(103100035, ICON_09); return;
    case 10: incentive(100200014, ICON_10); return;
    case 11: incentive(100200001, ICON_11); return;
    case 12: incentive(103400025, ICON_12); return;
    case 13:
        cx(Keyblade_create, {fa(F_ROW), uint32_t(id)});
        nameFromRow(4, 0x41);
        readRow(fa(F_VALUE), 0xCC, 4);
        imageFormat(FMT_KEYBLADE, ld32(fa(F_VALUE)));
        releaseRow();
        return;
    case 14: incentive(alt != 0 ? 103400101 : 103400001, ICON_14); return;
    case 15: incentive(alt != 0 ? 103400103 : 103400003, ICON_15); return;
    case 16: incentive(alt != 0 ? 103400127 : 103400027, ICON_16); return;
    case 17: incentive(alt != 0 ? 103400102 : 103400002, ICON_17); return;
    case 18:
    case 19:
    case 20:
    case 23:
        cx(AvatarParts_create, {fa(F_ROW), uint32_t(id)});
        nameFromRow(4, 0x81);
        readRow(fa(F_VALUE), 0x88, 4);
        if (ld32(fa(F_VALUE)) == 2) imageFormat(FMT_AVATAR3, uint32_t(id));
        else imageFormat(FMT_AVATAR, uint32_t(id));
        releaseRow();
        return;
    case 24: {
        cx(Stamp_create, {fa(F_ROW), uint32_t(id)});
        cx(TextData_get, {fa(F_TEXT), 0, 104200002});
        const Addr text = ld32(ld32(fa(F_TEXT)) + 4);
        assign(name, text, cx(STRLEN, {text}));
        sharedRelease(ld32(fa(F_TEXT + 4)));
        readRow(fa(0xCC), 4, 4);
        bool talk = ld32(fa(0xCC)) == 1;
        if (!talk) {
            readRow(fa(0xD0), 4, 4);
            talk = ld32(fa(0xD0)) == 2;
        }
        if (talk) {
            imageFormat(FMT_STAMP, uint32_t(id));
        } else {
            readRow(fa(0xD4), 4, 4);
            if (ld32(fa(0xD4)) == 3) imageFormat(FMT_MEDAL, uint32_t(id));
            else imageFormat(FMT_STAMP, 1);
        }
        releaseRow();
        return;
    }
    case 25:
        cx(Sphere_create, {fa(F_ROW), uint32_t(id)});
        nameFromRow(4, 0x81);
        readRow(fa(F_VALUE), 0, 4);
        imageFormat(FMT_SPHERE, ld32(fa(F_VALUE)));
        releaseRow();
        return;
    case 26: incentive(102600058, ICON_26); return;
    default:
        assign(name, STR_Default, 12);
        assign(image, ICON_02, 34);
        return;
    }
}

RW_FUNCTION(0x46C71C, "SceneUtil::setItemImageData(string&, string&, int, int, bool)", SceneUtil_setItemImageData);
