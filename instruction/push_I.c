100075:   6a 01                   push   $0x1
IDEXW  (0x6a, I, push, 1)

decode_I:
nemu/src/isa/x86/local-include/decode.h
static inline def_DHelper(I) {
  decode_op_I(s, id_dest, true);
}

decode_op_I:
nemu/src/isa/x86/local-include/decode.h
static inline def_DopHelper(I) {
  /* pc here is pointing to the immediate */
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

rtl_li:
nemu/include/rtl/pseudo.h
static inline def_rtl(li, rtlreg_t* dest, const rtlreg_t imm) {
  rtl_addi(s, dest, rz, imm);
}

rtl_addi:
nemu/src/engine/interpreter/rtl-basic.h
def_rtl_compute_reg_imm(add)
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

c_add:
nemu/src/engine/interpreter/c_op.h
#define c_add(a, b) ((a) + (b))
