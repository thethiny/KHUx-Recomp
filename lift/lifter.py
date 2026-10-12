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
        for rid in range(1, 200):
            n = cs_reg_name(rid)
            if not n:
                continue
            idx = {"sb": 9, "sl": 10, "fp": 11, "ip": 12, "sp": 13, "lr": 14, "pc": 15}.get(n)
            if idx is None and n.startswith("r") and n[1:].isdigit():
                idx = int(n[1:])
            if idx is not None:
                self.reg_index[rid] = idx
        self.img = img

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

    def write_reg(self, k, expr, interwork=False):
        """Assign a register; writing PC returns from the function (the emulator continues there).
        ARMv5: loads to PC (ldr/ldm/pop) interwork on bit 0, but data-processing writes (mov/add pc, ...) stay
        in the current instruction set - e.g. a Thumb switch's `mov pc, r0` with an even r0 stays Thumb."""
        if k == 15:
            if interwork:
                return f"c->pc = {expr}; return;"
            if self.mode == THUMB:
                return f"c->pc = ({expr}) | 1u; return;"
            return f"c->pc = ({expr}) & ~1u; return;"
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
        if self.uses_lr_as_data(i):
            raise Unsupported("uses lr as data")
        body = self.body(i, iid, ops)
        cc = i.cc
        if self.mode == ARM and cc not in ALWAYS and iid != A.ARM_INS_B:
            return f"if ({COND[cc]}) {{ {body} }}"
        return body

    def body(self, i, iid, ops):
        nxt = self.pc + i.size
        uf = i.update_flags

        # ---------------- branches
        if iid == A.ARM_INS_B:
            t = ops[0].imm
            go = f"goto L_{t:X};" if t in self.addrs else self.leave(t)
            if i.cc in ALWAYS:
                return go
            return f"if ({COND[i.cc]}) {{ {go} }}"
        if iid == A.ARM_INS_BX:
            return f"c->pc = {self.rd(ops[0].reg)}; return;"
        if iid in (A.ARM_INS_BL, A.ARM_INS_BLX):
            ret = nxt | (1 if self.mode == THUMB else 0)
            if ops[0].type == A.ARM_OP_IMM:
                t = ops[0].imm
                if self.pc in self.switch_at:
                    raise Unsupported("switch helper call")
                if t in self.addrs and t not in self.fn_mode:      # Thumb-1 far branch inside the function
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

        if iid == A.ARM_INS_CLZ:
            return self.write_reg(self.ridx(ops[0].reg), f"CLZ({self.rd(ops[1].reg)})")

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

    def loadstore(self, i, iid, ops):
        t = self.ridx(ops[0].reg)
        mem = ops[1]
        if mem.type != A.ARM_OP_MEM:
            raise Unsupported("ldr/str operand")
        base, off = self.mem_addr(mem)
        post = i.post_index if hasattr(i, "post_index") else False
        wb = i.writeback
        code = "uint32_t dc_ = 0; (void)dc_; "
        if post:
            if len(ops) < 3:
                raise Unsupported("post-index without offset")
            po = ops[2]
            if po.type == A.ARM_OP_IMM:
                v = po.imm
                if po.subtracted and v > 0:
                    v = -v
                poff = f"(uint32_t){v}" if v >= 0 else f"-(uint32_t){-v}"
            elif po.type == A.ARM_OP_REG:
                r = self.rd(po.reg)
                if po.shift.type not in (A.ARM_SFT_INVALID,) and po.shift.value:
                    typ, by_reg = SHIFTS[po.shift.type]
                    if by_reg:
                        raise Unsupported("register-shifted post index")
                    r = f"SHIFT({r}, {typ}, {po.shift.value}, c->c, &dc_)"
                poff = f"-(uint32_t){r}" if po.subtracted else f"(uint32_t){r}"
            else:
                raise Unsupported("post-index operand")
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
        post = getattr(i, "post_index", False)
        if post or i.writeback:
            if post:
                raise Unsupported("ldrd/strd post-index")
        code = "uint32_t dc_ = 0; (void)dc_; " + f"uint32_t a_ = {base} + {off}; "
        bk = self.ridx(mem.mem.base)
        if iid == A.ARM_INS_LDRD:
            code += f"uint32_t v0_ = R32(a_), v1_ = R32(a_ + 4); c->r[{t1}] = v0_; c->r[{t2}] = v1_; "
        else:
            code += f"W32(c, a_, {self.rd(ops[0].reg)}); W32(c, a_ + 4, {self.rd(ops[1].reg)}); "
        if i.writeback:
            code += f"c->r[{bk}] = a_; "
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
                code += f"c->pc = {ret}; return; "
        else:
            for k, r in enumerate(regs):
                val = self.rd_reg_index(r)
                code += f"W32(c, a_ + {4 * k}, {val}); "
            if wb:
                code += f"c->r[{bk}] = {newb}; "
        return "{ " + code + "}"

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
        self.switch_at = {sw["at"] for sw in res["switches"]}
        self.escapes = 0
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
            lines.append(f"L_{pc:X}: /* {text.replace('*/', '* /')} */ c->lpc = 0x{pc:X}u; {code}")
            if not ends and (k + 1 >= len(addrs) or addrs[k + 1] != nxt):
                if nxt in self.addrs:
                    lines.append(f"  goto L_{nxt:X};")
                else:
                    lines.append("  " + self.leave(nxt))
        lines.append("}")
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
