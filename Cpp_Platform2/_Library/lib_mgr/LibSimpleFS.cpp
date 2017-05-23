
#include "Everything_Lib_Mgr.hpp"

int LibSimpleFS_GetMirrorInfo_FromFirstByteArray(u8 numOfMirrors, u8 *firstByteArray, OUT SFS_MirrorIndex_Info_t *info)
{
	u8 workingIndex;
	u8 workingFirstByte;
	u8 nextIndex;
	u8 nextFirstByte;
	
	//u8 currMirror = 0;
	//u8 currMirror_ActiveIndexByte;
	//u8 nextMirrorIndex;
	//u8 nextMirror_ActiveIndexByte;
	u8 i;

	// Find first valid index
	for (i=0; i<numOfMirrors; i++) {
		if (firstByteArray[i] != 0xFF) {
			workingIndex = i;
			workingFirstByte = firstByteArray[i];
			break;
		}
	}
	// if all index == 0xFF, than no active index!!
	if (i == numOfMirrors) {
		//fd.activeIndex = 0xFF; //Invalid first byte for no active index
		//info->activeMirrorIndex = 0; //don't care
		//info->ativeMirrorValue = 0xFF;
		//info->nextMirrorIndex = 0;
		//info->nextMirrorValue = 0xFF;
		if (info != NULL) {
			info->workingIndex = 0xFF;
			info->workingFirstByte = 0xFF;
			info->nextIndex = 0;
			info->nextFirstByte = 0;
		}
		return 1;
	}

	// find active mirror
	//currMirror_ActiveIndexByte = mirrorIndexBuf[currMirror];
	nextIndex = INC_IN_RANGE_CYCLE(workingIndex, 0, numOfMirrors);
	nextFirstByte = firstByteArray[nextIndex];
	u32 loopCtr = 0;
	while (1) {
		if (workingFirstByte == 0xFE) {
			if (nextFirstByte == 0) {
				;//curr is not active one
			} else {
				break; //curr is active one
			}
		} else if (workingFirstByte != 0xFF) {
			if (nextFirstByte == workingFirstByte+1) {
				;//curr is not active one
			} else {
				break; //curr is active one
			}
		} else {
			BASIC_ASSERT(0);
		}

		workingIndex = nextIndex;
		workingFirstByte = firstByteArray[workingIndex];
		nextIndex = INC_IN_RANGE_CYCLE(workingIndex, 0, numOfMirrors);
		nextFirstByte = firstByteArray[nextIndex];
		
		loopCtr++;
		BASIC_ASSERT(loopCtr <= numOfMirrors)
		REMOVE_UNUSED_WRANING(loopCtr);
	}

	//fd->activeIndex = workingIndex;
	
	if (info != NULL) {
		info->workingIndex = workingIndex;
		info->workingFirstByte = workingFirstByte;
		info->nextIndex = INC_IN_RANGE_CYCLE(workingIndex, 0, numOfMirrors);
		info->nextFirstByte = INC_IN_RANGE_CYCLE(workingFirstByte, 0, 0xFF);
	}
	return 0;
}

int LibSimpleFS_ConvertArray_ToFD(u8 *buf, OUT SFS_FileDescriptor_t *fd)
{
	u32 currOffset = 0;
	
	ARRAY_TO_INT(fd->activeIndex, buf, currOffset, DO_ENDIAN_SWAP);
	ARRAY_TO_INT(fd->usedSize, buf, currOffset, DO_ENDIAN_SWAP);
	ARRAY_TO_INT(fd->maxSize, buf, currOffset, DO_ENDIAN_SWAP);
	ARRAY_TO_INT(fd->numOfMirrors, buf, currOffset, DO_ENDIAN_SWAP);

	for (u8 i = 0; i < fd->numOfMirrors; i++) {
		ARRAY_TO_INT(fd->mirrorAddr[i], buf, currOffset, DO_ENDIAN_SWAP);
	}

	return 0;
}

int LibSimpleFS_Dump_FD(SFS_FileDescriptor_t *fd)
{
	printf("activeIndex   = %d\n", fd->activeIndex);
	printf("usedSize      = 0x%x\n", fd->usedSize);
	printf("maxSize       = 0x%x\n", fd->maxSize);
	printf("numOfMirrors  = %d\n", fd->numOfMirrors);

	for (u8 i = 0; i < fd->numOfMirrors; i++) {
		printf("mirrorAddr[%d] = 0x%x\n", i, fd->mirrorAddr[i]);
	}

	return 0;
}

int LibSimpleFS_Dump_MirrorInfo(SFS_MirrorIndex_Info_t *info)
{
	printf("> workingIndex     = %d\n", info->workingIndex);
	printf("> workingFirstByte = 0x%x\n", info->workingFirstByte);
	printf("> nextIndex        = 0x%x\n", info->nextIndex);
	printf("> nextFirstByte    = %d\n", info->nextFirstByte);

	return 0;
}

int LibSimpleFS_CreateFD(u8 numOfMirrors, OUT SFS_FileDescriptor_t **fd_ptr)
{
	*fd_ptr = (SFS_FileDescriptor_t *)malloc(sizeof(SFS_FileDescriptor_t) - sizeof(u32) + (numOfMirrors * sizeof(u32)));

	if (fd_ptr == NULL) {
		LibError_SetExtErrorMessage("%s(): malloc failed in *fd_ptr.\n", __func__);
		return 1;
	}
/*
	(*fd_ptr)->mirrorAddr = (u32 *)malloc(sizeof(u32) * numOfMirrors);
	if ((*fd_ptr)->mirrorAddr == NULL) {
		LibError_SetExtErrorMessage("%s(): malloc failed in (*fd_ptr)->mirrorAddr.\n", __func__);
		free(*fd_ptr);
		return 2;
	}
*/
	return 0;
}

int LibSimpleFS_DestroyFD(SFS_FileDescriptor_t *fd_ptr)
{
	//free(fd_ptr->mirrorAddr);
	free(fd_ptr);

	return 0;
}

int LibSimpleFS_CreateRawArray_FromFD(SFS_FileDescriptor_t *fd_ptr, OUT u8 **ary_ptr, OUT u32 *len)
{
	u32 totalLen = 10 + (4 * fd_ptr->numOfMirrors);

	*len = totalLen;
	*ary_ptr = (u8 *)malloc(totalLen);

	if (ary_ptr == NULL) {
		LibError_SetExtErrorMessage("%s(): malloc failed in *ary_ptr.\n", __func__);
		return 1;
	}

	u32 currOffset = 0;
	INT_TO_ARRAY(*ary_ptr, fd_ptr->activeIndex, currOffset, DO_ENDIAN_SWAP);
	INT_TO_ARRAY(*ary_ptr, fd_ptr->usedSize, currOffset, DO_ENDIAN_SWAP);
	INT_TO_ARRAY(*ary_ptr, fd_ptr->maxSize, currOffset, DO_ENDIAN_SWAP);
	INT_TO_ARRAY(*ary_ptr, fd_ptr->numOfMirrors, currOffset, DO_ENDIAN_SWAP);
	for (u8 i=0; i<fd_ptr->numOfMirrors; i++) {
		INT_TO_ARRAY(*ary_ptr, fd_ptr->mirrorAddr[i], currOffset, DO_ENDIAN_SWAP);
	}
	
	return 0;
}

int LibSimpleFS_DestroyRawArray_FromFD(u8 *ary)
{
	free(ary);

	return 0;
}

void LibSimpleFS_Demo(void)
{
}