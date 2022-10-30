static inline def_EHelper(mov) {
  Log("mov...");
  Log("ddest = %x, dsrc1 = %x", *ddest, *dsrc1);
  operand_write(s, id_dest, dsrc1);
  print_asm_template2(mov);
}

static inline def_EHelper(push) {
  Log("push...");
  Log("*ddest = %x", *ddest);
  if (id_dest->type == OP_TYPE_MEM) {
     Log("BEFORE *ddest = %x, moff = %d", *ddest, s->isa.moff);
	 rtl_addi(s, ddest, s->isa.mbase, s->isa.moff);
     Log("AFTER *ddest = %x", *ddest);
  } else if (id_dest->type == OP_TYPE_IMM) {
	 if (id_dest->width == 1) {
	     *ddest = *ddest & 0xF;
	 } else if (id_dest-> width == 2) {
		 *ddest = *ddest & 0xFF;
	 }	
  } else if (id_dest->type == OP_TYPE_REG) {
	  Log("id_dest->reg : %d", id_dest->reg);
	  Log("reg_l(%d) = %x", id_dest->reg, reg_l(id_dest->reg));
  }
  rtl_push(s, ddest);
  print_asm_template1(push);
}

static inline def_EHelper(pop) {
  Log("pop...");
  rtl_pop(s, ddest);
  if (id_dest->type == OP_TYPE_REG) {
	  Log("id_dest->reg : %d", id_dest->reg);
	  reg_l(id_dest->reg) = *ddest;
	  *(&cpu.eax + id_dest->reg) = *ddest;
	  Log("reg_l(%d) = %x", id_dest->reg, reg_l(id_dest->reg));
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
	  reg_l(R_SP) = reg_l(R_BP);
	  cpu.esp = reg_l(R_BP);
  } else {
	  reg_l(R_ESP) = reg_l(R_EBP);
      cpu.esp = reg_l(R_EBP);
  }
  Log("AFTER R_ESP = %x, cpu.esp = %x", reg_l(R_ESP), cpu.esp);
  rtl_pop(s, s0);
  Log("pop ddest = %x", *s0);
  Log("BEFORE R_EBP = %x, cpu.ebp = %x", reg_l(R_EBP), cpu.ebp);
  if (s->isa.is_operand_size_16) {
	  reg_l(R_BP) = *s0 & 0xFF;
	  cpu.ebp = *s0 & 0xFF;
  } else {
	  reg_l(R_EBP) = *s0;
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
  id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
  rtl_sext(s, ddest, dsrc1, id_src1->width);
  operand_write(s, id_dest, ddest);
  print_asm_template2(movsx);
}

static inline def_EHelper(movzx) {
  id_dest->width = s->isa.is_operand_size_16 ? 2 : 4;
  operand_write(s, id_dest, dsrc1);
  print_asm_template2(movzx);
}

static inline def_EHelper(lea) {
  Log("lea...");
  rtl_addi(s, ddest, s->isa.mbase, s->isa.moff);
  operand_write(s, id_dest, ddest);
  if (id_dest->type == OP_TYPE_REG) {
	  reg_l(id_dest->reg) = *ddest;
	  *(&cpu.eax + id_dest->reg) = *ddest;
  }
  print_asm_template2(lea);
}
