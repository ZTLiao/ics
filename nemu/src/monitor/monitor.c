#include <isa.h>
#include <memory/paddr.h>
#include <monitor/monitor.h>
#include <getopt.h>
#include <stdlib.h>

void init_log(const char *log_file);
void init_mem();
void init_regex();
void init_wp_pool();
void init_difftest(char *ref_so_file, long img_size, int port);
word_t expr(char *e, bool *success);
void expr_test();

static char *log_file = NULL;
static char *diff_so_file = NULL;
static char *img_file = NULL;
static int batch_mode = false;
static int difftest_port = 1234;
static char *expr_file = NULL;

int is_batch_mode() { return batch_mode; }

static inline void welcome() {
#ifdef DEBUG
  Log("Debug: \33[1;32m%s\33[0m", "ON");
  Log("If debug mode is on, A log file will be generated to record every instruction NEMU executes. "
      "This may lead to a large log file. "
      "If it is not necessary, you can turn it off in include/common.h.");
#else
  Log("Debug: \33[1;32m%s\33[0m", "OFF");
#endif

  Log("Build time: %s, %s", __TIME__, __DATE__);
  printf("Welcome to \33[1;41m\33[1;33m%s\33[0m-NEMU!\n", str(__ISA__));
  printf("For help, type \"help\"\n");
}

static inline long load_img() {
  if (img_file == NULL) {
    Log("No image is given. Use the default build-in image.");
    return 4096; // built-in image size
  }

  FILE *fp = fopen(img_file, "rb");
  Assert(fp, "Can not open '%s'", img_file);

  Log("The image is %s", img_file);

  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);

  fseek(fp, 0, SEEK_SET);
  int ret = fread(guest_to_host(IMAGE_START), size, 1, fp);
  assert(ret == 1);

  fclose(fp);
  return size;
}

void expr_test() {
	if (expr_file == NULL) {
		Log("no expr file is execute. ");
		return;
	}
	Log("expr_file = %s", expr_file);
	FILE *fp = fopen(expr_file, "r");
	Assert(fp, "Can not open '%s'", expr_file);
	Log("The expr is %s", expr_file);
	char buf[65536] = {};
	bool b;
	while (fgets(buf, (sizeof(buf) / sizeof(buf[0])), fp) != NULL) {
		strtok(buf, " ");
		char* exprStr = strtok(NULL, " ");
		char* tempStr = exprStr;
		int index = 0;
		while (*tempStr != '\n') {
			tempStr++;
			index++;
		}
		exprStr[index] = '\0';
		word_t result = expr(exprStr, &b);
		printf("%u %s\n", result, exprStr);
	}
	fclose(fp);
}

static inline void parse_args(int argc, char *argv[]) {
  const struct option table[] = {
    {"batch"    , no_argument      , NULL, 'b'},
    {"log"      , required_argument, NULL, 'l'},
    {"diff"     , required_argument, NULL, 'd'},
    {"port"     , required_argument, NULL, 'p'},
    {"help"     , no_argument      , NULL, 'h'},
    {"expr"     , required_argument, NULL, 'e'},
    {0          , 0                , NULL,  0 },
  };
  int o;
  while ( (o = getopt_long(argc, argv, "-bhl:d:p:e:", table, NULL)) != -1) {
    switch (o) {
      case 'b': batch_mode = true; break;
      case 'p': sscanf(optarg, "%d", &difftest_port); break;
      case 'l': log_file = optarg; break;
      case 'd': diff_so_file = optarg; break;
      case 'e': expr_file = optarg; break;
      case 1:
        if (img_file != NULL) Log("too much argument '%s', ignored", optarg);
        else img_file = optarg;
        break;
      default:
        printf("Usage: %s [OPTION...] IMAGE\n\n", argv[0]);
        printf("\t-b,--batch              run with batch mode\n");
        printf("\t-l,--log=FILE           output log to FILE\n");
        printf("\t-d,--diff=REF_SO        run DiffTest with reference REF_SO\n");
        printf("\t-p,--port=PORT          run DiffTest with port PORT\n");
        printf("\n");
        exit(0);
    }
  }
}

void init_monitor(int argc, char *argv[]) {
  /* Perform some global initialization. */

  /* Parse arguments. */
  parse_args(argc, argv);

  /* Open the log file. */
  init_log(log_file);

  /* Fill the memory with garbage content. */
  init_mem();

  /* Perform ISA dependent initialization. */
  init_isa();

  /* Load the image to memory. This will overwrite the built-in image. */
  long img_size = load_img();

  /* Compile the regular expressions. */
  init_regex();

  /* Initialize the watchpoint pool. */
  init_wp_pool();

  /* Initialize differential testing. */
  init_difftest(diff_so_file, img_size, difftest_port);

  /* Display welcome message. */
  welcome();

  /* Test cul expression */
  expr_test();
}
