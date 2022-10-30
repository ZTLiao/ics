10001c:   c9                      leave
EX   (0xc9, leave)

exec_leave:
nemu/src/isa/x86/exec/data-mov.h
static inline def_EHelper(leave) {
  Log("leave...");
  Log("BEFORE R_ESP = %x", reg_l(R_ESP));
  if (id_dest->width == 2) {
      reg_l(R_SP) = reg_l(R_BP);
  } else {
      reg_l(R_ESP) = reg_l(R_EBP);
  }
  Log("AFTER R_ESP = %x", reg_l(R_ESP));
  rtl_pop(s, ddest);
  Log("pop ddest = %x", *ddest);
  Log("BEFORE R_EBP = %x", reg_l(R_EBP));
  if (s->isa.is_operand_size_16) {
      rtl_sm(s, &reg_l(R_BP), 0, ddest, id_dest->width);
  } else {
      rtl_sm(s, &reg_l(R_EBP), 0, ddest, id_dest->width);
  }
  Log("AFTER R_EBP = %x", reg_l(R_EBP));
  print_asm("leave");
} 
