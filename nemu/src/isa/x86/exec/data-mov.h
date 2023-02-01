static inline def_EHelper(mov) {
  Log("mov...");
  operand_write(s, id_dest, dsrc1);
  print_asm_template2(mov);
}

static inline def_EHelper(push) {
  Log("push...");
  Log("*ddest = %x", *ddest);
  if (id_dest->type == OP_TYPE_MEM) {
     Log("BEFORE *ddest = %x, moff = %d", *ddest, s->isa.moff);
	 rtl_addi(s, ddest, s->isa.mbase, s->isa.moff);
	 rtl_lm(s, ddest, ddest, 0, id_dest->width);
     Log("AFTER *ddest = %x", *ddest);
  } else if (id_dest->type == OP_TYPE_IMM) {
	 Log("IMM BEFORE *ddest = %x", *ddest);
	 if (id_dest->width == 1) {
	     *ddest = *ddest & 0xFF;
	 } else if (id_dest-> width == 2) {
		 *ddest = *ddest & 0xFFFF;
	 }	
	 Log("IMM AFTER *ddest = %x", *ddest);
  } else if (id_dest->type == OP_TYPE_REG) {
	 Log("id_dest->reg : %d", id_dest->reg);
	 Log("reg_name : %s", reg_name(id_dest->reg, 4));
  }
  rtl_push(s, ddest);
  print_asm_template1(push);
}

static inline def_EHelper(pop) {
  Log("pop...");
  rtl_pop(s, ddest);
  if (id_dest->type == OP_TYPE_REG) {
	  Log("id_dest->reg : %d", id_dest->reg);
	  *(&cpu.eax + id_dest->reg) = *ddest;
	  Log("&cpu.eax + %d = %x", id_dest->reg, *(&cpu.eax + id_dest->reg));
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
  Log("leave...");
  Log("BEFORE R_ESP = %x, R_EBP = %x, cpu.esp = %x", reg_l(R_ESP), reg_l(R_EBP), cpu.esp);
  if (id_dest->width == 2) {
	  cpu.esp = reg_l(R_BP);
  } else {
      cpu.esp = reg_l(R_EBP);
  }
  Log("AFTER R_ESP = %x, cpu.esp = %x", reg_l(R_ESP), cpu.esp);
  rtl_pop(s, s0);
  Log("pop ddest = %x", *s0);
  Log("BEFORE R_EBP = %x, cpu.ebp = %x", reg_l(R_EBP), cpu.ebp);
  if (s->isa.is_operand_size_16) {
	  cpu.ebp = *s0 & 0xFF;
  } else {
	  cpu.ebp = *s0;
  }
  Log("AFTER R_EBP = %x, cpu.ebp = %x", reg_l(R_EBP), cpu.ebp);
  print_asm("leave");
}

static inline def_EHelper(cltd) {
  Log("cltd...");
  Log("BEFORE cpu.eax = %d, cpu.edx = %d", cpu.eax, cpu.edx);
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
  Log("AFTER cpu.eax = %d, cpu.edx = %d", cpu.eax, cpu.edx);
  print_asm(s->isa.is_operand_size_16 ? "cwtl" : "cltd");
}

static inline def_EHelper(cwtl) {
  if (s->isa.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }
  print_asm(s->isa.is_operand_size_16 ? "cbtw" : "cwtl");
}

static inline def_EHelper(movsx) {
  Log("movsx...");
  id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
  Log("dsrc1 = %x", *dsrc1);
  rtl_sext(s, ddest, dsrc1, id_src1->width);
  operand_write(s, id_dest, ddest);
  print_asm_template2(movsx);
}

static inline def_EHelper(movzx) {
  Log("movzx...");
  id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
  operand_write(s, id_dest, dsrc1);
  print_asm_template2(movzx);
}

static inline def_EHelper(lea) {
  Log("lea...");
  rtl_addi(s, ddest, s->isa.mbase, s->isa.moff);
  Log("BEFORE ddest = %x", *ddest);
  operand_write(s, id_dest, ddest);
  Log("ATFER ddest = %x", *ddest);
  print_asm_template2(lea);
}
