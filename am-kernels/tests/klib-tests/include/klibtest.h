#ifndef __KLIBTEST_H__
#define __KLIBTEST_H__

#include <am.h>
#include <klib.h>
#include <klib-macros.h>

#define N 32
typedef unsigned char uint8_t;
static uint8_t data[N];

extern void (*entry)();

static inline void reset() {
  int i;
  for (i = 0; i < N; i ++) {
	  data[i] = i + 1;
  }
}

#define CASE(id, entry_, ...) \
		case id: { \
			void entry_(); \
			entry = entry_; \
			__VA_ARGS__; \
			entry_(); \
			break; \
		}

#endif

