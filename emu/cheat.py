"""Dev/test cheats on a running game, through the control channel's 'call' endpoint (native exe and emulator).

python -m recomp.emu.cheat gauge     fill the special gauge (CP)
python -m recomp.emu.cheat heal      fill the player's HP
python -m recomp.emu.cheat kill      defeat every enemy on the quest map (each dies at its next hit or at once
                                     in the current fight; the stage clears when the last group falls)
python -m recomp.emu.cheat all       gauge + heal + kill

They call the game's own functions (SceneBattleBase::addPlayerCP / addPlayerHP, EnemyActor::addDamage), so the
game's state stays consistent. Testing only: never part of what the client ships. See recomp/emu/AUTOMATION.md.
"""
import sys

from .auto_battle import get, rush_fill_gauge, rush_heal
from .play_quest import A_ENEMY_ADD_DAMAGE


def gauge():
    rush_fill_gauge(get("ui").get("widgets", []))


def heal():
    rush_heal()


def kill():
    """EnemyActor::addDamage(enemy, 10,000,000, &empty Burst shared_ptr) on every enemy actor; returns how many."""
    n = 0
    for w in get("ui", all=1).get("widgets", []):
        if w["class"] == "EnemyActor":
            get("call", fn=hex(A_ENEMY_ADD_DAMAGE), args=f"{w['addr']},10000000,zero")
            n += 1
    return n


def main():
    what = sys.argv[1] if len(sys.argv) > 1 else "all"
    if what in ("gauge", "all"):
        gauge()
    if what in ("heal", "all"):
        heal()
    if what in ("kill", "all"):
        print("enemies hit:", kill())
    if what not in ("gauge", "heal", "kill", "all"):
        sys.exit(__doc__)


if __name__ == "__main__":
    main()
