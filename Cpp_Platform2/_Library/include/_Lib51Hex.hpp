

#ifndef __LIB_51HEX_HPP_INCLUDED_

// ============================== Debug ==============================

// ============================== Define ==============================
//Error Code: Hex format error
#define COLON_NOT_AT_START     (1)
#define RECORD_LENGTH_TO_SMALL (2)
#define RECORD_LENGTH_TO_BIG   (3)
#define RECORD_LENGTH_ERROR    (4)
#define DATA_LENGTH_ERROR      (5)
#define ILLEGAL_RECORD_TYPE    (6)
#define CHECK_SUM_ERROR        (7)



//Hex record position --> :llaaaatt[dd...]cc
#define COLON_POS        (0)
#define LENGTH_0_OF_DATA (1)
#define LENGTH_1_OF_DATA (2)
#define START_ADDRESS_0  (3)
#define START_ADDRESS_1  (4)
#define START_ADDRESS_2  (5)
#define START_ADDRESS_3  (6)
#define RECORD_TYPE_0    (7)
#define RECORD_TYPE_1    (8)
#define DATA_FIELD       (9)

//Record Type
#define DATA_RECORD           (0)
#define EOF_RECORD            (1)
#define EXT_SEG_ADDR_RECORD   (2)
#define EXT_LIN_ADDR_RECORD   (4)
#define START_LIN_ADDR_RECORD (5)

typedef struct {
	u8 *dataBuf;
	int dataLen;
	int addr16btis;
	int checkSum;
	int recordType;
}HEX_RECORD_DATA_t;

u32  Lib51Hex_MakeDataRecord(u32 start_addr, char *hex_string, OUT u8 *hex_data);
void Lib51Hex_SprintDataRecord(OUT char *buf, u8 *hex_data, u32 hex_data_len);
void Lib51Hex_PrintDataRecord(u8 *hex_data, u32 hex_data_len);

int  Lib51Hex_StringToRecord(const char *s, OUT HEX_RECORD_DATA_t *record);
void Lib51Hex_DumpRecord(HEX_RECORD_DATA_t *record);
void Lib51Hex_DumpHexFile(const char *fileName);

#include "_LibLinkedList.hpp" // for VirtualMemClass
class Lib51HexReader{
public:
	u32 GlobalAddress;
	HEX_RECORD_DATA_t workingRecord;
	u8 *workingReadOutString;
	VirtualMemClass virMem;
	
	Lib51HexReader(u32 node_size = 0x8000, u32 init_val = 0x100);
	~Lib51HexReader(void);
	int ReadFile(const char *file_name);
};

int Lib51Hex_Demo(void);

#define __LIB_51HEX_HPP_INCLUDED_
#endif//__LIB_51HEX_HPP_INCLUDED_

