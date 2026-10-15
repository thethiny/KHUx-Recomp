// SceneUtil: static UI / scene helpers shared by every scene (node lookup, layout against the visible area,
// in/out animations, text styling, dialogs, tutorial flow, master-data lookups for icons and labels).
// It has no instance: every member is a static function. Guest objects (cocos2d nodes, actions, std::function
// and std::string temporaries) are passed around as 32-bit guest addresses.
#pragma once

#include "SceneCommon.hpp"

struct SceneUtil {
    using Addr = uint32_t;     // a guest pointer (cocos2d::Node*, std::string*, std::function*...)

    // ---- nodes and layout (SceneUtilNodes.cpp)
    // Depth-first search for a node called `name` (a std::string) below and including `root`.
    static Addr searchNodeByName(Addr root, Addr name);
    // Load a cocostudio scene file, centre it on the visible area and pin its LeftUI / RightUI groups.
    static Addr createNodeWithSceneFile(Addr fileName);
    // Shift a node by half the extra visible width (screens wider than the 960 design width).
    static void leftAdsorptionNode(Addr node);
    static void rightAdsorptionNode(Addr node);
    static float getSideMargin();             // (visibleWidth - 960) / 2
    static float getSideHeightMargin();       // (visibleHeight - 640) / 2
    // A black half-transparent full-screen sprite that swallows touches (modal background).
    static Addr createShadowSprite();
    static void backBtnSetPosY(Addr button);
    static void setCascadeOpacityEnabled(Addr node, uint32_t enabled);

    // ---- animations (SceneUtilNodes.cpp); `done` is a std::function<void()>* run at the end
    static Addr createTopInAnimation(Addr node, float offset, float duration, Addr done);
    static Addr createTopOutAnimation(Addr node, float offset, float duration, Addr done);
    static Addr createBottomInAnimation(Addr node, float offset, float duration, Addr done);
    static Addr createBottomOutAnimation(Addr node, float offset, float duration, Addr done);
    static Addr createLeftInAnimation(Addr node, float offset, float duration, Addr done);
    static Addr createLeftOutAnimation(Addr node, float offset, float duration, Addr done);
    static Addr createRightInAnimation(Addr node, float offset, float duration, Addr done);
    static Addr createRightOutAnimation(Addr node, float offset, float duration, Addr done);
    static Addr createTapAnimation(Addr node, Addr done);
    static Addr createOpenPopupAnimation(Addr node, Addr done);
    static Addr createClosePopupAnimation(Addr node, Addr done);
    static Addr createFadeInAnimation(Addr node, Addr done);
    static Addr createFadeOutAnimation(Addr node, Addr done);

    // ---- text styling (SceneUtilNodes.cpp)
    static void setShadow(Addr text);
    static void setUserTitleOutline(Addr text);
    static void setBlackOutline(Addr text);
    // Replace a ui::Text by a CustomTextView (rich text) with the same placement and font; returns the view.
    static Addr textToCustomTextView(Addr text, Addr name);

    // ---- text input (SceneUtilInput.cpp)
    static Addr textFieldToEditBox(Addr editBoxRef, Addr labelRef, Addr textField, Addr placeholder,
                                   Addr initialText, Addr fontFile, Addr areaSize, Addr delegate, Addr editingFlag,
                                   Addr layerOut, int32_t maxLength);
    static Addr textFieldToUiText(Addr textField, Addr text, Addr fontName);
    static void editBoxEditingDidEnd(Addr layerRef, Addr label, Addr text);

    // ---- dialogs (SceneUtilDialogs.cpp); message = std::string*, callback = std::function<void(bool)>*
    static void openOkDialog4Line(Addr message, Addr callback, Addr okLabel);
    static void openOkDialog4LineSetLayer(Addr layer, Addr message, Addr callback, Addr okLabel);
    static void openOkCancelDialog4Line(Addr message, Addr callback, Addr okLabel, Addr cancelLabel);
    static void openOkDialog8Line(Addr message, Addr callback, Addr okLabel);
    static void openOkCancelDialog8Line(Addr message, Addr callback, Addr okLabel, Addr cancelLabel);
    static void openChargeApDialog(Addr callback);       // std::function<void(bool,bool)>*
    static void openChargeApPopup(Addr callback);
    static Addr settingTitleLabel(Addr root, Addr name, int32_t textId);
    static bool hasMagicStone(int32_t count);
    static void openContinueDialog(Addr callback);       // std::function<void(bool,bool)>*
    static void openContinuPopup(Addr callback);

    // ---- medal box (SceneUtilMedal.cpp)
    static bool isMaxMedalSlot();
    static void openMedalMaxUpResultDialog(int32_t spent, int32_t added, int32_t newLimit, Addr callback);
    static void openMedalOverDialog(uint32_t altMessage, uint32_t backScene, Addr callback);

    // ---- panels and formatting (SceneUtilPanels.cpp); keyblade / medal = std::shared_ptr<...>*
    static void updateKeyBladePanel(Addr panel, Addr keyblade, uint32_t compare, int32_t keybladeId);
    static void updateMedalRarePanel(Addr panel, int32_t rare, int32_t align, float scale, uint32_t small,
                                     int32_t maxStars, int32_t zOrder);
    static void getMoneyStr(Addr out, uint64_t value);   // out: the returned std::string (sret)
    static int32_t getMedalMaxRare(Addr medal);

    // ---- text colours, effects, number strings (SceneUtilStyle.cpp); out = returned std::string (sret)
    static void setParameterUpEffect(Addr text);
    static void setStatusChangeColor(Addr text);
    static void setStatusCountStopColor(Addr text);
    static void setNormalColor(Addr text);
    static void setNeedMedalColor(Addr text);
    static void setWarningColor(Addr text);
    static void setNormalTextFieldColor(Addr field);
    static void setFocusTextFieldColor(Addr field);
    static void setButtonEnabled(Addr button, Addr label, uint32_t enabled, uint32_t style);
    static void setImageButtonEnabled(Addr button, Addr label, uint32_t enabled, uint32_t style);   // overload
    static void runTicker(Addr node, float width, float startX, float restX);
    static void showSupportMedalEffect(Addr widget, uint32_t small);
    static void hideSupportMedalEffect(Addr widget, uint32_t small);
    static void setAdditionalKerningForAtlasLabel(Addr atlas, int32_t kerning);
    static void numberToAtlasString(Addr out, int32_t value);
    static void numberToAtlasString64(Addr out, uint64_t value);     // the unsigned long long overload
    static void numberToPlaceString(Addr out, int32_t value);

    // ---- queries, gauges, image paths, user titles (SceneUtilQueries.cpp)
    static bool isLoggedInFacebook();
    static uint32_t getLinkPlatformType();
    static bool isMaxAp();
    static bool isMaxApOk(int32_t ap);
    static bool isApOk(uint32_t ap);
    static bool isOverMedalSlot(int32_t adding);
    static bool isOverMaxMedalSlot(int32_t adding);
    static int32_t getStageResumptionStatus();
    static void clearMedalNewMark();
    static int32_t isBenefits();
    static int32_t getCampaignPoint(int32_t base, int32_t kind, int32_t id, Addr ids, int32_t target,
                                    uint32_t roundUp);
    static void calcLuxGaugePer(int32_t rank, uint64_t lux, Addr callback);       // function<void(float, u64)>*
    static void calcSkillExpGaugePer(Addr skill, uint64_t exp, Addr callback);
    static void createUnionImagePath(Addr out, int32_t unionId, uint32_t large, uint32_t selected);
    static void createUserTitleImagePath(Addr out, int32_t id);
    static void createPartyRankImagePath(Addr out, int32_t rank);
    static void createPartyTitleRankImagePath(Addr out, int32_t rank);
    static void updateUserTitlePanel(Addr panel, int32_t firstWord, int32_t secondWord, int32_t plateId);
    static void updateUserTitlePanelDetail(Addr panel, Addr detail);      // overload: shared_ptr<UserDetail>
    static void updateUserTitlePanelSupport(Addr panel, Addr user);       // overload: SupportUser
    static Addr settingTitleLabelText(Addr root, Addr name, Addr text);  // overload: text as std::string

    // ---- deck (SceneUtilDeck.cpp); keyblade = shared_ptr<UserKeyBladeData>*, medals = vector<shared_ptr<...>>*
    static void calcDeckStatus(Addr keyblade, Addr medals, Addr out, Addr medalList);
    static void updateDeckStauts(Addr panel, Addr keyblade, Addr medals);

    // ---- flow (SceneUtilFlow.cpp)
    static void moveToMyPageOrResumeStage();
    static void callPrepareMypageAPIs(Addr callback);     // std::function<void(bool)>*
    static void addBackKeyCallback(Addr node, Addr callback);
    static void callUpdateLockMedalAPI(Addr callback);    // std::function<void(bool)>*
    static void setBossTypeImage(Addr image, int32_t id, uint32_t raid);

    // ---- small popups, checks, screenshots (SceneUtilMisc.cpp)
    static void openPartyInfoPopup(uint64_t party, uint32_t flag, Addr callback);
    static void openPartyInfoRequestPopup(uint64_t party, uint32_t flag, Addr callback);
    static void openPartyInfoJoinAlertPopup(uint32_t flag, Addr callback);
    static void openAvatarInfoPopup(uint64_t user, uint32_t flag, Addr callback, Addr canClose);
    static void openWebViewInfoPopupDefault(Addr title, Addr url, Addr onClose, Addr extra);   // overload
    static bool splitSpaceCheck(Addr text);
    static Addr generateScreenShotTexture(Addr node);
    static void updateMedalPossessionLabel(Addr node);

    // ---- tutorial (SceneUtilTutorial.cpp); callback = std::function<void()>*, result = returned function
    static int32_t getNexTutorialphase();
    static int32_t getNowTutorialphase();
    static int32_t BiId2TutorialId(int32_t biId);
    static int32_t TutorialId2BiId(uint32_t id);
    static void updateTutorialStatus(int32_t phase, uint64_t flags, uint32_t skipped, Addr callback,
                                     uint32_t blockLoading);
    static void updateTutorialPhase(int32_t phase, Addr callback, uint32_t skip, uint32_t blockLoading);
    static void CompleteTutorialPhase(Addr callback, uint32_t skip, uint32_t blockLoading);
    static void updateTutorialpopupFlag(uint64_t flags, Addr callback, uint32_t blockLoading);
    static void ClearTutorialPopupIDs(uint32_t id, Addr callback);
    static void freeTutorialConvertData(Addr callback);
    static int32_t tutorial_check_sub_100(int32_t id);
    static int32_t tutorial_check_sub_200(int32_t id);
    static int32_t tutorial_check_sub_210(int32_t id);
    static int32_t tutorial_check_sub_220(int32_t id);
    static int32_t tutorial_check_sub_300(int32_t id);
    static int32_t tutorial_check_sub_400(int32_t id);
    static int32_t tutorial_check_sub_410(int32_t id);
    static int32_t tutorial_check_sub_500(int32_t id);
    static int32_t tutorial_check_sub_600(int32_t id);
    static int32_t tutorial_check_sub_700(int32_t id);
    static int32_t tutorial_check_sub_710(int32_t id);
    static int32_t dispTutorialCheck(int32_t id);
    static void openTutorialPopup(Addr result, uint32_t id, Addr callback, Addr target1, Addr target2, Addr target3,
                                  uint32_t force);
    static void TutorialMask(Addr result, uint32_t id, Addr callback, Addr target1, Addr target2, Addr target3,
                             uint32_t force);
    static void NormalTutorialPopup(Addr result, int32_t id, Addr callback, uint32_t unused);

    // ---- declarations owned by helper files (one .inc per topic, included here)
#include "SceneUtil_MedalInfo.inc"
#include "SceneUtil_MyPage.inc"
#include "SceneUtil_Popups2.inc"
#include "SceneUtil_Unnamed.inc"

    // A cocos2d::ui::Layout subclass laid over the scene by TutorialMask (multiple inheritance: three vtables).
    struct TutorialMaskWidget {
        static constexpr uint32_t SIZE = 0x500;
        static constexpr uint32_t ACTION = 0x4FC;
        rw::Field<uint32_t> vtable;            // +0x000
        const uint8_t pad_004[0x27C];                // cocos2d::ui::Layout
        rw::Field<uint32_t> vtable2;           // +0x280
        const uint8_t pad_284[0xCC];
        rw::Field<uint32_t> vtable3;           // +0x350
        const uint8_t pad_354[0x1A8];
        rw::Field<uint32_t> action;            // +0x4FC cocostudio::ActionObject* of the mask animation

        static Addr create();
        static TutorialMaskWidget *destroy(TutorialMaskWidget *self);       // ~TutorialMaskWidget (D2)
        static void destroyDelete(TutorialMaskWidget *self);                // deleting destructor (D0)
        static bool init(TutorialMaskWidget *self);
    };
};
RW_LAYOUT(SceneUtil::TutorialMaskWidget, vtable2, 0x280);
RW_LAYOUT(SceneUtil::TutorialMaskWidget, vtable3, 0x350);
RW_LAYOUT(SceneUtil::TutorialMaskWidget, action, 0x4FC);
RW_SIZE(SceneUtil::TutorialMaskWidget, 0x500);
