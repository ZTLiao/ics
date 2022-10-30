83 e4 f0                and    $0xfffffff0,%esp
and Ib,Ev
11 100 100
IDEX(0x83, SI2E, gp1)

EX(4, and)

decode_SI2E:
nemu/src/isa/x86/local-include/decode.h
static inline def_DHelper(SI2E) {
  assert(id_dest->width == 2 || id_dest->width == 4);
  operand_rm(s, id_dest, true, NULL, false);
  id_src1->width = 1;
  decode_op_SI(s, id_src1, true);
  if (id_dest->width == 2) {
    *dsrc1 &= 0xffff;
  }
}

operand_rm:
nemu/src/isa/x86/local-include/decode.h
static inline void operand_rm(DecodeExecState *s, Operand *rm, bool load_rm_val, Operand *reg, bool load_reg_val) {
 read_ModR_M(s, rm, load_rm_val, reg, load_reg_val);
}

read_ModR_M:
nemu/src/isa/x86/decode.c
typedef union {
  struct {
    uint8_t R_M		:3;
    uint8_t reg		:3;
    uint8_t mod		:2;
  };
  struct {
    uint8_t dont_care	:3;
    uint8_t opcode		:3;
  };
  uint8_t val;
} ModR_M;

typedef union {
  struct {
    uint8_t base	:3;
    uint8_t index	:3;
    uint8_t ss		:2;
  };
  uint8_t val;
} SIB;
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

operand_reg:
nemu/src/isa/x86/local-include/decode.h
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

decode_op_SI:
nemu/src/isa/x86/local-include/decode.h
static inline def_DopHelper(SI) {
  assert(op->width == 1 || op->width == 4);
  word_t imm = instr_fetch(&s->seq_pc, op->width);
  operand_imm(s, op, load_val, imm, op->width);
}

operand_imm:
nemu/src/isa/x86/local-include/decode.h
static inline void operand_imm(DecodeExecState *s, Operand *op, bool load_val, word_t imm, int width) {
  op->type = OP_TYPE_IMM;
  op->imm = imm;
  if (load_val) {
    rtl_li(s, &op->val, imm);
    op->preg = &op->val;
  }
  print_Dop(op->str, OP_STR_SIZE, "$0x%x", imm);
}

exec_and:
nemu/src/isa/x86/exec/logic.h
static inline def_EHelper(and) {
  Log("and...");
  if (id_src1->type == OP_TYPE_IMM) {
     Log("dsrc1 : %d", *dsrc1);
     rtl_andi(s, ddest, rz, *dsrc1);
  } else {
     rtl_and(s, ddest, rz, dsrc1);
  }
  print_asm_template2(and);
}

rtl_and:
nemu/src/engine/interpreter/rtl-basic.h
 def_rtl_compute_reg_imm(and)

#define def_rtl_compute_reg(name) \
  static inline def_rtl(name, rtlreg_t* dest, const rtlreg_t* src1, const rtlreg_t* src2) { \
    *dest = concat(c_, name) (*src1, *src2); \
  }

#define def_rtl_compute_imm(name) \
  static inline def_rtl(name ## i, rtlreg_t* dest, const rtlreg_t* src1, const sword_t imm) { \
    *dest = concat(c_, name) (*src1, imm); \
  }

#define def_rtl_compute_reg_imm(name) \
  def_rtl_compute_reg(name) \
  def_rtl_compute_imm(name) \

c_and:
nemu/src/engine/interpreter/c_op.h
#define c_and(a, b) ((a) & (b))
