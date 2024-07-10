
#include "Everything_App.hpp"

#include "BH_Converter.hpp"

LibFile_INI gIniFile = LibFile_INI("ini\\BusHound.ini", "r+b");
LibFileIoClass gInputFile;
LibFileIoClass gOutputFile;
LibStringClass gCurrStr;
u32 gPacketRemainLen = 0;

int BHC_S0_Init_Function(BH_CONVERTER_t *nextState)
{
	EXIT_CHK( rc, gIniFile.StartParse() );
	
	EXIT_CHK( rc, gIniFile.GetValueString("[ATTR]", "InputFile", gInputFile.fileName) );

	//DUMPS(gInputFile.fileName.c_str());
	gInputFile.openMode = "r+b";

	EXIT_CHK( rc, gIniFile.GetValueString("[ATTR]", "OutputFile", gOutputFile.fileName) );
	gOutputFile.openMode = "w+b";

	gInputFile.FileOpenForRead(2000);
	gOutputFile.FileOpen();

	*nextState = BHC_S1_Cut_Useless_Header;
	return 0;
}

int BHC_S1_Cut_Useless_Header_Function(BH_CONVERTER_t *nextState)
{
	while (0 == gInputFile.GetLine()) {
		//DUMPS(gInputFile.lineStr);
		if (0 == strncmp(gInputFile.lineStr, "------", 6))
			break;
	}

	*nextState = BHC_S2_Judge_Is_First_Or_Continue;
	return 0;
}

int BHC_S2_Judge_Is_First_Or_Continue_Function(BH_CONVERTER_t *nextState)
{
	int retVal = gInputFile.GetLine();

	switch (retVal) {
		case 0: {
			gCurrStr = gInputFile.lineStr;
			gCurrStr.Split();

			if (gPacketRemainLen != 0) {
				*nextState = BHC_S4_Parsing_Continue_Line_Section;
				break;
			}
			
			if (gCurrStr.subStrVector.size() < 3) {
				*nextState = BHC_S5_Exit;
			} else {
				if (gCurrStr.subStrVector[2] == "OUT" || gCurrStr.subStrVector[2] == "IN") {
					*nextState = BHC_S3_Parsing_First_Line_Section;
				} else {
					*nextState = BHC_S5_Exit;
				}
			}
		} break;

		case RC_BUFFER_FULL: {
			BASIC_ASSERT(0);
		} break;

		case RC_FILE_REACH_EOF: {
			*nextState = BHC_S5_Exit;
		} break;

		default:
			BASIC_ASSERT(0);
			break;
	}

	return 0;
}

int BHC_S3_Parsing_First_Line_Section_Function(BH_CONVERTER_t *nextState)
{
	sscanf(gCurrStr.subStrVector[1].c_str(), "%u", &gPacketRemainLen);
	DUMPD(gPacketRemainLen);
	DUMPS(gCurrStr.subStrVector[2].c_str());

	gOutputFile.FilePrint("[ %3s ] [ %4d ] ", gCurrStr.subStrVector[2].c_str(), gPacketRemainLen);
	for (u32 i = 0; i < gPacketRemainLen; i++) {
		gOutputFile.FilePrint("%s ", gCurrStr.subStrVector[3+i].c_str());
		if (i == 31) {
			gPacketRemainLen -= 32;
			*nextState = BHC_S2_Judge_Is_First_Or_Continue;
			return 0;
		}
	}
	gPacketRemainLen = 0;
	gOutputFile.FilePrint("\n");
	*nextState = BHC_S2_Judge_Is_First_Or_Continue;
	return 0;
}

int BHC_S4_Parsing_Continue_Line_Section_Function(BH_CONVERTER_t *nextState)
{
	for (u32 i = 0; i < gPacketRemainLen; i++) {
		gOutputFile.FilePrint("%s ", gCurrStr.subStrVector[i].c_str());// It's [i], not [3+i].
		if (i == 31) {
			gPacketRemainLen -= 32;
			*nextState = BHC_S2_Judge_Is_First_Or_Continue;
			return 0;
		}
	}
	gPacketRemainLen = 0;
	gOutputFile.FilePrint("\n");
	*nextState = BHC_S2_Judge_Is_First_Or_Continue;
	return 0;
}

int BHC_S5_Exit_Function(BH_CONVERTER_t *nextState)
{
	return 0;
}

