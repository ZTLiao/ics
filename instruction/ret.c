nemu/src/isa/x86/exec/exec.c
fetch_decode_exec:
EX(0xc3, ret)
case 0xc3:
	set_width(s, 0);
	decode_empty(s);
	exec_ret(s);
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

nemu/src/isa/x86/exec/control.h
exec_ret:
exec_ret(s);

nemu/src/isa/x86/local-include/rtl.h
rtl_pop:
rtl_pop(s, ddest);

nemu/src/engine/interpreter/rtl-basic.h
static inline def_rtl(lm, rtlreg_t *dest, const rtlreg_t* addr, word_t offset, int len) {
  *dest = vaddr_read(*addr + offset, len);
}
