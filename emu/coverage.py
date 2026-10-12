"""Record which guest code blocks execute (python -m recomp.emu.run --coverage).

Every executed basic block start is added to a set, with bit 0 set when the block runs in Thumb state (the
mode is ground truth - the static analysis has to guess it for some functions). The set is written to
out/coverage.json every few
seconds (the emulator is usually stopped by killing it). The hybrid runtime uses it to patch only functions
the game really enters: an "entry" IDA placed on data would otherwise be corrupted by the patch.
"""
import json
import threading
import time

from unicorn import UC_HOOK_BLOCK
from unicorn.arm_const import UC_ARM_REG_CPSR

from .. import paths
from .core import IMAGE_HI, IMAGE_LO

OUT_FILE = paths.OUT / "coverage.json"


class Coverage:
    def __init__(self, emu):
        self.emu = emu
        self.blocks = set()
        if OUT_FILE.exists():                       # accumulate across runs
            self.blocks.update(json.loads(OUT_FILE.read_text())["blocks"])
        self.lock = threading.Lock()
        emu.uc.hook_add(UC_HOOK_BLOCK, self._on_block, begin=IMAGE_LO, end=IMAGE_HI)
        threading.Thread(target=self._saver, daemon=True).start()
        emu.log(f"coverage: recording ({len(self.blocks)} blocks known)")

    def _on_block(self, uc, addr, size, _):
        self.blocks.add(addr | (1 if self.emu.reg(UC_ARM_REG_CPSR) & 0x20 else 0))

    def _saver(self):
        while True:
            time.sleep(5)
            self.save()

    def save(self):
        with self.lock:
            OUT_FILE.write_text(json.dumps({"blocks": sorted(self.blocks)}))


def load_blocks():
    """Executed block start addresses with the Thumb bit (bit 0) of the observed mode, or None when no
    coverage was recorded."""
    if not OUT_FILE.exists():
        return None
    return set(json.loads(OUT_FILE.read_text())["blocks"])


def install(emu):
    emu.coverage = Coverage(emu)
    return emu.coverage
