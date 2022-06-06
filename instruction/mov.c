nemu/src/isa/x86/exec/exec.c
fetch_decode_exec:
IDEX(0xbd, mov_I2r, mov);

case 0xbd:
	set_width(s, 0);
	decode_mov_I2r(s);
	exec_mov(s);
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
decode_mov_I2r:
def_DHelper(I2r)
decode_op_r(s, id_dest, false);
decode_op_I(s, id_src1, true);

nemu/src/isa/x86/local-include/decode.h
decode_op_r:
def_DopHelper(r)
operand_reg(s, op, load_val, s->opcode & 0x7, op->width);
OP_TYPE_REG = 0

nemu/src/isa/x86/local-include/reg.h
reg_l:
static inline int check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
}
#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)

nemu/src/isa/x86/local-include/decode.h
print_Dop(op->str, OP_STR_SIZE, "%%%s", reg_name(r, width));
OP_STR_SIZE = 40

nemu/src/isa/x86/reg.c
reg_name(r, width);
const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};

nemu/include/cpu/decode.h
#define print_Dop(...) snprintf(__VA_ARGS__)
//给s->dest->str赋值寄存器名称

nemu/src/isa/x86/local-include/decode.h
decode_op_I:
def_DopHelper(I)
word_t imm = instr_fetch(&s->seq_pc, op->width);
operand_imm(s, op, load_val, imm, op->width);
OP_TYPE_IMM = 2

operand_imm:
rtl_li(s, &op->val, imm);

nemu/include/rtl/rtl.h
#define def_rtl(name, ...) void concat(rtl_, name)(DecodeExecState *s, __VA_ARGS__)

nemu/include/rtl/pseudo.h
rtl_li:
static inline def_rtl(li, rtlreg_t* dest, const rtlreg_t imm) {
	rtl_addi(s, dest, rz, imm);
}

nemu/include/rtl/rtl.h
extern const rtlreg_t rzero;
#define rz (&rzero)

nemu/src/engine/interpreter/rtl-basic.h
rtl_addi:
def_rtl_compute_reg_imm(add)
#define def_rtl_compute_imm(name) \
	static inline def_rtl(name ## i, rtlreg_t* dest, const rtlreg_t* src1, const sword imm) { \
		*dest = concat(c_, name) (*src1, imm); \
	}

nemu/src/engine/interpreter/c_op.h
c_add:
#define c_add(a, b) ((a) + (b))


nemu/src/isa/x86/exec/data-mov.h
exec_mov(s) => def_EHelper(mov)
operand_write(s, id_dest, dsrc1);
print_asm_template2(mov);

nemu/src/isa/x86/local-include/decode.h
operand_write:
OP_TYPE_REG = 0
static inline void operand_write(DecodeExecState *s, Operand *op, rtlreg_t *src) {
	if (op->type == OP_TYPE_REG) {
		rtl_sr(s, op->reg, src, op->width);
	} else if (op->type == OP_TYPE_MEM) {
		rtl_sm(s, s->isa.mbase, s->isa.moff, src, op->width);
	} else {
		assert(0);
	}
}

nemu/src/engine/interpreter/rtl-basic.h
rtl_sm:
static inline def_rtl(sm, const rtlreg_t* addr, word_t offset, const rtlreg_t* src1, int len) {
	vaddr_write(*addr + offset, *src1, len);
}

nemu/src/isa/x86/local-inlcude/rtl.h
rtl_sr:
static inline def_rtl(sr, int r, const rtlreg_t* src1, int width) {
	switch (width) {
		case 4:
			rtl_mv(s, &reg_l(r), src1);
			return;
		case 1:
			rtl_host_sm(s, &reg_b(r), src1, 1);
			return;
		case 2:
			rtl_host_sm(s, &reg_w(r), src1, 2);
			return;
		default:
			assert(0);	
	}
}


nemu/include/rtl/pseudo.h
rtl_mv:
static inline def_rtl(mv, rtlreg_t* dest, const rtlreg_t* src1) {
	if (dest != src1) {
		rtl_add(s, dest, src1, rz);
	}
}

nemu/src/engine/interpreter/rtl-basic.h
rtl_add:
def_rtl_compute_reg_imm(add)

#define def_rtl_compute_reg_imm(name) \
	def_rtl_compute_reg(name)	\
	def_rtl_compute_imm(name)	\

#define def_rtl_compute_reg(name) \
	static inline def_rtl(name, rtlreg_t* dest, const rtlreg_t* src1, const rtlreg_t* src2) { \
		*dest = concat(c_, name) (*src1, *src2); \
	}

#define def_rtl_compute_imm(name) \
	static inline def_rtl(name ## i, rtlreg_t* dest, const rtlreg_t* src1, const sword_t imm) { \
		*dest = concat(c_, name) (*src1, imm); \
	}

nemu/src/engine/interpreter/c_op.h
c_add:
#define c_add(a, b) ((a) + (b))
