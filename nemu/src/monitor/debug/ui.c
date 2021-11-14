#include <isa.h>
#include <memory/paddr.h>
#include <memory/vaddr.h>
#include "expr.h"
#include "watchpoint.h"

#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

void cpu_exec(uint64_t);
int is_batch_mode();
void isa_reg_display();

/* We use the `readline' library to provide more flexibility to read from stdin. */
static char* rl_gets() {
  static char *line_read = NULL;

  if (line_read) {
    free(line_read);
    line_read = NULL;
  }

  line_read = readline("(nemu) ");

  if (line_read && *line_read) {
    add_history(line_read);
  }

  return line_read;
}

static int cmd_c(char *args) {
  cpu_exec(-1);
  return 0;
}

static int cmd_q(char *args) {
  return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args);

static int cmd_info(char *args);

static int cmd_r(char *args);

static int cmd_x(char *args);

static int cmd_p(char *args);

static int cmd_w(char *args);

static struct {
  char *name;
  char *description;
  int (*handler) (char *);
} cmd_table [] = {
  { "help", "Display informations about all supported commands", cmd_help },
  { "c", "Continue the execution of the program", cmd_c },
  { "q", "Exit NEMU", cmd_q },
  { "si", "Single step execution", cmd_si },
  { "info", "Print program status", cmd_info },
  { "r", "Print register information", cmd_r },
  { "x", "Scanning memory", cmd_x },
  { "p", "Print expression", cmd_p },
  { "w", "Watch point memory", cmd_w },
  /* TODO: Add more commands */

};

#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
  /* extract the first argument */
  char *arg = strtok(NULL, " ");
  int i;

  if (arg == NULL) {
    /* no argument given */
    for (i = 0; i < NR_CMD; i ++) {
      printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
    }
  }
  else {
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(arg, cmd_table[i].name) == 0) {
        printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
        return 0;
      }
    }
    printf("Unknown command '%s'\n", arg);
  }
  return 0;
}

static int cmd_si(char *args) {
	uint64_t n = (uint64_t) 1;
	if (args != NULL) {
		n = (uint64_t) atol(args);
	}
	Log("cmd_si: n = %ld", n);
	cpu_exec(n);
	return 0;
}

static int cmd_info(char *args) {
	if (args == NULL) {
		printf("no sub cmmand!\n");
		return 0;
	}
	int i;
	for (i = 0; i < NR_CMD; i++) {
		char* name = cmd_table[i].name;
		if (strcmp(args, name) == 0) {
			Log("name = %s", name);
			if (cmd_table[i].handler(NULL) < 0) {
				return 0;
			}
		}
	}
	return 0;
}

static int cmd_r(char* args) {
	Log("print register!");
	isa_reg_display();
	return 0;
}

static int cmd_x(char* args) {
	if (args == NULL) {
		return 0;
	}
	Log("scanning memory!");
	char* nStr = strtok(args, " ");
	char* exprStr = strtok(NULL, " ");
	Log("n = %s", nStr);
	int32_t n = (int32_t) atoi(nStr);
	uint64_t expr = (uint64_t) strtol(exprStr, NULL, 16);
	Log("expr = %lx", expr);
	int i;
	for (i = 0; i <= n; i++) {
		Log("i = %d", i);
#ifdef __ISA_x86__
		word_t vaddr = vaddr_read(expr + i, 4);
		printf("0x%lx\t\t0x%x\n", (expr + i), vaddr);
#endif
	}
	return 0;
}

static int cmd_p(char* args) {
	Log("print expression!");
	Log("args = '%s'", args);
	bool b = false;
	word_t n = expr(args, &b);
	Log("b = %d, n = %u", b, n);
#ifdef __ISA_x86__
	word_t vaddr = vaddr_read(n, 4);
	printf("vaddr = %x\n", vaddr);
#endif
	return 0;
}

static int cmd_w(char* args) {
	Log("watch point!");
	Log("args = '%s'", args);
	bool b = false;
	expr(args, &b);
	return 0;
}

void ui_mainloop() {
  if (is_batch_mode()) {
    cmd_c(NULL);
    return;
  }

  for (char *str; (str = rl_gets()) != NULL; ) {
    char *str_end = str + strlen(str);

    /* extract the first token as the command */
    char *cmd = strtok(str, " ");
    if (cmd == NULL) { continue; }

    /* treat the remaining string as the arguments,
     * which may need further parsing
     */
    char *args = cmd + strlen(cmd) + 1;
    if (args >= str_end) {
      args = NULL;
    }

#ifdef HAS_IOE
    extern void sdl_clear_event_queue();
    sdl_clear_event_queue();
#endif

    int i;
    for (i = 0; i < NR_CMD; i ++) {
      if (strcmp(cmd, cmd_table[i].name) == 0) {
        if (cmd_table[i].handler(args) < 0) { return; }
        break;
      }
    }

    if (i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
  }
}
