// AvatarPartsScrollView::changeRow: fill one cell with a part (icon, name, category / series text, gender
// marks, "new" badge, selection marks).
#include "AvatarPartsScrollView.hpp"

#include "../scene_social/SocialCommon.hpp"

using namespace social;
using Self = AvatarPartsScrollView;

namespace {
constexpr uint32_t throwOutOfRange = 0x47CCE1;             // std::vector::at out of range (throws)
constexpr uint32_t AvatarParts_create = 0x778DE1;          // master::AvatarParts::create(shared_ptr out, id)
constexpr uint32_t AppState_getInstance = 0x4E66DD;
constexpr uint32_t AppState_getNewAvatarPartsIDs = 0x4EBB45;      // const std::vector<int>&
constexpr uint32_t AppState_getNewAvatarPartsSetIDs = 0x4EBCBF;
constexpr uint32_t ImageView_loadTexture = rwlib::cocos2d::ui::ImageView::loadTexture__RKNSt3__112basic_stringIcNS2_11char_traitsIcEENS2_9allocatorIcEEEENS0_6Widget14TextureResTypeE;       // (image, const string&, TextureResType)
constexpr uint32_t SceneUtil_setNormalColor = 0x468669;
constexpr uint32_t Color3B_ctor = rwlib::cocos2d::Color3B::ctor_base;                // (out, r, g, b)
constexpr uint32_t SLOT_setColor = 0x22C;

constexpr uint32_t S_Area_Point = 0xDF9FB4;
constexpr uint32_t S_Icon_Base = 0xDFEB70;
constexpr uint32_t S_Icon_Base_L = 0xDFEB7A;
constexpr uint32_t S_Gender_M = 0xDFEB86;
constexpr uint32_t S_Gender_L = 0xDFEB8F;
constexpr uint32_t S_Icon_New = 0xDFDD8A;
constexpr uint32_t S_Txt_Equip_New = 0xDFEB98;
constexpr uint32_t S_Panel_On = 0xDFD167;
constexpr uint32_t S_Empty = 0xDF8A1A;                     // ""
constexpr uint32_t S_set = 0xDFEBA6;                       // "set/"
constexpr uint32_t S_Icon_Avatar = 0xDFEBAB;
constexpr uint32_t S_IconPath03 = 0xDFEBB7;                // "img/avatarParts/%sai%03d.png"
constexpr uint32_t S_IconPath = 0xDFEBD4;                  // "img/avatarParts/%sai%d.png"
constexpr uint32_t S_Txt_Name = 0xDFDA10;
constexpr uint32_t S_Txt_Category = 0xDFEBEF;
constexpr uint32_t S_Dash = 0xDFC8FD;                      // "-"
constexpr uint32_t S_TwoStrings = 0xDFDAC7;                // "%s %s"

// master::AvatarParts row (SafeBuffer) offsets
constexpr uint32_t AP_NAME = 0x04;        // char[0x81]
constexpr uint32_t AP_TYPE = 0x88;        // tab type (2 = accessory (3-digit icon), 7 = set)
constexpr uint32_t AP_SERIES = 0x98;      // 1..10: series text
constexpr uint32_t AP_IS_SET = 0x9C;      // 1 = a set of parts
constexpr uint32_t AP_SET_COUNT = 0xA4;   // members in the set / 1 = member of a set
constexpr uint32_t AP_SET_IDS = 0xA8;     // int[5] member ids / owning set id

void setVis(Addr node, uint32_t v) { vcall(node, node::setVisible, {node, v}); }

// text->setString(TextData text `id`)
void setTextFromId(Addr text, int32_t id) {
    Frame f;
    Addr sp = textData(f, id);
    Addr s = newStr(f, textDataStr(sp));
    call(sfn::Text_setString, {text, s});
    strFree(s);
    sharedRelease(sp);
}
// str.assign(TextData text `id`)
void assignFromId(Addr str, int32_t id) {
    Frame f;
    Addr sp = textData(f, id);
    Addr t = textDataStr(sp);
    call(fn::string_assign, {str, t, host_strlen(t)});
    sharedRelease(sp);
}
void setDash(Addr text) {
    withStr(S_Dash, 1, [&](Addr s) { call(sfn::Text_setString, {text, s}); });
}

}  // namespace

void Self::changeRow(Self *self, Addr cell, uint32_t index, int32_t column) {
    (void)column;
    uint32_t count = (self->itemIDs.end - self->itemIDs.begin) >> 2;
    if (count <= index) {
        call(throwOutOfRange, {});
        return;
    }
    uint32_t idAddr = self->itemIDs.begin + 4 * index;
    if (ld32(idAddr) == 0) return;
    Addr point = search(cell, S_Area_Point, 10);
    if (!point) return;
    Addr iconBase = search(cell, S_Icon_Base, 9);
    Addr iconBaseL = search(cell, S_Icon_Base_L, 11);
    setVis(iconBase, self->isMale);
    setVis(iconBaseL, self->isMale ? 0 : 1);
    Addr genderM = search(iconBase, S_Gender_M, 8);
    Addr genderL = search(iconBaseL, S_Gender_L, 8);
    setVis(genderM, self->showGender);
    setVis(genderL, self->showGender);

    Frame f;
    Addr row = f.alloc(8);
    call(AvatarParts_create, {row, ld32(idAddr)});
    const SafeBuffer *part = rw::host_ptr<SafeBuffer>(ld32(row));

    // "new" badge: the part (or set) is in AppState's new ids
    Addr ids = self->isSet ? call(AppState_getNewAvatarPartsSetIDs, {call(AppState_getInstance, {})})
                           : call(AppState_getNewAvatarPartsIDs, {call(AppState_getInstance, {})});
    uint32_t p = ld32(ids), end = ld32(ids + 4);
    for (; p != end; p += 4)
        if (ld32(p) == ld32(idAddr)) break;
    uint32_t isNew = p != end ? 1 : 0;
    Addr iconNew = search(cell, S_Icon_New, 8);
    setVis(iconNew, isNew);
    withStr(S_Txt_Equip_New, 13, [&](Addr s) { call(sfn::SceneUtil_settingTitleLabel, {cell, s, 102500008}); });
    Addr equipNew = search(cell, S_Txt_Equip_New, 13);
    Addr panelOn = search(cell, S_Panel_On, 8);
    setVis(panelOn, 0);

    // selection marks
    if (!self->showSelection) {
        setVis(point, 0);
        setVis(equipNew, 0);
    } else if (!self->selectedIDsEnabled) {
        setVis(point, ld32(rw::guest_addr(self) + 0x67C) == ld32(idAddr) ? 1 : 0);
        setVis(equipNew, ld32(rw::guest_addr(self) + 0x67C) == ld32(idAddr) ? 1 : 0);
    } else {
        uint32_t selected = 0;
        for (uint32_t q = self->selectedIDs.begin; q != self->selectedIDs.end; q += 4) {
            if (ld32(q) == ld32(idAddr)) {
                Addr badge = search(cell, S_Icon_New, 8);
                setVis(badge, 0);
                selected = 1;
                break;
            }
        }
        setVis(point, selected);
        setVis(equipNew, selected);
    }

    // icon: img/avatarParts/[set/]ai<id>.png (accessories: three digits)
    Addr prefix = newStr(f, S_Empty, 0);
    if (self->isSet) call(fn::string_assign, {prefix, S_set, 4});
    Addr icon = search(cell, S_Icon_Avatar, 11);
    {
        Frame g;
        int32_t type = part->get32(AP_TYPE);
        Addr path = stringBuffer(g, {type == 2 ? S_IconPath03 : S_IconPath, strData(prefix), ld32(idAddr)});
        Addr s = newStr(g, path);
        call(ImageView_loadTexture, {icon, s, 0});
        strFree(s);
    }

    // name
    Addr name = search(cell, S_Txt_Name, 8);
    call(sfn::SceneUtil_setShadow, {name, 1});
    {
        Frame g;
        Addr text = safeBytes(g, ld32(row), AP_NAME, 0x81);
        Addr s = newStr(g, text);
        call(sfn::Text_setString, {name, s});
        strFree(s);
    }

    // category / series
    Addr category = search(cell, S_Txt_Category, 12);
    call(sfn::SceneUtil_setShadow, {category, 1});
    int32_t type = part->get32(AP_TYPE);
    if (type != 2 && type != 7) {
        call(SceneUtil_setNormalColor, {category});
        switch (part->get32(AP_TYPE)) {
        case 2: setTextFromId(category, 102500002); break;
        case 3: setTextFromId(category, 102500004); break;
        case 4: setTextFromId(category, 102500006); break;
        case 5: setTextFromId(category, 102500007); break;
        case 6: setTextFromId(category, 102500005); break;
        default: setDash(category); break;   // 1, 7 and anything else
        }
    } else {
        // accessories and sets: the series name, plus the number of set pieces
        int32_t pieces = type != 2 ? 1 : 0;
        if (part->get32(AP_IS_SET) == 1) {
            int32_t members = part->get32(AP_SET_COUNT);
            for (int32_t k = 0; k < members; k++) {
                int32_t member = part->get32(AP_SET_IDS + 4 * uint32_t(k));
                Frame g;
                Addr mrow = g.alloc(8);
                call(AvatarParts_create, {mrow, uint32_t(member)});
                const SafeBuffer *m = rw::host_ptr<SafeBuffer>(ld32(mrow));
                if (m->get32(AP_IS_SET) != 1 && m->get32(AP_SET_COUNT) == 1) {
                    if (ld32(idAddr) == uint32_t(m->get32(AP_SET_IDS))) pieces++;
                }
                sharedRelease(mrow);
            }
        }
        {
            uint32_t setColor = ld32(ld32(category) + SLOT_setColor);
            Frame g;
            Addr color = g.alloc(4);
            call(Color3B_ctor, {color, 0xFF, 0xA5, 0xE9});
            call(setColor, {category, color});
        }
        Addr text = f.alloc(12);
        st32(text, 0);
        st32(text + 4, 0);
        st32(text + 8, 0);
        switch (part->get32(AP_SERIES)) {
        case 1: assignFromId(text, 102510001); break;
        case 2: assignFromId(text, 102510003); break;
        case 3: assignFromId(text, 102510004); break;
        case 4: assignFromId(text, 102510002); break;
        case 5: assignFromId(text, 102510005); break;
        case 6: assignFromId(text, 102510006); break;
        case 7: assignFromId(text, 102510007); break;
        case 8: assignFromId(text, 102510008); break;
        case 9: assignFromId(text, 102510009); break;
        case 10: assignFromId(text, 102510010); break;
        default: break;
        }
        if (type == 2) assignFromId(text, 102500002);
        if (pieces < 1) {
            call(sfn::Text_setString, {category, text});
        } else {
            Frame g;
            Addr fmt = textData(g, 102500017);
            Addr count = stringBuffer(g, {textDataStr(fmt), uint32_t(pieces)});
            Addr both = stringBuffer(g, {S_TwoStrings, strData(text), count});
            Addr s = newStr(g, both);
            call(sfn::Text_setString, {category, s});
            strFree(s);
            sharedRelease(fmt);
        }
        strFree(text);
    }
    strFree(prefix);
    sharedRelease(row);
}
RW_FUNCTION(0x553C8C, "AvatarPartsScrollView::changeRow", AvatarPartsScrollView::changeRow);
