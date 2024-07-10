
#include "Everything_Lib_Mgr.hpp"

LibBufferBasic::LibBufferBasic(u32 buf_size /* = 0 */)
{
	if (buf_size != 0) {
		Init(buf_size);
	} else {
		bufSize = 0;
		bufPtr = NULL;
	}
}

void *LibBufferBasic::Init(u32 buf_size)
{
	bufSize = buf_size;
	bufPtr = malloc(buf_size);
	if (bufPtr==NULL) {
		BASIC_ASSERT(0);
	}

	return bufPtr;
}

LibBufferBasic::~LibBufferBasic(void)
{
	if (bufPtr!=NULL) {
		free(bufPtr);
		bufPtr = NULL;
	}
}

int LibBufferQueue::InitQueue(u32 buf_size)
{
	BASIC_ASSERT(buf_size != 0);
	
	Init(buf_size);
	ptr = (u8 *)bufPtr;
	len = 0;

	return 0;
}

int LibBufferQueue::Push(u8 *inputBuf, u32 inputLen)
{
	if (len + inputLen > bufSize) {
		LibError_SetExtErrorMessage("%s() line:%d, buf overflow. len=%d, inputLen=%d, bufSize=%d\n", __func__, __LINE__, len, inputLen, bufSize);
		return 1;
	}

	memcpy(ADDRX(ptr, len), inputBuf, inputLen);
	len += inputLen;
	
	return 0;
}

void LibBufferQueue::Dump(void)
{
	DUMPA(bufPtr);
	DUMPD(bufSize);
	DUMPA(ptr);
	DUMPD(len);
}

void LibBufPrinter::Init(IN char * start_ptr, IN u32 buf_size, IN u32 in_threshold)
{
	startPtr = start_ptr;
	currPtr = start_ptr;
	bufSize = buf_size;
	threshold = in_threshold; //Wrap around threshold
	lastProfile = NULL;
}

char *LibBufPrinter::GetAlignedAddr(char *addr)
{
	u32 pad_size = POINTER_TO_U32(addr) % 8;
	if (pad_size != 0) {
		pad_size = 8 - pad_size;
	}
	for (u32 i = 0; i < pad_size; i++) {
		*addr = 0xDD;
		addr++;
	}
	return addr;
}

int LibBufPrinter::Print(const char *format, ...)
{
	int totalPrintNum;
	PRINT_PROFILE_t *newProfile;
	char *stringStartPtr = currPtr;
	
	va_list vl;
	va_start(vl, format);
	totalPrintNum = vsprintf(stringStartPtr, format, vl);
	va_end(vl);

	currPtr += totalPrintNum;
	*currPtr = 0;
	currPtr++;
	currPtr = GetAlignedAddr(currPtr);
	
	newProfile = (PRINT_PROFILE_t *)currPtr;
	newProfile->prevProfile = lastProfile;
	newProfile->nextProfile = NULL;
	lastProfile = newProfile;
	if (newProfile->prevProfile != NULL) {
		newProfile->prevProfile->nextProfile = newProfile;
	}

	MakeLineProfile(stringStartPtr, totalPrintNum);
	
	currPtr = GetAlignedAddr(currPtr);
	u32 currSize = (u32)(currPtr - startPtr);
	if (currSize > bufSize - threshold) {
		//Wrap around
		currPtr = startPtr;
	}

	return totalPrintNum;
}

void LibBufPrinter::MakeLineProfile(char *str, int len)
{
	u32 strSegLen;
	PRINT_PROFILE_t *newProfile = lastProfile;
	
	newProfile->addr = str;
	newProfile->lineType = INVALIED_LINE_TYPE;
	
	strSegLen = 0;
	for (int i = 0; i < len; i++) {
		if (str[i] == '\n') {
			newProfile->lineType = A_LINE;
			newProfile->lineLen = strSegLen;

			strSegLen = 0;
			if (i != len - 1) {
				newProfile++;
				newProfile->prevProfile = lastProfile;
				lastProfile = newProfile;
				if (newProfile->prevProfile != NULL) {
					newProfile->prevProfile->nextProfile = newProfile;
				}
				
				newProfile->addr = &(str[i+1]);
				newProfile->lineType = INVALIED_LINE_TYPE;
			}
		} else {
			strSegLen++;
		}
	}
	if (newProfile->lineType == INVALIED_LINE_TYPE) {
		newProfile->lineType = A_SEGMENT;
		newProfile->lineLen = strSegLen;
	}

	currPtr = (char *)newProfile->next;
}

int LibBuffer_DumpMem(char *memory_address)
{
	char lineStr[17];
	u8 *mem = (u8 *)memory_address;

	mem = (u8 *)memory_address;
	for (int i = 0; i < 16; i++) {
		for (int j = 0; j < 16; j++) {
			if (LibString_IsCharPrintable(mem[j])) {
				lineStr[j] = mem[j];
			} else {
				if(mem[j]=='\n')
					lineStr[j] = '|';
				else
					lineStr[j] = '-';
			}
		}
		lineStr[16] = 0;
		
		printf(
			"[%p] %02X %02X %02X %02X %02X %02X %02X %02X - %02X %02X %02X %02X %02X %02X %02X %02X  **%s**\n", 
			mem,
			mem[0], mem[1], mem[2], mem[3], mem[4], mem[5], mem[6], mem[7], mem[8], mem[9], mem[10], mem[11], mem[12], mem[13], mem[14], mem[15],
			lineStr
			);

		mem+=16;
	}

	return 0;
}

void LibBuffer_DemoBasic(void)
{
	LibBufferBasic obj;//(102400);
	obj.Init(102400);
	
	LibBufPrinter printer;

	printer.Init((char *)obj.bufPtr, 102400, 1024);
	printer.Print("%s aab\nbcc %d, %f\nss", "STR", 12, 55.321);

	//printf((char *)obj.bufPtr);

	DUMPP(printer.currPtr);

	printer.Print("CCCCCCCC");
	DUMPP(printer.currPtr);

	LibBuffer_DumpMem(printer.startPtr);

	char str[50];
	PRINT_PROFILE_t *profile = printer.lastProfile;
	while (1) {
		memcpy(str, profile->addr, profile->lineLen);
		str[profile->lineLen] = '\n';
		str[profile->lineLen+ 1] = 0;
		printf("%s", str);

		if (profile->prevProfile == NULL)
			break;
		else
			profile = profile->prevProfile;
	}
	printf("------REVERSE------\n");
	while (1) {
		memcpy(str, profile->addr, profile->lineLen);
		str[profile->lineLen] = '\n';
		str[profile->lineLen+ 1] = 0;
		printf("%s", str);

		if (profile->nextProfile == NULL)
			break;
		else
			profile = profile->nextProfile;
	}
}
