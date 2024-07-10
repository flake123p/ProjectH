
#include "Everything_App.hpp"

#define HT "\t"
#define LF "\n"

LibFile_INI gIni_File_Obj;

std::string gOut_File_Header_C;
std::string gOut_File_Source_C;

u32 gStateTotalCount = 0;
std::vector<std::string> gStateNameVector;
u32 gStateNameVector_MaxLen = 0;
std::string gState_Name;
std::string gState_Name_InTypeDefine;
std::string gState_Name_GlobalVarName;
std::string gState_Name_FuncName;
std::string gState_Prefix;
u32 gState_Prefix_Sx_Enable = 0;

int StateMachineMaker_C_HeaderGen(void)
{
	LibFileIoClass outFile(gOut_File_Header_C.c_str(), "w+b");

	outFile.FileOpen();

	// 1. avoid re-define
	LibStringClass tempStr(gOut_File_Header_C.c_str());
	tempStr.ReplaceSubString(".", "_");
	tempStr.ToUpperCase();
	tempStr.str = tempStr.str.insert(0, "_");
	tempStr.str = tempStr.str.insert(tempStr.str.length(), "_");
	
	DUMPS(tempStr.str.c_str());
	
	outFile.FilePrint(
		"\n" \
		"#ifndef %s\n\n",
		tempStr.CStr());

	outFile.FilePrint("typedef enum {\n");
	std::string commentStr;
	std::string stateStr;
	for (u32 i = 0; i < gStateTotalCount; i++) {
		if (i == 0) {
			commentStr = "INIT STATE";
		} else if (i == gStateTotalCount-1) {
			commentStr = "EXIT STATE";
		} else {
			commentStr = "";
		}
		stateStr = gStateNameVector[i];
		//DUMPD(gStateNameVector_MaxLen - stateStr.length());
		stateStr.insert(stateStr.length(), gStateNameVector_MaxLen - stateStr.length(), ' ');
		//DUMPD(stateStr.length());
		outFile.FilePrint("\t%s = %2d, // 0x%02x, %s\n", stateStr.c_str(), i, i, commentStr.c_str());
	}
	outFile.FilePrint("} %s;\n\n", gState_Name_InTypeDefine.c_str());

	outFile.FilePrint("int %s(void);\n\n", gState_Name_FuncName.c_str());

	for (u32 i = 0; i < gStateTotalCount; i++) {
		outFile.FilePrint("int %s_Function(%s &nextState);\n", gStateNameVector[i].c_str(), gState_Name_InTypeDefine.c_str());
	}

	outFile.FilePrint(
		"\n#define %s\n" \
		"#endif //%s\n\n",
		tempStr.CStr(),
		tempStr.CStr());

	return 0;
}

int StateMachineMaker_C_SourceGen(void)
{
	LibFileIoClass outFile(gOut_File_Source_C.c_str(), "w+b");

	outFile.FileOpen();

	outFile.FilePrint(
		"\n" \
		"#include \"%s\"\n" \
		"\n"
		"%s %s = %s;\n\n",
		gOut_File_Header_C.c_str(),
		gState_Name_InTypeDefine.c_str(), gState_Name_GlobalVarName.c_str(), gStateNameVector[0].c_str());

	outFile.FilePrint("int %s(void)\n{\n", gState_Name_FuncName.c_str());

	outFile.FilePrint(
		"\tint retVal;\n" \
		"\twhile (1) {\n" \
		"\t\tswitch (%s) {\n", gState_Name_GlobalVarName.c_str());

	for (u32 i = 0; i < gStateTotalCount; i++) {
		if (i == 0) {
			outFile.FilePrint(
				HT	HT	HT	"case %s: { // INIT STATE" LF \
				HT	HT	HT	HT	"retVal = %s_Function(%s);" LF \
				HT	HT	HT	HT	"if (retVal) {" LF \
				HT	HT	HT	HT	HT	"return retVal;" LF \
				HT	HT	HT	HT	"}" LF \
				HT	HT	HT	"} break;" LF LF,
				gStateNameVector[i].c_str(),
				gStateNameVector[i].c_str(),
				gState_Name_GlobalVarName.c_str());
		} else if (i == gStateTotalCount-1) {
			outFile.FilePrint(
				HT	HT	HT	"case %s: { // EXIT STATE\n" \
				HT	HT	HT	HT	"retVal = %s_Function(%s);\n" \
				HT	HT	HT	HT	"if (retVal) {\n" \
				HT	HT	HT	HT	HT	"return retVal;\n" \
				HT	HT	HT	HT	"}" LF \
				HT	HT	HT	HT	"goto END_OF_WHILE_1;\n" \
				HT	HT	HT	"} break;\n",
				gStateNameVector[i].c_str(),
				gStateNameVector[i].c_str(),
				gState_Name_GlobalVarName.c_str());
		} else {
			outFile.FilePrint(
				HT	HT	HT	"case %s: {\n" \
				HT	HT	HT	HT	"retVal = %s_Function(%s);\n" \
				HT	HT	HT	HT	"if (retVal) {\n" \
				HT	HT	HT	HT	HT	"return retVal;\n" \
				HT	HT	HT	HT	"}" LF \
				HT	HT	HT	"} break;\n\n",
				gStateNameVector[i].c_str(),
				gStateNameVector[i].c_str(),
				gState_Name_GlobalVarName.c_str());
		}
	}

	outFile.FilePrint(
		HT HT "}\n" \
		HT "}\n" \
		"END_OF_WHILE_1:\n" \
		HT ";\n\n" \
		HT "return 0;\n}\n");

	return 0;
}

int StateMachineMaker_C_StateFunctionsGen(void)
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
		outFile.FilePrint(
			"int %s_Function(%s &nextState)\n" \
			"{\n" \
			HT	"return 0; // Error Code, true for error.\n" \
			"}\n\n",
			gStateNameVector[i].c_str(),
			gState_Name_InTypeDefine.c_str());
	}

	return 0;
}

int StateMachineMaker(int argc, char *argv[])
{
	if (argc != 2) {
		printf("ERROR : argc is not 2\n");
		return 1;
	}

	//LibFile_INI gIni_File_Obj(argv[1], "r+b");

	gIni_File_Obj.fileName = argv[1];
	gIni_File_Obj.openMode = "r+b";

	EXIT_CHK( rc, gIni_File_Obj.StartParse() );

	gIni_File_Obj.Dump();

	u32 val;
	
	EXIT_CHK( rc, gIni_File_Obj.GetValueU32("[ATTR]", "State_Prefix_Sx", val) );

	std::string str;
	EXIT_CHK( rc, gIni_File_Obj.GetValueString("[ATTR]", "State_Prefix", str) );
	
	DUMPD(val);
	DUMPX(val);
	DUMPS(str.c_str());

	EXIT_CHK( rc, gIni_File_Obj.GetSingleVarString("[STATES]", 0, str) );
	DUMPS(str.c_str());
	EXIT_CHK( rc, gIni_File_Obj.GetSingleVarString("[STATES]", 1, str) );
	DUMPS(str.c_str());
	EXIT_CHK( rc, gIni_File_Obj.GetSingleVarString("[STATES]", 2, str) );
	DUMPS(str.c_str());

	EXIT_CHK( rc, gIni_File_Obj.GetSingleVarStringTotalCount("[STATES]", val) );
	gStateTotalCount = val;
	
	EXIT_CHK( rc, gIni_File_Obj.GetValueString("[ATTR]", "State_Name", gState_Name) );
	
	LibStringClass tempStr(gState_Name.c_str());
	tempStr.ToUpperCase();
	gState_Name_InTypeDefine = tempStr.str + "_t";

	gState_Name_GlobalVarName = "g" + gState_Name;

	gState_Name_FuncName = gState_Name + "_StateMachine";
	
	EXIT_CHK( rc, gIni_File_Obj.GetValueString("[ATTR]", "State_Prefix", gState_Prefix) );
	EXIT_CHK( rc, gIni_File_Obj.GetValueU32("[ATTR]", "State_Prefix_Sx", val) );
	gState_Prefix_Sx_Enable = val;
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
			EXIT_CHK( rc, gIni_File_Obj.GetSingleVarString("[STATES]", i, stateStr) );
			stateStr = gState_Prefix + sxStr + stateStr;
			gStateNameVector.push_back(stateStr);
			LibUtil_MaxMinMgr_Input((s32)stateStr.length());
		}
	}
	gStateNameVector_MaxLen = (u32)LibUtil_MaxMinMgr_GetMax();
	DUMPD(gStateNameVector_MaxLen);
	EXIT_CHK( rc, gIni_File_Obj.GetValueString("[ATTR]", "Out_File_Header_C", gOut_File_Header_C) );
	EXIT_CHK( rc, gIni_File_Obj.GetValueString("[ATTR]", "Out_File_Source_C", gOut_File_Source_C) );
	
	EXIT_CHK( rc, StateMachineMaker_C_HeaderGen() );
	EXIT_CHK( rc, StateMachineMaker_C_SourceGen() );
	EXIT_CHK( rc, StateMachineMaker_C_StateFunctionsGen() );

	return 0;
}

