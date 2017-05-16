
#include "Everything_Lib_Mgr.hpp"

u16 ISDAP_CRC_H5_Table[] =
{
	0x0000, 0x1081, 0x2102, 0x3183,
	0x4204, 0x5285, 0x6306, 0x7387,
	0x8408, 0x9489, 0xa50a, 0xb58b,
	0xc60c, 0xd68d, 0xe70e, 0xf78f
};

int LibEncrypt_CalcCRC16(u8 *buf, u32 len, OUT u16 *crc16)
{
	u32 j = 0;;
	u16 crc = 0xFFFF;
	u8 value = 0;
	
	for (j = 0; j < len; j++)
	{
		value = buf[j];
		crc = (crc >> 4) ^ ISDAP_CRC_H5_Table[(crc ^ value) & 0x0F];
		crc = (crc >> 4) ^ ISDAP_CRC_H5_Table[(crc ^ (value >> 4)) & 0x0F];
	}
	crc = ((crc & 0xFF00) >> 8) | ((crc & 0x00FF) << 8);
	crc = ((crc & 0xF0F0) >> 4) | ((crc & 0x0F0F) << 4);
	crc = ((crc & 0xCCCC) >> 2) | ((crc & 0x3333) << 2);
	crc = ((crc & 0xAAAA) >> 1) | ((crc & 0x5555) << 1);
	
	*crc16 = crc;
	
	return 0;
}

void LibEncrypt_Demo(void)
{
	u8 buf[] = {0xFA, 0x01};
	u16 crc16;
	LibEncrypt_CalcCRC16((u8 *)buf, 1, &crc16);

	DUMPX(crc16);
}
