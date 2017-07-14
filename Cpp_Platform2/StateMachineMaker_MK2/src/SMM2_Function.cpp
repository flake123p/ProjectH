
#include "Everything_App.hpp"

#define HT "\t"
#define LF "\n"

#include "SMM2.hpp"

LibFile_INI gIni_File_Obj;

std::string gOut_File_Header_C;
std::string gOut_File_Source_C;

std::string gState_Name;
std::string gState_Name_InTypeDefine;
std::string gState_Name_GlobalVarName;
#define STR_STATE_MACHINE "_StateMachine"
#define STR_STATE_CHANGE  "_StateChange"
#define STR_FUNCTION      "_Function"
#define STR_SENT_EVENT    "_SendEvent"
#define STR_EVENT_PARA    "_EvtPara_t"
#define STR_ERROR_CODE    "_EC_t"
//std::string gState_Name_FuncName;
std::string gState_Prefix;
u32 gState_Prefix_Sx_Enable = 0;

u32 gStateTotalCount = 0;
std::vector<std::string> gStateNameVector;
u32 gStateNameString_MaxLen_ForAlign = 0;

std::string gEvent_Name;
std::string gEvent_Name_InTypeDefine;
//std::string gEvent_Name_GlobalVarName;
//std::string gEvent_Name_FuncName;
std::string gEvent_Prefix;
u32 gEvent_Prefix_Sx_Enable = 0;

u32 gEventTotalCount = 0;
std::vector<std::string> gEventNameVector;
u32 gEventNameString_MaxLen_ForAlign = 0;

std::vector<std::string> gSE_Table_Vector;
std::vector<std::string> gIncludes_Vector;
std::vector<std::string> gStates_Vector;
std::vector<std::string> gEvents_Vector;

std::string gError_Code_Prefix;

int SMM2_IniFile_ExtProc_CB(LibStringClass &str)
{
	static bool is_SE_TABLE_section = false;
	static bool is_INCLUDE_section = false;

	if (str.str == "<SE_TABLE>") {
		is_SE_TABLE_section = true;
		return 1;
	}

	if (str.str == "</SE_TABLE>") {
		is_SE_TABLE_section = false;
		return 1;
	}

	if (is_SE_TABLE_section) {
		if (str.str.length() == 0)
			return 1;

		gSE_Table_Vector.push_back(str.str);
		//DUMPS(str.str.c_str());
		return 1;
	}

	if (str.str == "<INCLUDE>") {
		is_INCLUDE_section = true;
		return 1;
	}

	if (str.str == "</INCLUDE>") {
		is_INCLUDE_section = false;
		return 1;
	}

	if (is_INCLUDE_section) {
		if (str.str.length() == 0)
			return 1;

		gIncludes_Vector.push_back(str.str);
		return 1;
	}

	return 0;
}

int SMM2_S0_Read_INI_Function(SMM2_t &nextState)
{
	gIni_File_Obj.fileName = "__STATES__.ini";
	gIni_File_Obj.openMode = "r+b";
	EXIT_CHK( rc, gIni_File_Obj.StartParse(true, SMM2_IniFile_ExtProc_CB) );
	gIni_File_Obj.Dump();

	EXIT_CHK( rc, gIni_File_Obj.GetValueString("[ATTR]", "Out_File_Header_C", gOut_File_Header_C) );
	EXIT_CHK( rc, gIni_File_Obj.GetValueString("[ATTR]", "Out_File_Source_C", gOut_File_Source_C) );

	EXIT_CHK( rc, gIni_File_Obj.GetValueString("[ATTR]", "State_Name", gState_Name) );
	LibStringClass tempStr(gState_Name.c_str());
	tempStr.ToUpperCase();
	gState_Name_InTypeDefine = tempStr.str + "_t";
	gState_Name_GlobalVarName = "g" + gState_Name;
	//gState_Name_FuncName = gState_Name + "_StateMachine";

	EXIT_CHK( rc, gIni_File_Obj.GetValueString("[ATTR]", "State_Prefix", gState_Prefix) );
	u32 val;
	EXIT_CHK( rc, gIni_File_Obj.GetValueU32("[ATTR]", "State_Prefix_Sx", val) );
	gState_Prefix_Sx_Enable = val;

	EXIT_CHK( rc, gIni_File_Obj.GetValueString("[ATTR]", "Event_Name", gEvent_Name) );
	//LibStringClass tempStr(gEvent_Name.c_str());
	tempStr.Init(gEvent_Name.c_str());
	tempStr.ToUpperCase();
	gEvent_Name_InTypeDefine = tempStr.str + "_t";
	//gEvent_Name_GlobalVarName = "g" + gEvent_Name;
	//gEvent_Name_FuncName = gEvent_Name + "_StateMachine";

	EXIT_CHK( rc, gIni_File_Obj.GetValueString("[ATTR]", "Event_Prefix", gEvent_Prefix) );
	//u32 val;
	EXIT_CHK( rc, gIni_File_Obj.GetValueU32("[ATTR]", "Event_Prefix_Ex", val) );
	gEvent_Prefix_Sx_Enable = val;

	EXIT_CHK( rc, gIni_File_Obj.GetValueString("[ATTR]", "Error_Code_Prefix", gError_Code_Prefix) );

	nextState = SMM2_S1_Extrace_States_From_Table;
	return 0; // Error Code, true for error.
}

int SMM2_S1_Extrace_States_From_Table_Function(SMM2_t &nextState)
{
	LibStringClass str(gSE_Table_Vector[0].c_str());
	str.Split();
	if (str.subStrVector[0] != "S\\E") {
		LibError_PrintExtErrorMessage("1st element in 1st row should be \"E\\S\".\n");
		EXIT_LOC_IF(1);
	}

	for (u32 i = 1; i < gSE_Table_Vector.size(); i++) {
		str.Init(gSE_Table_Vector[i].c_str());
		str.Split();
		
		gStates_Vector.push_back(str.subStrVector[0]);
		//DUMPS(str.subStrVector[0].c_str());
	}
/*
	LibStringClass str(gSE_Table_Vector[0].c_str());
	str.Split();
	for (u32 i = 1; i < str.subStrVector.size(); i++) {
		gStates_Vector.push_back(str.subStrVector[i]);
		//DUMPS(str.subStrVector[i].c_str());
	}
*/
	gStateTotalCount = gStates_Vector.size();
	
	LibUtil_MaxMinMgr_Init();
	if (gState_Prefix_Sx_Enable) {
		std::string stateStr;
		std::string sxStr;
		char buffer [50];
		for (u32 i = 0; i < gStateTotalCount; i++) {
			if (gState_Prefix_Sx_Enable) {
				sprintf (buffer, "%d", i);
				sxStr = "S";
				sxStr += buffer;
				sxStr += "_";
			}
			stateStr = gStates_Vector[i];
			stateStr = gState_Prefix + sxStr + stateStr;
			gStateNameVector.push_back(stateStr);
			LibUtil_MaxMinMgr_Input((s32)stateStr.length());
		}
	}
	gStateNameString_MaxLen_ForAlign = (u32)LibUtil_MaxMinMgr_GetMax();
	//DUMPD(gStateNameVector_MaxLen);

	nextState = SMM2_S2_Extrace_Events_From_Table;
	return 0; // Error Code, true for error.
}

int SMM2_S2_Extrace_Events_From_Table_Function(SMM2_t &nextState)
{
	LibStringClass str(gSE_Table_Vector[0].c_str());
	str.Split();
	for (u32 i = 1; i < str.subStrVector.size(); i++) {
		gEvents_Vector.push_back(str.subStrVector[i]);
		//DUMPS(str.subStrVector[i].c_str());
	}
/*
	LibStringClass str;
	
	for (u32 i = 1; i < gSE_Table_Vector.size(); i++) {
		str.Init(gSE_Table_Vector[i].c_str());
		str.Split();
		
		gEvents_Vector.push_back(str.subStrVector[0]);
		DUMPS(str.subStrVector[0].c_str());
	}
*/
	gEventTotalCount = gEvents_Vector.size();
	
	LibUtil_MaxMinMgr_Init();
	if (gEvent_Prefix_Sx_Enable) {
		std::string eventStr;
		std::string exStr;
		char buffer [50];
		for (u32 i = 0; i < gEventTotalCount; i++) {
			if (gEvent_Prefix_Sx_Enable) {
				sprintf (buffer, "%d", i);
				exStr = "E";
				exStr += buffer;
				exStr += "_";
			}
			eventStr = gEvents_Vector[i];
			eventStr = gEvent_Prefix + exStr + eventStr;
			gEventNameVector.push_back(eventStr);
			LibUtil_MaxMinMgr_Input((s32)eventStr.length());
		}
	}
	gEventNameString_MaxLen_ForAlign = (u32)LibUtil_MaxMinMgr_GetMax();

	nextState = SMM2_S3_Make_Header_File;
	return 0; // Error Code, true for error.
}

int SMM2_S3_Make_Header_File_Function(SMM2_t &nextState)
{
	LibFileIoClass outFile(gOut_File_Header_C.c_str(), "w+b");

	outFile.FileOpen();

	// 1. avoid re-define
	LibStringClass tempStr(gOut_File_Header_C.c_str());
	tempStr.ReplaceSubString(".", "_");
	tempStr.ToUpperCase();
	tempStr.str = tempStr.str.insert(0, "_");
	tempStr.str = tempStr.str.insert(tempStr.str.length(), "_");
	
	//DUMPS(tempStr.str.c_str());
	
	outFile.FilePrint(
		"\n" \
		"#ifndef %s\n\n",
		tempStr.CStr());

	// STD Library
	outFile.FilePrint(
		"#include <stdlib.h>" LF);
	// Customized Library
	for (u32 i = 0; i < gIncludes_Vector.size(); i++) {
		outFile.FilePrint("%s\n", gIncludes_Vector[i].c_str());
	}
	outFile.FilePrint("\n");

	// typedef EVENT
	outFile.FilePrint("typedef enum {\n");
	//std::string commentStr;
	std::string eventStr;
	for (u32 i = 0; i < gEventTotalCount; i++) {
		eventStr = gEventNameVector[i];
		//DUMPD(gStateNameVector_MaxLen - stateStr.length());
		eventStr.insert(eventStr.length(), gEventNameString_MaxLen_ForAlign - eventStr.length(), ' ');
		//DUMPD(stateStr.length());
		outFile.FilePrint("\t%s = %2d, // 0x%02x\n", eventStr.c_str(), i, i);
	}
	outFile.FilePrint("} %s;\n\n", gEvent_Name_InTypeDefine.c_str());

	// typedef STATE
	outFile.FilePrint("typedef enum {\n");
	std::string commentStr;
	std::string stateStr;
	for (u32 i = 0; i < gStateTotalCount; i++) {
		if (i == 0) {
			commentStr = "INIT STATE";
		} else if (i == gStateTotalCount-1) {
			commentStr = "";//"EXIT STATE";
		} else {
			commentStr = "";
		}
		stateStr = gStateNameVector[i];
		//DUMPD(gStateNameVector_MaxLen - stateStr.length());
		stateStr.insert(stateStr.length(), gStateNameString_MaxLen_ForAlign - stateStr.length(), ' ');
		//DUMPD(stateStr.length());
		outFile.FilePrint("\t%s = %2d, // 0x%02x, %s\n", stateStr.c_str(), i, i, commentStr.c_str());
	}
	outFile.FilePrint("} %s;\n\n", gState_Name_InTypeDefine.c_str());

	// typedef EVENT_PARA
	outFile.FilePrint(
		"typedef struct {" LF \
		HT "int dummy;" LF \
		"} %s%s;" LF LF,
		gEvent_Name.c_str(),
		STR_EVENT_PARA);

	// typedef ERROR CODE
	outFile.FilePrint("//ERROR CODE\n");
	std::string errorCodeStr;
	u32 errorCodeTotalCount;
	gIni_File_Obj.GetSingleVarStringTotalCount("[ERROR_CODE]", errorCodeTotalCount);
	LibUtil_MaxMinMgr_Init();
	for (u32 i = 0; i < errorCodeTotalCount; i++) {
		gIni_File_Obj.GetSingleVarString("[ERROR_CODE]", i, errorCodeStr);
		LibUtil_MaxMinMgr_Input((s32)errorCodeStr.length());
	}
	u32 maxLen_ForAlign = (u32)LibUtil_MaxMinMgr_GetMax();
	for (u32 i = 0; i < errorCodeTotalCount; i++) {
		gIni_File_Obj.GetSingleVarString("[ERROR_CODE]", i, errorCodeStr);
		errorCodeStr.insert(errorCodeStr.length(), maxLen_ForAlign - errorCodeStr.length(), ' ');
		//DUMPD(stateStr.length());
		outFile.FilePrint("#define %s%s  ( %2d ), // 0x%02x\n", gError_Code_Prefix.c_str(), errorCodeStr.c_str(), i, i);
	}
	outFile.FilePrint("\n");//, gState_Name.c_str(), STR_ERROR_CODE);


	
	// State change function
	outFile.FilePrint(
		"int %s%s(%s nextState, %s%s *evtPara = NULL);" LF LF,
		gState_Name.c_str(),
		STR_STATE_CHANGE,
		gState_Name_InTypeDefine.c_str(),
		gEvent_Name.c_str(),
		STR_EVENT_PARA);

	// Core source file
	outFile.FilePrint("int %s%s(%s event, %s%s *evtPara = NULL);" LF LF,
		gState_Name.c_str(),
		STR_STATE_MACHINE,
		gEvent_Name_InTypeDefine.c_str(),
		gEvent_Name.c_str(),
		STR_EVENT_PARA);

	// state functions
	for (u32 i = 0; i < gStateTotalCount; i++) {
		#if 0
		outFile.FilePrint("int %s%s(%s &nextState, %s%s &evtPara = NULL);\n",
			gStateNameVector[i].c_str(),
			STR_FUNCTION,
			gState_Name_InTypeDefine.c_str(),
			gEvent_Name.c_str(),
			STR_EVENT_PARA);
		#else
		outFile.FilePrint("int %s%s(%s%s *evtPara = NULL);\n",
			gStateNameVector[i].c_str(),
			STR_FUNCTION,
			gEvent_Name.c_str(),
			STR_EVENT_PARA);
		#endif
	}

	// Event functions (Special Events)
	#if 0
	outFile.FilePrint(LF);
	LibStringClass str;
	for (u32 i = 1; i < gSE_Table_Vector.size(); i++) {
		str.Init(gSE_Table_Vector[i].c_str());
		str.Split();
		//DUMPS(str.subStrVector[0].c_str());
		//DUMPS(str.subStrVector[1].c_str());
		if (str.subStrVector.size() >= 2 && str.subStrVector[1] == "E") {
			outFile.FilePrint("int %s_Function(%s &state, %s%s &evtPara = NULL);\n",
				gEventNameVector[i-1].c_str(),
				gEvent_Name_InTypeDefine.c_str(),
				gEvent_Name.c_str(),
				STR_EVENT_PARA);
		}
	}
	#else
	outFile.FilePrint(LF);
	LibStringClass str(gSE_Table_Vector[1].c_str());
	str.Split();
	for (u32 i = 1; i < str.subStrVector.size(); i++) {
		if (str.subStrVector[i] == "E") {
			outFile.FilePrint("int %s_Function(%s &state, %s%s *evtPara = NULL);\n",
				gEventNameVector[i-1].c_str(),
				gState_Name_InTypeDefine.c_str(),
				gEvent_Name.c_str(),
				STR_EVENT_PARA);
		}
	}
	#endif

	outFile.FilePrint(
		LF \
		"#define %s" LF \
		"#endif //%s" LF LF,
		tempStr.CStr(),
		tempStr.CStr());

	nextState = SMM2_S4_Make_Function_File;
	return 0; // Error Code, true for error.
}

int SMM2_S4_Make_Function_File_Function(SMM2_t &nextState)
{
	LibStringClass functionsFileStr(gOut_File_Source_C.c_str());

	functionsFileStr.ReplaceSubString(".c", "_Function.cpp");

	LibFileIoClass outFile(functionsFileStr.CStr(), "w+b");

	outFile.FileOpen();

	outFile.FilePrint(
		"\n" \
		"#include \"%s\"\n\n",
		gOut_File_Header_C.c_str());

	for (u32 i = 0; i < gStateTotalCount; i++) {
		#if 0
		outFile.FilePrint(
			"int %s%s(%s &nextState, %s%s &evtPara /* = NULL */)\n" \
			"{\n" \
			HT	"return 0; // Error Code, true for error.\n" \
			"}\n\n",
			gStateNameVector[i].c_str(),
			STR_FUNCTION,
			gState_Name_InTypeDefine.c_str(),
			gEvent_Name.c_str(),
			STR_EVENT_PARA);
		#else
		outFile.FilePrint(
			"int %s%s(%s%s *evtPara /* = NULL */)\n" \
			"{\n" \
			HT	"return 0; // Error Code, true for error.\n" \
			"}\n\n",
			gStateNameVector[i].c_str(),
			STR_FUNCTION,
			gEvent_Name.c_str(),
			STR_EVENT_PARA);
		#endif
	}

	// Event functions
	#if 0
	LibStringClass str;
	for (u32 i = 1; i < gSE_Table_Vector.size(); i++) {
		str.Init(gSE_Table_Vector[i].c_str());
		str.Split();
		//DUMPS(str.subStrVector[0].c_str());
		//DUMPS(str.subStrVector[1].c_str());
		if (str.subStrVector.size() >= 2 && str.subStrVector[1] == "E") {
			outFile.FilePrint(
				"int %s%s(%s &state, %s%s *evtPara /* = NULL */)" LF \
				"{" LF,
				gEventNameVector[i-1].c_str(),
				STR_FUNCTION,
				gEvent_Name_InTypeDefine.c_str(),
				gEvent_Name.c_str(),
				STR_EVENT_PARA);

			outFile.FilePrint(HT "switch (state) {" LF);
			for (u32 i = 0; i < gStateNameVector.size(); i++) {
				outFile.FilePrint(HT HT "case %s:" LF, gStateNameVector[i].c_str());
			}
			outFile.FilePrint(
				HT HT "default:" LF \
				HT HT HT "break;" LF \
				HT "}" LF \
				LF \
				HT "return 0; // Error Code, true for error." LF \
				"}" LF);
		}
	}
	#else
	LibStringClass str(gSE_Table_Vector[1].c_str());
	str.Split();
	for (u32 i = 1; i < str.subStrVector.size(); i++) {
		if (str.subStrVector[i] == "E") {
			outFile.FilePrint(
				"int %s%s(%s &state, %s%s *evtPara /* = NULL */)" LF \
				"{" LF,
				gEventNameVector[i-1].c_str(),
				STR_FUNCTION,
				gState_Name_InTypeDefine.c_str(),
				gEvent_Name.c_str(),
				STR_EVENT_PARA);

			outFile.FilePrint(HT "switch (state) {" LF);
			for (u32 i = 0; i < gStateNameVector.size(); i++) {
				outFile.FilePrint(HT HT "case %s:" LF, gStateNameVector[i].c_str());
			}
			outFile.FilePrint(
				HT HT "default:" LF \
				HT HT HT "break;" LF \
				HT "}" LF \
				LF \
				HT "return 0; // Error Code, true for error." LF \
				"}" LF);
		}
	}
	#endif

	nextState = SMM2_S5_Make_Source_File_Start;
	return 0; // Error Code, true for error.
}

LibFileIoClass gOutSourceFile;
int SMM2_S5_Make_Source_File_Start_Function(SMM2_t &nextState)
{
	gOutSourceFile.fileName = gOut_File_Source_C;
	gOutSourceFile.openMode = "w+b";
	gOutSourceFile.FileOpen();

	gOutSourceFile.FilePrint(
		"\n" \
		"#include \"%s\"\n" \
		"\n"
		"%s  %s = %s;\n\n",
		gOut_File_Header_C.c_str(),
		gState_Name_InTypeDefine.c_str(), gState_Name_GlobalVarName.c_str(), gStateNameVector[0].c_str());

	// State change function
	gOutSourceFile.FilePrint(
		"int %s%s(%s nextState, %s%s *evtPara /* = NULL */)" LF \
		"{" LF \
		HT "switch (nextState) {" LF,
		gState_Name.c_str(),
		STR_STATE_CHANGE,
		gState_Name_InTypeDefine.c_str(),
		gEvent_Name.c_str(),
		STR_EVENT_PARA);
	for (u32 i = 0; i < gStateNameVector.size(); i++) {
		gOutSourceFile.FilePrint(
			HT HT "case %s:" LF \
			HT HT HT "%s = nextState;" LF \
			HT HT HT "return %s%s(evtPara);" LF LF,
			gStateNameVector[i].c_str(),
			gState_Name_GlobalVarName.c_str(),
			gStateNameVector[i].c_str(),
			STR_FUNCTION);
	}
	gOutSourceFile.FilePrint(
		HT HT "default:" LF \
		HT HT HT "return -1;" LF \
		HT "}" LF\
		"}" LF LF);

						
	gOutSourceFile.FilePrint("int %s%s(%s event, %s%s *evtPara /* = NULL */)\n{\n",
		gState_Name.c_str(),
		STR_STATE_MACHINE,
		gEvent_Name_InTypeDefine.c_str(),
		gEvent_Name.c_str(),
		STR_EVENT_PARA);

	//gOutSourceFile.FilePrint(HT "int retVal;" LF LF);

	gOutSourceFile.FilePrint(
		HT "int retVal;" LF LF);
//		"\tswitch (%s) {\n", gState_Name_GlobalVarName.c_str());
/*
	std::string commentStr;
	for (u32 i = 0; i < gStateTotalCount; i++) {
		if (i == 0) {
			commentStr = " // INIT STATE";
		} else if (i == gStateTotalCount-1) {
			commentStr = " // EXIT STATE";
		} else {
			commentStr = "";
		}
		gOutSourceFile.FilePrint(
			HT	HT	"case %s: {%s\n" \
			HT	HT	HT	"retVal = %s_Function(%s);\n" \
			HT	HT	HT	"if (retVal) {\n" \
			HT	HT	HT	HT	"return retVal;\n" \
			HT	HT	HT	"}" LF \
			HT	HT	"} break;\n\n",
			gStateNameVector[i].c_str(),
			commentStr.c_str(),
			gStateNameVector[i].c_str(),
			gState_Name_GlobalVarName.c_str());
	}

	gOutSourceFile.FilePrint(
		HT "}\n\n" \
		HT "return 0;\n}\n");
*/
	nextState = SMM2_S6_Make_Source_File_Special_Event;
	return 0; // Error Code, true for error.
}

int SMM2_S6_Make_Source_File_Special_Event_Function(SMM2_t &nextState)
{
	bool switchHeaderPrinted = false;
	
	LibStringClass str(gSE_Table_Vector[1].c_str());
	str.Split();
	for (u32 i = 1; i < str.subStrVector.size(); i++) {
		if (str.subStrVector[i] == "E") {
			if (false == switchHeaderPrinted) {
				switchHeaderPrinted = true;

				gOutSourceFile.FilePrint(
					HT "switch (event) {" LF);
			}

			gOutSourceFile.FilePrint(HT HT "case %s:" LF, gEventNameVector[i-1].c_str());

			gOutSourceFile.FilePrint(HT HT HT "return %s_Function(%s, evtPara);" LF,
				gEventNameVector[i-1].c_str(),
				gState_Name_GlobalVarName.c_str());
		}
	}

	if (switchHeaderPrinted) {
		gOutSourceFile.FilePrint(
			HT HT "default:" LF \
			HT HT HT "break;" LF \
			HT "}" LF LF);
	}

	nextState = SMM2_S7_Make_Source_File_SE_SwitchCase;
	return 0; // Error Code, true for error.
}

int SMM2_S7_Make_Source_File_SE_SwitchCase_Function(SMM2_t &nextState)
{
	gOutSourceFile.FilePrint(
		HT "bool repeat;" LF \
		HT "do {" LF \
		HT HT "repeat = false;" LF\
		LF \
		HT HT "switch (%s) {" LF,
		gState_Name_GlobalVarName.c_str());

	for (u32 i = 0; i < gStateNameVector.size(); i++) {
		gOutSourceFile.FilePrint(HT HT HT "case %s: {" LF, gStateNameVector[i].c_str());

		gOutSourceFile.FilePrint(HT HT HT HT "switch (event) {" LF);
		LibStringClass str(gSE_Table_Vector[i+1].c_str());
		str.Split();
		LibStringClass currCell;
		std::string nextState;
		for (u32 j = 0; j < gEventNameVector.size(); j++) {
			//u32 subStrIndex = j + 1;
			if (j+1 >= str.subStrVector.size())
				currCell.Init("N");
			else
				currCell.Init(str.subStrVector[j+1].c_str());
			gOutSourceFile.FilePrint(HT HT HT HT HT "case %s: {" LF, gEventNameVector[j].c_str());
			if (currCell.str == "N") {
				//do nothing
			} else {
				currCell.InsertBefore(",", " ");
				currCell.InsertAfter(",", " ");
				currCell.Split();
				if (currCell.subStrVector.size() == 3) {
					// REPEAT
					if (currCell.subStrVector[0] == "R") {
						gOutSourceFile.FilePrint(HT HT HT HT HT HT "repeat = true;" LF);
					}
					// NEXT STATE
					s32 findIndex;
					if (currCell.subStrVector[0] == "R" || currCell.subStrVector[0] == "D") {
						findIndex = LibUtil_FindInContainer_ReturnIndex<std::vector<std::string>, std::string>(gStates_Vector, currCell.subStrVector[2]);
						if (findIndex < 0) {
							printf("Illegal next state(%s) cell: %s\n", currCell.subStrVector[2].c_str(), currCell.CStr());
							EXIT_LOC_IF(findIndex);
						}
					}
					//gOutSourceFile.FilePrint(HT HT HT HT HT HT "%s = %s;" LF, gState_Name_GlobalVarName.c_str(), gStateNameVector[findIndex].c_str());
					gOutSourceFile.FilePrint(
						HT HT HT HT HT HT "retVal = %s%s(%s, evtPara);" LF \
						HT HT HT HT HT HT "if (retVal) {" LF \
						HT HT HT HT HT HT HT "return retVal;" LF \
						HT HT HT HT HT HT "}" LF,
						gState_Name.c_str(),
						STR_STATE_CHANGE,
						gStateNameVector[findIndex].c_str());
				}
			}
			//gOutSourceFile.FilePrint("%s" LF, currCell.CStr());
			gOutSourceFile.FilePrint(HT HT HT HT HT "} break;" LF LF);
		}
		gOutSourceFile.FilePrint(
			HT HT HT HT HT "default: " LF \
			HT HT HT HT HT HT "break;" LF \
			HT HT HT HT "}" LF);

		gOutSourceFile.FilePrint(HT HT HT "} break;" LF LF);
	}

	gOutSourceFile.FilePrint(
		HT HT HT "default:" LF \
		HT HT HT HT "break;" LF \
		HT HT "}" LF);

	nextState = SMM2_S8_Make_Source_File_End;
	return 0; // Error Code, true for error.
}

int SMM2_S8_Make_Source_File_End_Function(SMM2_t &nextState)
{
	gOutSourceFile.FilePrint(
		HT "} while (repeat);" LF \
		LF \
		HT "return retVal;" LF \
		"}" LF);

	return 1; // Error Code, true for error.
}

int SMM2_S9_Exit_Function(SMM2_t &nextState)
{
	return 0; // Error Code, true for error.
}

