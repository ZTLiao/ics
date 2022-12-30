#ifndef __CC_H__
#define __CC_H__

#include "../local-include/rtl.h"

enum {
  CC_O, CC_NO, CC_B,  CC_NB,
  CC_E, CC_NE, CC_BE, CC_NBE,
  CC_S, CC_NS, CC_P,  CC_NP,
  CC_L, CC_NL, CC_LE, CC_NLE
};


/* Condition Code */

static inline const char* get_cc_name(int subcode) {
  static const char *cc_name[] = {
    "o", "no", "b", "nb",
    "e", "ne", "be", "nbe",
    "s", "ns", "p", "np",
    "l", "nl", "le", "nle"
  };
  return cc_name[subcode];
}

static inline void rtl_setcc(DecodeExecState *s, rtlreg_t* dest, uint32_t subcode) {
  uint32_t invert = subcode & 0x1;
  Log("setcc...");
  // TODO: Query EFLAGS to determine whether the condition code is satisfied.
  // dest <- ( cc is satisfied ? 1 : 0)
  switch (subcode & 0xe) {
    case CC_O:
    case CC_B:
    case CC_E:
	  Log("cpu.eflags.ZF = %d", cpu.eflags.ZF);
	  *dest = cpu.eflags.ZF;
	  break;
    case CC_BE:
	  Log("cpu.eflags.CF = %d, cpu.eflags.ZF = %d", cpu.eflags.CF, cpu.eflags.ZF);
	  *dest = cpu.eflags.CF || cpu.eflags.ZF;
	  break;
    case CC_S:
	  Log("cpu.eflags.SF = %d", cpu.eflags.SF);
	  *dest = cpu.eflags.SF;
	  break;
    case CC_L:
	  Log("cpu.eflags.SF = %d, cpu.eflags.OF = %d", cpu.eflags.SF, cpu.eflags.OF);
	  *dest = cpu.eflags.SF != cpu.eflags.OF;
	  break;
    case CC_LE:
       Log("cpu.eflags.ZF = %d, cpu.eflags.SF = %d, cpu.eflags.OF = %d", cpu.eflags.ZF, cpu.eflags.SF, cpu.eflags.OF);
	   *dest = cpu.eflags.ZF || (cpu.eflags.SF != cpu.eflags.OF); 
	   break;
    case CC_P: panic("PF is not supported");
    default: panic("should not reach here");
  }

  if (invert) {
	Log("invert...");
    rtl_xori(s, dest, dest, 0x1);
  }
  assert(*dest == 0 || *dest == 1);
}

#endif
