// SceneManager: the single layer that hosts every scene. It owns the scene layer stack (the current scene layer,
// the popup layers, the fade and touch-effect layers), the header / title header / info header, the popups opened
// through openPopup (with their "can close" / "on close" callbacks), the background, the chat window and the
// tutorial chains run from the global menu. One instance (getInstance), kept in a guest global.
#pragma once

#include "SceneCommon.hpp"

struct SceneManager {
    const uint8_t layer[0x288];                                   // cocos2d::Layer
    rw::Field<uint32_t> reserved288;                        // +0x288 zeroed by the constructor (with 0x28C..0x29B)
    rw::Field<uint32_t> sceneParam;                         // +0x28C set before some scene changes (9 adventure select, 1 sphere board select)
    const uint8_t pad_290[0x8];
    rw::Field<int32_t> backgroundType;                      // +0x298 SceneManager::BackGroundType of the current background
    const uint8_t pad_29C[0x4];
    rw::Field<uint32_t> field2A0;                           // +0x2A0
    rw::Field<uint32_t> field2A4;                           // +0x2A4
    rw::Field<uint8_t> flag2A8;                             // +0x2A8
    const uint8_t pad_2A9[0x7];
    rw::Field<uint32_t> nextA;                              // +0x2B0 where to go next (campaign stage, set by SceneUtil::moveToMyPageOrResumeStage)
    rw::Field<uint32_t> nextB;                              // +0x2B4
    rw::Field<uint32_t> field2B8;                           // +0x2B8
    const uint8_t pad_2BC[0x4];
    rw::Field<uint32_t> nextC;                              // +0x2C0
    rw::Field<uint32_t> nextD;                              // +0x2C4
    rw::Field<uint32_t> nextE;                              // +0x2C8
    const uint8_t pad_2CC[0x4];
    rw::Field<uint32_t> nextF;                              // +0x2D0
    rw::Field<uint32_t> nextG;                              // +0x2D4
    rw::Field<uint32_t> nextH;                              // +0x2D8
    rw::Field<uint32_t> nextI;                              // +0x2DC
    rw::Field<uint8_t> flag2E0;                             // +0x2E0 cleared before SceneDeckEdit
    rw::Field<uint8_t> flag2E1;                             // +0x2E1 cleared before SceneDeckEdit
    const uint8_t pad_2E2[0x2];
    rw::Field<uint32_t> field2E4;                           // +0x2E4
    rw::Field<uint32_t> field2E8;                           // +0x2E8
    rw::Field<uint8_t> flag2EC;                             // +0x2EC
    const uint8_t pad_2ED[0x3];
    rw::Field<uint32_t> field2F0;                           // +0x2F0
    rw::Field<uint16_t> field2F4;                           // +0x2F4 the constructor zeroes 0x2F4..0x2F7 with one word
    rw::Field<uint8_t> flag2F6;                             // +0x2F6
    rw::Field<uint8_t> avatarBoardDirty;                    // +0x2F7 new avatar board ids to check on the next scene change
    rw::Field<uint32_t> avatarBoardMode;                    // +0x2F8 starts at 1; avatar board kind compared with master Sphere 0x98
    rw::Field<uint8_t> flag2FC;                             // +0x2FC the chat stamp list keeps paging (migration result)
    const uint8_t pad_2FD[0x3];
    rw::Field<float> field300;                              // +0x300 starts at 0.4
    rw::Field<uint32_t> apRecoveryText;                     // +0x304 ui::Text of the AP recovery countdown (AP popup)
    rw::Field<uint8_t> flag308;                             // +0x308
    rw::Field<uint8_t> flag309;                             // +0x309
    rw::Field<uint8_t> flag30A;                             // +0x30A starts at 1
    rw::Field<uint8_t> flag30B;                             // +0x30B starts at 1
    rw::Field<uint32_t> field30C;                           // +0x30C
    rw::Field<uint8_t> raidAlertsShown;                     // +0x310 the header raid alert button may show
    const uint8_t pad_311[0x3];
    rw::Field<float> infoHeaderOffset;                      // +0x314 y shift of the header while the info header shows
    rw::Field<uint32_t> sceneState;                         // +0x318 update(): 0 wait for APIs, 1 error dialog, 2 change scene, 3 fade out, 4 running, 5 fade in
    rw::Field<uint8_t> flag31C;                             // +0x31C
    rw::Field<uint8_t> flag31D;                             // +0x31D
    rw::Field<uint8_t> flag31E;                             // +0x31E
    rw::Field<uint8_t> abortOnNextScene;                    // +0x31F createNextScene aborts purchases / API responses, clears popups
    rw::Field<uint8_t> touchEffectEnabled;                  // +0x320
    const uint8_t pad_321[0x3];
    rw::Field<uint32_t> touchEffectLayer;                   // +0x324 cocos2d::Layer* (z 10) for the touch effects
    rw::Field<uint8_t> headerRequested;                     // +0x328 showHeader / hideHeader
    rw::Field<uint8_t> flag329;                             // +0x329
    const uint8_t pad_32A[0x2];
    rw::Field<float> vec32C[2];                             // +0x32C cocos2d::Vec2
    rw::Field<uint32_t> backButtonType;                     // +0x334 SceneManager::BackButtonType
    rw::Field<uint32_t> header;                             // +0x338 the header widget (or null)
    rw::Field<uint8_t> infoHeaderShown;                     // +0x33C
    const uint8_t pad_33D[0x3];
    rw::Field<uint32_t> infoHeader;                         // +0x340 the info header widget (or null)
    rw::Field<float> tickerRight;                           // +0x344 ticker area right edge x (scroll start)
    rw::Field<float> tickerLeft;                            // +0x348 ticker label start x
    rw::Field<float> tickerEnd;                             // +0x34C tickerLeft - text width (scroll end)
    const uint8_t tickerText[12];                                 // +0x350 std::string
    rw::Field<uint32_t> globalMenuAnimation;                // +0x35C Armature of the global menu open / close animation
    rw::Field<uint32_t> headerShadow;                       // +0x360 header shadow sprite (z 3), created once
    rw::Field<uint32_t> globalMenuSelected;                 // +0x364 the picked global menu button (-1 none)
    rw::Field<uint8_t> globalMenuOpen;                      // +0x368 the global menu is open (counts as a popup)
    rw::Field<uint8_t> globalMenuClosed;                    // +0x369 set when the global menu closes
    const uint8_t pad_36A[0x6];
    const uint8_t globalMenuSceneFactory[20];                     // +0x370 std::function<Layer*()>: the scene picked in the global menu
    const uint8_t pad_384[0x4];
    rw::Field<uint8_t> titleHeaderShown;                    // +0x388
    const uint8_t pad_389[0x3];
    rw::Field<uint32_t> titleHeader;                        // +0x38C the title header widget (or null)
    rw::Field<uint32_t> titleBoard;                         // +0x390
    rw::Field<uint32_t> titleLogo;                          // +0x394
    rw::Field<uint32_t> titleSub1Board;                     // +0x398
    rw::Field<uint32_t> titleSub1Label;                     // +0x39C
    rw::Field<float> titleSub1X;                            // +0x3A0 original layout x
    rw::Field<uint32_t> titleSub2Board;                     // +0x3A4
    rw::Field<uint32_t> titleSub2Label;                     // +0x3A8
    rw::Field<float> titleSub2X;                            // +0x3AC original layout x
    rw::Field<uint32_t> titleMainShown;                     // +0x3B0 copied from titleMain when the title header is created
    rw::Field<uint32_t> titleMain;                          // +0x3B4 SceneManager::TitleMain
    rw::Field<uint32_t> titleSub1Shown;                     // +0x3B8
    rw::Field<uint32_t> titleSub1;                          // +0x3BC
    rw::Field<uint32_t> titleSub2Shown;                     // +0x3C0
    rw::Field<uint32_t> titleSub2;                          // +0x3C4
    const uint8_t nextSceneFactory[20];                           // +0x3C8 std::function<Layer*()>: the next scene layer to create
    const uint8_t pad_3DC[0x4];
    rw::Field<uint32_t> fadeLayer;                          // +0x3E0 cocos2d::LayerColor (black, z 11)
    rw::Field<uint32_t> sceneLayer;                         // +0x3E4 the current scene layer (z 2)
    rw::Field<uint32_t> popupLayer;                         // +0x3E8 z 6
    rw::Field<uint32_t> popupLayer2;                        // +0x3EC z 7
    rw::Field<uint32_t> popupLayer3;                        // +0x3F0 z 13
    rw::Field<uint32_t> popupLayerDerived;                  // +0x3F4 z 8
    rw::Field<uint32_t> popupLayerDerived2;                 // +0x3F8 z 9
    rw::Field<uint32_t> chatNewIcon;                        // +0x3FC chat button Icon_New badge (0 when a scene has no header)
    rw::Field<uint32_t> chatInputPanel;                     // +0x400 chat input panel (Panel_1)
    rw::Field<uint32_t> chatScrollView;                     // +0x404 ChatScrollView*: the message list
    rw::Field<uint32_t> chatTotalMessages;                  // +0x408 total messages on the server
    rw::Field<uint32_t> chatFirstRequested;                 // +0x40C first message requested (pages of 50)
    const uint8_t pad_410[0x4];
    rw::Field<uint32_t> chatLastSeenId;                     // +0x414 last seen message id
    rw::Field<uint32_t> chatKeepScrollId;                   // +0x418 message id to keep the scroll at
    const uint8_t pad_41C[0x4];
    rw::Field<int32_t> chatRowCount;                        // +0x420
    const uint8_t pad_424[0x4];
    rw::Field<int32_t> chatBlockCount;                      // +0x428
    rw::Field<uint32_t> field42C;                           // +0x42C cleared when a chat request starts
    rw::Field<uint32_t> chatMessages[3];                    // +0x430 vector<ChatManager::Message> (76 bytes): last received page
    rw::Field<uint32_t> chatHiddenIds[3];                   // +0x43C vector<u32>: hidden message ids
    rw::Field<uint32_t> chatLocalBlocks[3];                 // +0x448 vector<std::string>: user ids blocked on this device
    rw::Field<uint32_t> chatServerBlocks[3];                // +0x454 vector<ChatManager::Block> (68 bytes, user id +0x18)
    const uint8_t pad_460[0xC];
    rw::Field<uint8_t> chatPopupDisp;                       // +0x46C the chat popup is shown
    rw::Field<uint8_t> chatMessageRequest;                  // +0x46D a message request is running
    rw::Field<uint8_t> chatPageRequest;                     // +0x46E a page request is running
    rw::Field<uint8_t> chatRebuild;                         // +0x46F rebuild the scroll view
    rw::Field<uint8_t> chatOpened;                          // +0x470 set when the popup opens
    const uint8_t pad_471[0x3];
    rw::Field<float> field474;                              // +0x474 chat poll timer
    rw::Field<uint32_t> field478;                           // +0x478 Txt_Chat_Label ui::TextField* (0 once the edit box replaces it)
    const uint8_t pad_47C[0x4];
    const uint8_t chatClose[20];                                  // +0x480 std::function<void()>: closes the chat popup
    const uint8_t pad_494[0x4];
    rw::Field<uint8_t> flag498;                             // +0x498 the stamp list popup is open / migrating
    const uint8_t pad_499[0x3];
    rw::Field<uint32_t> field49C;                           // +0x49C stamp page
    rw::Field<float> field4A0;                              // +0x4A0 stamp paging timer
    rw::Field<uint16_t> field4A4;                           // +0x4A4 byte 0: stamp page to the left, byte 1: the chat edit box is being edited
    const uint8_t pad_4A6[0x2];
    rw::Field<uint32_t> chatStampList;                      // +0x4A8 ChatStampListScrollView* (created once)
    rw::Field<uint32_t> chatEditBox;                        // +0x4AC EditBox* of the chat input
    rw::Field<uint32_t> chatEditBoxLayer;                   // +0x4B0
    rw::Field<uint32_t> chatSendDelegate;                   // +0x4B4 ChatEditBoxMessageSend* (created once)
    rw::Field<uint32_t> noTapShadow;                        // +0x4B8 the shadow sprite of openNoTapScreen (popup layer 3)
    const uint8_t pad_4BC[0x8];
    rw::Field<uint32_t> field4C4;                           // +0x4C4
    rw::Field<uint32_t> loadingIndicator;                   // +0x4C8 LoadingIndicator*
    rw::Field<uint8_t> loadingBlocked;                      // +0x4CC set before an API call that must not show the loading indicator
    rw::Field<uint8_t> flag4CD;                             // +0x4CD
    const uint8_t pad_4CE[0x2];
    const uint8_t prepareFinishSceneCallback[20];                 // +0x4D0 std::function<bool()>
    const uint8_t pad_4E4[0x4];
    const uint8_t finishSceneCallback[20];                        // +0x4E8 std::function<void(std::function<void(bool)>)>
    const uint8_t pad_4FC[0x4];
    const uint8_t function500[20];                                // +0x500 std::function<void(std::function<void(bool)>)> (global menu scene change)
    const uint8_t pad_514[0x4];
    const uint8_t chatButtonCallback[20];                         // +0x518 std::function<bool()>
    const uint8_t pad_52C[0x4];
    const uint8_t onChangeProfileCallback[20];                    // +0x530 std::function<void()>
    const uint8_t pad_544[0x4];
    const uint8_t onFinishLoading[20];                            // +0x548 std::function<void(bool)>
    const uint8_t pad_55C[0x4];
    const uint8_t backgroundName[12];                             // +0x560 std::string
    rw::Field<uint32_t> background;                         // +0x56C the background node (or null)
    rw::Field<uint32_t> apShown;                            // +0x570 AP shown in the header (-1 at creation)
    rw::Field<uint32_t> apMaxShown;                         // +0x574 max AP shown
    rw::Field<uint32_t> apSeconds;                          // +0x578 seconds to the next AP last shown
    const uint8_t pushToken[12];                                  // +0x57C std::string: device push token to register
    rw::Field<uint8_t> flag588;                             // +0x588
    const uint8_t pad_589[0x3];
    rw::Field<uint32_t> popupCanClose[3];                   // +0x58C std::vector<std::function<bool()>> (24-byte entries): the open popups
    rw::Field<uint32_t> popupOnClose[3];                    // +0x598 std::vector<std::function<void(bool)>> (24-byte entries)
    rw::Field<uint32_t> popupFlags[3];                      // +0x5A4 std::vector<bool> (begin, bit count, capacity): one flag per open popup
    const uint8_t onBackKey[20];                                  // +0x5B0 std::function<void()>
    const uint8_t pad_5C4[0x4];
    rw::Field<uint32_t> barButtonCopy;                      // +0x5C8 the RightUI_BarButton_Copy node
    rw::Field<uint32_t> faceImageUrls[3];                   // +0x5CC std::map<std::string, std::string> (begin, root, size)

    using Addr = uint32_t;
    static constexpr uint32_t SIZE = 0x5D8;
    static constexpr uint32_t INSTANCE = 0x1003E80;      // guest global: SceneManager* (getInstance)
    // cocos2d::Node std::function<void()> members (base class)
    static constexpr uint32_t NODE_ON_ENTER_CALLBACK = 0x210, NODE_ON_EXIT_CALLBACK = 0x228;

    static SceneManager *instance() { return rw::host_ptr<SceneManager>(rw::load<uint32_t>(INSTANCE)); }
    uint32_t addr() const { return rw::guest_addr(this); }

    // ---- lifetime (SceneManagerCore.cpp)
    static Addr createScene();
    static SceneManager *getInstance();
    static SceneManager *create();
    static SceneManager *construct(SceneManager *self);                 // SceneManager::SceneManager()
    static bool init(SceneManager *self);

    // ---- scene changes, loading, title (SceneManagerCore.cpp)
    static void setEnabledTouchEffect(SceneManager *self, uint32_t enabled);
    static void moveToTitleForcibly(SceneManager *self, int32_t textId, Addr callback);
    static void enableLoadingIndicator(SceneManager *self, uint32_t enable);
    static void moveToTitleIfPossible(SceneManager *self, int32_t textId, Addr callback);
    static void callPrevChangeNextSceneAPIs(SceneManager *self);
    static void setCanTouchHeader(SceneManager *self, uint32_t canTouch);
    static void createNextScene(SceneManager *self);
    static void removeBackground(SceneManager *self);

    // ---- header visibility (SceneManagerCore.cpp); callback = std::function<void()>*
    static void changeTitleHeader(SceneManager *self, uint32_t main, uint32_t sub1, uint32_t sub2);
    static void hideHeader(SceneManager *self, Addr callback);
    static void showInfoHeader(SceneManager *self);
    static void hideInfoHeader(SceneManager *self, Addr callback);
    static void hideHeaderRight(SceneManager *self);
    static void showTitleHeader(SceneManager *self, uint32_t main, uint32_t sub1, uint32_t sub2);
    static void hideTitleHeader(SceneManager *self, Addr callback);
    static void showHeader(SceneManager *self);
    static void updateHeaderRaidAlertsBtn(SceneManager *self);
    static void setHeaderRaidAlertsBtn(SceneManager *self, uint32_t shown);
    static void showHeaderRight(SceneManager *self);
    static void setBackButtonType(SceneManager *self, uint32_t type);
    static void setBackButtonTypeVisible(SceneManager *self, uint32_t type);
    static void openOtherPopup(SceneManager *self);

    // ---- callbacks and accessors (SceneManagerCore.cpp); function = std::function<...>*
    static void setPrepareFinishSceneCallback(SceneManager *self, Addr function);
    static void setFinishSceneCallback(SceneManager *self, Addr function);
    static void setChatButtonCallback(SceneManager *self, Addr function);
    static void setOnBackKeyCallback(SceneManager *self, Addr function);
    static void setOnChangeProfile(SceneManager *self, Addr function);
    static void onChangeProfile(SceneManager *self);
    static bool canFinishScene(SceneManager *self);
    static Addr getPopupLayer(SceneManager *self);
    static Addr getPopupLayer2(SceneManager *self);
    static Addr getPopupLayer3(SceneManager *self);
    static Addr getPopupLayerDerived(SceneManager *self);
    static Addr getPopupLayerDerived2(SceneManager *self);
    static uint8_t getChatPopupDisp(SceneManager *self);
    static void chatBlockListAdd(SceneManager *self, int32_t count);

    // ---- popup stack, scene layers, background (SceneManagerPopups.cpp)
    static void openPopup(SceneManager *self, Addr canClose, Addr onClose, uint32_t flag);
    static bool closeCurrentPopup(SceneManager *self, uint32_t byBackKey);
    static bool isOpenedPopup(SceneManager *self);
    static bool isOpenedPopupOnly(SceneManager *self);
    static void openPopupAllClear(SceneManager *self);
    static void changeSceneLayer(SceneManager *self, Addr factory, uint32_t fadeOut, uint32_t fadeIn,
                                 uint32_t fadeType);
    static void changeBackground(SceneManager *self, int32_t type, Addr name);

    // ---- declarations owned by helper files (one .inc per topic, included here)
#include "SceneManager_Chat.inc"
#include "SceneManager_ChatPopups.inc"
#include "SceneManager_Header.inc"
#include "SceneManager_Tutorial.inc"
#include "SceneManager_HeaderBuild.inc"
#include "SceneManager_HeaderUpdate.inc"
#include "SceneManager_Party.inc"
#include "SceneManager_LambdasA.inc"
#include "SceneManager_LambdasB.inc"
#include "SceneManager_LambdasC.inc"
};
RW_LAYOUT(SceneManager, reserved288, 0x288);
RW_LAYOUT(SceneManager, sceneParam, 0x28C);
RW_LAYOUT(SceneManager, backgroundType, 0x298);
RW_LAYOUT(SceneManager, field2A0, 0x2A0);
RW_LAYOUT(SceneManager, field2A4, 0x2A4);
RW_LAYOUT(SceneManager, flag2A8, 0x2A8);
RW_LAYOUT(SceneManager, nextA, 0x2B0);
RW_LAYOUT(SceneManager, nextB, 0x2B4);
RW_LAYOUT(SceneManager, field2B8, 0x2B8);
RW_LAYOUT(SceneManager, nextC, 0x2C0);
RW_LAYOUT(SceneManager, nextD, 0x2C4);
RW_LAYOUT(SceneManager, nextE, 0x2C8);
RW_LAYOUT(SceneManager, nextF, 0x2D0);
RW_LAYOUT(SceneManager, nextG, 0x2D4);
RW_LAYOUT(SceneManager, nextH, 0x2D8);
RW_LAYOUT(SceneManager, nextI, 0x2DC);
RW_LAYOUT(SceneManager, flag2E0, 0x2E0);
RW_LAYOUT(SceneManager, flag2E1, 0x2E1);
RW_LAYOUT(SceneManager, field2E4, 0x2E4);
RW_LAYOUT(SceneManager, field2E8, 0x2E8);
RW_LAYOUT(SceneManager, flag2EC, 0x2EC);
RW_LAYOUT(SceneManager, field2F0, 0x2F0);
RW_LAYOUT(SceneManager, field2F4, 0x2F4);
RW_LAYOUT(SceneManager, flag2F6, 0x2F6);
RW_LAYOUT(SceneManager, avatarBoardDirty, 0x2F7);
RW_LAYOUT(SceneManager, avatarBoardMode, 0x2F8);
RW_LAYOUT(SceneManager, flag2FC, 0x2FC);
RW_LAYOUT(SceneManager, field300, 0x300);
RW_LAYOUT(SceneManager, apRecoveryText, 0x304);
RW_LAYOUT(SceneManager, flag308, 0x308);
RW_LAYOUT(SceneManager, flag309, 0x309);
RW_LAYOUT(SceneManager, flag30A, 0x30A);
RW_LAYOUT(SceneManager, flag30B, 0x30B);
RW_LAYOUT(SceneManager, field30C, 0x30C);
RW_LAYOUT(SceneManager, raidAlertsShown, 0x310);
RW_LAYOUT(SceneManager, infoHeaderOffset, 0x314);
RW_LAYOUT(SceneManager, sceneState, 0x318);
RW_LAYOUT(SceneManager, flag31C, 0x31C);
RW_LAYOUT(SceneManager, flag31D, 0x31D);
RW_LAYOUT(SceneManager, flag31E, 0x31E);
RW_LAYOUT(SceneManager, abortOnNextScene, 0x31F);
RW_LAYOUT(SceneManager, touchEffectEnabled, 0x320);
RW_LAYOUT(SceneManager, touchEffectLayer, 0x324);
RW_LAYOUT(SceneManager, headerRequested, 0x328);
RW_LAYOUT(SceneManager, flag329, 0x329);
RW_LAYOUT(SceneManager, vec32C, 0x32C);
RW_LAYOUT(SceneManager, backButtonType, 0x334);
RW_LAYOUT(SceneManager, header, 0x338);
RW_LAYOUT(SceneManager, infoHeaderShown, 0x33C);
RW_LAYOUT(SceneManager, infoHeader, 0x340);
RW_LAYOUT(SceneManager, tickerRight, 0x344);
RW_LAYOUT(SceneManager, tickerLeft, 0x348);
RW_LAYOUT(SceneManager, tickerEnd, 0x34C);
RW_LAYOUT(SceneManager, tickerText, 0x350);
RW_LAYOUT(SceneManager, globalMenuAnimation, 0x35C);
RW_LAYOUT(SceneManager, headerShadow, 0x360);
RW_LAYOUT(SceneManager, globalMenuSelected, 0x364);
RW_LAYOUT(SceneManager, globalMenuOpen, 0x368);
RW_LAYOUT(SceneManager, globalMenuClosed, 0x369);
RW_LAYOUT(SceneManager, globalMenuSceneFactory, 0x370);
RW_LAYOUT(SceneManager, titleHeaderShown, 0x388);
RW_LAYOUT(SceneManager, titleHeader, 0x38C);
RW_LAYOUT(SceneManager, titleBoard, 0x390);
RW_LAYOUT(SceneManager, titleLogo, 0x394);
RW_LAYOUT(SceneManager, titleSub1Board, 0x398);
RW_LAYOUT(SceneManager, titleSub1Label, 0x39C);
RW_LAYOUT(SceneManager, titleSub1X, 0x3A0);
RW_LAYOUT(SceneManager, titleSub2Board, 0x3A4);
RW_LAYOUT(SceneManager, titleSub2Label, 0x3A8);
RW_LAYOUT(SceneManager, titleSub2X, 0x3AC);
RW_LAYOUT(SceneManager, titleMainShown, 0x3B0);
RW_LAYOUT(SceneManager, titleMain, 0x3B4);
RW_LAYOUT(SceneManager, titleSub1Shown, 0x3B8);
RW_LAYOUT(SceneManager, titleSub1, 0x3BC);
RW_LAYOUT(SceneManager, titleSub2Shown, 0x3C0);
RW_LAYOUT(SceneManager, titleSub2, 0x3C4);
RW_LAYOUT(SceneManager, nextSceneFactory, 0x3C8);
RW_LAYOUT(SceneManager, fadeLayer, 0x3E0);
RW_LAYOUT(SceneManager, sceneLayer, 0x3E4);
RW_LAYOUT(SceneManager, popupLayer, 0x3E8);
RW_LAYOUT(SceneManager, popupLayer2, 0x3EC);
RW_LAYOUT(SceneManager, popupLayer3, 0x3F0);
RW_LAYOUT(SceneManager, popupLayerDerived, 0x3F4);
RW_LAYOUT(SceneManager, popupLayerDerived2, 0x3F8);
RW_LAYOUT(SceneManager, chatNewIcon, 0x3FC);
RW_LAYOUT(SceneManager, chatInputPanel, 0x400);
RW_LAYOUT(SceneManager, chatScrollView, 0x404);
RW_LAYOUT(SceneManager, chatTotalMessages, 0x408);
RW_LAYOUT(SceneManager, chatFirstRequested, 0x40C);
RW_LAYOUT(SceneManager, chatLastSeenId, 0x414);
RW_LAYOUT(SceneManager, chatKeepScrollId, 0x418);
RW_LAYOUT(SceneManager, chatRowCount, 0x420);
RW_LAYOUT(SceneManager, chatBlockCount, 0x428);
RW_LAYOUT(SceneManager, field42C, 0x42C);
RW_LAYOUT(SceneManager, chatMessages, 0x430);
RW_LAYOUT(SceneManager, chatHiddenIds, 0x43C);
RW_LAYOUT(SceneManager, chatLocalBlocks, 0x448);
RW_LAYOUT(SceneManager, chatServerBlocks, 0x454);
RW_LAYOUT(SceneManager, chatPopupDisp, 0x46C);
RW_LAYOUT(SceneManager, chatMessageRequest, 0x46D);
RW_LAYOUT(SceneManager, chatPageRequest, 0x46E);
RW_LAYOUT(SceneManager, chatRebuild, 0x46F);
RW_LAYOUT(SceneManager, chatOpened, 0x470);
RW_LAYOUT(SceneManager, field474, 0x474);
RW_LAYOUT(SceneManager, field478, 0x478);
RW_LAYOUT(SceneManager, chatClose, 0x480);
RW_LAYOUT(SceneManager, flag498, 0x498);
RW_LAYOUT(SceneManager, field49C, 0x49C);
RW_LAYOUT(SceneManager, field4A0, 0x4A0);
RW_LAYOUT(SceneManager, field4A4, 0x4A4);
RW_LAYOUT(SceneManager, chatStampList, 0x4A8);
RW_LAYOUT(SceneManager, chatEditBox, 0x4AC);
RW_LAYOUT(SceneManager, chatEditBoxLayer, 0x4B0);
RW_LAYOUT(SceneManager, chatSendDelegate, 0x4B4);
RW_LAYOUT(SceneManager, noTapShadow, 0x4B8);
RW_LAYOUT(SceneManager, field4C4, 0x4C4);
RW_LAYOUT(SceneManager, loadingIndicator, 0x4C8);
RW_LAYOUT(SceneManager, loadingBlocked, 0x4CC);
RW_LAYOUT(SceneManager, flag4CD, 0x4CD);
RW_LAYOUT(SceneManager, prepareFinishSceneCallback, 0x4D0);
RW_LAYOUT(SceneManager, finishSceneCallback, 0x4E8);
RW_LAYOUT(SceneManager, function500, 0x500);
RW_LAYOUT(SceneManager, chatButtonCallback, 0x518);
RW_LAYOUT(SceneManager, onChangeProfileCallback, 0x530);
RW_LAYOUT(SceneManager, onFinishLoading, 0x548);
RW_LAYOUT(SceneManager, backgroundName, 0x560);
RW_LAYOUT(SceneManager, background, 0x56C);
RW_LAYOUT(SceneManager, apShown, 0x570);
RW_LAYOUT(SceneManager, apMaxShown, 0x574);
RW_LAYOUT(SceneManager, apSeconds, 0x578);
RW_LAYOUT(SceneManager, pushToken, 0x57C);
RW_LAYOUT(SceneManager, flag588, 0x588);
RW_LAYOUT(SceneManager, popupCanClose, 0x58C);
RW_LAYOUT(SceneManager, popupOnClose, 0x598);
RW_LAYOUT(SceneManager, popupFlags, 0x5A4);
RW_LAYOUT(SceneManager, onBackKey, 0x5B0);
RW_LAYOUT(SceneManager, barButtonCopy, 0x5C8);
RW_LAYOUT(SceneManager, faceImageUrls, 0x5CC);
RW_SIZE(SceneManager, 0x5D8);
