nemu/src/isa/x86/exec/exec.c
fetch_decode_exec:
IDEX(0x83, SI2E, gp1)

case 0x83:
	set_width(s, 0);
	decode_SI2E(s);
	exec_gp1(s);
	break;

nemu/include/cpu/exec.h
#define IDEXW(idx, id, ex, w) CASE_ENTRY(idx, concat(decode_, id), concat(exec_, ex), w)
#define IDEX(idx, id, ex) IDEXW(idx, id, ex, 0)
#define CASE_ENTRY(idx, id, ex, w) case idx: set_width(s, w); id(s); ex(s); break;

nemu/src/isa/x86/exec/exec.c
set_width:
if (width == 0) {
	width = s->isa.is_operand_size_16 ? 2 : 4;
}
s->src1.width = s->dest.width = s->src2.width = width;

nemu/include/cpu/decode.h
#define def_DHelper(name) void concat(decode_, name) (DecodeExecState *s)

nemu/src/isa/x86/local-include/decode.h
#define def_DopHelper(name) void concat(decode_op_, name) (DecodeExecState *s, Operand *op, bool load_val)

nemu/src/isa/x86/local-include/decode.h
decode_SI2E:
static inline def_DHelper(SI2E) {
	assert(id_dest->width == 2 || id_dest->width == 4);
	operand_rm(s, id_dest, true, NULL, false);
	id_src1->width = 1;
	decode_op_SI(s, id_src1, true);
	if (id_dest->width == 2) {
		*dsrc1 &= 0xffff;
	}
}

nemu/src/isa/x86/local-include/decode.h
operand_rm:
static inline void operand_rm(DecodeExecState *s, Operand *rm, bool load_rm_val, Operand *reg, bool load_reg_val) {
	read_ModR_M(s, rm, load_rm_val, reg, load_reg_val);
}

nemu/src/isa/x86/decode.c
typedef union {
	struct {
		uint8_t R_M		:3;
		uint8_t reg		:3;
		uint8_t mod		:2;
	};
	struct {
		uint8_t dont_care :3;
		uint8_t opcode	  :3;
	};
	uint8_t val;
} ModR_M;

void read_ModR_M(DecodeExecState *s, Operand *rm, bool load_rm_val, Operand *reg, bool load_reg_val) {
	ModR_M m;
	m.val = instr_fetch(&s->seq_pc, 1);
	s->isa.ext_opcode = m.opcode;
	if (reg != NULL) operand_reg(s, reg, load_reg_val, m.reg, reg->width);
	if (m.mod == 3) operand_reg(s, rm, load_rm_val, m.R_M, rm->width);
	else {
		load_addr(s, &m, rm);
		if (load_rm_val) rtl_lm(s, &rm->val, s->isa.mbase, s->isa.mbase, s->isa.moff, rm->width);
		rm->preg = &rm->val;
	}
}

nemu/include/cpu/exec.h
instr_fetch:
static inline uint32_t instr_fetch(vaddr_t *pc, int len) {
  uint32_t instr = vaddr_ifetch(*pc, len);
#ifdef DEBUG
  uint8_t *p_instr = (void *)&instr;
  int i;
  for (i = 0; i < len; i ++) {
    extern char log_bytebuf[];
    strcatf(log_bytebuf, "%02x ", p_instr[i]);
  }
#endif
  (*pc) += len;
  return instr;
}

nemu/include/memory/vaddr.h
vaddr_ifetch:

static inline word_t vaddr_ifetch(vaddr_t addr, int len) {
  word_t vaddr_ifetch1(vaddr_t addr);
  word_t vaddr_ifetch2(vaddr_t addr);
  word_t vaddr_ifetch4(vaddr_t addr);
#ifdef ISA64
  word_t vaddr_ifetch8(vaddr_t addr);
#endif
  switch (len) {
    case 1: return vaddr_ifetch1(addr);
    case 2: return vaddr_ifetch2(addr);
    case 4: return vaddr_ifetch4(addr);
#ifdef ISA64
    case 8: return vaddr_ifetch8(addr);
#endif
    default: assert(0);
  }
}

nemu/src/memory/paddr.c
vaddr_ifetch:
#define def_vaddr_template(bytes) \
word_t concat(vaddr_ifetch, bytes) (vaddr_t addr) { \
  int ret = isa_vaddr_check(addr, MEM_TYPE_IFETCH, bytes); \
  if (ret == MEM_RET_OK) return paddr_read(addr, bytes); \
  return 0; \
} \
def_vaddr_template(1)
def_vaddr_template(2)
def_vaddr_template(4)
#ifdef ISA64
def_vaddr_template(8)
#endif

nemu/src/isa/x86/local-include/decode.h
operand_reg:
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

nemu/src/isa/x86/local-include/reg.h
reg_l:
#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)

static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 32);
	return index;
}

nemu/src/isa/x86/decode.c
load_addr:
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

nemu/src/engine/interpreter/rtl-basic.h
rtl_shli:
def_rtl_compute_reg_imm(shl)

#define def_rtl_compute_reg(name) \
  static inline def_rtl(name, rtlreg_t* dest, const rtlreg_t* src1, const rtlreg_t* src2) { \
    *dest = concat(c_, name) (*src1, *src2); \
  }

#define def_rtl_compute_imm(name) \
  static inline def_rtl(name ## i, rtlreg_t* dest, const rtlreg_t* src1, const sword_t imm) { \
    *dest = concat(c_, name) (*src1, imm); \
  }

#define def_rtl_compute_reg_imm(name) \
  def_rtl_compute_reg(name) \
  def_rtl_compute_imm(name) \

nemu/src/engine/interpreter/c_op.h
c_shl:

#define c_shl(a, b) ((a) << ((b) & c_shift_mask))

nemu/src/isa/x86/exec/exec.c
static inline def_EHelper(gp1) {
	switch (s->isa.ext_opcode) {
		EMPTY(0)
		EMPTY(1)
		EMPTY(2)
		EMPTY(3)
		EMPTY(4)
		EMPTY(5)
		EMPTY(6)
		EMPTY(7)
	}
}

nemu/include/cpu/exec.h
#define IDEXW(idx, id, ex, w) CASE_ENTRY(idx, concat(decode_, id), concat(exec_, ex), w)
#define IDEX(idx, id, ex)     IDEXW(idx, id, ex, 0)
#define EXW(idx, ex, w)       IDEXW(idx, empty, ex, w)
#define EX(idx, ex)           EXW(idx, ex, 0)
#define EMPTY(idx)            EX(idx, inv)

// set_width() is defined in src/isa/$isa/exec/exec.c
#define CASE_ENTRY(idx, id, ex, w) case idx: set_width(s, w); id(s); ex(s); break;

EMPTY(5) -> EX(5, inv) -> EXW(5, inv, 0) -> IDEXW(5, empty, inv, 0) -> CASE_ENTRY(5, concat(decode_, empty), concat(exec_, inv), 0) -> case 5: set_width(s, 0); decode_empty(s); exec_inv(s); break;

nemu/include/cpu/exec.h
decode_empty:

#define decode_empty(s)

nemu/src/isa/x86/exec/special.c
exec_inv:
def_EHelper(inv) {
  /* invalid opcode */

  uint32_t temp[2];
  s->seq_pc = cpu.pc;
  temp[0] = instr_fetch(&s->seq_pc, 4);
  temp[1] = instr_fetch(&s->seq_pc, 4);

  uint8_t *p = (void *)temp;
  printf("invalid opcode(PC = 0x%08x): %02x %02x %02x %02x %02x %02x %02x %02x ...\n\n",
      cpu.pc, p[0], p[1], p[2], p[3], p[4], p[5], p[6], p[7]);

  display_inv_msg(cpu.pc);

  rtl_exit(NEMU_ABORT, cpu.pc, -1);

  print_asm("invalid opcode");
}

nemu/src/isa/x86/exec/arith.h
EX(5, sub) ->
case 5: set_width(s, 0); decode_empty(s); exec_sub(s); break;
