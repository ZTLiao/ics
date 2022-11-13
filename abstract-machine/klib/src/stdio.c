#include <am.h>
#include <klib.h>
#include <klib-macros.h>
#include <stdarg.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

#define ASCII " !\"#$\%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"

int my_putchar(int c);

int my_sys_write(int fd, const void *buf, int count);

int my_itoa(int n, char *buffer);

int printf(const char *fmt, ...) {
	va_list ap;
	va_start(ap, fmt);
	char buffer[128];
	int written = vsprintf(buffer, fmt, ap);
	char *tmpbuf = buffer;
	int i = 0;
	while (i < written) {
		my_putchar(tmpbuf[i++]);
	}
	va_end(ap);
	return written;
}

int vsprintf(char *out, const char *fmt, va_list ap) {
	int written = 0;
	char tmpbuf[21];
	while (*fmt) {
		if (*fmt == '%') {
			fmt++;
			switch (*fmt) {
				case 'd':
				case 'i':
				{
					char *buffer = tmpbuf;
					int var = va_arg(ap, int);
					int len = my_itoa(var, buffer);
					int i = 0;
					while (i < len) {
						out[written++] = buffer[i++];
					}
				}
				break;
				case 'b':
				break;
				case 'B':
				break;
				case 'o':
				break;
				case 'u':
				break;
				case 'x':
				break;
				case 'X':
				break;
				case 'f':
				break;
				case 'e':
				break;
				case 'E':
				break;
				case 'g':
				break;
				case 'G':
				break;
				case 'a':
				break;
				case 'A':
				break;
				case 'c':
				{
					int c = va_arg(ap, int);
					out[written++] = ASCII[c - 32];
				}
				break;
				case 'C':
				break;
				case 's':
				{
					char *buffer = va_arg(ap, char *);
					while (*buffer) {
						out[written++] = *(buffer++);
					}
				}
				break;
				case 'S':
				break;
				case 'p':
				break;
				case 'n':
				break;
				case 'm':
				break;
				case '%':
				default:
					out[written++] = *fmt;
				break;
			}
		} else {
			out[written++] = *fmt;
		}
		fmt++;
	}
  return written;
}

int sprintf(char *out, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  int written = vsprintf(out, fmt, ap);
  va_end(ap);
  return written;
}

int snprintf(char *out, size_t n, const char *fmt, ...) {
  va_list ap;
  va_start(ap, fmt);
  char buffer[n];
  int written = vsprintf(buffer, fmt, ap);
  char *tmpbuf = buffer;
  while (n--) {
       *(out++) = *(tmpbuf++);
  }
  va_end(ap);
  return written;
}

int vsnprintf(char *out, size_t n, const char *fmt, va_list ap) {
	char buffer[n];
	int written = vsprintf(buffer, fmt, ap);
	char *tmpbuf = buffer;
	while (n--) {
		*(out++) = *(tmpbuf++);
	}
	return written;
}

int my_putchar(int c) {
	unsigned char ch = c;
	int fd = 1;
	if (my_sys_write(fd, &ch, 1) <= 0) {
		return -1;
	}
	return ch;
}

int my_sys_write(int fd, const void *buf, int count) {
	long _ret;
	register long _num  __asm__("rax") = 0x1;
	register long _arg1 __asm__("rdi") = (long) (fd);
	register long _arg2 __asm__("rsi") = (long) (buf);
	register long _arg3 __asm__("rdx") = (long) (count);
	__asm__ volatile (
	"syscall;"
	: "=a"(_ret)
	: "r"(_arg1), "r"(_arg2), "r"(_arg3), "0"(_num)
	: "rcx", "r11", "memory", "cc"
	);
	return (int) _ret;
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
