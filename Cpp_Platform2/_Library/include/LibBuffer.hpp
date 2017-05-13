

#ifndef _LIB_BUFFER_HPP_INCLUDED_

#include "stdio.h" //printf
#include "stdarg.h"
#include "My_Basics.hpp"

// ============================== Debug ==============================

// ============================== Define ==============================

// ============================== Library: Cross-Platform (Manager) ==============================
template<typename T1>
class LibBufferLog{
public:
	IN  T1 *buf;
	IN  u32 bufMaxLen; // 1 bassed. (Means start from 1, not 0)
	OUT u32 curUseLen; // 0 bassed. (Means start from 0)
	OUT u32 size_of_type;
	OUT u32 bufMaxLen_In_Byte;

	LibBufferLog(IN T1 *input_buffer_pointer, IN u32 buffer_max_length);
	~LibBufferLog(void){}; // Do nothing
	void Init(void);
	bool Push(IN T1 val);      // Return true for push success.
	bool Pop(OUT T1 *val);      // Return true for pop  success.
	void Dump(void);
};

template<typename T1>
LibBufferLog<T1>::LibBufferLog(IN T1 *input_buffer_pointer, IN u32 buffer_max_length)
{
	buf = input_buffer_pointer;
	bufMaxLen = buffer_max_length;

	size_of_type = sizeof(T1);
	bufMaxLen_In_Byte = size_of_type * bufMaxLen;

	Init();
}

template<typename T1>
void LibBufferLog<T1>::Init(void)
{
	buf[0] = 0;
	curUseLen = 0;
}

template<typename T1>
bool LibBufferLog<T1>::Push(IN T1 val)
{
	if (curUseLen < bufMaxLen) {
		buf[curUseLen] = val;
		curUseLen++;
		return true;
	} else {
		return false;
	}
}

template<typename T1>
bool LibBufferLog<T1>::Pop(OUT T1 *val)
{
	if (curUseLen > 0) {
		*val = buf[curUseLen-1];
		curUseLen--;
		return true;
	} else {
		return false;
	}
}

template<typename T1>
void LibBufferLog<T1>::Dump(void)
{
	u32 i;
	printf("buf[] = ");
	for (i = 0; i < curUseLen; i++) {
		printf("0x%X ", buf[i]);
	}
	printf("\n");
	DUMPD(bufMaxLen);
	DUMPD(curUseLen);
	DUMPD(size_of_type);
	DUMPD(bufMaxLen_In_Byte);
}

class LibBufferBasic{
public:
	void *bufPtr;
	u32 bufSize;

	LibBufferBasic(u32 buf_size = 0);
	void *Init(u32 buf_size);
	~LibBufferBasic(void);
};

class LibBufferQueue : private LibBufferBasic {
public:
	u8 *ptr;
	u32 len;
	LibBufferQueue(void) : LibBufferBasic(0){};
	int InitQueue(u32 buf_size);
	int Push(u8 *inputBuf, u32 inputLen);
	void Dump(void);
};

typedef enum {
	A_LINE    = 0x454E494C,
	A_SEGMENT = 0x4D474553,
	
	INVALIED_LINE_TYPE,
} LINE_TYPE_t;
typedef struct PRINT_PROFILE_t{
	struct PRINT_PROFILE_t *prevProfile;
	struct PRINT_PROFILE_t *nextProfile;
	LINE_TYPE_t lineType;
	u32         lineLen;  //Not include '\n', very important
	char        *addr;
	u8          next[];
} PRINT_PROFILE_t;
class LibBufPrinter{
public:
	char *startPtr;
	char *currPtr;
	u32 bufSize;
	u32 threshold; //Wrap around threshold
	PRINT_PROFILE_t *lastProfile;

	LibBufPrinter(){}; // Do nothing
	~LibBufPrinter(void){}; // Do nothing
	void Init(char * start_ptr, u32 buf_size, u32 in_threshold);
	char *GetAlignedAddr(char *addr);
	int Print(const char *format, ...);
	void MakeLineProfile(char *str, int len);
};

int LibBuffer_DumpMem(char *memory_address);

void LibBuffer_DemoBasic(void);
// ============================== Library: Platform Dependant (Depend on Windows or Linux)==============================

#define _LIB_BUFFER_HPP_INCLUDED_
#endif//_LIB_BUFFER_HPP_INCLUDED_

