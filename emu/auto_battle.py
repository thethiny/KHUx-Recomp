"""Blind battle driver (dev helper): attack whatever enemy the UI shows, acknowledge tutorial prompts.

python -m recomp.emu.auto_battle [max_seconds]

Each round: if a prompt with an OK button is up, log its text and tap OK; else if enemy level labels
(LV_NumLabel) are on screen, tap just below the first one (the enemy body); else tap the right side of
the screen to walk on. Stops when the screen leaves SceneActionMap, on a crash, or at the time limit.
"""
import sys
import time

from .ctl import get

VERBOSE = bool(__import__("os").environ.get("KHUX_BATTLE_VERBOSE"))


def main():
    limit = float(sys.argv[1]) if len(sys.argv) > 1 else 300
    t0 = time.time()
    since = get("state")["events"]
    last_prompt = None
    walk_step = 0
    bare = 0
    # attack progress: when tapping on-screen enemies changes nothing (enemy behind an obstacle), explore
    # (gate / target / detours) for a while instead
    last_score, attack_taps, explore_until = None, 0, 0
    # long-range progress: if the nearest off-screen enemy gets no closer for a minute, random-walk for a while
    # (committed random directions get round terrain the straight-line + detour steering cannot)
    best_dist, best_t, wander_until, wander_dir, wander_taps = None, time.time(), 0, 0, 0
    wanders = 0
    npc_goal, talked, npc_since = None, set(), 0
    warped_to = set()
    last_view, detour, detour_dir = None, 0, 0
    # walking directions tried when a walk tap does not move the camera (blocked straight path)
    detours = [(568 + 380 * dx, 320 + 230 * dy) for dx, dy in
               ((1, 0), (1, -1), (0, -1), (-1, -1), (-1, 0), (-1, 1), (0, 1), (1, 1))]

    # Exploration state for walking to off-screen goals. The camera follows the player, so a fixed map
    # object's screen position gives the player's world position (pw = -anchor). The game has no pathfinding
    # (AvatarActor moves in a straight line with collision), so steer greedily over 200 px cells: prefer the
    # neighbour cell closest to the goal, penalised by visits and by directions found blocked from a cell.
    nav = {"anchor": None, "last": None, "last_dir": None, "visits": {}, "blocked": set()}
    DIRS = [(1, 0), (1, 1), (0, 1), (-1, 1), (-1, 0), (-1, -1), (0, -1), (1, -1)]

    def player_world():
        allw = get("ui", all=1).get("widgets", [])
        if nav["anchor"] is None or not any(w.get("addr") == nav["anchor"] for w in allw):
            # static objects only (placement objects can be moving props, e.g. the Mine's carts)
            a = next((w for c in ("MapMaterial", "MapGateMarker", "MapTreasure") for w in allw if w["class"] == c), None)                 or next((w for w in allw if w["class"] == "MapPlacementObject"), None)
            if a is None:
                return None
            nav["anchor"] = a["addr"]
            nav["last"] = None
        a = next(w for w in allw if w.get("addr") == nav["anchor"])
        return (-a["rect"][0], -a["rect"][1])

    def walk(x, y):
        """Walk toward screen point (x, y) (possibly far off screen)."""
        import math
        pw = player_world()
        if pw is None:
            get("tap", x=min(max(x, 60), 1076), y=min(max(y, 90), 600))
            return
        cell = (round(pw[0] / 200), round(pw[1] / 200))
        nav["visits"][cell] = nav["visits"].get(cell, 0) + 1
        if nav["last"] is not None and nav["last_dir"] is not None:
            moved = abs(pw[0] - nav["last"][0]) + abs(pw[1] - nav["last"][1])
            if moved < 20:
                nav["blocked"].add((nav["last_cell"], nav["last_dir"]))
        goal = (pw[0] + (x - 568), pw[1] + (y - 320))        # goal in world coordinates
        best = None
        for d in DIRS:
            if (cell, d) in nav["blocked"]:
                continue
            nc = (cell[0] + d[0], cell[1] + d[1])
            dist = math.hypot(nc[0] * 200 - goal[0], nc[1] * 200 - goal[1])
            score = dist + 300 * nav["visits"].get(nc, 0)
            if best is None or score < best[0]:
                best = (score, d)
        if best is None:                                     # every direction blocked here: forget and retry
            nav["blocked"] = {b for b in nav["blocked"] if b[0] != cell}
            best = (0, DIRS[int(time.time()) % 8])
        d = best[1]
        n = math.hypot(*d)
        tx, ty = 568 + 360 * d[0] / n, 320 + 230 * d[1] / n
        nav["last"], nav["last_dir"], nav["last_cell"] = pw, d, cell
        if VERBOSE:
            print(f"  walk goal=({goal[0]:.0f},{goal[1]:.0f}) at={pw} dir={d} -> ({tx:.0f},{ty:.0f})")
        get("tap", x=tx, y=min(max(ty, 90), 600))

    while time.time() - t0 < limit:
        st = get("state")
        screen = st.get("screen") or []
        if len(screen) <= 1:
            # bare SceneManager between scenes (story -> battle): wait, but not forever (error popups sit here)
            bare += 1
            if bare < 30:
                time.sleep(2)
                continue
        else:
            bare = 0
        if any(x in screen for x in ("SceneLight", "SceneFlashDrama")) or screen[1:2] == ["LoadingIndicator"]:
            # mid-stage story (SceneLight + SpeechDisp, drama) or loading: tap on, the battle continues after
            get("tap", x=568, y=320)
            time.sleep(3)
            continue
        if "SceneActionMap" not in screen and "SceneRaidBattle" not in screen:
            print("left battle:", st.get("screen"))
            break
        for ev in get("events", since=since):
            if ev["kind"] in ("api", "crash", "scene"):
                print(f"  event {ev['kind']}: {ev.get('action', ev.get('error', ev.get('name')))}")
            since = ev["seq"] + 1
            if ev["kind"] == "crash":
                return
        ui = get("ui")
        if "error" in ui:
            time.sleep(1)
            continue
        widgets = ui["widgets"]
        ok = next((w for w in widgets if w["touch"] and w["name"] == "Button_OK"), None)
        if ok:
            prompt = next((w["text"] for w in widgets if w["name"] in ("Txt", "Txt_1Line_Label") and w["text"]), "")
            if prompt != last_prompt:
                print(f"{time.time() - t0:5.0f}s prompt: {prompt!r}")
                last_prompt = prompt
            r = ok["rect"]
            get("tap", x=(r[0] + r[2]) / 2, y=(r[1] + r[3]) / 2)
            time.sleep(1.5)
            continue
        # attackable: label well inside the view (the body is ~45 px below the label; one at the bottom edge
        # is reached by walking toward it instead)
        if "SceneRaidBattle" in screen:
            # raid: the boss parts carry RaidBossHPIndicator markers (under BattleMarker); tap just below them
            # in turn (arms first, then the head, as the raid tutorial asks)
            marks = sorted((w["rect"][:2] for w in get("ui", all=1).get("widgets", [])
                            if w["class"] == "RaidBossHPIndicator" and "/BattleMarker/" in w["path"]),
                           key=lambda r: -r[1])
            if marks:
                x, y = marks[walk_step % len(marks)]
                walk_step += 1
                get("tap", x=min(max(x, 40), 1096), y=min(max(y + 30, 90), 600))
            time.sleep(2.0)
            continue
        enemies = [w for w in widgets if w["name"] == "LV_NumLabel" and 40 <= w["rect"][0] <= 1096
                   and 60 <= w["rect"][1] <= 520]
        tutorial = any(w["class"].endswith("TutorialWidget") for w in widgets)
        score = (tuple(w.get("text") for w in widgets if w["name"] in ("Manny_Label", "Lux_Label")),
                 sum(1 for w in widgets if w["name"] == "LV_NumLabel"))
        if score != last_score:
            last_score, attack_taps = score, 0
        if enemies and attack_taps >= 8 and time.time() >= explore_until:
            explore_until = time.time() + 60
            attack_taps = 0
            print(f"{time.time() - t0:5.0f}s attacks have no effect - exploring for a minute")
        if time.time() < explore_until:
            enemies = []
        if enemies and tutorial and last_prompt and "wipe" in last_prompt:
            # "Swipe the screen to attack all enemies!": drag through every enemy body, left to right
            pts = sorted((w["rect"][0], min(w["rect"][3] + 45, 600)) for w in enemies)
            (x0, y0), (x1, y1) = pts[0], pts[-1]
            get("swipe", x0=x0 - 40, y0=y0, x1=x1 + 40, y1=y1)
        elif enemies:
            r = enemies[0]["rect"]
            get("tap", x=r[0], y=min(r[3] + 45, 600))
            attack_taps += 1
        elif npc_goal and time.time() - npc_since > 60:
            talked.add(npc_goal)                  # could not reach it: give up on this NPC
            npc_goal, best_t = None, time.time() - 61
            print(f"{time.time() - t0:5.0f}s could not reach the NPC")
        elif npc_goal and (npc := next((w for w in get("ui", all=1).get("widgets", [])
                                         if w.get("addr") == npc_goal), None)):
            x, y = npc["rect"][:2]
            if 60 <= x <= 1076 and 90 <= y <= 600:
                get("tap", x=x, y=y - 20)          # talk to it
                talked.add(npc_goal)
                npc_goal = None
                print(f"{time.time() - t0:5.0f}s tapped the NPC")
                time.sleep(3)
            else:
                walk(x, y)
        elif (offscreen := [w for w in widgets if w["name"] == "LV_NumLabel"]) and time.time() >= explore_until:
            # enemies exist but off screen: walk toward the nearest one (tap between centre and edge)
            cx, cy = 568, 320
            r = min(offscreen, key=lambda w: (w["rect"][0] - cx) ** 2 + (w["rect"][1] - cy) ** 2)["rect"]
            dx, dy = r[0] - cx, r[1] - cy
            k = min(400 / max(abs(dx), 1), 250 / max(abs(dy), 1))
            dist = (dx * dx + dy * dy) ** 0.5
            if best_dist is None or dist < best_dist - 50:
                best_dist, best_t = dist, time.time()
            elif time.time() - best_t > 60 and time.time() >= wander_until:
                wanders += 1
                best_dist = None
                allw = get("ui", all=1).get("widgets", [])
                npcs = [w for w in allw if w["class"] == "MapNonPlayableCharacter" and w["addr"] not in talked]
                avatar = next((w for w in allw if w["class"] == "AvatarActor"), None)
                foes = [w for w in allw if w["class"] == "EnemyActor"]
                if wanders >= 2 and avatar and foes:
                    # DEV HARNESS: blind steering failed twice; let the game place the player next to the nearest
                    # enemy (control 'warp' -> AvatarActor::setSefeMoveMapPotision). Not part of the server.
                    fresh = [w for w in foes if w["addr"] not in warped_to] or foes
                    if len(fresh) == len(foes):
                        warped_to.clear()
                    foe = min(fresh, key=lambda w: (w["rect"][0] - cx) ** 2 + (w["rect"][1] - cy) ** 2)
                    warped_to.add(foe["addr"])   # next time try another one (this one may sit on a ledge)
                    print(f"{time.time() - t0:5.0f}s warp next to an enemy: {get('warp', avatar=avatar['addr'], to=foe['addr'])}")
                    wanders = 0
                elif npcs:
                    # an NPC on the map often blocks the way until talked to (StageFieldManager::checkEventAreaTap
                    # handles NPCs as event areas): go and tap it
                    npc_goal, npc_since = npcs[0]["addr"], time.time()
                    print(f"{time.time() - t0:5.0f}s no progress toward enemies - going to talk to an NPC")
                else:
                    wander_until = time.time() + 40
                    print(f"{time.time() - t0:5.0f}s no progress toward enemies - wandering")
            if time.time() < wander_until:
                import math, random
                if wander_taps == 0:
                    wander_dir, wander_taps = random.uniform(0, 2 * math.pi), 8
                wander_taps -= 1
                get("tap", x=568 + 420 * math.cos(wander_dir), y=min(max(320 + 250 * math.sin(wander_dir), 90), 600))
            else:
                walk(r[0], r[1] + 45)          # the enemy's real (off-screen) position
        elif boss := next((w for w in sorted(get("ui", all=1).get("widgets", []),
                                             key=lambda w: w["class"] != "MapGateMarker")
                           if w["class"] == "MapGateMarker" or w["name"] == "Adv_BossMarker"), None):
            # no enemy in view: walk to the area exit (MapGateMarker), else toward the quest TARGET (boss
            # marker), clamped onto the screen
            x, y = boss["rect"][:2]
            walk(x, y)
        else:
            # nothing to attack: walk on. The tutorial's later phases lead up-right (chest, then the
            # TARGET boss; phone auto_taps.py used (975,208) and (1388,234) at 1920x1080)
            walk_targets = [(821, 138), (577, 123), (900, 330)]
            x, y = walk_targets[walk_step % len(walk_targets)]
            walk_step += 1
            get("tap", x=x, y=y)
        time.sleep(2.0)


if __name__ == "__main__":
    main()
