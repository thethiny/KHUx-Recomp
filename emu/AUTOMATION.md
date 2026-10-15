# Driving the game automatically (agents: read this before any test run)

Everything here works on the **native exe** (`recomp/out/host/khux.exe`) and on the Unicorn emulator
(`python -m recomp.emu.run`): both serve the same control channel on `127.0.0.1:8765`. The cheats are dev/test
tools only — they call the game's own functions, nothing ships with them.

## 1. Start things

| What | Command (bash, repo root unless noted) |
|---|---|
| Private server | `cd server && KHUX_VERSION_RES=1 python run.py` (run_in_background; port 80) |
| Game, normal | `cd recomp && out/host/khux.exe > out/host/rwtest.log 2>&1` (run_in_background) |
| Game, verifying rewrites | prefix `KHUX_RW_VERIFY=1` (slow: every rewritten call runs twice; crackling audio) |
| Tap through to the title | `python -m recomp.emu.boot_to_title` |
| Close the notice | `python -m recomp.emu.step tap name=Button_Close` |
| Stop the game | `powershell -c "Stop-Process -Name khux"` |

One game instance at a time. Never take screenshots: read `/ui` instead, or ask the user to look (he is faster
than any script at "is it black / stuck / crashed").

## 2. Play: which script for which screen

| Script | Starts from | Does |
|---|---|---|
| `python -u -m recomp.emu.campaign N [secs]` | **anywhere** (MyPage, a resumed quest, quest list) | N rounds: finishes a battle in progress, goes to Quests, picks the newest quest (or a raid boss), runs `play_quest` |
| `python -m recomp.emu.play_quest [secs]` | **the quest detail screen only** (SceneAdventureSelect with Begin) | Begin → keyblade Confirm → support → Start (103) → battle → results. From MyPage it fails with "no 102 after Begin" |
| `python -m recomp.emu.auto_battle [secs]` | a quest map (SceneActionMap / SceneRaidBattle) | fights until the map is left |
| `python -m recomp.emu.cheat gauge\|heal\|kill\|all` | a quest map | full special gauge / full HP / every enemy defeated |

Use `campaign` unless you know you are on the quest detail screen.

## 3. Rush and insta-win (use them for anything that is not a battle test)

Both are **environment variables read by the Python scripts** (campaign / play_quest / auto_battle), not by the
game. Setting them only on `khux.exe` does nothing — export them in the shell that runs the scripts:

```bash
export KHUX_RUSH=1 KHUX_INSTAWIN=1
python -u -m recomp.emu.campaign 5 600
```

- **`KHUX_INSTAWIN=1`** (exe only): when a *fresh* quest's map is in field mode, `play_quest.instawin()` calls the
  game's own `SceneActionMap::stageFinishNotification` (0x600EF0): finish animation, `/stage/clear` (106), results.
  ~30–40 s per quest. The clear carries no loot; story progress and missions count. It does **not** fire in a fight
  (stage mode 1) or on a quest resumed at boot — for those, `campaign` (with INSTAWIN set) and `cheat kill` call
  `EnemyActor::addDamage(enemy, 10,000,000, &empty Burst)` on every `EnemyActor`: the current fight ends at once,
  the other groups die as they are reached.
- **`KHUX_RUSH=1`**: `auto_battle` skips the regular enemies — warps next to the boss (the `EnemyActor` under
  `BossName_Panel`) or to the area gate — and keeps the special gauge and HP full
  (`SceneBattleBase::addPlayerCP` 0x5FAD2C with 1,000,000 = full; `addPlayerHP` 0x5FAC9C), using specials on every
  boss part. Needed for **raids** (insta-win does not cover them).
- Stage modes (`auto_battle.scene_mode(widgets)`): 1 battle, 2 field (walking the map), 3 interlude, 4 stage title,
  5 talk.

If the user is playing by hand and asks for gauge / health / a win: `python -m recomp.emu.cheat gauge`,
`... heal`, `... kill`.

## 4. The control channel (`curl 127.0.0.1:8765/<endpoint>`)

| Endpoint | Use |
|---|---|
| `state` | current screen (layer names), event counter |
| `events?since=N` | scene changes (`>>> screen`), API results (`>>> api {"action": 106, ...}`) |
| `ui` / `ui?all=1` | on-screen widgets (name, class, text, rect, addr) / the whole node tree |
| `tap?x=&y=`, `tap?name=W`, `swipe?...`, `text?...`, `key?...` | input |
| `peek?addr=0x..&n=4` | read guest words |
| `call?fn=0xADDR&args=a,b,zero` | DEV ONLY: call a guest function on the main thread (≤8 int args; `zero` = pointer to 64 zero bytes) |
| `warp?avatar=0x..&to=0x..` (or `&x=&y=`) | DEV ONLY: move the player through the game's own walkability check |
| `textdata?id=` | a text table entry |
| `threads` | dump guest threads to the log (hangs) |

The exe also prints `>>> screen` / `>>> api` lines into its log (`recomp/out/host/rwtest.log` above).

## 5. Verifier runs (M3)

- `KHUX_RW_VERIFY=1` shadow-runs every rewritten function; `KHUX_RW_ONLY=@file` / `KHUX_RW_SKIP=@file` (one name
  prefix per line) limit which rewrites are installed (the verifier's effect hooks always stay).
- Results: `python recomp/rewrite/tools/merge_stats.py` → `rewrite/status.tsv`, `rewrite/verified.txt`
  (`--reset ADDR` after fixing a function). Mismatch reports are the `RW MISMATCH` blocks in the exe log.
- A full verifying session: start the exe with `KHUX_RW_VERIFY=1`, `boot_to_title`, close the notice, then the
  campaign with rush + insta-win as in section 3.
