#include <klibtest.h>

char *s[] = {
    "aaaaaaaaaaaaaaaaaaaaaaaa",
	"aaaaaaaaaaaaaaaaaaaaaaab",
	"aaaaaaaaaaaaaaaaaaaaaaaa",
	", World!\n",
	"Hello, World!\n",
	"#####"
};

char str1[] = "Hello";
char str[20];

void test_memcmp() {
  assert(memcmp(s[0], s[2], strlen(s[0])) == 0);
  assert(memcmp(s[0], s[1], strlen(s[1])) != 0);
  assert(memcmp("NES", "NES", 3) == 0);
}
