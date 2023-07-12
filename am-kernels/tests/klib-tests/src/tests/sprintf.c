#include <klibtest.h>

void test_sprintf() {
  char buf[128];
  sprintf(buf, "%s", "Hello world!\n");
  assert(strcmp(buf, "Hello world!\n") == 0);
  printf("%s", buf);
}
