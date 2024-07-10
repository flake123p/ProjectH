
#include "Everything_Lib_Mgr.hpp"

int LibMemory_SetBit(u8 *buf, u32 bit, u32 val)
{
	BASIC_ASSERT(val < 2);

	u32 byteNum = bit / 8;
	u32 bitNum = 7 - (bit % 8);

	if (val) {
		SET_BIT(buf[byteNum], bitNum);
	} else {
		CLEAR_BIT(buf[byteNum], bitNum);
	}

	return 0;
}

int LibMemory_GetBit(u8 *buf, u32 bit, OUT u32 *val)
{
	u32 byteNum = bit / 8;
	u32 bitNum = 7 - (bit % 8);

	if (CHECK_BIT(buf[byteNum], bitNum)) {
		*val = 1;
	} else {
		*val = 0;
	}

	return 0;
}

/*
	Return value:
		0: Last page
		1: Not last page
*/
int LibMemory_CalculateCrossPageInfo(u32 dstAddr, u32 totalLen, u32 pageStartAddr, u32 pageSize, OUT u32 *newLen)
{
	u32 nextPageStartAddr = pageStartAddr + pageSize;

	if (dstAddr + totalLen > nextPageStartAddr) {
		*newLen = nextPageStartAddr - dstAddr;
		return 1;
	} else {
		*newLen = totalLen;
		return 0;
	}
}

void LibMemory_CalculateCrossPageInfo_TestCase(u32 dstAddr, u32 totalLen, u32 pageSize)
{
	u32 pageStartAddr = PAGE_START_ADDR2(dstAddr, pageSize);
	int isLastPage;
	u32 newLen;

	while(1) {
		isLastPage = LibMemory_CalculateCrossPageInfo(dstAddr, totalLen, pageStartAddr, pageSize, &newLen);
		printf("isLastPage=%d, dstAddr=0x%X, newLen=0x%X\n", isLastPage, dstAddr, newLen);

		if (0 == isLastPage) {
			break;
		} else {
			dstAddr += newLen;
			totalLen -= newLen;
			pageStartAddr += pageSize;
		}
	}
}

VirtualMemClass::VirtualMemClass(void) 
{
	nodeSize = 0x4000;
	initVal=0x100;
	initPageAttr=0;
};

VirtualMemClass::~VirtualMemClass(void)
{
	RemoveAll(DO_AUTO_FREE);
}

void VirtualMemClass::SetParameters(u32 node_size, u32 init_val, u32 init_page_attr /* = 0 */)
{
	nodeSize = node_size;
	initVal=init_val;
	initPageAttr=init_page_attr;
}

int VirtualMemClass::Write(u32 dstAddr, u8 *src, u32 len, bool autoCreatePage /* = true */)
{
	u32 pageStartAddr = PAGE_START_ADDR(dstAddr, nodeSize);
	int isLastPage;
	u32 newLen;
	VIR_MEM_NODE_t *currNode;
	int retVal;
	
	while(len) {
		isLastPage = LibMemory_CalculateCrossPageInfo(dstAddr, len, pageStartAddr, nodeSize, &newLen);

		unless (_NodeExist(pageStartAddr, &currNode)) {
			if (autoCreatePage) {
				_CreateNode(pageStartAddr, &currNode);
				LINKED_LIST_LOG_MSG("NEW NODE\n");
			} else {
				return 0xF0;
			}
		} else {
			LINKED_LIST_LOG_MSG("NODE EXIST\n");
		}
		retVal = _PageWrite(currNode, dstAddr, src, newLen);
		RETURN_IF(retVal);

		dstAddr += newLen;
		len -= newLen;
		pageStartAddr += nodeSize;
		src += newLen;
	}
	BASIC_ASSERT(0 == isLastPage);
	return 0;
}

int VirtualMemClass::Read(u8 *dst, u32 srcAddr, u32 len)
{
	u32 pageStartAddr = PAGE_START_ADDR(srcAddr, nodeSize);
	int isLastPage;
	u32 newLen;
	VIR_MEM_NODE_t *currNode;
	int retVal;

	while(len) {
		isLastPage = LibMemory_CalculateCrossPageInfo(srcAddr, len, pageStartAddr, nodeSize, &newLen);

		unless (_NodeExist(pageStartAddr, &currNode)) {
			LibError_SetExtErrorMessage("MEM_ERROR_PAGE_NOT_EXIST\n");
			return MEM_ERROR_PAGE_NOT_EXIST;
		}
		retVal = _PageRead(currNode, dst, srcAddr, newLen);
		RETURN_IF(retVal);

		srcAddr += newLen;
		len -= newLen;
		pageStartAddr += nodeSize;
		dst += newLen;
	}
	BASIC_ASSERT(0 == isLastPage);
	return 0;
}

int VirtualMemClass::ClearPage(u32 start_addr)
{
	u32 nodeAddr = _CalculateNodeStartAddr(start_addr);

	VIR_MEM_NODE_t *currNode;
	if (_NodeExist(nodeAddr, &currNode)) {
		currNode->usedLen = 0;
		u32 memsetVal;
		if (initVal < 0x100) {
			memsetVal = initVal;
		} else {
			memsetVal = 0;
		}
		memset(currNode->data, memsetVal, nodeSize);
		return 0;
	} else {
		return 1;
	}
}

u8 *VirtualMemClass::GetRealAddress(u32 addr, VIR_MEM_NODE_t **matchNode /* = NULL */)
{
	u32 nodeAddr = _CalculateNodeStartAddr(addr);

	VIR_MEM_NODE_t *currNode;
	if (_NodeExist(nodeAddr, &currNode)) {
		if (matchNode != NULL)
			*matchNode = currNode;
		return &(currNode->data[addr - currNode->startAddr]);
	} else {
		return NULL;
	}
}

int VirtualMemClass::CreateEmptyPage(u32 dstAddr, VIR_MEM_NODE_t **matchNode /* = NULL */)
{
	u32 pageStartAddr = _CalculateNodeStartAddr(dstAddr);
	
	unless (_NodeExist(pageStartAddr, matchNode)) {
		VIR_MEM_NODE_t *currNode;
		_CreateNode(pageStartAddr, &currNode);
		if (matchNode != NULL)
			*matchNode = currNode;
	}

	return 0;
}

bool VirtualMemClass::GetPage(u32 start_addr, OUT VIR_MEM_NODE_t **matchNode)
{
	BASIC_ASSERT(matchNode != NULL);
	
	u32 pageStartAddr = _CalculateNodeStartAddr(start_addr);

	return _NodeExist(pageStartAddr, matchNode);
}

void VirtualMemClass::DumpVirMemInfo(void)
{
	printf("==================================== %s() start\n", __func__);
	DUMPD(info.count);
	DUMPP(info.head);
	DUMPP(info.tail);

	VIR_MEM_NODE_t *currVirMemNode;
	LinkedListNode *currNode = info.head;
	for (u32 i = 0; i < info.count; i++) {
		currVirMemNode = (VIR_MEM_NODE_t *)currNode;
		printf("Node(%d):\n", i+1);
		printf("[ %p ]\n", currNode);
		DUMPP(currNode->prev);
		DUMPP(currNode->next);
		DUMPX(currVirMemNode->startAddr);
		DUMPX(currVirMemNode->usedLen);
		DUMPX(currVirMemNode->pageAttr);
		currNode = currNode->next;
	}
	BASIC_ASSERT(currNode == NULL);
	printf("==================================== %s() end\n", __func__);
}

int  VirtualMemClass::DumpVirMemContent_ToFile(const char *fileName, bool verbose /* = false */, bool memDumpMode /* = false */, bool dumpAllData /* = false */)
{
	u32 minAddr = 0;
	VIR_MEM_NODE_t *currVirMemNode;
	
	LibFileIoClass outFile(fileName, "w+b");

	outFile.FileOpen();

	if (outFile.fp == NULL) {
		return 1;
	}

	if (memDumpMode == false) {
		fprintf(outFile.fp, "info.count = %d\n", info.count);
		if (verbose) {
			fprintf(outFile.fp, "info.head = %p\n", info.head);
			fprintf(outFile.fp, "info.tail = %p\n", info.tail);
		}
		fprintf(outFile.fp, "nodeSize = 0x%x\n", nodeSize);
		fprintf(outFile.fp, "initVal  = 0x%x\n", initVal);

		LinkedListNode *currNode = info.head;
		for (u32 i = 0; i < info.count; i++) {
			currVirMemNode = (VIR_MEM_NODE_t *)currNode;

			// for dumping total size
			if (i == 0)minAddr = currVirMemNode->startAddr;
			
			fprintf(outFile.fp, "Node(%d):\n", i+1);
			if (verbose) {
				fprintf(outFile.fp, "[ %p ]\n", currNode);
				fprintf(outFile.fp, "currNode->prev = %p\n", currNode->prev);
				fprintf(outFile.fp, "currNode->next = %p\n", currNode->next);
			}
			fprintf(outFile.fp, "currVirMemNode->startAddr = 0x%x\n", currVirMemNode->startAddr);
			fprintf(outFile.fp, "currVirMemNode->usedLen   = 0x%x\n", currVirMemNode->usedLen);
			fprintf(outFile.fp, "currVirMemNode->pageAttr  = 0x%x\n", currVirMemNode->pageAttr);

			char buf[100];
			u32 maxLen;
			if (dumpAllData && i != info.count -1 /*not dump all in last page*/) {
				maxLen = nodeSize;
			} else {
				maxLen = currVirMemNode->usedLen;
				if (maxLen % 16 != 0) {
					maxLen = ((maxLen / 16) + 1) * 16;
				}
			}
			for (u32 j = 0; j < maxLen; j+=16) {
				sprintf(
					buf,
					"[%08X]  %02X %02X %02X %02X, %02X %02X %02X %02X, %02X %02X %02X %02X, %02X %02X %02X %02X\n",
					currVirMemNode->startAddr + j,
					currVirMemNode->data[j+0],
					currVirMemNode->data[j+1],
					currVirMemNode->data[j+2],
					currVirMemNode->data[j+3],
					currVirMemNode->data[j+4],
					currVirMemNode->data[j+5],
					currVirMemNode->data[j+6],
					currVirMemNode->data[j+7],
					currVirMemNode->data[j+8],
					currVirMemNode->data[j+9],
					currVirMemNode->data[j+10],
					currVirMemNode->data[j+11],
					currVirMemNode->data[j+12],
					currVirMemNode->data[j+13],
					currVirMemNode->data[j+14],
					currVirMemNode->data[j+15]);
				fprintf(outFile.fp, "%s", buf);
			}

			fprintf(outFile.fp, "\n");
			currNode = currNode->next;
		}
		BASIC_ASSERT(currNode == NULL);
	} 
	else {
		
		LinkedListNode *currNode = info.head;
		for (u32 i = 0; i < info.count; i++) {
			currVirMemNode = (VIR_MEM_NODE_t *)currNode;

			char buf[100];
			u32 maxLen = currVirMemNode->usedLen;
			if (dumpAllData && i != info.count -1 /*not dump all in last page*/) {
				maxLen = nodeSize;
			} else {
				maxLen = currVirMemNode->usedLen;
				if (maxLen % 16 != 0) {
					maxLen = ((maxLen / 16) + 1) * 16;
				}
			}
			for (u32 j = 0; j < maxLen; j+=16) {
				#define FOR_PRINT(a) (LibString_IsCharPrintable(a)?a:' ')

				// for dumping total size
				if (i == 0)minAddr = currVirMemNode->startAddr;
			
				sprintf(
					buf,
					"%08x: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X |%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c|\n",
					currVirMemNode->startAddr + j,
					currVirMemNode->data[j+0],
					currVirMemNode->data[j+1],
					currVirMemNode->data[j+2],
					currVirMemNode->data[j+3],
					currVirMemNode->data[j+4],
					currVirMemNode->data[j+5],
					currVirMemNode->data[j+6],
					currVirMemNode->data[j+7],
					currVirMemNode->data[j+8],
					currVirMemNode->data[j+9],
					currVirMemNode->data[j+10],
					currVirMemNode->data[j+11],
					currVirMemNode->data[j+12],
					currVirMemNode->data[j+13],
					currVirMemNode->data[j+14],
					currVirMemNode->data[j+15],
					FOR_PRINT(currVirMemNode->data[j+0]),
					FOR_PRINT(currVirMemNode->data[j+1]),
					FOR_PRINT(currVirMemNode->data[j+2]),
					FOR_PRINT(currVirMemNode->data[j+3]),
					FOR_PRINT(currVirMemNode->data[j+4]),
					FOR_PRINT(currVirMemNode->data[j+5]),
					FOR_PRINT(currVirMemNode->data[j+6]),
					FOR_PRINT(currVirMemNode->data[j+7]),
					FOR_PRINT(currVirMemNode->data[j+8]),
					FOR_PRINT(currVirMemNode->data[j+9]),
					FOR_PRINT(currVirMemNode->data[j+10]),
					FOR_PRINT(currVirMemNode->data[j+11]),
					FOR_PRINT(currVirMemNode->data[j+12]),
					FOR_PRINT(currVirMemNode->data[j+13]),
					FOR_PRINT(currVirMemNode->data[j+14]),
					FOR_PRINT(currVirMemNode->data[j+15]));
					fprintf(outFile.fp, "%s", buf);
			}
			currNode = currNode->next;
		}
		BASIC_ASSERT(currNode == NULL);
	}

	u32 maxAddr = currVirMemNode->startAddr + currVirMemNode->usedLen;
	u32 totalSize = maxAddr - minAddr;
	
	printf("[ %s ] Dump Result:\n", fileName);
	printf("\tTotal Size = 0x%X\n", totalSize);
	printf("\tTotal Size = %d KB\n", (totalSize) / 1024 + 1);

	fprintf(outFile.fp, "\nDump Result:\n");
	fprintf(outFile.fp, "\tTotal Size = 0x%X\n", totalSize);
	fprintf(outFile.fp, "\tTotal Size = %d KB\n", (totalSize) / 1024 + 1);
	
	return 0;
}

int  VirtualMemClass::ExportVirMemContent(const char *outFileName)
{
	u32 minAddr = 0;
	VIR_MEM_NODE_t *currVirMemNode;
	
	LibFileIoClass outFile(outFileName, "w+b");

	outFile.FileOpen();

	if (outFile.fp == NULL) {
		return 1;
	}

	fprintf(outFile.fp, "nodeSize = 0x%x\n\n", nodeSize);

	LinkedListNode *currNode = info.head;
	for (u32 i = 0; i < info.count; i++) {
		currVirMemNode = (VIR_MEM_NODE_t *)currNode;

		// for dumping total size
		if (i == 0)minAddr = currVirMemNode->startAddr;

		if (currVirMemNode->usedLen) {
			fprintf(outFile.fp, "startAddr = 0x%x\n", currVirMemNode->startAddr);
			fprintf(outFile.fp, "usedLen   = 0x%x\n", currVirMemNode->usedLen);
			fprintf(outFile.fp, "pageAttr  = 0x%x\n", currVirMemNode->pageAttr);
		}

		char buf[100];
		u32 maxLen;
		{
			maxLen = currVirMemNode->usedLen;
			if (maxLen % 16 != 0) {
				maxLen = ((maxLen / 16) + 1) * 16;
			}
		}
		for (u32 j = 0; j < maxLen; j+=16) {
			sprintf(
				buf,
				"[%08X]  %02X %02X %02X %02X  %02X %02X %02X %02X  %02X %02X %02X %02X  %02X %02X %02X %02X\n",
				currVirMemNode->startAddr + j,
				currVirMemNode->data[j+0],
				currVirMemNode->data[j+1],
				currVirMemNode->data[j+2],
				currVirMemNode->data[j+3],
				currVirMemNode->data[j+4],
				currVirMemNode->data[j+5],
				currVirMemNode->data[j+6],
				currVirMemNode->data[j+7],
				currVirMemNode->data[j+8],
				currVirMemNode->data[j+9],
				currVirMemNode->data[j+10],
				currVirMemNode->data[j+11],
				currVirMemNode->data[j+12],
				currVirMemNode->data[j+13],
				currVirMemNode->data[j+14],
				currVirMemNode->data[j+15]);
			fprintf(outFile.fp, "%s", buf);
		}

		if (currVirMemNode->usedLen) {
			fprintf(outFile.fp, "\n");
		}
		currNode = currNode->next;
	}
	BASIC_ASSERT(currNode == NULL);

	u32 maxAddr = currVirMemNode->startAddr + currVirMemNode->usedLen;
	u32 totalSize = maxAddr - minAddr;
	
	printf("[ %s ] Dump Result:\n", outFileName);
	printf("\tTotal Size = 0x%X\n", totalSize);
	printf("\tTotal Size = %d KB\n", (totalSize) / 1024 + 1);

	fprintf(outFile.fp, "\nEOF:\n");
	fprintf(outFile.fp, "\nDump Result:\n");
	fprintf(outFile.fp, "\tTotal Size = 0x%X\n", totalSize);
	fprintf(outFile.fp, "\tTotal Size = %d KB\n", (totalSize) / 1024 + 1);
	
	return 0;
}

int  VirtualMemClass::ImportVirMemContent(const char *inFileName)
{
	return 0;
}

bool VirtualMemClass::_NodeExist(u32 start_addr, OUT VIR_MEM_NODE_t **matchNode /* = NULL */)
{
	if (info.count == 0) {
		return false;
	}

	VIR_MEM_NODE_t *currVirMemNode;
	for (LinkedListNode *currNode = info.head; currNode != NULL; currNode = currNode->next) {
		currVirMemNode = (VIR_MEM_NODE_t *)currNode;
		if (currVirMemNode->startAddr == start_addr) {
			if (matchNode != NULL)
				*matchNode = currVirMemNode;
			return true;
		}
	}

	return false;
}

u32 VirtualMemClass::_CalculateNodeStartAddr(u32 addr)
{
	return PAGE_START_ADDR(addr, nodeSize);
}

int VirtualMemClass::_CreateNode(u32 start_addr, OUT VIR_MEM_NODE_t **newNode /* = NULL */)
{
	VIR_MEM_NODE_t *newVirMemNode = (VIR_MEM_NODE_t *)malloc(sizeof(VIR_MEM_NODE_t) + nodeSize + 32);
	BASIC_ASSERT(newVirMemNode != NULL);
	if (newNode != NULL) {
		*newNode = newVirMemNode;
	}
	if (initVal < 0x100) {
		memset(newVirMemNode, (u8)initVal, sizeof(VIR_MEM_NODE_t) + nodeSize + 32);
	}

	newVirMemNode->usedLen = 0;
	newVirMemNode->startAddr = start_addr;
	newVirMemNode->pageAttr = initPageAttr;
	
	VIR_MEM_NODE_t *currVirMemNode;
	for (LinkedListNode *currNode = info.head; currNode != NULL; currNode = currNode->next) {
		currVirMemNode = (VIR_MEM_NODE_t *)currNode;
		BASIC_ASSERT(newVirMemNode->startAddr != currVirMemNode->startAddr);
		if (newVirMemNode->startAddr < currVirMemNode->startAddr) {
			InsertFront(currNode, (LinkedListNode *)newVirMemNode);
			return 0;
		}
	}

	PushBack((LinkedListNode *)newVirMemNode);
	return 0;
}

int VirtualMemClass::_PageWrite(VIR_MEM_NODE_t *node, u32 dstAddr, u8 *src, u32 len)
{
	BASIC_ASSERT(IsThisNodeInList((LinkedListNode *)node) != NODE_ISNT_IN_LIST);
	BASIC_ASSERT(((dstAddr/nodeSize)*nodeSize) == node->startAddr);
	BASIC_ASSERT((dstAddr+len) <= (node->startAddr+nodeSize)); // Cross Page

	u32 nodeAddr = dstAddr - node->startAddr;
	u8 *dst = &(node->data[nodeAddr]);

	if (FLG_CHK(node->pageAttr, MEMORY_WRITE_PROTECT_FLAG)) {
		LibError_SetExtErrorMessage("%s(): MEM_ERROR_WRITE_PROTECTED\n", __func__);
		return MEM_ERROR_WRITE_PROTECTED;
	}
	
	memcpy(dst, src, len);

	if ((len+nodeAddr) > node->usedLen) {
		node->usedLen = (len+nodeAddr);
	}

	return 0;
}

int VirtualMemClass::_PageRead(VIR_MEM_NODE_t *node, u8 *dst, u32 srcAddr, u32 len)
{
	BASIC_ASSERT(IsThisNodeInList((LinkedListNode *)node) != NODE_ISNT_IN_LIST);
	BASIC_ASSERT(((srcAddr/nodeSize)*nodeSize) == node->startAddr);
	BASIC_ASSERT((srcAddr+len) <= (node->startAddr+nodeSize)); // Cross Page
	
	u8 *src = GetRealAddress(srcAddr);
	BASIC_ASSERT(src != NULL);

	if (FLG_CHK(node->pageAttr, MEMORY_READ_PROTECT_FLAG)) {
		LibError_SetExtErrorMessage("%s(): MEM_ERROR_READ_PROTECTED\n", __func__);
		return MEM_ERROR_READ_PROTECTED;
	}
	
	memcpy(dst, src, len);

	return 0;
}

ProtectedMemClass::ProtectedMemClass(void)
{
	virMem.SetParameters(0x4000, 0x100 /*not init with value, because bigger than 0xFF*/);
	attrMem.SetParameters(0x4000, 0);
}

ProtectedMemClass::~ProtectedMemClass(void)
{
}

void ProtectedMemClass::SetParameters(u32 node_size, u32 initVal, u32 pageAttr /* = 0 */)
{
	virMem.SetParameters(node_size, initVal, pageAttr);
	attrMem.SetParameters(node_size, 0, 0);
}

int ProtectedMemClass::InitMemory(u32 startAddr, u32 size)
{
	u32 firstPageStartAddr = PAGE_START_ADDR(startAddr, virMem.nodeSize);
	u32 lastPageStartAddr  = PAGE_START_ADDR(startAddr+size, virMem.nodeSize);

	for (u32 currPageStartAddr = firstPageStartAddr; currPageStartAddr < lastPageStartAddr; currPageStartAddr += virMem.nodeSize) {
		virMem.CreateEmptyPage(currPageStartAddr);
		attrMem.CreateEmptyPage(currPageStartAddr);
	}
	return 0;
}

int ProtectedMemClass::Write(u32 dstAddr, u8 *src, u32 len, bool doWriteOnceCheck /* = false */)
{
	int retVal;
	LibBufferBasic bufObj;
	bufObj.Init(len);
	u8 *attrSrc = (u8 *)bufObj.bufPtr;

	EXIT_CHK(retVal, attrMem.Read(attrSrc, dstAddr, len));
	
	for (u32 i = 0; i < len; i++) {
		if (doWriteOnceCheck && FLG_CHK(attrSrc[i], MEMROY_WAS_WRITTEN_FLAG)) {
			LibError_SetExtErrorMessage("MEM_ERROR_BYTE_ALREADY_WRITTEN\n");
			return MEM_ERROR_BYTE_ALREADY_WRITTEN;
		}
		attrSrc[i] |= MEMROY_WAS_WRITTEN_FLAG;
	}
	
	retVal = virMem.Write(dstAddr, src, len, false);
	RETURN_IF(retVal);
		
	retVal = attrMem.Write(dstAddr, attrSrc, len, false);
	RETURN_IF(retVal);

	return 0;
}

int ProtectedMemClass::Read(u8 *dst, u32 srcAddr, u32 len)
{
	return virMem.Read(dst, srcAddr, len);
}

int ProtectedMemClass::AddPageAttributes(u32 addr, u32 flags, OUT u32 *attr /* = NULL */)
{
	VIR_MEM_NODE_t *matchNode;
	if (virMem.GetPage(addr, &matchNode)) {
		FLG_ADD(matchNode->pageAttr, flags);

		if (attr != NULL)
			*attr = matchNode->pageAttr;
			
		if (attrMem.GetPage(addr, &matchNode)) {
			FLG_ADD(matchNode->pageAttr, flags);
		} else {
			BASIC_ASSERT(0);
		}

		return 0;
	}

	return 1;
}

int ProtectedMemClass::RemovePageAttributes(u32 addr, u32 flags, OUT u32 *attr /* = NULL */)
{
	VIR_MEM_NODE_t *matchNode;
	if (virMem.GetPage(addr, &matchNode)) {
		FLG_RMV(matchNode->pageAttr, flags);

		if (attr != NULL)
			*attr = matchNode->pageAttr;
			
		if (attrMem.GetPage(addr, &matchNode)) {
			FLG_RMV(matchNode->pageAttr, flags);
		} else {
			BASIC_ASSERT(0);
		}

		return 0;
	}

	return 1;
}

int ProtectedMemClass::ClearByteWrittenFlag(u32 startAddr, u32 len)
{
	u32 pageStartAddr = PAGE_START_ADDR(startAddr, attrMem.nodeSize);
	int isLastPage;
	u32 newLen;
	u8 *src;
	u32 i;

	while(len) {
		isLastPage = LibMemory_CalculateCrossPageInfo(startAddr, len, pageStartAddr, attrMem.nodeSize, &newLen);
		src = attrMem.GetRealAddress(startAddr);
		if (src == NULL)
			return MEM_ERROR_PAGE_NOT_EXIST;
			
		for (i = 0; i < newLen; i++) {
			FLG_RMV(src[i], MEMROY_WAS_WRITTEN_FLAG);
		}
		
		startAddr += newLen;
		len -= newLen;
		pageStartAddr += attrMem.nodeSize;
	}
	BASIC_ASSERT(0 == isLastPage);
	return 0;
}

int ProtectedMemClass::DumpProtectedMemContent_ToFile(const char *fileName1, const char *fileName2, bool memDumpMode /* = false */, bool dumpAllData /* = false */)
{
	virMem.DumpVirMemContent_ToFile(fileName1, memDumpMode, dumpAllData);
	attrMem.DumpVirMemContent_ToFile(fileName2, memDumpMode, dumpAllData);
	return 0;
}

int ProtectedMemClass::ExportProtectedMemContent(const char *outFileName)
{
	virMem.ExportVirMemContent(outFileName);
	return 0;
}

int ProtectedMemClass::ImportProtectedMemContent(const char *inFileName)
{
	virMem.ImportVirMemContent(inFileName);
	return 0;
}

void LibMemory_Demo(void)
{
	DUMPX(PAGE_START_ADDR(0x11, 0x100));
	DUMPX(PAGE_START_ADDR(0x111, 0x100));
	DUMPX(PAGE_START_ADDR(0x2211, 0x100));
	DUMPX(PAGE_START_ADDR2(0x11, 0x100));
	DUMPX(PAGE_START_ADDR2(0x111, 0x100));
	DUMPX(PAGE_START_ADDR2(0x2211, 0x100));

	LibMemory_CalculateCrossPageInfo_TestCase(0x3FE, 0x103, 0x100);
}

void LibMemory_Demo2(void)
{
	VirtualMemClass virMem;
	u8 ary1[10] = {0x11, 0x22, 0x33};
	virMem.SetParameters(0x1000, 0xFF);
	virMem.Write(0x00000000, ary1, 3);
	virMem.Write(0x00000010, ary1, 3);
	virMem.Write(0x00000020, ary1, 3);
	virMem.Write(0x00000021, ary1, 3);
	virMem.Write(0x00007FFF, ary1, 3);
	virMem.DumpVirMemInfo();
	virMem.CreateEmptyPage(0x2001);

	VIR_MEM_NODE_t *currPage;
	
	bool retVal = virMem.GetPage(0x2001, &currPage);
	BASIC_ASSERT(retVal == true);
	
	FLG_ADD(currPage->pageAttr, MEMORY_WRITE_PROTECT_FLAG);
	DUMPD(virMem.Write(0x00002000, ary1, 3));
	
	FLG_RMV(currPage->pageAttr, MEMORY_WRITE_PROTECT_FLAG);
	DUMPD(virMem.Write(0x00002003, ary1, 3));



	retVal = virMem.GetPage(0x00007FFC, &currPage);
	BASIC_ASSERT(retVal == true);
	
	FLG_ADD(currPage->pageAttr, MEMORY_READ_PROTECT_FLAG);
	memset(ary1, 0, 10);
	DUMPD(virMem.Read(ary1, 0x00007FFC, 10));
	ARRAYDUMPX2(ary1, 10);

	FLG_RMV(currPage->pageAttr, MEMORY_READ_PROTECT_FLAG);
	memset(ary1, 0, 10);
	DUMPD(virMem.Read(ary1, 0x00007FFE, 10));
	ARRAYDUMPX2(ary1, 10);

	virMem.DumpVirMemContent_ToFile("LibMemoryDEMO2.txt");
}

void LibMemory_Demo3(void)
{
	ProtectedMemClass protectedMem;
	protectedMem.SetParameters(0x1000, 0xFF, MEMORY_WRITE_PROTECT_FLAG | MEMORY_READ_PROTECT_FLAG);

	protectedMem.InitMemory(0, 0x3000);

	u8 ary1[] = {0x11, 0x22, 0x33};
	int retVal = protectedMem.Write(0x00000FFF, ary1, 3);
	DUMPX(retVal);
	
	protectedMem.RemovePageAttributes(0x0000, MEMORY_WRITE_PROTECT_FLAG);
	retVal = protectedMem.Write(0x00000FFF, ary1, 3);
	DUMPX(retVal);

	protectedMem.RemovePageAttributes(0x1000, MEMORY_WRITE_PROTECT_FLAG);
	retVal = protectedMem.Write(0x00000FFF, ary1, 3);
	DUMPX(retVal);

	retVal = protectedMem.Write(0x00000FFF, ary1, 3, true);
	DUMPX(retVal);

	protectedMem.ClearByteWrittenFlag(0x0FFF, 3);

	protectedMem.DumpProtectedMemContent_ToFile("LibMemoryDEMO3_A.txt", "LibMemoryDEMO3_B.txt", false, true);
}

void LibMemory_Demo4(void)
{
	u8 buf[4] = {0};

	LibMemory_SetBit((u8 *)buf, 0, 1);

	ARRAYDUMPX2(buf, 4);

	LibMemory_SetBit((u8 *)buf, 7, 1);

	ARRAYDUMPX2(buf, 4);

	LibMemory_SetBit((u8 *)buf, 8, 1);

	ARRAYDUMPX2(buf, 4);
}