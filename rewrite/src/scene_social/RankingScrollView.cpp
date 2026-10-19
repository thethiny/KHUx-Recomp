// RankingScrollView: the ranking list of SceneRanking (see RankingScrollView.hpp). Rows (createRow / changeRow and
// the layer helpers) are not rewritten yet.
#include "RankingScrollView.hpp"

#include "../scene/SceneUi.hpp"

using namespace scene;
using Self = RankingScrollView;

namespace {
namespace g {
constexpr uint32_t Layout_ctor = 0x828671;                 // cocos2d::ui::Layout::Layout()
constexpr uint32_t aeabi_memset = 0x40E7A0;                // __aeabi_memset(dst, n, c)
constexpr uint32_t Ref_retain = 0x8FD0B5;
constexpr uint32_t Ref_release = 0x8FD0BD;
constexpr uint32_t Ref_autorelease = 0x8FD0D5;
constexpr uint32_t VectorBaseInt_dtor = 0x478961;          // std::__vector_base<int>::~__vector_base
constexpr uint32_t ImageView_setScale9Enabled = 0x839A15;
constexpr uint32_t ArmatureDataManager_getInstance = 0x7AC9B5;
constexpr uint32_t ArmatureDataManager_addArmatureFileInfo = 0x7AD7BD;   // (mgr, const string&)
constexpr uint32_t ExpressionVector_allocate = 0x487635;   // vector<Avatar::Expression>::allocate(n)
constexpr uint32_t ExpressionVector_destroy = 0x46BE0D;    // __vector_base<Avatar::Expression>::~__vector_base
constexpr uint32_t AvatarIconCache_ctor = 0x5CA741;        // (Texture2D*, const Vec2&, float, float, const vector<Expression>&)
constexpr uint32_t AvatarIconCache_dtor = 0x5CA8FD;
constexpr uint32_t Size_copy = 0x8DF02D;
constexpr uint32_t Vec2_ctor = 0x8E4B9D;                   // Vec2(float, float)
constexpr uint32_t Vec2_copy = 0x8E4C03;
constexpr uint32_t Vec2_dtor = 0x8E4C17;
constexpr uint32_t Widget_create = 0x825875;
constexpr uint32_t WidgetVector_clear = 0x485B45;          // cocos2d::Vector<ui::Widget*>::clear
constexpr uint32_t WidgetVector_pushBack = 0x4F6B5D;
constexpr uint32_t DIVSI3 = 0xCD5E8D, MODSI3 = 0xCD6009;
}  // namespace g

// strings
constexpr uint32_t S_MainPanelJson = 0xDFE9B6;     // "cocostudio/publish/Ranking_MainPanel.json"
constexpr uint32_t S_PartyPanelJson = 0xDFE959;    // "cocostudio/publish/Ranking_PartyPanel.json"
constexpr uint32_t S_Avatar = 0xDF8F56;            // "Avatar"
constexpr uint32_t S_CursorAnim2 = 0xDFE9E0;       // "cocostudio/publish/Cursor_Anim_Ranking2.ExportJson"
constexpr uint32_t S_CursorAnim = 0xDFE984;        // "cocostudio/publish/Cursor_Anim_Ranking.ExportJson"

constexpr int32_t TYPE_LUX = 0, TYPE_PARTY = 1;    // anything else: colosseum
constexpr uint32_t AVATAR_ICON_CACHE_SIZE = 0x60;

// virtual slots (byte offsets)
constexpr uint32_t V_deleting = 0x04, V_init = 0x350;
constexpr uint32_t V_getVirtualRenderer = 0x2D8;   // ui::ImageView
constexpr uint32_t V_getTexture = 0x280;           // Sprite
constexpr uint32_t V_widgetSetTouchEnabled = 0x29C;
constexpr uint32_t V_setTouchEnabled = 0x31C;      // the list itself (Layout)

inline int32_t idiv(int32_t a, int32_t b) { return int32_t(call(g::DIVSI3, {uint32_t(a), uint32_t(b)})); }
inline int32_t imod(int32_t a, int32_t b) { return int32_t(call(g::MODSI3, {uint32_t(a), uint32_t(b)})); }
inline int32_t idCount(uint32_t ids) { return int32_t(ld32(ids + 4) - ld32(ids)) >> 2; }

// the row template's avatar icons: an AvatarIconCache over the template's "Avatar" image texture
void createAvatarIcons(Self *self, uint32_t panel) {
    uint32_t avatar = findNode(panel, S_Avatar, 6);
    call(g::ImageView_setScale9Enabled, {avatar, 0});
    uint32_t renderer = vcall(avatar, V_getVirtualRenderer, {avatar});
    uint32_t texture = vcall(renderer, V_getTexture, {renderer});
    uint32_t cache = call(fn::operator_new, {AVATAR_ICON_CACHE_SIZE});
    Frame f;
    uint32_t pos = f.alloc(8);
    call(g::Vec2_ctor, {pos, fbits(-90.0f), fbits(132.0f)});
    // vector<Avatar::Expression>{1}
    uint32_t expression = f.alloc(4);
    uint32_t expressions = f.alloc(12);
    st32(expression, 1);
    st32(expressions, 0);
    st32(expressions + 4, 0);
    st32(expressions + 8, 0);
    call(g::ExpressionVector_allocate, {expressions, 1});
    uint32_t end = ld32(expressions + 4);
    if (end) {
        st32(end, ld32(expression));
        end = ld32(expressions + 4);
    }
    st32(expressions + 4, end + 4);
    call(g::AvatarIconCache_ctor, {cache, texture, pos, fbits(-1.43f), fbits(1.43f), expressions});
    self->avatarIcons = cache;
    call(g::ExpressionVector_destroy, {expressions});
    call(g::Vec2_dtor, {pos});
}
void addArmatureFile(uint32_t path, uint32_t len) {
    uint32_t mgr = call(g::ArmatureDataManager_getInstance, {});
    Frame f;
    uint32_t s = newStr(f, path, len);
    call(g::ArmatureDataManager_addArmatureFileInfo, {mgr, s});
    strFree(s);
}
void deleteAvatarIcons(Self *self) {
    if (self->avatarIcons) {
        call(g::AvatarIconCache_dtor, {self->avatarIcons});
        call(fn::operator_delete, {self->avatarIcons});
    }
    self->avatarIcons = 0;
}
}  // namespace

// ------------------------------------------------------------------------------------------------ life cycle
Self::Addr Self::create(int32_t type) {
    uint32_t p = call(fn::operator_new, {SIZE});
    construct(rw::host_ptr<Self>(p));
    if (vcall(p, V_init, {p, uint32_t(type)}) == 1) {
        call(g::Ref_autorelease, {p});
        return p;
    }
    vcall(p, V_deleting, {p});
    return 0;
}
RW_FUNCTION(0x54F3FC, "RankingScrollView::create(RankingScrollViewType)", RankingScrollView::create);

Self *Self::construct(Self *self) {
    call(g::Layout_ctor, {self->self()});
    initScrollDefaults(self);
    self->itemIDs.begin = 0;
    self->itemIDs.end = 0;
    self->itemIDs.cap = 0;
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    self->vtable3 = VTABLE3;
    rw::set<uint32_t>(self, 0x648, 0);    // the std::function is empty
    self->field_664 = 0;
    self->field_668 = 0;
    self->field_662 = 0;
    self->field_660 = 0;
    self->avatarIcons = 0;
    self->panel = 0;
    self->selectedIds.cap = 0;
    self->selectedIds.end = 0;
    self->selectedIds.begin = 0;
    self->field_66C = 0;
    return self;
}
RW_FUNCTION(0x54F458, "RankingScrollView::RankingScrollView()", RankingScrollView::construct);

Self *Self::destroy(Self *self) {
    self->vtable = VTABLE;
    self->vtable2 = VTABLE2;
    self->vtable3 = VTABLE3;
    if (self->panel) call(g::Ref_release, {self->panel});
    deleteAvatarIcons(self);
    call(g::VectorBaseInt_dtor, {rw::guest_addr(&self->selectedIds)});
    functionDestroy(self->self() + 0x638);
    self->vtable = VTABLE_INT;
    self->vtable2 = VTABLE2_INT;
    self->vtable3 = VTABLE3_INT;
    call(g::VectorBaseInt_dtor, {rw::guest_addr(&self->itemIDs)});
    IScrollProtocol::destroy(self);
    return self;
}
RW_FUNCTION(0x54F260, "RankingScrollView::~RankingScrollView()", RankingScrollView::destroy);

uint32_t Self::init(Self *self, int32_t type) {
    self->viewType = type;
    self->itemWidth = 724.0f;
    IScrollProtocol::setEnabledScrollBar(self, 1);
    changeRankingScrollViewType(self, type);
    return BaseScrollView::initInt(self);
}
RW_FUNCTION(0x54F5D4, "RankingScrollView::init(RankingScrollViewType)", RankingScrollView::init);

// The row template of the type: Lux and colosseum rows are 120 high with the avatar icon, party rows 110.
void Self::changeRankingScrollViewType(Self *self, int32_t type) {
    if (self->panel) call(g::Ref_release, {self->panel});
    deleteAvatarIcons(self);
    self->viewType = type;
    if (type == TYPE_PARTY) {
        self->itemHeight = 110.0f;
        self->offsetX = float(self->itemWidth) * 0.5f;
        self->offsetY = -110.0f;
        self->panel = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_PartyPanelJson});
        addArmatureFile(S_CursorAnim, 49);
    } else {
        self->itemHeight = 120.0f;
        self->offsetX = float(self->itemWidth) * 0.5f;
        self->offsetY = type == TYPE_LUX ? -100.0f : -65.0f;
        uint32_t panel = call(fn::GUIReader_widgetFromJsonFile, {call(fn::GUIReader_getInstance, {}), S_MainPanelJson});
        self->panel = panel;
        createAvatarIcons(self, panel);
        addArmatureFile(S_CursorAnim2, 50);
    }
    call(g::Ref_retain, {self->panel});
    vcall(self->panel, V_widgetSetTouchEnabled, {self->panel, 0});
}
RW_FUNCTION(0x54F614, "RankingScrollView::changeRankingScrollViewType(RankingScrollViewType)",
            RankingScrollView::changeRankingScrollViewType);

// ------------------------------------------------------------------------------------------------ data
// A new season rebuilds the row pool (like IScrollProtocol::setContentSize, with the current size); then the view
// is sized for the entries: up to three it is shrunk to their height and does not scroll, more fill 518 pixels.
void Self::setRankingData(Self *self, int32_t season, Addr ids) {
    uint32_t me = self->self();
    if (self->season != season) {
        Frame f;
        uint32_t size = f.alloc(8);
        call(g::Size_copy, {size, vcall(me, node::getContentSize, {me})});
        float pixel = IScrollProtocol::getItemPixel(self);
        float view = ldf(self->scrollType ? size : size + 4);
        if (int32_t(uint32_t(self->items.end) - uint32_t(self->items.begin)) >= 1) {
            int32_t k = 0;
            do {
                uint32_t w = ld32(self->items.begin + 4 * uint32_t(k));
                if (vcall(w, node::getParent, {w})) vcall(me, node::removeChild, {me, w, 1});
                k++;
            } while (k < (int32_t(uint32_t(self->items.end) - uint32_t(self->items.begin)) >> 2));
        }
        call(g::WidgetVector_clear, {me + 0x51C});
        call(g::WidgetVector_clear, {me + 0x5A8});
        call(g::WidgetVector_clear, {me + 0x5B4});
        float rows = self->rowCount;
        if (!(rows <= 0.0f) && rows == rows) {
            float half = pixel * 0.5f;
            uint32_t v = f.alloc(8);
            int32_t k = 0;
            do {
                uint32_t w = call(g::Widget_create, {});
                int32_t columns = self->maxColumn;
                int32_t col = imod(k - 1, columns) + 1;
                int32_t row = idiv(k - 1, columns);
                float x, y;
                if (self->scrollType) {
                    float h = self->itemHeight;
                    y = float(col) * h + h * 0.5f;
                    x = pixel * float(row);
                    if (columns == 1) y = bitsf(vcall(w, node::getPositionY, {w})) + self->offsetY;
                } else {
                    float wd = self->itemWidth;
                    float t = float(col) * wd + wd * -0.5f;
                    float xa = self->offsetX + t;
                    y = ((view - pixel * float(row)) - half) + self->offsetY;
                    if (columns == 1) x = bitsf(vcall(w, node::getPositionX, {w})) + self->offsetX;
                    else x = xa;
                }
                call(g::Vec2_ctor, {v, fbits(x), fbits(y)});
                vcall(w, node::setPosition, {w, v});
                call(g::Vec2_dtor, {v});
                call(g::WidgetVector_pushBack, {me + 0x51C, w});
                vcall(me, IScrollProtocol::V_registItemTouchEvent, {me, w, uint32_t(k)});
                k++;
            } while (float(k) < self->rowCount);
        }
    }
    self->season = season;
    uint32_t count = uint32_t(idCount(ids));
    Frame f;
    uint32_t size = f.alloc(8);
    uint32_t pos = f.alloc(8);
    if (count <= 3) {
        vcall(me, V_setTouchEnabled, {me, 0});
        float lift;
        int32_t type = self->viewType;
        if (type == TYPE_LUX) {
            self->maxOffset = 20.0f;
            lift = 81.0f;
        } else if (type == TYPE_PARTY) {
            self->maxOffset = 0.0f;
            lift = 30.0f;
        } else {
            self->maxOffset = 0.0f;
            lift = 81.0f;
        }
        call(g::Size_copy, {size, vcall(me, node::getContentSize, {me})});
        stf(size + 4, float(uint32_t(idCount(ids) + 1)) * self->itemHeight);
        vcall(me, node::setContentSize, {me, size});
        call(g::Vec2_copy, {pos, vcall(me, node::getPosition, {me})});
        stf(pos + 4, float(uint32_t(4 - idCount(ids))) * self->itemHeight - lift);
        vcall(me, node::setPosition, {me, pos});
        call(g::Vec2_dtor, {pos});
    } else {
        vcall(me, V_setTouchEnabled, {me, 1});
        int32_t type = self->viewType;
        self->maxOffset = type == TYPE_LUX ? 180.0f : type == TYPE_PARTY ? 195.0f : 140.0f;
        call(g::Size_copy, {size, vcall(me, node::getContentSize, {me})});
        stf(size + 4, 518.0f);
        vcall(me, node::setContentSize, {me, size});
        call(g::Vec2_copy, {pos, vcall(me, node::getPosition, {me})});
        stf(pos + 4, 0.0f);
        vcall(me, node::setPosition, {me, pos});
        call(g::Vec2_dtor, {pos});
    }
    BaseScrollView::setItemIDsInt(self, ids);
}
RW_FUNCTION(0x5529AC, "RankingScrollView::setRankingData(RankingSeason, std::vector<int>)",
            RankingScrollView::setRankingData);
