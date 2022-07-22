nemu/src/isa/x86/exec/exec.c
fetch_decode_exec:
IDEX(0x31, G2E, xor)

case 0x31:
	set_width(s, 0);
	decode_G2E(s);
	exec_xor(s);
	break;

nemu/include/cpu/exec.h
#define IDEXW(idx, id, ex, w) CASE_ENTRY(idx, concat(decode_, id), concat(exec_, ex), w)
#define IDEX(idx, id, ex) IDEXW(idx, id, ex, 0)
#define CASE_ENTRY(idx, id, ex, w) case idx: set_width(s, w); id(s); ex(s); break;

nemu/src/isa/x86/exec/exec.c
set_width:
if (width == 0) {
	width = s->isa.is_operand_size_16 ? 2 : 4;
}
s->src1.width = s->dest.width = s->src2.width = width;

nemu/include/cpu/decode.h
#define def_DHelper(name) void concat(decode_, name) (DecodeExecState *s)

nemu/src/isa/x86/local-include/decode.h
#define def_DopHelper(name) void concat(decode_op_, name) (DecodeExecState *s, Operand *op, bool load_val)

nemu/src/isa/x86/local-include/decode.h
decode_G2E:
def_DHelper(G2E)
decode_G2E(s)
operand_rm(s, id_dest, true, id_src1, true);

nemu/src/isa/x86/local-include/decode.h
operand_rm:
read_ModR_M(s, rm, load_rm_val, reg, load_reg_val);

nemu/src/isa/x86/decode.c
read_ModR_M:
void read_ModR_M(DecodeExecState *s, Operand *rm, bool load_rm_val, Operand *reg, bool load_reg_val) {
  ModR_M m;
  m.val = instr_fetch(&s->seq_pc, 1);
  s->isa.ext_opcode = m.opcode;
  if (reg != NULL) operand_reg(s, reg, load_reg_val, m.reg, reg->width);
  if (m.mod == 3) operand_reg(s, rm, load_rm_val, m.R_M, rm->width);
  else {
    load_addr(s, &m, rm);
    if (load_rm_val) rtl_lm(s, &rm->val, s->isa.mbase, s->isa.moff, rm->width);
    rm->preg = &rm->val;
  }
}

nemu/src/isa/x86/local-include/decode.h
operand_reg:
static inline void operand_reg(DecodeExecState *s, Operand *op, bool load_val, int r, int width) {
  op->type = OP_TYPE_REG;
  op->reg = r;

  if (width == 4) {
    op->preg = &reg_l(r);
  } else {
    assert(width == 1 || width == 2);
    op->preg = &op->val;
    if (load_val) rtl_lr(s, &op->val, r, width);
  }
  print_Dop(op->str, OP_STR_SIZE, "%%%s", reg_name(r, width));
}

nemu/src/isa/x86/exec/logic.h
exec_xor:
def_EHelper(xor)
rtl_xor(s, ddest, dest, dsrc1);

nemu/src/engine/interpreter/rtl-basic.h
def_rtl_compute_reg_imm(xor)

nemu/src/engine/interpreter/c_op.h
c_xor:
#define c_xor(a, b) ((a ^ b))
