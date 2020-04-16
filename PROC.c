#include <stdio.h>	/* fprintf(), printf() */
#include <stdlib.h>	/* atoi() */
#include <stdint.h>	/* uint32_t */

#include "RegFile.h"
#include "Syscall.h"
#include "utils/heap.h"
#include "elf_reader/elf_reader.h"

int main(int argc, char * argv[]) {

	/*
	 * This variable will store the maximum
	 * number of instructions to run before
	 * forcibly terminating the program. It
	 * is set via a command line argument.
	 */
	uint32_t MaxInstructions;

	/*
	 * This variable will store the address
	 * of the next instruction to be fetched
	 * from the instruction memory.
	 */
	uint32_t ProgramCounter;

	/*
	 * This variable will store the instruction
	 * once it is fetched from instruction memory.
	 */
	uint32_t CurrentInstruction;

	//IF THE USER HAS NOT SPECIFIED ENOUGH COMMAND LINE ARUGMENTS
	if(argc < 3){

		//PRINT ERROR AND TERMINATE
		fprintf(stderr, "ERROR: Input argument missing!\n");
		fprintf(stderr, "Expected: file-name, max-instructions\n");
		return -1;

	}

     	//CONVERT MAX INSTRUCTIONS FROM STRING TO INTEGER	
	MaxInstructions = atoi(argv[2]);	

	//Open file pointers & initialize Heap & Regsiters
	initHeap();
	initFDT();
	initRegFile(0);

	//LOAD ELF FILE INTO MEMORY AND STORE EXIT STATUS
	int status = LoadOSMemory(argv[1]);

	//IF LOADING FILE RETURNED NEGATIVE EXIT STATUS
	if(status < 0){ 
		
		//PRINT ERROR AND TERMINATE
		fprintf(stderr, "ERROR: Unable to open file at %s!\n", argv[1]);
		return status; 
	
	}

	printf("\n ----- BOOT Sequence ----- \n");
	printf("Initializing sp=0x%08x; gp=0x%08x; start=0x%08x\n", exec.GSP, exec.GP, exec.GPC_START);

	RegFile[28] = exec.GP;
	RegFile[29] = exec.GSP;
	RegFile[31] = exec.GPC_START;

	printRegFile();

	printf("\n ----- Execute Program ----- \n");
	printf("Max Instruction to run = %d \n",MaxInstructions);
	fflush(stdout);
	ProgramCounter = exec.GPC_START;
	
	/***************************/
	/* ADD YOUR VARIABLES HERE */
	/***************************/
	
	uint8_t opcode;
	uint8_t rs;
	uint8_t rt;
	uint8_t rd;
	uint8_t shamt;
	uint8_t funct;
	uint32_t jump_index;
	uint32_t syscall_code;

	char type;


	int i;
	for(i = 0; i < MaxInstructions; i++) {

		//FETCH THE INSTRUCTION AT 'ProgramCounter'		
		CurrentInstruction = readWord(ProgramCounter,false);

		//PRINT CONTENTS OF THE REGISTER FILE	
		printRegFile();
		
		/********************************/
		/* ADD YOUR IMPLEMENTATION HERE */
		/********************************/

		opcode = (CurrentInstruction & 0xFC000000) >> 26;
		

		if (opcode == 0)
		{
			type = 'r';
		}

		else if (opcode == 2 || opcode == 3)
		{
			type = 'j';
		}

		else 
		{
			type = 'i';
		}

		if (type == 'r')
		{
			funct = (CurrentInstruction & 0x0000003F);

			if (funct == 12) {
				syscall_code = (CurrentInstruction & 0x03ffffc0) >> 6;
			}

			else {
				rs = (CurrentInstruction & 0x03e00000) >> 21;
				rt = (CurrentInstruction & 0x001f0000) >> 16;
				rd = (CurrentInstruction & 0x0000f800) >> 11;
				shamt = (CurrentInstruction & 0x000007c0) >> 6;
			}

			// add instruction
			if (funct == 32)
			{
				int32_t source1 = RegFile[rs];
				int32_t source2 = RegFile[rt];
				int32_t result;

				result = source1 + source2;
				RegFile[rd] = result;

				ProgramCounter += 4;
			}

			// add unsigned instruction
			else if (funct == 33)
			{
				uint32_t source1 = (uint32_t)RegFile[rs];
				uint32_t source2 = (uint32_t)RegFile[rt];
				uint32_t result;

				result = source1 + source2;
				RegFile[rd] = (int32_t)result;

				ProgramCounter += 4;
			}

			// subtract instruction
			else if (funct == 34)
			{
				int32_t source1 = RegFile[rs];
				int32_t source2 = RegFile[rt];
				int32_t result;

				result = source1 - source2;
				RegFile[rd] = result;

				ProgramCounter += 4;
			}

			// subtract unsigned instruction
			else if (funct == 35)
			{
				uint32_t source1 = (uint32_t)RegFile[rs];
				uint32_t source2 = (uint32_t)RegFile[rt];
				uint32_t result;

				result = source1 - source2;
				RegFile[rd] = (int32_t)result;

				ProgramCounter += 4;
			}

			// bitwise AND instruction
			else if (funct == 36)
			{
				uint32_t source1 = (uint32_t)RegFile[rs];
				uint32_t source2 = (uint32_t)RegFile[rt];
				uint32_t result;

				result = source1 & source2;
				RegFile = (int32_t)result;

				ProgramCounter += 4;
			}

			// bitwise OR instruction
			else if (funct == 37)
			{
				uint32_t source1 = (uint32_t)RegFile[rs];
				uint32_t source2 = (uint32_t)RegFile[rt];
				uint32_t result;

				result = source1 | source2;
				RegFile = (int32_t)result;

				ProgramCounter += 4;
			}

			// bitwise XOR instruction
			else if (funct == 38)
			{
				uint32_t source1 = (uint32_t)RegFile[rs];
				uint32_t source2 = (uint32_t)RegFile[rt];
				uint32_t result;

				result = source1 ^ source2;
				RegFile[rd] = (int32_t)result;

				ProgramCounter += 4;
			}

			// bitwise NOR instruction
			else if (funct == 39)
			{
				uint32_t source1 = (uint32_t)RegFile[rs];
				uint32_t source2 = (uint32_t)RegFile[rt];
				uint32_t result;

				result = ~(source1 | source2);
				RegFile[rd] = (int32_t)result;

				ProgramCounter += 4;
			}

			// set on less than instruction
			else if (funct == 42)
			{
				int32_t source1 = RegFile[rs];
				int32_t source2 = RegFile[rt];
				int32_t result;

				if (source1 < source2) {
					result = 1;
				}

				else {
					result = 0;
				}

				RegFile[rd] = result;

				ProgramCounter += 4;
			}

			// set on less than unsigned instruction
			else if (funct == 43)
			{
				uint32_t source1 = (uint32_t)RegFile[rs];
				uint32_t source2 = (uint32_t)RegFile[rt];
				uint32_t result;
				
				if(source1 < source2) {
					result = 1;
				}

				else {
					result = 0;
				}

				RegFile[rd] = (int32_t)result;

				ProgramCounter += 4;
			}

			// shift left logical instruction
			else if (funct == 0)
			{
				uint32_t source1 = (uint32_t)RegFile[rs];
				uint32_t source2 = (uint32_t)RegFile[rt];
				uint32_t result;
				
				if(source1 < source2) {
					result = 1;
				}

				else {
					result = 0;
				}

				RegFile[rd] = (int32_t)result;

				ProgramCounter += 4;
			}

			// shift right logical instruction
			else if (funct == 2)
			{
				uint32_t source = (uint32_t)RegFile[rt];
				uint32_t result;

				result = source >> shamt;
				RegFile[rd] = (int32_t)result;

				ProgramCounter += 4;
			}

			// shift right arithmetic instruction
			else if (funct == 3)
			{
				int32_t source = RegFile[rt];
				int32_t result;

				result = source >> shamt;
				RegFile[rd] = result;

				ProgramCounter += 4;
			}

			// shift left logical variable instruction
			else if (funct == 4)
			{
				uint32_t source = (uint32_t)RegFile[rt];
				uint32_t shift = (uint32_t)RegFile[rs];
				uint32_t result;

				result = source << shift;
				RegFile[rd] = (int32_t)result;

				ProgramCounter += 4;
			}

			// shift right logical variable instruction
			else if (funct == 6)
			{
				uint32_t source = (uint32_t)RegFile[rt];
				uint32_t shift = (uint32_t)RegFile[rs];
				uint32_t result;

				result = source >> shift;
				RegFile[rd] = (int32_t)result;

				ProgramCounter += 4;
			}

			// shift right arithmetic variable instruction
			else if (funct == 7)
			{
				int32_t source = RegFile[rt];
				int32_t shift = RegFile[rs];
				int32_t result;

				result = source >> shift;
				RegFile[rd] = result;

				ProgramCounter += 4;
			}

			// move from hi instruction
			else if (funct == 16)
			{
				RegFile[rd] = RegFile[32];
				ProgramCounter += 4;
			}

			// move to hi instruction
			else if (funct == 17)
			{
				RegFile[32] = RegFile[rs];
				ProgramCounter += 4;
			}

			// move from lo instruction
			else if (funct == 18)
			{
				RegFile[rd] = RegFile[33];
				ProgramCounter += 4;
			}

			// move to lo instruction
			else if (funct == 19)
			{
				RegFile[33] = RegFile[rs];
				ProgramCounter += 4;
			}

			// multiply instruction
			else if (funct == 24)
			{
				int32_t source1 = RegFile[rs];
				int32_t source2 = RegFile[rt];
				int64_t result;

				result = source1 * source2;

				int64_t HI = (result & 0xffffffff00000000) >> 32;
				int64_t LO = result & 0x00000000ffffffff;

				RegFile[32] = (int32_t)HI;
				RegFile[33] = (int32_t)LO;

				ProgramCounter += 4;
			}

			// multiply unsigned instruction
			else if (funct == 25)
			{
				uint32_t source1 = (uint32_t)RegFile[rs];
				uint32_t source2 = (uint32_t)RegFile[rt];
				uint64_t result;

				result = source1 * source2;

				uint32_t HI = (uint32_t)((result & 0xffffffff00000000) >> 32);
				uint32_t LO = (uint32_t)(result & 0x00000000ffffffff);

				RegFile[32] = (int32_t)HI;
				RegFile[33] = (int32_t)LO;

				ProgramCounter += 4;
			}

			// divide instruction
			else if (funct == 26)
			{
				int32_t source1 = RegFile[rs];
				int32_t source2 = RegFile[rt];

				RegFile[33] = source1 / source2;
				RegFile[32] = source1 % source2;

				ProgramCounter += 4;
			}

			// divide unsigned instruction
			else if (funct == 27)
			{
				uint32_t source1 = (uint32_t)RegFile[rs];
				uint32_t source2 = (uint32_t)RegFile[rt];

				RegFile[33] = (int32_t) (source1/source2);
				RegFile[32] = (int32_t) (source1%source2);

				ProgramCounter += 4;
			}

			// jump register instruction
			else if (funct == 8)
			{
				uint32_t source = (uint32_t)RegFile[rs];
				ProgramCounter = source;
			}

			// jump and link register instruction
			else if (funct == 9)
			{
				RegFile[rd] = ProgramCounter + 4;
				ProgramCounter = RegFile[rs];
			}

			// system call instruction
			else if (funct == 12)
			{
				uint32_t SID = RegFile[2];
				SyscallExe(SID);

				ProgramCounter += 4;
			}

			// default to error message if instruction not interpreted correctly
			else
			{
				printf("there was an error with R-type instruction\n");
			}

		}

		else if (type == 'i')
		{
			opcode = (CurrentInstruction & 0xFC000000) >> 26;
			rs = (CurrentInstruction & 0x03e00000) >> 21;
			rt = (CurrentInstruction & 0x001f0000) >> 16;

			// add immediate instruction
			if (opcode == 8)
			{
				int32_t source = RegFile[rs];
				int16_t temp = (int16_t)(CurrentInstruction & 0x0000ffff);
				int32_t immediate = (int32_t)temp;

				RegFile[rt] = source + immediate;

				ProgramCounter += 4;
			}

			// add immediate unsigned instruction
			else if (opcode == 9)
			{
				uint32_t source = (uint32_t)RegFile[rs];
				uint32_t immediate = CurrentInstruction & 0x0000ffff;

				RegFile[rt] = (int32_t)(source + immediate);

				ProgramCounter += 4;
			}

			// set on less than immediate instruction
			else if (opcode == 10)
			{
				int32_t source = RegFile[rs];
				int16_t temp = (int16_t)(CurrentInstruction & 0x0000ffff);
				int32_t immediate = (int32_t)temp;

				if (source < immediate)
				{
					RegFile[rt] = 1;
				}

				else
				{
					RegFile[rt] = 0;
				}

				ProgramCounter += 4;
			}

			// set on less than immediate unsigned instruction
			else if (opcode == 11)
			{
				uint32_t source = (uint32_t)RegFile[rs];
				uint32_t immediate = CurrentInstruction & 0x0000ffff;

				if (source < immediate)
				{
					RegFile[rt] = 1;
				}

				else
				{
					RegFile[rt] = 0;
				}

				ProgramCounter += 4;
			}

			// and immediate instruction
			else if (opcode == 12)
			{
				uint32_t source = (uint32_t)RegFile[rs];
				uint32_t immediate = CurrentInstruction & 0x0000ffff;

				RegFile[rt] = (int32_t)(source & immediate);

				ProgramCounter += 4;
			}

			// or immediate instruction
			else if (opcode == 13)
			{
				uint32_t source = (uint32_t)RegFile[rs];
				uint32_t immediate = CurrentInstruction & 0x0000ffff;

				RegFile[rt] = (int32_t)(source | immediate);

				ProgramCounter += 4;
			}

			// exclusive or immediate instruction
			else if (opcode == 14)
			{
				uint32_t source = (uint32_t)RegFile[rs];
				uint32_t immediate = CurrentInstruction & 0x0000ffff;

				RegFile[rt] = (int32_t)(source ^ immediate);

				ProgramCounter += 4;
			}

			// load upper immediate instruction
			else if (opcode == 15)
			{
				uint32_t immediate = CurrentInstruction & 0x0000ffff;

				RegFile[rt] = (int32_t)(immediate << 16);

				ProgramCounter += 4;
			}

			// branch on less/greater than instructions. There are multiple, so must further decode
			else if (opcode == 1)
			{
				int32_t source = RegFile[rs];
				int16_t temp = (int16_t)(CurrentInstruction & 0x0000ffff);
				int32_t offset = (int32_t)temp;

				switch(rt)
				{
					// branch on less than zero instruction
					case 0:
						if (source < 0)
						{
							offset = offset << 2;
							ProgramCounter = ProgramCounter + 4 + offset;
						}

						else
						{
							ProgramCounter += 4;
						}

						break;

					// branch on greater than or equal to zero instruction
					case 1:
						if (source >= 0)
						{
							offset = offset << 2;
							ProgramCounter = ProgramCounter + 4 + offset;
						}

						else
						{
							ProgramCounter += 4;
						}

						break;

					// branch on less than zero and link instruction
					case 16:
						if (source < 0)
						{
							RegFile[31] = ProgramCounter + 4;
							offset = offset << 2;
							ProgramCounter = ProgramCounter + 4 + offset;
						}

						else
						{
							ProgramCounter += 4;
						}

						break;

					// branch on greater than or equal to zero and link instruction
					case 17:
						if (source >= 0)
						{
							RegFile[31] = ProgramCounter + 4;
							offset = offset << 2;
							ProgramCounter = ProgramCounter + 4 + offset;
						}

						else
						{
							ProgramCounter += 4;
						}

						break;
				}
			}

			// branch on equal instruction
			else if (opcode == 4)
			{
				int32_t source1 = RegFile[rs];
				int32_t source2 = RegFile[rt];
				int16_t temp = (int16_t)(CurrentInstruction & 0x0000ffff);
				int32_t offset = (int32_t)temp;

				if (source1 == source2)
				{
					offset = offset << 2;
					ProgramCounter = ProgramCounter + 4 + offset;
				}

				else
				{
					ProgramCounter += 4;
				}
			}

			// branch on not equal instruction
			else if (opcode == 5)
			{
				int32_t source1 = RegFile[rs];
				int32_t source2 = RegFile[rt];
				int16_t temp = (int16_t)(CurrentInstruction & 0x0000ffff);
				int32_t offset = (int32_t)temp;

				if (source1 != source2)
				{
					offset = offset << 2;
					ProgramCounter = ProgramCounter + 4 + offset;
				}

				else
				{
					ProgramCounter += 4;
				}
			}

			// branch on less than or equal to zero instruction
			else if (opcode == 6)
			{
				int32_t source = RegFile[rs];
				int16_t temp = (int16_t)(CurrentInstruction & 0x0000ffff);
				int32_t offset = (int32_t)temp;

				if (source <= 0)
				{
					offset = offset << 2;
					ProgramCounter = ProgramCounter + 4 + offset;
				}

				else
				{
					ProgramCounter += 4;
				}
			}

			// branch on greater than zero instruction
			else if (opcode == 7)
			{
				int32_t source = RegFile[rs];
				int16_t temp = (int16_t)(CurrentInstruction & 0x0000ffff);
				int32_t offset = (int32_t)temp;

				if (source > 0)
				{
					offset = offset << 2;
					ProgramCounter = ProgramCounter + 4 + offset;
				}

				else
				{
					ProgramCounter += 4;
				}
			}

			// load byte instruction
			else if (opcode == 32)
			{
				uint32_t base_address = (uint32_t)RegFile[rs];
				int16_t offset = (int16_t)(CurrentInstruction & 0x0000ffff);

				uint32_t mem_addr = base_address + (int32_t)offset;
				int8_t byte = (int8_t)readByte(mem_addr, false);
				
				RegFile[rt] = (int32_t) byte;

				ProgramCounter += 4;
			}

			// load halfword instruction
			else if (opcode == 33)
			{
				uint32_t base_address = (uint32_t)RegFile[rs];
				int16_t offset = (int16_t)(CurrentInstruction & 0x0000ffff);

				uint32_t mem_addr = base_address + (int32_t)offset;
				uint32_t word = readWord(mem_addr, false);

				uint32_t sign_bit = (word & 0x0000ffff) >> 15;

				if (sign_bit == 1)
				{
					RegFile[rt] = (int32_t)(word | 0xffff0000);
				}

				else if (sign_bit == 0)
				{
					RegFile[rt] = (int32_t)(word & 0x0000ffff);
				}

				else
				{
					printf("There was an error loading halfword\n");
				}

				ProgramCounter += 4;
			}

			// load word left instruction
			else if (opcode == 34)
			{
				printf("idk man\n");
				ProgramCounter += 4;
			}

			// load word instruction
			else if (opcode == 35)
			{
				uint32_t base_address = (uint32_t)RegFile[rs];
				int16_t offset = (int16_t)(CurrentInstruction & 0x0000ffff);

				uint32_t mem_addr = base_address + (int32_t)offset;
				RegFile[rt] = (int32_t) readWord(mem_addr, false);

				ProgramCounter += 4;
			}

			// load byte unsigned instruction
			else if (opcode == 36)
			{
				uint32_t base_address = (uint32_t)RegFile[rs];
				int16_t offset = (int16_t)(CurrentInstruction & 0x0000ffff);

				uint32_t mem_addr = base_address + (int32_t)offset;
				uint32_t result = (uint32_t)readByte(mem_addr, false);

				RegFile[rt] = (int32_t)result;

				ProgramCounter += 4;
			}

			// load halfword unsigned instruction
			else if (opcode == 37)
			{
				uint32_t base_address = (uint32_t)RegFile[rs];
				int16_t offset = (int16_t)(CurrentInstruction & 0x0000ffff);

				uint32_t mem_addr = base_address + (int32_t)offset;

				uint32_t word = readWord(mem_addr, false);
				word = word & 0x0000ffff;

				RegFile[rt] = (int32_t) word;

				ProgramCounter += 4;
			}

			// load word right instruction
			else if (opcode == 38)
			{
				printf("idk man\n");
				ProgramCounter += 4;
			}

			// store byte instruction
			else if (opcode == 40)
			{
				uint32_t base_address = (uint32_t)RegFile[rs];
				uint8_t store_data = (uint8_t)RegFile[rt];
				int16_t offset = (int16_t)(CurrentInstruction & 0x0000ffff);

				uint32_t mem_addr = base_address + (int32_t)offset;
				writeByte(mem_addr, store_data, false);

				ProgramCounter += 4;
			}

			// store halfword instruction
			else if (opcode == 41)
			{
				printf("idk man\n");
				ProgramCounter += 4;
			}

			// store word left instruction
			else if (opcode == 42)
			{
				printf("idk man\n");
				ProgramCounter += 4;
			}

			// store word instruction
			else if (opcode == 43)
			{
				uint32_t base_address = (uint32_t)RegFile[rs];
				uint32_t store_data = (uint32_t)RegFile[rt];
				int16_t offset = (int16_t)(CurrentInstruction & 0x0000ffff);

				uint32_t mem_addr = base_address + (int32_t)offset;
				writeByte(mem_addr, store_data, false);

				ProgramCounter += 4;
			}

			// store word right instruction
			else if (opcode == 46)
			{
				printf("idk man\n");
				ProgramCounter += 4;
			}

			else
			{
				printf("There was an error with I-type instruction\n");
			}

		}

		else
		{
			jump_index = (CurrentInstruction & 0x03ffffff);

			// jump instruction
			if (opcode == 2)
			{
				jump_index = jump_index << 2;
				ProgramCounter = jump_index;
			}

			// jump and link instruction
			else if (opcode == 3)
			{
				RegFile[31] = ProgramCounter + 4;
				jump_index = jump_index << 2;
				ProgramCounter = jump_index;
			}

			else
			{
				printf("there was an error with J-type instruction\n");
			}
		}

	}   

	//Close file pointers & free allocated Memory
	closeFDT();
	CleanUp();

	return 0;

}
