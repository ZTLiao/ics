#include "cc.h"

static inline def_EHelper(test) {
  Log("test...");
  if (id_src1->type == OP_TYPE_IMM) {
	  rtl_andi(s, s0, ddest, *dsrc1);
  } else {
	  rtl_and(s, s0, ddest, dsrc1);	
  }
  rtl_update_ZFSF(s, s0, id_dest->width);
  rtl_update_PF(s, s0, id_dest->width);
  rtl_set_CF(s, rz);
  rtl_set_OF(s, rz);
  print_asm_template2(test);
}

static inline def_EHelper(and) {
  Log("and...");
  if (id_src1->type == OP_TYPE_IMM) {
     Log("ddest : %x, dsrc1 : %x", *ddest, *dsrc1);
     rtl_andi(s, ddest, ddest, *dsrc1);
  } else {
     rtl_and(s, ddest, ddest, dsrc1);
  }
  print_asm_template2(and);
}

static inline def_EHelper(xor) {
  Log("xor...");
  Log("before reg_l(id_dest->reg) : %d, cpu.eax : %d, reg_l(id_dest->reg) == cpu.eax : %d", reg_l(id_dest->reg), cpu.eax, (reg_l(id_dest->reg) == cpu.eax));
  Log("before ddest = %p", ddest);
  rtl_xor(s, s0, ddest, dsrc1);
  Log("after ddest = %p", ddest);
  if (id_dest->type == OP_TYPE_REG) {
	reg_l(id_dest->reg) = *s0;
	*(&cpu.eax + id_dest->reg) = *s0;
	Log("reg_l(id_dest->reg) : %d, cpu.eax : %d, reg_l(id_dest->reg) == cpu.eax : %d", reg_l(id_dest->reg), cpu.eax, (reg_l(id_dest->reg) == cpu.eax));
	Log("id_dest->type : %d, id_dest->reg : %d, id_dest->width : %d", id_dest->type, id_dest->reg, id_dest->width);
	Log("id_dest->reg : %d", reg_l(id_dest->reg));
	Log("ddest : %d", *ddest);
	Log("cpu.eax : %d", cpu.eax);
  }
  rtl_update_ZFSF(s, s0, id_dest->width);
  rtl_update_PF(s, s0, id_dest->width);
  rtl_set_CF(s, rz);
  rtl_set_OF(s, rz);
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
