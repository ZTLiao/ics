#include <klibtest.h>
char *ss[] = {
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaab",
    "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa",
    ", World!\n",
    "Hello, World!\n",
    "#####"
};

char buf[128];

void test_strcmp() {
    char *str0 = "hello,world!";
	char *str1 = "hello,world!";
	char *str2 = "1";
	char *str3 = "2";
	sprintf(buf, "%s", "Hello world!\n");
    printf("%d\n", strcmp(buf, "Hello world!\n") == 0);
	assert(strcmp(ss[0], ss[1]) < 0);
	assert(strcmp(str0, str1) == 0);
    assert(strcmp(str2, str3) < 0);
}
