static inline def_EHelper(mov) {
#ifdef LOG
  Log("mov...");
#endif
  operand_write(s, id_dest, dsrc1);
  print_asm_template2(mov);
}

static inline def_EHelper(push) {
#ifdef LOG
  Log("push...");
  Log("*ddest = %x", *ddest);
#endif
  if (id_dest->type == OP_TYPE_MEM) {
#ifdef LOG
     Log("BEFORE *ddest = %x, moff = %d", *ddest, s->isa.moff);
#endif
	 rtl_addi(s, ddest, s->isa.mbase, s->isa.moff);
	 rtl_lm(s, ddest, ddest, 0, id_dest->width);
#ifdef LOG
     Log("AFTER *ddest = %x", *ddest);
#endif
  } else if (id_dest->type == OP_TYPE_IMM) {
#ifdef LOG
	 Log("IMM BEFORE *ddest = %x", *ddest);
#endif
	 if (id_dest->width == 1) {
	     *ddest = *ddest & 0xFF;
	 } else if (id_dest-> width == 2) {
		 *ddest = *ddest & 0xFFFF;
	 }	
#ifdef LOG
	 Log("IMM AFTER *ddest = %x", *ddest);
#endif
  } else if (id_dest->type == OP_TYPE_REG) {
#ifdef LOG
	 Log("id_dest->reg : %d", id_dest->reg);
	 Log("reg_name : %s", reg_name(id_dest->reg, 4));
#endif
  }
  rtl_push(s, ddest);
  print_asm_template1(push);
}

static inline def_EHelper(pop) {
#ifdef LOG
  Log("pop...");
#endif
  rtl_pop(s, ddest);
  if (id_dest->type == OP_TYPE_REG) {
#ifdef LOG
	  Log("id_dest->reg : %d", id_dest->reg);
#endif
	  *(&cpu.eax + id_dest->reg) = *ddest;
#ifdef LOG
	  Log("&cpu.eax + %d = %x", id_dest->reg, *(&cpu.eax + id_dest->reg));
#endif
  }
  print_asm_template1(pop);
}

static inline def_EHelper(pusha) {
  TODO();
  print_asm("pusha");
}

static inline def_EHelper(popa) {
  TODO();
  print_asm("popa");
}

static inline def_EHelper(leave) {
#ifdef LOG
  Log("leave...");
  Log("BEFORE R_ESP = %x, R_EBP = %x, cpu.esp = %x", reg_l(R_ESP), reg_l(R_EBP), cpu.esp);
#endif
  if (id_dest->width == 2) {
	  cpu.esp = reg_l(R_BP);
  } else {
      cpu.esp = reg_l(R_EBP);
  }
#ifdef LOG
  Log("AFTER R_ESP = %x, cpu.esp = %x", reg_l(R_ESP), cpu.esp);
#endif
  rtl_pop(s, s0);
#ifdef LOG
  Log("pop ddest = %x", *s0);
  Log("BEFORE R_EBP = %x, cpu.ebp = %x", reg_l(R_EBP), cpu.ebp);
#endif
  if (s->isa.is_operand_size_16) {
	  cpu.ebp = *s0 & 0xFF;
  } else {
	  cpu.ebp = *s0;
  }
#ifdef LOG
  Log("AFTER R_EBP = %x, cpu.ebp = %x", reg_l(R_EBP), cpu.ebp);
#endif
  print_asm("leave");
}

static inline def_EHelper(cltd) {
#ifdef LOG
  Log("cltd...");
  Log("BEFORE cpu.eax = %d, cpu.edx = %d", cpu.eax, cpu.edx);
#endif
  if (s->isa.is_operand_size_16) {
    rtl_sext(s, &cpu.eax, &cpu.eax, 2);
  } else {
    rtl_sext(s, &cpu.eax, &cpu.eax, 4);
	if ((cpu.eax & 0x80000000) != 0) {
	  cpu.edx = 0xFFFFFFFF;
	} else {
	  cpu.edx = 0x00000000;
	}
  }
#ifdef LOG
  Log("AFTER cpu.eax = %d, cpu.edx = %d", cpu.eax, cpu.edx);
#endif
  print_asm(s->isa.is_operand_size_16 ? "cwtl" : "cltd");
}

static inline def_EHelper(cwtl) {
#ifdef LOG
  Log("cwtl...");
#endif
  if (s->isa.is_operand_size_16) {
    rtl_sext(s, &cpu.eax, &cpu.eax, 2);
  } else {
    rtl_sext(s, &cpu.eax, &cpu.eax, 4);
  }
  print_asm(s->isa.is_operand_size_16 ? "cbtw" : "cwtl");
}

static inline def_EHelper(movsx) {
#ifdef LOG
  Log("movsx...");
#endif
  id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
#ifdef LOG
  Log("dsrc1 = %x", *dsrc1);
#endif
  rtl_sext(s, ddest, dsrc1, id_src1->width);
  operand_write(s, id_dest, ddest);
  print_asm_template2(movsx);
}

static inline def_EHelper(movzx) {
#ifdef LOG
  Log("movzx...");
#endif
  id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
  operand_write(s, id_dest, dsrc1);
  print_asm_template2(movzx);
}

static inline def_EHelper(lea) {
#ifdef LOG
  Log("lea...");
#endif
  rtl_addi(s, ddest, s->isa.mbase, s->isa.moff);
#ifdef LOG
  Log("BEFORE ddest = %x", *ddest);
#endif
  operand_write(s, id_dest, ddest);
#ifdef LOG
  Log("ATFER ddest = %x", *ddest);
#endif
  print_asm_template2(lea);
}

static inline def_EHelper(movsb) {
#ifdef LOG
  Log("movsb...");
#endif
  if (s->isa.is_operand_size_16) {
    rtl_lr(s, s0, R_SI, 2);
    rtl_lr(s, s1, R_DI, 2);
  } else {
    rtl_lr(s, s0, R_ESI, 4);
    rtl_lr(s, s1, R_EDI, 4);
  }
  int IncDec = 0;
  switch (s->opcode) {
    case 0xa4: {
	  rtl_lm(s, s2, s0, 0, 1);
	  rtl_sm(s, s1, 0, s2, 1);
	  IncDec = 1;
	  break;
	}
	default: {
	  if (s->isa.is_operand_size_16) {
	    rtl_lm(s, s2, s0, 0, 2);
	    rtl_sm(s, s1, 0, s2, 2);
	    IncDec = 2;
	  } else {
	    rtl_lm(s, s2, s0, 0, 4);
	    rtl_sm(s, s1, 0, s2, 4);
	    IncDec = 4;
	  }
	  break;
	}
  }
  if (s->isa.is_operand_size_16) {
    rtl_addi(s, s2, s0, IncDec);
    rtl_sr(s, R_SI, s2, 2);
    rtl_addi(s, s2, s1, IncDec);
    rtl_sr(s, R_DI, s2, 2);
  } else {
    rtl_addi(s, s2, s0, IncDec);
    rtl_sr(s, R_ESI, s2, 4);
    rtl_addi(s, s2, s1, IncDec);
    rtl_sr(s, R_EDI, s2, 4);
  }
  print_asm("movsb");
}
