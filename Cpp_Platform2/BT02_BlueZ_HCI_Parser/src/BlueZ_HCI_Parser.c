
#include "Everything_App.hpp"

extern void hci_dump(int level, struct frame *frm);

int BlueZ_HCI_Parser(void)
{
	u8 testAry[] = {0x01, 0x03, 0x0c, 0x00};

	struct frame testFrame;

	testFrame.ptr = (uint8_t *)testAry;
	testFrame.len = sizeof(testAry);

	hci_dump(0,&testFrame);
	
	return 111;
}
