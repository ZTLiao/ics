#ifndef __X86_RTL_H__
#define __X86_RTL_H__

#include <rtl/rtl.h>
#include "reg.h"

/* RTL pseudo instructions */

#define def_rtl_setget_eflags(f) \
  static inline def_rtl(concat(set_, f), const rtlreg_t* src) { \
    cpu.eflags.f = *src; \
  } \
  static inline def_rtl(concat(get_, f), rtlreg_t* dest) { \
    *dest = cpu.eflags.f; \
  }

def_rtl_setget_eflags(CF)
def_rtl_setget_eflags(OF)
def_rtl_setget_eflags(ZF)
def_rtl_setget_eflags(SF)
def_rtl_setget_eflags(PF)

static inline def_rtl(lr, rtlreg_t* dest, int r, int width) {
  switch (width) {
    case 4: rtl_mv(s, dest, &reg_l(r)); return;
    case 1: rtl_host_lm(s, dest, &reg_b(r), 1); return;
    case 2: rtl_host_lm(s, dest, &reg_w(r), 2); return;
    default: assert(0);
  }
}

static inline def_rtl(sr, int r, const rtlreg_t* src1, int width) {
  switch (width) {
    case 4: rtl_mv(s, &reg_l(r), src1); return;
    case 1: rtl_host_sm(s, &reg_b(r), src1, 1); return;
    case 2: rtl_host_sm(s, &reg_w(r), src1, 2); return;
    default: assert(0);
  }
}

static inline def_rtl(push, const rtlreg_t* src1) {
  // esp <- esp - 4
  // M[esp] <- src1
  int width = s->isa.is_operand_size_16 ? 2 : 4;
  Log("BEFORE R_ESP = %x", cpu.esp);
  cpu.esp = cpu.esp - width;
  rtl_sm(s, &cpu.esp, 0, src1, width);
  rtlreg_t tmp;
  rtl_lm(s, &tmp, &reg_l(R_ESP), 0, 4);
  Log("AFTER R_ESP = %x, src1 = %x, reg_l(%d) = %x, tmp = %x", cpu.esp, *src1, R_ESP, reg_l(R_ESP), tmp);
}

static inline def_rtl(pop, rtlreg_t* dest) {
  // dest <- M[esp]
  // esp <- esp + 4
  int width = s->isa.is_operand_size_16 ? 2 : 4;
  Log("rtl_pop...");
  Log("cpu.esp = %x, dest = %p", cpu.esp, dest);
  rtl_lm(s, dest, &cpu.esp, 0, width);
  Log("BEFORE R_ESP = %x, dest = %x", cpu.esp, *dest);
  cpu.esp = cpu.esp + width;
  Log("AFTER R_ESP = %x", cpu.esp);
}

static inline bool is_overflow(const rtlreg_t* res, const rtlreg_t* src1, const rtlreg_t* src2, int width) {
  uint8_t over1 = 0;
  uint8_t over2 = 0;
  if (width == 1) {
	over1 = ((*res ^ *src1 ^ *src2) & 0x80) >> 7;
    over2 = ((*res ^ *src1 ^ *src2) & 0x40) >> 6;
  } else if (width == 2) {
    over1 = ((*res ^ *src1 ^ *src2) & 0x8000) >> 15;
	over2 = ((*res ^ *src1 ^ *src2) & 0x4000) >> 14;
  } else if (width == 4) {
    over1 = ((*res ^ *src1 ^ *src2) & 0x80000000) >> 31;
	over2 = ((*res ^ *src1 ^ *src2) & 0x40000000) >> 30;
  }
  return over1 ^ over2;
}

static inline def_rtl(is_sub_overflow, rtlreg_t* dest,
    const rtlreg_t* res, const rtlreg_t* src1, const rtlreg_t* src2, int width) {
  // dest <- is_overflow(src1 - src2)
  Log("res = %X, src1 = %X, src2 = %X, width = %d", *res, *src1, *src2, width);
  uint8_t over  = 0;
  uint8_t over1 = 0;
  uint8_t over2 = 0;
  if (width == 1) {
	over  = (*res  & 0x80) >> 7;
	over1 = (*src1 & 0x80) >> 7;
	over2 = (*src2 & 0x80) >> 7;
  } else if (width == 2) {
    over  = (*res  & 0x8000) >> 15;
    over1 = (*src1 & 0x8000) >> 15;
    over2 = (*src2 & 0x8000) >> 15;
  } else if (width == 4) {
    over  = (*res  & 0x80000000) >> 31;
    over1 = (*src1 & 0x80000000) >> 31;
    over2 = (*src2 & 0x80000000) >> 31;
  }
  bool flag = 0;
  if (over1 != over2 && over != over1) {
    flag = 1;
  }
  Log("is_sub_overflow eflags.OF = %d", flag);
  *dest = flag;
}

static inline bool is_carry(const rtlreg_t* src1, const rtlreg_t* src2) {
  if (*src1 == 0 || *src2 == 0) {
    return 0;
  }
  uint8_t cin = 0;
  int i;
  for (i = 0; i < 32; i++) {
	uint8_t tmp0 = *src1 >> i & 1;
	uint8_t tmp1 = *src2 >> i & 1;
    cin = (tmp0 & tmp1) | ((tmp0 ^ tmp1) & cin);
  }
  return cin;
}

static inline def_rtl(is_sub_carry, rtlreg_t* dest,
    const rtlreg_t* src1, const rtlreg_t* src2) {
  // dest <- is_carry(src1 - src2)
  bool flag = 0;
  uint8_t cin = 0;
  int i;
  for (i = 0; i < 32; i++) {
    uint8_t tmp0 = *src1 >> i & 1;
    uint8_t tmp1 = *src2 >> i & 1;
    cin = ((~tmp0) & (~tmp1) & cin) | ((~tmp0) & tmp1 & (~cin)) | ((~tmp0) & tmp1 & cin) | (tmp0 & tmp1 & cin);
  }
  flag = cin;
  Log("is_sub_carry = %d", flag);
  Log("eflags.CF = %d", flag);
  *dest = flag;
}

static inline def_rtl(is_add_overflow, rtlreg_t* dest,
    const rtlreg_t* res, const rtlreg_t* src1, const rtlreg_t* src2, int width) {
  // dest <- is_overflow(src1 + src2)
  bool flag = is_overflow(res, src1, src2, width);
  Log("is_add_overflow = %d", flag);
  Log("eflags.OF = %d", flag);
  *dest = flag;
}

static inline def_rtl(is_add_carry, rtlreg_t* dest,
    const rtlreg_t* res, const rtlreg_t* src1) {
  // dest <- is_carry(src1 + src2)
  bool flag = is_carry(res, src1);
  *dest = flag;
  Log("is_add_carry eflags.CF = %d", flag);
}



static inline bool is_zero(const rtlreg_t* result, int width) {
	uint8_t* pResult = (uint8_t*)result;
	int sum = 0;
	int i;
	for (i = 0; i < width; i++) {
		sum += *(pResult + i);
	} 
	return sum == 0;
}

static inline def_rtl(update_ZF, const rtlreg_t* result, int width) {
  // eflags.ZF <- is_zero(result[width * 8 - 1 .. 0])
  bool flag = is_zero(result, width);
  Log("result = %x, is_zero = %d", *result, flag);
  rtlreg_t src = flag;
  rtl_set_ZF(s, &src);
  Log("eflags.ZF = %d", cpu.eflags.ZF);
}

static inline bool is_sign(const rtlreg_t* result, int width) {
  return result[0] >> (width * 8 - 1) & 1;
}

static inline def_rtl(update_SF, const rtlreg_t* result, int width) {
  // eflags.SF <- is_sign(result[width * 8 - 1 .. 0])
  bool flag = is_sign(result, width);
  Log("result = %x, is_sign = %d", *result, flag);
  rtlreg_t src = flag;
  rtl_set_SF(s, &src);
  Log("eflags.SF = %d", cpu.eflags.SF);
}

static inline def_rtl(update_ZFSF, const rtlreg_t* result, int width) {
  rtl_update_ZF(s, result, width);
  rtl_update_SF(s, result, width);
}

static inline bool is_even(const rtlreg_t* result, int width) {
	int count = 0;
	int i;
	for (i = 0; i < width * 8; i++) {
		count += (result[0] >> i) & 1;
	}
	return count && (count % 2) == 0;
}

static inline def_rtl(update_PF, const rtlreg_t* result, int width) {
	bool flag = is_even(result, width);
	Log("result = %x, is_even = %d", *result, flag);
	rtlreg_t src = flag;
	rtl_set_PF(s, &src);
	Log("eflag.PF = %d", cpu.eflags.PF);
}

#endif
