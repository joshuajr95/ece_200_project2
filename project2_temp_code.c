#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>


/* This function checks the type of the current instruction being processed
and returns 'r' if r-type, 'i' if i-type, and 'j' if j-type. */
char getInstructionType(uint32_t CurrentInstruction) {

	uint8_t opcode = (CurrentInstruction & 0xFC000000) >> 26;
	char type;

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

	return type;
}

/* This function checks whether an instruction is an instruction with unsigned
operands and returns true if so and false otherwise */
bool isUnsigned(uint32_t CurrentInstruction) {

	uint8_t opcode = (CurrentInstruction & 0xFC000000) >> 26;
	bool isUnsigned;

	if (opcode == 0)
	{
		uint8_t funct = (CurrentInstruction & 0x0000003F);

		if (funct == 33 || funct == 35 || funct == 43 || funct == 25 || funct == 27)
			isUnsigned = true;

		else
			isUnsigned = false;
	}

	else if (opcode == 9 || opcode == 11 || opcode == 36 || opcode == 37)
		isUnsigned = true;

	else
		isUnsigned = false;

	return isUnsigned;
}


int main(int argc, char const *argv[])
{

	char type;
	type = getInstructionType(CurrentInstruction);


	// decode and execute R-type instructions
	if (type == 'r')
	{
		uint8_t funct = (CurrentInstruction & 0x0000003F);
		uint8_t rs;
		uint8_t rt;
		uint8_t rd;
		uint8_t shamt;
		uint32_t syscall_code;


		if (funct == 12) {
			syscall_code = (CurrentInstruction & 0x03ffffc0) >> 6;
		}

		else {
			rs = (CurrentInstruction & 0x03e00000) >> 21;
			rt = (CurrentInstruction & 0x001f0000) >> 16;
			rd = (CurrentInstruction & 0x0000f800) >> 11;
			shamt = (CurrentInstruction & 0x000007c0) >> 6;
		}


		switch(funct) {

			// add instruction implementation
			case 32:
				int32_t source1 = RegFile[rs];
				int32_t source2 = RegFile[rt];
				int32_t result;

				result = source1 + source2;
				RegFile[rd] = result;
				break;

			// add unsigned instruction implementation
			case 33:
				uint32_t source1 = (uint32_t)RegFile[rs];
				uint32_t source2 = (uint32_t)RegFile[rt];
				uint32_t result;

				result = source1 + source2;
				RegFile[rd] = (int32_t)result;
				break;

			// subtract instruction implementation
			case 34:
				int32_t source1 = RegFile[rs];
				int32_t source2 = RegFile[rt];
				int32_t result;

				result = source1 - source2;
				RegFile[rd] = result;
				break;

			// subtract unsigned instruction implementation
			case 35:
				uint32_t source1 = (uint32_t)RegFile[rs];
				uint32_t source2 = (uint32_t)RegFile[rt];
				uint32_t result;

				result = source1 - source2;
				RegFile[rd] = (int32_t)result;
				break;

			// bitwise AND instruction implementation
			case 36:
				uint32_t source1 = (uint32_t)RegFile[rs];
				uint32_t source2 = (uint32_t)RegFile[rt];
				uint32_t result;

				result = source1 & source2;
				RegFile = (int32_t)result;
				break;

			// bitwise OR instruction
			case 37:
				uint32_t source1 = (uint32_t)RegFile[rs];
				uint32_t source2 = (uint32_t)RegFile[rt];
				uint32_t result;

				result = source1 | source2;
				RegFile = (int32_t)result;
				break;

			// bitwise XOR instruction
			case 38:
				uint32_t source1 = (uint32_t)RegFile[rs];
				uint32_t source2 = (uint32_t)RegFile[rt];
				uint32_t result;

				result = source1 ^ source2;
				RegFile[rd] = (int32_t)result;
				break;

			// bitwise NOR instruction
			case 39:
				uint32_t source1 = (uint32_t)RegFile[rs];
				uint32_t source2 = (uint32_t)RegFile[rt];
				uint32_t result;

				result = ~(source1 | source2);
				RegFile[rd] = (int32_t)result;
				break;

			// set on less than instruction
			case 42:
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
				break;

			// set on less than unsigned instruction
			case 43:
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
				break;

			// shift left logical instruction
			case 0:
				uint32_t source = (uint32_t)RegFile[rt];
				uint32_t result;

				result = source << shamt;
				RegFile[rd] = (int32_t)result;
				break;

			// shift right logical instruction
			case 2:
				uint32_t source = (uint32_t)RegFile[rt];
				uint32_t result;

				result = source >> shamt;
				RegFile[rd] = (int32_t)result;
				break;

			// shift right arithmetic instruction
			case 3:
				int32_t source = RegFile[rt];
				int32_t result;

				result = source >> shamt;
				RegFile[rd] = result;
				break;

			// shift left logical variable instruction
			case 4:
				uint32_t source = RegFile[rt];
				uint32_t shift = RegFile[rs];
				uint32_t result;

				result = source << shift;
				RegFile[rd] = (int32_t)result;
				break;

			// shift right logical variable instruction
			case 6:
				uint32_t source = RegFile[rt];
				uint32_t shift = RegFile[rs];
				uint32_t result;

				result = source >> shift;
				RegFile[rd] = (int32_t)result;
				break;

			// shift right arithmetic variable instruction
			case 7:
				int32_t source = RegFile[rt];
				int32_t shift = RegFile[rs];
				int32_t result;

				result = source >> shift;
				RegFile[rd] = result;
				break;

			// move from hi instruction
			case 16:
				RegFile[rd] = RegFile[32];
				break;

			// move to hi instruction
			case 17:
				RegFile[32] = RegFile[rs];
				break;

			// move from lo instruction
			case 18:
				RegFile[rd] = RegFile[33];
				break;

			// move to lo instruction
			case 19:
				RegFile[33] = RegFile[rs];
				break;

			// multiply instruction
			case 24:
				int32_t source1 = RegFile[rs];
				int32_t source2 = RegFile[rt];
				int64_t result;

				result = source1 * source2;

				int64_t HI = (result & 0xffffffff00000000) >> 32;
				int64_t LO = result & 0xffffffff;

				RegFile[32] = (int32_t)HI;
				RegFile[33] = (int32_t)LO;
				break;

			// multiply unsigned instruction
			case 25:
				uint32_t source1 = (uint32_t)RegFile[rs];
				uint32_t source2 = (uint32_t)RegFile[rt];
				uint64_t result;

				result = source1 * source2;

				uint64_t HI = (result & 0xffffffff00000000) >> 32;
				uint64_t LO = result & 0xffffffff;



				RegFile[32] = (int32_t)HI;
				RegFile[33] = (int32_t)LO;
				break;

			// divide instruction
			case 26:
				int32_t source1 = RegFile[rs];
				int32_t source2 = RegFile[rt];

				RegFile[33] = source1 / source2;
				RegFile[32] = source1 % source2;
				break;

			// divide unsigned instruction
			case 27:
				uint32_t source1 = (uint32_t)RegFile[rs];
				uint32_t source2 = (uint32_t)RegFile[rt];

				RegFile[33] = (int32_t) (source1/source2);
				RegFile[32] = (int32_t) (source1%source2);
				break;

			// jump register instruction
			case 8:
				uint32_t source = (uint32_t)RegFile[rs];
				ProgramCounter = source;
				break;

			// jump and link register instruction
			case 9:
				RegFile[rd] = ProgramCounter + 4;
				ProgramCounter = RegFile[rs];
				break;

			// system call instruction
			case 12:
				/* INSERT IMPLEMENTATION HERE */
				break;
		}

		ProgramCounter += 4;

	}

	// decode and execute I-type instructions
	else if (type == 'i')
	{
		uint8_t opcode = (CurrentInstruction & 0xFC000000) >> 26;
		uint8_t rs = (CurrentInstruction & 0x03e00000) >> 21;
		uint8_t rt = (CurrentInstruction & 0x001f0000) >> 16;


		switch(opcode)
		{
			// add immediate instruction
			case 8:
				int32_t source = RegFile[rs];
				int32_t immediate = (CurrentInstruction & 0x0000ffff);
				int8_t sign = immediate >> 15;

				if (sign == 1)
				{
					immediate = immediate | 0xffff0000;
				}
				

				RegFile[rt] = source + immediate;
				break;

			// add immediate unsigned instruction
			case 9:
				uint32_t source = (uint32_t)RegFile[rs];
				uint32_t immediate = CurrentInstruction & 0x0000ffff;

				RegFile[rt] = (int32_t)(source + immediate);
				break;

			//set on less than immediate instruction
			case 10:
				int32_t source = RegFile[rs];
				int32_t immediate = CurrentInstruction & 0x0000ffff;
				int8_t sign = immediate >> 15;

				if (sign == 1)
				{
					immediate = immediate | 0xffff0000;
				}

				if (source < immediate)
				{
					RegFile[rt] = 1;
				}

				else
				{
					RegFile[rt] = 0;
				}

				break;

			// set on less than immediate unsigned instruction
			case 11:
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

				break;

			// and immediate instruction
			case 12:
				uint32_t source = (uint32_t)RegFile[rs];
				uint32_t immediate = CurrentInstruction & 0x0000ffff;

				RegFile[rt] = source & immediate;
				break;


			// or immediate instruction
			case 13:
				uint32_t source = (uint32_t)RegFile[rs];
				uint32_t immediate = CurrentInstruction & 0x0000ffff;

				RegFile[rt] = source | immediate;
				break;


			// exclusive or immediate instruction
			case 14:
				uint32_t source = (uint32_t)RegFile[rs];
				uint32_t immediate = CurrentInstruction & 0x0000ffff;

				RegFile[rt] = source ^ immediate;
				break;


			// load upper immediate instruction
			case 15:
				uint32_t immediate = CurrentInstruction & 0x0000ffff;

				RegFile[rt] = immediate << 16;
				break;

			//

		}

	}

	// decode and execute J-type instructions
	else
	{

	}




	return 0;
}
