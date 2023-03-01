#include "cc.h"

static inline def_EHelper(test) {
#ifdef LOG
  Log("test...");
  Log("id_dest->type : %d, id_src1->type : %d", id_dest->type, id_src1->type);
#endif
  if (id_dest->type == OP_TYPE_REG) {
	if (id_src1->type == OP_TYPE_REG) {
      if (id_dest->width == 1) {
#ifdef LOG
        Log("ddest : %x, dsrc1 : %x", reg_b(id_dest->reg), reg_b(id_src1->reg));
#endif
	    rtl_and(s, s0, (rtlreg_t *) &reg_b(id_dest->reg), (rtlreg_t *) &reg_b(id_src1->reg));	
      } else {
#ifdef LOG
        Log("ddest : %x, dsrc1 : %x", reg_l(id_dest->reg), reg_l(id_src1->reg));
#endif
	    rtl_and(s, s0, (rtlreg_t *) &reg_l(id_dest->reg), (rtlreg_t *) &reg_l(id_src1->reg));	
	  }
    } else if (id_src1->type == OP_TYPE_IMM) {
      if (id_dest->width == 1) {
#ifdef LOG
        Log("ddest : %x, dsrc1 : %x", reg_b(id_dest->reg), *dsrc1);
#endif
	    rtl_and(s, s0, (rtlreg_t *) &reg_b(id_dest->reg), dsrc1);	
      } else {
#ifdef LOG
        Log("ddest : %x, dsrc1 : %x", reg_l(id_dest->reg), *dsrc1);
#endif
	    rtl_and(s, s0, (rtlreg_t *) &reg_l(id_dest->reg), dsrc1);	
	  }
	}
  } else {
	if (id_src1->type == OP_TYPE_REG) {
	  if (id_src1->width == 1) {
	    rtl_and(s, s0, ddest, (rtlreg_t *) &reg_b(id_src1->reg));
	  } else {
	    rtl_and(s, s0, ddest, (rtlreg_t *) &reg_l(id_src1->reg));
	  }
	}
#ifdef LOG
	Log("test ddest = %x", *ddest);
#endif
  }
#ifdef LOG
  Log("s0 : %x", *s0);
#endif
  rtl_update_ZFSF(s, s0, id_dest->width);
  rtl_update_PF(s, s0, id_dest->width);
  rtl_set_CF(s, rz);
  rtl_set_OF(s, rz);
  print_asm_template2(test);
}

static inline def_EHelper(and) {
#ifdef LOG
  Log("and...");
#endif
  if (id_src1->type == OP_TYPE_IMM) {
     rtl_andi(s, ddest, ddest, *dsrc1);
  } else if (id_src1->type == OP_TYPE_MEM) {
	 if (id_dest->type == OP_TYPE_REG) {
	   if (id_dest->width == 1) {
         rtl_and(s, s0, (rtlreg_t *) &reg_b(id_dest->reg), dsrc1);
		 reg_b(id_dest->reg) = (0xFF & *s0);
	   } else if (id_dest->width == 2) {
         rtl_and(s, s0, (rtlreg_t *) &reg_w(id_dest->reg), dsrc1);
		 reg_w(id_dest->reg) = (0xFFFF & *s0);
	   } else {
         rtl_and(s, s0, (rtlreg_t *) &reg_l(id_dest->reg), dsrc1);
		 reg_l(id_dest->reg) = *s0;
	   }
	 } else {
       rtl_and(s, ddest, ddest, dsrc1);
	 }
  } else {
     rtl_and(s, ddest, ddest, dsrc1);
  }
#ifdef LOG
  Log("ddest : %x, dsrc1 : %x", *ddest, *dsrc1);
#endif
  rtl_set_CF(s, rz);
  rtl_set_OF(s, rz);
  print_asm_template2(and);
}

static inline def_EHelper(xor) {
#ifdef LOG
  Log("xor...");
  Log("before reg_l(id_dest->reg) : %d, cpu.eax : %d, reg_l(id_dest->reg) == cpu.eax : %d", reg_l(id_dest->reg), cpu.eax, (reg_l(id_dest->reg) == cpu.eax));
  Log("before ddest = %p", ddest);
#endif 
  rtl_xor(s, s0, ddest, dsrc1);
#ifdef LOG
  Log("after ddest = %p", ddest);
#endif
  if (id_dest->type == OP_TYPE_REG) {
	*(&cpu.eax + id_dest->reg) = *s0;
#ifdef LOG
	Log("reg_l(id_dest->reg) : %d, cpu.eax : %d, reg_l(id_dest->reg) == cpu.eax : %d", reg_l(id_dest->reg), cpu.eax, (reg_l(id_dest->reg) == cpu.eax));
	Log("id_dest->type : %d, id_dest->reg : %d, id_dest->width : %d", id_dest->type, id_dest->reg, id_dest->width);
	Log("id_dest->reg : %d", reg_l(id_dest->reg));
	Log("ddest : %d", *ddest);
	Log("cpu.eax : %d", cpu.eax);
#endif
  }
  rtl_update_ZFSF(s, s0, id_dest->width);
  rtl_update_PF(s, s0, id_dest->width);
  rtl_set_CF(s, rz);
  rtl_set_OF(s, rz);
  print_asm_template2(xor);
}

static inline def_EHelper(or) {
#ifdef LOG
  Log("or...");
#endif
  if (id_src1->type == OP_TYPE_IMM) {
     rtl_ori(s, ddest, ddest, *dsrc1);
  } else if (id_src1->type == OP_TYPE_MEM) {
	 if (id_dest->type == OP_TYPE_REG) {
	   if (id_dest->width == 1) {
         rtl_or(s, s0, (rtlreg_t *) &reg_b(id_dest->reg), dsrc1);
		 reg_b(id_dest->reg) = (0xFF & *s0);
	   } else if (id_dest->width == 2) {
         rtl_or(s, s0, (rtlreg_t *) &reg_w(id_dest->reg), dsrc1);
		 reg_w(id_dest->reg) = (0xFFFF & *s0);
	   } else {
         rtl_or(s, s0, (rtlreg_t *) &reg_l(id_dest->reg), dsrc1);
		 reg_l(id_dest->reg) = *s0;
	   }
	 } else {
       rtl_or(s, ddest, ddest, dsrc1);
	 }
  } else {
     rtl_or(s, ddest, ddest, dsrc1);
  }
#ifdef LOG
  Log("s0 = %x, ddest : %x, dsrc1 : %x", *s0, *ddest, *dsrc1);
#endif
  rtl_update_ZFSF(s, ddest, id_dest->width);
  rtl_update_PF(s, ddest, id_dest->width);
  rtl_set_CF(s, rz);
  rtl_set_OF(s, rz);
  print_asm_template2(or);
}

static inline def_EHelper(not) {
#ifdef LOG
  Log("not...");
#endif
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
#ifdef LOG
  if (id_dest->type == OP_TYPE_REG) {
	Log("reg ddest = %x", reg_l(id_dest->reg));
  } else {
    Log("other ddest = %x", *ddest);
  }
#endif
  print_asm_template1(not);
}

static inline def_EHelper(sar) {
#ifdef LOG
  Log("sar...");
#endif
  int temp = 0;
  if (id_src1->type == OP_TYPE_IMM) {
    temp = *dsrc1;
  } else if (id_src1->type == OP_TYPE_REG) {
    temp = reg_b(id_src1->reg);
  }
#ifdef LOG
  Log("temp = %X, id_dest->type = %d", temp, id_dest->type);
#endif
  if (id_dest->type == OP_TYPE_REG) {
    rtlreg_t reg_val = reg_l(id_dest->reg);
	int sign = 0;
	int width = id_dest->width;
	if (width == 1) {
	  sign = (reg_val & 0x80) >> 7;
    } else if (width == 2) {
	  sign = (reg_val & 0x8000) >> 15;
	} else if (width == 4) {
	  sign = (reg_val & 0x80000000) >> 31;
	}
	if (sign) {
	  if (width == 1) {
	    sign = 0x80;
	  } else if (width == 2) {
	    sign = 0x8000;
	  } else if (width == 4) {
	    sign = 0x80000000;
	  }
	}
	while (temp != 0) {
	  reg_val = (reg_val / 2) | sign;
	  sign = sign / 2;
	  temp = temp - 1;
    }
	*(&cpu.eax + id_dest->reg) = reg_val;
  }
#ifdef LOG
  if (id_dest->type == OP_TYPE_REG) {
    Log("dsrc1 = %X, id_dest->reg = %d, ddest = %X", *dsrc1, id_dest->reg, reg_l(id_dest->reg));
  }
#endif
  // unnecessary to update CF and OF in NEMU
  print_asm_template2(sar);
}

static inline def_EHelper(shl) {
#ifdef LOG
  Log("shl...");
#endif
  int temp = 0;
  if (id_src1->type == OP_TYPE_IMM) {
#ifdef LOG
	Log("IMM...");
#endif
    temp = *dsrc1;
  } else if (id_src1->type == OP_TYPE_REG) {
#ifdef LOG
	Log("REG...");
#endif
	if (id_src1->width == 1) {
	  temp = reg_b(id_src1->reg);
	} else if (id_src1->width == 2) {
	  temp = reg_w(id_src1->reg);
	} else {
	  temp = reg_l(id_src1->reg);
	}
  }
#ifdef LOG
  if (id_dest->type == OP_TYPE_REG) {
    Log("shl ddest = %x", reg_l(id_dest->reg));
  }
  Log("temp = %x, dsrc1 = %x", temp, *dsrc1);
#endif
  while (temp != 0) {
	if (id_dest->type == OP_TYPE_REG) {
	  *(&cpu.eax + id_dest->reg) = reg_l(id_dest->reg) * 2;
    }
	temp = temp - 1;
  }
#ifdef LOG
  if (id_dest->type == OP_TYPE_REG) {
    Log("dsrc1 = %x, ddest = %x", *dsrc1, reg_l(id_dest->reg));
  }
#endif
  // unnecessary to update CF and OF in NEMU
  print_asm_template2(shl);
}

static inline def_EHelper(shr) {
#ifdef LOG
  Log("shr...");
#endif
  if (id_src1->type == OP_TYPE_IMM) {
#ifdef LOG
     Log("ddest : %x, dsrc1 : %x", *ddest, *dsrc1);
#endif
     rtl_shri(s, ddest, ddest, *dsrc1);
  } else {
     rtl_shr(s, ddest, ddest, dsrc1);
  }
#ifdef LOG
  Log("shr ddest : %x", *ddest);
#endif
  // unnecessary to update CF and OF in NEMU
  print_asm_template2(shr);
}


static inline def_EHelper(setcc) {
#ifdef LOG
  Log("setcc...");
#endif
  uint32_t cc = s->opcode & 0xf;
  rtl_setcc(s, s0, cc);
#ifdef LOG
  Log("s0 = %d", *s0);
#endif
  operand_write(s, id_dest, s0);

  print_asm("set%s %s", get_cc_name(cc), id_dest->str);
}

static inline def_EHelper(bsr) {
#ifdef LOG
  Log("bsr...");
#endif
  if (id_src1->val == 0) {
    rtl_set_ZF(s, rz);
  } else {
    int temp = id_dest->width * 8;
	rtl_set_ZF(s, rz);
	while (((id_src1->val >> temp) & 0x1) == 0) {
	  temp = temp - 1;
	  rtl_sr(s, id_dest->reg, dsrc1, id_dest->width);
    } 
  }
  print_asm_template2(bsr);
}
