
#include "Everything_App.hpp"

char testString1[] = "{03 04 04 FF 07 04 01 FF}>>> 01 7d fc [??] [00 20 02 00] 80";
char testString2[] = "{03 04 04 FF 07 04 01 FF}>>> 01 7d fc ?? [00 20 02 80] 80";

void abc(char ch)
{
    char str[] = "abcdXYZabcdXYZabcdXYZabcdXYZ";
	str[0] = ch;
	printf("%s\n", str);
}
int main(int argc, char *argv[])
{
	#if 0
	//LibTime_StartClock();
	//LibUartMgr_DemoTxRx();
	//LibTime_StopClock_ShowResult();
	LibTime_StartClock();
	LibTime_StartMicroSecondClock();
	//LibOs_SleepMiliSeconds(500);
	//LibOs_SleepSeconds(1);

	LibTuiMgr_DemoSimple();
	printf("press any key to next run\n");
	printf("press any key to next run\n");
	LibSysCmd_Pause();
	LibTuiMgr_DemoSimple();

	LibTime_StopClock_ShowResult();
	printf("\n");
	LibTime_StopMicroSecondClock_ShowResult();
	printf("\n");


	//DUMPD(LibUtil_GetRand());
	//LibUtil_Print_RAND_MAX();
	//Demo_Output_A_File_Cpp_Lite();
	#else
		#if 0
		u8 ch = 0x6F;
		int i = CHECK_BIT_FIELD(ch, BIT_0|BIT_1, SIZE_2, OFFSET_5);
		DUMPD(i);
		i = CLEAR_BIT_FIELD(ch, SIZE_2, OFFSET_5);
		DUMPD(i);
		i = SET_BIT_FIELD(ch, BIT_0|BIT_1, SIZE_2, OFFSET_6);
		DUMPX(i);
		ch = 0x80;
		i = GET_MSB(ch, u8);
		DUMPX(i);
		i = BIT_0|BIT_1;
		DUMPX(i);
		i = BIT_0|BIT_1<<1;
		DUMPX(i);
		#endif
	#endif

	//LibTuiMgr_DemoDumpAttr();
	//_LibMenu_Demo();
	//LibBuffer_DemoBasic();
#if 0
	testString1[0] = 'x';
	DUMPS(testString1);

	LibStringClass obj;
	char abc[] = "AB? C D [??] !! 11 ?? ??";
	DUMPD(strlen(abc));
	obj.Init(abc);
	DUMPD(obj.Length());
	DUMPD(obj.Size());
	DUMPD(obj.FindChar('?'));
	DUMPD(obj.FindChar('x'));
	DUMPD(obj.FindString("??"));
	DUMPD(obj.FindString("!!"));
	obj.ReplaceWithRestLength("??");
	DUMPS(obj.str.c_str());
	//LibThreadMgr_Demo();
#endif
	//Lib51Hex_Demo();
	//LibLinkedList_Demo();
	abc('X');
	abc('Y');

    LibBT_HCI_Demo();
	//LibScreenLogic_Demo();
	return 0;//LibSharedMemory_Demo_Server();
	//return LibSharedMemory_Demo_Client();
}
