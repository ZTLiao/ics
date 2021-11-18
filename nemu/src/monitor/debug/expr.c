#include <isa.h>
#include <memory/vaddr.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>
#include <string.h>
#include <stdlib.h>

enum {
  /* token types */
  TK_NOTYPE = 256, 
  TK_EQ,
  TK_NE,
  TK_NUM,
  TK_HEX,
  TK_BRACKETS,
  TK_REG,
  TK_DEREF,
  TK_NEG,
  TK_SAL,
  TK_SAR,
  TK_LE,
  TK_LT,
  TK_GE,
  TK_GT,
  TK_AND,
  TK_OR,
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
  {"!=", TK_NE},					// not equal
  {"<<", TK_SAL},					// shift arithmetic left
  {">>", TK_SAR},					// shift arithmetic right
  {"<=", TK_LE},					// less equals
  {"<", TK_LT},						// less than
  {">=", TK_GE},					// greater equals
  {">", TK_GT},						// greater than
  {"&&", TK_AND},					// and
  {"\\|\\|", TK_OR},				// or
  {"\\([0-9]+\\)", TK_BRACKETS},	// brackets number
  {"0[xX][0-9a-fA-F]+", TK_HEX},	// hex
  {"[0-9]+", TK_NUM},				// number
  {"\\+", '+'},						// plus
  {"-", '-'},						// subtract
  {"\\*", '*'},						// multiply
  {"/", '/'},						// divide
  {"=", '='},						// assignment
  {"\\(", '('},						// left brackets
  {"\\)", ')'},						// right brackets
  {"\\$[a-zA-Z]+", TK_REG},			// registers
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

static Token tokens[TOKEN_SIZE] __attribute__((used)) = {};
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
		if (rules[i].token_type == TK_NOTYPE) {
			continue;
		}
		Token token;
		token.type = rules[i].token_type;
		strxfrm(token.str, substr_start, substr_len);
		token.str[substr_len] = '\0';
		switch (rules[i].token_type) {
			case TK_NOTYPE:
				Log("token type = ' +'");
				break;
			case TK_EQ:
				Log("token type = '=='");
				break;
			case TK_NE:
				Log("token type = '!='");
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
				Log("before token.str = %s", token.str);
				sprintf(token.str, "%ld", strtol(token.str, NULL, 16));
				Log("after token.str = %s", token.str);
				break;
			case TK_BRACKETS:
				Log("token type = '([0-9]+)'");
				token.type = TK_NUM;
				Log("before token.str = %s", token.str);
				strxfrm(token.str, token.str + 1, strlen(token.str) - 2);	
				int length = strlen(token.str);
				token.str[length - 2] = '\0';
				Log("after token.str = %s", token.str);
				break;
			case TK_REG:
				Log("token type = '$[a-zA-Z]+'");
				int len = strlen(token.str);
				int i;
				for (i = 1; i < len; i++) {
					int c = token.str[i];
					if (c < 97) {
						token.str[i] = (char) (c + 32);
					}
				}
				bool b;
				word_t num = isa_reg_str2val(token.str, &b);
				if (b) {
					sprintf(token.str, "%d", num); 
					token.type = TK_NUM;
				}
				break;
			case TK_LE:
				Log("token type = '<='");
				break;
			case TK_LT:
				Log("token type = '<'");
				break;
			case TK_GE:
				Log("token type = '>='");
				break;
			case TK_GT:
				Log("token type = '>'");
				break;
			case TK_AND:
				Log("token type = '&&'");
				break;
			case TK_OR:
				Log("token type = '||'");
				break;
			case TK_SAL:
				Log("token type = '<<'");
				break;
			case TK_SAR:
				Log("token type = '>>'");
				break;
			default: TODO();
		}
		tokens[nr_token] = token;
		nr_token++;
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
	  printf("%s", tokens[i].str);
	  if (tokens[i].type == '*' && (i == 0 || tokens[i - 1].type != TK_NUM)) {
		  tokens[i].type = TK_DEREF;
	  }
	  if (tokens[i].type == '-' && (i == 0 || tokens[i - 1].type != TK_NUM)) {
		  tokens[i].type = TK_NEG;
	  }
  }
  printf("\n");
  /* evaluate the expression. */
  return eval(0, nr_token - 1);
}

static bool check_parentheses(int p, int q) {
	if (strcmp(tokens[p].str, "(") == 0 && strcmp(tokens[q].str, ")") == 0) {
		int n = 0;
		int i;
		for (i = p; i <= q; i++) {
			if (strcmp(tokens[i].str, "(") == 0) {
				n++;
			}
			if (strcmp(tokens[i].str, ")") == 0) {
				n--;
			}
		}
		return n == 0;
	} else {
		return false;
	}
}

static uint32_t eval(int p, int q) {
	if (p > q) {
		Log("p = %d, tokens[%d] = %s", p, p, tokens[p].str);
		Log("q = %d, tokens[%d] = %s", q, q, tokens[q].str);
		panic("expression error.");
	} else if (p == q) {
		Token token = tokens[p];	
		return (uint32_t) atoi(token.str);
	} else if (check_parentheses(p, q)) {
		Log("p + 1 = %d, q - 1 = %d", (p + 1), (q - 1));
		return eval(p + 1, q - 1);
	} else {
		int op = get_op(p, q);
		Log("main tokens[%d].str = %s", op, tokens[op].str);
		Log("p = %d, op - 1 = %d", p, (op - 1));
		Log("op + 1 = %d, q = %d", (op + 1), q);
		uint32_t val1 = 0;
		uint32_t val2 = 0;
		if (tokens[op].type == TK_DEREF || tokens[op].type == TK_NEG) {
			val2 = eval(op + 1, q);
		} else {
			val1 = eval(p, op - 1);
			val2 = eval(op + 1, q);
		}
		Log("val1 = %d, val2 = %d", val1, val2);
		switch (tokens[op].type) {
			case '+': 
				Log("%d + %d = %d", val1, val2, (val1 + val2));
				return val1 + val2;
			case '-': 
				Log("%d - %d = %d", val1, val2, (val1 - val2));
				return val1 - val2;
			case '*': 
				Log("%d * %d = %d", val1, val2, (val1 * val2));
				return val1 * val2;
			case '/': 
				Log("%d / %d = %d", val1, val2, (val1 / val2));
				return val1 / val2;
			case TK_DEREF:
				Log("*%d", val2);
				return vaddr_read(val2, 4);
			case TK_NEG:
				Log("-%d = %d", val2, -val2);
				return -val2;
			case TK_LE:
				Log("%d <= %d = %d", val1, val2, (val1 <= val2));
				return val1 <= val2;
			case TK_LT:
				Log("%d < %d = %d", val1, val2, (val1 < val2));
				return val1 < val2;
			case TK_GE:
				Log("%d >= %d = %d", val1, val2, (val1 >= val2));
				return val1 >= val2;
			case TK_GT:
				Log("%d > %d = %d", val1, val2, (val1 > val2));
				return val1 > val2;
			case TK_AND:
				Log("%d && %d = %d", val1, val2, (val1 && val2));
				return val1 && val2;
			case TK_OR:
				Log("%d || %d = %d", val1, val2, (val1 || val2));
				return val1 || val2;
			case TK_SAL:
				Log("%d << %d = %d", val1, val2, (val1 << val2));
				return val1 << val2;
			case TK_SAR:
				Log("%d >> %d = %d", val1, val2, (val1 >> val2));
				return val1 >> val2;
			case TK_EQ:
				Log("%d == %d = %d", val1, val2, (val1 == val2));
				return val1 == val2;
			case TK_NE:
				Log("%d != %d = %d", val1, val2, (val1 != val2));
				return val1 != val2;
			default: assert(0);
		}
	}
	return 0;
}

static char symbol[12][6][4] = {
	{{'(', '\0'}, {')', '\0'}, {'[', '\0'}, {']', '\0'}, {'.', '\0'}},
	{{'!', '\0'}, {'~', '\0'}, {'+', '+', '\0'}, {'-', '-', '\0'}},
	{{'*', '\0'}, {'/', '\0'}, {'%', '\0'}},
	{{'+', '\0'}, {'-', '\0'}},
	{{'<', '<', '\0'}, {'>', '>', '\0'}, {'>', '>', '>', '\0'}},
	{{'<', '\0'}, {'<', '=', '\0'}, {'>', '\0'}, {'>', '=', '\0'}},
	{{'=', '=', '\0'}, {'!', '=', '\0'}},
	{{'&', '\0'}},
	{{'^', '\0'}},
	{{'|', '\0'}},
	{{'&', '&', '\0'}},
	{{'|', '|', '\0'}}
};

static int get_op(int p, int q) {
	int weight[TOKEN_SIZE] = { 0 };
	bool b = false;
	int i;
	for (i = p; i <= q; i++) {
		Token token = tokens[i];
		int token_type = token.type;
		if (token_type == TK_NUM || token_type == TK_REG) {
			weight[i] = 0;
			continue;
		}
		if (token_type == TK_DEREF || token_type == TK_NEG) {
			weight[i] = 1;
			continue;
		}
		if (token_type == '(') {
			b = true;
		}
		if (token_type == ')') {
			b = false;
		}
		if (b) {
			weight[i] = 0;
			continue;
		}
		if (token_type == '(' || token_type == ')') {
			weight[i] = 0;
			continue;
		}
		int j, k;
		for (j = 0; j < (sizeof(symbol) / sizeof(symbol[0])); j++) {
			for (k = 0; k < (sizeof(symbol[j]) / sizeof(symbol[j][0])); k++) {
				if (strcmp(symbol[j][k], token.str) == 0) {
					Log("symbol[%d][%d] = %s", j, k, symbol[j][k]);
					weight[i] = j;
					break;
				}
			}
		}
	}
	int index = 0;
	for (i = 0; i < (sizeof(weight) / sizeof(weight[0])); i++) {
		//Log(" weight[%d] = %d", i, weight[i]);
		Token token = tokens[i];
		int token_type = token.type;
		if (weight[i] == 0 || token_type == TK_NUM) {
			continue;
		}
		if ((weight[i] > weight[index]) || (weight[i] == weight[index] && (weight[i] + i) > (weight[index] + index))) {
			index = i;
		}
	} 
	return index;
}
