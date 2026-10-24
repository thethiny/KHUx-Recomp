// SceneDrawRaise::openDrawResultsPopup(ids): the results of a draw (DrowResult2Scene). A single medal is centred in
// Medal_Area2 (large card); more go into Medal_Area: up to 5 in one row, more in two rows (the first n - n/2 in the
// upper row), 186 x 205 points apart, centred. Each card shows "New" when the id is among the API's newly obtained
// medals and a sparkle LWF from rarity 5. The cards are remembered as DropMedalLayer {card, new, id} for the drop
// animation. Then the buttons: the whole-screen ImageButton (skip), OK ($_10), share ($_11), and after a short
// delay $_12 starts the flash / drop (GachaLambdas.cpp).
//
// The original frame (0x310 bytes) is kept: getMedalData / createDrawMedal take stack arguments from it and the
// DropMedalLayer's padding bytes are copied from it.
#include "../scene/SceneUi.hpp"

using namespace scene;

namespace {
namespace g {
constexpr uint32_t SceneManager_getPopupLayer = 0x613E69;
constexpr uint32_t SceneUtil_createShadowSprite = 0x45AD69;
constexpr uint32_t SceneUtil_createNodeWithSceneFile = 0x45AA49;
constexpr uint32_t ImageView_create = rwlib::cocos2d::ui::ImageView::create;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff, Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t Size_div = rwlib::cocos2d::Size::op_div;                       // Size::operator/(Size out, const Size*, float)
constexpr uint32_t VectorULL_copy = 0x468431, VectorULL_dtor = 0x468499;
constexpr uint32_t MedalDataManager_getInstance = 0x76AD69;
constexpr uint32_t MedalDataManager_getMedalData = 0x76ADB5;   // (sret, mgr, u64 id, int, Medal by value)
constexpr uint32_t SceneDrawRaise_createDrawMedal = 0x6914CD;  // (this unused, u64 id, bool large, int)
constexpr uint32_t DropMedalVector_pushBackSlow = 0x69421D;
constexpr uint32_t AudioLWFNode_create = 0x5EB6B5;
constexpr uint32_t ImageButton_create = 0x531239;              // (std::string by value, bool)
constexpr uint32_t ImageButton_setTargetView = 0x5318C1;
constexpr uint32_t ImageButton_addTouchEventListener = 0x5315B1;
constexpr uint32_t Widget_addTouchEventListener = rwlib::cocos2d::ui::Widget::addTouchEventListener;
constexpr uint32_t SafeBuffer_read = 0x45A269;
constexpr uint32_t aeabi_memset = 0x40E7A0;
constexpr uint32_t modsi3 = 0xCD6009;
constexpr uint32_t ThrowOutOfRange = 0x47CCE1;
}  // namespace g

constexpr uint32_t S_SceneJson = 0xE04BC3;    // "cocostudio/publish/DrowResult2Scene_ver120.json"
constexpr uint32_t S_BG = 0xE04BF3, S_BgPng = 0xE04BF6;   // "cocostudio/publish/Draw_MedalGet_BG.png"
constexpr uint32_t S_CenterUI = 0xE02FBA, S_Medal_Area2 = 0xE04C1E, S_Medal_Area = 0xE00CB0;
constexpr uint32_t S_Icon_New = 0xDFDD8A, S_Area_Point = 0xDF9FB4;
constexpr uint32_t S_RareLwf = 0xE04C2A;      // "lwf/raisedraw/demo/Draw_RareMedal/Draw_RareMedal.lwf"
constexpr uint32_t S_ShopBuy_But = 0xE00327, S_CenterButton = 0xDFFB40, S_Button_OK = 0xDF8688, S_Txt_OK = 0xDF869F;
constexpr uint32_t S_Button_FB_Share = 0xDFE775, S_Txt_FB_Share = 0xDFE785;
constexpr uint32_t S_Dummy_Panel = 0xDFF409, S_Txt_Wording = 0xDF862F;
constexpr int32_t TXT_OK = 100100001, TXT_SHARE = 104900010, TXT_WORDING = 102600046;

constexpr uint32_t F_DROP_MEDALS = 0x294, F_DROP_INTERVAL = 0x2A4, F_DROP_DELAY = 0x2A8;   // SceneDrawRaise
constexpr uint32_t API_USER = 0x28C;          // -> +0x28 vector<ull>: the medals obtained for the first time
constexpr uint32_t MEDAL_SB = 0x58, MEDAL_RARITY = 0x470;
constexpr uint32_t V_REMOVE_CHILD_BY_NAME = 0x134;
constexpr float CELL_W = 186.0f, CELL_H = 205.0f;

constexpr uint32_t VT_Skip = 0xF65310;        // the whole-screen button's lambda {this}
constexpr uint32_t VT_Ok = 0xF65234;          // $_10 {this, this, root, bg, layer, shadow}
constexpr uint32_t VT_Share = 0xF651B0;       // $_11 {this, layer}
constexpr uint32_t VT_Flash = 0xF65158;       // $_12 {root, this, ok, dummy, share}

void setVisible(uint32_t node, uint32_t v) { vcall(node, node::setVisible, {node, v}); }
void setPos(uint32_t node, float x, float y) {
    Frame f;
    uint32_t v = f.alloc(8);
    call(g::Vec2_ctor, {v, fbits(x), fbits(y)});
    vcall(node, node::setPosition, {node, v});
    call(g::Vec2_dtor, {v});
}
// node->getContentSize() / 2
Size halfSize(uint32_t node) {
    Frame f;
    uint32_t out = f.alloc(8);
    call(g::Size_div, {out, vcall(node, node::getContentSize, {node}), fbits(2.0f)});
    return Size{ldf(out), ldf(out + 4)};
}

struct DrawResultsPopup {
    static void open(uint32_t self, uint32_t ids) {
        // first: every call runs below the original frame, so the stale bytes in it are the original's
        guest::Frame frame(0x310);
        const uint32_t sp = frame.addr();
        call(fn::AudioPlaySystemSEPopUp, {});
        uint32_t layer = call(g::SceneManager_getPopupLayer, {sceneManager()});
        uint32_t shadow = call(g::SceneUtil_createShadowSprite, {});
        vcall(shadow, node::setOpacity, {shadow, 0});
        vcall(layer, node::addChild, {layer, shadow});
        uint32_t root;
        {
            Frame f;
            uint32_t s = newStr(f, S_SceneJson, 0x2F);
            root = call(g::SceneUtil_createNodeWithSceneFile, {s});
            strFree(s);
        }
        uint32_t bg = call(g::ImageView_create, {});
        if (findNode(root, S_BG, 2)) {
            Frame f;
            uint32_t s = newStr(f, S_BG, 2);
            vcall(root, V_REMOVE_CHILD_BY_NAME, {root, s, 1});
            strFree(s);
        }
        {
            Frame f;
            uint32_t s = newStr(f, S_BgPng, 0x27);
            call(fn::ImageView_loadTexture, {bg, s, 0});
            strFree(s);
        }
        Size vis = visibleSize();
        setPos(bg, vis.width * 0.5f, vis.height * 0.5f);
        vcall(layer, node::addChild, {layer, bg});
        vcall(layer, node::addChild, {layer, root});
        {
            Frame f;
            uint32_t anchor = f.alloc(8);
            call(g::Vec2_ctor, {anchor, fbits(0.5f), fbits(0.5f)});
            vcall(root, node::setAnchorPoint, {root, anchor});
            call(g::Vec2_dtor, {anchor});
        }
        vcall(root, node::setPosition, {root, vcall(shadow, node::getPosition, {shadow})});
        uint32_t centerUI = findNode(root, S_CenterUI, 8);
        uint32_t area2 = findNode(root, S_Medal_Area2, 11);
        uint32_t area = findNode(root, S_Medal_Area, 10);
        Size areaHalf = halfSize(area);
        const int32_t bytes = int32_t(ld32(ids + 4) - ld32(ids));
        const int32_t count = bytes >> 3;
        const int32_t perRow = uint32_t(count) >= 6 ? count / 2 : count;

        const uint32_t newIds = sp + 0x1C4;
        call(g::VectorULL_copy, {newIds, ld32(call(fn::APIManager_getInstance, {}) + API_USER) + 0x28});
        if (bytes >= 1) {
            const float fPerRowM1 = float(perRow - 1);
            for (int32_t i = 0;;) {
                if (uint32_t(count) <= uint32_t(i)) call(g::ThrowOutOfRange, {ids});
                const uint32_t lo = ld32(ld32(ids) + 8 * i), hi = ld32(ld32(ids) + 8 * i + 4);
                const uint32_t drop = sp + 0x1B0;          // DropMedalLayer {card, new, id}
                st32(drop + 8, lo);
                st32(drop + 0xC, hi);
                uint32_t it = ld32(newIds), end = ld32(newIds + 4);
                for (; it != end; it += 8)
                    if (ld32(it) == lo && ld32(it + 4) == hi) break;
                const bool isNew = it != end;
                *rw::frame_ptr<uint8_t>(drop + 4) = isNew ? 1 : 0;
                // the medal (its rarity)
                const uint32_t mgr = call(g::MedalDataManager_getInstance, {});
                const uint32_t record = sp + 0x2A8;
                call(g::aeabi_memset, {record, 0x2C, 0});
                call(g::aeabi_memset, {record + 0x30, 0x18, 0});
                *rw::frame_ptr<uint32_t>(sp) = 0;
                for (uint32_t k = 0; k < 0x48; k += 4) *rw::frame_ptr<uint32_t>(sp + 8 + k) = ld32(record + k);
                const uint32_t medal = sp + 0x1A8;
                rw::call_exact(g::MedalDataManager_getMedalData, {medal, mgr, lo, hi});
                // the card: large and alone in Medal_Area2, or small in Medal_Area (the first 5 in front)
                uint32_t card;
                if (bytes == 8) {
                    *rw::frame_ptr<uint32_t>(sp) = 1;
                    *rw::frame_ptr<uint32_t>(sp + 4) = 0;
                    card = uint32_t(rw::call_exact(g::SceneDrawRaise_createDrawMedal, {sp + 8, 1, lo, hi}));
                    st32(drop, card);
                    vcall(area2, node::addChild, {area2, card});
                } else if (i <= 4) {
                    *rw::frame_ptr<uint32_t>(sp) = 0;
                    *rw::frame_ptr<uint32_t>(sp + 4) = 10;
                    card = uint32_t(rw::call_exact(g::SceneDrawRaise_createDrawMedal, {sp, 10, lo, hi}));
                    st32(drop, card);
                    vcall(area, node::addChildZ, {area, card, 10});
                } else {
                    *rw::frame_ptr<uint32_t>(sp) = 0;
                    *rw::frame_ptr<uint32_t>(sp + 4) = 0;
                    card = uint32_t(rw::call_exact(g::SceneDrawRaise_createDrawMedal, {sp, 0, lo, hi}));
                    st32(drop, card);
                    vcall(area, node::addChildZ, {area, card, 0});
                }
                vcall(card, node::setAnchorPoint, {card, VEC2_ANCHOR_MIDDLE});
                vcall(card, node::setOpacity, {card, 0});
                uint32_t icon = findNode(card, S_Icon_New, 8);
                setVisible(icon, isNew ? 1 : 0);
                const int32_t rarity = safe32(ld32(ld32(medal) + MEDAL_SB), MEDAL_RARITY);
                if (rarity >= 5) {
                    uint32_t point = findNode(card, S_Area_Point, 10);
                    uint32_t lwf = call(g::AudioLWFNode_create, {S_RareLwf, 0});
                    vcall(point, node::addChild, {point, lwf});
                    if (bytes != 8) vcall(lwf, node::setScale, {lwf, fbits(0.45f)});
                }
                const uint32_t cell = sp + 0x188;
                call(g::Vec2_ctor, {cell, fbits(CELL_W), fbits(CELL_H)});
                const float cellW = ldf(cell), cellH = ldf(cell + 4);
                if (bytes == 8) {
                    Size half2 = halfSize(area2);
                    setPos(card, half2.width, half2.height);
                } else if (uint32_t(int32_t(ld32(ids + 4) - ld32(ids)) >> 3) <= 5) {
                    // one row of `perRow`, centred
                    const float x0 = areaHalf.width + fPerRowM1 * (cellW * -0.5f), y0 = areaHalf.height;
                    const float m = float(int32_t(call(g::modsi3, {uint32_t(i), uint32_t(perRow)})));
                    setPos(card, x0 + m * cellW, y0);
                } else {
                    // two rows: the upper one holds n - perRow cards, the lower one perRow
                    const int32_t upper = count - perRow;
                    const float halfW = cellW * 0.5f;
                    float x0, y0;
                    if (i < upper) {
                        x0 = areaHalf.width - float(upper - 1) * halfW;
                        y0 = areaHalf.height + cellH * 0.5f;
                    } else {
                        x0 = areaHalf.width - fPerRowM1 * halfW;
                        y0 = areaHalf.height + cellH * -0.5f;
                    }
                    const float m = float(int32_t(call(g::modsi3, {uint32_t(i), uint32_t(upper)})));
                    setPos(card, x0 + m * cellW, y0);
                }
                // remembered for the drop animation
                const uint32_t drops = self + F_DROP_MEDALS;
                const uint32_t last = ld32(drops + 4);
                if (last == ld32(drops + 8)) {
                    call(g::DropMedalVector_pushBackSlow, {drops, drop});
                } else {
                    uint32_t next = 0;
                    if (last) {
                        for (uint32_t k = 0; k < 16; k += 4) st32(last + k, ld32(drop + k));
                        next = ld32(drops + 4);
                    }
                    st32(drops + 4, next + 0x10);
                }
                call(g::Vec2_dtor, {cell});
                if (ld32(medal + 4)) call(fn::shared_release, {ld32(medal + 4)});
                ++i;
                if (i >= int32_t(ld32(ids + 4) - ld32(ids)) >> 3) break;
            }
        }
        stf(self + F_DROP_INTERVAL, 0.15f);
        stf(self + F_DROP_DELAY, 0.1f);
        // the whole-screen button (skip the drop)
        uint32_t skip;
        {
            Frame f;
            uint32_t name = newStr(f, S_ShopBuy_But, 11);
            skip = call(g::ImageButton_create, {name, 0});
            strFree(name);
        }
        call(g::ImageButton_setTargetView, {skip, root, fbits(1.0f)});
        {
            Frame f;
            uint32_t name = newStr(f, S_CenterButton, 12);
            vcall(root, node::addChildZName, {root, skip, 0, name});
            strFree(name);
        }
        {
            Frame f;
            uint32_t touch = newFunction(f, VT_Skip, {self});
            call(g::ImageButton_addTouchEventListener, {skip, touch});
            functionDestroy(touch);
        }
        uint32_t ok = findNode(centerUI, S_Button_OK, 9);
        {
            Frame f;
            uint32_t touch = newEmptyFunction(f);
            uint32_t heap = call(fn::operator_new, {0x1C});
            if (heap) {
                st32(heap, VT_Ok);
                st32(heap + 4, self);
                st32(heap + 8, self);
                st32(heap + 0xC, root);
                st32(heap + 0x10, bg);
                st32(heap + 0x14, layer);
                st32(heap + 0x18, shadow);
            }
            st32(touch + 16, heap);
            call(g::Widget_addTouchEventListener, {ok, touch});
            functionDestroy(touch);
        }
        setVisible(ok, 0);
        titleLabel(ok, S_Txt_OK, 6, TXT_OK);
        uint32_t share = findNode(centerUI, S_Button_FB_Share, 15);
        {
            Frame f;
            uint32_t touch = newFunction(f, VT_Share, {self, layer});
            call(g::Widget_addTouchEventListener, {share, touch});
            functionDestroy(touch);
        }
        setVisible(share, 0);
        titleLabel(share, S_Txt_FB_Share, 12, TXT_SHARE);
        uint32_t dummy = findNode(centerUI, S_Dummy_Panel, 11);
        setVisible(dummy, 0);
        titleLabel(dummy, S_Txt_Wording, 11, TXT_WORDING);
        // after the delay, the flash and the drop ($_12)
        uint32_t delay = call(fn::DelayTime_create, {ld32(self + F_DROP_DELAY)});
        {
            Frame f;
            uint32_t flash = newEmptyFunction(f);
            uint32_t heap = call(fn::operator_new, {0x18});
            if (heap) {
                st32(heap, VT_Flash);
                st32(heap + 4, root);
                st32(heap + 8, self);
                st32(heap + 0xC, ok);
                st32(heap + 0x10, dummy);
                st32(heap + 0x14, share);
            }
            st32(flash + 16, heap);
            uint32_t seq = call(fn::Sequence_create, {delay, call(fn::CallFunc_create, {flash}), 0});
            call(fn::Node_runAction, {root, seq});
            functionDestroy(flash);
        }
        call(g::VectorULL_dtor, {newIds});
    }
};
}  // namespace

RW_FUNCTION(0x690658, "SceneDrawRaise::openDrawResultsPopup(const vector<unsigned long long>&)", DrawResultsPopup::open);
