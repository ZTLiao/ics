#include <klibtest.h>

void test_strcat() {
  char dst[N * 2];
  char *src = "hello,world!";
  char *s = strcat(dst, src);
  int i;
  for (i = 0; i < strlen(src); i++) {
    assert(s[i] == src[i]);
  }
}
