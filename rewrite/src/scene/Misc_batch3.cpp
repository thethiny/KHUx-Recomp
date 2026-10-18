// Lambda bodies and std::function slots of several classes that run in game but were left after batch 2 (batch 3,
// named from their __func vtables' target_type). `functor` is the __func object: vtable at +0, then the captures.
#include "SceneUi.hpp"

using namespace scene;

namespace {

constexpr uint32_t SafeBuffer_update = 0x45A395;            // (buffer, bool* pageDone): false = tampered
constexpr uint32_t EXIT = 0x40DE04;                         // exit (import)
constexpr uint32_t audio_Manager_getInstance = 0x5BECFD;
constexpr uint32_t audio_Manager_updateQueue = 0x5BF66D;
constexpr uint32_t audio_Manager_unload = 0x5BF0A1;         // (mgr, ResourceID)
constexpr uint32_t SdSoundSystem_SoundCtrl_IsExist = 0xCDADB0;
constexpr uint32_t PlayingMap_erase = 0x5C20C9;             // unordered_map<int, ResourceID>::erase(node) -> next
constexpr uint32_t PLAYING_SOUNDS = 0x1003C30;              // AudioSystemInit's map of playing sound handles
constexpr uint32_t String_compare = 0x47CBD5;               // std::string::compare(const char*): 0 = equal
constexpr uint32_t CarouselDeck_setDeckMedal = 0x5D17A1;    // (deck, bool)
constexpr uint32_t SafeBuffer_read = 0x45A269;              // (buffer, dst, src, n, index)
constexpr uint32_t Vec2_copy = 0x8E4C03;
constexpr uint32_t MEMCPY = 0x40E164;
constexpr uint32_t V_getAnimation = 0x298;                  // cocostudio::Armature::getAnimation
constexpr uint32_t V_animStop = 0xC;                        // ArmatureAnimation slot 0xC
constexpr uint32_t S_START = 0xE01812;                      // "start"

// FlashMovieView
constexpr uint32_t FMV_ONCE = 0x274, FMV_DONE = 0x278;      // byte / std::function<void()> (callable at +0x288)
// CarouselDeck
constexpr uint32_t CD_DECK = 0x270, CD_ARMATURE = 0x31C, CD_FIELD_378 = 0x378;
// StageUtil: APIManager's treasure table (SafeBuffer: +0x20 slot, +0x24 base row, 24-byte rows)
constexpr uint32_t API_TREASURES = 0x13C;

constexpr uint32_t audio_Manager_getLayer = 0x5BF33D;       // (shared_ptr<Layer>* out, mgr, layer)
constexpr uint32_t audio_Layer_playSE = 0x5BF785;           // (layer, ResourceID, int, int)
constexpr uint32_t CarouselDeck_setDeckMedalInfoPanel = 0x5D212D;   // (deck, widget, const MedalData&)
constexpr uint32_t SceneManager_changeSceneLayer = 0x612B99;        // (mgr, function<Layer*()>*, bool, bool, FadeType)
constexpr uint32_t V_widgetSetTouchEnabled = 0x29C;
constexpr uint32_t V_boneArmature = 0x158;                  // the bone's armature
// EnemyActor
constexpr uint32_t EA_SE = 0x338, EA_SYMBOL = 0x330, EA_BOARD = 0x40C, EA_SHOW_STATE = 0x2A4;
constexpr uint32_t SE_ENEMY_SHOW = 0x1E56B99A, SE_ENEMY_HIDE = 0x1E56B9A4, SE_PRIZE_RARE = 0x1E56BB16;
// CarouselDeck
constexpr uint32_t CD_SLOT_WIDGETS = 0x29C, CD_SLOT_VALUES = 0x394;
// SceneMovie
constexpr uint32_t SM_MOVIE_SKIP = 0x290;
constexpr uint32_t GOT_LAYER_FACTORY_VT = 0xFD6074, GOT_NEXT_LAYER_FN = 0xFD68B4;
constexpr uint32_t S_RARE = 0xDFB52A, S_END = 0xDFCD73;

void playSE(uint32_t id) {
    Frame f;
    const uint32_t layer = f.alloc(8);
    call(audio_Manager_getLayer, {layer, call(audio_Manager_getInstance, {}), 0});
    call(audio_Layer_playSE, {ld32(layer), id, 0, 0});
    if (ld32(layer + 4)) call(fn::shared_release, {ld32(layer + 4)});
}

// __clone(__base*) of lambdas that hold plain words
void cloneInto2(uint32_t self, uint32_t p, uint32_t vtable) {
    if (!p) return;
    st32(p, vtable);
    const uint32_t a = ld32(self + 4), b = ld32(self + 8);
    st32(p + 8, b);
    st32(p + 4, a);
}

constexpr uint32_t VT_imageButton_0 = 0xF5746C;
constexpr uint32_t VT_deckEdit_8 = 0xF59B70, VT_deckEdit_7 = 0xF59BC8, VT_deckEdit_4 = 0xF59CD0;
constexpr uint32_t VT_enemyActor_1 = 0xF6DE90, VT_enemyActor_0 = 0xF6DEBC;
constexpr uint32_t VT_myPageCamera_20 = 0xF61630;
constexpr uint32_t VT_supportMedalTouch = 0xF55EF0;

// run the FlashMovieView's end callback once
void flashMovieDone(uint32_t self) {
    if (!ld32(self + FMV_DONE + 16)) return;
    call(fn::function_vv_call, {self + FMV_DONE});
    functionDestroy(self + FMV_DONE);
    st32(self + FMV_DONE + 16, 0);
}

}  // namespace

struct MiscBatch3 {
    // Scheduler::scheduleUpdate<Guardian> lambda(float) [guardian]: re-hash one SafeBuffer page per frame along
    // the update list; a buffer that fails its hash ends the game
    static void guardianUpdate(uint32_t functor) {
        const uint32_t list = ld32(functor + 4);
        if (!ld32(list + 8)) return;                          // no buffers
        Frame f;
        const uint32_t pageDone = f.alloc(4);
        if (!call(SafeBuffer_update, {ld32(ld32(list + 0xC) + 8), pageDone + 3})) call(EXIT, {0});
        if (!ld8(pageDone + 3)) return;
        const uint32_t next = ld32(ld32(list + 0xC) + 4);
        st32(list + 0xC, next);
        if (next == list) st32(list + 0xC, ld32(list + 4));
    }

    // AudioSystemInit $_0 (float): pump the audio queue; unload the sounds that stopped
    static void audioUpdate(uint32_t /*functor*/) {
        call(audio_Manager_updateQueue, {call(audio_Manager_getInstance, {})});
        uint32_t node = ld32(PLAYING_SOUNDS + 0x28);
        while (node) {
            if (call(SdSoundSystem_SoundCtrl_IsExist, {ld32(node + 8)})) {
                node = ld32(node);
            } else {
                const uint32_t id = ld32(node + 0xC);
                call(audio_Manager_unload, {call(audio_Manager_getInstance, {}), id});
                node = call(PlayingMap_erase, {PLAYING_SOUNDS + 0x20, node});
            }
        }
    }

    // FlashMovieView init $_2 [this] (touch ended) / $_0 [this](Movie*) (enterFrame on the last frame): the end
    // callback, once
    static void flashMovieTouched(uint32_t functor) {
        const uint32_t self = ld32(functor + 4);
        if (ld8(self + FMV_ONCE)) flashMovieDone(self);
    }
    static void flashMovieFrame(uint32_t functor, uint32_t moviePtr) {
        const uint32_t movie = ld32(moviePtr);
        if (ld32(movie + 0x9C) != ld32(movie + 0x98)) return;
        flashMovieDone(ld32(functor + 4));
    }

    // CarouselDeck init $_3 [this](Ref*, TouchEventType): began / moved... hide the armature and stop it
    static void carouselTouched(uint32_t functor, uint32_t /*sender*/, uint32_t typePtr) {
        if (ld32(typePtr) - 1 > 1) return;                    // MOVED or ENDED
        const uint32_t self = ld32(functor + 4);
        const uint32_t armature = ld32(self + CD_ARMATURE);
        vcall(armature, node::setVisible, {armature, 0});
        const uint32_t again = ld32(self + CD_ARMATURE);
        const uint32_t animation = vcall(again, V_getAnimation, {again});
        vcall(animation, V_animStop, {animation});
    }
    // CarouselDeck init $_2 [this](Armature*, MovementEventType, const std::string& id): "start" done
    static void carouselAnimation(uint32_t functor, uint32_t /*armature*/, uint32_t id) {
        const uint32_t self = ld32(functor + 4);
        if (call(String_compare, {id, S_START}) != 0) return;
        st32(self + CD_FIELD_378, 0);
        call(CarouselDeck_setDeckMedal, {self, 1});
        const uint32_t deck = ld32(self + CD_DECK);
        vcall(deck, node::setVisible, {deck, 1});
    }

    // StageUtil::getTreasurePrizes $_0 (int k): treasure row `k` (24 bytes, returned by value)
    static void treasureRow(uint32_t out, uint32_t /*lambda*/, uint32_t kPtr) {
        const int32_t k = int32_t(ld32(kPtr));
        const uint32_t sb = ld32(call(fn::APIManager_getInstance, {}) + API_TREASURES);
        const uint32_t index = (ld32(sb + 0x24) + uint32_t(k)) * 0x18 + (ld32(sb + 0x20) << 2);
        call(SafeBuffer_read, {sb, out, ld32(sb + 0xC) + index, 0x18, index});
    }

    // EnemyActor::EnemyActor $_1 / $_0 [this, last frame](int frame): the hide / show movie - the SE on frame 1
    // (when the actor has none of its own), then hide / show the symbol and its HP board from the last frame on
    static void enemyHideFrame(uint32_t functor, uint32_t framePtr) {
        const int32_t frame = int32_t(ld32(framePtr));
        const uint32_t self = ld32(functor + 4);
        if (frame == 1) {
            if (!ld32(self + EA_SE)) playSE(SE_ENEMY_HIDE);
            return;
        }
        if (int32_t(ld32(functor + 8)) > frame) return;
        vcall(ld32(self + EA_SYMBOL), node::setVisible, {ld32(self + EA_SYMBOL), 0});
        vcall(ld32(self + EA_BOARD), node::setVisible, {ld32(self + EA_BOARD), 0});
        st32(self + EA_SHOW_STATE, 0);
    }
    static void enemyShowFrame(uint32_t functor, uint32_t framePtr) {
        const int32_t frame = int32_t(ld32(framePtr));
        const uint32_t self = ld32(functor + 4);
        if (frame == 1) {
            if (!ld32(self + EA_SE)) playSE(SE_ENEMY_SHOW);
            return;
        }
        if (int32_t(ld32(functor + 8)) > frame) return;
        vcall(ld32(self + EA_SYMBOL), node::setVisible, {ld32(self + EA_SYMBOL), 1});
        const uint32_t board = ld32(self + EA_BOARD);
        const uint32_t setVisible = ld32(ld32(board) + node::setVisible);
        call(setVisible, {board, vcall(self, node::isVisible, {self})});
        st32(self + EA_SHOW_STATE, 2);
    }

    // CarouselDeck::setDeckInfoWindow $_5 [&slot, deck](const MedalData&): fill the next slot's medal panel
    static uint32_t deckSlotFilled(uint32_t functor, uint32_t medal) {
        const uint32_t deck = ld32(functor + 8);
        const uint32_t slot = ld32(ld32(functor + 4));
        call(CarouselDeck_setDeckMedalInfoPanel, {deck, ld32(deck + slot * 4 + CD_SLOT_WIDGETS), medal});
        const int32_t value = safe32(medal + 8, 0x14);
        st32(deck + ld32(ld32(functor + 4)) * 4 + CD_SLOT_VALUES, uint32_t(value));
        st32(ld32(functor + 4), ld32(ld32(functor + 4)) + 1);
        return 1;
    }

    // MapPrizes::createArmature $_1 (Bone*, const std::string& event): "rare" plays the SE, "end" flags the armature
    static void prizeFrameEvent(uint32_t /*functor*/, uint32_t bonePtr, uint32_t event) {
        const uint32_t bone = ld32(bonePtr);
        if (call(String_compare, {event, S_RARE}) == 0) {
            playSE(SE_PRIZE_RARE);
            return;
        }
        if (call(String_compare, {event, S_END}) != 0) return;
        if (const uint32_t armature = vcall(bone, V_boneArmature, {bone})) st8(armature + 0x2B0, 1);
    }

    // SceneMovie init $_3 [?, this](Ref*, EventType): playing - the skip button works; completed - the next layer
    static void movieEvent(uint32_t functor, uint32_t /*sender*/, uint32_t typePtr) {
        const uint32_t type = ld32(typePtr);
        if (type == 0) {
            const uint32_t skip = ld32(ld32(functor + 8) + SM_MOVIE_SKIP);
            vcall(skip, V_widgetSetTouchEnabled, {skip, 1});
            return;
        }
        if (type != 3) return;
        for (int k = 0; k < 2; k++) {                           // (the original changes the layer twice)
            const uint32_t mgr = call(fn::SceneManager_getInstance, {});
            Frame f;
            const uint32_t factory = newFunction(f, ld32(GOT_LAYER_FACTORY_VT) + 8, {ld32(GOT_NEXT_LAYER_FN)});
            call(SceneManager_changeSceneLayer, {mgr, factory, 1, 1, 1});
            functionDestroy(factory);
        }
    }

    // __clone(__base*) / __clone() slots
    static void imageButtonCloneInto(uint32_t self, uint32_t p) {
        if (!p) return;
        st32(p, VT_imageButton_0);
        const uint32_t a = ld32(self + 4), b = ld32(self + 8), c = ld32(self + 0xC);
        st32(p + 0xC, c);
        st32(p + 8, b);
        st32(p + 4, a);
    }
    static void deckEdit8CloneInto(uint32_t self, uint32_t p) { cloneInto2(self, p, VT_deckEdit_8); }
    static void deckEdit7CloneInto(uint32_t self, uint32_t p) { cloneInto2(self, p, VT_deckEdit_7); }
    static void deckEdit4CloneInto(uint32_t self, uint32_t p) { cloneInto2(self, p, VT_deckEdit_4); }
    static void enemyActor1CloneInto(uint32_t self, uint32_t p) { cloneInto2(self, p, VT_enemyActor_1); }
    static void enemyActor0CloneInto(uint32_t self, uint32_t p) { cloneInto2(self, p, VT_enemyActor_0); }
    // SceneMyPage::setCameraPosition $_20 [word, Vec2]
    static uint32_t myPageCameraClone(uint32_t self) {
        const uint32_t obj = call(fn::operator_new, {0x10});
        if (obj) {
            st32(obj, VT_myPageCamera_20);
            st32(obj + 4, ld32(self + 4));
            call(Vec2_copy, {obj + 8, self + 8});
            st32(obj + 4, ld32(self + 4));
        }
        return obj;
    }
    // SupportCharacterScrollView::changeRow $_0 [this, SupportUser ...] (0x138 bytes, copied whole)
    static uint32_t supportMedalTouchClone(uint32_t self) {
        const uint32_t obj = call(fn::operator_new, {0x138});
        if (obj) {
            st32(obj, VT_supportMedalTouch);
            call(MEMCPY, {obj + 8, self + 8, 0x130});
        }
        return obj;
    }
};

using L = MiscBatch3;
RW_FUNCTION(0x45A950, "Scheduler::scheduleUpdate<Guardian> lambda(float) operator()", L::guardianUpdate);
RW_FUNCTION(0x5C2034, "AudioSystemInit::$_0::operator()", L::audioUpdate);
RW_FUNCTION(0x568986, "FlashMovieView::init::$_2::operator()", L::flashMovieTouched);
RW_FUNCTION(0x568AEA, "FlashMovieView::init::$_0::operator()", L::flashMovieFrame);
RW_FUNCTION(0x5D31BE, "CarouselDeck::init::$_3::operator()", L::carouselTouched);
RW_FUNCTION(0x5D3284, "CarouselDeck::init::$_2::operator()", L::carouselAnimation);
RW_FUNCTION(0x76493E, "StageUtil::getTreasurePrizes::$_0::operator()", L::treasureRow);
RW_FUNCTION(0x53212C, "ImageButton::init::$_0 __clone(__base*)", L::imageButtonCloneInto);
RW_FUNCTION(0x57CA64, "StartDeckEditDialog::initLayout::$_8 __clone(__base*)", L::deckEdit8CloneInto);
RW_FUNCTION(0x57CCF4, "StartDeckEditDialog::initLayout::$_7 __clone(__base*)", L::deckEdit7CloneInto);
RW_FUNCTION(0x57D4C8, "StartDeckEditDialog::initLayout::$_4 __clone(__base*)", L::deckEdit4CloneInto);
RW_FUNCTION(0x74E47C, "EnemyActor::EnemyActor::$_1 __clone(__base*)", L::enemyActor1CloneInto);
RW_FUNCTION(0x74E5E4, "EnemyActor::EnemyActor::$_0 __clone(__base*)", L::enemyActor0CloneInto);
RW_FUNCTION(0x64E100, "SceneMyPage::setCameraPosition::$_20 __clone()", L::myPageCameraClone);
RW_FUNCTION(0x50544C, "SupportCharacterScrollView::changeRow::$_0 __clone()", L::supportMedalTouchClone);
RW_FUNCTION(0x74E4AC, "EnemyActor::EnemyActor::$_1::operator()", L::enemyHideFrame);
RW_FUNCTION(0x74E614, "EnemyActor::EnemyActor::$_0::operator()", L::enemyShowFrame);
RW_FUNCTION(0x5D2FD4, "CarouselDeck::setDeckInfoWindow::$_5::operator()", L::deckSlotFilled);
RW_FUNCTION(0x5DECCC, "MapPrizes::createArmature::$_1::operator()", L::prizeFrameEvent);
RW_FUNCTION(0x6393F8, "SceneMovie::init::$_3::operator()", L::movieEvent);
