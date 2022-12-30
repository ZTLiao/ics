static inline def_EHelper(mov) {
  Log("mov...");
  Log("before ddest = %x, dsrc1 = %x, id_src1->type = %d", *ddest, *dsrc1, id_src1->type);
  operand_write(s, id_dest, dsrc1);
  Log("after ddest = %x, dsrc1 = %x", *ddest, *dsrc1);
  if (id_dest->type == OP_TYPE_REG) {
	if (id_dest->width == 1) {
      Log("id_dest->reg = %d, reg_b(%d) = %x", id_dest->reg, id_dest->reg, reg_b(id_dest->reg));
	} else if (id_dest->width == 4) {
      Log("id_dest->reg = %d, reg_l(%d) = %x", id_dest->reg, id_dest->reg, reg_l(id_dest->reg));
	}
  }
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
  if (s->isa.is_operand_size_16) {
    TODO();
  }
  else {
    TODO();
  }
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
