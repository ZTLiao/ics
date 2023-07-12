#include <klibtest.h>

void (*entry)() = NULL;

static const char *tests[256] = {
	['a'] = "test memcpy",
	['b'] = "test memcmp",
	['c'] = "test memset",
	['d'] = "test strcat",
	['e'] = "test strcmp",
	['f'] = "test memmove",
	['g'] = "test sprintf",
	['h'] = "test printf",
	['i'] = "test shl",
};

int main(const char *args) {
	switch (args[0]) {
		CASE('a', test_memcpy);
		CASE('b', test_memcmp);
		CASE('c', test_memset);
		CASE('d', test_strcat);
		CASE('e', test_strcmp);
		CASE('f', test_memmove);
		CASE('g', test_sprintf);
		CASE('h', test_printf);
		CASE('i', test_shl);
		default:
			printf("Usage: make run mainargs=*\n");
			int ch;
			for (ch = 0; ch < 256; ch++) {
				if (tests[ch]) {
					printf("  %c: %s\n", ch, tests[ch]);
				}
			}
	}
	return 0;
}
