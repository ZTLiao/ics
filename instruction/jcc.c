10001a:   74 02                   je     10001e
IDEXW(0x74, J, jcc, 1)

decode_J:
nemu/src/isa/x86/local-include/decode.h
static inline def_DHelper(J) {
  decode_op_SI(s, id_dest, false);
  // the target address can be computed in the decode stage
  Log("id_dest->simm=%x, s->seq_pc=%x", id_dest->simm, s->seq_pc);
  s->jmp_pc = id_dest->simm + s->seq_pc;
}

decode_op_SI:
nemu/src/isa/x86/local-include/decode.h
static inline def_DopHelper(SI) {
  assert(op->width == 1 || op->width == 4);

  /* Use instr_fetch() to read `op->width' bytes of memory
   * pointed by 's->seq_pc'. Interpret the result as a signed immediate,
   * and call `operand_imm()` as following.
   *
   operand_imm(s, op, load_val, ???, op->width);
   */
  Log("op-width : %d", op->width);
  word_t imm = instr_fetch(&s->seq_pc, op->width);
  Log("BEFORE imm : %x", imm);
  if (op->width == 1) {
      if (imm >> 7) {
          imm |= 0xFFFFFF00;
      }
  } else if (op->width == 2) {
      if (imm >> 15) {
          imm |= 0xFFFF0000;
      }
  }
  Log("AFTER imm : %x", imm);
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
