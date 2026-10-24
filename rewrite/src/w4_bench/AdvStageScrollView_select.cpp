// AdvStageScrollView: stage type, centre selection, row touch lambdas, the party icons area and the "new stage" /
// raid boss marks.
#include "AdvStageScrollView.hpp"

using namespace scene;

namespace {
namespace lib {
constexpr uint32_t APIManager_getInstance = 0x4BA8C9;
constexpr uint32_t SceneManager_getInstance = 0x60C26D;
constexpr uint32_t SceneUtil_getNexTutorialphase = 0x472B31;
constexpr uint32_t SceneUtil_isLoggedInFacebook = 0x4697E9;
constexpr uint32_t BaseStageScrollView_getItemIDs = 0x59D341;    // (vector<int> out, view)
constexpr uint32_t BaseStageScrollView_getMinPosition = 0x59CB15;
constexpr uint32_t BaseStageScrollView_getItemPixel = 0x59CDDD;
constexpr uint32_t BaseStageScrollView_migrationRight = 0x59DAB3;
constexpr uint32_t BaseStageScrollView_migrationBounceCenterPosition = 0x59CE05;
constexpr uint32_t IntVector_dtor = 0x478961;
constexpr uint32_t WidgetVector_pushBackSlow = 0x4F8405;         // vector<ui::Widget*>::__push_back_slow_path
constexpr uint32_t FunctionVI_call = 0x4F321D;                   // std::function<void(int)>::operator()
constexpr uint32_t divsi3 = 0xCD5E8D;                            // __divsi3
constexpr uint32_t THROW_OUT_OF_RANGE = 0x47CCE1;                // vector::at failure (never returns)
constexpr uint32_t Touch_getLocation = rwlib::cocos2d::Touch::getLocation;                 // (Vec2 out, touch)
constexpr uint32_t Node_convertToNodeSpace = rwlib::cocos2d::Node::convertToNodeSpace;           // (Vec2 out, node, const Vec2&)
constexpr uint32_t Node_runAction = rwlib::cocos2d::Node::runAction;
constexpr uint32_t Vec2_ctor = rwlib::cocos2d::Vec2::ctor_base__ff;                         // Vec2::Vec2(float, float)
constexpr uint32_t Vec2_dtor = rwlib::cocos2d::Vec2::dtor_base;
constexpr uint32_t Rect_ctor = rwlib::cocos2d::Rect::ctor_base__ffff;                         // Rect::Rect(float x, y, w, h)
constexpr uint32_t Rect_containsPoint = rwlib::cocos2d::Rect::containsPoint;
constexpr uint32_t FadeTo_create = rwlib::cocos2d::FadeTo::create;                     // (float duration, uint8 opacity)
constexpr uint32_t Sequence_create = rwlib::cocos2d::Sequence::create;                   // (action, ..., nullptr)
constexpr uint32_t LWFNode_create = rwlib::cocos2d::LWFNode::create;                    // (const char* path, void*)
constexpr uint32_t GUIReader_getInstance = rwlib::cocostudio::GUIReader::getInstance;
constexpr uint32_t GUIReader_widgetFromJsonFile = rwlib::cocostudio::GUIReader::widgetFromJsonFile;
constexpr uint32_t Date_calculateIntervalSinceNow = 0x4922D5;
constexpr uint32_t EvStage_create = 0x782D2D;                    // master::EvStage::create(shared_ptr out, id)
constexpr uint32_t audio_Manager_getInstance = 0x5BECFD;
constexpr uint32_t audio_Manager_getLayer = 0x5BF33D;            // (shared_ptr<Layer> out, manager, int)
constexpr uint32_t audio_Layer_playSE = 0x5BF785;                // (layer, ResourceID, int, int)
constexpr uint32_t ImageDownloader_loadImage = 0x5BD0CB;         // (downloader, ImageView*, url, name)
constexpr uint32_t setStoryStageFrame = 0x568C89;                // AdvStageScrollView statics (widget, index)
constexpr uint32_t setEventStageFrame = 0x5698AD;
constexpr uint32_t setColosseumStageFrame = 0x569EAD;
constexpr uint32_t settingAvatarFrame = 0x56FC1D;                // (view, widget, index, shared_ptr*, unsigned)
}  // namespace lib

// globals
constexpr uint32_t VEC2_ANCHOR_BOTTOM_LEFT = 0x1006958;          // cocos2d::Vec2::ANCHOR_BOTTOM_LEFT
constexpr uint32_t SCENEUTIL_IN_OUT_TIME = 0xDEC3D0;             // SceneUtil::IN_OUT_TIME (float)
constexpr uint32_t FLICK_MIN_DISTANCE = 0xE4099C;                // BaseStageScrollView::FLICK_MIN_DISTANCE (float)

// strings
constexpr uint32_t S_BALL = 0xDFF3DD;                            // "Ball"
constexpr uint32_t S_FACEBOOK_PHOTO_PANEL = 0xDF8E5A;            // "FaceBook_PhotoPanel"
constexpr uint32_t S_BOSSBTN_LWF = 0xDFF7B3;                     // "lwf/raid/bossbtn/bossbtn.lwf"
constexpr uint32_t S_MINIG_LWF = 0xDFF795;                       // "lwf/adventure/minig/minig.lwf"
constexpr uint32_t JSON_BALL2 = 0xDFF7D0;                        // "cocostudio/publish/AdventureTop2_Ball2.json"
constexpr uint32_t S_AVATAR = 0xDF8F56;                          // "Avatar"
constexpr uint32_t S_PARTY_ICON = 0xDFF7FC;                      // "PartyIcon"

constexpr uint32_t SE_ROW_TAP = 0x1E56B9EA;                      // audio::ResourceID played on a row tap

// APIManager members
constexpr uint32_t API_USER = 132;              // SafeBuffer* user data (+8: 64-bit user id), shared_ptr ctrl at 136
constexpr uint32_t API_CURRENT_STAGE = 292;     // SafeBuffer* (+12: current stage id)
constexpr uint32_t API_EVENTS = 300;            // std::vector<Event (56 bytes)>*
constexpr uint32_t API_RAID = 332;              // raid info (+16 Date end, +52 count, +240 boss)
constexpr uint32_t API_PARTY = 508;             // shared_ptr<PartyMemberList> (+512 ctrl)
constexpr uint32_t API_FB_FRIENDS = 812;        // shared_ptr<PartyMemberList> of Facebook friends (+816 ctrl)
// SceneManager members
constexpr uint32_t SM_SCENE_KIND = 652;
constexpr uint32_t SM_EVENT_ID = 752;

// virtual slots (byte offsets)
constexpr uint32_t VS_SET_ENABLED = 0x29C;      // ui::Widget::setEnabled / setTouchEnabled(bool) (slot 167)
constexpr uint32_t VS_NODE_10 = 0x10;           // Node slot 4 (z order setter) called with the view's z + 1
constexpr uint32_t VS_NODE_GET_LOCAL_Z = 0x1C;  // Node slot 7
constexpr uint32_t VS_WIDGET_ADD_CHILD_NAME = 0xFC;   // addChild(child, z, const std::string& name)

void setVisible(uint32_t node, uint32_t visible) { vcall(node, node::setVisible, {node, visible}); }

AdvStageScrollView *view(uint32_t a) { return rw::host_ptr<AdvStageScrollView>(a); }

float floatAt(uint32_t addr) { return rw::load<float>(addr); }

// (int)(pixel * 0.5 + (pos - minPos)) / (int)pixel: the item index at the centre (with __divsi3)
int32_t centerIndex(float pos, float minPos, float pixel) {
    float t = pos - minPos;
    float v = pixel * 0.5f + t;
    return int32_t(call(lib::divsi3, {uint32_t(f2i(v)), uint32_t(f2i(pixel))}));
}

// a 64-bit value of a SafeBuffer is non-zero (SafeBuffer::read of 8 bytes)
bool safe64NonZero(uint32_t sb, uint32_t index) { return (uint32_t(safe32(sb, index)) | uint32_t(safe32(sb, index + 4))) != 0; }

// Rect(widget x, y, width, height) into `rect`
void widgetRect(uint32_t rect, uint32_t w) {
    uint32_t x = vcall(w, node::getPositionX, {w});
    uint32_t y = vcall(w, node::getPositionY, {w});
    uint32_t width = ld32(vcall(w, node::getContentSize, {w}));
    uint32_t height = ld32(vcall(w, node::getContentSize, {w}) + 4);
    call(lib::Rect_ctor, {rect, x, y, width, height});
}
}  // namespace

// ------------------------------------------------------------------------------------------------ stage type
void AdvStageScrollView::setStageType(AdvStageScrollView *self, int32_t type) {
    self->stageType = type;
    if (type == 0) return;
    if (int32_t(self->widgets.end - self->widgets.begin) < 1) return;
    int32_t k = 0;
    do {
        uint32_t ball = findNode(ld32(self->widgets.begin + uint32_t(k) * 4), S_BALL, 4);
        if (ball) setVisible(ball, 0);
        uint32_t photo = findNode(ld32(self->widgets.begin + uint32_t(k) * 4), S_FACEBOOK_PHOTO_PANEL, 0x13);
        if (photo) setVisible(photo, 0);
        k++;
    } while (k < (int32_t(self->widgets.end - self->widgets.begin) >> 2));
}

// ------------------------------------------------------------------------------------------------ selection
// The y of the visible row whose item is the (first) selected one.
uint32_t AdvStageScrollView::getSelestItemPositionY(AdvStageScrollView *self, Addr outY) {
    uint32_t me = rw::guest_addr(self);
    Frame f;
    uint32_t ids = f.alloc(12);
    call(lib::BaseStageScrollView_getItemIDs, {ids, me});
    uint32_t result = 0;
    uint32_t begin = ld32(ids), end = ld32(ids + 4);
    float rows = rw::get<float>(self, 1436);            // visible rows
    if (end != begin && rows > 0.0f) {                  // fcmple / unordered: not (rows <= 0 or NaN)
        int32_t first = rw::get<int32_t>(self, 1476) - 1;
        uint32_t base = begin + uint32_t(first) * 4;
        uint32_t n = uint32_t(int32_t(end - begin) >> 2);
        int32_t k = 0;
        for (;;) {
            if (n > uint32_t(first + k)) {
                int32_t selected = -1;
                if (self->selectedItems.end != self->selectedItems.begin) selected = int32_t(ld32(self->selectedItems.begin));
                if (uint32_t(selected) == ld32(base + uint32_t(k) * 4)) {
                    uint32_t row = ld32(self->widgets.begin + uint32_t(k) * 4);
                    rw::save<uint32_t>(outY, vcall(row, node::getPositionY, {row}));
                    result = 1;
                    break;
                }
            }
            k++;
            if (!(float(k) < rows)) break;
        }
    }
    call(lib::IntVector_dtor, {ids});
    return result;
}

// Reports the item at the centre of the list to the selection callback.
void AdvStageScrollView::selectCenterStage(AdvStageScrollView *self) {
    uint32_t me = rw::guest_addr(self);
    if (self->itemIDs.end == self->itemIDs.begin) return;
    float minPos = rw::bitsf(call(lib::BaseStageScrollView_getMinPosition, {me}));
    float pixel = rw::bitsf(call(lib::BaseStageScrollView_getItemPixel, {me}));
    int32_t index = centerIndex(rw::get<float>(self, 1472), minPos, pixel);
    uint32_t begin = self->itemIDs.begin;
    int32_t last = (int32_t(self->itemIDs.end - begin) >> 2) - 1;
    if (index >= last) index = last;
    if (!self->onSelect_f) return;
    if (index < 0) index = 0;
    call(lib::FunctionVI_call, {me + ON_SELECT, ld32(begin + uint32_t(index) * 4)});
}

// ------------------------------------------------------------------------------------------------ rows
void AdvStageScrollView::setAdvemtureStageFrame(AdvStageScrollView *self, Addr widget, int32_t index) {
    vcall(widget, VS_SET_ENABLED, {widget, 0});
    int32_t type = self->stageType;
    if (type == 2) {
        call(lib::setColosseumStageFrame, {widget, uint32_t(index)});
        return;
    }
    if (type == 1) {
        call(lib::setEventStageFrame, {widget, uint32_t(index)});
        return;
    }
    if (type != 0) return;
    call(lib::setStoryStageFrame, {widget, uint32_t(index)});
    uint32_t me = rw::guest_addr(self);
    uint32_t api = call(lib::APIManager_getInstance, {});
    Frame f;
    // settingAvatarFrame's last argument is whatever the original left at [sp]: the 8 it passed to SafeBuffer::read
    constexpr uint32_t STALE_ARG = 8;
    uint32_t shown = 0;
    if (safe64NonZero(ld32(api + API_USER), 8)) {
        api = call(lib::APIManager_getInstance, {});
        uint32_t party = f.alloc(8);          // shared_ptr<PartyMemberList> passed by value
        st32(party, ld32(api + API_PARTY));
        uint32_t ctrl = ld32(api + API_PARTY + 4);
        st32(party + 4, ctrl);
        if (ctrl) call(guest::SHARED_ADD_REF, {ctrl});
        shown = call(lib::settingAvatarFrame, {me, widget, uint32_t(index), party, STALE_ARG});
        if (ctrl) call(guest::SHARED_RELEASE, {ctrl});
        if (shown) return;
    }
    if (call(lib::SceneUtil_isLoggedInFacebook, {}) != 1) return;
    api = call(lib::APIManager_getInstance, {});
    uint32_t friends = f.alloc(8);
    st32(friends, ld32(api + API_FB_FRIENDS));
    uint32_t ctrl = ld32(api + API_FB_FRIENDS + 4);
    st32(friends + 4, ctrl);
    if (ctrl) call(guest::SHARED_ADD_REF, {ctrl});
    call(lib::settingAvatarFrame, {me, widget, uint32_t(index), friends, STALE_ARG});
    if (ctrl) call(guest::SHARED_RELEASE, {ctrl});
}

// The party icons panel (AdventureTop2_Ball2.json) added to `widget` when the user has a party.
void AdvStageScrollView::setPartyIconsArea(AdvStageScrollView *self, Addr widget) {
    if (!widget) return;
    uint32_t api = call(lib::APIManager_getInstance, {});
    uint32_t ctrl = ld32(api + API_USER + 4);
    uint32_t user = ld32(api + API_USER);
    if (ctrl) call(guest::SHARED_ADD_REF, {ctrl});
    if (safe64NonZero(user, 8)) {
        self->partyIconsShown = 1;
        uint32_t panel = call(lib::GUIReader_widgetFromJsonFile, {call(lib::GUIReader_getInstance, {}), JSON_BALL2});
        Frame f;
        uint32_t pPanel = f.alloc(4);
        st32(pPanel, panel);
        uint32_t avatar = findNode(panel, S_AVATAR, 6);
        vcall(panel, VS_SET_ENABLED, {panel, 1});
        vcall(avatar, VS_SET_ENABLED, {avatar, 1});
        uint32_t addChild = ld32(ld32(widget) + VS_WIDGET_ADD_CHILD_NAME);
        uint32_t name = newStr(f, S_PARTY_ICON, 9);
        call(addChild, {widget, panel, 0, name});
        strFree(name);
        uint32_t end = self->rowWidgets.end;
        if (end == self->rowWidgets.cap) {
            call(lib::WidgetVector_pushBackSlow, {rw::guest_addr(self) + 1704, pPanel});
        } else {
            uint32_t e = 0;
            if (end) {
                st32(end, panel);
                e = self->rowWidgets.end;
            }
            self->rowWidgets.end = e + 4;
        }
    }
    if (ctrl) call(guest::SHARED_RELEASE, {ctrl});
}

// ------------------------------------------------------------------------------------------------ marks
// The "new stage" mark (minig.lwf) for the current stage or, on the event screens, the current event's newest
// stage. 1 when shown.
uint32_t AdvStageScrollView::initNewStage(AdvStageScrollView *self, Addr) {
    if (self->raidBossShown) return 0;
    uint32_t me = rw::guest_addr(self);
    uint32_t stageId = ld32(ld32(call(lib::APIManager_getInstance, {}) + API_CURRENT_STAGE) + 12);
    self->eventNewStage = 0;
    uint32_t kind = ld32(call(lib::SceneManager_getInstance, {}) + SM_SCENE_KIND);
    if (kind - 2 <= 2) {
        // event screens: the event shown by the scene manager
        if (!ld32(call(lib::SceneManager_getInstance, {}) + SM_EVENT_ID)) return 0;
        uint32_t events = ld32(call(lib::APIManager_getInstance, {}) + API_EVENTS);
        if (ld32(events + 4) == ld32(events)) return 0;
        uint32_t off = 0, k = 0;
        for (;;) {
            uint32_t id = ld32(ld32(events) + off + 12);
            if (id == ld32(call(lib::SceneManager_getInstance, {}) + SM_EVENT_ID)) break;
            uint32_t n = (uint32_t(int32_t(ld32(events + 4) - ld32(events)) >> 3)) * 0xB6DB6DB7u;   // / 56
            off += 56;
            k++;
            if (!(k < n)) return 0;
        }
        uint32_t n = (uint32_t(int32_t(ld32(events + 4) - ld32(events)) >> 3)) * 0xB6DB6DB7u;
        if (n <= k) call(lib::THROW_OUT_OF_RANGE, {events});
        uint32_t ev = ld32(events) + off;
        Frame f;
        uint32_t row = masterRow(f, lib::EvStage_create, ld32(ev + 12));
        if (ld32(row)) {
            if (safe32(ld32(row), 40) == 3) {
                stageId = ld32(ev + 48);
                self->eventNewStage = 1;
                // the last of the event's 24-byte stage records
                uint32_t sb = ld32(ev), se = ld32(ev + 4);
                uint32_t count = (uint32_t(int32_t(se - sb) >> 3)) * 0xAAAAAAABu;   // / 24
                uint32_t last = count - 1;
                if (!(last < count)) call(lib::THROW_OUT_OF_RANGE, {ev});
                if (ld32(sb + 24 * last) == stageId) {
                    stageId = 0;
                    self->eventNewStage = 0;
                }
            }
        }
        sharedRelease(row);
    }
    if (!stageId) return 0;
    rw::set<uint8_t>(self, 1441, 1);
    rw::set<uint8_t>(self, 1440, 1);
    if (!self->newStageLwf) self->newStageLwf = call(lib::LWFNode_create, {S_MINIG_LWF, 0});
    vcall(me, node::addChild, {me, self->newStageLwf});
    uint32_t lwf = self->newStageLwf;
    uint32_t setZ = ld32(ld32(lwf) + VS_NODE_10);
    uint32_t z = vcall(me, VS_NODE_GET_LOCAL_Z, {me});
    call(setZ, {lwf, z + 1});
    float pixel = rw::bitsf(call(lib::BaseStageScrollView_getItemPixel, {me}));
    uint32_t node = self->newStageLwf;
    uint32_t setPosition = ld32(ld32(node) + node::setPosition);
    float width = floatAt(vcall(me, node::getContentSize, {me}));
    uint32_t size = vcall(me, node::getContentSize, {me});
    float x = pixel + (width * 0.5f + -46.0f);
    float y = floatAt(size + 4) * 0.5f + -64.0f;
    Frame f;
    uint32_t pos = f.alloc(8);
    call(lib::Vec2_ctor, {pos, rw::fbits(x), rw::fbits(y)});
    call(setPosition, {node, pos});
    call(lib::Vec2_dtor, {pos});
    return 1;
}

// The raid boss button (bossbtn.lwf) when a raid boss is up. 1 when there is a raid boss.
uint32_t AdvStageScrollView::initRaidBoss(AdvStageScrollView *self) {
    uint32_t raid = ld32(call(lib::APIManager_getInstance, {}) + API_RAID);
    if (!raid || !ld32(raid + 240)) return 0;
    if (int32_t(ld32(raid + 52)) < 1) return 1;
    self->raidBossShown = 1;
    if (self->raidBossLwf) return 1;
    uint32_t me = rw::guest_addr(self);
    uint32_t lwf = call(lib::LWFNode_create, {S_BOSSBTN_LWF, 0});
    self->raidBossLwf = lwf;
    vcall(me, node::addChild, {me, lwf});
    uint32_t node = self->raidBossLwf;
    vcall(node, node::setAnchorPoint, {node, VEC2_ANCHOR_BOTTOM_LEFT});
    return 1;
}

// Hides the raid boss button once the raid is over.
void AdvStageScrollView::CheckTimeOverRaidBoss(AdvStageScrollView *self) {
    if (!self->raidBossShown) return;
    uint32_t raid = ld32(call(lib::APIManager_getInstance, {}) + API_RAID);
    if (int32_t(call(lib::Date_calculateIntervalSinceNow, {raid + 16})) > 0) return;
    self->raidBossShown = 0;
    setVisible(self->raidBossLwf, 0);
}

// ------------------------------------------------------------------------------------------------ lambdas
// registItemTouchEvent $_18 (onTouchBegan; captures view, row index): starts a row tap when the touch is inside
// the row.
uint32_t AdvStageScrollView::onTouchBegan(Addr fn, Addr touch, Addr) {
    uint32_t me = ld32(fn + 4);
    AdvStageScrollView *self = view(me);
    if (rw::get<uint8_t>(self, 1440)) return 0;
    if (rw::get<uint8_t>(self, 1504)) return 0;
    uint32_t t = ld32(touch);
    if (int32_t(call(lib::SceneUtil_getNexTutorialphase, {})) < 995) return 0;
    int32_t index = int32_t(ld32(fn + 8));
    uint32_t row = ld32(self->widgets.begin + uint32_t(index) * 4);
    Frame f;
    uint32_t rect = f.alloc(16);
    widgetRect(rect, row);
    uint32_t loc = f.alloc(8);
    call(lib::Touch_getLocation, {loc, t});
    uint32_t p = f.alloc(8);
    call(lib::Node_convertToNodeSpace, {p, me, loc});
    call(lib::Vec2_dtor, {loc});
    uint32_t result = 0;
    if (call(lib::Rect_containsPoint, {rect, p}) == 1) {
        self->field_678 = 0;
        float minPos = rw::bitsf(call(lib::BaseStageScrollView_getMinPosition, {me}));
        float pixel = rw::bitsf(call(lib::BaseStageScrollView_getItemPixel, {me}));
        float pos = rw::get<float>(self, 1472);
        int32_t q = centerIndex(pos, minPos, pixel);
        int32_t centre = pos < 0.0f ? q + 1 : q;
        int32_t rows = f2i(rw::get<float>(self, 1436));
        int32_t item = int32_t(uint32_t(centre) + uint32_t(index) - uint32_t(rows / 2));
        int32_t n = int32_t(self->itemIDs.end - self->itemIDs.begin) >> 2;
        if (item == n - 1) {
            self->touchingLast = 1;
            self->field_694 = 0;
        }
        if (item >= 0 && uint32_t(item) < uint32_t(n)) {
            self->touchedIndex = item;
            result = 1;
            self->touching = 1;
        }
    }
    call(lib::Vec2_dtor, {p});
    call(lib::Vec2_dtor, {rect});
    return result;
}

// registItemTouchEvent $_19 (onTouchMoved; captures view, row widget): the tap is cancelled when the touch
// leaves the row.
void AdvStageScrollView::onTouchMoved(Addr fn, Addr touch, Addr) {
    uint32_t t = ld32(touch);
    uint32_t me = ld32(fn + 4);
    uint32_t parent = vcall(me, node::getParent, {me});
    Frame f;
    uint32_t loc = f.alloc(8);
    call(lib::Touch_getLocation, {loc, t});
    uint32_t p = f.alloc(8);
    call(lib::Node_convertToNodeSpace, {p, parent, loc});
    call(lib::Vec2_dtor, {loc});
    uint32_t rect = f.alloc(16);
    widgetRect(rect, ld32(fn + 8));
    if (!call(lib::Rect_containsPoint, {rect, p})) {
        AdvStageScrollView *self = view(me);
        self->touching = 0;
        self->touchingLast = 0;
    }
    call(lib::Vec2_dtor, {rect});
    call(lib::Vec2_dtor, {p});
}

// registItemTouchEvent $_20 (onTouchEnded / onTouchCancelled; captures view): a tap that moved less than 30
// points scrolls the tapped row to the centre (with the tap sound when it moved less than FLICK_MIN_DISTANCE).
void AdvStageScrollView::onTouchEnded(Addr fn, Addr touch, Addr) {
    uint32_t me = ld32(fn + 4);
    AdvStageScrollView *self = view(me);
    if (rw::get<uint8_t>(self, 1440)) return;
    uint32_t t = ld32(touch);
    if (int32_t(call(lib::SceneUtil_getNexTutorialphase, {})) < 995) return;
    if (self->onSelect_f && self->touching && !self->field_691) {
        Frame f;
        uint32_t loc = f.alloc(8);
        call(lib::Touch_getLocation, {loc, t});
        float startX = rw::get<float>(self, 1484);
        float x = floatAt(loc);
        call(lib::Vec2_dtor, {loc});
        uint32_t loc2 = f.alloc(8);
        call(lib::Touch_getLocation, {loc2, t});
        float startY = rw::get<float>(self, 1480);
        float y = floatAt(loc2 + 4);
        call(lib::Vec2_dtor, {loc2});
        float dx = std::fabs(x - startX);
        if (dx <= 30.0f) {                      // not (dx > 30 or NaN)
            float dy = std::fabs(y - startY);
            if (dy <= 30.0f) {
                float flick = floatAt(FLICK_MIN_DISTANCE);
                if (flick > dx && flick > dy) {     // fcmple / unordered both false

                    uint32_t layer = f.alloc(8);
                    call(lib::audio_Manager_getLayer, {layer, call(lib::audio_Manager_getInstance, {}), 0});
                    call(lib::audio_Layer_playSE, {ld32(layer), SE_ROW_TAP, 0, 0});
                    sharedRelease(layer);
                }
                float minPos = rw::bitsf(call(lib::BaseStageScrollView_getMinPosition, {me}));
                float pixel = rw::bitsf(call(lib::BaseStageScrollView_getItemPixel, {me}));
                int32_t q = centerIndex(rw::get<float>(self, 1472), minPos, pixel);
                call(lib::BaseStageScrollView_migrationBounceCenterPosition,
                     {me, uint32_t(self->touchedIndex) - uint32_t(q)});
            }
        }
    }
    self->touching = 0;
    self->field_691 = 0;
    self->touchingLast = 0;
}

// updateScroll $_22 (captures view): fades the raid boss button in and scrolls right by one.
void AdvStageScrollView::raidBossFadeIn(Addr fn) {
    uint32_t me = ld32(fn + 4);
    uint32_t lwf = view(me)->raidBossLwf;
    if (lwf) {
        uint32_t fade = call(lib::FadeTo_create, {ld32(SCENEUTIL_IN_OUT_TIME), 0xFF});
        uint32_t seq = call(lib::Sequence_create, {fade, 0});
        call(lib::Node_runAction, {lwf, seq});
    }
    call(lib::BaseStageScrollView_migrationRight, {me, 1});
}

// settingAvatarFrame $_17 (captures view, ImageView*, std::string name at +12): the profile image URL arrived.
void AdvStageScrollView::avatarImageLoaded(Addr fn, Addr ok, Addr url) {
    if (!ld8(ok)) return;
    uint32_t u = ld32(url);
    if (!u) return;
    uint32_t me = ld32(fn + 4);
    uint32_t image = ld32(fn + 8);
    uint32_t downloader = view(me)->imageDownloader;
    call(lib::ImageDownloader_loadImage, {downloader, image, u, strData(fn + 12)});
}

RW_FUNCTION(0x57186C, "AdvStageScrollView::setStageType", AdvStageScrollView::setStageType);
RW_FUNCTION(0x570AB0, "AdvStageScrollView::getSelestItemPositionY", AdvStageScrollView::getSelestItemPositionY);
RW_FUNCTION(0x571A04, "AdvStageScrollView::selectCenterStage", AdvStageScrollView::selectCenterStage);
RW_FUNCTION(0x56FAB4, "AdvStageScrollView::setAdvemtureStageFrame", AdvStageScrollView::setAdvemtureStageFrame);
RW_FUNCTION(0x570F04, "AdvStageScrollView::setPartyIconsArea", AdvStageScrollView::setPartyIconsArea);
RW_FUNCTION(0x570BDC, "AdvStageScrollView::initNewStage", AdvStageScrollView::initNewStage);
RW_FUNCTION(0x570E7C, "AdvStageScrollView::initRaidBoss", AdvStageScrollView::initRaidBoss);
RW_FUNCTION(0x5719C8, "AdvStageScrollView::CheckTimeOverRaidBoss", AdvStageScrollView::CheckTimeOverRaidBoss);
RW_FUNCTION(0x5721CC, "AdvStageScrollView::registItemTouchEvent::$_18 operator()", AdvStageScrollView::onTouchBegan);
RW_FUNCTION(0x57201C, "AdvStageScrollView::registItemTouchEvent::$_19 operator()", AdvStageScrollView::onTouchMoved);
RW_FUNCTION(0x571D40, "AdvStageScrollView::registItemTouchEvent::$_20 operator()", AdvStageScrollView::onTouchEnded);
RW_FUNCTION(0x571BC0, "AdvStageScrollView::updateScroll::$_22 operator()", AdvStageScrollView::raidBossFadeIn);
RW_FUNCTION(0x57255C, "AdvStageScrollView::settingAvatarFrame::$_17 operator()", AdvStageScrollView::avatarImageLoaded);

// ------------------------------------------------------------------------------------------------ updateScroll
namespace {
namespace lib2 {
constexpr uint32_t BaseStageScrollView_updateScroll = 0x59D66D;
constexpr uint32_t BaseStageScrollView_checkScrollInterval = 0x59D9F1;   // (view, float& position)
constexpr uint32_t DelayTime_create = rwlib::cocos2d::DelayTime::create;
constexpr uint32_t CallFunc_create = rwlib::cocos2d::CallFunc::create;                           // (const std::function<void()>&)
constexpr uint32_t Vec2_copy = rwlib::cocos2d::Vec2::ctor_base__RKS0_;
constexpr uint32_t Vec2_add = rwlib::cocos2d::Vec2::add;
}  // namespace lib2
constexpr uint32_t VT_updateScroll_21 = 0xF59304;   // (row widget): shows the row again
constexpr uint32_t VT_updateScroll_22 = 0xF592D8;   // (view): raidBossFadeIn
constexpr uint32_t VT_updateScroll_23 = 0xF592AC;   // (view)
constexpr uint32_t S_RAID_AREA = 0xDFF806;          // "Raid_Area"
constexpr uint32_t VS_SET_OPACITY = 0x214;
constexpr uint32_t LWF_DATA_FRAMES = 0x98;          // LWFNode +992: LWF*; LWF +0x18: data; data +0x98: frame count

// the last item id (std::vector::at(size - 1): throws on an empty list)
uint32_t lastItem(AdvStageScrollView *self) {
    uint32_t begin = self->itemIDs.begin;
    uint32_t n = uint32_t(int32_t(self->itemIDs.end - begin) >> 2);
    uint32_t last = n - 1;
    if (!(last < n)) call(lib::THROW_OUT_OF_RANGE, {rw::guest_addr(self) + 1412});
    return ld32(begin + last * 4);
}

// the centre page after scrolling by dt: (int)(position + dt, clamped) / (int)itemPixel
int32_t scrolledIndex(AdvStageScrollView *self, float dt) {
    uint32_t me = rw::guest_addr(self);
    float pixel = rw::bitsf(call(lib::BaseStageScrollView_getItemPixel, {me}));
    Frame f;
    uint32_t pos = f.alloc(4);
    stf(pos, rw::get<float>(self, 1472) + dt);
    call(lib2::BaseStageScrollView_checkScrollInterval, {me, pos});
    return int32_t(call(lib::divsi3, {uint32_t(f2i(ldf(pos))), uint32_t(f2i(pixel))}));
}

// the event entry shown by the scene manager (56-byte entries of APIManager + 300): its address, or 0
uint32_t currentEvent() {
    uint32_t events = ld32(call(lib::APIManager_getInstance, {}) + API_EVENTS);
    if (ld32(events + 4) == ld32(events)) return 0;
    uint32_t off = 0;
    for (uint32_t k = 0;;) {
        uint32_t ev = ld32(events) + off;
        uint32_t id = ld32(ev + 12);
        k++;
        if (id == ld32(call(lib::SceneManager_getInstance, {}) + SM_EVENT_ID)) return ev;
        uint32_t n = (uint32_t(int32_t(ld32(events + 4) - ld32(events)) >> 3)) * 0xB6DB6DB7u;
        off += 56;
        if (!(k < n)) return 0;
    }
}

// the row of item `base + k` holds `stage`
bool rowShowsStage(AdvStageScrollView *self, int32_t base, int32_t k, uint32_t stage) {
    int32_t item = int32_t(uint32_t(base) + uint32_t(k));
    if (item < 1) return false;
    uint32_t begin = self->itemIDs.begin;
    if (!(uint32_t(item - 1) < uint32_t(int32_t(self->itemIDs.end - begin) >> 2))) return false;
    return ld32(begin + uint32_t(base) * 4 - 4 + uint32_t(k) * 4) == stage;
}
}  // namespace

// Places the "new stage" mark (playing it once when its row scrolls in) and the raid boss button on the row of
// the current stage.
void AdvStageScrollView::updateScroll(AdvStageScrollView *self, float dt) {
    uint32_t me = rw::guest_addr(self);
    call(lib2::BaseStageScrollView_updateScroll, {me, rw::fbits(dt)});
    if (self->itemIDs.end == self->itemIDs.begin) return;

    // the new stage mark, armed by initNewStage (+1440 / +1441)
    if (rw::get<uint8_t>(self, 1440) && rw::get<uint8_t>(self, 1441)) {
        int32_t page = scrolledIndex(self, dt);
        int32_t perPage = self->field_5B8;
        uint32_t stage = ld32(ld32(call(lib::APIManager_getInstance, {}) + API_CURRENT_STAGE) + 12);
        if (!stage) stage = lastItem(self);
        if (self->eventNewStage) {
            // (the event's own stage is not used here: the last item stands for it)
            if (!ld32(call(lib::SceneManager_getInstance, {}) + SM_EVENT_ID)) stage = lastItem(self);
            else if (currentEvent()) stage = lastItem(self);
        }
        if (rw::get<float>(self, 1436) > 0.0f) {
            int32_t base = int32_t(uint32_t(page) * uint32_t(perPage));
            int32_t k = 0;
            do {
                uint32_t rowPtr = self->widgets.begin + uint32_t(k) * 4;
                if (rowShowsStage(self, base, k, stage)) {
                    uint32_t row = ld32(rowPtr);
                    if (self->raidBossLwf) vcall(self->raidBossLwf, VS_SET_OPACITY, {self->raidBossLwf, 0});
                    vcall(row, VS_SET_OPACITY, {row, 0});
                    rw::set<uint8_t>(self, 1441, 0);
                    uint32_t lwf = self->newStageLwf;
                    int32_t frames = int32_t(ld32(ld32(ld32(lwf + 992) + 0x18) + LWF_DATA_FRAMES));
                    Frame f;
                    uint32_t d1 = call(lib2::DelayTime_create, {0x3F444444u});   // 0.7666667f
                    uint32_t fnShow = newFunction(f, VT_updateScroll_21, {row});
                    uint32_t c1 = call(lib2::CallFunc_create, {fnShow});
                    uint32_t d2 = call(lib2::DelayTime_create, {rw::fbits((float(frames) + 20.0f) / 60.0f)});
                    uint32_t fnFade = newFunction(f, VT_updateScroll_22, {me});
                    uint32_t c2 = call(lib2::CallFunc_create, {fnFade});
                    uint32_t d3 = call(lib2::DelayTime_create, {ld32(me + 1276)});
                    uint32_t fnEnd = newFunction(f, VT_updateScroll_23, {me});
                    uint32_t c3 = call(lib2::CallFunc_create, {fnEnd});
                    uint32_t seq = call(lib::Sequence_create, {d1, c1, d2, c2, d3, c3, 0});
                    call(lib::Node_runAction, {lwf, seq});
                    functionDestroy(fnEnd);
                    functionDestroy(fnFade);
                    functionDestroy(fnShow);
                }
                k++;
            } while (float(k) < rw::get<float>(self, 1436));
        }
    }

    // the raid boss button, armed by initRaidBoss
    if (!self->raidBossShown) return;
    int32_t page = scrolledIndex(self, dt);
    int32_t perPage = self->field_5B8;
    uint32_t stage = ld32(ld32(call(lib::APIManager_getInstance, {}) + API_CURRENT_STAGE) + 12);
    if (!stage) stage = lastItem(self);
    if (self->eventNewStage) {
        if (!ld32(call(lib::SceneManager_getInstance, {}) + SM_EVENT_ID)) {
            stage = lastItem(self);
        } else {
            uint32_t ev = currentEvent();
            if (ev) stage = ld32(ev + 48);    // the event's newest stage
        }
    }
    if (!(rw::get<float>(self, 1436) > 0.0f)) {
        setVisible(self->raidBossLwf, 0);
        return;
    }
    int32_t base = int32_t(uint32_t(page) * uint32_t(perPage));
    bool found = false;
    int32_t k = 0;
    do {
        uint32_t rowPtr = self->widgets.begin + uint32_t(k) * 4;
        if (rowShowsStage(self, base, k, stage)) {
            uint32_t row = ld32(rowPtr);
            uint32_t area = findNode(row, S_RAID_AREA, 9);
            if (area) {
                uint32_t lwf = self->raidBossLwf;
                uint32_t setZ = ld32(ld32(lwf) + VS_NODE_10);
                uint32_t z = vcall(area, VS_NODE_GET_LOCAL_Z, {area});
                call(setZ, {lwf, z + 1});
                Frame f;
                uint32_t size = f.alloc(8);
                call(lib::Vec2_ctor, {size, rw::fbits(156.0f), rw::fbits(280.0f)});
                uint32_t lwf2 = self->raidBossLwf;
                uint32_t setPosition = ld32(ld32(lwf2) + node::setPosition);
                uint32_t rowPos = vcall(row, node::getPosition, {row});
                float hx = ldf(size) * 0.5f;
                float hy = ldf(size + 4) * 0.5f;
                uint32_t half = f.alloc(8);
                call(lib::Vec2_ctor, {half, rw::fbits(hx), rw::fbits(hy)});
                uint32_t pos = f.alloc(8);
                call(lib2::Vec2_copy, {pos, rowPos});
                call(lib2::Vec2_add, {pos, half});
                call(setPosition, {lwf2, pos});
                call(lib::Vec2_dtor, {pos});
                call(lib::Vec2_dtor, {half});
                call(lib::Vec2_dtor, {size});
                found = true;
            }
        }
        k++;
    } while (float(k) < rw::get<float>(self, 1436));
    setVisible(self->raidBossLwf, (found && self->stageType == 0) ? 1 : 0);
}

RW_FUNCTION(0x5710D8, "AdvStageScrollView::updateScroll", AdvStageScrollView::updateScroll);
