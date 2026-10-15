// SceneUtil::updateMixCircle: mix circle and guilt display of a medal.
#include "B2sCommon.hpp"

using namespace b2s;

namespace {
constexpr Addr SceneUtil_setShadow = 0x45BD15;
constexpr Addr StringBuffer_ctor = 0x49140D;
constexpr Addr TextData_get = 0x4928AD;
constexpr Addr ImageView_loadTexture = 0x839731;
constexpr Addr Text_setString = 0x83A2DD;
constexpr Addr Vec2_ctorXY = 0x8E4B9D;
constexpr Addr Vec2_dtor = 0x8E4C17;
}  // namespace

// ---------------------------------------------------------------------------------------------------------------
// SceneUtil::updateMixCircle(Widget *panel, Widget *guiltPanel, Widget *guiltPlate,
//                            shared_ptr<UserMedalData> *medal, bool a5, int a6, bool a7, bool a8, bool a9,
//                            bool a10, bool a11)
// The mix circle of a medal: one check circle sprite per burst slot (filled as many as the medal has), and the
// guilt icon / plate with the guilt factor texts and colours. a5..a11 are stack words of the caller (frame +0x1568).
namespace {
constexpr Addr Burst_create = 0x77A491;
constexpr Addr SceneMixedMedal_getGuiltFactorValueRange = 0x69A309;     // (int guiltId, unsigned (&range)[2])
constexpr Addr SceneUtil_setParameterUpEffect = 0x465995;
constexpr Addr Sprite_create = 0x8A637D;
constexpr Addr FadeIn_create = 0x86B115;
constexpr Addr FadeOut_create = 0x86B25D;
constexpr Addr Sequence_create = 0x867999;
constexpr Addr RepeatForever_create = 0x86815D;
constexpr Addr Node_stopAllActions = 0x893815;
constexpr Addr Color3B_ctor = 0x9062E7;
constexpr Addr aeabi_udivsi3 = 0xCD5DF9;
constexpr Addr floatsisf = 0xCD6D68;
constexpr Addr subsf3 = 0xCD6BCC;
constexpr uint32_t HALF = 0x3F000000, SIXTEEN = 0x41800000, TEN = 0x41200000, BLINK_TIME = 0x3EA9FBE7;
}  // namespace

void SceneUtil_updateMixCircle(uint32_t a1, uint32_t a2, uint32_t a3, uint32_t a4) {
    guest::Frame F(0x1568);
    auto fa = [&](uint32_t off) { return F.addr(off); };
    const Addr args = fa(0x1568);                       // the caller's stack words
    auto arg = [&](uint32_t k) { return ld32(args + 4 * k); };
    const uint32_t a5 = arg(0), a6 = arg(1), a7 = arg(2), a8 = arg(3), a9 = arg(4), a10 = arg(5), a11 = arg(6);
    auto row = [&]() { return ld32(ld32(a4) + 0x58); };   // the SafeBuffer of the medal data
    auto readRow = [&](Addr out, uint32_t offset) {
        const Addr sb = row();
        cx(SAFEBUFFER_READ, {sb, out, ld32(sb + 0xC) + offset, 4, offset});
    };
    auto blink = [&](Addr node) {                       // node->runAction(RepeatForever(Sequence(FadeIn, FadeOut)))
        const Addr in = cx(FadeIn_create, {BLINK_TIME});
        const Addr out = cx(FadeOut_create, {BLINK_TIME});
        const Addr seq = cx(Sequence_create, {in, out, 0});
        cx(RUN_ACTION, {node, cx(RepeatForever_create, {seq})});
    };
    auto fadd = [&](uint32_t x, uint32_t y) { return cx(FADD, {x, y}); };
    auto fmul = [&](uint32_t x, uint32_t y) { return cx(FMUL, {x, y}); };
    auto fsub = [&](uint32_t x, uint32_t y) { return cx(subsf3, {x, y}); };
    auto setColor = [&](Addr node, Addr slotFn, Addr colorBuf, uint32_t g) {
        cx(Color3B_ctor, {colorBuf, 0xFF, g, 0});
        cx(slotFn, {node, colorBuf});
    };

    vcx(a1, 0x138, {a1});                               // removeAllChildren
    {
        const Addr sb = row();
        cx(SAFEBUFFER_READ, {sb, fa(0x114), ld32(sb + 0xC) + 0x4A8, 4, 0x4A8});
    }
    if (ld32(fa(0x114)) != 1) return;
    readRow(fa(0x118), 0x4AC);
    cx(Burst_create, {fa(0x10C), ld32(fa(0x118))});
    {
        const Addr burst = ld32(fa(0x10C));
        cx(SAFEBUFFER_READ, {burst, fa(0x118), ld32(burst + 0xC) + 0x158, 4, 0x158});
    }
    const int32_t circles = int32_t(ld32(fa(0x118)));                // burst slots
    const uint32_t sumA = a10 | a11;
    if (circles >= 1) {
        const uint32_t emptyCircles = uint32_t(5 - circles);
        for (int32_t i = 0;;) {
            Addr sprite;
            if (i >= int32_t(ld32(ld32(a4) + 0x50)) + int32_t(a6)) {
                strInit(fa(0xF4), 0xDF9AB3, 0x18);                         // "img/ui/Check_Circle1.png"
                sprite = cx(Sprite_create, {fa(0xF4)});
                strFree(fa(0xF4));
            } else {
                strInit(fa(0x100), 0xDF9A9A, 0x18);                        // "img/ui/Check_Circle2.png"
                sprite = cx(Sprite_create, {fa(0x100)});
                strFree(fa(0x100));
                if (!(int32_t(ld32(ld32(a4) + 0x50)) > i)) blink(sprite);
            }
            vcx(sprite, 0x9C, {sprite, ld32(0xFD6054)});                   // anchor: middle
            const Addr setPosition = ld32(ld32(sprite) + 0x4C);
            const Addr size = vcx(a1, 0xAC, {a1});
            const uint32_t slotIndex = sumA == 1 ? uint32_t(i) : emptyCircles + uint32_t(i);
            const uint32_t x = fmul(fadd(cx(floatsisf, {slotIndex}), HALF), SIXTEEN);
            const uint32_t y = fmul(ld32(size + 4), HALF);
            const Addr vec = fa(sumA == 1 ? 0xEC : 0xE4);
            cx(Vec2_ctorXY, {vec, x, y});
            cx(setPosition, {sprite, vec});
            cx(Vec2_dtor, {vec});
            vcx(a1, 0xF0, {a1, sprite});
            i++;
            if (i >= circles) break;
        }
    }
    if (a2 == 0) {                                      // (the original runs the common ending: release, return)
        sharedRelease(ld32(fa(0x110)));
        return;
    }
    const uint32_t circleShift = fmul(cx(floatsisf, {uint32_t(5 - circles)}), SIXTEEN);
    uint32_t v84;                                        // width of the guilt panel, 0 once the guilt icon is shown
    {
        const Addr size = vcx(a2, 0xAC, {a2});
        v84 = ld32(size);
    }
    const Addr guiltIcon = findNode(a2, fa(0xD8), 0xDF9ACC, 0xA);                      // "Guilt_Icon"
    const Addr flashIcon = findNode(guiltIcon, fa(0xCC), 0xDF9AD7, 0xE);               // "GuiltFlashIcon"
    if (flashIcon != 0) vcx(flashIcon, 0x124, {flashIcon});                            // removeFromParent
    // right edge of the plate (or of the guilt panel)
    uint32_t edge;
    {
        const Addr ref = a3 != 0 ? a3 : a2;
        const uint32_t posX = vcx(ref, 0x68, {ref});
        const Addr size = vcx(ref, 0xAC, {ref});
        const uint32_t width = ld32(size);
        const uint32_t scale = vcx(ref, 0x44, {ref});
        edge = fadd(posX, fmul(width, scale));
    }
    // a node's width times its scale
    auto scaledWidth = [&](Addr node) {
        const Addr size = vcx(node, 0xAC, {node});
        const uint32_t width = ld32(size);
        const uint32_t scale = vcx(node, 0x44, {node});
        return fmul(width, scale);
    };
    readRow(fa(0x11C), 0x478);
    const uint32_t guiltKind = ld32(fa(0x11C));
    if (guiltKind != 0 && a5 == 1) {
        vcx(a2, slot::setVisible, {a2, 1});
        if (ld32(ld32(a4) + 0x54) != 0) {
            // ---- guilt on
            cx(StringBuffer_ctor, {fa(0x1148), 0xDF9AE6, guiltKind});                  // "img/ui/guilt/Guilt%d_On.png"
            strInit(fa(0xC0), fa(0x1148), cx(STRLEN, {fa(0x1148)}));
            v84 = 0;
            cx(ImageView_loadTexture, {guiltIcon, fa(0xC0), 0});
            strFree(fa(0xC0));
            if (a3 != 0) {
                st32(fa(0xB8), 0);                                                      // guilt factor range (low, high)
                st32(fa(0xBC), 0);
                readRow(fa(0x120), 0x478);
                cx(SceneMixedMedal_getGuiltFactorValueRange, {ld32(fa(0x120)), fa(0xB8)});
                vcx(a3, slot::setVisible, {a3, 1});
                edge = fsub(edge, scaledWidth(a3));
                const Addr plate = findNode(a3, fa(0xAC), 0xDF9B02, 0xF);              // "Txt_Guilt_Plate"
                cx(SceneUtil_setShadow, {plate});
                cx(Node_stopAllActions, {plate});
                vcx(plate, slot::setOpacity, {plate, 255});
                const uint32_t low = ld32(fa(0xB8)), high = ld32(fa(0xBC));
                Addr textCtrl;
                if (int32_t(a6) >= 1) {
                    const Addr setColorFn = ld32(ld32(plate) + 0x22C);
                    setColor(plate, setColorFn, fa(0x124), 0xEA);
                    blink(plate);
                    cx(TextData_get, {fa(0x98), 0, 0x609B742});
                    const uint32_t lowPct = cx(aeabi_udivsi3, {low, 100});
                    const uint32_t highPct = cx(aeabi_udivsi3, {high, 100});
                    cx(StringBuffer_ctor, {fa(0xD44), ld32(ld32(fa(0x98)) + 4), lowPct, highPct});
                    strInit(fa(0xA0), fa(0xD44), cx(STRLEN, {fa(0xD44)}));
                    cx(Text_setString, {plate, fa(0xA0)});
                    strFree(fa(0xA0));
                    textCtrl = ld32(fa(0x9C));
                } else {
                    const Addr setColorFn = ld32(ld32(plate) + 0x22C);
                    const uint32_t rank = ld32(ld32(a4) + 0x54);
                    if (a9 == 1) {
                        cx(SceneUtil_setParameterUpEffect, {plate});
                        if (high == rank) setColor(plate, ld32(ld32(plate) + 0x22C), fa(0x128), 0x84);
                        else setColor(plate, ld32(ld32(plate) + 0x22C), fa(0x12C), 0xEA);
                    } else if (high != rank || a8 != 0) {
                        setColor(plate, setColorFn, fa(0x134), 0xEA);
                    } else {
                        setColor(plate, setColorFn, fa(0x130), 0x84);
                    }
                    if (a8 == 1) {
                        cx(TextData_get, {fa(0x84), 0, 0x609B746});
                        const Addr text = ld32(ld32(fa(0x84)) + 4);
                        strInit(fa(0x8C), text, cx(STRLEN, {text}));
                        cx(Text_setString, {plate, fa(0x8C)});
                        strFree(fa(0x8C));
                        textCtrl = ld32(fa(0x88));
                    } else {
                        cx(TextData_get, {fa(0x70), 0, 0x609B741});
                        const uint32_t pct = cx(aeabi_udivsi3, {ld32(ld32(a4) + 0x54), 100});
                        cx(StringBuffer_ctor, {fa(0x940), ld32(ld32(fa(0x70)) + 4), pct});
                        strInit(fa(0x78), fa(0x940), cx(STRLEN, {fa(0x940)}));
                        cx(Text_setString, {plate, fa(0x78)});
                        strFree(fa(0x78));
                        textCtrl = ld32(fa(0x74));
                    }
                }
                sharedRelease(textCtrl);
            }
            // the guilt panel / a1 move next to the plate
            {
                const uint32_t nextEdge = fsub(edge, scaledWidth(a2));
                if (a10 == 0 && a11 == 0) vcx(a2, 0x64, {a2, nextEdge});               // setPositionX
                edge = nextEdge;
            }
            {
                const uint32_t nextEdge = fsub(edge, scaledWidth(a1));
                if ((a10 | a11) == 0) vcx(a1, 0x64, {a1, nextEdge});
                // (the 2nd subtraction feeds the final position only through a1's own X: kept in `edge`)
                edge = nextEdge;
                vcx(a1, slot::setVisible, {a1, a7});
            }
        } else {
            // ---- guilt off
            cx(StringBuffer_ctor, {fa(0x53C), 0xDF9B12, guiltKind});                   // "img/ui/guilt/Guilt%d_Off.png"
            strInit(fa(0x64), fa(0x53C), cx(STRLEN, {fa(0x53C)}));
            cx(ImageView_loadTexture, {guiltIcon, fa(0x64), 0});
            v84 = 0;
            strFree(fa(0x64));
            if (a3 != 0) vcx(a3, slot::setVisible, {a3, 0});
            const uint32_t nextEdge = fsub(edge, scaledWidth(a2));
            const uint32_t gate = a10 | a11;
            if (gate == 0) vcx(a2, 0x64, {a2, nextEdge});
            {
                const uint32_t w1 = scaledWidth(a1);
                if (gate == 0) vcx(a1, 0x64, {a1, fsub(nextEdge, w1)});
            }
            vcx(a1, slot::setVisible, {a1, 1});
            if (int32_t(a6) >= 1 && circles < int32_t(ld32(ld32(a4) + 0x50)) + int32_t(a6)) {
                cx(StringBuffer_ctor, {fa(0x138), 0xDF9AE6, guiltKind});               // "img/ui/guilt/Guilt%d_On.png"
                strInit(fa(0x58), fa(0x138), cx(STRLEN, {fa(0x138)}));
                const Addr flash = cx(Sprite_create, {fa(0x58)});
                strFree(fa(0x58));
                if (gate == 0) {
                    const Addr setPositionXY = ld32(ld32(flash) + 0x5C);
                    const Addr sizeA = vcx(guiltIcon, 0xAC, {guiltIcon});
                    const uint32_t width = ld32(sizeA);
                    const Addr sizeB = vcx(guiltIcon, 0xAC, {guiltIcon});
                    const uint32_t halfW = fmul(width, HALF);
                    const uint32_t halfH = fmul(ld32(sizeB + 4), HALF);
                    cx(setPositionXY, {flash, halfW, halfH});
                }
                const Addr addChildZName = ld32(ld32(guiltIcon) + slot::addChildZName);
                strInit(fa(0x4C), 0xDF9AD7, 0xE);                                      // "GuiltFlashIcon"
                cx(addChildZName, {guiltIcon, flash, 0, fa(0x4C)});
                strFree(fa(0x4C));
                v84 = 0;
                blink(flash);
            }
        }
    } else {
        vcx(a1, slot::setVisible, {a1, 1});
        vcx(a2, slot::setVisible, {a2, 0});
        if (a3 != 0) vcx(a3, slot::setVisible, {a3, 0});
        const uint32_t w1 = scaledWidth(a1);
        if (a10 == 0 && a11 == 0) {
            vcx(a1, 0x64, {a1, fsub(edge, fadd(w1, TEN))});
        }
    }
    if (a11 == 1) {
        const Addr setPositionX = ld32(ld32(a1) + 0x64);
        const uint32_t posX = vcx(a1, 0x68, {a1});
        const uint32_t shifted = fmul(fadd(circleShift, v84), HALF);
        cx(setPositionX, {a1, fadd(shifted, posX)});
    }
    sharedRelease(ld32(fa(0x110)));                     // control block of the Burst row
}

RW_FUNCTION(0x465C84, "SceneUtil::updateMixCircle(Widget*, Widget*, Widget*, shared_ptr<UserMedalData>, bool, int, bool, bool, bool, bool, bool)", SceneUtil_updateMixCircle);
