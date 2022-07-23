#include "cc.h"

static inline def_EHelper(test) {
  TODO();
  print_asm_template2(test);
}

static inline def_EHelper(and) {
  TODO();
  print_asm_template2(and);
}

static inline def_EHelper(xor) {
  Log("xor...");
  Log("before reg_l(id_dest->reg) : %d, cpu.eax : %d, reg_l(id_dest->reg) == cpu.eax : %d", reg_l(id_dest->reg), cpu.eax, (reg_l(id_dest->reg) == cpu.eax));
  rtl_xor(s, s0, ddest, dsrc1);
  operand_write(s, id_dest, s0);
  if (id_dest->type == OP_TYPE_REG) {
	rtlreg_t* reg = &cpu.eax;
	reg = reg + id_dest->reg;
	*reg = *s0;
	Log("reg_l(id_dest->reg) : %d, cpu.eax : %d, reg_l(id_dest->reg) == cpu.eax : %d", reg_l(id_dest->reg), cpu.eax, (reg_l(id_dest->reg) == cpu.eax));
	Log("id_dest->type : %d, id_dest->reg : %d, id_dest->width : %d", id_dest->type, id_dest->reg, id_dest->width);
	Log("id_dest->reg : %d", reg_l(id_dest->reg));
	Log("ddest : %d", *ddest);
	Log("cpu.eax : %d", cpu.eax);
  }
  print_asm_template2(xor);
}

static inline def_EHelper(or) {
  TODO();
  print_asm_template2(or);
}

static inline def_EHelper(not) {
  TODO();
  print_asm_template1(not);
}

static inline def_EHelper(sar) {
  TODO();
  // unnecessary to update CF and OF in NEMU
  print_asm_template2(sar);
}

static inline def_EHelper(shl) {
  TODO();
  // unnecessary to update CF and OF in NEMU
  print_asm_template2(shl);
}

static inline def_EHelper(shr) {
  TODO();
  // unnecessary to update CF and OF in NEMU
  print_asm_template2(shr);
}


static inline def_EHelper(setcc) {
  uint32_t cc = s->opcode & 0xf;
  rtl_setcc(s, ddest, cc);
  operand_write(s, id_dest, ddest);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}
