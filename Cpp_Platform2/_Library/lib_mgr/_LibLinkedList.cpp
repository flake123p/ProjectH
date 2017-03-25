
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
	DUMPP(info->head);
	DUMPP(info->tail);
	LinkedListNode *currNode = info->head;
	for (u32 i = 0; i < info->count; i++) {
		printf("Node(%d):\n", i+1);
		printf("[ %p ]\n", currNode);
		DUMPP(currNode->prev);
		DUMPP(currNode->next);
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

}

