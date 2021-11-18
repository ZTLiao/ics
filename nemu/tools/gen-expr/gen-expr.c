#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <assert.h>
#include <string.h>

// this should be enough
static char buf[65536] = {};
static char code_buf[65536 + 128] = {}; // a little larger than `buf`
static char *code_format =
"#include <stdio.h>\n"
"int main() { "
"  unsigned result = %s; "
"  printf(\"%%u\", result); "
"  return 0; "
"}";

static int n_buf = 0;

static int is_parentheses = 0;

static int is_div = 0;

static int is_gen_num = 0;

static inline int choose(int n);

static inline void gen_num();

static inline void gen(char c);

static inline void gen_rand_op();

static inline void gen_rand_expr() {
	int c = choose(3);
	//printf("c = %d\n", c);
	switch(c) {
	  case 0: gen_num(); break;
	  case 1: gen('('); gen_rand_expr(); gen(')'); break;
	  default: gen_rand_expr(); gen_rand_op(); gen_rand_expr(); break;
	}
	buf[n_buf] = '\0';
	//printf("buf=%s\n", buf);
}

static inline int choose(int n) {
	int c = rand() % n;
	while (is_parentheses && c == 1) {
		c = rand() % n;
	}
	return c;
}

static inline void gen_num() {
	is_gen_num = 1;
	int n = rand() % 10;
	while (is_div && n == 0) {
		n = rand() % 10;
	}
	//printf("n = %d\n", n);
	sprintf(&buf[n_buf], "%d", n);
	//printf(" gen_num = %s\n", buf);
	//printf(" n_buf = %d\n", n_buf);
	n_buf++;
}

static inline void gen(char c) {
	if (strcmp(&c, "(") == 0 || strcmp(&c, ")") == 0) {
		is_parentheses = !is_parentheses;
	}
	buf[n_buf] = c;
	n_buf++;
}

static inline void gen_rand_op() {
	is_gen_num = 0;
	int i = rand() % 10;
	char c;
	switch (i) {
		case 0: c = '+'; is_div = 0; break;
		case 1: c = '-'; is_div = 0; break;
		case 2: c = '*'; is_div = 0; break;
		case 3: c = '/'; is_div = 1; break;
		default: c = '+'; is_div = 0; break;
	}
	buf[n_buf] = c;
	n_buf++;
}

int main(int argc, char *argv[]) {
  int seed = time(0);
  srand(seed);
  int loop = 1;
  if (argc > 1) {
    sscanf(argv[1], "%d", &loop);
  }
  int i;
  for (i = 0; i < loop; i ++) {
    gen_rand_expr();
	n_buf = 0;

    sprintf(code_buf, code_format, buf);

    FILE *fp = fopen("/tmp/.code.c", "w");
    assert(fp != NULL);
    fputs(code_buf, fp);
    fclose(fp);

    int ret = system("gcc /tmp/.code.c -o /tmp/.expr");
    if (ret != 0) continue;

    fp = popen("/tmp/.expr", "r");
    assert(fp != NULL);

    int result;
    fscanf(fp, "%d", &result);
    pclose(fp);

    printf("%u %s\n", result, buf);
  }
  return 0;
}
