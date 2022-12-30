#include "cc.h"

static inline def_EHelper(test) {
  Log("test...");
  Log("id_dest->reg : %d, id_src1->reg : %d", id_dest->reg, id_src1->reg);
  if (id_dest->type == OP_TYPE_REG) {
    if (id_dest->width == 1) {
      Log("ddest : %x, dsrc1 : %x", reg_b(id_dest->reg), reg_b(id_src1->reg));
	  rtl_and(s, s0, (rtlreg_t *) &reg_b(id_dest->reg), (rtlreg_t *) &reg_b(id_src1->reg));	
    } else {
      Log("ddest : %x, dsrc1 : %x", reg_l(id_dest->reg), reg_l(id_src1->reg));
	  rtl_and(s, s0, (rtlreg_t *) &reg_l(id_dest->reg), (rtlreg_t *) &reg_l(id_src1->reg));	
    }
  } else {
	if (id_src1->type == OP_TYPE_REG) {
	  if (id_src1->width == 1) {
	    rtl_and(s, s0, ddest, (rtlreg_t *) &reg_b(id_src1->reg));
	  } else {
	    rtl_and(s, s0, ddest, (rtlreg_t *) &reg_l(id_src1->reg));
	  }
	}
	Log("test ddest = %x", *ddest);
  }
  Log("s0 : %x", *s0);
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
  } else if (id_src1->type == OP_TYPE_MEM) {
	 if (id_dest->type == OP_TYPE_REG) {
	   if (id_dest->width == 1) {
         rtl_and(s, (rtlreg_t *) &reg_b(id_dest->reg), (rtlreg_t *) &reg_b(id_dest->reg), dsrc1);
	   } else if (id_dest->width == 2) {
         rtl_and(s, (rtlreg_t *) &reg_w(id_dest->reg), (rtlreg_t *) &reg_w(id_dest->reg), dsrc1);
	   } else {
         rtl_and(s, (rtlreg_t *) &reg_l(id_dest->reg), (rtlreg_t *) &reg_l(id_dest->reg), dsrc1);
	   }
	 } else {
       rtl_and(s, ddest, ddest, dsrc1);
	 }
  } else {
     rtl_and(s, ddest, ddest, dsrc1);
  }
  rtl_set_CF(s, rz);
  rtl_set_OF(s, rz);
  print_asm_template2(and);
}

static inline def_EHelper(xor) {
  Log("xor...");
  Log("before reg_l(id_dest->reg) : %d, cpu.eax : %d, reg_l(id_dest->reg) == cpu.eax : %d", reg_l(id_dest->reg), cpu.eax, (reg_l(id_dest->reg) == cpu.eax));
  Log("before ddest = %p", ddest);
  rtl_xor(s, s0, ddest, dsrc1);
  Log("after ddest = %p", ddest);
  if (id_dest->type == OP_TYPE_REG) {
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
  Log("or...");
  if (id_src1->type == OP_TYPE_IMM) {
     Log("ddest : %x, dsrc1 : %x", *ddest, *dsrc1);
     rtl_ori(s, ddest, ddest, *dsrc1);
  } else {
     rtl_or(s, ddest, ddest, dsrc1);
  }
  rtl_update_ZFSF(s, ddest, id_dest->width);
  rtl_update_PF(s, ddest, id_dest->width);
  rtl_set_CF(s, rz);
  rtl_set_OF(s, rz);
  print_asm_template2(or);
}

static inline def_EHelper(not) {
  Log("not...");
  if (id_dest->type == OP_TYPE_IMM) {
    rtl_not(s, ddest, ddest);
  } else if (id_dest->type == OP_TYPE_REG) {
	if (id_dest->width == 1) {
      rtl_not(s, (rtlreg_t *) &reg_b(id_dest->reg), (rtlreg_t *) &reg_b(id_dest->reg));
	} else if (id_dest->width == 2) {
      rtl_not(s, (rtlreg_t *) &reg_w(id_dest->reg), (rtlreg_t *) &reg_w(id_dest->reg));
	} else {
      rtl_not(s, (rtlreg_t *) &reg_l(id_dest->reg), (rtlreg_t *) &reg_l(id_dest->reg));
	}
  }
  print_asm_template1(not);
}

static inline def_EHelper(sar) {
  Log("sar...");
  int temp = 0;
  if (id_src1->type == OP_TYPE_IMM) {
    temp = *dsrc1;
  } else if (id_src1->type == OP_TYPE_REG) {
	if (id_src1->width == 1) {
	  temp = reg_b(id_src1->reg);
	} else if (id_src1->width == 2) {
	  temp = reg_w(id_src1->reg);
	} else {
	  temp = reg_l(id_src1->reg);
	}
  }
  Log("temp = %x", temp);
  while (temp != 0) {
	if (id_dest->type == OP_TYPE_REG) {
	  *(&cpu.eax + id_dest->reg) = reg_l(id_dest->reg) / 2;
    }
	temp = temp - 1;
  }
  if (id_dest->type == OP_TYPE_REG) {
    Log("dsrc1 = %x, ddest = %x", *dsrc1, reg_l(id_dest->reg));
  }
  // unnecessary to update CF and OF in NEMU
  print_asm_template2(sar);
}

static inline def_EHelper(shl) {
  Log("shl...");
  int temp = 0;
  if (id_src1->type == OP_TYPE_IMM) {
	Log("IMM...");
    temp = *dsrc1;
  } else if (id_src1->type == OP_TYPE_REG) {
	Log("REG...");
	if (id_src1->width == 1) {
	  temp = reg_b(id_src1->reg);
	} else if (id_src1->width == 2) {
	  temp = reg_w(id_src1->reg);
	} else {
	  temp = reg_l(id_src1->reg);
	}
  }
  if (id_dest->type == OP_TYPE_REG) {
    Log("shl ddest = %x", reg_l(id_dest->reg));
  }
  Log("temp = %x, dsrc1 = %x", temp, *dsrc1);
  while (temp != 0) {
	if (id_dest->type == OP_TYPE_REG) {
	  *(&cpu.eax + id_dest->reg) = reg_l(id_dest->reg) * 2;
    }
	temp = temp - 1;
  }
  if (id_dest->type == OP_TYPE_REG) {
    Log("dsrc1 = %x, ddest = %x", *dsrc1, reg_l(id_dest->reg));
  }
  // unnecessary to update CF and OF in NEMU
  print_asm_template2(shl);
}

static inline def_EHelper(shr) {
  Log("shr...");
  if (id_src1->type == OP_TYPE_IMM) {
     Log("ddest : %x, dsrc1 : %x", *ddest, *dsrc1);
     rtl_shri(s, ddest, ddest, *dsrc1);
  } else {
     rtl_shr(s, ddest, ddest, dsrc1);
  }
  Log("shr ddest : %x", *ddest);
  // unnecessary to update CF and OF in NEMU
  print_asm_template2(shr);
}


static inline def_EHelper(setcc) {
  uint32_t cc = s->opcode & 0xf;
  rtl_setcc(s, ddest, cc);
  operand_write(s, id_dest, ddest);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}
