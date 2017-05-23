

#ifndef _LIB_SIMPLE_FILE_SYSTEM_HPP_INCLUDED_

// ============================== Include ==============================
#include "My_Types.h"

// ============================== Debug ==============================

// ============================== Define ==============================
typedef struct {
	u8  activeIndex; // 0xFF is invalid index for no active index
	u32 usedSize;
	u32 maxSize;
	u8  numOfMirrors;
	u32 mirrorAddr[1]; //dummy size
}SFS_FileDescriptor_t;

/*
typedef struct {
	u8  activeIndex; // 0xFF is invalid index for no active index
	u32 usedSize;
	u32 maxSize;
	u8  numOfMirrors;
	u32 mirrorAddr[];
}SFS_FileDescriptorEx_t; // struct for IC
*/

typedef struct {
	OUT u8 workingIndex;      //range is between 0 ~ (mirrorCount-1)
	OUT u8 workingFirstByte;  //range is between 0 ~ 0xFE (0xFF for empty mirror)
	OUT u8 nextIndex;         //range is between 0 ~ (mirrorCount-1)
	OUT u8 nextFirstByte;     //range is between 0 ~ 0xFE (0xFF for empty mirror)
}SFS_MirrorIndex_Info_t;

// ============================== Manager ==============================
int LibSimpleFS_GetMirrorInfo_FromFirstByteArray(u8 numOfMirrors, u8 *firstByteArray, OUT SFS_MirrorIndex_Info_t *info);
int LibSimpleFS_ConvertArray_ToFD(u8 *buf, OUT SFS_FileDescriptor_t *fd);
int LibSimpleFS_Dump_FD(SFS_FileDescriptor_t *fd);
int LibSimpleFS_Dump_MirrorInfo(SFS_MirrorIndex_Info_t *info);
int LibSimpleFS_CreateFD(u8 numOfMirrors, OUT SFS_FileDescriptor_t **fd_ptr);
int LibSimpleFS_DestroyFD(SFS_FileDescriptor_t *fd_ptr);
int LibSimpleFS_CreateRawArray_FromFD(SFS_FileDescriptor_t *fd_ptr, OUT u8 **ary_ptr, OUT u32 *len);
int LibSimpleFS_DestroyRawArray_FromFD(u8 *ary);

// ============================== Demo ==============================
void LibSimpleFS_Demo(void);

// ============================== Library: Platform Dependant ==============================



#define _LIB_SIMPLE_FILE_SYSTEM_HPP_INCLUDED_
#endif//_LIB_SIMPLE_FILE_SYSTEM_HPP_INCLUDED_


