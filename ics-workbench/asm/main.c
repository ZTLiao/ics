#include "asm.h"
#include <assert.h>
#include <stdio.h>

int main() {
  asm_jmp_buf buf;
  int r = asm_setjmp(buf);
  if (r == 0) {
    assert(asm_add(1234, 5678) == 6912);
    assert(asm_popcnt(0x0123456789abcdefULL) == 32);
    // TODO: add more tests here.
	char src[20] = "hello,world!";
	char dest[20] = {0};
	void *ptr = asm_memcpy(dest, src, 20);
	printf("src = %s, dest = %s\n", src, dest);
	printf("ptr = %p, src = %p, dest = %p\n", ptr, src, dest);
    asm_longjmp(buf, 123);
  } else {
    assert(r == 123);
    printf("PASSED.\n");
  }
}
