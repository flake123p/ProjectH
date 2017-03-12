
#include "Everything_App.hpp"

#define HEX_DATA_HEADER_SIZE (4)
#define HEX_DATA_SIZE        (255)
#define HEX_DATA_TAILER_SIZE (1) /* 8bits check sum*/
#define HEX_RECORD_SIZE (HEX_DATA_HEADER_SIZE+HEX_DATA_SIZE+HEX_DATA_TAILER_SIZE)
#define DATA_PTR(a) (&(a[HEX_DATA_HEADER_SIZE]))

char gHexString[] = "01 01";
u8  gHexData[HEX_RECORD_SIZE];
u32 gHexDataLen;
char gHexString_2[] = "AA BB";
u8  gHexData_2[HEX_RECORD_SIZE];
u32 gHexDataLen_2;

u32 MakeHexRecord(u32 start_addr, char *hex_string, OUT u8 *hex_data) 
{
	u32 dataLen = LibString_HexStringToCharString(hex_string, DATA_PTR(hex_data), HEX_DATA_SIZE);
	hex_data[0] = (u8)dataLen;
	hex_data[1] = (u8)(start_addr>>8);
	hex_data[2] = (u8)start_addr;
	hex_data[3] = 0;
	hex_data[dataLen+HEX_DATA_HEADER_SIZE] = LibString_GetCheckSumU8(hex_data, dataLen+HEX_DATA_HEADER_SIZE);
	
	return dataLen + HEX_DATA_HEADER_SIZE + HEX_DATA_TAILER_SIZE;
}

void PrintHexRecord(u8 *hex_data, u32 hex_data_len) 
{
	printf(":");
	for (u32 i=0; i<hex_data_len; i++) {
		printf("%02X", hex_data[i]);
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	gHexDataLen = MakeHexRecord(0x0100, gHexString, gHexData);
	PrintHexRecord(gHexData, gHexDataLen);
	
	gHexDataLen_2 = MakeHexRecord(0x2233, gHexString_2, gHexData_2);
	PrintHexRecord(gHexData_2, gHexDataLen_2);
	

	return 0;
}
