"""Lift ARMv5TE (ARM + Thumb-1) functions of the game library to C.

Each guest function becomes `void F_<addr>(CPU *c)` (see rt.h). Instructions are decoded by the analyzer's
recursive descent (analyze.analyze_function), so function extents, switch tables and noreturn calls match
the M0 analysis. A function the lifter cannot fully translate raises Unsupported and stays emulated.

Control flow the C code cannot follow is handed back to the emulator: the function returns with c->pc set to
the guest address to continue at (an "escape"). Guest state lives entirely in the CPU struct, so the emulator
resumes exactly there. Callers check c->pc after every call and return too when it is not their own return
address, so an escape anywhere unwinds the whole C call chain. This covers unsupported instructions, switch
tables, indirect jumps and helpers that return to a computed address.
"""
import struct
import sys
from pathlib import Path

from capstone import arm as A

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent))
import analyze  # noqa: E402

ARM, THUMB = analyze.ARM, analyze.THUMB


class Unsupported(Exception):
    pass


COND = {
    A.ARM_CC_EQ: "c->z", A.ARM_CC_NE: "!c->z", A.ARM_CC_HS: "c->c", A.ARM_CC_LO: "!c->c",
    A.ARM_CC_MI: "c->n", A.ARM_CC_PL: "!c->n", A.ARM_CC_VS: "c->v", A.ARM_CC_VC: "!c->v",
    A.ARM_CC_HI: "(c->c && !c->z)", A.ARM_CC_LS: "(!c->c || c->z)", A.ARM_CC_GE: "(c->n == c->v)",
    A.ARM_CC_LT: "(c->n != c->v)", A.ARM_CC_GT: "(!c->z && c->n == c->v)", A.ARM_CC_LE: "(c->z || c->n != c->v)",
}
ALWAYS = (A.ARM_CC_AL, A.ARM_CC_INVALID)

# capstone shift type -> (rt.h SHIFT type, by register?)
SHIFTS = {
    A.ARM_SFT_LSL: (0, False), A.ARM_SFT_LSR: (1, False), A.ARM_SFT_ASR: (2, False), A.ARM_SFT_ROR: (3, False),
    A.ARM_SFT_RRX: (4, False), A.ARM_SFT_LSL_REG: (0, True), A.ARM_SFT_LSR_REG: (1, True),
    A.ARM_SFT_ASR_REG: (2, True), A.ARM_SFT_ROR_REG: (3, True), A.ARM_SFT_RRX_REG: (4, True),
}

# instructions read as data-processing "op2" users
LOGICAL = {A.ARM_INS_AND, A.ARM_INS_EOR, A.ARM_INS_ORR, A.ARM_INS_BIC, A.ARM_INS_MOV, A.ARM_INS_MVN,
           A.ARM_INS_TST, A.ARM_INS_TEQ}
ARITH = {A.ARM_INS_ADD, A.ARM_INS_ADC, A.ARM_INS_SUB, A.ARM_INS_SBC, A.ARM_INS_RSB, A.ARM_INS_RSC,
         A.ARM_INS_CMP, A.ARM_INS_CMN}
COMPARE = {A.ARM_INS_CMP, A.ARM_INS_CMN, A.ARM_INS_TST, A.ARM_INS_TEQ}
UNARY = {A.ARM_INS_MOV, A.ARM_INS_MVN}
SHIFT_INS = {A.ARM_INS_LSL: 0, A.ARM_INS_LSR: 1, A.ARM_INS_ASR: 2, A.ARM_INS_ROR: 3, A.ARM_INS_RRX: 4}
MOVS_ID = getattr(A, "ARM_INS_MOVS", None)
DSP_MUL = {getattr(A, "ARM_INS_" + n.upper()): n for n in (
    "smulbb", "smulbt", "smultb", "smultt", "smlabb", "smlabt", "smlatb", "smlatt", "smulwb", "smulwt",
    "smlawb", "smlawt", "smlalbb", "smlalbt", "smlaltb", "smlaltt", "mls")}

EXCL_LOAD = {A.ARM_INS_LDREX: "R32", A.ARM_INS_LDREXB: "R8", A.ARM_INS_LDREXH: "R16"}
EXCL_STORE = {A.ARM_INS_STREX: "STREX32", A.ARM_INS_STREXB: "STREX8", A.ARM_INS_STREXH: "STREX16"}

LOADS = {A.ARM_INS_LDR: ("R32", 4), A.ARM_INS_LDRB: ("R8", 1), A.ARM_INS_LDRH: ("R16", 2),
         A.ARM_INS_LDRSB: ("RS8", 1), A.ARM_INS_LDRSH: ("RS16", 2)}
STORES = {A.ARM_INS_STR: ("W32", 4), A.ARM_INS_STRB: ("W8", 1), A.ARM_INS_STRH: ("W16", 2)}
LDM = {A.ARM_INS_LDM: "ia", A.ARM_INS_LDMIB: "ib", A.ARM_INS_LDMDA: "da", A.ARM_INS_LDMDB: "db",
       A.ARM_INS_POP: "pop"}
STM = {A.ARM_INS_STM: "ia", A.ARM_INS_STMIB: "ib", A.ARM_INS_STMDA: "da", A.ARM_INS_STMDB: "db",
       A.ARM_INS_PUSH: "push"}


class FuncLifter:
    def __init__(self, cs_reg_name, img, fn_mode=None, plt=None, fragments=None):
        self.fragments = fragments or set()   # IDA-split pieces that return through the parent's frame
        self.fn_mode = fn_mode or {}       # function entry -> ARM/THUMB
        self.plt = plt or {}               # plt stub -> internal target (with Thumb bit) or None
        self.reg_index = {}
        self.vreg = {}                     # VFP register id -> ("s" | "d" | "q", number), or a system register
        for rid in range(1, 200):
            n = cs_reg_name(rid)
            if not n:
                continue
            idx = {"sb": 9, "sl": 10, "fp": 11, "ip": 12, "sp": 13, "lr": 14, "pc": 15}.get(n)
            if idx is None and n.startswith("r") and n[1:].isdigit():
                idx = int(n[1:])
            if idx is not None:
                self.reg_index[rid] = idx
            elif n[0] in "sdq" and n[1:].isdigit():
                self.vreg[rid] = (n[0], int(n[1:]))
            elif n in ("apsr_nzcv", "fpscr"):
                self.vreg[rid] = (n, 0)
        self.img = img
        self.cs_reg_name = cs_reg_name

    # ------------------------------------------------------------------ operands
    def ridx(self, rid):
        if rid not in self.reg_index:
            raise Unsupported(f"register id {rid}")
        return self.reg_index[rid]

    def rd(self, rid):
        """C expression for reading a register (PC reads as the instruction address + 8/4)."""
        k = self.ridx(rid)
        if k == 15:
            return f"0x{(self.pc + (8 if self.mode == ARM else 4)) & 0xFFFFFFFF:X}u"
        return f"c->r[{k}]"

    def op2(self, o, want_carry):
        """(statements, value expr, carry expr or None) for a data-processing operand."""
        if o.type == A.ARM_OP_IMM:
            v = o.imm & 0xFFFFFFFF
            carry = None
            if want_carry and self.mode == ARM:
                w = struct.unpack("<I", self.ins.bytes[:4])[0]
                if (w >> 25) & 7 == 1 and (w >> 8) & 0xF:
                    carry = str(v >> 31)
            return "", f"0x{v:X}u", carry
        if o.type != A.ARM_OP_REG:
            raise Unsupported("op2 type")
        base = self.rd(o.reg)
        st = o.shift.type
        if st == A.ARM_SFT_INVALID or (st == A.ARM_SFT_LSL and o.shift.value == 0):
            return "", base, None
        typ, by_reg = SHIFTS[st]
        if by_reg:
            amt = f"({self.rd(o.shift.value)} & 0xFF)"
        else:
            amt = str(o.shift.value)
        return (f"uint32_t sc_ = c->c; uint32_t sv_ = SHIFT({base}, {typ}, {amt}, c->c, &sc_);",
                "sv_", "sc_")

    def pc_target(self, expr, interwork):
        """Expression for a computed PC value: loads to PC interwork on bit 0, data-processing writes stay in
        the current instruction set."""
        if interwork:
            return f"({expr})"
        return f"(({expr}) | 1u)" if self.mode == THUMB else f"(({expr}) & ~1u)"

    def jump_to(self, expr, interwork):
        """Control goes to a computed address. At an analysed switch site, known case targets inside this
        function become gotos; at an analysed indirect jump it is a tail call; otherwise (returns) it hands the
        address back to the caller."""
        v = self.pc_target(expr, interwork)
        cases = [t for t in self.switch_at.get(self.pc, ()) if t in self.addrs]
        if self.far_rets and not cases:
            # a piece reached by a far-branch bl may return through LR to just after that bl: stay in C
            body = " ".join(f"case 0x{r:X}u: goto L_{r & ~1:X};" for r in sorted(self.far_rets))
            tail = f"TAIL(j_);" if self.pc in self.indirect else "c->pc = j_; return;"
            return f"{{ uint32_t j_ = {v}; switch (j_) {{ {body} default: break; }} {tail} }}"
        if cases:
            body = " ".join(f"case 0x{t:X}u: goto L_{t:X};" for t in sorted(set(cases)))
            return (f"{{ uint32_t j_ = {v}; switch (j_ & ~1u) {{ {body} default: break; }} "
                    f"c->pc = j_; return; }}")
        if self.pc in self.indirect:
            return f"TAIL({v});"
        return f"c->pc = {v}; return;"

    def write_reg(self, k, expr, interwork=False):
        """Assign a register; writing PC returns from the function (the emulator continues there).
        ARMv5: loads to PC (ldr/ldm/pop) interwork on bit 0, but data-processing writes (mov/add pc, ...) stay
        in the current instruction set - e.g. a Thumb switch's `mov pc, r0` with an even r0 stays Thumb."""
        if k == 15:
            return self.jump_to(expr, interwork)
        return f"c->r[{k}] = {expr};"

    def call_target(self, t, tmode):
        """Guest address to call for a branch-with-link target: PLT stubs that point back into the library
        are resolved to the function itself; the Thumb bit comes from the function's mode."""
        if t in self.plt:
            internal = self.plt[t]
            if internal is not None:
                return internal
            return t                                  # real import: ARM PLT stub, runs in the emulator
        mode = self.fn_mode.get(t, tmode)
        return t | (1 if mode == THUMB else 0)

    def leave(self, t):
        """Control leaves the function at t: a tail call when t is a function or PLT stub, else an escape."""
        if t in self.fragments:
            target = self.call_target(t, self.mode)
            return f"c->pc = 0x{target:X}u; return;"     # escape: the emulator jumps into the fragment
        if t in self.fn_mode or t in self.plt:
            target = self.call_target(t, self.mode)
            return f"TAIL(0x{target:X}u);"
        return f"c->pc = 0x{t | (1 if self.mode == THUMB else 0):X}u; return;"

    # ------------------------------------------------------------------ memory operands
    def mem_addr(self, o):
        """(address expr, writeback offset expr or None) for a memory operand + the post-index operand."""
        m = o.mem
        if m.base == A.ARM_REG_PC:
            base = f"0x{((self.pc + 4) & ~3 if self.mode == THUMB else self.pc + 8) & 0xFFFFFFFF:X}u"
        else:
            base = self.rd(m.base)
        if m.index:
            idx = self.rd(m.index)
            if o.shift.type not in (A.ARM_SFT_INVALID,) and o.shift.value:
                typ, by_reg = SHIFTS[o.shift.type]
                if by_reg:
                    raise Unsupported("register-shifted index")
                idx = f"SHIFT({idx}, {typ}, {o.shift.value}, c->c, &dc_)"
            off = f"-(uint32_t){idx}" if o.subtracted else f"(uint32_t){idx}"
        else:
            d = m.disp
            if o.subtracted and d > 0:
                d = -d
            off = f"(uint32_t){d}" if d >= 0 else f"-(uint32_t){-d}"
        return base, off

    # ------------------------------------------------------------------ one instruction
    LR_OK = None

    def insn(self, i):
        self.ins = i
        iid = i.id
        ops = i.operands
        body = self.body(i, iid, ops)
        cc = i.cc
        if self.mode == ARM and cc not in ALWAYS and iid != A.ARM_INS_B:
            return f"if ({COND[cc]}) {{ {body} }}"
        return body

    def body(self, i, iid, ops):
        nxt = self.pc + i.size
        uf = i.update_flags
        if self.mode == ARM:
            # Capstone reports update_flags for ARM adc/sbc/rsc even without the S suffix (they read the
            # carry). For data-processing and multiply encodings (bits 27:26 == 00) the S bit (20) decides.
            w = struct.unpack("<I", i.bytes[:4])[0]
            if (w >> 26) & 3 == 0:
                uf = bool((w >> 20) & 1)

        # ---------------- branches
        if iid == A.ARM_INS_B:
            t = ops[0].imm
            go = f"goto L_{t:X};" if t in self.addrs else self.leave(t)
            if i.cc in ALWAYS:
                return go
            return f"if ({COND[i.cc]}) {{ {go} }}"
        if iid == A.ARM_INS_BX:
            return self.jump_to(self.rd(ops[0].reg), True)
        if iid in (A.ARM_INS_BL, A.ARM_INS_BLX):
            ret = nxt | (1 if self.mode == THUMB else 0)
            if ops[0].type == A.ARM_OP_IMM:
                t = ops[0].imm
                if self.pc in self.switch_at:
                    cases = self.switch_at[self.pc]
                    if not cases or any(t not in self.addrs for t in cases):
                        raise Unsupported("switch helper call")
                    body = " ".join(f"case {k}: c->r[14] = 0x{t | 1:X}u; goto L_{t:X};"
                                    for k, t in enumerate(cases))
                    return f"switch (c->r[0]) {{ {body} default: break; }} c->pc = 0x{self.pc | 1:X}u; return;"
                if t in self.addrs and (t not in self.fn_mode or t in self.fragments):
                    # Thumb-1 far branch inside the function (or into an absorbed fragment)
                    return f"c->r[14] = 0x{ret:X}u; goto L_{t:X};"
                tmode = self.mode if iid == A.ARM_INS_BL else (THUMB if self.mode == ARM else ARM)
                target = self.call_target(t, tmode)
                call = "rt_call_frag" if t in self.fragments else "rt_call"
                return (f"c->r[14] = 0x{ret:X}u; {call}(c, 0x{target:X}u); "
                        f"if (c->pc != 0x{ret:X}u) return;")
            t = self.rd(ops[0].reg)
            return (f"{{ uint32_t t_ = {t}; c->r[14] = 0x{ret:X}u; rt_call(c, t_); "
                    f"if (c->pc != 0x{ret:X}u) return; }}")

        # ---------------- data processing
        if iid in SHIFT_INS and not (len(ops) == 2 and ops[1].type == A.ARM_OP_REG and
                                     ops[1].shift.type not in (A.ARM_SFT_INVALID,)):
            d = self.ridx(ops[0].reg)
            typ = SHIFT_INS[iid]
            if typ == 4:                                # rrx rd, rm
                src, amt = self.rd(ops[1].reg), "1"
            elif len(ops) == 3:
                src = self.rd(ops[1].reg)
                amt = f"({self.rd(ops[2].reg)} & 0xFF)" if ops[2].type == A.ARM_OP_REG else str(ops[2].imm)
            else:                                        # thumb: lsls rd, rs (rd = rd << rs)
                src = self.rd(ops[0].reg)
                amt = f"({self.rd(ops[1].reg)} & 0xFF)" if ops[1].type == A.ARM_OP_REG else str(ops[1].imm)
            code = f"uint32_t sc_ = c->c; uint32_t sv_ = SHIFT({src}, {typ}, {amt}, c->c, &sc_); "
            if uf:
                code += "NZ(c, sv_); c->c = sc_; "
            return "{ " + code + self.write_reg(d, "sv_") + " }"

        if iid in LOGICAL or iid in ARITH or iid in SHIFT_INS or iid == MOVS_ID:
            return self.dataproc(i, iid, ops, uf)

        if iid == A.ARM_INS_ADR:
            v = ops[1].imm
            if self.mode == THUMB:
                v = ((self.pc + 4) & ~3) + v
            return self.write_reg(self.ridx(ops[0].reg), f"0x{v & 0xFFFFFFFF:X}u")

        if iid in (A.ARM_INS_MUL, A.ARM_INS_MLA):
            d = self.ridx(ops[0].reg)
            if len(ops) == 2:                           # thumb muls rd, rm
                expr = f"{self.rd(ops[0].reg)} * {self.rd(ops[1].reg)}"
            else:
                expr = f"{self.rd(ops[1].reg)} * {self.rd(ops[2].reg)}"
            if iid == A.ARM_INS_MLA:
                expr += f" + {self.rd(ops[3].reg)}"
            code = f"uint32_t mv_ = {expr}; "
            if uf:
                code += "NZ(c, mv_); "
            return "{ " + code + self.write_reg(d, "mv_") + " }"

        if iid in (A.ARM_INS_SMULL, A.ARM_INS_UMULL, A.ARM_INS_SMLAL, A.ARM_INS_UMLAL):
            lo, hi = self.ridx(ops[0].reg), self.ridx(ops[1].reg)
            a, b = self.rd(ops[2].reg), self.rd(ops[3].reg)
            if iid in (A.ARM_INS_SMULL, A.ARM_INS_SMLAL):
                prod = f"(uint64_t)((int64_t)(int32_t){a} * (int64_t)(int32_t){b})"
            else:
                prod = f"((uint64_t){a} * (uint64_t){b})"
            if iid in (A.ARM_INS_SMLAL, A.ARM_INS_UMLAL):
                prod += f" + (((uint64_t)c->r[{hi}] << 32) | c->r[{lo}])"
            code = f"uint64_t p_ = {prod}; c->r[{lo}] = (uint32_t)p_; c->r[{hi}] = (uint32_t)(p_ >> 32); "
            if uf:
                code += "c->n = (uint32_t)(p_ >> 63); c->z = p_ == 0; "
            return "{ " + code + "}"

        if iid in (A.ARM_INS_SXTB, A.ARM_INS_SXTH, A.ARM_INS_UXTB, A.ARM_INS_UXTH):
            src = self.rd(ops[1].reg)
            if ops[1].shift.type == A.ARM_SFT_ROR and ops[1].shift.value:
                r = ops[1].shift.value
                src = f"(({src} >> {r}) | ({src} << {32 - r}))"
            cast = {A.ARM_INS_SXTB: "(uint32_t)(int32_t)(int8_t)", A.ARM_INS_SXTH: "(uint32_t)(int32_t)(int16_t)",
                    A.ARM_INS_UXTB: "(uint32_t)(uint8_t)", A.ARM_INS_UXTH: "(uint32_t)(uint16_t)"}[iid]
            return self.write_reg(self.ridx(ops[0].reg), f"{cast}{src}")
        if iid in (A.ARM_INS_UXTAB, A.ARM_INS_UXTAH, A.ARM_INS_SXTAB, A.ARM_INS_SXTAH):
            src = self.rd(ops[2].reg)
            if ops[2].shift.type == A.ARM_SFT_ROR and ops[2].shift.value:
                r = ops[2].shift.value
                src = f"(({src} >> {r}) | ({src} << {32 - r}))"
            cast = {A.ARM_INS_SXTAB: "(uint32_t)(int32_t)(int8_t)", A.ARM_INS_SXTAH: "(uint32_t)(int32_t)(int16_t)",
                    A.ARM_INS_UXTAB: "(uint32_t)(uint8_t)", A.ARM_INS_UXTAH: "(uint32_t)(uint16_t)"}[iid]
            return self.write_reg(self.ridx(ops[0].reg), f"({self.rd(ops[1].reg)} + {cast}{src})")
        if iid == A.ARM_INS_MRS:
            if self.vreg.get(ops[1].reg, ("",))[0] not in ("apsr", "apsr_nzcv") and \
                    (self.cs_reg_name(ops[1].reg) or "") not in ("apsr", "cpsr"):
                raise Unsupported(f"mrs {i.op_str}")
            return self.write_reg(self.ridx(ops[0].reg),
                                  "((c->n << 31) | (c->z << 30) | (c->c << 29) | (c->v << 28) | 0x10u)")
        if iid in (A.ARM_INS_DMB, A.ARM_INS_DSB, A.ARM_INS_ISB):
            return "FENCE();"
        if iid in EXCL_LOAD:
            self.uses_excl = True
            a, off = self.mem_addr(ops[1])
            fn = EXCL_LOAD[iid]
            return (f"{{ uint32_t a_ = {a} + {off}; xa_ = a_; xv_ = {fn}(a_); "
                    f"{self.write_reg(self.ridx(ops[0].reg), 'xv_')} }}")
        if iid in EXCL_STORE:
            self.uses_excl = True
            a, off = self.mem_addr(ops[2])
            fn = EXCL_STORE[iid]
            # succeeds only if memory still holds what ldrex read (compare-exchange; correct with real threads)
            return (f"{{ uint32_t a_ = {a} + {off}; "
                    f"c->r[{self.ridx(ops[0].reg)}] = (a_ == xa_) ? {fn}(c, a_, xv_, {self.rd(ops[1].reg)}) : 1u; "
                    f"xa_ = 0xFFFFFFFFu; }}")
        if iid == A.ARM_INS_CLREX:
            self.uses_excl = True
            return "xa_ = 0xFFFFFFFFu;"
        if iid in (A.ARM_INS_UBFX, A.ARM_INS_SBFX):
            lsb, w = ops[2].imm, ops[3].imm
            src = self.rd(ops[1].reg)
            if iid == A.ARM_INS_UBFX:
                mask = (1 << w) - 1
                return self.write_reg(self.ridx(ops[0].reg), f"(({src} >> {lsb}) & 0x{mask:X}u)")
            return self.write_reg(self.ridx(ops[0].reg),
                                  f"(uint32_t)((int32_t)({src} << {32 - lsb - w}) >> {32 - w})")
        if iid in (A.ARM_INS_BFI, A.ARM_INS_BFC):
            d = self.ridx(ops[0].reg)
            if iid == A.ARM_INS_BFI:
                src, lsb, w = self.rd(ops[1].reg), ops[2].imm, ops[3].imm
            else:
                src, lsb, w = "0u", ops[1].imm, ops[2].imm
            mask = (((1 << w) - 1) << lsb) & 0xFFFFFFFF
            return self.write_reg(d, f"((c->r[{d}] & 0x{(~mask) & 0xFFFFFFFF:X}u) | (({src} << {lsb}) & 0x{mask:X}u))")
        if iid == A.ARM_INS_MOVW:
            return self.write_reg(self.ridx(ops[0].reg), f"0x{ops[1].imm & 0xFFFF:X}u")
        if iid == A.ARM_INS_MOVT:
            d = self.ridx(ops[0].reg)
            return self.write_reg(d, f"((c->r[{d}] & 0xFFFFu) | 0x{(ops[1].imm & 0xFFFF) << 16:X}u)")
        if iid == A.ARM_INS_REV:
            v = self.rd(ops[1].reg)
            return self.write_reg(self.ridx(ops[0].reg),
                                  f"((({v}) >> 24) | ((({v}) >> 8) & 0xFF00u) | ((({v}) << 8) & 0xFF0000u) | (({v}) << 24))")
        if iid == A.ARM_INS_REV16:
            v = self.rd(ops[1].reg)
            return self.write_reg(self.ridx(ops[0].reg),
                                  f"(((({v}) >> 8) & 0x00FF00FFu) | ((({v}) << 8) & 0xFF00FF00u))")
        if iid == A.ARM_INS_CLZ:
            return self.write_reg(self.ridx(ops[0].reg), f"CLZ({self.rd(ops[1].reg)})")

        # ---------------- VFP, DSP multiplies, saturation
        if i.mnemonic.startswith("v"):
            return "VFP_USE(); " + self.vfp(i, ops)
        if iid in DSP_MUL:
            return self.dsp_mul(iid, ops)
        if iid in (A.ARM_INS_USAT, A.ARM_INS_SSAT):
            return self.saturate(iid, ops)
        if iid in (A.ARM_INS_QADD, A.ARM_INS_QSUB):
            op = "+" if iid == A.ARM_INS_QADD else "-"
            e = f"(int64_t)(int32_t){self.rd(ops[1].reg)} {op} (int64_t)(int32_t){self.rd(ops[2].reg)}"
            sat = "(q_ > 0x7FFFFFFFLL ? 0x7FFFFFFFu : q_ < -0x80000000LL ? 0x80000000u : (uint32_t)q_)"
            return "{ int64_t q_ = " + e + "; " + self.write_reg(self.ridx(ops[0].reg), sat) + " }"

        # ---------------- loads / stores
        if iid in LOADS or iid in STORES:
            return self.loadstore(i, iid, ops)
        if iid in (A.ARM_INS_LDRD, A.ARM_INS_STRD):
            return self.dual(i, iid, ops)
        if iid in LDM or iid in STM:
            return self.multi(i, iid, ops)

        if iid == A.ARM_INS_NOP or (iid == A.ARM_INS_MOV and False):
            return ";"
        raise Unsupported(f"{i.mnemonic} {i.op_str}")

    def dataproc(self, i, iid, ops, uf):
        if iid in COMPARE:
            n_expr, o2 = self.rd(ops[0].reg), ops[1]
            d = None
        elif iid in UNARY or iid == MOVS_ID or (iid in SHIFT_INS):
            d, n_expr, o2 = self.ridx(ops[0].reg), None, ops[1]
        elif len(ops) == 2:                              # thumb two-operand form: rd = rd OP rm
            d, n_expr, o2 = self.ridx(ops[0].reg), self.rd(ops[0].reg), ops[1]
        else:
            d, n_expr, o2 = self.ridx(ops[0].reg), self.rd(ops[1].reg), ops[2]
        setflags = uf or iid in COMPARE
        logical = iid in LOGICAL or iid == MOVS_ID or iid in SHIFT_INS
        pre, v2, carry = self.op2(o2, setflags and logical)

        if iid in (A.ARM_INS_MOV,) or iid == MOVS_ID or iid in SHIFT_INS:
            res = v2
        elif iid == A.ARM_INS_MVN:
            res = f"~{v2}"
        elif iid in (A.ARM_INS_AND, A.ARM_INS_TST):
            res = f"({n_expr} & {v2})"
        elif iid in (A.ARM_INS_EOR, A.ARM_INS_TEQ):
            res = f"({n_expr} ^ {v2})"
        elif iid == A.ARM_INS_ORR:
            res = f"({n_expr} | {v2})"
        elif iid == A.ARM_INS_BIC:
            res = f"({n_expr} & ~{v2})"
        elif iid in (A.ARM_INS_ADD, A.ARM_INS_CMN):
            res = f"ADD_F({n_expr}, {v2})" if setflags else f"({n_expr} + {v2})"
        elif iid in (A.ARM_INS_SUB, A.ARM_INS_CMP):
            res = f"SUB_F({n_expr}, {v2})" if setflags else f"({n_expr} - {v2})"
        elif iid == A.ARM_INS_RSB:
            res = f"SUB_F({v2}, {n_expr})" if setflags else f"({v2} - {n_expr})"
        elif iid == A.ARM_INS_ADC:
            res = f"ADC_F(c, {n_expr}, {v2}, c->c)" if setflags else f"({n_expr} + {v2} + c->c)"
        elif iid == A.ARM_INS_SBC:
            res = f"SBC_F({n_expr}, {v2}, c->c)" if setflags else f"({n_expr} - {v2} - !c->c)"
        elif iid == A.ARM_INS_RSC:
            res = f"SBC_F({v2}, {n_expr}, c->c)" if setflags else f"({v2} - {n_expr} - !c->c)"
        else:
            raise Unsupported(f"dataproc {i.mnemonic}")

        code = pre + " " if pre else ""
        if setflags and logical:
            code += f"uint32_t r_ = NZ(c, {res}); "
            if carry:
                code += f"c->c = {carry}; "
        else:
            code += f"uint32_t r_ = {res}; "
        if d is not None:
            if d == 15 and setflags:
                raise Unsupported("flag-setting write to pc")
            code += self.write_reg(d, "r_")
        else:
            code += "(void)r_;"
        return "{ " + code + " }"

    def post_offset(self, po):
        """C expression for a post-index offset operand (immediate or optionally shifted register)."""
        if po.type == A.ARM_OP_IMM:
            v = po.imm
            if po.subtracted and v > 0:
                v = -v
            return f"(uint32_t){v}" if v >= 0 else f"-(uint32_t){-v}"
        if po.type == A.ARM_OP_REG:
            r = self.rd(po.reg)
            if po.shift.type not in (A.ARM_SFT_INVALID,) and po.shift.value:
                typ, by_reg = SHIFTS[po.shift.type]
                if by_reg:
                    raise Unsupported("register-shifted post index")
                r = f"SHIFT({r}, {typ}, {po.shift.value}, c->c, &dc_)"
            return f"-(uint32_t){r}" if po.subtracted else f"(uint32_t){r}"
        raise Unsupported("post-index operand")

    def loadstore(self, i, iid, ops):
        t = self.ridx(ops[0].reg)
        mem = ops[1]
        if mem.type != A.ARM_OP_MEM:
            raise Unsupported("ldr/str operand")
        base, off = self.mem_addr(mem)
        post = getattr(i, "post_index", False) or len(ops) > 2
        wb = i.writeback
        code = "uint32_t dc_ = 0; (void)dc_; "
        if post:
            if len(ops) < 3:
                raise Unsupported("post-index without offset")
            poff = self.post_offset(ops[2])
            code += f"uint32_t a_ = {base}; "
            wbexpr = f"a_ + {poff}"
        else:
            code += f"uint32_t a_ = {base} + {off}; "
            wbexpr = "a_" if wb else None
        bk = self.ridx(mem.mem.base)
        if iid in LOADS:
            fn, _ = LOADS[iid]
            code += f"uint32_t v_ = {fn}(a_); "
            if wbexpr:
                code += f"c->r[{bk}] = {wbexpr}; "
            code += self.write_reg(t, "v_", interwork=True)
        else:
            fn, _ = STORES[iid]
            val = self.rd(ops[0].reg) if t != 15 else f"0x{(self.pc + (8 if self.mode == ARM else 4)) & 0xFFFFFFFF:X}u"
            code += f"{fn}(c, a_, {val}); "
            if wbexpr:
                code += f"c->r[{bk}] = {wbexpr}; "
        return "{ " + code + " }"

    def dual(self, i, iid, ops):
        t1, t2 = self.ridx(ops[0].reg), self.ridx(ops[1].reg)
        mem = ops[2]
        base, off = self.mem_addr(mem)
        post = getattr(i, "post_index", False) or len(ops) > 3
        code = "uint32_t dc_ = 0; (void)dc_; "
        if post:
            code += f"uint32_t a_ = {base}; uint32_t wb_ = a_ + {self.post_offset(ops[3])}; "
        else:
            code += f"uint32_t a_ = {base} + {off}; uint32_t wb_ = a_; "
        bk = self.ridx(mem.mem.base)
        if iid == A.ARM_INS_LDRD:
            code += f"uint32_t v0_ = R32(a_), v1_ = R32(a_ + 4); c->r[{t1}] = v0_; c->r[{t2}] = v1_; "
        else:
            code += f"W32(c, a_, {self.rd(ops[0].reg)}); W32(c, a_ + 4, {self.rd(ops[1].reg)}); "
        if i.writeback or post:
            code += f"c->r[{bk}] = wb_; "
        return "{ " + code + " }"

    def multi(self, i, iid, ops):
        if iid in (A.ARM_INS_POP, A.ARM_INS_PUSH):
            bk, regs, wb = 13, [self.ridx(o.reg) for o in ops], True
            kind = "ia" if iid == A.ARM_INS_POP else "db"
        else:
            bk, regs, wb = self.ridx(ops[0].reg), [self.ridx(o.reg) for o in ops[1:]], i.writeback
            kind = LDM.get(iid) or STM.get(iid)
        regs.sort()
        n = len(regs)
        start = {"ia": "b_", "ib": "b_ + 4", "da": f"b_ - {4 * n - 4}", "db": f"b_ - {4 * n}"}[kind]
        newb = f"b_ + {4 * n}" if kind in ("ia", "ib") else f"b_ - {4 * n}"
        code = f"uint32_t b_ = c->r[{bk}]; uint32_t a_ = {start}; "
        if iid in LDM:
            loads = []
            for k, r in enumerate(regs):
                loads.append((r, f"R32(a_ + {4 * k})"))
            code += " ".join(f"uint32_t l{k}_ = {e};" for k, (r, e) in enumerate(loads)) + " "
            if wb and bk not in regs:
                code += f"c->r[{bk}] = {newb}; "
            ret = None
            for k, (r, e) in enumerate(loads):
                if r == 15:
                    ret = f"l{k}_"
                else:
                    code += f"c->r[{r}] = l{k}_; "
            if ret:
                code += self.jump_to(ret, True) + " "
        else:
            for k, r in enumerate(regs):
                val = self.rd_reg_index(r)
                code += f"W32(c, a_ + {4 * k}, {val}); "
            if wb:
                code += f"c->r[{bk}] = {newb}; "
        return "{ " + code + "}"

    # ------------------------------------------------------------------ DSP multiplies, saturation
    def dsp_mul(self, iid, ops):
        """ARMv5TE halfword multiplies (smulXY, smlaXY, smulwY, smlawY, smlalXY) and ARMv6T2 mls. The sticky Q
        flag set by smla overflow is not modelled (only readable through mrs)."""
        def half(o, top):
            v = self.rd(o.reg)
            return f"((int32_t){v} >> 16)" if top else f"(int32_t)(int16_t){v}"
        name = DSP_MUL[iid]
        if name == "mls":
            e = f"({self.rd(ops[3].reg)} - {self.rd(ops[1].reg)} * {self.rd(ops[2].reg)})"
            return self.write_reg(self.ridx(ops[0].reg), e)
        if name.startswith("smlal"):                     # smlalXY lo, hi, rn, rm
            lo, hi = self.ridx(ops[0].reg), self.ridx(ops[1].reg)
            prod = f"(int64_t)({half(ops[2], name[5] == 't')} * {half(ops[3], name[6] == 't')})"
            return (f"{{ uint64_t p_ = (((uint64_t)c->r[{hi}] << 32) | c->r[{lo}]) + (uint64_t){prod}; "
                    f"c->r[{lo}] = (uint32_t)p_; c->r[{hi}] = (uint32_t)(p_ >> 32); }}")
        if name[4] == "w":                                # smulwY / smlawY: (rn * half(rm)) >> 16
            prod = (f"(uint32_t)(((int64_t)(int32_t){self.rd(ops[1].reg)} * "
                    f"{half(ops[2], name[5] == 't')}) >> 16)")
        else:                                             # smulXY / smlaXY
            prod = f"(uint32_t)({half(ops[1], name[4] == 't')} * {half(ops[2], name[5] == 't')})"
        if name.startswith("smla"):
            prod += f" + {self.rd(ops[3].reg)}"
        return self.write_reg(self.ridx(ops[0].reg), f"({prod})")

    def saturate(self, iid, ops):
        v = self.rd(ops[2].reg)
        if ops[2].shift.type != A.ARM_SFT_INVALID and ops[2].shift.value:
            typ, by_reg = SHIFTS[ops[2].shift.type]
            if by_reg:
                raise Unsupported("register-shifted saturate operand")
            v = f"SHIFT({v}, {typ}, {ops[2].shift.value}, c->c, &dc_)"
        bits = ops[1].imm
        if iid == A.ARM_INS_USAT:
            hi = (1 << bits) - 1
            e = f"(sv_ < 0 ? 0u : sv_ > {hi}LL ? {hi}u : (uint32_t)sv_)"
        else:
            hi, lo = (1 << (bits - 1)) - 1, -(1 << (bits - 1))
            e = f"(sv_ < ({lo}LL) ? (uint32_t)({lo}LL) : sv_ > {hi}LL ? {hi}u : (uint32_t)sv_)"
        return ("{ uint32_t dc_ = 0; (void)dc_; int64_t sv_ = (int32_t)" + v + "; " +
                self.write_reg(self.ridx(ops[0].reg), e) + " }")

    # ------------------------------------------------------------------ VFP
    def vr(self, o):
        """("s" | "d", number) of a VFP data register operand."""
        if o.type != A.ARM_OP_REG or self.vreg.get(o.reg, ("",))[0] not in ("s", "d"):
            raise Unsupported("vfp operand")
        return self.vreg[o.reg]

    @staticmethod
    def vval(r):
        """C lvalue of VFP register r as a float / double."""
        return f"c->vf.f[{r[1]}]" if r[0] == "s" else f"c->vf.df[{r[1]}]"

    @staticmethod
    def vraw(r):
        """C lvalue of VFP register r as raw bits."""
        return f"c->vf.s[{r[1]}]" if r[0] == "s" else f"c->vf.d[{r[1]}]"

    @staticmethod
    def vload(r, a):
        return f"c->vf.s[{r[1]}] = R32({a}); " if r[0] == "s" else f"c->vf.d[{r[1]}] = RD64(c, {a}); "

    @staticmethod
    def vstore(r, a):
        if r[0] == "s":
            return f"W32(c, {a}, c->vf.s[{r[1]}]); "
        return f"W32(c, {a}, (uint32_t)c->vf.d[{r[1]}]); W32(c, {a} + 4, (uint32_t)(c->vf.d[{r[1]}] >> 32)); "

    def vclass(self, o):
        if o.type == A.ARM_OP_REG and o.reg in self.reg_index:
            return "r"
        if o.type == A.ARM_OP_REG and o.reg in self.vreg:
            return self.vreg[o.reg][0]
        return "F" if o.type == A.ARM_OP_FP else "?"

    def vfp(self, i, ops):
        """VFPv3 instructions (data in CPU.vf, flags in CPU.fpscr). Capstone reports some under the old FPA-style
        ids (FCONSTS, FMSTAT, ...), so this dispatches on the mnemonic."""
        parts = i.mnemonic.split(".")
        base, dts = parts[0], parts[1:]
        if self.mode == ARM and i.cc not in ALWAYS:
            base = base[:-2]                               # condition suffix (vmovmi.f32)
        if any(o.type == A.ARM_OP_REG and self.vreg.get(o.reg, ("",))[0] == "q" for o in ops):
            raise Unsupported(f"neon {i.mnemonic}")

        if base in ("vldr", "vstr"):
            r = self.vr(ops[0])
            a, off = self.mem_addr(ops[1])
            code = f"uint32_t a_ = {a} + {off}; "
            code += self.vload(r, "a_") if base == "vldr" else self.vstore(r, "a_")
            return "{ " + code + "}"

        if base in ("vpush", "vpop", "vldmia", "vstmia", "vldmdb", "vstmdb"):
            if base in ("vpush", "vpop"):
                bk, lst, wb = 13, [self.vr(o) for o in ops], True
                kind = "db" if base == "vpush" else "ia"
            else:
                bk, lst, wb = self.ridx(ops[0].reg), [self.vr(o) for o in ops[1:]], i.writeback
                kind = base[-2:]
            size = 4 if lst[0][0] == "s" else 8
            total = size * len(lst)
            code = f"uint32_t b_ = c->r[{bk}]; uint32_t a_ = {'b_' if kind == 'ia' else f'b_ - {total}'}; "
            load = base in ("vpop", "vldmia", "vldmdb")
            for k, r in enumerate(lst):
                code += self.vload(r, f"a_ + {k * size}") if load else self.vstore(r, f"a_ + {k * size}")
            if wb:
                code += f"c->r[{bk}] = b_ {'+' if kind == 'ia' else '-'} {total}; "
            return "{ " + code + "}"

        if base == "vmov":
            if any(getattr(o, "vector_index", -1) not in (-1, None) for o in ops):
                raise Unsupported("vmov scalar")
            cls = "".join(self.vclass(o) for o in ops)
            if cls == "sr":
                return f"c->vf.s[{self.vr(ops[0])[1]}] = {self.rd(ops[1].reg)};"
            if cls == "rs":
                return self.write_reg(self.ridx(ops[0].reg), f"c->vf.s[{self.vr(ops[1])[1]}]")
            if cls == "rrd":
                k = self.vr(ops[2])[1]
                return (f"{{ uint64_t v_ = c->vf.d[{k}]; c->r[{self.ridx(ops[0].reg)}] = (uint32_t)v_; "
                        f"c->r[{self.ridx(ops[1].reg)}] = (uint32_t)(v_ >> 32); }}")
            if cls == "drr":
                return (f"c->vf.d[{self.vr(ops[0])[1]}] = (uint64_t){self.rd(ops[1].reg)} | "
                        f"((uint64_t){self.rd(ops[2].reg)} << 32);")
            if cls == "ssrr":
                return (f"{{ uint32_t v0_ = {self.rd(ops[2].reg)}, v1_ = {self.rd(ops[3].reg)}; "
                        f"c->vf.s[{self.vr(ops[0])[1]}] = v0_; c->vf.s[{self.vr(ops[1])[1]}] = v1_; }}")
            if cls == "rrss":
                return (f"{{ uint32_t v0_ = c->vf.s[{self.vr(ops[2])[1]}], v1_ = c->vf.s[{self.vr(ops[3])[1]}]; "
                        f"c->r[{self.ridx(ops[0].reg)}] = v0_; c->r[{self.ridx(ops[1].reg)}] = v1_; }}")
            if cls in ("ss", "dd"):
                return f"{self.vraw(self.vr(ops[0]))} = {self.vraw(self.vr(ops[1]))};"
            if cls == "sF":
                bits = struct.unpack("<I", struct.pack("<f", ops[1].fp))[0]
                return f"c->vf.s[{self.vr(ops[0])[1]}] = 0x{bits:X}u;"
            if cls == "dF":
                bits = struct.unpack("<Q", struct.pack("<d", ops[1].fp))[0]
                return f"c->vf.d[{self.vr(ops[0])[1]}] = 0x{bits:X}ull;"
            raise Unsupported(f"vmov form {cls}")

        if base == "vmrs":
            if len(ops) != 2 or self.vreg.get(ops[1].reg, ("",))[0] != "fpscr":
                raise Unsupported("vmrs source")
            if self.vreg.get(ops[0].reg, ("",))[0] == "apsr_nzcv":
                return "FPFLAGS(c);"
            return self.write_reg(self.ridx(ops[0].reg), "c->fpscr")
        if base == "vmsr":
            if len(ops) != 2 or self.vreg.get(ops[0].reg, ("",))[0] != "fpscr":
                raise Unsupported("vmsr target")
            return f"c->fpscr = {self.rd(ops[1].reg)};"

        if base in ("vcmp", "vcmpe"):
            a = self.vr(ops[0])
            if ops[1].type in (A.ARM_OP_IMM, A.ARM_OP_FP):
                b = "0.0"
            else:
                b = f"(double){self.vval(self.vr(ops[1]))}"
            return f"FCMP(c, (double){self.vval(a)}, {b});"

        if base == "vcvt":
            if len(ops) != 2 or len(dts) != 2:
                raise Unsupported(f"vcvt form {i.mnemonic}")
            to, frm = dts
            d, m = self.vr(ops[0]), self.vr(ops[1])
            fl = {"f32": "float", "f64": "double"}
            if to in fl and frm in fl:
                return f"{self.vval(d)} = ({fl[to]}){self.vval(m)};"
            if to in fl and frm in ("s32", "u32"):
                cast = "(int32_t)" if frm == "s32" else ""
                return f"{self.vval(d)} = ({fl[to]}){cast}c->vf.s[{m[1]}];"
            if to in ("s32", "u32") and frm in fl:
                return f"c->vf.s[{d[1]}] = CVT_{to.upper()}((double){self.vval(m)});"
            raise Unsupported(f"vcvt {i.mnemonic}")

        if base in ("vneg", "vabs"):
            d, m = self.vr(ops[0]), self.vr(ops[1])
            if d[0] == "s":
                op = "^ 0x80000000u" if base == "vneg" else "& 0x7FFFFFFFu"
            else:
                op = "^ 0x8000000000000000ull" if base == "vneg" else "& 0x7FFFFFFFFFFFFFFFull"
            return f"{self.vraw(d)} = {self.vraw(m)} {op};"
        if base == "vsqrt":
            d, m = self.vr(ops[0]), self.vr(ops[1])
            return f"{self.vval(d)} = {'sqrtf' if d[0] == 's' else 'sqrt'}({self.vval(m)});"

        if base in ("vadd", "vsub", "vmul", "vdiv", "vnmul", "vmla", "vmls", "vnmla", "vnmls"):
            if len(ops) == 3:
                d, n, m = (self.vr(o) for o in ops)
            elif len(ops) == 2:
                d, m = self.vr(ops[0]), self.vr(ops[1])
                n = d
            else:
                raise Unsupported("vfp arith operands")
            if not (d[0] == n[0] == m[0]):
                raise Unsupported("vfp mixed precision")
            t = "float" if d[0] == "s" else "double"
            vd, vn, vm = self.vval(d), self.vval(n), self.vval(m)
            if base in ("vadd", "vsub", "vmul", "vdiv"):
                op = {"vadd": "+", "vsub": "-", "vmul": "*", "vdiv": "/"}[base]
                return f"{{ {t} a_ = {vn}, b_ = {vm}; {vd} = a_ {op} b_; }}"
            # the product is rounded on its own, then added (VFP vmla is not fused)
            code = f"{{ {t} p_ = {vn} * {vm}; "
            code += {"vnmul": f"{vd} = -p_; ",
                     "vmla": f"{vd} = {vd} + p_; ",
                     "vmls": f"{vd} = {vd} - p_; ",
                     "vnmla": f"{vd} = -{vd} - p_; ",
                     "vnmls": f"{vd} = -{vd} + p_; "}[base]
            return code + "}"
        raise Unsupported(f"{i.mnemonic} {i.op_str}")

    def rd_reg_index(self, k):
        if k == 15:
            return f"0x{(self.pc + (8 if self.mode == ARM else 4)) & 0xFFFFFFFF:X}u"
        return f"c->r[{k}]"

    def uses_lr_as_data(self, i):
        """LR read or written other than saving it (push / str to sp) and returning through it (bx lr,
        mov pc, lr). Such functions (e.g. __gnu_thumb1_case_*) return to a computed address."""
        lr = [o for o in i.operands if o.type == A.ARM_OP_REG and self.reg_index.get(o.reg) == 14]
        mem_lr = [o for o in i.operands if o.type == A.ARM_OP_MEM and self.reg_index.get(o.mem.base) == 14]
        if mem_lr:
            return True
        if not lr:
            return False
        if i.id in (A.ARM_INS_PUSH, A.ARM_INS_STMDB, A.ARM_INS_BX):
            return False
        if i.id == A.ARM_INS_STR and i.operands[1].type == A.ARM_OP_MEM and                 self.reg_index.get(i.operands[1].mem.base) == 13:
            return False
        if i.id == A.ARM_INS_MOV and self.reg_index.get(i.operands[0].reg) == 15:
            return False
        return True

    # ------------------------------------------------------------------ one function
    def function(self, d, entry, mode, res):
        """C source for one function, from the analyzer's decode `res` (its "sizes" = instruction addresses).
        Instructions the lifter cannot translate become escapes to the emulator."""
        self.mode = mode
        addrs = sorted(res["sizes"])
        self.addrs = set(addrs)
        self.switch_at = {sw["at"]: list(sw["targets"]) for sw in res["switches"]}
        self.indirect = set(res["indirect_jumps"])
        self.escapes = 0
        self.uses_excl = False
        # return addresses of far-branch bls (jumps into this function or an absorbed fragment that set LR)
        self.far_rets = set()
        for pc in addrs:
            i = d.one(pc, mode)
            if i is not None and i.id in (A.ARM_INS_BL, A.ARM_INS_BLX) and i.operands and                     i.operands[0].type == A.ARM_OP_IMM:
                t = i.operands[0].imm
                if t in self.addrs and (t not in self.fn_mode or t in self.fragments) and                         pc not in self.switch_at and (pc + i.size) in self.addrs:
                    self.far_rets.add((pc + i.size) | (1 if mode == THUMB else 0))
        tb = 1 if mode == THUMB else 0
        lines = [f"void F_{entry:X}(CPU *c) {{", f"  goto L_{entry:X};"]   # code may sit below the entry
        for k, pc in enumerate(addrs):
            self.pc = pc
            i = d.one(pc, mode)
            ends = True
            if i is None:
                code = f"c->pc = 0x{pc | tb:X}u; return; /* undecodable */"
                self.escapes += 1
                nxt = pc + 2
            else:
                nxt = pc + i.size
                try:
                    if mode == THUMB and i.size == 4 and i.id not in (A.ARM_INS_BL, A.ARM_INS_BLX):
                        raise Unsupported("thumb2")
                    code = self.insn(i)
                    ends = self.ends_flow(i)
                except Unsupported as ex:
                    code = f"c->pc = 0x{pc | tb:X}u; return; /* escape: {str(ex)[:60]} */"
                    self.escapes += 1
            text = f"{i.mnemonic} {i.op_str}" if i is not None else "?"
            lines.append(f"L_{pc:X}: /* {text.replace('*/', '* /')} */ c->lpc = 0x{pc:X}u; c->icount++; {code}")
            if not ends and (k + 1 >= len(addrs) or addrs[k + 1] != nxt):
                if nxt in self.addrs:
                    lines.append(f"  goto L_{nxt:X};")
                else:
                    lines.append("  " + self.leave(nxt))
        lines.append("}")
        if self.uses_excl:   # exclusive monitor: address and value seen by the last ldrex
            lines.insert(1, "  uint32_t xa_ = 0xFFFFFFFFu, xv_ = 0; (void)xa_; (void)xv_;")
        return chr(10).join(lines)

    def ends_flow(self, i):
        """True when execution never continues at the next instruction."""
        uncond = i.cc in ALWAYS or self.mode == THUMB and i.id != A.ARM_INS_B
        if i.id == A.ARM_INS_B:
            return i.cc in ALWAYS
        if i.id in (A.ARM_INS_BL, A.ARM_INS_BLX):
            return False
        if not uncond:
            return False
        if i.id == A.ARM_INS_BX:
            return True
        return analyze.pc_written(i)
