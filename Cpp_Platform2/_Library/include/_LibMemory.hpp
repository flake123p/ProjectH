

#ifndef __LIB_MEMORY_HPP_INCLUDED_

#include "My_Types.h"

// ============================== Debug ==============================

// ============================== Define ==============================
int LibMemory_SetBit(u8 *buf, u32 bit, u32 val);
int LibMemory_GetBit(u8 *buf, u32 bit, OUT u32 *val);

#define PAGE_START_ADDR(addr,pageSize)  (((addr)/(pageSize))*(pageSize))
#define PAGE_START_ADDR2(addr,pageSize) ((addr) & (~((pageSize)-1)))

int  LibMemory_CalculateCrossPageInfo(u32 dstAddr, u32 totalLen, u32 pageStartAddr, u32 pageSize, OUT u32 *newLen);
void LibMemory_CalculateCrossPageInfo_TestCase(u32 dstAddr, u32 totalLen, u32 pageSize);

#include "_LibLinkedList.hpp" //LinkedListClass
// Page Attributes
#define MEMORY_WRITE_PROTECT_FLAG      BIT_0
#define MEMORY_READ_PROTECT_FLAG       BIT_1
// Error Code
// 0x01 ~ 0x3F: for LinkedListClass
// 0x40 ~ 0x7F: for VirtualMemClass
#define MEM_ERROR_WRITE_PROTECTED (0x40)
#define MEM_ERROR_READ_PROTECTED  (0x41)
#define MEM_ERROR_PAGE_NOT_EXIST  (0x50)
typedef struct {
	LinkedListNode node;
	u32 startAddr;
	u32 usedLen;
	u32 pageAttr;
	void *DummyPad;
	u8 data[];
}VIR_MEM_NODE_t;
class VirtualMemClass: public LinkedListClass{
private:
	bool _NodeExist(u32 start_addr, OUT VIR_MEM_NODE_t **matchNode = NULL);
	u32  _CalculateNodeStartAddr(u32 addr);
	int  _CreateNode(u32 start_addr, OUT VIR_MEM_NODE_t **newNode = NULL);
	int  _PageWrite(VIR_MEM_NODE_t *node, u32 dstAddr, u8 *src, u32 len);
	int  _PageRead(VIR_MEM_NODE_t *node, u8 *dst, u32 srcAddr, u32 len);
public:
	u32 nodeSize;  // Must be multiple of 16
	u32 initVal; // if bigger than 0xFF, won't init array from malloc()
	u32 initPageAttr;
	
	VirtualMemClass(void);
	~VirtualMemClass(void);
	void SetParameters(u32 node_size, u32 initVal, u32 init_page_attr = 0);
	int  Write(u32 dstAddr, u8 *src, u32 len, bool autoCreatePage = true);
	int  Read(u8 *dst, u32 srcAddr, u32 len);
	int  ClearPage(u32 start_addr);
	u8  *GetRealAddress(u32 addr, VIR_MEM_NODE_t **matchNode = NULL);
	int  CreateEmptyPage(u32 dstAddr, VIR_MEM_NODE_t **matchNode = NULL);
	bool GetPage(u32 start_addr, OUT VIR_MEM_NODE_t **matchNode);
	void DumpVirMemInfo(void);
	int  DumpVirMemContent_ToFile(const char *fileName, bool verbose = false, bool memDumpMode = false, bool dumpAllData = false);
	int  ExportVirMemContent(const char *outFileName);
	int  ImportVirMemContent(const char *inFileName);
};


// Byte Attributes
#define MEMROY_WAS_WRITTEN_FLAG        BIT_0
// Error Code
// 0x01 ~ 0x3F: for LinkedListClass
// 0x40 ~ 0x7F: for VirtualMemClass
// 0x80 ~ 0xCF: for ProtectedMemClass
#define MEM_ERROR_BYTE_ALREADY_WRITTEN (0x80)

class ProtectedMemClass{
public:
	VirtualMemClass virMem;
	VirtualMemClass attrMem;

	ProtectedMemClass(void);
	~ProtectedMemClass(void);
	void SetParameters(u32 node_size, u32 initVal, u32 pageAttr = 0);
	int  InitMemory(u32 startAddr, u32 size);
	int  Write(u32 dstAddr, u8 *src, u32 len, bool doWriteOnceCheck = false);
	int  Read(u8 *dst, u32 srcAddr, u32 len);
	int  AddPageAttributes(u32 addr, u32 flags, OUT u32 *attr = NULL);
	int  RemovePageAttributes(u32 addr, u32 flags, OUT u32 *attr = NULL);
	int  ClearByteWrittenFlag(u32 startAddr, u32 len);
	int  DumpProtectedMemContent_ToFile(const char *fileName1, const char *fileName2, bool memDumpMode = false, bool dumpAllData = false);
	int  ExportProtectedMemContent(const char *outFileName);
	int  ImportProtectedMemContent(const char *inFileName);

};

void LibMemory_Demo(void);
void LibMemory_Demo2(void);
void LibMemory_Demo3(void);
void LibMemory_Demo4(void);

#define __LIB_MEMORY_HPP_INCLUDED_
#endif//__LIB_MEMORY_HPP_INCLUDED_

