#include <am.h>
#include <klib.h>
#include <klib-macros.h>

#if !defined(__ISA_NATIVE__) || defined(__NATIVE_USE_KLIB__)
static unsigned long int next = 1;

int rand(void) {
  // RAND_MAX assumed to be 32767
  next = next * 1103515245 + 12345;
  return (unsigned int)(next/65536) % 32768;
}

void srand(unsigned int seed) {
  next = seed;
}

int abs(int x) {
  return (x < 0 ? -x : x);
}

int atoi(const char* nptr) {
  int x = 0;
  while (*nptr == ' ') { nptr ++; }
  while (*nptr >= '0' && *nptr <= '9') {
    x = x * 10 + *nptr - '0';
    nptr ++;
  }
  return x;
}

int itoa(int in, char *buffer) {
  char *ptr = buffer;
  int len = 0;
  if (in < 0) {
    in = -in;
	*(ptr++) = '-';
	len++;
  }
  unsigned long long lim;
  int digits = 0;
  int pos = 19;
  int dig;
  do {
    for (dig = 0, lim = 1; dig < pos; dig++) {
      lim *= 10;
	}
	if (digits || in >= lim || !pos) {
      for (dig = 0; in >= lim; dig++) {
	    in -= lim;
	  }
	  buffer[digits++] = '0' + dig;
	}
  } while(pos--);
  buffer[digits] = 0;
  return digits;
}

static char *mem;

void *malloc(size_t size) {
  size = (size_t) ROUNDUP(size, 4);
  if (mem == NULL) {
    mem = (void *) ROUNDUP(heap.start, 4);
  }
  char *old = mem;
  mem += size;
  assert((uintptr_t) heap.start <= (uintptr_t) mem && (uintptr_t) mem < (uintptr_t) heap.end);
  uint32_t *p;
  for (p = (uint32_t *) old; p != (uint32_t *) mem; p ++) {
    *p = 0;
  }
  return old;
}

void free(void *ptr) {
}

#endif
