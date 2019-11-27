
#if 0
#include "Everything_Lib_Mgr.hpp"
#else
#include "Everything_Lib_Mgr_Lite.hpp"
#include "_LibString.hpp"
#include "_LibMemory.hpp"
#include "_Lib51Hex.hpp"
#include "LibTime.hpp"
#include "LibFileIO.hpp"
#include "LibFileIO2.hpp"
#endif

#define HEX_DATA_HEADER_SIZE (4)
#define HEX_DATA_SIZE        (255)
#define HEX_DATA_TAILER_SIZE (1) /* 8bits check sum*/
#define HEX_RECORD_SIZE (HEX_DATA_HEADER_SIZE+HEX_DATA_SIZE+HEX_DATA_TAILER_SIZE)
#define DATA_PTR(a) (&(a[HEX_DATA_HEADER_SIZE]))

u32 Lib51Hex_MakeDataRecord(u32 start_addr, char *hex_string, OUT u8 *hex_data)
{
	u32 dataLen = LibString_HexStringToCharString(DATA_PTR(hex_data), hex_string, HEX_DATA_SIZE);
	hex_data[0] = (u8)dataLen;
	hex_data[1] = (u8)(start_addr>>8);
	hex_data[2] = (u8)start_addr;
	hex_data[3] = 0;
	hex_data[dataLen+HEX_DATA_HEADER_SIZE] = LibString_GetCheckSumU8(hex_data, dataLen+HEX_DATA_HEADER_SIZE);
	
	return dataLen + HEX_DATA_HEADER_SIZE + HEX_DATA_TAILER_SIZE;
}

void Lib51Hex_SprintDataRecord(OUT char *buf, u8 *hex_data, u32 hex_data_len)
{
	u32 i=0;
	sprintf(buf, ":");
	for (i=0; i<hex_data_len; i++) {
		sprintf(&(buf[(i*2)+1]), "%02X", hex_data[i]);
	}
}

void Lib51Hex_PrintDataRecord(u8 *hex_data, u32 hex_data_len)
{
	printf(":");
	for (u32 i=0; i<hex_data_len; i++) {
		printf("%02X", hex_data[i]);
	}
	printf("\n");
}

int Lib51Hex_StringToRecord(const char *s, OUT HEX_RECORD_DATA_t *record)
{
	char tempStr[5];
	int tempInt;
	u32 strLength = strlen(s);
	u32 sum = 0;

	if (strLength < 11) {
		BASIC_ASSERT(0);
		return RECORD_LENGTH_TO_SMALL;
	}
	
	if (s[COLON_POS] != ':') {
		BASIC_ASSERT(0);
		return COLON_NOT_AT_START;
	}

	// Record Type
	memset(tempStr, 0, 5);
	tempStr[0] = s[RECORD_TYPE_0];
	tempStr[1] = s[RECORD_TYPE_1];
	sscanf(tempStr, "%x", &tempInt);
	if (tempInt < DATA_RECORD || tempInt > START_LIN_ADDR_RECORD || tempInt == 3) {
		BASIC_ASSERT(0);
		return ILLEGAL_RECORD_TYPE;
	}
	record->recordType = tempInt;
	sum += record->recordType;

	// Data Length
	memset(tempStr, 0, 5);
	tempStr[0] = s[LENGTH_0_OF_DATA];
	tempStr[1] = s[LENGTH_1_OF_DATA];
	sscanf(tempStr, "%x", &tempInt);
	if ((u32)((tempInt*2)+11) != strLength) {
		BASIC_ASSERT(0);
		return DATA_LENGTH_ERROR;
	}
	record->dataLen = tempInt;
	sum += record->dataLen;

	// Start address
	memset(tempStr, 0, 5);
	tempStr[0] = s[START_ADDRESS_0];
	tempStr[1] = s[START_ADDRESS_1];
	sscanf(tempStr, "%x", &tempInt);
	sum += tempInt;
	
	memset(tempStr, 0, 5);
	tempStr[0] = s[START_ADDRESS_2];
	tempStr[1] = s[START_ADDRESS_3];
	sscanf(tempStr, "%x", &tempInt);
	sum += tempInt;
	
	memset(tempStr, 0, 5);
	tempStr[0] = s[START_ADDRESS_0];
	tempStr[1] = s[START_ADDRESS_1];
	tempStr[2] = s[START_ADDRESS_2];
	tempStr[3] = s[START_ADDRESS_3];
	sscanf(tempStr, "%x", &tempInt);
	record->addr16btis = tempInt;

	int i = 0;
	memset(tempStr, 0, 5);
	for (i = 0; i < record->dataLen; i++) {
		tempStr[0] = s[(i*2)+DATA_FIELD];
		tempStr[1] = s[(i*2)+DATA_FIELD+1];
		sscanf(tempStr, "%x", &tempInt);
		record->dataBuf[i] = (u8)tempInt;
		sum += tempInt;
	}

	tempStr[0] = s[(i*2)+DATA_FIELD];
	tempStr[1] = s[(i*2)+DATA_FIELD+1];
	sscanf(tempStr, "%x", &tempInt);
	sum = (0x100 - sum) % 0x100;
	if (sum != (u32)tempInt) {
		BASIC_ASSERT(0);
		return CHECK_SUM_ERROR;
	}
	record->checkSum = sum;

	// Illegal Value Check
	switch (record->recordType) {
		case DATA_RECORD: {
		} break;
		
		case EOF_RECORD: {
			if (record->dataLen != 0) {
				BASIC_ASSERT(0);
				return RECORD_LENGTH_ERROR;
			}
		} break;
		
		case EXT_SEG_ADDR_RECORD: {
			if (record->dataLen != 2) {
				BASIC_ASSERT(0);
				return RECORD_LENGTH_ERROR;
			}
		} break;
		
		case EXT_LIN_ADDR_RECORD: {
			if (record->dataLen != 2) {
				BASIC_ASSERT(0);
				return RECORD_LENGTH_ERROR;
			}
		} break;
		
		case START_LIN_ADDR_RECORD: {
			if (record->dataLen != 4) {
				BASIC_ASSERT(0);
				return RECORD_LENGTH_ERROR;
			}
		} break;

		default:
			BASIC_ASSERT(0);
			break;
	}
	
	return 0;
}

void Lib51Hex_DumpRecord(HEX_RECORD_DATA_t *record)
{
	printf("==================================== %s() start\n", __func__);
	DUMPD(record->recordType);
	DUMPX(record->checkSum);
	DUMPX(record->addr16btis);
	DUMPX(record->dataLen);
	for (int i = 0; i < record->dataLen; i++) {
		printf("0x%02X  ", record->dataBuf[i]);
		if (i%16 == 15 && i != record->dataLen-1) {
			printf("\n");
		}
	}
	printf("\n");
	printf("==================================== %s() end\n", __func__);
}

void Lib51Hex_DumpHexFile(const char *fileName)
{
	LibFileIoClass inFile(fileName, "r");

	inFile.FileOpen();

	if (inFile.fp == NULL) {
		printf("FILE NOT EXIST\n");
		return;
	}

	HEX_RECORD_DATA_t record;
	record.dataBuf = (u8 *)malloc(500);
	u8 *tempString = (u8 *)malloc(1000);

	int retVal;
	while (1) {
		retVal = inFile.GetLine(tempString, 1000);
		DUMPD(retVal);
		if (retVal == EOF) {
			break;
		}
		if (retVal == 0) {
			continue;
		}
		Lib51Hex_StringToRecord((const char *)tempString, &record);
		Lib51Hex_DumpRecord(&record);
	}
	
	free(record.dataBuf);
	free(tempString);
}

Lib51HexReader::Lib51HexReader(u32 node_size /* = 0x8000 */, u32 init_val /* = 0x100 */)
{
	//Dump();
	GlobalAddress = 0;
	workingRecord.dataBuf = (u8 *)malloc(500);
	workingReadOutString = (u8 *)malloc(1000);
	virMem.SetParameters(node_size, init_val);
}

Lib51HexReader::~Lib51HexReader(void)
{
	free(workingRecord.dataBuf);
	free(workingReadOutString);
}

int Lib51HexReader::ReadFile(const char *fileName)
{
	int retVal;
	
	LibFileIoClass inFile(fileName, "r");

	retVal = inFile.FileOpen();
	RETURN_IF(retVal);
	
	while (1) {
		retVal = inFile.GetLine(workingReadOutString, 1000);
		//DUMPD(retVal);
		if (retVal == EOF) {
			break;
		}
		if (retVal == 0) {
			continue;
		}
		Lib51Hex_StringToRecord((const char *)workingReadOutString, &workingRecord);
		//Lib51Hex_DumpRecord(&workingRecord);
		if (workingRecord.recordType == EXT_LIN_ADDR_RECORD) {
			GlobalAddress = (u32)workingRecord.dataBuf[0];
			GlobalAddress = GlobalAddress << 8;
			GlobalAddress += (u32)workingRecord.dataBuf[1];
			GlobalAddress = GlobalAddress << 16;
			//DUMPX(GlobalAddress);
		} else if (workingRecord.recordType == EXT_SEG_ADDR_RECORD) {
			GlobalAddress = (u32)workingRecord.dataBuf[0];
			GlobalAddress = GlobalAddress << 8;
			GlobalAddress += (u32)workingRecord.dataBuf[1];
			GlobalAddress = GlobalAddress << 8;
			//DUMPX(GlobalAddress);
		}
		if (workingRecord.dataLen) {
			//DUMPD(IsAddressInAnyNode(((GlobalAddress+(u32)workingRecord.addr16btis)/NODE_DATA_SIZE)*NODE_DATA_SIZE));
			//AddData();
			virMem.Write((GlobalAddress+(u32)workingRecord.addr16btis), workingRecord.dataBuf, workingRecord.dataLen);
		}
	}
	//DUMPD(sizeof(HexBinDataNode_t));
	return 0;
}

char gHexString[] = "DA CB";
u8  gHexData[20];
u32 gHexDataLen;
char gHexString_2[] = "AA BB FF";
u8  gHexData_2[20];
u32 gHexDataLen_2;
char gOutputHexString[40];
int Lib51Hex_Demo(void)
{
	gHexDataLen = Lib51Hex_MakeDataRecord(0x03FF, gHexString, gHexData);
	Lib51Hex_SprintDataRecord((char *)gOutputHexString, gHexData, gHexDataLen);
	printf("%s\n", gOutputHexString);
	
	gHexDataLen_2 = Lib51Hex_MakeDataRecord(0x2233, gHexString_2, gHexData_2);
	Lib51Hex_SprintDataRecord((char *)gOutputHexString, gHexData_2, gHexDataLen_2);
	printf("%s\n", gOutputHexString);

	HEX_RECORD_DATA_t record;
	record.dataBuf = (u8 *)malloc(500);
	Lib51Hex_StringToRecord((const char *)gOutputHexString, &record);
	Lib51Hex_DumpRecord(&record);
	free(record.dataBuf);

	//Lib51Hex_DumpHexFile("C:\\Users\\a18109\\Documents\\BT5511\\Dallas_5506_FY17_c6594_merged_medium_task\\APP_SHS_OBJECT\\BT5511_SHS\\HexFrom200000x.hex");

	LibTime_StartMicroSecondClock();
	
	Lib51HexReader reader;
	//reader.ReadFile("C:\\Users\\a18109\\Documents\\BT5511\\Dallas_5506_FY17_c6594_merged_medium_task\\APP_SHS_OBJECT\\BT5511_SHS\\HexFrom200000x.hex");
	reader.ReadFile("C:\\Users\\a18109\\Documents\\BT5511\\Dallas_5506_FY17_c6594_merged_medium_task\\APP_SHS_OBJECT\\BT5511_SHS\\BT5511_SHS.hex");

	LibTime_StopMicroSecondClock_ShowResult();
	PRINT_NEXT_LINE;

	LibTime_StartMicroSecondClock();
	
	reader.virMem.DumpVirMemContent_ToFile("Lib51DEMO.txt");

	LibTime_StopMicroSecondClock_ShowResult();
	PRINT_NEXT_LINE;
	
	return 0;
}
