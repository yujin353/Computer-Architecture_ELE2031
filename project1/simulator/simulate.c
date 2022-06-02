/* LC-2K Instruction-level simulator */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define NUMMEMORY 65536 /* maximum number of words in memory */
#define NUMREGS 8 /* number of machine registers */
#define MAXLINELENGTH 1000

#define ADD 0
#define NOR 1
#define LW 2
#define SW 3
#define BEQ 4
#define JALR 5
#define HALT 6
#define NOOP 7

typedef struct stateStruct {
	int pc;
	int mem[NUMMEMORY];
	int reg[NUMREGS];
	int numMemory;
} stateType;

void printState(stateType *);
int convertNum(int num);

int main(int argc, char *argv[])
{
	char line[MAXLINELENGTH];
	stateType state;
	FILE *filePtr;
	int halted = 0;
	int countInstr = 0;
	int regA, regB, destReg, offsetField, address;

	if (argc != 2) {
		printf("error: usage: %s <machine-code file>\n", argv[0]);
		exit(1);
	}
	
	filePtr = fopen(argv[1], "r");
	if (filePtr == NULL) {
		printf("error: can't open file %s", argv[1]);
		perror("fopen");
		exit(1);
	}
	
	/* read in the entire machine-code file into memory */
	for (state.numMemory = 0; fgets(line, MAXLINELENGTH, filePtr) != NULL; state.numMemory++) {
		if (sscanf(line, "%d", state.mem+state.numMemory) != 1) {
			printf("error in reading address %d\n", state.numMemory);
			exit(1);
		}
		printf("memory[%d]=%d\n", state.numMemory, state.mem[state.numMemory]);
	}

	state.pc = 0;
	memset(state.reg, 0, (sizeof(int) * NUMREGS));

	while(!halted) {
		int instr = state.mem[state.pc];
		int opCode = instr >> 22;

		printState(&state);

		regA = (instr >> 19) & 0x00000007;
		regB = (instr >> 16) & 0x00000007;
		destReg = instr & 0x00000007;
		offsetField = instr & 0x0000FFFF;

		switch(opCode) {
			case ADD:
				state.reg[destReg] = state.reg[regA] + state.reg[regB];
				break;
			case NOR:
				state.reg[destReg] = ~(state.reg[regA] | state.reg[regB]);
				break;
			case LW:
				address = convertNum(offsetField + state.reg[regA]);
				state.reg[regB] = state.mem[address];
				break;
			case SW:
				address = convertNum(offsetField + state.reg[regA]);
				state.mem[address] = state.reg[regB];
				break;
			case BEQ:
				if(state.reg[regA] == state.reg[regB])
					state.pc += convertNum(offsetField);
				break;
			case JALR:
				state.reg[regB] = state.pc + 1;
				state.pc = state.reg[regA] - 1;
				break;
			case HALT:
				halted = 1;
				break;
			case NOOP:
				break;
			default:
				printf("error: wrong opcod name\n");
				break;
		}

		state.pc++;
		countInstr++;
	}
	printf("\nmachine halted\n");
	printf("total of %d instructions executed\n", countInstr);
	printf("final state of machine:\n");
	
	printState(&state);

	fclose(filePtr);
	exit(0);
}
void printState(stateType *statePtr) {
	int i;

	printf("\n@@@\nstate:\n");
	printf("\tpc %d\n", statePtr->pc);
	printf("\tmemory:\n");

	for (i=0; i<statePtr->numMemory; i++) {
		printf("\t\tmem[ %d ] %d\n", i, statePtr->mem[i]);
	}
	printf("\tregisters:\n");

	for (i=0; i<NUMREGS; i++) {
		printf("\t\treg[ %d ] %d\n", i, statePtr->reg[i]);
	}
	printf("end state\n");
}

int convertNum(int num) {
	if (num & (1 << 15)) {
		num -= (1 << 16);
	}
	return(num);
}