#include <stdio.h>
#include <stdint.h>

#define NREG 4
#define NMEM 16

typedef union {
	struct {
		uint8_t rs:2, rt:2, op:4;
	} rtype;
	struct {
		uint8_t addr:4, op:4;
	} mtype;
	uint8_t inst;
} inst_t;

#define DECODE_R(inst) uint8_t rs = (inst).rtype.rs, rt = (inst).rtype.rt
#define DECODE_M(inst) uint8_t addr = (inst).mtype.addr

int pc = 0;
int halt = 0;
uint8_t R[NREG] = {};
uint8_t M[NMEM] = {
	0b11100110,
	0b00010100,
	0b11100101,
	0b00100001,
	0b11110111,
	0b00000001,
	0b00000010,
	0b00000000
};

void exec_once() {
	inst_t this;
	this.inst = M[pc];
	DECODE_R(this);
	DECODE_M(this);
	printf("inst = %u\n", this.inst);
	printf("R[%d] = %d, R[%d] = %d\n", rs, rt, R[rs], R[rt]);
	switch (this.rtype.op) {
		case 0b0001:
			R[rt] = R[rs];
			break;
		case 0b0010:
			R[rt] += R[rs];
			break;
		case 0b1110:
			R[0] = M[addr];
			break;
		case 0b1111:
			printf("M[%d] = %d, R[0] = %d\n", addr, M[addr], R[0]);
			M[addr] = R[0];
			break;
		default:
			printf("not instruction run!\n");
			halt = 1;
			break;
	}
	pc++;
}

int main(int argv, char* argc[]) {
	while (1) {
		exec_once();
		if (halt) {
			printf("M[%d] + M[%d] = %d + %d = %d\n", 5, 6, M[5], M[6], M[7]);
			break;
		}
	}
	return 0;
}
