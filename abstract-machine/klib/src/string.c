#include <klib.h>
#include <stdint.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)

size_t strlen(const char *s) {
  const char *sc;
  for (sc = s; *sc != '\0'; ++sc);
  return sc - s;
}

char *strcpy(char* dst, const char* src) {
  char *tmp = dst;
  while ((*dst++ = *src++) != '\0');
  return tmp;
}

char* strncpy(char* dst, const char* src, size_t n) {
  char *tmp = dst;
  while (n--) {
	if ((*dst = *src) != '\0') {
		src++;
	}  
	dst++;
  }
  return tmp;
}

char* strcat(char* dst, const char* src) {
  char *tmp = dst;
  while (*dst || *src) {
    if (*dst != '\0') {
	  dst++;
	}
	if (*dst == '\0' && *src != '\0') {
	  *dst++ = *src++;
	  *dst = '\0';
	}
  }
  return tmp;
}

int strcmp(const char* s1, const char* s2) {
  int res = 0;
  while (*s1 != '\0' && *s2 != '\0') {
	if ((res = (*s1 - *s2)) != 0) {
	  break;
	}
	s1 ++;
	s2 ++;
  }
  return res;
}

int strncmp(const char* s1, const char* s2, size_t n) {
  int b = 1;
  while (n--) {
    if (*s1++ != *s2++) {
		b = 0;
		break;
	}
  }
  return b;
}

void* memset(void* v, int c, size_t n) {
  char *ch = (char *) v;
  while (n--) {
	 *ch++ = c;
  }
  return v;
}

void* memmove(void* dst, const void* src, size_t n) {
  char *dst_tmp = (char *)dst;
  const char *src_tmp = (const char *)src;
  if (dst_tmp <= src_tmp || src_tmp + n < dst_tmp) {
    while (n --) {
	  *dst_tmp ++ = *src_tmp ++;
	}
  } else {
    dst_tmp += n - 1;
	src_tmp += n - 1;
	while (n --) {
	  *dst_tmp ++ = *src_tmp ++;
	}
  }
  return dst;
}

void* memcpy(void* out, const void* in, size_t n) {
  char *t1 = (char *)out;
  char *t2 = (char *)in;
  while (n--) {
	*t1++ = *t2++;
  }
  return out;
}

int memcmp(const void* s1, const void* s2, size_t n) {
  int res = 0;
  char *t1 = (char *)s1;
  char *t2 = (char *)s2;
  while (n--) {
    if ((res = (*t1 - *t2)) != 0) {
	  break;
	}
	t1 ++;
	t2 ++;
  }
  return res;
}

#endif
