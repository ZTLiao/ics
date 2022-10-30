100016:   80 7d 08 00             cmpb   $0x0,0x8(%ebp)
01 111 101
IDEXW(0x80, I2E, gp1, 1)
static inline def_EHelper(gp1) {
  switch (s->isa.ext_opcode) {
    //EX(0, add)
    //EX(1, or)
    //EX(2, adc)
    //EX(3, sbb)
    //EX(4, and)
    //EX(5, sub)
    //EX(6, xor)
    //EX(7, cmp)
    EX (0, add)
    EMPTY(1)
    EMPTY(2)
    EMPTY(3)
    EX (4, and)
    EX (5, sub)
    EMPTY(6)
    EX (7, cmp)
  }
}

decode_I2E:
nemu/src/isa/x86/local-include/decode.h
/* Eb <- Ib
 * Ev <- Iv
 */
static inline def_DHelper(I2E) {
  operand_rm(s, id_dest, true, NULL, false);
  decode_op_I(s, id_src1, true);
}

operand_rm:
nemu/src/isa/x86/local-include/decode.h
static inline void operand_rm(DecodeExecState *s, Operand *rm, bool load_rm_val, Operand *reg, bool load_reg_val) {
  read_ModR_M(s, rm, load_rm_val, reg, load_reg_val);
}

read_ModR_M:
nemu/src/isa/x86/decode.c
typedef union {
  struct {
    uint8_t R_M		:3;
    uint8_t reg		:3;
    uint8_t mod		:2;
  };
  struct {
    uint8_t dont_care	:3;
    uint8_t opcode		:3;
  };
  uint8_t val;
} ModR_M;

typedef union {
  struct {
    uint8_t base	:3;
    uint8_t index	:3;
    uint8_t ss		:2;
  };
  uint8_t val;
} SIB;

static inline void load_addr(DecodeExecState *s, ModR_M *m, Operand *rm) {
  assert(m->mod != 3);

  sword_t disp = 0;
  int disp_size = 4;
  int base_reg = -1, index_reg = -1, scale = 0;

  if (m->R_M == R_ESP) {
    SIB sib;
    sib.val = instr_fetch(&s->seq_pc, 1);
    base_reg = sib.base;
    scale = sib.ss;

    if (sib.index != R_ESP) { index_reg = sib.index; }
  }
  else {
    /* no SIB */
    base_reg = m->R_M;
  }

  if (m->mod == 0) {
    if (base_reg == R_EBP) { base_reg = -1; }
    else { disp_size = 0; }
  }
  else if (m->mod == 1) { disp_size = 1; }

  if (disp_size != 0) {
    /* has disp */
    disp = instr_fetch(&s->seq_pc, disp_size);
    if (disp_size == 1) { disp = (int8_t)disp; }
  }

  s->isa.mbase = (base_reg != -1 ? &reg_l(base_reg) : rz);
  if (index_reg != -1) {
    rtl_shli(s, s1, &reg_l(index_reg), scale);
    rtl_add(s, &s->isa.mbr, s->isa.mbase, s1);
    s->isa.mbase = &s->isa.mbr;
  }
  s->isa.moff = disp;

#ifdef DEBUG
  char disp_buf[16];
  char base_buf[8];
  char index_buf[8];

  if (disp_size != 0) {
    /* has disp */
    sprintf(disp_buf, "%s%#x", (disp < 0 ? "-" : ""), (disp < 0 ? -disp : disp));
  }
  else { disp_buf[0] = '\0'; }

  if (base_reg == -1) { base_buf[0] = '\0'; }
  else {
    sprintf(base_buf, "%%%s", reg_name(base_reg, 4));
  }

  if (index_reg == -1) { index_buf[0] = '\0'; }
  else {
    sprintf(index_buf, ",%%%s,%d", reg_name(index_reg, 4), 1 << scale);
  }

  if (base_reg == -1 && index_reg == -1) {
    sprintf(rm->str, "%s", disp_buf);
  }
  else {
    sprintf(rm->str, "%s(%s%s)", disp_buf, base_buf, index_buf);
  }
#endif

  rm->type = OP_TYPE_MEM;
}

void read_ModR_M(DecodeExecState *s, Operand *rm, bool load_rm_val, Operand *reg, bool load_reg_val) {
  ModR_M m;
  m.val = instr_fetch(&s->seq_pc, 1);
  s->isa.ext_opcode = m.opcode;
  if (reg != NULL) operand_reg(s, reg, load_reg_val, m.reg, reg->width);
  if (m.mod == 3) operand_reg(s, rm, load_rm_val, m.R_M, rm->width);
  else {
    load_addr(s, &m, rm);
    if (load_rm_val) rtl_lm(s, &rm->val, s->isa.mbase, s->isa.moff, rm->width);
    rm->preg = &rm->val;
  }
}


decode_op_I:
nemu/src/isa/x86/local-include/decode.h
static inline def_DopHelper(I) {
  /* pc here is pointing to the immediate */
  word_t imm = instr_fetch(&s->seq_pc, op->width);
  operand_imm(s, op, load_val, imm, op->width);
}
