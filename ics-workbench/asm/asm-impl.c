#include "asm.h"
#include <string.h>

int64_t asm_add(int64_t a, int64_t b) {
  asm volatile(
  "movq %1,%0;"
  "addq %2,%0;"
  :"=a"(a)
  :"m"(a), "m"(b)
  );
  return a;
}

int asm_popcnt(uint64_t x) {
  int s = 0;
  asm volatile(
  "xorq %%rax,%%rax;"
  "movq %%rax,%%rbx;"
  ".range:"
  "cmpq $0x40,%%rbx;"
  "jge .ret;"
  "movq %%rbx,%%rcx;"
  "movq %1,-0x10(%%rsp);"
  "shrq %%rcx,-0x10(%%rsp);"
  "andb $1,-0x10(%%rsp);"
  "je .jmpinc;"
  "incq %%rax;"
  ".jmpinc:"
  "incq %%rbx;"
  "jmp .range;"
  ".ret:"
  "movl %%eax,%0;"
  :"=r" (s)
  :"D" (x)
  :"%rax", "%rbx", "%rcx", "cc" 
  );
  return s;
}

void *asm_memcpy(void *dest, const void *src, size_t n) {
  asm volatile(
  "cld;"
  "rep movsb;"
  :
  :"S"(src), "D"(dest), "c"(n)
  :"memory"
  );
  return dest;
}

int asm_setjmp(asm_jmp_buf env) {
   asm volatile(
   "movq %%rsp,0x48(%0);"
   "pushfq;"
   "movq %%rbx,0x10(%%rsp);"
   "movq (%%rsp),%%rbx;"
   "movq %%rbx,0x8(%0);"
   "movq 0x8(%%rsp),%%rbx;"
   "movq %%rbx,0x90(%0);"
   "movq 0x10(%%rsp),%%rbx;"
   "movl $0,(%0);"
   "movq %%rax,0x10(%0);"
   "movq %%rbx,0x18(%0);"
   "movq %%rcx,0x20(%0);"
   "movq %%rdx,0x28(%0);"
   "movq %%rsi,0x30(%0);"
   "movq %%rdi,0x38(%0);"
   "movq %%rbp,0x40(%0);"
   "movq %%r8,0x50(%0);"
   "movq %%r9,0x58(%0);"
   "movq %%r10,0x60(%0);"
   "movq %%r11,0x68(%0);"
   "movq %%r12,0x70(%0);"
   "movq %%r13,0x78(%0);"
   "movq %%r14,0x80(%0);"
   "movq %%r15,0x88(%0);"
   "popfq;"
   :"=D"(env)
   );
   return 0;
}

void asm_longjmp(asm_jmp_buf env, int val) {
   asm volatile(
   "movq %%rbx,0x8(%%rsp);"
   "movq 0x8(%0),%%rbx;"
   "movq %%rbx,(%%rsp);"
   "movq 0x8(%%rsp),%%rbx;"
   "popfq;"
   "movq 0x10(%0),%%rax;"
   "movq 0x18(%0),%%rbx;"
   "movq 0x20(%0),%%rcx;"
   "movq 0x28(%0),%%rdx;"
   "movq 0x38(%0),%%rdi;"
   "movq 0x40(%0),%%rbp;"
   "movq 0x48(%0),%%rsp;"
   "movq 0x50(%0),%%r8;"
   "movq 0x58(%0),%%r9;"
   "movq 0x60(%0),%%r10;"
   "movq 0x68(%0),%%r11;"
   "movq 0x70(%0),%%r12;"
   "movq 0x78(%0),%%r13;"
   "movq 0x80(%0),%%r14;"
   "movq 0x88(%0),%%r15;"
   "movq %%rbx,0x8(%%rsp);"
   "movq 0x90(%0),%%rbx;"
   "movq %%rbx,(%%rsp);"
   "movq 0x8(%%rsp),%%rbx;"
   "movl %1,%%eax;"
   "movq 0x30(%0),%%rsi;"
   :
   :"D"(env), "S"(val)
   );
}
