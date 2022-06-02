/* Assembler code fragment for LC-2K */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define MAXLINELENGTH 1000

// R-type			
#define ADD 0x00000000
#define NOR 0x00400000

// I-type
#define LW 0x00800000
#define SW 0x00C00000
#define BEQ 0x01000000

// J-type
#define JALR 0x01400000

// O-type
#define HALT 0x01800000
#define NOOP 0x01C00000

int readAndParse(FILE *, char *, char *, char *, char *, char *);
int isNumber(char *);

char label_list[1<<16][7];

int checkDup(char *label, int cnt_line) {
	int i;
	for (i = 0; i < cnt_line; i++) {
		if (!strcmp(label_list[i], label)) {
			return i;
		}
	}
	return -1;
}

int main(int argc, char *argv[])
{
	char *inFileString, *outFileString;
	FILE *inFilePtr, *outFilePtr;
	char label[MAXLINELENGTH], opcode[MAXLINELENGTH], arg0[MAXLINELENGTH], arg1[MAXLINELENGTH], arg2[MAXLINELENGTH];
	int countLine = 0, index = 0, i = 0;
	int memory[65536];
	int regA, regB, offsetField, opCode, machineCode;

	if (argc != 3) {
		printf("error: usage: %s <assembly-code-file> <machine-code-file>\n", argv[0]);
		exit(1);
	}
	inFileString = argv[1];
	outFileString = argv[2];

	inFilePtr = fopen(inFileString, "r");
	if (inFilePtr == NULL) {
		printf("error in opening %s\n", inFileString);
		exit(1);
	}

	outFilePtr = fopen(outFileString, "w");
	if (outFilePtr == NULL) {
		printf("error in opening %s\n", outFileString);
		exit(1);
	}

	while(1) {
		/* here is an example for how to use readAndParse to read a line from inFilePtr */
		if (! readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
		/* reached end of file */
			break;
		}
		if(!strcmp(label, "")) {
			countLine++;
			continue;
		}
		if(strlen(label) > 6 || ('0' <= label[0] && label[0] <= '9')) {
			printf("error: invalid label name\n");
			if (strlen(label) > 6)
				printf("%s is too long\n", label);
			else
				printf("%s : don't start number\n", label);
			exit(1);
		}
		if(checkDup(label, countLine) != -1) {
			printf("error: duplicate label\n");
			printf("%s\n", label);
			exit(1);
		}

		strcpy(label_list[countLine++], label);
	}

	/* this is how to rewind the file ptr so that you start reading from the beginning of the file */
	rewind(inFilePtr);

	while(1) {
		/* here is an example for how to use readAndParse to read a line from inFilePtr */
		if (! readAndParse(inFilePtr, label, opcode, arg0, arg1, arg2) ) {
		/* reached end of file */
			break;
		}
		regA = regB = offsetField = opCode = machineCode = 0;

		/* after doing a readAndParse, you may want to do the following to test the opcode */
		// R-type instr
		if (!strcmp(opcode, "add") || !strcmp(opcode, "nor")) {
		/* do whatever you need to do for opcode "add" */
			if(!(isNumber(arg0) && isNumber(arg1) && isNumber(arg2))) {
				printf("error: invalid argument\n");
				printf("%s %s %s %s\n", opcode, arg0, arg1, arg2);
				exit(1);
			}
			if(!strcmp(opcode, "add"))
				opCode = ADD;
			else
				opCode = NOR;

			regA = atoi(arg0) << 19;
			regB = atoi(arg1) << 16;
			offsetField = atoi(arg2);

			machineCode = regA | regB | offsetField;
		}
		// I-type instr
		else if (!strcmp(opcode, "lw") || !strcmp(opcode, "sw") || !strcmp(opcode, "beq")) {
			if(!(isNumber(arg0) && isNumber(arg1)) || !strcmp(arg2, "")) {
				printf("error: invalid argument\n");
				printf("%s %s %s %s\n", opcode, arg0, arg1, arg2);
				exit(1);
			}
			if(!strcmp(opcode, "lw"))
				opCode = LW;
			else if(!strcmp(opcode, "sw"))
				opCode = SW;
			else
				opCode = BEQ;

			regA = atoi(arg0) << 19;
			regB = atoi(arg1) << 16;

			if(isNumber(arg2))
				offsetField = atoi(arg2);
			else {
				if ((offsetField = checkDup(arg2, countLine)) == -1) {
					printf("error: unknown label\n");
					printf("%s %s %s %s\n", opcode, arg0, arg1, arg2);
					exit(1);
				}

				if(opCode == BEQ)
					offsetField -= (index + 1);
			}
			if (offsetField < -32768 || offsetField > 32767) {
				printf("error: invalid offsetField range\n");
				printf("%d\n", offsetField);
				exit(1);
			}

			if (opCode == BEQ)
				offsetField &= 0x0000ffff;
			machineCode = regA | regB | offsetField;
		}
		// J-type
		else if(!strcmp(opcode, "jalr")) {
			if(!isNumber(arg0) && isNumber(arg1)) {
				printf("error: invalid argument\n");
				printf("%s %s %s\n", opcode, arg0, arg1);
				exit(1);
			}
			opCode = JALR;

			regA = atoi(arg0) << 19;
			regB = atoi(arg1) << 16;

			machineCode = regA | regB;
		}
		// O-type
		else if(!strcmp(opcode, "halt") || !strcmp(opcode, "noop")) {
			if(!strcmp(opcode, "halt"))
				opCode = HALT;
			else
				opCode = NOOP;
		}
		// .fill
		else if(!strcmp(opcode, ".fill")) {
			if(!strcmp(arg0, "")) {
				printf("error: invalid argument\n");
				exit(1);
			}

			if(isNumber(arg0))
				machineCode = atoi(arg0);
			else {
				if((machineCode = checkDup(arg0, countLine)) == -1) {
					printf("error: label not found\n");
					printf("%s\n", label);
					exit(1);
				}
			}
		}
		else {
			printf("error: wrong opcode name\n");
			printf("%s\n", opcode);
			exit(1);
		}

		memory[index++] = opCode | machineCode;
	}

	for(i = 0; i < countLine; i++){
		printf("(address %d): %d (hex 0x%x)\n", i, memory[i], memory[i]);
		if(outFilePtr != NULL)
			fprintf(outFilePtr, "%d\n", memory[i]);
	}

	exit(0);
}
/*
* Read and parse a line of the assembly-language file. Fields are returned
* in label, opcode, arg0, arg1, arg2 (these strings must have memory already
* allocated to them). 
*
* Return values:
* 0 if reached end of file
* 1 if all went well 
*
* exit(1) if line is too long.
*/

int readAndParse(FILE *inFilePtr, char *label, char *opcode, char *arg0, char *arg1, char *arg2) {
	char line[MAXLINELENGTH];
	char *ptr = line;

	/* delete prior values */
	label[0] = opcode[0] = arg0[0] = arg1[0] = arg2[0] = '\0';

	/* read the line from the assembly-language file */
	if (fgets(line, MAXLINELENGTH, inFilePtr) == NULL)
		/* reached end of file */
		return 0;

	/* check for line too long (by looking for a \n) */
	if (strchr(line, '\n') == NULL) {
		/* line too long */
		printf("error: line too long\n");
		exit(1);
	}

	/* is there a label? */
	ptr = line;
	if (sscanf(ptr, "%[^\t\n\r ]", label))
		/* successfully read label; advance pointer over the label */
		ptr += strlen(label);

	/*
	* Parse the rest of the line. Would be nice to have real regular
	* expressions, but scanf will suffice.
	*/
	sscanf(ptr, "%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]%*[\t\n\r ]%[^\t\n\r ]", opcode, arg0, arg1, arg2);
	
	return 1;
}

int isNumber(char *str) {
	/* return 1 if string is a number */
	int i;
	return ((sscanf(str, "%d", &i)) == 1);
}