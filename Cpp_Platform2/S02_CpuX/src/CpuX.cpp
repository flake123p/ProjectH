
#include "Everything_App.hpp"

/*
	23,22,21,20,  19,18,17,16,  15,14,13,12,  11,10,09,08,  07,06,05,04,  03,02,01,00
	[    OPCODE(6)    ] [                 ]   [             Operand(12)             ]
*/
#if 0
static OPCODE_TABLE_t gOpcodetable[] = {
	{"LDA", 0x01, 1},
	{"LDB", 0x02, 1},
};
#endif

LibFileIoClass gInputObjFile;

static volatile u8 *Memory;
static volatile u32 RegisterA = 0;
static volatile u32 RegisterB = 0;
static volatile u32 PC = 0;
static volatile u32 PC_Max = 0;
static volatile u32 MachineCode;
static volatile u32 OpCode = 0;
static volatile u32 Addressing = 0;
static volatile u32 Operand = 0;

typedef enum {
	CS_FETCH_OPCODE,

	CS_INVALID,
}CPU_STATE_t;
CPU_STATE_t CPU_State = CS_FETCH_OPCODE;
static int _CPU_X_Run_One_Instruction(void)
{
	MachineCode = SHIFT_OR_3(Memory[PC], Memory[PC+1], Memory[PC+2]);
	OpCode     = GET_BIT_FIELD_2(MachineCode, SIZE_6, OFFSET_18);
	Addressing = GET_BIT_FIELD_2(MachineCode, SIZE_6, OFFSET_12);
	Operand    = GET_BIT_FIELD_2(MachineCode, SIZE_12, OFFSET_0);

	switch (Addressing) {
		case 0: {
			Operand = Operand; // Immediate
		} break;

		default :
			BASIC_ASSERT(0);
			break;
	}

	switch (OpCode) {
		case 0x01: {
			RegisterA = Operand;
		} break;

		case 0x02: {
			RegisterB = Operand;
		} break;

		default:
			BASIC_ASSERT(0);
			break;
	}

	return 0;
}

static int _CPU_X_Run(void)
{


	DUMPD(PC_Max);
	while (PC < PC_Max) {
		_CPU_X_Run_One_Instruction();
		PC+=3;
	}

	return 0;
}

static int _CPU_X_FileToMemory(void)
{
	int retVal;
	u32 memOffset = 0;
	u32 val;
	u32 tempVal;
	u32 nibbleCtr;

	nibbleCtr = 0;
	while (1) {

		retVal = gInputObjFile.GetCharacter();

		if (retVal == EOF) {
			break;
		} else if (retVal == '\r' || retVal == '\n') {
			;//do nogthing
		} else if (LibString_IsCharHexadecimal(retVal, &tempVal)) {
			if (nibbleCtr == 0) {
				nibbleCtr++;
				val = tempVal << 4;
			} else {
				nibbleCtr = 0;
				val |= tempVal;
				Memory[memOffset] = (u8)val;
				memOffset++;
			}
		} else {
			BASIC_ASSERT(0);
		}
	}
	PC_Max = memOffset;
	return 0;
}

void CPU_X_PrintUsage(void)
{
	printf("Usage:\n");
	printf("  cpu.exe [input file]\n\n");
}

int CPU_X_Main(int argc, char *argv[])
{
	if (argc != 1) {
		CPU_X_PrintUsage();
		return 1;
	}

	DUMPS(argv[0]);
	gInputObjFile.openMode = "r+b";
	gInputObjFile.fileName = argv[0];

	RETURN_CHK(rc, gInputObjFile.FileOpenForRead());

	LibBufferBasic bufObj;
	bufObj.Init(0x2000);
	Memory = (u8 *)bufObj.bufPtr;
	RETURN_CHK(rc, _CPU_X_FileToMemory());

	RETURN_CHK(rc, _CPU_X_Run());

	return 0;
}

