
#include "Everything_Lib_Mgr.hpp"

#define ENABLE_DIAGNOSE ( 1 )
#if ENABLE_DIAGNOSE
#define DIAGNOSE() LibLinkedList_Diagnose(info, __func__)
#else
#define DIAGNOSE()
#endif

void LibLinkedList_InitInfo(LinkedListInfo *info)
{
	info->head = NULL;
	info->tail = NULL;
	info->count = 0;

	DIAGNOSE();
}

u32 LibLinkedList_Length(LinkedListInfo *info)
{
	DIAGNOSE();
	
	return info->count;
}

void LibLinkedList_PushBack(LinkedListInfo *info, LinkedListNode *newNode)
{
	if (info->head == NULL) {
		info->head = newNode;
		info->tail = newNode;
		newNode->next = NULL;
		newNode->prev = NULL;
	} else {
		// 1. Handle tail
		LinkedListNode *oldLastnode = info->tail;
		info->tail = newNode;

		// 2. Handle old last node
		oldLastnode->next = newNode;

		// 3. Handle new node
		newNode->next = NULL;
		newNode->prev = oldLastnode;
	}

	info->count++;

	DIAGNOSE();
}

void LibLinkedList_PopBack(LinkedListInfo *info, AUTO_FREE autoFree /* = 0 */)
{
	if (info->head == NULL) {
		DIAGNOSE();
		return;
	}
	
	LinkedListNode *nodeForPop = info->tail;

	// 1 Handle head
	if (NODE_IS_HEAD(nodeForPop)) {
		info->head = NULL;
		info->tail = NULL;
	} else {
		LinkedListNode *nodeBeforePop = nodeForPop->prev;
		BASIC_ASSERT(nodeForPop->prev != NULL);
		
		// 2 Handle tail
		info->tail = nodeBeforePop;

		// 3 Handle prev node
		nodeBeforePop->next = NULL;
		
		// 4 Handle next node
		//NONE
	}

	info->count--;
	
	if (autoFree == DO_AUTO_FREE) {
		free(nodeForPop);
	}
	
	DIAGNOSE();
}

// Return remove node count, 0 for nothing removed
int LibLinkedList_Remove(LinkedListInfo *info, LinkedListNode *nodeForRemove, AUTO_FREE autoFree /* = 0 */)
{
	if (NODE_ISNT_IN_LIST ==LibLinkedList_IsThisNodeInList(info, nodeForRemove)) {
		return NODE_ISNT_IN_LIST;
	}

	BASIC_ASSERT(info->head != NULL);

	LinkedListNode *nodeBeforeRemove = nodeForRemove->prev;
	LinkedListNode *nodeAfterRemove = nodeForRemove->next;
	
	// 1 Handle head
	if (NODE_IS_HEAD(nodeForRemove)) {
		info->head = nodeAfterRemove;
	}
	// 2 Handle tail
	if (NODE_IS_TAIL(nodeForRemove)) {
		info->tail = nodeBeforeRemove;
	}

	// 3 Handle prev node
	if (nodeBeforeRemove != NULL) {
		nodeBeforeRemove->next = nodeAfterRemove;
	}

	// 4 Handle next node
	if (nodeAfterRemove != NULL) {
		nodeAfterRemove->prev = nodeBeforeRemove;
	}
	
	info->count--;
	
	if (autoFree == DO_AUTO_FREE) {
		free(nodeForRemove);
	}
	
	DIAGNOSE();
	return 0;
}

void LibLinkedList_RemoveAll(LinkedListInfo *info, AUTO_FREE autoFree /* = 0 */)
{
	LinkedListNode *currheadNode = info->head;
	while (currheadNode != NULL) {
		if (LibLinkedList_Remove(info, currheadNode, autoFree)) {
			BASIC_ASSERT(0);
		}
		currheadNode = info->head;
	}

	info->count = 0;

	DIAGNOSE();
	return;
}

int LibLinkedList_InsertBack(LinkedListInfo *info, LinkedListNode *nodeBeInserted, LinkedListNode *nodeForInsert)
{
	if (NODE_ISNT_IN_LIST == LibLinkedList_IsThisNodeInList(info, nodeBeInserted)) {
		DIAGNOSE();
		return NODE_ISNT_IN_LIST;
	}

	BASIC_ASSERT(nodeBeInserted != NULL);
	BASIC_ASSERT(nodeForInsert != NULL);
	
	LinkedListNode *nodeBeforeInsertNode = nodeBeInserted;
	LinkedListNode *nodeAfterInsertNode = nodeBeInserted->next;
	
	// 1 Handle head
	//NONE
	
	// 2 Handle tail
	if (NODE_IS_TAIL(nodeBeInserted)) {
		info->tail = nodeForInsert;
	}

	// 3 Handle prev node
	nodeBeforeInsertNode->next = nodeForInsert;

	// 4 Handle next node
	if (nodeAfterInsertNode != NULL) {
		nodeAfterInsertNode->prev = nodeForInsert;
	}

	// 5 Handle new node
	nodeForInsert->prev = nodeBeforeInsertNode;
	nodeForInsert->next = nodeAfterInsertNode;
	
	info->count++;

	DIAGNOSE();
	return 0;
}

int LibLinkedList_InsertFront(LinkedListInfo *info, LinkedListNode *nodeBeInserted, LinkedListNode *nodeForInsert)
{
	if (NODE_ISNT_IN_LIST == LibLinkedList_IsThisNodeInList(info, nodeBeInserted)) {
		DIAGNOSE();
		return NODE_ISNT_IN_LIST;
	}

	BASIC_ASSERT(nodeBeInserted != NULL);
	BASIC_ASSERT(nodeForInsert != NULL);
	
	LinkedListNode *nodeBeforeInsertNode = nodeBeInserted->prev;
	LinkedListNode *nodeAfterInsertNode = nodeBeInserted;
	
	// 1 Handle head
	if (NODE_IS_HEAD(nodeBeInserted)) {
		info->head = nodeForInsert;
	}
	
	// 2 Handle tail
	//NONE

	// 3 Handle prev node
	if (nodeBeforeInsertNode != NULL) {
		nodeBeforeInsertNode->next = nodeForInsert;
	}
	
	// 4 Handle next node
	nodeAfterInsertNode->prev = nodeForInsert;

	// 5 Handle new node
	nodeForInsert->prev = nodeBeforeInsertNode;
	nodeForInsert->next = nodeAfterInsertNode;
	
	info->count++;

	DIAGNOSE();
	return 0;
}

int LibLinkedList_IsThisNodeInList(LinkedListInfo *info, LinkedListNode *nodeForCheck)
{
	if (info->head == NULL) {
		DIAGNOSE();
		return NODE_ISNT_IN_LIST;
	}

	for (LinkedListNode *currNode = info->head; currNode != NULL; currNode = currNode->next) {
		if (currNode == nodeForCheck) {
			return 0;
		}
	}

	return NODE_ISNT_IN_LIST;
}

void LibLinkedList_Dump(LinkedListInfo *info)
{
	printf("==================================== %s() start\n", __func__);
	DUMPD(info->count);
	DUMPX((u32)info->head);
	DUMPX((u32)info->tail);
	LinkedListNode *currNode = info->head;
	for (u32 i = 0; i < info->count; i++) {
		printf("Node(%d):\n", i+1);
		printf("[ 0x%X ]\n", (u32)currNode);
		DUMPX((u32)currNode->prev);
		DUMPX((u32)currNode->next);
		currNode = currNode->next;
	}
	BASIC_ASSERT(currNode == NULL);
	printf("==================================== %s() end\n", __func__);
}

void LibLinkedList_Diagnose(LinkedListInfo *info, const char *caller)
{
	BASIC_ASSERT(caller != NULL);

	// Length check
	u32 count = 0;
	LinkedListNode *currNode = info->head;
	while (currNode != NULL) {
		count++;
		currNode = currNode->next;
	}
	if (count != info->count) {
		DUMPD(count);
		DUMPD(info->count);
		CALLER_ASSERT(count == info->count);
	}
	// Length check in backward
	count = 0;
	currNode = info->tail;
	while (currNode != NULL) {
		count++;
		currNode = currNode->prev;
	}
	CALLER_ASSERT(count == info->count);

	// Generic check
	if (info->head != NULL) {
		CALLER_ASSERT(info->head->prev == NULL);
		CALLER_ASSERT(info->tail != NULL);
		CALLER_ASSERT(info->tail->next == NULL);

		// Check relation address between 2 nodes
		for (LinkedListNode *currNode = info->head; currNode != NULL; currNode = currNode->next) {
			if (currNode->next != NULL) {
				CALLER_ASSERT(currNode == currNode->next->prev);
			}
		}
		for (LinkedListNode *currNode = info->tail; currNode != NULL; currNode = currNode->prev) {
			if (currNode->prev != NULL) {
				CALLER_ASSERT(currNode == currNode->prev->next);
			}
		}
	} else {
		CALLER_ASSERT(info->tail == NULL);
	}
}

void LinkedListClass::InitInfo(void)
{
	LibLinkedList_InitInfo(&info);
}

u32  LinkedListClass::Length(void)
{
	return LibLinkedList_Length(&info);
}

void LinkedListClass::PushBack(LinkedListNode *newNode)
{
	LibLinkedList_PushBack(&info, newNode);
}

void LinkedListClass::PopBack(AUTO_FREE autoFree /* = 0 */)
{
	LibLinkedList_PopBack(&info, autoFree);
}

int LinkedListClass::Remove(LinkedListNode *nodeForRemove, AUTO_FREE autoFree /* = 0 */)
{
	return LibLinkedList_Remove(&info, nodeForRemove, autoFree);
}

void LinkedListClass::RemoveAll(AUTO_FREE autoFree /* = 0 */)
{
	LibLinkedList_RemoveAll(&info, autoFree);
}

int LinkedListClass::InsertBack(LinkedListNode *nodeBeInserted, LinkedListNode *nodeForInsert)
{
	return LibLinkedList_InsertBack(&info, nodeBeInserted, nodeForInsert);
}

int LinkedListClass::InsertFront(LinkedListNode *nodeBeInserted, LinkedListNode *nodeForInsert)
{
	return LibLinkedList_InsertFront(&info, nodeBeInserted, nodeForInsert);
}

int LinkedListClass::IsThisNodeInList(LinkedListNode *nodeForCheck)
{
	return LibLinkedList_IsThisNodeInList(&info, nodeForCheck);
}

void LinkedListClass::Dump(void)
{
	LibLinkedList_Dump(&info);
}

void LinkedListClass::Diagnose(const char *caller)
{
	LibLinkedList_Diagnose(&info, caller);
}

VirtualMemClass::VirtualMemClass(void) 
{
	nodeSize = 0x4000;
	initVal=0x100;
};

VirtualMemClass::~VirtualMemClass(void)
{
	RemoveAll(DO_AUTO_FREE);
}

void VirtualMemClass::SetParameters(u32 node_size, u32 init_val)
{
	nodeSize = node_size;
	initVal=init_val;
}

int VirtualMemClass::Write(u32 dstAddr, u8 *src, u32 len, bool doFirstWriteCheck /* = false */)
{
	u32 currDstAddr = dstAddr;
	//u32 nextDstAddr = 0;
	u32 currSrcOffset = 0;
	//u32 nextSrcOffset = 0;
	u32 currLen = 0;
	u32 remainLen = len;

	u32 currNodeStartAddr = (currDstAddr / nodeSize) * nodeSize;
	u32 nextNodeStartAddr = currNodeStartAddr + nodeSize;
	VIR_MEM_NODE_t *currNode;
	while (remainLen) {
		unless (NodeExist(currNodeStartAddr, &currNode)) {
			NewNode(currNodeStartAddr, &currNode);
			LINKED_LIST_LOG_MSG("NEW NODE\n");
		} else {
			LINKED_LIST_LOG_MSG("NODE EXIST\n");
		}

		if (remainLen + currDstAddr > nextNodeStartAddr) {
			currLen = nextNodeStartAddr - currDstAddr;
			PageWrite(currNode, currDstAddr, &(src[currSrcOffset]), currLen, doFirstWriteCheck);
			currDstAddr = nextNodeStartAddr;
		} else {
			currLen = remainLen;
			PageWrite(currNode, currDstAddr, &(src[currSrcOffset]), currLen, doFirstWriteCheck);
		}
		remainLen -= currLen;
		currSrcOffset += currLen;
		currNodeStartAddr = nextNodeStartAddr;
		nextNodeStartAddr = currNodeStartAddr + nodeSize;
	}
	
	return 0;
}

int VirtualMemClass::CreateDummyPage(u32 dstAddr)
{
	u8 src[1] = {0};
	u32 pageStartAddr = GetPageStartAddr_ByRandomAddr(dstAddr);

	if (initVal < 0x100) {
		src[0] = (u8)initVal;
	}

	unless (NodeExist(pageStartAddr)) {
		VIR_MEM_NODE_t *currNode;
		Write(dstAddr, src, 1);
		if (true == NodeExist(pageStartAddr, &currNode)) {
			currNode->usedLen = 0;
		} else {
			BASIC_ASSERT(0);
		}
	}

	return 0;
}

void VirtualMemClass::DumpVirMemNodeInfo(void)
{
	printf("==================================== %s() start\n", __func__);
	DUMPD(info.count);
	DUMPX((u32)info.head);
	DUMPX((u32)info.tail);

	VIR_MEM_NODE_t *currVirMemNode;
	LinkedListNode *currNode = info.head;
	for (u32 i = 0; i < info.count; i++) {
		currVirMemNode = (VIR_MEM_NODE_t *)currNode;
		printf("Node(%d):\n", i+1);
		printf("[ 0x%X ]\n", (u32)currNode);
		DUMPX((u32)currNode->prev);
		DUMPX((u32)currNode->next);
		DUMPX(currVirMemNode->startAddr);
		DUMPX(currVirMemNode->usedLen);
		currNode = currNode->next;
	}
	BASIC_ASSERT(currNode == NULL);
	printf("==================================== %s() end\n", __func__);
}

int  VirtualMemClass::DumpVirMemNodeContent_ToFile(const char *fileName, bool memDumpMode /* = false */)
{
	LibFileIoClass outFile(fileName, "w+b");

	outFile.FileOpen();

	if (outFile.fp == NULL) {
		return 1;
	}

	if (memDumpMode == false) {
		fprintf(outFile.fp, "info.count = %d\n", info.count);
		fprintf(outFile.fp, "(u32)info.head = 0x%x\n", (u32)info.head);
		fprintf(outFile.fp, "(u32)info.tail = 0x%x\n", (u32)info.tail);
		fprintf(outFile.fp, "nodeSize = 0x%x\n", nodeSize);
		fprintf(outFile.fp, "nodeSize = 0x%x\n", initVal);

		VIR_MEM_NODE_t *currVirMemNode;
		LinkedListNode *currNode = info.head;
		for (u32 i = 0; i < info.count; i++) {
			currVirMemNode = (VIR_MEM_NODE_t *)currNode;
			fprintf(outFile.fp, "Node(%d):\n", i+1);
			fprintf(outFile.fp, "[ 0x%X ]\n", (u32)currNode);
			fprintf(outFile.fp, "(u32)currNode->prev = 0x%x\n", (u32)currNode->prev);
			fprintf(outFile.fp, "(u32)currNode->next = 0x%x\n", (u32)currNode->next);
			fprintf(outFile.fp, "currVirMemNode->startAddr = 0x%x\n", currVirMemNode->startAddr);
			fprintf(outFile.fp, "currVirMemNode->usedLen   = 0x%x\n", currVirMemNode->usedLen);

			char buf[100];
			u32 maxLen = currVirMemNode->usedLen;
			if (maxLen % 16 != 0) {
				maxLen = ((maxLen / 16) + 1) * 16;
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
		VIR_MEM_NODE_t *currVirMemNode;
		LinkedListNode *currNode = info.head;
		for (u32 i = 0; i < info.count; i++) {
			currVirMemNode = (VIR_MEM_NODE_t *)currNode;

			char buf[100];
			u32 maxLen = currVirMemNode->usedLen;
			if (maxLen % 16 != 0) {
				maxLen = ((maxLen / 16) + 1) * 16;
			}
			for (u32 j = 0; j < maxLen; j+=16) {
				#define FOR_PRINT(a) (LibString_IsCharPrintable(a)?a:' ')
				sprintf(
					buf,
					"%08x: %02X %02X %02X %02X %02X %02X %02X %02X - %02X %02X %02X %02X %02X %02X %02X %02X |%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c|\n",
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

	return 0;
}

bool VirtualMemClass::NodeExist(u32 start_addr, OUT VIR_MEM_NODE_t **matchNode /* = NULL */)
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

u32 VirtualMemClass::GetPageStartAddr_ByRandomAddr(u32 addr)
{
	return (addr / nodeSize) * nodeSize;
}

int VirtualMemClass::NewNode(u32 start_addr, OUT VIR_MEM_NODE_t **newNode /* = NULL */)
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

int VirtualMemClass::PageWrite(VIR_MEM_NODE_t *node, u32 dstAddr, u8 *src, u32 len, bool doFirstWriteCheck /* = false */)
{
	BASIC_ASSERT(IsThisNodeInList((LinkedListNode *)node) != NODE_ISNT_IN_LIST);
	BASIC_ASSERT(((dstAddr/nodeSize)*nodeSize) == node->startAddr);
	BASIC_ASSERT((dstAddr+len) <= (node->startAddr+nodeSize)); // Cross Page

	u32 nodeAddr = dstAddr - node->startAddr;
	u8 *dst = &(node->data[nodeAddr]);
	
	if (doFirstWriteCheck && initVal < 0x100) {
		for (u32 i=0; i<len; i++) {
			if (dst[i] != (u8)initVal) {
				BASIC_ASSERT(0);
			}
		}
	}
	
	memcpy(dst, src, len);

	if ((len+nodeAddr) > node->usedLen) {
		node->usedLen = (len+nodeAddr);
	}

	return 0;
}

void LibLinkedList_Demo(void)
{
/*
	LinkedListInfo listInfo;
	LibLinkedList_InitInfo(&listInfo);
	LibLinkedList_Length(&listInfo);
	

	LibLinkedList_Dump(&listInfo);

	LinkedListNode newNode1;
	LibLinkedList_PushBack(&listInfo, &newNode1);
	LibLinkedList_Length(&listInfo);
	LibLinkedList_Dump(&listInfo);

	LinkedListNode newNode2;
	LibLinkedList_PushBack(&listInfo, &newNode2);
	LibLinkedList_Length(&listInfo);
	LibLinkedList_Dump(&listInfo);

	LinkedListNode newNode3;
	LibLinkedList_PushBack(&listInfo, &newNode3);
	LibLinkedList_Length(&listInfo);
	LibLinkedList_Dump(&listInfo);
*/
	LinkedListClass linkedList;
	linkedList.InitInfo();
	//linkedList.Dump();

	LinkedListNode newNode1;
	linkedList.PushBack(&newNode1);
	//linkedList.Dump();

	LinkedListNode newNode2;
	linkedList.PushBack(&newNode2);
	//linkedList.Dump();

	LinkedListNode newNode3;
	linkedList.PushBack(&newNode3);
	linkedList.Dump();

	//linkedList.PopBack();
	//linkedList.Dump();
	
	//linkedList.Remove(&newNode3);
	//linkedList.Dump();

	//linkedList.RemoveAll();
	//linkedList.Dump();
	LinkedListNode newNode4;
	linkedList.InsertBack(&newNode4, &newNode4);
	//linkedList.Dump();

	
	VirtualMemClass virMem;
	u8 ary1[] = {0x11, 0x22, 0x33};
	virMem.SetParameters(0x1000, 0xFF);
	virMem.Write(0x00000000, ary1, 3);
	virMem.Write(0x00000010, ary1, 3);
	virMem.Write(0x00000020, ary1, 3);
	virMem.Write(0x00000021, ary1, 3);
	virMem.Write(0x00007FFF, ary1, 3);
	virMem.DumpVirMemNodeInfo();
	virMem.CreateDummyPage(0x2001);
	virMem.DumpVirMemNodeContent_ToFile("LibLinkedListDEMO.txt");
	//todo: CreateNode(); DeleteNode();
}
