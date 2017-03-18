
#include "Everything_Lib_Mgr.hpp"

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

Lib51HexReader::Lib51HexReader(void)
{
	//Dump();
	GlobalAddress = 0;
	workingRecord.dataBuf = (u8 *)malloc(500);
	workingReadOutString = (u8 *)malloc(1000);
}

Lib51HexReader::~Lib51HexReader(void)
{
	free(workingRecord.dataBuf);
	free(workingReadOutString);
	RemoveAll(DO_AUTO_FREE);
}

int Lib51HexReader::ReadFile(const char *fileName)
{
	LibFileIoClass inFile(fileName, "r");

	inFile.FileOpen();

	if (inFile.fp == NULL) {
		printf("FILE NOT EXIST\n");
		return 1;
	}

	int retVal;
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
			AddData();
		}
	}
	//DUMPD(sizeof(HexBinDataNode_t));
	return 0;
}

int Lib51HexReader::AddData(void)
{
	u32 addr = GlobalAddress+(u32)workingRecord.addr16btis;
	u32 dataLen = workingRecord.dataLen;
	BASIC_ASSERT(dataLen>0);

	u32 alignedAddr0 = (addr / NODE_DATA_SIZE) * NODE_DATA_SIZE;
	u32 alignedAddr1 = alignedAddr0 + NODE_DATA_SIZE;
	bool overBoundary = false;
	u32 writeAddr0;
	u32 writeAddr1;
	u32 writeLen0;
	u32 writeLen1;
	HexBinDataNode_t *newHexBinDataNode0;
	HexBinDataNode_t *newHexBinDataNode1;
	u32 recordOffset0;
	u32 recordOffset1;
	
	if (addr+dataLen > alignedAddr1) {
		overBoundary = true;
		writeAddr0 = addr;
		writeLen0  = alignedAddr1 - writeAddr0;
		writeAddr1 = alignedAddr1;
		writeLen1  = dataLen - writeLen0;
		recordOffset0 = 0;
		recordOffset1 = writeLen0;
	} else {
		overBoundary = false;
		writeAddr0 = addr;
		writeLen0 = dataLen;
		recordOffset0 = 0;
	}
	
	if (info.count == 0) {
		NewNode(alignedAddr0, &newHexBinDataNode0);
		if (overBoundary) {
			NewNode(alignedAddr1, &newHexBinDataNode1);
		}
	} else {
		if (NOT(IsAddressInAnyNode(alignedAddr0, &newHexBinDataNode0))) {
			NewNode(alignedAddr0, &newHexBinDataNode0);
		}

		if (overBoundary) {
			if (NOT(IsAddressInAnyNode(alignedAddr1, &newHexBinDataNode1))) {
				NewNode(alignedAddr1, &newHexBinDataNode1);
			}
		}
	}

	CopyRecordToNode(writeAddr0, recordOffset0, writeLen0, newHexBinDataNode0);
	if (overBoundary) {
		CopyRecordToNode(writeAddr1, recordOffset1, writeLen1, newHexBinDataNode1);
	}
	
	return 0;
#if 0
	HexBinDataNode_t *currHexBinNode;
	HexBinDataNode_t *afterCurrHexBinNode;
	for (LinkedListNode *currNode = info.head; currNode != NULL; currNode = currNode->next) {
		currHexBinNode = (HexBinDataNode_t *)currNode;
		if (addr >= currHexBinNode->startAddr && addr < currHexBinNode->startAddr+NODE_DATA_SIZE) {
			if ((addr+dataLen-1) < currHexBinNode->startAddr+NODE_DATA_SIZE) {
				return true;
			} else {
				// Check next node can fill rest data
				if (currNode->next == NULL) {
					return false;
				} else {
					afterCurrHexBinNode = (HexBinDataNode_t *)currNode->next;
					if (afterCurrHexBinNode->startAddr != currHexBinNode->startAddr+NODE_DATA_SIZE) {
						return false;
					} else {
						return true;
					}
				}
			}
		}
	}

	return false;
#endif
}

bool Lib51HexReader::IsAddressInAnyNode(u32 addr, OUT HexBinDataNode_t **matchNode /* = NULL */)
{
	if (info.count == 0) {
		return false;
	}

	HexBinDataNode_t *currHexBinNode;
	for (LinkedListNode *currNode = info.head; currNode != NULL; currNode = currNode->next) {
		currHexBinNode = (HexBinDataNode_t *)currNode;
		if (currHexBinNode->startAddr == addr) {
			if (matchNode != NULL)
				*matchNode = currHexBinNode;
			return true;
		}
	}

	return false;
}

int Lib51HexReader::NewNode(u32 addr, OUT HexBinDataNode_t **newHexBinDataNode /* = NULL */)
{
	HexBinDataNode_t *newHexBinNode = (HexBinDataNode_t *)malloc(sizeof(HexBinDataNode_t)+NODE_DATA_SIZE+32);
	BASIC_ASSERT(newHexBinNode != NULL);
	if (newHexBinDataNode != NULL) {
		*newHexBinDataNode = newHexBinNode;
	}

	newHexBinNode->usedLen = 0;
	newHexBinNode->startAddr = addr;
	
	HexBinDataNode_t *currHexBinNode;
	for (LinkedListNode *currNode = info.head; currNode != NULL; currNode = currNode->next) {
		currHexBinNode = (HexBinDataNode_t *)currNode;
		BASIC_ASSERT(newHexBinNode->startAddr != currHexBinNode->startAddr);
		if (newHexBinNode->startAddr < currHexBinNode->startAddr) {
			InsertFront(currNode, (LinkedListNode *)newHexBinNode);
			return 0;
		}
	}

	PushBack((LinkedListNode *)newHexBinNode);
	return 0;
}

int Lib51HexReader::CopyRecordToNode(u32 dst_addr, u32 src_record_addr, u32 len, HexBinDataNode_t *hexBinDataNode)
{
	BASIC_ASSERT(IsThisNodeInList((LinkedListNode *)hexBinDataNode) != NODE_ISNT_IN_LIST);
	BASIC_ASSERT(((dst_addr/NODE_DATA_SIZE)*NODE_DATA_SIZE) == hexBinDataNode->startAddr);

	u32 nodeAddr = dst_addr - hexBinDataNode->startAddr;
	u8 *src = &(workingRecord.dataBuf[src_record_addr]);

	memcpy(&(hexBinDataNode->data[nodeAddr]), src, len);

	if ((len+nodeAddr) > hexBinDataNode->usedLen) {
		hexBinDataNode->usedLen = (len+nodeAddr);
	}

	return 0;
}

void Lib51HexReader::DumpAll51HexNode(void)
{
	printf("==================================== %s() start\n", __func__);
	DUMPD(info.count);
	DUMPX((u32)info.head);
	DUMPX((u32)info.tail);

	HexBinDataNode_t *currHexBinNode;
	LinkedListNode *currNode = info.head;
	for (u32 i = 0; i < info.count; i++) {
		currHexBinNode = (HexBinDataNode_t *)currNode;
		printf("Node(%d):\n", i+1);
		printf("[ 0x%X ]\n", (u32)currNode);
		DUMPX((u32)currNode->prev);
		DUMPX((u32)currNode->next);
		DUMPX(currHexBinNode->startAddr);
		DUMPX(currHexBinNode->usedLen);
		currNode = currNode->next;
	}
	BASIC_ASSERT(currNode == NULL);
	printf("==================================== %s() end\n", __func__);
}

int  Lib51HexReader::DumpAll51HexNode_ToFile(const char *fileName)
{
	LibFileIoClass outFile(fileName, "w+b");

	outFile.FileOpen();

	if (outFile.fp == NULL) {
		return 1;
	}

	fprintf(outFile.fp, "info.count = %d\n", info.count);
	fprintf(outFile.fp, "(u32)info.head = 0x%x\n", (u32)info.head);
	fprintf(outFile.fp, "(u32)info.tail = 0x%x\n", (u32)info.tail);

	HexBinDataNode_t *currHexBinNode;
	LinkedListNode *currNode = info.head;
	for (u32 i = 0; i < info.count; i++) {
		currHexBinNode = (HexBinDataNode_t *)currNode;
		fprintf(outFile.fp, "Node(%d):\n", i+1);
		fprintf(outFile.fp, "[ 0x%X ]\n", (u32)currNode);
		fprintf(outFile.fp, "(u32)currNode->prev = 0x%x\n", (u32)currNode->prev);
		fprintf(outFile.fp, "(u32)currNode->next = 0x%x\n", (u32)currNode->next);
		fprintf(outFile.fp, "currHexBinNode->startAddr = 0x%x\n", currHexBinNode->startAddr);
		fprintf(outFile.fp, "currHexBinNode->usedLen   = 0x%x\n", currHexBinNode->usedLen);

		for (u32 j = 0; j < currHexBinNode->usedLen; j++) {
			if (j%16==0) {
				fprintf(outFile.fp, "[%08X]  ", currHexBinNode->startAddr + j);
			}

			if (j%4==3) {
				fprintf(outFile.fp, "%02X, ", currHexBinNode->data[j]);
			} else {
				fprintf(outFile.fp, "%02X ", currHexBinNode->data[j]);
			}

			if (j%16==15 && j != currHexBinNode->usedLen) {
				fprintf(outFile.fp, "\n");
			}
		}
		fprintf(outFile.fp, "\n");
		currNode = currNode->next;
	}
	BASIC_ASSERT(currNode == NULL);

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
	
	//reader.DumpAll51HexNode();
	reader.DumpAll51HexNode_ToFile("TestHexDump.txt");

	LibTime_StopMicroSecondClock_ShowResult();
	PRINT_NEXT_LINE;
	
	return 0;
}
