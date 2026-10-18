// LuxBoard (the Lux gauge board) and Field (the tiled 2048x2048 scrolling map texture of the action map).
#include "../core/Guest.hpp"
#include "../scene/SceneCommon.hpp"

namespace w4b2 {
using namespace scene;

constexpr uint32_t REF_AUTORELEASE = 0x8FD0D5;
constexpr uint32_t NODE_CTOR = 0x892531, NODE_DTOR = 0x8927ED;
constexpr uint32_t VEC2_CTOR = 0x8E4B95, VEC2_DTOR = 0x8E4C17;

// ============================================================================================ LuxBoard
// (a cocos2d::Node) two LWF-backed panels +0x270 / +0x274, +0x278, positions +0x27C / +0x284 (Vec2)
struct LuxBoard {
    static uint32_t create() {
        uint32_t obj = call(fn::operator_new, {0x290});
        call(0x7639C1, {obj});
        if (vcall(obj, 0x248, {obj}) == 1) {  // init
            call(REF_AUTORELEASE, {obj});
            return obj;
        }
        vcall(obj, 4, {obj});
        return 0;
    }
    static uint32_t construct(uint32_t self) {
        call(NODE_CTOR, {self});
        st32(self, ld32(0xFD6794) + 8);
        st32(self + 0x270, 0);
        st32(self + 0x274, 0);
        st32(self + 0x278, 0);
        call(VEC2_CTOR, {self + 0x27C});
        call(VEC2_CTOR, {self + 0x284});
        return self;
    }
    static uint32_t destruct(uint32_t self) {
        st32(self, ld32(0xFD6794) + 8);
        call(0x894369, {self, 1});  // Node::removeAllChildrenWithCleanup(true)
        call(VEC2_DTOR, {self + 0x284});
        call(VEC2_DTOR, {self + 0x27C});
        call(NODE_DTOR, {self});
        return self;
    }
    // the panel's LWF: play `label` (-1, -1), stop, and hook its two callbacks
    static void setupPanel(Frame &f, uint32_t self, uint32_t panel, uint32_t label, uint32_t labelLen,
                           uint32_t vtA, uint32_t vtB) {
        uint32_t lwf = vcall(panel, 0x298, {panel});
        uint32_t play = ld32(ld32(lwf) + 0x68);
        uint32_t s = f.alloc(12);
        strInit(s, label, labelLen);
        call(play, {lwf, s, 0xFFFFFFFF, 0xFFFFFFFF});
        strFree(s);
        vcall(lwf, 0x80, {lwf, 0});
        uint32_t a = newFunction(f, vtA, {self});
        call(0x7A3C69, {lwf, a});
        functionDestroy(a);
        uint32_t b = newFunction(f, vtB, {self});
        call(0x7A3CE5, {lwf, b});
        functionDestroy(b);
    }
    static uint32_t init(uint32_t self) {
        Frame f;
        if (call(0x892B69, {self}) != 1) return 0;  // Node::init
        uint32_t s = f.alloc(12);
        strInit(s, 0xE01F45, 0x13);
        st32(self + 0x270, call(0x79F3C7, {s}));  // the panel node for that LWF
        strFree(s);
        uint32_t s2 = f.alloc(12);
        strInit(s2, 0xE01F45, 0x13);
        st32(self + 0x274, call(0x79F3C7, {s2}));
        strFree(s2);
        setupPanel(f, self, ld32(self + 0x270), 0xE01F36, 0xE, 0xF5D490, 0xF5D464);
        setupPanel(f, self, ld32(self + 0x274), 0xE01F25, 0x10, 0xF5D438, 0xF5D40C);
        uint32_t panel = ld32(self + 0x270);
        uint32_t getChild = ld32(ld32(panel) + 0x27C);
        uint32_t s3 = f.alloc(12);
        strInit(s3, 0xE01F1B, 9);
        uint32_t child = call(getChild, {panel, s3});
        strFree(s3);
        vcall(child, 0xB0, {child, 1});                            // setVisible(true)
        vcall(self, 0xF4, {self, ld32(self + 0x270), 10});         // addChild(panel, 10)
        vcall(self, 0xF0, {self, ld32(self + 0x274)});             // addChild
        vcall(self, 0xB0, {self, 0});                              // setVisible(false)
        return 1;
    }
    static uint32_t firstCall() {
        guest::Frame f(0x424);
        uint32_t mgr = call(0x7AC9B5, {});
        const uint32_t buf = f.addr(0x1C), s = f.addr(0x10);
        call(0x49140D, {buf, 0xE01EED, 0xE01F45});  // StringBuffer(fmt, name)
        strInit(s, buf, host_strlen(buf));
        call(0x7AD7BD, {mgr, s});
        strFree(s);
        return 0;
    }

    // the turn counter: bones LuxTurnNum1..5 of the number armature show the digits (units first, -1 = blank),
    // the armature shifted right by the digit count
    static constexpr uint32_t L_NUMBER = 0x270, L_TEXT = 0x274;   // cocostudio::Armature*
    static constexpr uint32_t ARMATURE_GET_BONE = 0x27C, ARMATURE_GET_ANIMATION = 0x298;
    static constexpr uint32_t BONE_CHANGE_DISPLAY_WITH_INDEX = 0x7A2681;   // Bone::changeDisplayWithIndex(int, bool)
    static constexpr uint32_t LOG10 = 0x419E7C, DIV = 0x419E88;            // imports: log10(double), div(int, int)
    static constexpr uint32_t S_TURN_BONE = 0xE01F0E;                      // "LuxTurnNum%d"
    static constexpr uint32_t DIGITS_X = 0xE4B3D0;                         // float[5]: 75, 98, 121, 144, 167
    static constexpr int32_t MAX_NUMBER = 99999;
    static void setNumber(uint32_t self, int32_t number) {
        int32_t n = number > MAX_NUMBER ? MAX_NUMBER : number;
        int32_t digits = 1;
        if (n >= 1) {
            double x = double(n);
            uint64_t bits;
            std::memcpy(&bits, &x, 8);
            const uint64_t r = rw::call64(LOG10, {uint32_t(bits), uint32_t(bits >> 32)});
            double l;
            std::memcpy(&l, &r, 8);
            digits = int32_t(l) + 1;
        }
        guest::Frame f(0x44C);
        const uint32_t quotRem = f.addr(0x20), name = f.addr(0x34), nameStr = f.addr(0x28);
        uint32_t armature = ld32(self + L_NUMBER);
        for (int32_t k = 0; k < 5;) {
            const uint32_t getBone = ld32(ld32(armature) + ARMATURE_GET_BONE);
            k++;
            call(0x49140D, {name, S_TURN_BONE, uint32_t(k)});   // StringBuffer(fmt, k)
            strInit(nameStr, name, host_strlen(name));
            const uint32_t bone = call(getBone, {armature, nameStr});
            strFree(nameStr);
            if (bone) {
                if (k - 1 < digits) {
                    call(DIV, {quotRem, uint32_t(n), 10});
                    call(BONE_CHANGE_DISPLAY_WITH_INDEX, {bone, ld32(quotRem + 4), 1});
                    n = int32_t(ld32(quotRem));
                } else {
                    call(BONE_CHANGE_DISPLAY_WITH_INDEX, {bone, 0xFFFFFFFFu, 1});
                }
            }
            armature = ld32(self + L_NUMBER);
        }
        vcall(armature, node::setPositionX, {armature, ld32(DIGITS_X + 4 * uint32_t(digits) - 4)});
    }

    // shows the board (raised by 37 when `raised`), restarting both armatures' finished animations
    static void play(uint32_t self, uint32_t raised) {
        vcall(self, node::setPositionY, {self, raised ? fbits(37.0f) : 0u});
        restart(self + L_NUMBER);
        restart(self + L_TEXT);
        vcall(self, node::setVisible, {self, 1});
    }
    static void restart(uint32_t slot) {
        const uint32_t armature = ld32(slot);
        const uint32_t animation = vcall(armature, ARMATURE_GET_ANIMATION, {armature});
        if (vcall(animation, 0x1C, {animation}) != 0) return;    // isPlaying
        if (vcall(animation, 0x2C, {animation}) != 1) return;    // isComplete
        vcall(animation, 0x10, {animation});                     // resume
        vcall(ld32(slot), node::setVisible, {ld32(slot), 1});
    }

    // both animations' isPlaying() checked (and the results dropped): always false
    static uint32_t isFinish(uint32_t self) {
        for (uint32_t slot : {L_NUMBER, L_TEXT}) {
            const uint32_t armature = ld32(self + slot);
            const uint32_t animation = vcall(armature, ARMATURE_GET_ANIMATION, {armature});
            if (vcall(animation, 0x1C, {animation})) vcall(animation, 0x34, {animation});
        }
        return 0;
    }

    // the board's origin in world space, kept at +0x284
    static uint32_t getUIPos(uint32_t self) {
        Frame f;
        const uint32_t p = f.alloc(8);
        call(0x895A39, {p, self, ld32(0xFD605C)});   // convertToWorldSpace(Vec2::ZERO)
        st32(self + 0x288, ld32(p + 4));
        st32(self + 0x284, ld32(p));
        call(VEC2_DTOR, {p});
        return self + 0x284;
    }

    // `done` hidden and stopped; the board hides too unless `other` is still showing
    static void setCompleteVisible(uint32_t self, uint32_t done, uint32_t other) {
        vcall(done, node::setVisible, {done, 0});
        const uint32_t animation = vcall(done, ARMATURE_GET_ANIMATION, {done});
        vcall(animation, 0x80, {animation, 0});
        if (!vcall(other, node::isVisible, {other})) vcall(self, node::setVisible, {self, 0});
    }
};
// init: the turn-number panel's frame event: on "lux" the prizes waiting at +0x278 burst out at the board
static void onFrameEvent(uint32_t functor, uint32_t, uint32_t event) {
    const uint32_t self = ld32(functor + 4);
    if (call(0x47CBD5, {event, 0xDFA9FF}) != 0) return;   // std::string::compare("lux")
    if (const uint32_t prizes = ld32(self + 0x278)) {
        call(0x5DEAE1, {prizes, self + 0x27C, self + 0x27C});   // MapPrizes::eruption(from, to)
        st32(self + 0x278, 0);
    }
}
// init: a panel's movement event: when its animation completes it hides (the board too, unless the other panel
// still shows)
static void onNumberMovementEvent(uint32_t functor, uint32_t, uint32_t type) {
    if (ld32(type) != 1) return;
    const uint32_t self = ld32(functor + 4);
    LuxBoard::setCompleteVisible(self, ld32(self + LuxBoard::L_NUMBER), ld32(self + LuxBoard::L_TEXT));
}
static void onTextMovementEvent(uint32_t functor, uint32_t, uint32_t type) {
    if (ld32(type) != 1) return;
    const uint32_t self = ld32(functor + 4);
    LuxBoard::setCompleteVisible(self, ld32(self + LuxBoard::L_TEXT), ld32(self + LuxBoard::L_NUMBER));
}
RW_FUNCTION(0x5D4652, "LuxBoard::init number panel movement event lambda operator()", onNumberMovementEvent);
RW_FUNCTION(0x5D44CE, "LuxBoard::init text panel movement event lambda operator()", onTextMovementEvent);
RW_FUNCTION(0x5D4580, "LuxBoard::init frame event lambda operator() (\"lux\")", onFrameEvent);
RW_FUNCTION(0x5D38C0, "LuxBoard::setNumber", LuxBoard::setNumber);
RW_FUNCTION(0x5D3A44, "LuxBoard::play", LuxBoard::play);
RW_FUNCTION(0x5D3B08, "LuxBoard::isFinish", LuxBoard::isFinish);
RW_FUNCTION(0x5D3B68, "LuxBoard::getUIPos", LuxBoard::getUIPos);
RW_FUNCTION(0x5D3CAA, "LuxBoard::setCompleteVisible", LuxBoard::setCompleteVisible);
RW_FUNCTION(0x75F4F8, "LuxBoard::create", LuxBoard::create);
RW_FUNCTION(0x7639C0, "LuxBoard::LuxBoard", LuxBoard::construct);
RW_FUNCTION(0x5D3478, "LuxBoard::~LuxBoard", LuxBoard::destruct);
RW_FUNCTION(0x5D34F4, "LuxBoard::init", LuxBoard::init);
RW_FUNCTION(0x5D3BDC, "LuxBoard::firstCall", LuxBoard::firstCall);

// ============================================================================================ Field
// (a cocos2d::Node) texture +0x270, sprite +0x274, image name format +0x278, tiles across / down +0x27C/+0x280,
// map size px +0x284/+0x288, rounded to 256 +0x28C/+0x290, view size +0x294/+0x298, half view +0x29C/+0x2A0,
// loaded +0x2A4 (byte), 8x8 cache of the tile index in each 256px cell of the texture +0x2A8
struct Field {
    static int32_t div256(int32_t v) { return (v + int32_t(uint32_t(v >> 31) >> 24)) >> 8; }  // truncating
    static int32_t mod8(int32_t v) { return v - ((v + int32_t(uint32_t(v >> 31) >> 29)) & ~7); }
    static int32_t floor2048(int32_t v) { return (v + int32_t(uint32_t(v >> 31) >> 21)) & int32_t(0xFFFFF800); }

    static uint32_t create(int32_t viewW, int32_t viewH, uint32_t info) {
        uint32_t obj = call(fn::operator_new, {0x3A8});
        call(0x5CD101, {obj, uint32_t(viewW), uint32_t(viewH), info});
        call(REF_AUTORELEASE, {obj});
        return obj;
    }
    static uint32_t construct(uint32_t self, int32_t viewW, int32_t viewH, uint32_t info) {
        call(NODE_CTOR, {self});
        st32(self, ld32(0xFD6784) + 8);
        st32(self + 0x270, 0);
        st32(self + 0x274, 0);
        st32(self + 0x278, ld32(info + 8));
        int32_t w = rw::load<int32_t>(info) + 0xFF;
        w = w + int32_t(uint32_t(w >> 31) >> 24);
        st32(self + 0x27C, uint32_t(w >> 8));
        int32_t h = rw::load<int32_t>(info + 4) + 0xFF;
        h = h + int32_t(uint32_t(h >> 31) >> 24);
        st32(self + 0x280, uint32_t(h >> 8));
        st32(self + 0x284, ld32(info));
        st32(self + 0x288, ld32(info + 4));
        st32(self + 0x294, uint32_t(viewW));
        st32(self + 0x298, uint32_t(viewH));
        st8(self + 0x2A4, 0);
        st32(self + 0x29C, uint32_t(viewW / 2));
        st32(self + 0x2A0, uint32_t(viewH / 2));
        st32(self + 0x28C, uint32_t(w) & ~0xFFu);
        st32(self + 0x290, uint32_t(h) & ~0xFFu);
        call(0x40E7A0, {self + 0x2A8, 0x100, 0});  // __aeabi_memset(cache, 256, 0)
        return self;
    }
    static uint32_t destruct(uint32_t self) {
        st32(self, ld32(0xFD6784) + 8);
        if (ld32(self + 0x270) != 0) call(0x8FD0BD, {ld32(self + 0x270)});  // Ref::release
        call(NODE_DTOR, {self});
        return self;
    }

    // scrolls the view to (x, y) (map pixels, y up): keeps the window inside the map, loads the tiles it needs
    // into the texture and sets the sprite's texture rect
    static uint32_t update(uint32_t self, float x, float y) {
        Frame f;
        int32_t mapH = rw::load<int32_t>(self + 0x288);
        float fh = float(mapH);
        float ty = fh - y;
        float halfH = float(rw::load<int32_t>(self + 0x2A0));
        float top = ty - halfH;
        float bottom = ty + halfH;
        float halfW = float(rw::load<int32_t>(self + 0x29C));
        float left = x - halfW;
        if (left < 0.0f) {
            left = 0.0f;
        } else {
            float right = halfW + x;
            int32_t mapW = rw::load<int32_t>(self + 0x284);
            if (right > float(mapW)) left = float(mapW - rw::load<int32_t>(self + 0x294));
        }
        float topClamped;
        if (top < 0.0f) {
            topClamped = 0.0f;
        } else if (bottom > fh) {
            topClamped = float(mapH - rw::load<int32_t>(self + 0x298));
        } else {
            topClamped = top;
        }
        int32_t cols = rw::load<int32_t>(self + 0x27C);
        int32_t rows = rw::load<int32_t>(self + 0x280);
        int32_t px = f2i(bitsf(call(0x419C54, {fbits(left)})));        // roundf
        int32_t viewH = rw::load<int32_t>(self + 0x298);
        float fViewH = float(viewH);
        int32_t py = f2i(bitsf(call(0x419C54, {fbits(topClamped)})));
        float rx = float(px - floor2048(px));
        float ry = float(py - floor2048(py));
        int32_t viewW = rw::load<int32_t>(self + 0x294);
        uint32_t rect = f.alloc(16);
        call(0x8DF18B, {rect, fbits(rx), fbits(ry), fbits(float(viewW)), fbits(fViewH)});  // Rect::Rect
        int32_t lastRow = div256(viewH + py + 0xFF) + 1;
        if (rows >= lastRow) rows = lastRow;
        int32_t lastCol = div256(viewW + px + 0xFF) + 1;
        if (cols >= lastCol) cols = lastCol;
        int32_t firstRow = div256(py) - 1;
        if (firstRow < 0) firstRow = 0;
        int32_t firstCol = div256(px) - 1;
        if (firstCol < 0) firstCol = 0;
        if (ld8(self + 0x2A4) == 0) {
            st8(self + 0x2A4, 1);
            call(0x5CD549, {self, uint32_t(firstCol), uint32_t(cols), uint32_t(firstRow), uint32_t(rows)});  // load
            uint32_t sprite = call(0x8A62FD, {ld32(self + 0x270), rect, 0});  // Sprite::createWithTexture
            st32(self + 0x274, sprite);
            vcall(self, 0xF0, {self, sprite});  // addChild
        } else {
            bool done = false;
            for (int32_t r = firstRow; !done && r < rows; r++)
                for (int32_t c = firstCol; c < cols; c++)
                    if (call(0x5CD76D, {self, uint32_t(c), uint32_t(r)}) != 0) {  // load(col, row)
                        done = true;
                        break;
                    }
            uint32_t sprite = ld32(self + 0x274);
            vcall(sprite, 0x284, {sprite, rect});  // setTextureRect
        }
        call(VEC2_DTOR, {rect});
        return 0;
    }

    // fills the texture cells of tiles [firstCol, endCol) x [firstRow, endRow) (creating the texture first)
    static uint32_t loadRange(uint32_t self, int32_t firstCol, int32_t endCol, int32_t firstRow, int32_t endRow) {
        Frame f;
        uint32_t buf = call(0xC98F6F, {0x1000000});  // operator new[](2048 * 2048 * 4)
        call(0x40E7A0, {buf, 0x1000000, 0});
        uint32_t tex = ld32(self + 0x270);
        bool created = false;
        if (tex == 0) {
            tex = call(fn::operator_new, {0x40});
            call(0x9149FD, {tex});  // Texture2D::Texture2D
            st32(self + 0x270, tex);
            if (endCol + firstCol < rw::load<int32_t>(self + 0x27C)) endCol--;
            else firstCol++;
            created = true;
        }
        if (firstRow < endRow) {
            int32_t row = firstRow;
            for (;;) {
                if (firstCol < endCol) {
                    int32_t rowMod = mod8(row);
                    for (int32_t c = firstCol; c < endCol; c++) {
                        int32_t cols = rw::load<int32_t>(self + 0x27C);
                        uint32_t idx = uint32_t(cols * row) + uint32_t(c + 1);
                        uint32_t img = call(0x5CD7F5, {self, idx});  // loadImage
                        if (img == 0) break;
                        int32_t colMod = mod8(c);
                        uint32_t w = ld32(img + 0x1C);
                        int32_t h = rw::load<int32_t>(img + 0x20);
                        uint32_t rowBytes = w << 2;
                        if (h >= 1) {
                            uint32_t dst = buf + ((uint32_t(colMod) << 8) + (uint32_t(rowMod) << 19)) * 4;
                            uint32_t src = ld32(img + 0x14);
                            for (int32_t k = h; k != 0; k--) {
                                call(0x40E164, {dst, src, rowBytes});  // memcpy
                                src += rowBytes;
                                dst += 0x2000;
                            }
                        }
                        vcall(img, 4, {img});  // delete the Image
                        st32(self + 0x2A8 + uint32_t(rowMod * 8 + colMod) * 4, idx);
                    }
                }
                if (row == endRow - 1) break;
                row++;
            }
            tex = ld32(self + 0x270);
        }
        if (created) {
            uint32_t size = f.alloc(8);
            uint32_t sz = call(0x8DF027, {size, fbits(2048.0f), fbits(2048.0f)});  // Size::Size
            call(0x914B49, {tex, buf, 0x1000000, 2, 2048, 2048, sz, 1});           // Texture2D::initWithData
            uint32_t params = f.alloc(16);
            st32(params, 0x2600);       // GL_NEAREST
            st32(params + 4, 0x2600);
            st32(params + 8, 0x2901);   // GL_REPEAT
            st32(params + 12, 0x2901);
            call(0x915D29, {ld32(self + 0x270), params});  // setTexParameters
        } else {
            call(0x914F25, {tex, buf, 0, 0, 2048, 2048});  // Texture2D::updateWithData
        }
        call(0xC98FB3, {buf});  // operator delete[]
        return 0;
    }

    // puts tile (col, row) into its texture cell unless it is there already; 1 when it was loaded
    static uint32_t loadTile(uint32_t self, int32_t col, int32_t row) {
        Frame f;
        int32_t colMod = mod8(col), rowMod = mod8(row);
        uint32_t slot = self + 0x2A8 + uint32_t(rowMod * 8 + colMod) * 4;
        uint32_t cur = ld32(slot);
        uint32_t idx = uint32_t(rw::load<int32_t>(self + 0x27C) * row) + uint32_t(col) + 1;
        if (idx == cur) return 0;
        uint32_t img = call(0x5CD7F5, {self, idx});
        call(0x914F25, {ld32(self + 0x270), ld32(img + 0x14), uint32_t(colMod) << 8, uint32_t(rowMod) << 8,
                        ld32(img + 0x1C), ld32(img + 0x20)});  // updateWithData
        if (img != 0) vcall(img, 4, {img});
        st32(slot, idx);
        return 1;
    }

    // the Image of tile `index` (file name from the format), or null when the file does not exist
    static uint32_t loadImage(uint32_t self, uint32_t index) {
        guest::Frame f(0x434);
        const uint32_t name = f.addr(0x2C), tmp = f.addr(0x14), path = f.addr(0x20);
        st32(name + 0x400, 0);
        st8(name, 0);
        call(0x491501, {name, ld32(self + 0x278), index});  // StringBuffer::format(fmt, index)
        uint32_t fu = call(0xA3F44D, {});                     // FileUtils::getInstance
        uint32_t fullPath = ld32(ld32(fu) + 0x1C);
        strInit(tmp, name, host_strlen(name));
        call(fullPath, {path, fu, tmp});
        strFree(tmp);
        uint32_t img = 0;
        if (strSize(path) != 0) {
            img = call(fn::operator_new, {0xC4});
            call(0x8D967D, {img});          // Image::Image
            call(0x8D9721, {img, path});    // Image::initWithImageFile
        }
        strFree(path);
        return img;
    }
};
RW_FUNCTION(0x5CD0B0, "Field::create", Field::create);
RW_FUNCTION(0x5CD100, "Field::Field", Field::construct);
RW_FUNCTION(0x5CD1D0, "Field::~Field", Field::destruct);
RW_FUNCTION(0x5CD258, "Field::update", Field::update);
RW_FUNCTION(0x5CD548, "Field::load(int,int,int,int)", Field::loadRange);
RW_FUNCTION(0x5CD76C, "Field::load(int,int)", Field::loadTile);
RW_FUNCTION(0x5CD7F4, "Field::loadImage", Field::loadImage);

}  // namespace w4b2
