8d 4c 24 04
lea M,Gv
MODR/M: 01 001 100
SIB: 00 100 100
disp8: 04
nemu/src/isa/x86/exec/exec.c
IDEX(0x8d, lea_M2G, lea)

lea_M2G:
nemu/src/isa/x86/local-include/decode.h
static inline def_DHelper(lea_M2G) {
	operand_rm(s, id_src1, false, id_dest, false);
}

operand_rm:
nemu/src/isa/x86/local-include/decode.h
static inline void operand_rm(DecodeExecState *s, Operand *rm, bool bool_rm_val, Operand *reg, bool load_reg_val) {
	read_ModR_M(s, rm, load_rm_val, reg, load_reg_val);
}

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

static inline void operand_reg(DecodeExecState *s, Operand *op, bool load_val, int r, int width) {
  op->type = OP_TYPE_REG;
  op->reg = r;

  if (width == 4) {
    op->preg = &reg_l(r);
  } else {
    assert(width == 1 || width == 2);
    op->preg = &op->val;
    if (load_val) rtl_lr(s, &op->val, r, width);
  }
  print_Dop(op->str, OP_STR_SIZE, "%%%s", reg_name(r, width));
}

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

exec_lea:
nemu/src/isa/x86/exec/data-mov.h
static inline def_EHelper(lea) {
  rtl_addi(s, ddest, s->isa.mbase, s->isa.moff);
  operand_write(s, id_dest, ddest);
  print_asm_template2(lea);
} 
