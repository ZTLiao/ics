#include <isa.h>
#include <monitor/difftest.h>
#include "../local-include/reg.h"
#include "difftest.h"

void printEflags(CPU_state *ref_r) {
  Log("ref_r->eflags.CF = %d, cpu.eflags.CF = %d", ref_r->eflags.CF, cpu.eflags.CF);
  Log("ref_r->eflags.PF = %d, cpu.eflags.PF = %d", ref_r->eflags.PF, cpu.eflags.PF);
  Log("ref_r->eflags.AF = %d, cpu.eflags.AF = %d", ref_r->eflags.AF, cpu.eflags.AF);
  Log("ref_r->eflags.ZF = %d, cpu.eflags.ZF = %d", ref_r->eflags.ZF, cpu.eflags.ZF);
  Log("ref_r->eflags.SF = %d, cpu.eflags.SF = %d", ref_r->eflags.SF, cpu.eflags.SF);
  Log("ref_r->eflags.TF = %d, cpu.eflags.TF = %d", ref_r->eflags.TF, cpu.eflags.TF);
  Log("ref_r->eflags.IF = %d, cpu.eflags.IF = %d", ref_r->eflags.IF, cpu.eflags.IF);
  Log("ref_r->eflags.DF = %d, cpu.eflags.DF = %d", ref_r->eflags.DF, cpu.eflags.DF);
  Log("ref_r->eflags.OF = %d, cpu.eflags.OF = %d", ref_r->eflags.OF, cpu.eflags.OF);
}

bool isa_difftest_checkregs(CPU_state *ref_r, vaddr_t pc) {
  Log("isa_difftest_checkregs...");
  Log("ref_r->pc = %X, cpu.pc = %X", ref_r->pc, cpu.pc);
  printEflags(ref_r);
  if (ref_r->eax != cpu.eax) {
    Log("ref_r->eax = %X, cpu.eax = %X", ref_r->eax, cpu.eax);
	return false;
  }
  if (ref_r->ebx != cpu.ebx) {
    Log("ref_r->ebx = %X, cpu.ebx = %X", ref_r->ebx, cpu.ebx);
	return false;
  }
  if (ref_r->ecx != cpu.ecx) {
    Log("ref_r->ecx = %X, cpu.ecx = %X", ref_r->ecx, cpu.ecx);
	return false;
  }
  if (ref_r->edx != cpu.edx) {
    Log("ref_r->edx = %X, cpu.edx = %X", ref_r->edx, cpu.edx);
	return false;
  }
  if (ref_r->esi != cpu.esi) {
    Log("ref_r->esi = %X, cpu.esi = %X", ref_r->esi, cpu.esi);
	return false;
  }
  if (ref_r->edi != cpu.edi) {
    Log("ref_r->edi = %X, cpu.edi = %X", ref_r->edi, cpu.edi);
	return false;
  }
  if (ref_r->esp != cpu.esp) {
    Log("ref_r->esp = %X, cpu.esp = %X", ref_r->esp, cpu.esp);
	return false;
  }
  if (ref_r->ebp != cpu.ebp) {
    Log("ref_r->ebp = %X, cpu.ebp = %X", ref_r->ebp, cpu.ebp);
	return false;
  }
  return true;
}

void isa_difftest_attach() {
  Log("isa_difftest_attach...");
}
