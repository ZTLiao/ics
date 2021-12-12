#include <stdint.h>
#include <stdio.h>

uint64_t bit_off(uint64_t, uint64_t);
uint64_t full_add(uint64_t, uint64_t, uint64_t);
uint64_t carry(uint64_t, uint64_t, uint64_t);
uint64_t add(uint64_t, uint64_t, size_t);
uint64_t multiply(uint64_t, uint64_t, size_t);
uint64_t complement(uint64_t, uint64_t, size_t);
uint64_t full_substract(uint64_t, uint64_t, uint64_t);
uint64_t borrow(uint64_t, uint64_t, uint64_t);
uint64_t substract(uint64_t, uint64_t, size_t);
uint64_t division(uint64_t, uint64_t, size_t);
uint64_t mod(uint64_t, uint64_t, size_t);

uint64_t multimod(uint64_t a, uint64_t b, uint64_t m) {
	size_t n = 0;
#ifdef __x86_64__
	n = sizeof(uint64_t);
//	printf("a = %lx, b = %lx\n", a, b);
#elif __i386__
	n = sizeof(uint32_t);
//	printf("a = %llx, b = %llx\n", a, b);
#endif
	uint64_t cout = mod(multiply(a, b, n << 3), m, n << 3);
//#ifdef __x86_64__
//	printf("hex = %lx, cout = %ld\n", cout, cout);
//#elif __i386__
//	printf("hex = %llx, cout = %lld\n", cout, cout);
//#endif
	return cout;
}

uint64_t bit_off(uint64_t s, uint64_t i) {
	return s >> i & 1;
}

uint64_t full_add(uint64_t s0, uint64_t s1, uint64_t cin) {
	return s0 ^ s1 ^ cin;	
}

uint64_t carry(uint64_t s0, uint64_t s1, uint64_t cin) {
	return (s0 & s1) | ((s0 ^ s1) & cin);
}

uint64_t add(uint64_t a, uint64_t b, size_t n) {
	uint64_t cout = 0;
	uint64_t cin = 0;
	int i;
	for (i = 0; i < n; i++) {
		uint64_t s = full_add(bit_off(a, i), bit_off(b, i), cin);
		cout |= s << i;
		cin = carry(bit_off(a, i), bit_off(b, i), cin);
	}
	cout |= cin << i;
	return cout;
}

uint64_t multiply(uint64_t a, uint64_t b, size_t n) {
#ifdef __x86_64__
	uint64_t c = 63;
	uint64_t s0 = a & 0x8000000000000000;
	uint64_t s1 = b & 0x8000000000000000;
	uint64_t e0 = s0 >> c;
	uint64_t e1 = s1 >> c;
#elif __i386__
	uint64_t c = 31;
	uint64_t s0 = a & 0x80000000;
	uint64_t s1 = b & 0x80000000;
	uint64_t e0 = s0 >> c;
	uint64_t e1 = s1 >> c;
#endif
	uint64_t cout = 0;
	uint64_t x = complement(a, e0, n);
	uint64_t y = complement(b, e1, n);
	uint64_t z = 0;
	uint64_t ACC = 0;
	uint64_t MQ = y;
	int i;
	for (i = 0; i < c; i++) {
		uint64_t bit = (MQ >> 1) & 1;
		switch (bit) {
			case 0:
				break;
			case 1:
				ACC = add(ACC, x, n);
				break;
		}
//#ifdef __x86_64__
//		printf("i = %d, bit = %ld, ACC = %lx, MQ = %lx\n", i, bit, ACC, MQ);
//#elif __i386__
//		printf("i = %d, bit = %lld, ACC = %llx, MQ = %llx\n", i, bit, ACC, MQ);
//#endif
		z |= (MQ & 1) << i;
		MQ >>= 1;
		MQ |= (ACC & 1) << c;
		ACC >>= 1;
	}
//#ifdef __x86_64__
//		printf("MQ = %lx, z = %lx\n", MQ, z);
//#elif __i386__
//		printf("MQ = %llx, z = %llx\n", MQ, z);
//#endif
	cout = MQ | (s0 ^ s1);
	return cout;
}

uint64_t complement(uint64_t a, uint64_t e, size_t n) {
	uint64_t cout = 0;
	uint64_t cin = 0;
	int i;
	for (i = 0; i < n; i++) {
		uint64_t bit = bit_off(a, i);
		cout |= ((cin & e) ^ bit) << i;
		cin |= bit;
	} 
	return cout;
}

uint64_t full_substract(uint64_t Xi, uint64_t Yi, uint64_t Bi) {
	return Xi ^ Yi ^ Bi;
}

uint64_t borrow(uint64_t Xi, uint64_t Yi,uint64_t Bi) {
	return ((~Xi) & (~Yi) & Bi) | ((~Xi) & Yi & (~Bi)) | ((~Xi) & Yi & Bi) | (Xi & Yi & Bi);
}

uint64_t substract(uint64_t Xi, uint64_t Yi, size_t n) {
	uint64_t cout = 0;
	uint64_t Bi = 0;
	int i;
	for (i = 0; i < n; i++) {
		uint64_t Di = full_substract(bit_off(Xi, i), bit_off(Yi, i), Bi);
		cout |= Di << i;
		Bi = borrow(bit_off(Xi, i), bit_off(Yi, i), Bi);
	}
	return cout;
}

uint64_t division(uint64_t a, uint64_t b, size_t n) {
#ifdef __x86_64__
	uint64_t c = 63;
	uint64_t bit = 0x8000000000000000;
	uint64_t s0 = a & 0x8000000000000000;
	uint64_t s1 = b & 0x8000000000000000;
	uint64_t e0 = s0 >> c;
	uint64_t e1 = s1 >> c;
#elif __i386__
	uint64_t c = 31;
	uint64_t bit = 0x80000000;
	uint64_t s0 = a & 0x80000000;
	uint64_t s1 = b & 0x80000000;
	uint64_t e0 = s0 >> c;
	uint64_t e1 = s1 >> c;
#endif
	uint64_t dividend = complement(a, e0, n);
	uint64_t divisor = complement(b, e1, n);
#ifdef __x86_64__
		printf("divisor = %ld, dividend = %ld\n", divisor, dividend);
#elif __i386__
		printf("divisor = %lld, dividend = %lld\n", divisor, dividend);
#endif	
	divisor <<= (n >> 1);
	uint64_t quotient = 0;
	int i;
	for (i = 0; i <= (n >> 1); i++) {
		uint64_t remainder = dividend;
		dividend = substract(dividend, divisor, n);
#ifdef __x86_64__
		printf("i = %d, divisor = %lx, dividend = %lx, (dividend & bit) = %lx, quotient = %ld\n", i, divisor, dividend, (dividend & bit), quotient);
#elif __i386__
		printf("i = %d, divisor = %llx, dividend = %llx, (dividend & bit) = %llx, quotient = %lld\n", i, divisor, dividend, (dividend & bit), quotient);
#endif	
		quotient <<= 1;
		if (dividend & bit) {
			dividend = remainder;
			quotient |= 0;
		} else {
			quotient |= 1;
		}
		divisor >>= 1;
	}
	quotient |= (s0 ^ s1);
	return quotient;
}

uint64_t mod(uint64_t a, uint64_t b, size_t n) {
#ifdef __x86_64__
	uint64_t c = 63;
	uint64_t bit = 0x8000000000000000;
	uint64_t s0 = a & 0x8000000000000000;
	uint64_t s1 = b & 0x8000000000000000;
	uint64_t e0 = s0 >> c;
	uint64_t e1 = s1 >> c;
#elif __i386__
	uint64_t c = 31;
	uint64_t bit = 0x80000000;
	uint64_t s0 = a & 0x80000000;
	uint64_t s1 = b & 0x80000000;
	uint64_t e0 = s0 >> c;
	uint64_t e1 = s1 >> c;
#endif
	uint64_t dividend = complement(a, e0, n);
	uint64_t divisor = complement(b, e1, n);
//#ifdef __x86_64__
//		printf("divisor = %ld, dividend = %ld\n", divisor, dividend);
//#elif __i386__
//		printf("divisor = %lld, dividend = %lld\n", divisor, dividend);
//#endif	
	divisor <<= (n >> 1);
	uint64_t quotient = 0;
	int i;
	for (i = 0; i <= (n >> 1); i++) {
		uint64_t remainder = dividend;
		dividend = substract(dividend, divisor, n);
//#ifdef __x86_64__
//		printf("i = %d, divisor = %lx, dividend = %lx, (dividend & bit) = %lx, quotient = %ld\n", i, divisor, dividend, (dividend & bit), quotient);
//#elif __i386__
//		printf("i = %d, divisor = %llx, dividend = %llx, (dividend & bit) = %llx, quotient = %lld\n", i, divisor, dividend, (dividend & bit), quotient);
//#endif	
		quotient <<= 1;
		if (dividend & bit) {
			dividend = remainder;
			quotient |= 0;
		} else {
			quotient |= 1;
		}
		divisor >>= 1;
	}
	return dividend;
}
