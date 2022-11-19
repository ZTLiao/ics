#include <klibtest.h>

void test_strcmp() {
    char *str0 = "hello,world!";
	char *str1 = "hello,world!";
	char *str2 = "1";
	char *str3 = "2";
	assert(strcmp(str0, str1) == 0);
    assert(strcmp(str2, str3) < 0);
}
