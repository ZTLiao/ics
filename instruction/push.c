nemu/src/isa/x86/exec/exec.c
fetch_decode_exec:
IDEX(0x55, r, push)

case 0x55:
	set_width(s, 0);
	decode_r(s);
	exec_push(s);
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
decode_r:
def_Dhelper(r)
decode_op_r(s, id_dest, true);

nemu/src/isa/x86/local-include/decode.h
decode_op_r:
def_DopHelper(r)
operand_reg(s, op, load_val, s->opcode & 0x7, op->width);

nemu/src/isa/x86/local-include/decode.h
operand_reg:
op->type = OP_TYPE_REG;
op->reg = r;
if (width == 4) {
	op->preg = &reg_l(r);
} else {
	assert(width == 1 || width == 2);
	op->preg = &op->val;
	if (load_val) {
		rtl_lr(s, &op->val, r, width);
	}
}
print_Dop(op->str, OP_STR_SIZE, "%%%s", reg_name(r, width));

nemu/src/isa/x86/exec/data-mov.h
exec_push:
def_EHelper(push)
rtl_push(s, ddest);
print_asm_template1(push);

nemu/include/rtl/rtl.h
#define id_dest (&s->dest)

#define ddest (id_dest->preg)

nemu/src/isa/x86/local-include/rtl.h
static inline def_rtl(push, const rtlreg_t* src1) {
	int width = s->isa.is_operand_size_16 ? 2 : 4;
	reg_l(R_ESP) = reg_l(R_ESP) - width;
	rtl_sm(s, &reg_l(R_ESP), 0, src1, width);
}
