#include <klibtest.h>

void test_memmove() {
  char *str0 = "hello,world!";
  char *str1[20];
  memmove(str1, str0, 7);
  assert(memcmp(str1, str0, 7) == 0);
}
