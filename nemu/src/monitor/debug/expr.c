#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <string.h>
#include <stdlib.h>

enum {
  TK_NOTYPE = 256, 
  TK_EQ,
  TK_NUM,
  TK_HEX,
  /* TODO: Add more token types */

};

static struct rule {
  char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},				// spaces
  {"==", TK_EQ},					// equal
  {"0[xX][0-9a-fA-F]+", TK_HEX},	// hex
  {"[0-9]+", TK_NUM},				// number
  {"\\+", '+'},						// plus
  {"-", '-'},						// subtract
  {"\\*", '*'},						// multiply
  {"/", '/'},						// divide
  {"=", '='},						// assignment
  {"\\(", '('},						// left brackets
  {"\\)", ')'},						// right brackets
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[65536] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

/*
 * check brackets complete
 */
static bool check_parentheses(int p, int q);

/*
 * calculation expression
 */
static uint32_t eval(int p, int q);

/*
 * get main operate symbol
 */
static int get_op(int p, int q);

static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

		if (rules[i].token_type != TK_NOTYPE) {
			Token token;
			token.type = rules[i].token_type;
			strxfrm(token.str, substr_start, substr_len);
			token.str[substr_len] = '\0';
			tokens[nr_token] = token;
			nr_token++;

			switch (rules[i].token_type) {
				case TK_NOTYPE:
					Log("token type = ' +'");
					break;
				case TK_EQ:
					Log("token type = '=='");
					break;
				case '+':
					Log("token type = '+'");
					break;
				case '-':
					Log("token type = '-'");
					break;
				case '*':
					Log("token type = '*'");
					break;
				case '/':
					Log("token type = '/'");
					break;
				case TK_NUM:
					Log("token type = '[0-9]+'");
					break;
				case '=':
					Log("token type = '='");
					break;
				case '(':
					Log("token type = '('");
					break;
				case ')':
					Log("token type = ')'");
					break;
				case TK_HEX:
					Log("token type = '0[xX][0-9a-fA-F]+'");
					break;
				default: TODO();
			}

		}
        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}


word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  Log("start evaluate expression");
  
  int i;
  for (i = 0; i < nr_token; i++) {
	  Log("i = %d\t str = '%s'", i, tokens[i].str);
  }

  /* evaluate the expression. */
  uint32_t n = eval(0, nr_token - 1);

  //Log("n = %u", n);

  return n;
}

static bool check_parentheses(int p, int q) {
	return strcmp(tokens[p].str, "(") == 0 && strcmp(tokens[q].str, ")") == 0;
}

static uint32_t eval(int p, int q) {
	if (p > q) {
		panic("expression error.");
	} else if (p == q) {
		Token token = tokens[p];	
		return (uint32_t) atoi(token.str);
	} else if (check_parentheses(p, q)) {
		return eval(p + 1, q - 1);
	} else {
		int op = get_op(p, q);
		Log("main op = %s", tokens[op].str);
		uint32_t val1 = eval(p, op - 1);
		uint32_t val2 = eval(op + 1, q);
		switch (tokens[op].type) {
			case '+': return val1 + val2;
			case '-': return val1 - val2;
			case '*': return val1 * val2;
			case '/': return val1 / val2;
			default: assert(0);
		}
	}
	return 0;
}

static int get_op(int p, int q) {
	int index = 0;
	int i;
	bool isParentheses = false;
	for (i = p; i <= q; i++) {
		if (tokens[i].type == TK_NUM) {
			continue;
		}
		switch (tokens[i].type) {
			case '(':
			case ')':
				isParentheses = !isParentheses;
				break;
			case '-':
			case '+':
				if (!isParentheses) {
					index = i;
				}
				break;
			case '*':
			case '/':
				if (!isParentheses && index == 0) {
					index = i;
				}
				break;
		}
	}
	return index;
}
