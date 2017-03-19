

#ifndef __LIB_LINKED_LIST_HPP_INCLUDED_

#include "My_Types.h"

// ============================== Debug ==============================
#define LINKED_LIST_LOG  (0)
#define LINKED_LIST_WARN (0)
#define LINKED_LIST_ERR  (0)

#if LINKED_LIST_LOG
#define LINKED_LIST_LOG_MSG printf
#else
#define LINKED_LIST_LOG_MSG(...)
#endif

#if LINKED_LIST_WARN
#define LINKED_LIST_WARN_MSG printf
#else
#define LINKED_LIST_WARN_MSG(...)
#endif

#if LINKED_LIST_ERR
#define LINKED_LIST_ERR_MSG printf
#else
#define LINKED_LIST_ERR_MSG(...)
#endif


typedef struct LinkedListNode {
	LinkedListNode *next;
	LinkedListNode *prev;
} LinkedListNode;

typedef struct LinkedListInfo {
	LinkedListNode *head;
	LinkedListNode *tail;
	u32 count;
} LinkedListInfo;

#define NODE_IS_HEAD(node) ((node)->prev==NULL)
#define NODE_IS_TAIL(node) ((node)->next==NULL)
#define NODE_IS_ONLY_ONE(node) (NODE_IS_HEAD(node)&&NODE_IS_TAIL(node))

//Error Code
#define NODE_ISNT_IN_LIST     (1)


typedef enum {
	DO_NOT_AUTO_FREE,
	DO_AUTO_FREE,
}AUTO_FREE;
void LibLinkedList_InitInfo(LinkedListInfo *info);
u32 LibLinkedList_Length(LinkedListInfo *info);
void LibLinkedList_PushBack(LinkedListInfo *info, LinkedListNode *newNode);
void LibLinkedList_PopBack(LinkedListInfo *info, AUTO_FREE autoFree = DO_NOT_AUTO_FREE);
int LibLinkedList_Remove(LinkedListInfo *info, LinkedListNode *nodeForRemove, AUTO_FREE autoFree = DO_NOT_AUTO_FREE);
void LibLinkedList_RemoveAll(LinkedListInfo *info, AUTO_FREE autoFree = DO_NOT_AUTO_FREE);
int LibLinkedList_InsertBack(LinkedListInfo *info, LinkedListNode *nodeBeInserted, LinkedListNode *nodeForInsert);
int LibLinkedList_InsertFront(LinkedListInfo *info, LinkedListNode *nodeBeInserted, LinkedListNode *nodeForInsert);
int LibLinkedList_IsThisNodeInList(LinkedListInfo *info, LinkedListNode *nodeForCheck);
void LibLinkedList_Dump(LinkedListInfo *info);
void LibLinkedList_Diagnose(LinkedListInfo *info, const char *caller);


class LinkedListClass{
public:
	LinkedListInfo info;
	
	LinkedListClass(){InitInfo();}; // Do nothing.
	~LinkedListClass(void){}; // Do nothing.

	void InitInfo(void);
	u32  Length(void);
	void PushBack(LinkedListNode *newNode);
	void PopBack(AUTO_FREE autoFree = DO_NOT_AUTO_FREE);
	int  Remove(LinkedListNode *nodeForRemove, AUTO_FREE autoFree = DO_NOT_AUTO_FREE);
	void RemoveAll(AUTO_FREE autoFree = DO_NOT_AUTO_FREE);
	int  InsertBack(LinkedListNode *nodeBeInserted, LinkedListNode *nodeForInsert);
	int  InsertFront(LinkedListNode *nodeBeInserted, LinkedListNode *nodeForInsert);
	int  IsThisNodeInList(LinkedListNode *nodeForCheck);
	void Dump(void);
	void Diagnose(const char *caller);
};


typedef struct {
	LinkedListNode node;
	u32 startAddr;
	u32 usedLen;
	void *DummyPad;
	u8 data[];
}VIR_MEM_NODE_t;
class VirtualMemClass: public LinkedListClass{
private:
	bool NodeExist(u32 start_addr, OUT VIR_MEM_NODE_t **matchNode = NULL);
	int  NewNode(u32 start_addr, OUT VIR_MEM_NODE_t **newNode = NULL);
	int  PageWrite(VIR_MEM_NODE_t *node, u32 dstAddr, u8 *src, u32 len);
	//bool IsAddressInAnyNode(u32 addr, OUT VIR_MEM_NODE_t **matchNode = NULL);
	//int NewNode(u32 addr, OUT VIR_MEM_NODE_t **newNode = NULL);
public:
	u32 nodeSize;  // Must be multiple of 16
	u32 initVal; // if bigger than 0xFF, won't init array from malloc()
	
	VirtualMemClass(void);
	~VirtualMemClass(void);
	void SetParameters(u32 node_size, u32 initVal);
	int  Write(u32 dstAddr, u8 *src, u32 len);
	void DumpVirMemNodeInfo(void);
	int  DumpVirMemNodeContent_ToFile(const char *fileName, bool memDumpMode = false);
	//u8 ReadByte(u32 addr); // call VirAddrToNodeAddr. If addr doesn't exist, just return 0.
	//bool VirAddrToNodeAddr(u32 addr, OUT u32 *bufMaxAccessLen = NULL, OUT VIR_MEM_NODE_t** matchNode = NULL);
	//void DumpVirMemNodeInfo(void);
	//int  DumpVirMemNodeContent_ToFile(const char *fileName);
};

void LibLinkedList_Demo(void);

#define __LIB_LINKED_LIST_HPP_INCLUDED_
#endif//__LIB_LINKED_LIST_HPP_INCLUDED_

