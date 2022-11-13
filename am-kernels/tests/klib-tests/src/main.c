#include <klibtest.h>

void (*entry)() = NULL;

static const char *tests[256] = {
	['m'] = "test memcpy",
};

int main(const char *args) {
	switch (args[0]) {
		CASE('m', test_memcpy);
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
