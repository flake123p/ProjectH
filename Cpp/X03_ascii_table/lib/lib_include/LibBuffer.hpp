

#ifndef _LIB_BUFFER_HPP_INCLUDED_

#include "stdio.h" //printf
#include "My_Basics.hpp"

// ============================== Library: Cross-Platform (Manager) ==============================
template<typename T1>
class LibBufferClass{
public:
	IN  T1 *buf;
	IN  u32 bufMaxLen; // 1 bassed. (Means start from 1, not 0)
	OUT u32 curUseLen; // 0 bassed. (Means start from 0)
	OUT u32 size_of_type;
	OUT u32 bufMaxLen_In_Byte;

	LibBufferClass(IN T1 *input_buffer_pointer, IN u32 buffer_max_length);
	~LibBufferClass(void){}; // Do nothing
	void Init(void);
	bool Push(IN T1 val);      // Return true for push success.
	bool Pop(OUT T1 *val);      // Return true for pop  success.
	void Dump(void);
};
template<typename T1>
LibBufferClass<T1>::LibBufferClass(IN T1 *input_buffer_pointer, IN u32 buffer_max_length)
{
	buf = input_buffer_pointer;
	bufMaxLen = buffer_max_length;

	size_of_type = sizeof(T1);
	bufMaxLen_In_Byte = size_of_type * bufMaxLen;

	Init();
}

template<typename T1>
void LibBufferClass<T1>::Init(void)
{
	buf[0] = 0;
	curUseLen = 0;
}

template<typename T1>
bool LibBufferClass<T1>::Push(IN T1 val)
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
bool LibBufferClass<T1>::Pop(OUT T1 *val)
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
void LibBufferClass<T1>::Dump(void)
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

// ============================== Library: Platform Dependant (Depend on Windows or Linux)==============================

#define _LIB_BUFFER_HPP_INCLUDED_
#endif//_LIB_BUFFER_HPP_INCLUDED_

