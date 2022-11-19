#include <klibtest.h>

void test_sprintf() {
  const char *str0 = "hello,world!";
  char str1[64];
  sprintf(str1, "%s", str0);
  assert(strcmp(str1, str0) == 0);
  sprintf(str1, "%d", 2022);
  assert(strcmp(str1, "2022") == 0);
}
