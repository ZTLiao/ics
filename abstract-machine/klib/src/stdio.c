#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

int my_itoa(int n, char *buffer);

int printf(const char *fmt, ...) {
  return 0;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
  return 0;
}

int sprintf(char *out, const char *fmt, ...) {
  int written = 0;
  char tmpbuf[21];
  va_list ap;
  va_start(ap, fmt);
  while (*fmt) {
	if (*fmt == '%') {
		fmt++;
		switch (*fmt) {
			case 'd':
			{
				char *buffer = tmpbuf;
				int var = va_arg(ap, int);
				int len = my_itoa(var, buffer);
				int i = 0;
				while (i < len) {
					*(out++) = buffer[i++];
				}
				written = 1;
			}
			break;
			case 's':
			{
				char *buffer = va_arg(ap, char *);
				while (*buffer) {
					*(out++) = *(buffer++);
				}
				written = 1;
			}
			break;
		}
	} else {
		*(out++) = *fmt;
	}
	fmt++;
  }
  va_end(ap);
  return written;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  return 0;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
  return 0;
}

int my_itoa(int n, char *buffer) {
	int len = 0;
	if (n < 0) {
	    n = -n;
	    *(buffer++) = '-';
	    len++;
	}
	int pos = 9;
	int i;
	int lim = 0;
	do {
	    for (i = 0, lim = 1; i < pos; i++) {
	        lim *= 10;
	    }
	    if (len || n >= lim || !pos) {
	        for (i = 0; n >= lim; i++) {
	            n -= lim;
	        }
	        buffer[len++] = '0' + i;
	    }
	} while(pos--);
	buffer[len] = 0;
	return len;
}

#endif
