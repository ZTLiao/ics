#include <klibtest.h>

void check_seq(int l, int r, int val) {
  int i;
  for (i = l; i < r; i ++) {
	  assert(data[i] == val + i - l);
  }
}

void check_eq(int l, int r, int val) {
  int i;
  for (i = l; i < r; i ++) {
    assert(data[i] == val);
  }
}

void test_memset() {
  int l, r;
  for (l = 0; l < N; l++) {
    for (r = l + 1; r <= N; r++) {
      reset();
	  uint8_t val = (l + r) / 2;
	  memset(data + l, val, r - l);
	  check_seq(0, l, 1);
	  check_eq(l, r, val);
	  check_seq(r, N, r + 1);
	}
  }
}
