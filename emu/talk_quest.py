"""Talk to a quest map's NPC (dev helper): quest detail screen -> pick quest QUEST from the strip -> start -> warp next
to the map's MapNonPlayableCharacter (control 'warp'), tap it, tap through the talk (scene mode 5) -> insta-win.

KHUX_INSTAWIN=1 python -m recomp.emu.talk_quest [QUEST]   (QUEST = the strip number, default 220 = stage 7200)

Stages with NPCs: the type-3 records of the map files' NPC section (header +48); e.g. quest 220 "The Missing
Handkerchief" (stage 7200, NPC at 2086,1898 in area 0). Native exe only (warp, insta-win).
"""
import os
import sys
import time

from recomp.emu.ctl import get
from recomp.emu.campaign import to_quest_select
from recomp.emu.play_quest import begin, button, tap_w, widgets, ack_popups, instawin, screen
from recomp.emu.auto_battle import scene_mode

quest = sys.argv[1] if len(sys.argv) > 1 else '220'
print('to quest select:', to_quest_select())
num = next((w for w in widgets() if w['name'] == 'Txt_Stage_Num_Label' and w.get('text') == quest), None)
if not num: print('quest', quest, 'not on the strip:', [w.get('text') for w in widgets() if w['name'] == 'Txt_Stage_Num_Label']); sys.exit(1)
tap_w(num); time.sleep(3)
# start: the same sequence as play_quest.main
from recomp.emu.play_quest import wait_api
from recomp.emu.ctl import is_story, advance_story, STORY_SCENES
since = get('state')['events']
ack_popups(); begin()
if not wait_api(102, since, 30): ack_popups(); begin(); wait_api(102, since, 30)
time.sleep(3)
for _ in range(10):
    b = button('Confirm')
    if b: tap_w(b); break
    time.sleep(1)
time.sleep(3); ack_popups(); get('tap', x=850, y=160); time.sleep(2)
since = get('state')['events']
for _ in range(5):
    b = button('Start')
    if b: tap_w(b)
    if wait_api((103,), since, 15): break
else: sys.exit('no 103 after Start')
for _ in range(150):
    if screen() == 'SceneActionMap': break
    if is_story(screen()): advance_story()
    time.sleep(2)
t0 = time.time(); talked = False
while time.time() - t0 < 300:
    allw = get('ui', all=1).get('widgets', [])
    mode = scene_mode(allw)
    if mode == 5:
        talked = True
        get('tap', x=568, y=500); time.sleep(float(os.environ.get('TALK_TAP_S', '0.4'))); continue
    if mode == 2:
        if talked: break
        npc = next((w for w in allw if w['class'] == 'MapNonPlayableCharacter'), None)
        av = next((w for w in allw if w['class'] == 'AvatarActor'), None)
        if npc and av:
            print('warp:', get('warp', avatar=av['addr'], to=npc['addr'])); time.sleep(3)
            npc = next((w for w in get('ui', all=1).get('widgets', []) if w.get('addr') == npc['addr']), npc)
            x, y = npc['rect'][:2]
            print('tap NPC at', x, y); get('tap', x=min(max(x, 60), 1076), y=min(max(y - 20, 90), 600)); time.sleep(4)
            continue
        print('no NPC on this area'); break
    if mode in (3, 4): get('tap', x=568, y=500)
    time.sleep(2)
print('talked:', talked)
print('instawin:', instawin(120))
for _ in range(30):
    sc = screen()
    if sc not in ('SceneBattleResult', 'LoadingIndicator', 'SceneActionMap', '') + STORY_SCENES: break
    if is_story(sc): advance_story()
    if sc == 'SceneBattleResult':
        b = button(name='Button_Ok')
        if b: tap_w(b)
        get('tap', x=568, y=600)
    time.sleep(4)
print('now at:', screen())
