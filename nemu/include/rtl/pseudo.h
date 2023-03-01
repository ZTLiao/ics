#ifndef __RTL_PSEUDO_H__
#define __RTL_PSEUDO_H__

#ifndef __RTL_RTL_H__
#error "Should be only included by <rtl/rtl.h>"
#endif

/* RTL pseudo instructions */

static inline def_rtl(li, rtlreg_t* dest, const rtlreg_t imm) {
  rtl_addi(s, dest, rz, imm);
}

static inline def_rtl(mv, rtlreg_t* dest, const rtlreg_t *src1) {
  if (dest != src1) rtl_add(s, dest, src1, rz);
}

static inline def_rtl(not, rtlreg_t *dest, const rtlreg_t* src1) {
  // dest <- ~src1
  *dest = ~(*src1);
}

static inline def_rtl(neg, rtlreg_t *dest, const rtlreg_t* src1) {
  // dest <- -src1
  TODO();
}

static inline def_rtl(sext, rtlreg_t* dest, const rtlreg_t* src1, int width) {
  // dest <- signext(src1[(width * 8 - 1) .. 0])
  int sign = 0;
  if (width == 1) {
    sign = (*src1 & 0x80) >> 7;
  } else if (width == 2) {
    sign = (*src1 & 0x8000) >> 15;
  }
  rtlreg_t tmp = *src1;
#ifdef LOG
  Log("sign = %d, *src = %X", sign, *src1);
#endif
#ifdef ISA64
  int len = 64;
#else
  int len = 32;
#endif
  if (sign) {
    int i;
    for (i = width * 8; i < len; i++) {
#ifdef LOG
	  Log("sign << %d = %X", i, (sign << i));
#endif
      if (sign) {
	    tmp |= (sign << i);
	  }
    }
  } else {
    if (width == 1) {
	  tmp &= 0xFF;
	} else if (width == 2) {
	  tmp &= 0xFFFF;
	}
  }
#ifdef LOG
  Log("tmp = %X", tmp);
#endif
  *dest = tmp;
}

static inline def_rtl(zext, rtlreg_t* dest, const rtlreg_t* src1, int width) {
  // dest <- zeroext(src1[(width * 8 - 1) .. 0])
  TODO();
}

static inline def_rtl(msb, rtlreg_t* dest, const rtlreg_t* src1, int width) {
  // dest <- src1[width * 8 - 1]
  TODO();
}

#endif
