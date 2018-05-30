
#include "Everything_Lib_Mgr.hpp"

u16 ISDAP_CRC_H5_Table[] =
{
	0x0000, 0x1081, 0x2102, 0x3183,
	0x4204, 0x5285, 0x6306, 0x7387,
	0x8408, 0x9489, 0xa50a, 0xb58b,
	0xc60c, 0xd68d, 0xe70e, 0xf78f
};

u16 LibEncrypt_CalculateCRC16(u8 *buf, u32 len)
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
	
	return crc;
}

/*
	Uses : Bisync, Modbus, USB, ANSI X3.28, SIA DC-07, many others; also known as CRC-16 and CRC-16-ANSI
	polyRep = Polynomial representations
		Normal:   0x8005
		Reversed: 0xA001
		Reversed reciprocal: 0xC002
	Ref:
		http://mcommit.hatenadiary.com/entry/2015/04/08/224244
		http://naeilproj.blogspot.tw/2015/07/linux-cmodbus-rtu-crc16.html
*/
u16 LibEncrypt_CalculateCRC16_IBM(u8 *buf, u32 len, u16 polyRep)
{
	u16 crc = 0xFFFF;
	u32 i,j;
	u8 carrayFlag;
	
	for (i = 0; i < len; i++) {
		crc ^= buf[i];
		for (j = 0; j < 8; j++) {
			carrayFlag = crc & 1;
			crc = crc >> 1;
			if (carrayFlag) {
				crc ^= polyRep;
			}
		}
	}
	
	return crc;
}

void LibEncrypt_Demo(void)
{
	//u8 buf[] = {0x44, 0x80, 0x04, 0x00};
	//u8 buf[] = {0x02, 0x20, 0x02, 0x82};
	//u8 buf[] = {0xF0, 0xFF, 0x1F, 0x00, 0x01, 0x00, 0x00, 0x00};
	u8 buf[] = {0x00, 0x00, 0x10, 0x00, 0x00, 0x00, 0x10, 0x00};
	
	u16 crc16;
	crc16 = LibEncrypt_CalculateCRC16((u8 *)buf, GET_ARRAY_LEN(buf));
	DUMPX(crc16);

	crc16 = LibEncrypt_CalculateCRC16_IBM((u8 *)buf, GET_ARRAY_LEN(buf), 0xC002);
	DUMPX(crc16);
}
