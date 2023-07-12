#include <isa.h>
#include <memory/vaddr.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "local-include/reg.h"

const char *regsl[] = {"eax", "ecx", "edx", "ebx", "esp", "ebp", "esi", "edi"};
const char *regsw[] = {"ax", "cx", "dx", "bx", "sp", "bp", "si", "di"};
const char *regsb[] = {"al", "cl", "dl", "bl", "ah", "ch", "dh", "bh"};

void reg_test() {
  srand(time(0));
  word_t sample[8];
  word_t pc_sample = rand();
  cpu.pc = pc_sample;

  int i;
  for (i = R_EAX; i <= R_EDI; i ++) {
    sample[i] = rand();
#ifdef LOG
    printf("sample[%d] = %x\n", i, sample[i]);
#endif
    reg_l(i) = sample[i];
#ifdef LOG
    printf("cpu.%s = %x, reg_l(%d) = %x, reg_w(%d) = %x, reg_b(%d) = %x\n", reg_name(i, 4), *((&cpu.eax) + i), i, reg_l(i), i, reg_w(i), i, reg_b(i));
#endif
    assert(reg_w(i) == (sample[i] & 0xffff));
  }

  assert(reg_b(R_AL) == (sample[R_EAX] & 0xff));
  assert(reg_b(R_AH) == ((sample[R_EAX] >> 8) & 0xff));
  assert(reg_b(R_BL) == (sample[R_EBX] & 0xff));
  assert(reg_b(R_BH) == ((sample[R_EBX] >> 8) & 0xff));
  assert(reg_b(R_CL) == (sample[R_ECX] & 0xff));
  assert(reg_b(R_CH) == ((sample[R_ECX] >> 8) & 0xff));
  assert(reg_b(R_DL) == (sample[R_EDX] & 0xff));
  assert(reg_b(R_DH) == ((sample[R_EDX] >> 8) & 0xff));

#ifdef LOG
  printf("sample[%d] = %x, cpu.ecx = %x\n", R_ECX, sample[R_ECX], cpu.ecx);
#endif
  assert(sample[R_EAX] == cpu.eax);
  assert(sample[R_ECX] == cpu.ecx);
  assert(sample[R_EDX] == cpu.edx);
  assert(sample[R_EBX] == cpu.ebx);
  assert(sample[R_ESP] == cpu.esp);
  assert(sample[R_EBP] == cpu.ebp);
  assert(sample[R_ESI] == cpu.esi);
  assert(sample[R_EDI] == cpu.edi);

  assert(pc_sample == cpu.pc);
  cpu.pc = 0;
  cpu.eax = 0;
  cpu.ebx = 0;
  cpu.ecx = 0;
  cpu.edx = 0;
  cpu.esi = 0;
  cpu.edi = 0;
  cpu.ebp = 0;
  cpu.esp = 0;
}

void isa_reg_display() {
	Log("isa_reg_display!");
#ifdef __ISA_x86__
	printf("ISA\t\tx86\n");
	printf("PC\t\t0x%08X\n", cpu.pc);
	printf("EAX\t\t0x%08X\n", cpu.eax);
	printf("EBX\t\t0x%08X\n", cpu.ebx);
	printf("ECX\t\t0x%08X\n", cpu.ecx);
	printf("EDX\t\t0x%08X\n", cpu.edx);
	printf("ESI\t\t0x%08X\n", cpu.esi);
	printf("EDI\t\t0x%08X\n", cpu.edi);
	printf("EBP\t\t0x%08X\n", cpu.ebp);
	printf("ESP\t\t0x%08X\n", cpu.esp);
#endif

}

word_t isa_reg_str2val(const char *s, bool *success) {
	Log("%s", s);
	*success = false;
	if (strcmp((s + 1), "pc") == 0) {
		*success = true;
		return cpu.pc;
	}
	int i;
	for (i = 0; i < (sizeof(regsl) / sizeof(regsl[0])); i++) {
		if (strcmp((s + 1), regsl[i]) == 0) {
			Log("reg_l(%d) = %x", i, reg_l(i));
			*success = true;
			return reg_l(i);
		}
	}
	for (i = 0; i < (sizeof(regsw) / sizeof(regsw[0])); i++) {
		if (strcmp((s + 1), regsw[i]) == 0) {
			Log("reg_w(%d) = %x", i, reg_w(i));
			*success = true;
			return reg_w(i);
		}
	}
	for (i = 0; i < (sizeof(regsb) / sizeof(regsb[0])); i++) {
		if (strcmp((s + 1), regsb[i]) == 0) {
			Log("reg_b(%d) = %x", i, reg_b(i));
			*success = true;
			return reg_b(i);
		}
	}
	return 0;
}
