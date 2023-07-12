nemu/src/isa/x86/exec/exec.c
fetch_decode_exec:
IDEX(0xe8, J, call)

case 0xe8:
	set_width(s, 0);
	decode_J(s);
	exec_call(s);
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
decode_J:
def_DHelper(J)
decode_op_SI(s, id_dest, false);
s->jmp_pc = id_dest->simm + s->seq_pc;

nemu/src/isa/x86/local-include/decode.h
decode_op_SI:
def_DopHelper(SI)
word_t imm = instr_fetch(&s->seq_pc, op->width);
operand_imm(s, op, load_val, imm, op->width);

nemu/src/isa/x86/exec/control.h
exec_call:
def_EHelper(call)
rtl_j(s, s->jmp_pc);
rtl_push(s, &s->seq_pc);

nemu/src/engine/interpreter/rtl-basic.h
static inline def_rtl(j, vaddr_t target) {
	s->jmp_pc = target;
	s->is_jmp = true;
}

nemu/src/isa/x86/local-include/rtl.h
rtl_push:
def_rtl(push)
int width = s->isa.is_operand_size_16 ? 2 : 4;
reg_l(R_ESP) = reg_l(R_ESP) - width;
rtl_sm(s, &reg_l(R_ESP), 0, src1, width);
