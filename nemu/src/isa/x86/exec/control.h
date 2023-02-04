#include "cc.h"

static inline def_EHelper(jmp) {
  Log("jmp...");
  // the target address is calculated at the decode stage
  rtl_j(s, s->jmp_pc);
  Log("s->jmp_pc = %x", s->jmp_pc);
  print_asm("jmp %x", s->jmp_pc);
}

static inline def_EHelper(jcc) {
  // the target address is calculated at the decode stage
  Log("jcc..");
  uint32_t cc = s->opcode & 0xf;
  rtl_setcc(s, s0, cc);
  Log("s0 : %x", *s0);
  rtl_jrelop(s, RELOP_NE, s0, rz, s->jmp_pc);

  print_asm("j%s %x", get_cc_name(cc), s->jmp_pc);
}

static inline def_EHelper(jmp_rm) {
  rtl_jr(s, ddest);

  print_asm("jmp *%s", id_dest->str);
}

static inline def_EHelper(call) {
  // the target address is calculated at the decode stage
  Log("call...");
  rtl_j(s, s->jmp_pc);
  Log("s->seq_pc = %x", s->seq_pc);
  rtl_push(s, &s->seq_pc);

  print_asm("call %x", s->jmp_pc);
}

static inline def_EHelper(ret) {
  Log("ret...");
  switch(s->opcode) {
	case 0xc2:
	case 0xc3:
		rtl_pop(s, s0);
		Log("*dsrc1 = %x", *s0);
		s->seq_pc = *s0;
		if (s->isa.is_operand_size_16) {
			s->seq_pc = s->seq_pc & 0x0000FFFF;
		}	
		break;
	default:
		break;
  }
  print_asm("ret");
}

static inline def_EHelper(ret_imm) {
  TODO();
  print_asm("ret %s", id_dest->str);
}

static inline def_EHelper(call_rm) {
  Log("call_rm...");
  rtl_j(s, *ddest);
  Log("ddest = %X, s->seq_pc = %X", *ddest, s->seq_pc);
  rtl_push(s, &s->seq_pc);
  print_asm("call *%s", id_dest->str);
}
