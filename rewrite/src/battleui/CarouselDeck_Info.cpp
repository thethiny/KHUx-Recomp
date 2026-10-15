// CarouselDeck: the deck info window (tactical boxes), the keyblade windows with the special attack info, the
// burst / skill labels and icons.
#include "CarouselDeck.hpp"

using namespace scene;
using namespace cdeck;

namespace {
constexpr uint32_t STR_Avatar_Burst_Num = 0xE01B5E, STR_BurstName_Label = 0xDF8ACA, STR_Cost = 0xDF8AAA;
constexpr uint32_t STR_Cost_Label = 0xE01BD6, STR_BurstInfo_Label = 0xE01BC6, STR_TickerField = 0xE01ACD;
constexpr uint32_t STR_Medal = 0xDFD05F, STR_Ability_Label = 0xDF8824, STR_Skill_Label = 0xE01B6F;
constexpr uint32_t STR_Txt_Ticker_Ability = 0xDF8B29, STR_Txt_Ticker_Skill = 0xDF8C82;
constexpr uint32_t STR_attack_Label = 0xE01B51, STR_Ability_Type_Label = 0xDF8AE2;
constexpr uint32_t STR_MultiplyNum_Label = 0xDF8B01, STR_FMT_D = 0xDF86DE, STR_FMT_SS = 0xDF8BBA;
constexpr uint32_t STR_TacticalPanel = 0xE01B43, STR_CurrentTurn = 0xE01B2C, STR_Current_EFF = 0xE01B20;

constexpr int32_t TEXT_COST = 100400007, TEXT_ABILITY = 100400001, TEXT_SKILL = 100450001;
constexpr int32_t TEXT_NO_ABILITY = 101200019, TEXT_NO_SKILL = 101200020;
constexpr int32_t TEXT_POWER = 101200031, TEXT_POWER_FIXED = 101200032;     // "attack x" / fixed damage
constexpr int32_t TEXT_MULTIPLY = 101200033, TEXT_MULTIPLY_SECOND = 101200035;
constexpr int32_t TEXT_TURN = 103100011;                                     // "turn %d"

constexpr uint32_t VT_LAMBDA_TICKER = 0xF5D1FC;      // setAbilityInfo $_8: setTickerAction
constexpr uint32_t VT_LAMBDA_DECK_INFO = 0xF5D2AC;   // setDeckInfoWindow $_5: one tactical box per medal
constexpr uint32_t TEXT_TICKER_CALLBACK = 0x210;     // the burst info label's std::function<void()> (ticker restart)
constexpr uint32_t APIManager_userSkills = 0xDC;     // shared_ptr<UserSkill>: SafeBuffer of 40-byte records
constexpr uint32_t STAGE_STATE_TURN = 0x4250;        // StageState data: the current turn

// The burst's special attack power (permyriad) for a medal of special attack level `level` (1..5; 0 = base).
// The original reads the five levels into a stack array and indexes it with level - 1 unchecked.
int32_t burstPower(uint32_t burst, int32_t level, uint32_t base, uint32_t levels) {
    if (level) return safe32(burst, levels + 4u * uint32_t(level - 1));
    return safe32(burst, base);
}
}  // namespace

// ------------------------------------------------------------------------------------------- keyblade window
// The special attack of medal `medalId` in a slot's ability info (cost, target icon) and keyblade window (name,
// cost, description ticker); both are hidden for a medal without one.
void CarouselDeck::setAbilityInfo(CarouselDeck *self, uint32_t ability, uint32_t window, int32_t medalId) {
    Frame f;
    uint32_t medal = masterRow(f, Medal_create, uint32_t(medalId));
    uint32_t m = ld32(medal);
    if (m && safe32(m, MEDAL_BURST_KIND) == 1) {
        uint32_t burst = masterRow(f, Burst_create, uint32_t(safe32(m, MEDAL_BURST_ID)));
        uint32_t b = ld32(burst);
        int32_t cost = safe32(b, BURST_COST);
        int32_t target = safe32(b, BURST_TARGET);
        uint32_t num = find(ability, STR_Avatar_Burst_Num);
        uint32_t costText = ld32(COST_STRINGS + 4u * uint32_t(cost));
        setAtlasC(num, costText);
        vcall(ability, node::setTag, {ability, uint32_t(cost)});
        controlTargetIcon(self, ability, target);
        uint32_t name = findShadowed(window, STR_BurstName_Label);
        {
            Frame g;
            setTextC(name, safeBytes(g, b, BURST_NAME, BURST_NAME_SIZE));
        }
        setTextId(findShadowed(window, STR_Cost), TEXT_COST);
        setTextC(findShadowed(window, STR_Cost_Label), costText);
        uint32_t info = findShadowed(window, STR_BurstInfo_Label);
        {
            Frame g;
            setTextC(info, safeBytes(g, b, BURST_INFO, BURST_INFO_SIZE));
        }
        {
            Frame g;
            uint32_t cb = newFunction(g, VT_LAMBDA_TICKER, {rw::guest_addr(self)});
            call(function_vv_assign, {info + TEXT_TICKER_CALLBACK, cb});
            functionDestroy(cb);
        }
        uint32_t field = find(window, STR_TickerField);
        vcall(field, VT_LAYOUT_SET_CLIPPING, {field, 1});
        if (self->tickerTextX == 0.0f) self->tickerTextX = bitsf(vcall(info, node::getPositionX, {info}));
        if (self->tickerFieldWidth == 0.0f) self->tickerFieldWidth = ldf(call(Widget_getCustomSize, {field}));
        controlTargetIcon(self, window, target);
        sharedRelease(burst);
    } else {
        setVisible(ability, 0);
        setVisible(window, 0);
    }
    sharedRelease(medal);
}
RW_FUNCTION(0x5CFB8C, "CarouselDeck::setAbilityInfo", CarouselDeck::setAbilityInfo);

// ------------------------------------------------------------------------------------------- tactical boxes
// A tactical box from master data: medal image, ability (special attack) and skill labels; a friend medal also
// shows its skill.
void CarouselDeck::setDeckMedalInfoPanel(CarouselDeck *self, uint32_t box, int32_t medalId, uint32_t friendMedal,
                                         int32_t skillId) {
    Frame f;
    uint32_t medal = masterRow(f, Medal_create, uint32_t(medalId));
    if (uint32_t mark = find(box, STR_FriendlMedalUI)) setVisible(mark, friendMedal);
    if (uint32_t image = find(box, STR_Medal)) {
        Frame g;
        uint32_t s = newStr(g, stringBuffer(g, {STR_FMT_MEDAL_S, uint32_t(safe32(ld32(medal), MEDAL_IMAGE))}));
        call(ImageView_loadTexture, {image, s, 0});
        strFree(s);
    }
    setTextId(findShadowed(box, STR_Ability_Label), TEXT_ABILITY);
    setTextId(findShadowed(box, STR_Skill_Label), TEXT_SKILL);
    uint32_t ticker = findShadowed(box, STR_Txt_Ticker_Ability);
    uint32_t num = find(box, STR_Avatar_Burst_Num);
    uint32_t m = ld32(medal);
    if (safe32(m, MEDAL_BURST_KIND) == 1) {
        uint32_t burst = masterRow(f, Burst_create, uint32_t(safe32(m, MEDAL_BURST_ID)));
        uint32_t b = ld32(burst);
        setAtlasC(num, ld32(COST_STRINGS + 4u * uint32_t(safe32(b, BURST_COST))));
        controlTargetIcon(self, box, safe32(b, BURST_TARGET));
        {
            Frame g;
            setTextC(ticker, safeBytes(g, b, BURST_INFO, BURST_INFO_SIZE));
        }
        sharedRelease(burst);
    } else {
        setVisible(num, 0);
        controlTargetIcon(self, box, -1);
        setTextId(ticker, TEXT_NO_ABILITY);
        setGray(ticker, 0x80);
    }
    if (friendMedal == 1) setDeckMedalInfoSkill(self, box, skillId);
    sharedRelease(medal);
}
RW_FUNCTION(0x5D017C, "CarouselDeck::setDeckMedalInfoPanel(Widget*, int, bool, int)",
            CarouselDeck::setDeckMedalInfoPanel);

// The skill line of a tactical box (icon and name); no skill: the "none" text in grey.
void CarouselDeck::setDeckMedalInfoSkill(CarouselDeck *self, uint32_t box, int32_t skillId) {
    Frame f;
    uint32_t skill = f.alloc(8);
    if (skillId) {
        call(Skill_create, {skill, uint32_t(skillId)});
    } else {
        st32(skill, 0);
        st32(skill + 4, 0);
    }
    uint32_t ticker = findShadowed(box, STR_Txt_Ticker_Skill);
    if (uint32_t s = ld32(skill)) {
        controlSkillIcon(self, box, safe32(s, SKILL_ICON));
        Frame g;
        setTextC(ticker, safeBytes(g, ld32(skill), SKILL_NAME, SKILL_NAME_SIZE));
    } else {
        controlSkillIcon(self, box, -1);
        setTextId(ticker, TEXT_NO_SKILL);
        setGray(ticker, 0x80);
    }
    sharedRelease(skill);
}
RW_FUNCTION(0x5D1F4C, "CarouselDeck::setDeckMedalInfoSkill", CarouselDeck::setDeckMedalInfoSkill);

// A tactical box from a deck medal (MedalDeck::MedalInfo): attack, special attack power; the user's own medals
// also show the skill of the user medal (looked up by the record's user medal id in the user's skills).
void CarouselDeck::setDeckMedalInfoPanelInfo(CarouselDeck *self, uint32_t box, uint32_t info) {
    const uint32_t data = info + INFO_DATA;
    uint32_t attack = findShadowed(box, STR_attack_Label);
    {
        Frame f;
        setTextC(attack, stringBuffer(f, {STR_FMT_D, uint32_t(safe32(data, UMEDAL_ATTACK))}));
    }
    uint32_t typeLabel = find(box, STR_Ability_Type_Label);
    uint32_t valueLabel = find(box, STR_MultiplyNum_Label);
    setBurstPower(self, info, typeLabel, valueLabel);
    uint32_t mark = find(box, STR_FriendlMedalUI);
    if (!mark || vcall(mark, node::isVisible, {mark})) return;
    uint32_t api = call(fn::APIManager_getInstance, {});
    uint32_t ctrl = ld32(api + APIManager_userSkills + 4), skills = ld32(api + APIManager_userSkills);
    if (ctrl) call(fn::shared_add, {ctrl});
    int32_t skill = 0;
    {
        Frame f;
        uint32_t key = f.alloc(8);
        st32(key, uint32_t(safe32(data, UMEDAL_SUPPORT_USER)));
        st32(key + 4, uint32_t(safe32(data, UMEDAL_SUPPORT_USER + 4)));
        uint32_t node = call(MedalIndex_find, {skills + UserMedal_index, key});
        if (node) {
            int32_t slot = int32_t(ld32(node + 0x10));
            if (slot >= 0) skill = safe32(skills, 40u * uint32_t(slot) + 8);
        }
    }
    setDeckMedalInfoSkill(self, box, skill);
    if (ctrl) call(fn::shared_release, {ctrl});
}
RW_FUNCTION(0x5D212C, "CarouselDeck::setDeckMedalInfoPanel(Widget*, const MedalDeck::MedalInfo&)",
            CarouselDeck::setDeckMedalInfoPanelInfo);

// The special attack power labels: fixed damage (type 2), a multiplier (flag 1), or "x / y" for two-hit specials.
// Powers are permyriad; the user medal's guilt bonus (permyriad over 10000) multiplies them.
void CarouselDeck::setBurstPower(CarouselDeck *, uint32_t info, uint32_t typeLabel, uint32_t valueLabel) {
    uint32_t m = ld32(info + INFO_MEDAL), ctrl = ld32(info + INFO_MEDAL + 4);
    if (ctrl) call(fn::shared_add, {ctrl});
    if (safe32(m, MEDAL_BURST_KIND) == 1) {
        Frame f;
        uint32_t burst = masterRow(f, Burst_create, uint32_t(safe32(m, MEDAL_BURST_ID)));
        const uint32_t data = info + INFO_DATA;
        int32_t level = safe32(data, UMEDAL_BURST_LEVEL);
        uint32_t guilt = uint32_t(safe32(data, UMEDAL_GUILT));
        float bonus = float(guilt + 10000u) / 10000.0f;
        SceneUtil::setShadow(typeLabel);
        SceneUtil::setShadow(valueLabel);
        uint32_t b = ld32(burst);
        float p = float(burstPower(b, level, BURST_POWER, BURST_POWER_LV)) / 10000.0f;
        float power = guilt ? bonus * p : p;
        if (safe32(b, BURST_TYPE) == 2) {
            setTextId(typeLabel, TEXT_POWER_FIXED);
            Frame g;
            setTextC(valueLabel, stringBuffer(g, {STR_FMT_D, uint32_t(bt::f2iz(power))}));
        } else {
            setTextId(typeLabel, TEXT_POWER);
            if (safe32(ld32(burst), BURST_FLAG) == 1) {
                Frame g;
                uint32_t td = textData(g, TEXT_MULTIPLY);
                uint64_t d = bt::bits(double(power));
                uint32_t buf = stringBuffer(g, {textDataStr(td), uint32_t(d), uint32_t(d >> 32)});
                setTextC(valueLabel, buf);
                sharedRelease(td);
            } else {
                float p2 = float(burstPower(ld32(burst), level, BURST_POWER2, BURST_POWER2_LV)) / 10000.0f;
                float power2 = guilt ? bonus * p2 : p2;
                Frame g;
                uint32_t td1 = textData(g, TEXT_MULTIPLY);
                uint32_t first = textDataStr(td1);
                uint32_t td2 = textData(g, TEXT_MULTIPLY_SECOND);
                uint32_t fmt = stringBuffer(g, {STR_FMT_SS, first, textDataStr(td2)});
                sharedRelease(td2);
                sharedRelease(td1);
                uint64_t d2 = bt::bits(double(power2)), d1 = bt::bits(double(power));
                uint32_t buf = stringBuffer(g, {fmt, uint32_t(d1), uint32_t(d1 >> 32), uint32_t(d2), uint32_t(d2 >> 32)});
                setTextC(valueLabel, buf);
            }
        }
        sharedRelease(burst);
    } else {
        setVisible(typeLabel, 0);
        setVisible(valueLabel, 0);
    }
    if (ctrl) call(fn::shared_release, {ctrl});
}
RW_FUNCTION(0x5D23FC, "CarouselDeck::setBurstPower", CarouselDeck::setBurstPower);

// Show only the target icon of the special attack's target type (1..3; -1: none).
void CarouselDeck::controlTargetIcon(CarouselDeck *, uint32_t widget, int32_t target) {
    for (uint32_t k = 0; k < 5; k++) {
        if ((k | 4) == 4) continue;
        uint32_t icon = find(widget, ld32(TARGET_ICON_NAMES + 4 * k));
        if (icon) setVisible(icon, uint32_t(target) == k ? 1 : 0);
    }
}
RW_FUNCTION(0x5D1B60, "CarouselDeck::controlTargetIcon", CarouselDeck::controlTargetIcon);

// Show only the skill icon `icon` (1..5; -1: none).
void CarouselDeck::controlSkillIcon(CarouselDeck *, uint32_t widget, int32_t icon) {
    for (uint32_t k = 1; k < 6; k++) {
        uint32_t n = find(widget, ld32(SKILL_ICON_NAMES + 4 * k));
        if (n) setVisible(n, k == uint32_t(icon) ? 1 : 0);
    }
}
RW_FUNCTION(0x5D1C3C, "CarouselDeck::controlSkillIcon", CarouselDeck::controlSkillIcon);

// A burst description wider than its field scrolls: from the start, wait 1 s, move left until it left the field
// (at a third of the field width per second), jump to the right edge, slide back in 0.5 s; repeated forever.
void CarouselDeck::setTickerAction(CarouselDeck *self) {
    uint32_t infoNode = self->keybladeInfo;
    if (!infoNode) return;
    uint32_t label = find(infoNode, STR_BurstInfo_Label);
    if (!label || vcall(label, node::isVisible, {label}) != 1) return;
    uint32_t y = vcall(label, node::getPositionY, {label});
    float width = ldf(call(Widget_getCustomSize, {label}));
    if (!(self->tickerFieldWidth < width)) return;
    float width2 = ldf(call(Widget_getCustomSize, {label}));
    float field = self->tickerFieldWidth;
    Frame f;
    uint32_t v1 = f.alloc(8), v2 = f.alloc(8), v3 = f.alloc(8), v4 = f.alloc(8);
    call(Vec2_ctor_xy, {v1, scene::fbits(self->tickerTextX), y});
    float duration = width2 / (field / 3.0f);
    uint32_t place1 = call(Place_create, {v1});
    uint32_t delay = call(fn::DelayTime_create, {fbits(1.0f)});
    call(Vec2_ctor_xy, {v2, fbits(-width), y});
    uint32_t move1 = call(fn::MoveTo_create, {fbits(duration), v2});
    call(Vec2_ctor_xy, {v3, scene::fbits(self->tickerFieldWidth), y});
    uint32_t place2 = call(Place_create, {v3});
    call(Vec2_ctor_xy, {v4, scene::fbits(self->tickerTextX), y});
    uint32_t move2 = call(fn::MoveTo_create, {fbits(0.5f), v4});
    uint32_t seq = call(fn::Sequence_create, {place1, delay, move1, place2, move2, 0});
    uint32_t forever = call(RepeatForever_create, {seq});
    call(Vec2_dtor, {v4});
    call(Vec2_dtor, {v3});
    call(Vec2_dtor, {v2});
    call(Vec2_dtor, {v1});
    call(fn::Node_runAction, {label, forever});
}
RW_FUNCTION(0x5D1D18, "CarouselDeck::setTickerAction", CarouselDeck::setTickerAction);

// ------------------------------------------------------------------------------------------- deck info window
// One tactical box per deck medal ($_5 fills them in order and keeps each medal's guilt value).
void CarouselDeck::setDeckInfoWindow(CarouselDeck *self, uint32_t medalDeck) {
    Frame f;
    uint32_t counter = f.alloc(4);
    st32(counter, 0);
    uint32_t cb = newFunction(f, VT_LAMBDA_DECK_INFO, {counter, rw::guest_addr(self)});
    call(MedalDeck_forEachMedal, {medalDeck, cb});
    functionDestroy(cb);
}
RW_FUNCTION(0x5D07D0, "CarouselDeck::setDeckInfoWindow", CarouselDeck::setDeckInfoWindow);

// The deck enters: Deck_in plays, the deck size label is set, the window laid out and the costs coloured.
void CarouselDeck::admission(CarouselDeck *self) {
    uint32_t anim = vcall(self->deckInArmature, VT_GET_ANIMATION, {self->deckInArmature});
    vcall(anim, VT_ANIM_GOTO_AND_PLAY, {anim, 0});
    uint32_t text = ld32(COST_STRINGS + 4u * uint32_t(self->deckSize) - 4);
    setAtlasC(self->deckLabel01, text);
    setDeckInfoWindowLayout(self);
    setActiveMedalBurstCost(self);
}
RW_FUNCTION(0x5D0874, "CarouselDeck::admission", CarouselDeck::admission);

// The tactical boxes side by side in the panel's MedalPanel; the panel widens beyond four medals.
void CarouselDeck::setDeckInfoWindowLayout(CarouselDeck *self) {
    uint32_t panel = find(self->tacticalPanel, STR_TacticalPanel);
    Frame f;
    uint32_t panelSize = f.alloc(8), boxSize = f.alloc(8);
    call(Size_ctor_copy, {panelSize, call(Widget_getCustomSize, {panel})});
    call(Size_ctor_copy, {boxSize, call(Widget_getCustomSize, {self->tacticalBoxes[0]})});
    int32_t n = self->deckSize;
    if (n >= 5) {
        stf(panelSize, ldf(panelSize) + float(n - 4) * ldf(boxSize));
        vcall(panel, VT_WIDGET_SET_SIZE, {panel, panelSize});
    }
    uint32_t medals = find(panel, STR_MedalPanel);
    if (n < 2) return;
    for (int32_t k = 0; k < n - 1; k++) {
        uint32_t box = self->tacticalBoxes[k];
        Frame g;
        uint32_t pos = g.alloc(8);
        call(Vec2_ctor_xy, {pos, fbits(float(k) * ldf(boxSize)), 0});
        vcall(box, node::setPosition, {box, pos});
        vcall(medals, node::addChild, {medals, box});
        call(Vec2_dtor, {pos});
    }
}
RW_FUNCTION(0x5D0954, "CarouselDeck::setDeckInfoWindowLayout", CarouselDeck::setDeckInfoWindowLayout);

// Each ability info is white when the special gauge (StageState CP) covers its cost (tag x 10000), else grey.
void CarouselDeck::setActiveMedalBurstCost(CarouselDeck *self) {
    for (auto &w : self->abilityInfoWidgets) {
        uint32_t widget = w;
        uint32_t cost = vcall(widget, node::getTag, {widget});
        uint32_t app = call(fn::AppState_getInstance, {});
        uint32_t cp = uint32_t(safe32(ld32(app + 0x20), 12));
        setGray(widget, 10000u * cost <= cp ? 0xFF : 0x80);
    }
}
RW_FUNCTION(0x5D0B18, "CarouselDeck::setActiveMedalBurstCost", CarouselDeck::setActiveMedalBurstCost);

// The deck info window: boxes before the current medal greyed, the turn counter, the current-medal effect.
void CarouselDeck::setVisibleDeckInfoWindow(CarouselDeck *self, uint32_t visible) {
    if (visible != 1) {
        removeArmature(self, self->currentEffect);
        setVisible(self->tacticalPanel, visible);
        return;
    }
    uint32_t turn = find(self->tacticalPanel, STR_CurrentTurn);
    setVisible(turn, self->inBattle);
    int32_t n = self->deckSize;
    uint32_t current = 0;
    if (n >= 2) {
        int32_t index = self->currentIndex;
        uint32_t prev = 0;
        for (int32_t k = 0; k < n - 1; k++) {
            uint32_t box = self->tacticalBoxes[k];
            setGray(box, k >= index ? 0xFF : 0x80);
            index = self->currentIndex;
            current = k == index ? box : prev;
            prev = current;
        }
    }
    if (self->inBattle) {
        {
            Frame f;
            uint32_t td = textData(f, TEXT_TURN);
            uint32_t fmt = textDataStr(td);
            uint32_t app = call(fn::AppState_getInstance, {});
            uint32_t t = uint32_t(safe32(ld32(app + 0x20), STAGE_STATE_TURN));
            uint32_t buf = stringBuffer(f, {fmt, (t >> 3) > 0x7C ? 999u : t});
            setTextC(turn, buf);
            sharedRelease(td);
        }
        SceneUtil::setShadow(turn);
        if (current) addArmature(self, self->currentEffect, find(current, STR_Current_EFF), 0);
    } else {
        removeArmature(self, self->currentEffect);
    }
    setVisible(self->tacticalPanel, 1);
}
RW_FUNCTION(0x5D14E8, "CarouselDeck::setVisibleDeckInfoWindow", CarouselDeck::setVisibleDeckInfoWindow);
