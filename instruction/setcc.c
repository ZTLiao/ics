nemu/src/isa/x86/exec/exec.c
EX   (0x0f, 2byte_esc)
IDEX (0x94, E, setcc)

decode_E:
nemu/src/isa/x86/local-include/decode.h
static inline def_DHelper(E) {
  operand_rm(s, id_dest, true, NULL, false);
}

exec_setcc:
nemu/src/isa/x86/exec/logic.h
 static inline def_EHelper(setcc) {
   uint32_t cc = s->opcode & 0xf;
   rtl_setcc(s, ddest, cc);
   operand_write(s, id_dest, ddest);
 
   print_asm("set%s %s", get_cc_name(cc), id_dest->str);
 } 

rtl_setcc:
nemu/src/isa/x86/exec/cc.h
static inline void rtl_setcc(DecodeExecState *s, rtlreg_t* dest, uint32_t subcode) {
  uint32_t invert = subcode & 0x1;

  // TODO: Query EFLAGS to determine whether the condition code is satisfied.
  // dest <- ( cc is satisfied ? 1 : 0)
  switch (subcode & 0xe) {
    case CC_O:
    case CC_B:
    case CC_E:
    case CC_BE:
    case CC_S:
    case CC_L:
    case CC_LE:
       TODO();
    case CC_P: panic("PF is not supported");
    default: panic("should not reach here");
  }

  if (invert) {
    rtl_xori(s, dest, dest, 0x1);
  }
  assert(*dest == 0 || *dest == 1);
}
