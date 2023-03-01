#include <monitor/difftest.h>

uint32_t pio_read_l(ioaddr_t);
uint32_t pio_read_w(ioaddr_t);
uint32_t pio_read_b(ioaddr_t);
void pio_write_l(ioaddr_t, uint32_t);
void pio_write_w(ioaddr_t, uint32_t);
void pio_write_b(ioaddr_t, uint32_t);

static inline def_EHelper(lidt) {
  TODO();
  print_asm_template1(lidt);
}

static inline def_EHelper(mov_r2cr) {
  TODO();
  print_asm("movl %%%s,%%cr%d", reg_name(id_src1->reg, 4), id_dest->reg);
}

static inline def_EHelper(mov_cr2r) {
  TODO();
  print_asm("movl %%cr%d,%%%s", id_src1->reg, reg_name(id_dest->reg, 4));

#ifndef __DIFF_REF_NEMU__
  difftest_skip_ref();
#endif
}

static inline def_EHelper(int) {
  TODO();
  print_asm("int %s", id_dest->str);

#ifndef __DIFF_REF_NEMU__
  difftest_skip_dut(1, 2);
#endif
}

static inline def_EHelper(iret) {
  TODO();
  print_asm("iret");

#ifndef __DIFF_REF_NEMU__
  difftest_skip_ref();
#endif
}

static inline def_EHelper(in) {
#ifdef LOG
  Log("in...");
#endif
  uint32_t temp;
  if (id_src1->width == 1) {
    temp = pio_read_b(id_src1->val);
  } else {
    temp = pio_read_w(id_src1->val);
  }
  if (id_dest->width == 1) {
    reg_b(id_dest->reg) = temp;
  } else if (id_dest->width == 4) {
    reg_l(id_dest->reg) = temp;
  }
  print_asm_template2(in);
}

static inline def_EHelper(out) {
#ifdef LOG
  Log("out...");
#endif
  if (id_dest->width == 1) {
    pio_write_b(reg_b(id_dest->reg), reg_b(id_src1->reg));
  } else {
    pio_write_b(reg_w(id_dest->reg), reg_l(id_src1->reg));
  }
  print_asm_template2(out);
}
