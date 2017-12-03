
#include "Everything_App.hpp"

#define HT "\t"
#define LF "\n"

#include "SMM3.hpp"

#define STR_EVENT_PARA    "_EvtPara_t"

LibStringClass gTempLibStr;
std::string gTempStr;
u32 gTempU32;

LibFile_INI gS0_Ini_File_Obj;
typedef struct {
	std::string str;
	std::string rawStr;
	u32 lineNumber;
}RawStr_Para_t;
std::vector<RawStr_Para_t> gCB_SE_TABLE_RawVector;
std::vector<RawStr_Para_t> gCB_INTER_STATE_RawVector;
std::vector<RawStr_Para_t> gCB_STATE_FUNC_RawVector;
std::vector<RawStr_Para_t> gCB_GLOBAL_EVENT_RawVector;
std::vector<RawStr_Para_t> gCB_TEST_CASE_RawVector;
std::vector<RawStr_Para_t> gCB_INCLUDE_FRONT_RawVector;
std::vector<RawStr_Para_t> gCB_INCLUDE_BACK_RawVector;
std::vector<RawStr_Para_t> gCB_EVENT_PARA_RawVector;
std::vector<RawStr_Para_t> gCB_ASSERT_RawVector;
std::string gS0_Out_File_Header_C;
std::string gS0_Out_File_Source_C;
std::string gS0_State_Name;
//std::string gS0_State_Name_InTypeDefine;
//std::string gS0_State_Name_GlobalVarName;
std::string gS0_State_Prefix;
u32 gS0_State_Prefix_Sx_Enable = 0;
std::string gS0_Event_Name;
//std::string gS0_Event_Name_InTypeDefine;
std::string gS0_Event_Prefix;
u32 gS0_Event_Prefix_Sx_Enable = 0;

//std::string gStrTempForPrint;
//LibStringClass gLibStrTempForPrint;
const char* _STR_HeaderFileName(void)
{
	return gS0_Out_File_Header_C.c_str();
}
const char* _STR_SourceFileName(void)
{
	return gS0_Out_File_Source_C.c_str();
}
const char *_STR_FuncFileName(void)
{
	static LibStringClass libStr;
	
	libStr.Init(gS0_Out_File_Source_C.c_str());

	libStr.ReplaceSubString(".c", "_Function.cpp");

	return libStr.CStr();
}
const char *_STR_TestCaseFileName(void)
{
	static LibStringClass libStr;
	
	libStr.Init(gS0_Out_File_Source_C.c_str());

	libStr.ReplaceSubString(".c", "_TestCase.cpp");

	return libStr.CStr();
}
const char *_STR_StateTypedefName(void)
{
	static std::string str;
	static LibStringClass libStr;
	
	libStr.Init(gS0_State_Name.c_str());
	libStr.ToUpperCase();
	str = libStr.str + "_t";

	return str.c_str();
}
const char *_STR_StateGlobalVarName(void)
{
	static bool inited = false;

	static std::string str;

	if (NOT(inited)) {
		str = "g" + gS0_State_Name;
		inited = true;
	}
	return str.c_str();
}
const char *_STR_StateMachineFuncName(void)
{
	static std::string str;
	
	str = gS0_State_Name + "_StateMachine";
	
	return str.c_str();
}
const char *_STR_StateChangeFuncName(void)
{
	static std::string str;
	
	str = gS0_State_Name + "_StateChange";
	
	return str.c_str();
}
const char *_STR_EventTypedefName(void)
{
	static std::string str;
	static LibStringClass libStr;
	
	libStr.Init(gS0_Event_Name.c_str());
	libStr.ToUpperCase();
	str = libStr.str + "_t";

	return str.c_str();
}
const char *_STR_EventParaType(void)
{
	static std::string str;
	str = gS0_Event_Name + STR_EVENT_PARA;
	return str.c_str();
}

int SMM3_IniFile_ExtProc_CB(LibStringClass &str, LibStringClass &rawStr, u32 lineNumber)
{
	static bool is_SE_TABLE_section = false;
	static bool is_INTER_STATE_section = false;
	static bool is_STATE_FUNC_section = false;
	static bool is_GLOBAL_EVENT_section = false;
	static bool is_TEST_CASE_section = false;
	static bool is_INCLUDE_FRONT_section = false;
	static bool is_INCLUDE_BACK_section = false;
	static bool is_EVENT_PARA_section = false;
	static bool is_ASSERT_section = false;

	RawStr_Para_t rawStrPara;
	rawStrPara.str = str.str;
	rawStrPara.rawStr = rawStr.str;
	rawStrPara.lineNumber = lineNumber;

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

		gCB_SE_TABLE_RawVector.push_back(rawStrPara);
		//DUMPS(str.str.c_str());
		return 1;
	}

	if (str.str == "<INTER_STATE>") {
		is_INTER_STATE_section = true;
		return 1;
	}
	if (str.str == "</INTER_STATE>") {
		is_INTER_STATE_section = false;
		return 1;
	}
	if (is_INTER_STATE_section) {
		if (str.str.length() == 0)
			return 1;

		gCB_INTER_STATE_RawVector.push_back(rawStrPara);
		return 1;
	}

	if (str.str == "<STATE_FUNC>") {
		is_STATE_FUNC_section = true;
		return 1;
	}
	if (str.str == "</STATE_FUNC>") {
		is_STATE_FUNC_section = false;
		return 1;
	}
	if (is_STATE_FUNC_section) {
		if (str.str.length() == 0)
			return 1;

		gCB_STATE_FUNC_RawVector.push_back(rawStrPara);
		return 1;
	}

	if (str.str == "<GLOBAL_EVENT>") {
		is_GLOBAL_EVENT_section = true;
		return 1;
	}
	if (str.str == "</GLOBAL_EVENT>") {
		is_GLOBAL_EVENT_section = false;
		return 1;
	}
	if (is_GLOBAL_EVENT_section) {
		if (str.str.length() == 0)
			return 1;

		gCB_GLOBAL_EVENT_RawVector.push_back(rawStrPara);
		return 1;
	}

	if (str.str == "<TEST_CASE>") {
		is_TEST_CASE_section = true;
		return 1;
	}
	if (str.str == "</TEST_CASE>") {
		is_TEST_CASE_section = false;
		return 1;
	}
	if (is_TEST_CASE_section) {
		if (str.str.length() == 0)
			return 1;

		gCB_TEST_CASE_RawVector.push_back(rawStrPara);
		return 1;
	}

	if (str.str == "<INCLUDE_FRONT>") {
		is_INCLUDE_FRONT_section = true;
		return 1;
	}
	if (str.str == "</INCLUDE_FRONT>") {
		is_INCLUDE_FRONT_section = false;
		return 1;
	}
	if (is_INCLUDE_FRONT_section) {
		if (str.str.length() == 0)
			return 1;

		gCB_INCLUDE_FRONT_RawVector.push_back(rawStrPara);
		return 1;
	}

	if (str.str == "<INCLUDE_BACK>") {
		is_INCLUDE_BACK_section = true;
		return 1;
	}
	if (str.str == "</INCLUDE_BACK>") {
		is_INCLUDE_BACK_section = false;
		return 1;
	}
	if (is_INCLUDE_BACK_section) {
		if (str.str.length() == 0)
			return 1;

		gCB_INCLUDE_BACK_RawVector.push_back(rawStrPara);
		return 1;
	}

	if (str.str == "<EVENT_PARA>") {
		is_EVENT_PARA_section = true;
		return 1;
	}
	if (str.str == "</EVENT_PARA>") {
		is_EVENT_PARA_section = false;
		return 1;
	}
	if (is_EVENT_PARA_section) {
		if (str.str.length() == 0)
			return 1;

		gCB_EVENT_PARA_RawVector.push_back(rawStrPara);
		return 1;
	}

	if (str.str == "<ASSERT>") {
		is_ASSERT_section = true;
		return 1;
	}
	if (str.str == "</ASSERT>") {
		is_ASSERT_section = false;
		return 1;
	}
	if (is_ASSERT_section) {
		if (str.str.length() == 0)
			return 1;

		gCB_ASSERT_RawVector.push_back(rawStrPara);
		return 1;
	}

	return 0;
}

int SMM3_S0_Read_All_INI_Data_Function(SMM3_t &nextState)
{
	gS0_Ini_File_Obj.fileName = "__STATES__.ini";
	gS0_Ini_File_Obj.openMode = "r+b";
	EXIT_CHK( rc, gS0_Ini_File_Obj.StartParse2(true, SMM3_IniFile_ExtProc_CB) );
	gS0_Ini_File_Obj.Dump();

	EXIT_CHK( rc, gS0_Ini_File_Obj.GetValueString("[ATTR]", "Out_File_Header_C", gS0_Out_File_Header_C) );
	EXIT_CHK( rc, gS0_Ini_File_Obj.GetValueString("[ATTR]", "Out_File_Source_C", gS0_Out_File_Source_C) );

	EXIT_CHK( rc, gS0_Ini_File_Obj.GetValueString("[ATTR]", "State_Name", gS0_State_Name) );

	EXIT_CHK( rc, gS0_Ini_File_Obj.GetValueString("[ATTR]", "State_Prefix", gS0_State_Prefix) );
	EXIT_CHK( rc, gS0_Ini_File_Obj.GetValueU32("[ATTR]", "State_Prefix_Sx", gTempU32) );
	gS0_State_Prefix_Sx_Enable = gTempU32;

	EXIT_CHK( rc, gS0_Ini_File_Obj.GetValueString("[ATTR]", "Event_Name", gS0_Event_Name) );

	EXIT_CHK( rc, gS0_Ini_File_Obj.GetValueString("[ATTR]", "Event_Prefix", gS0_Event_Prefix) );
	EXIT_CHK( rc, gS0_Ini_File_Obj.GetValueU32("[ATTR]", "Event_Prefix_Ex", gTempU32) );
	gS0_Event_Prefix_Sx_Enable = gTempU32;

	#if 1
	printf("File:\n");
	printf("Header   File: %s\n", _STR_HeaderFileName());
	printf("Source   File: %s\n", _STR_SourceFileName());
	printf("Function File: %s\n", _STR_FuncFileName());
	printf("TestCase File: %s\n", _STR_TestCaseFileName());

	printf("\nState:\n");
	printf("Typedef    Name: %s\n", _STR_StateTypedefName());
	printf("Global Var Name: %s\n", _STR_StateGlobalVarName());
	printf("FSM Func   Name: %s\n", _STR_StateMachineFuncName());

	printf("\nEvent:\n");
	printf("Typedef    Name: %s\n", _STR_EventTypedefName());
	#endif

	LibError_SetExtErrorMessage("");
	
	nextState = SMM3_S1_Extract_States;
	return 0; // Error Code, true for error.
}

typedef enum {
	SimpleState,
	IntermediateState,
}State_Type_t;
typedef struct {
	State_Type_t stateType;
	std::string stateStr;
	std::string prefixStr;
	std::string alignStr;
}State_Para_t;
std::vector<State_Para_t> gS1_StateVector;
u32 gS1_StateTotalCount = 0;
u32 _StateStringToIndex(std::string &inStr)
{
	for (u32 index = 0; index < gS1_StateTotalCount; index++) {
		if (inStr == gS1_StateVector[index].stateStr)
			return index;
	}

	return 0xFFFFFFFF;
}
static void _SMM3_S1_Fill_State_Prefix_n_Align_Strings(void)
{
	LibUtil_MaxMinMgr_Init();
	for (u32 i = 0; i < gS1_StateVector.size(); i++) {
		gS1_StateVector[i].prefixStr = gS0_State_Prefix;
		if (gS0_State_Prefix_Sx_Enable) {
			char tempBuf[10];
			sprintf(tempBuf, "S%d_", i);
			gS1_StateVector[i].prefixStr = gS1_StateVector[i].prefixStr + tempBuf;
			//DUMPS(gS2_EventVector[i].prefixStr.c_str());
		}
		LibUtil_MaxMinMgr_Input((s32)gS1_StateVector[i].stateStr.length());
	}

	u32 maxLen_ForAlign = (u32)LibUtil_MaxMinMgr_GetMax();
	DUMPD(maxLen_ForAlign);
	for (u32 i = 0; i < gS1_StateVector.size(); i++) {
		gS1_StateVector[i].alignStr = "";
		gS1_StateVector[i].alignStr.insert(0, maxLen_ForAlign - gS1_StateVector[i].stateStr.length(), ' ');
		//printf(">>> %s%s%s\n", gS1_StateVector[i].alignStr.c_str(), gS1_StateVector[i].prefixStr.c_str(), gS1_StateVector[i].stateStr.c_str());
	}
}
int SMM3_S1_Extract_States_Function(SMM3_t &nextState)
{
	gS1_StateVector.clear();

	for (u32 i = 0; i < gCB_SE_TABLE_RawVector.size(); i++) {
		switch (i) {
			case 0:
				if (0 != strncmp(gCB_SE_TABLE_RawVector[i].str.c_str(), "S\\E", 3)) {
					printf("SE_TABLE should be start with \"S\\E\", error in line:%d\n", gCB_SE_TABLE_RawVector[i].lineNumber);
				}
				break;

			default:
				if (gCB_SE_TABLE_RawVector[i].str[0] == '-') {
					//ignore
				} else {
					State_Para_t statePara;
					LibStringClass libStr(gCB_SE_TABLE_RawVector[i].str.c_str());
					libStr.Split();
					switch (libStr.subStrVector[0][0]) {
						case 'S':
							statePara.stateType = SimpleState;
							break;
						case 'I':
							statePara.stateType = IntermediateState;
							break;
						default:
							printf("State type should be \"S\" or \"I\", error in line:%d\n", gCB_SE_TABLE_RawVector[i].lineNumber);
							break;
					}
					libStr.subStrVector[0].erase(0, 2);
					statePara.stateStr = libStr.subStrVector[0];
					gS1_StateVector.push_back(statePara);
					printf("\t%d, %s\n", statePara.stateType, statePara.stateStr.c_str());
				}
				break;
		}
	}

	gS1_StateTotalCount = gS1_StateVector.size();
	DUMPD(gS1_StateTotalCount);

	_SMM3_S1_Fill_State_Prefix_n_Align_Strings();

	nextState = SMM3_S2_Extract_Events;
	return 0; // Error Code, true for error.
}

typedef enum {
	SimpleEvent,
	GlobalEvent,
}Event_Type_t;
typedef struct {
	Event_Type_t eventType;
	std::string eventStr;
	std::string prefixStr;
	std::string alignStr;
	std::string globalEventFunc;
}Event_Para_t;
std::vector<Event_Para_t> gS2_EventVector;
u32 gS2_SimpleEventTotalCount = 0;
int SMM3_S2_Extract_Events_Function(SMM3_t &nextState)
{
	gS2_EventVector.clear();

	LibStringClass libStr(gCB_SE_TABLE_RawVector[0].str.c_str());
	libStr.Split();
	
	for (u32 i = 1; i < libStr.subStrVector.size(); i++) {
		Event_Para_t eventPara;
		eventPara.eventType = SimpleEvent;
		eventPara.eventStr = libStr.subStrVector[i];
		gS2_EventVector.push_back(eventPara);
		printf("\t%d, %s\n", eventPara.eventType, eventPara.eventStr.c_str());
	}

	gS2_SimpleEventTotalCount = gS2_EventVector.size();
	DUMPD(gS2_SimpleEventTotalCount);

	nextState = SMM3_S3_Extract_GlobalEvents;
	return 0; // Error Code, true for error.
}

static void _SMM3_S3_Fill_Event_Prefix_n_Align_Strings(void)
{
	// Fill prefix & align strings
	//std::string prefixStr;
	//std::string alignStr;
	LibUtil_MaxMinMgr_Init();
	for (u32 i = 0; i < gS2_EventVector.size(); i++) {
		gS2_EventVector[i].prefixStr = gS0_Event_Prefix;
		if (gS0_Event_Prefix_Sx_Enable) {
			char tempBuf[10];
			sprintf(tempBuf, "E%d_", i);
			gS2_EventVector[i].prefixStr = gS2_EventVector[i].prefixStr + tempBuf;
			//DUMPS(gS2_EventVector[i].prefixStr.c_str());
		}
		LibUtil_MaxMinMgr_Input((s32)gS2_EventVector[i].eventStr.length());
	}

	u32 maxLen_ForAlign = (u32)LibUtil_MaxMinMgr_GetMax();
	DUMPD(maxLen_ForAlign);
	for (u32 i = 0; i < gS2_EventVector.size(); i++) {
		gS2_EventVector[i].alignStr = "";
		gS2_EventVector[i].alignStr.insert(0, maxLen_ForAlign - gS2_EventVector[i].eventStr.length(), ' ');
		//printf(">>> %s%s%s\n", gS2_EventVector[i].alignStr.c_str(), gS2_EventVector[i].prefixStr.c_str(), gS2_EventVector[i].eventStr.c_str());
	}
}

u32 gS3_GlobalEventTotalCount = 0;
int SMM3_S3_Extract_GlobalEvents_Function(SMM3_t &nextState)
{
	for (u32 i = 0; i < gCB_GLOBAL_EVENT_RawVector.size(); i++) {
		gTempLibStr.Init(gCB_GLOBAL_EVENT_RawVector[i].str);
		gTempLibStr.Split();
		
		Event_Para_t eventPara;
		eventPara.eventType = GlobalEvent;
		eventPara.eventStr = gTempLibStr.subStrVector[0];
		eventPara.globalEventFunc = "NULL";
		if (gTempLibStr.subStrVector.size() == 2) {
			eventPara.globalEventFunc = gTempLibStr.subStrVector[1];
		}
		gS2_EventVector.push_back(eventPara);
		printf("\t%d, %s\n", eventPara.eventType, eventPara.eventStr.c_str());
	}

	gS3_GlobalEventTotalCount = gS2_EventVector.size() - gS2_SimpleEventTotalCount;
	DUMPD(gS3_GlobalEventTotalCount);

	_SMM3_S3_Fill_Event_Prefix_n_Align_Strings();

	nextState = SMM3_S4_Make_SE_Table;
	return 0; // Error Code, true for error.
}

typedef enum {
	DirectPass,
	RepeatPass,
	NullPass,
}Event_Pass_Type_t;
typedef struct {
	Event_Pass_Type_t eventPassType;
	u32 nextState;
}SE_Cell_t;
typedef struct {
	std::vector<SE_Cell_t> row;
}SE_Row_t;
std::vector<SE_Row_t> gS4_SE_Table;
const char *_EventTypeToString(Event_Pass_Type_t eventPassType)
{
	switch (eventPassType) {
		case DirectPass:
			return "D";
		case RepeatPass:
			return "R";
		case NullPass:
			return "N";
		default:
			return "ERROR";
	}
}
u32 _StateStringLen_WithPrefix_WithAlign(void)
{
	static std::string str;
	str= gS1_StateVector[0].prefixStr + gS1_StateVector[0].stateStr + gS1_StateVector[0].alignStr;

	u32 len = str.length();
	return len;
}
const char *_StateIndexToString(u32 nextState)
{
	if(nextState < gS1_StateVector.size())
		return gS1_StateVector[nextState].stateStr.c_str();
	else
		return "NULL";
}
const char *_StateIndexToString_WithPrefix(u32 nextState, const char *notFoundString)
{
	if(nextState >= gS1_StateVector.size())
		return notFoundString;
		
	static std::string str;
	str = gS1_StateVector[nextState].prefixStr + gS1_StateVector[nextState].stateStr;
	return str.c_str();
}
const char *_StateIndexToString_WithPrefix_WithAlign(u32 nextState, const char *notFoundString)
{
	static std::string str;

	if(nextState >= gS1_StateVector.size()) {
		u32 alignLength = _StateStringLen_WithPrefix_WithAlign() - strlen(notFoundString);
		str = notFoundString;
		str.insert(strlen(notFoundString), alignLength, ' ');
	} else {
		str = gS1_StateVector[nextState].prefixStr + gS1_StateVector[nextState].stateStr + gS1_StateVector[nextState].alignStr;
	}

	return str.c_str();
}
int SMM3_S4_Make_SE_Table_Function(SMM3_t &nextState)
{
	for (u32 i = 1; i < gCB_SE_TABLE_RawVector.size(); i++) {
		SE_Row_t se_row;
		
		if (gCB_SE_TABLE_RawVector[i].str[0] == '-') {
			//ignore
		} else {
			LibStringClass libStr(gCB_SE_TABLE_RawVector[i].str.c_str());
			libStr.Split();
			BASIC_ASSERT(libStr.subStrVector.size()>=1);
			u32 rowEventCnt = libStr.subStrVector.size() - 1;

			for (u32 j = 0; j < gS2_SimpleEventTotalCount; j++) {
				SE_Cell_t se_cell;
				se_cell.nextState = gS1_StateTotalCount;

				if (j >= rowEventCnt) {
					se_cell.eventPassType = NullPass;
				} else {
					switch (libStr.subStrVector[j+1][0]) {
						case 'D':
							se_cell.eventPassType = DirectPass;
							libStr.subStrVector[j+1].erase(0,2);
							se_cell.nextState = _StateStringToIndex(libStr.subStrVector[j+1]);
							break;
						case 'R':
							se_cell.eventPassType = RepeatPass;
							libStr.subStrVector[j+1].erase(0,2);
							se_cell.nextState = _StateStringToIndex(libStr.subStrVector[j+1]);
							break;
						case 'N':
							se_cell.eventPassType = NullPass;
							break;
						default:
							printf("Event pass type should be \"D\" or \"R\" or \"N\", error in line:%d\n", gCB_SE_TABLE_RawVector[i].lineNumber);
							EXIT_LOC_IF(1);
							break;
					}
				}
				se_row.row.push_back(se_cell);
			}
			gS4_SE_Table.push_back(se_row);
		}
	}

	DUMPD(gS4_SE_Table.size());
	
	#if 1
	for (u32 i = 0; i < gS1_StateTotalCount; i++) {
		for (u32 j = 0; j < gS2_SimpleEventTotalCount; j++) {
			Event_Pass_Type_t eventPassType = gS4_SE_Table[i].row[j].eventPassType;
			u32 nextState = gS4_SE_Table[i].row[j].nextState;
			printf("%s,%-16s  ", _EventTypeToString(eventPassType), _StateIndexToString(nextState));
			//printf("%d,%-16d  ", (eventPassType), (nextState));
		}
		printf("\n");
	}
	#endif

	nextState = SMM3_S5_Make_InterState_Table;
	return 0; // Error Code, true for error.
}

typedef struct {
	u32 fatherState;
	std::vector<u32> childState;
	std::vector<std::string> childStateExtEnumName;
}Inter_State_Row_t;
std::vector<Inter_State_Row_t> gS5_InterState_Table;
int SMM3_S5_Make_InterState_Table_Function(SMM3_t &nextState)
{
typedef enum {
	Finding_Parent,
	Finding_Children,
}Inter_State_Parsing;

	Inter_State_Parsing st = Finding_Parent;

	Inter_State_Row_t interStateRow;

	gS5_InterState_Table.clear();

	for (u32 i = 0; i < gCB_INTER_STATE_RawVector.size(); i++) {
	
		gTempLibStr.Init(gCB_INTER_STATE_RawVector[i].str);
		gTempLibStr.Split();
		
		switch (st) {
			case Finding_Parent: {
				if (gTempLibStr.subStrVector.size() != 2 || gTempLibStr.subStrVector[1][0] != '{') {
					printf("Expect '{' in line: %d\n", gCB_INTER_STATE_RawVector[i].lineNumber);
					EXIT_LOC_IF(1);
				} else {
					st = Finding_Children;
					interStateRow.fatherState = _StateStringToIndex(gTempLibStr.subStrVector[0]);
					if (interStateRow.fatherState == 0xFFFFFFFF) {
						printf("Illegal state name in line: %d\n", gCB_INTER_STATE_RawVector[i].lineNumber);
						EXIT_LOC_IF(1);
					}
					if (IntermediateState != gS1_StateVector[interStateRow.fatherState].stateType) {
						printf("Parent state must be inter-state, error in line: %d\n", gCB_INTER_STATE_RawVector[i].lineNumber);
						EXIT_LOC_IF(1);
					}
				}
			} break;

			case Finding_Children: {
				if (gTempLibStr.subStrVector.size() == 1 && gTempLibStr.subStrVector[0][0] == '}') {
					gS5_InterState_Table.push_back(interStateRow);
					interStateRow.childState.clear();
					interStateRow.childStateExtEnumName.clear();
					st = Finding_Parent;
					break;
				}

				if (gTempLibStr.subStrVector.size() != 2) {
					printf("Expect 2 strings(state, enum name) in line: %d\n", gCB_INTER_STATE_RawVector[i].lineNumber);
					EXIT_LOC_IF(1);
				} else {
					u32 childState = _StateStringToIndex(gTempLibStr.subStrVector[0]);
					if (childState == 0xFFFFFFFF) {
						printf("Illegal state name in line: %d\n", gCB_INTER_STATE_RawVector[i].lineNumber);
						EXIT_LOC_IF(1);
					}
					interStateRow.childState.push_back(childState);
					interStateRow.childStateExtEnumName.push_back(gTempLibStr.subStrVector[1]);
				}
			} break;

			default:
				BASIC_ASSERT(0);
				break;
		}
		
	}

	#if 1
	printf("--- INTER STATE ---\n");
	for (u32 i = 0; i < gS5_InterState_Table.size(); i++) {
		printf("%s:\n", _StateIndexToString(gS5_InterState_Table[i].fatherState));
		for (u32 j = 0; j < gS5_InterState_Table[i].childState.size(); j++) {
			printf("\t%-15s, %s\n", _StateIndexToString(gS5_InterState_Table[i].childState[j]), gS5_InterState_Table[i].childStateExtEnumName[j].c_str());
		}
	}
	#endif

	nextState = SMM3_S6_Make_StateFunc_Table;
	return 0; // Error Code, true for error.
}

typedef struct {
	std::string entryFunc;
	std::string leaveFunc;
}State_Func_Row_t;
std::vector<State_Func_Row_t> gS6_StateFunc_Table;
int SMM3_S6_Make_StateFunc_Table_Function(SMM3_t &nextState)
{
	State_Func_Row_t stateFuncRow;

	if (gS1_StateTotalCount != gCB_STATE_FUNC_RawVector.size()) {
		printf("Total number of states are not match, in line:%d\n", gCB_STATE_FUNC_RawVector[0].lineNumber);
		EXIT_LOC_IF(1);
	}

	for (u32 i = 0; i < gCB_STATE_FUNC_RawVector.size(); i++) {
		gTempLibStr.Init(gCB_STATE_FUNC_RawVector[i].str);
		gTempLibStr.Split();

		if (gTempLibStr.subStrVector.size() < 1 || gTempLibStr.subStrVector.size() > 3) {
			printf("Should be 1~3 word: state/entry/leave in line:%d\n", gCB_STATE_FUNC_RawVector[i].lineNumber);
			EXIT_LOC_IF(1);
		}

		u32 stateIndex = _StateStringToIndex(gTempLibStr.subStrVector[0]);

		if (stateIndex != i) {
			printf("State name not match in line:%d\n", gCB_STATE_FUNC_RawVector[i].lineNumber);
			EXIT_LOC_IF(1);
		}

		if (gTempLibStr.subStrVector.size() < 2)
			stateFuncRow.entryFunc = "NULL";
		else
			stateFuncRow.entryFunc = gTempLibStr.subStrVector[1];

		if (gTempLibStr.subStrVector.size() < 3)
			stateFuncRow.leaveFunc = "NULL";
		else
			stateFuncRow.leaveFunc = gTempLibStr.subStrVector[2];

		gS6_StateFunc_Table.push_back(stateFuncRow);
	}

	#if 1
	printf("---\n");
	for (u32 i = 0; i < gS6_StateFunc_Table.size(); i++) {
		printf("%-15s, %-15s, %s\n", _StateIndexToString(i), gS6_StateFunc_Table[i].entryFunc.c_str(), gS6_StateFunc_Table[i].leaveFunc.c_str());
	}
	#endif

	nextState = SMM3_S8_Create_Header_File;
	return 0; // Error Code, true for error.
}

int SMM3_S7_Make_TestCase_Table_Function(SMM3_t &nextState)
{
	return 0; // Error Code, true for error.
}

static void _SMM3_S8_Print_IncludeLibrary_Front(LibFileIoClass &outFile)
{
	// STD Library
	outFile.FilePrint(
		"#include <stdlib.h>" LF LF);

	// Customized Library
	for (u32 i = 0; i < gCB_INCLUDE_FRONT_RawVector.size(); i++) {
		outFile.FilePrint("%s\n", gCB_INCLUDE_FRONT_RawVector[i].rawStr.c_str());
	}
	outFile.FilePrint("\n");
}

static void _SMM3_S8_Print_EventTypedef(LibFileIoClass &outFile)
{
	// typedef EVENT
	outFile.FilePrint("typedef enum {\n");
	for (u32 i = 0; i < gS2_EventVector.size(); i++) {
		outFile.FilePrint(
			"\t%s%s%s = %2d, // 0x%02x\n",
			gS2_EventVector[i].prefixStr.c_str(),
			gS2_EventVector[i].eventStr.c_str(),
			gS2_EventVector[i].alignStr.c_str(),
			i, i);
	}
	outFile.FilePrint("} %s;\n\n", _STR_EventTypedefName());
}

static void _SMM3_S8_Print_StateTypedef(LibFileIoClass &outFile)
{
	// typedef EVENT
	outFile.FilePrint("typedef enum {\n");
	for (u32 i = 0; i < gS1_StateVector.size(); i++) {
		outFile.FilePrint(
			"\t%s%s%s = %2d, // 0x%02x\n",
			gS1_StateVector[i].prefixStr.c_str(),
			gS1_StateVector[i].stateStr.c_str(),
			gS1_StateVector[i].alignStr.c_str(),
			i, i);
	}
	outFile.FilePrint(
		LF\
		HT "%sDUMMY," LF,
		gS0_State_Prefix.c_str());
	outFile.FilePrint("} %s;\n\n", _STR_StateTypedefName());
}

static void _SMM3_S8_Print_EventParaTypedef(LibFileIoClass &outFile)
{
	if (0 == gCB_EVENT_PARA_RawVector.size()) {
		outFile.FilePrint(
			"typedef struct {" LF \
			HT "int dummy;" LF \
			"} %s;" LF LF,
			_STR_EventParaType());
	} else {
		outFile.FilePrint(
			"typedef struct {" LF);
		for (u32 i = 0; i < gCB_EVENT_PARA_RawVector.size(); i++) {
			outFile.FilePrint("%s\n", gCB_EVENT_PARA_RawVector[i].rawStr.c_str());
		}
		outFile.FilePrint(
			"} %s;" LF LF,
			_STR_EventParaType());
	}
}

static void _SMM3_S8_Print_StateChangeFunc(LibFileIoClass &outFile)
{
	// State change function
	outFile.FilePrint(
		"%s %s(%s nextState, %s *evtPara = NULL);" LF LF,
		_STR_StateTypedefName(),
		_STR_StateChangeFuncName(),
		_STR_StateTypedefName(),
		_STR_EventParaType());
}

static void _SMM3_S8_Print_StateMachineFunc(LibFileIoClass &outFile)
{
	// Core source file
	outFile.FilePrint("%s %s(%s event, %s *evtPara = NULL);" LF,
		_STR_StateTypedefName(),
		_STR_StateMachineFuncName(),
		_STR_EventTypedefName(),
		_STR_EventParaType());

	outFile.FilePrint(
		"typedef enum {" LF\
		HT "SimpleState," LF\
		HT "InterState," LF\
		"} State_Type_t;" LF);

	outFile.FilePrint(
		"typedef enum {" LF\
		HT "DirectPass," LF\
		HT "RepeatPass," LF\
		HT "NullPass," LF\
		"} Event_Pass_Type_t;" LF);

	outFile.FilePrint(
		"typedef struct {" LF\
		HT "Event_Pass_Type_t passType;" LF\
		HT "%s nextState;" LF\
		"} State_Cell_t;" LF,
		_STR_StateTypedefName());

	outFile.FilePrint(
		"typedef struct {" LF);

	#if 0
	for (u32 i = 0; i < gS2_SimpleEventTotalCount; i++) {
		char tempStr[10];
		sprintf(tempStr, "%d", i);
		outFile.FilePrint(HT "State_Cell_t cell_%s;" LF, tempStr);
	}
	#endif
	outFile.FilePrint(
		HT "State_Type_t stateType;" LF\
		HT "State_Cell_t cell[%d];" LF, gS2_SimpleEventTotalCount);

	outFile.FilePrint(
		"} State_Row_t;" LF LF);
}

static void _SMM3_S8_Print_StateFunc(LibFileIoClass &outFile)
{
	// state functions
	for (u32 i = 0; i < gS1_StateVector.size(); i++) {
		if (0 == strcmp(gS6_StateFunc_Table[i].entryFunc.c_str(), "DEFAULT")) {
			outFile.FilePrint(
				"int %s%s_Entry(%s oldState, %s *evtPara = NULL);" LF,
				gS1_StateVector[i].prefixStr.c_str(),
				gS1_StateVector[i].stateStr.c_str(),
				_STR_StateTypedefName(),
				_STR_EventParaType());
		}
		#if 0
		else if (0 == strcmp(gS6_StateFunc_Table[i].entryFunc.c_str(), "PURE")) {
			outFile.FilePrint(
				"int %s%s(%s oldState, %s *evtPara = NULL);" LF,
				gS1_StateVector[i].prefixStr.c_str(),
				gS1_StateVector[i].stateStr.c_str(),
				_STR_StateTypedefName(),
				_STR_EventParaType());
		}
		#endif

		if (0 == strcmp(gS6_StateFunc_Table[i].leaveFunc.c_str(), "DEFAULT")) {
			outFile.FilePrint(
				"int %s%s_Leave(%s newState, %s *evtPara = NULL);" LF,
				gS1_StateVector[i].prefixStr.c_str(),
				gS1_StateVector[i].stateStr.c_str(),
				_STR_StateTypedefName(),
				_STR_EventParaType());
		}
		#if 0
		else if (0 == strcmp(gS6_StateFunc_Table[i].leaveFunc.c_str(), "PURE")) {
			outFile.FilePrint(
				"int %s%s(%s oldState, %s *evtPara = NULL);" LF,
				gS1_StateVector[i].prefixStr.c_str(),
				gS1_StateVector[i].stateStr.c_str(),
				_STR_StateTypedefName(),
				_STR_EventParaType());
		}
		#endif
	}
	
	outFile.FilePrint(
		LF \
		"typedef int (*State_CB)(%s state, %s *evtPara);" LF \
		"typedef struct {" LF \
		HT "State_CB entry;" LF \
		HT "State_CB leave;" LF \
		"} State_CB_t;" LF LF LF,
		_STR_StateTypedefName(),
		_STR_EventParaType());
}

static void _SMM3_S8_Print_GlobalEventFunc(LibFileIoClass &outFile)
{
	for (u32 i = 0; i < gS2_EventVector.size(); i++) {
		if (gS2_EventVector[i].eventType == GlobalEvent) {
			if (0 == strcmp("NULL", gS2_EventVector[i].globalEventFunc.c_str())) {
				; // do nothing
			} else if (0 == strcmp("DEFAULT", gS2_EventVector[i].globalEventFunc.c_str())) {
				outFile.FilePrint("%s %s%s_Function(%s &state, %s *evtPara = NULL);\n",
					_STR_StateTypedefName(),
					gS2_EventVector[i].prefixStr.c_str(),
					gS2_EventVector[i].eventStr.c_str(),
					_STR_StateTypedefName(),
					_STR_EventParaType());
			} else {
				; // do nothing
			}
		}
	}

	outFile.FilePrint(
		"typedef %s (*Event_CB)(%s &state, %s *evtPara);" LF LF,
		_STR_StateTypedefName(),
		_STR_StateTypedefName(),
		_STR_EventParaType());
}

static void _SMM3_S8_Print_IncludeLibrary_Back(LibFileIoClass &outFile)
{
	// Customized Library
	for (u32 i = 0; i < gCB_INCLUDE_BACK_RawVector.size(); i++) {
		outFile.FilePrint("%s\n", gCB_INCLUDE_BACK_RawVector[i].rawStr.c_str());
	}
	outFile.FilePrint("\n");
}

int SMM3_S8_Create_Header_File_Function(SMM3_t &nextState)
{
	LibFileIoClass outFile(gS0_Out_File_Header_C.c_str(), "w+b");

	outFile.FileOpen();

	// 1. avoid re-define
	LibStringClass redefineStr;
	redefineStr.Init(gS0_Out_File_Header_C.c_str());
	redefineStr.ReplaceSubString(".", "_");
	redefineStr.ToUpperCase();
	redefineStr.str = redefineStr.str.insert(0, "_");
	redefineStr.str = redefineStr.str.insert(redefineStr.str.length(), "_");

	// Avoid redefinition
	bool isRedefineTypeBefore;
	EXIT_CHK( rc, gS0_Ini_File_Obj.GetValueString("[ATTR]", "AVOID_REDEFINE_TYPE", gTempStr) );
	if (0 == strcmp(gTempStr.c_str(), "BEFORE")) {
		isRedefineTypeBefore = true;
	} else if (0 == strcmp(gTempStr.c_str(), "AFTER")) {
		isRedefineTypeBefore = false;
	} else {
		printf("AVOID_REDEFINE_TYPE must be \"BEFORE\" or \"AFTER\".\n");
		EXIT_LOC_IF(1);
	}
	// Avoid redefinition
	if (isRedefineTypeBefore) {
		outFile.FilePrint(
			"\n" \
			"#ifndef %s" LF \
			"#define %s" LF LF,
			redefineStr.CStr(),
			redefineStr.CStr());
	} else {
		outFile.FilePrint(
			"\n" \
			"#ifndef %s\n\n",
			redefineStr.CStr());
	}

	_SMM3_S8_Print_IncludeLibrary_Front(outFile);
	_SMM3_S8_Print_EventTypedef(outFile);
	_SMM3_S8_Print_StateTypedef(outFile);
	_SMM3_S8_Print_EventParaTypedef(outFile);
	_SMM3_S8_Print_StateChangeFunc(outFile);
	_SMM3_S8_Print_StateMachineFunc(outFile);
	_SMM3_S8_Print_StateFunc(outFile);
	_SMM3_S8_Print_GlobalEventFunc(outFile);
	_SMM3_S8_Print_IncludeLibrary_Back(outFile);

	// Avoid redefinition
	if (isRedefineTypeBefore) {
		outFile.FilePrint(
			LF \
			"#endif //%s" LF LF,
			redefineStr.CStr());
	} else {
		outFile.FilePrint(
			LF \
			"#define %s" LF \
			"#endif //%s" LF LF,
			redefineStr.CStr(),
			redefineStr.CStr());
	}

	nextState = SMM3_S9_Create_Core_File;
	return 0; // Error Code, true for error.
}

static void _SMM3_S9_Print_Include_n_StateGlobalVariable(LibFileIoClass &outFile)
{
	outFile.FilePrint(
		"\n" \
		"#include \"%s\"\n" \
		"\n"
		"%s  %s = %s%s;\n\n",
		gS0_Out_File_Header_C.c_str(),
		_STR_StateTypedefName(), _STR_StateGlobalVarName(), gS1_StateVector[0].prefixStr.c_str(), gS1_StateVector[0].stateStr.c_str());
}

static void _SMM3_S9_Print_SE_Table(LibFileIoClass &outFile)
{
	gTempStr = "";
	gTempStr.insert(0, _StateStringLen_WithPrefix_WithAlign() + 14 + 2, ' ');
	
	outFile.FilePrint(
		"State_Row_t gSE_Table[] = {" LF\
		"// %s      ",
		gTempStr.c_str());
	u32 columnWidth = _StateStringLen_WithPrefix_WithAlign() + 11 + 4;
	for (u32 i = 0; i < gS2_SimpleEventTotalCount; i++) {
		std::string colStr = gS2_EventVector[i].prefixStr + gS2_EventVector[i].eventStr;
		u32 alignLen = columnWidth - colStr.length();
		colStr.insert(colStr.length(), alignLen, ' ');
		outFile.FilePrint("%s ", colStr.c_str());
	}
	outFile.FilePrint(LF);

	for (u32 i = 0; i < gS4_SE_Table.size(); i++) {
		switch (gS1_StateVector[i].stateType) {
			case SimpleState:
				gTempStr = "SimpleState";
				break;
			case IntermediateState:
				gTempStr = "InterState";
				break;
			default:
				EXIT_LOC_IF(1);
				break;
		}
			
		outFile.FilePrint(
			"/* %s%s%s */ { %-11s, { ",
			gS1_StateVector[i].prefixStr.c_str(),
			gS1_StateVector[i].stateStr.c_str(),
			gS1_StateVector[i].alignStr.c_str(),
			gTempStr.c_str());
		for (u32 j = 0; j < gS4_SE_Table[i].row.size(); j++) {
			switch (gS4_SE_Table[i].row[j].eventPassType) {
				case DirectPass:
					gTempStr = "DirectPass,";
					break;
				case RepeatPass:
					gTempStr = "RepeatPass,";
					break;
				case NullPass:
					gTempStr = "NullPass,";
					break;
				default:
					EXIT_LOC_IF(1);
					break;
			}
			u32 nextState = gS4_SE_Table[i].row[j].nextState;
			std::string dummyStr = gS0_State_Prefix + "DUMMY";
			outFile.FilePrint("{%-11s %s}, ", gTempStr.c_str(), _StateIndexToString_WithPrefix_WithAlign(nextState, dummyStr.c_str()));
		}
		outFile.FilePrint("}, }," LF);
	}

	outFile.FilePrint("};" LF LF);
}

static void _SMM3_S9_Print_State_CB_Table(LibFileIoClass &outFile)
{
	// State change function
	outFile.FilePrint("State_CB_t gState_CB_Table[] = {" LF);

	for (u32 i = 0; i < gS6_StateFunc_Table.size(); i++) {
		std::string entryStr;
		if (0 == strcmp("NULL", gS6_StateFunc_Table[i].entryFunc.c_str())) {
			entryStr = "NULL";
		} else if (0 == strcmp("DEFAULT", gS6_StateFunc_Table[i].entryFunc.c_str())) {
			entryStr = gS1_StateVector[i].prefixStr + gS1_StateVector[i].stateStr + "_Entry";
		#if 0
		} else if (0 == strcmp("PURE", gS6_StateFunc_Table[i].entryFunc.c_str())) {
			entryStr = gS1_StateVector[i].prefixStr + gS1_StateVector[i].stateStr + "";
		#endif
		} else {
			entryStr = gS6_StateFunc_Table[i].entryFunc;
		}

		std::string leaveStr;
		if (0 == strcmp("NULL", gS6_StateFunc_Table[i].leaveFunc.c_str())) {
			leaveStr = "NULL";
		} else if (0 == strcmp("DEFAULT", gS6_StateFunc_Table[i].leaveFunc.c_str())) {
			leaveStr = gS1_StateVector[i].prefixStr + gS1_StateVector[i].stateStr + "_Leave";
		#if 0
		} else if (0 == strcmp("PURE", gS6_StateFunc_Table[i].leaveFunc.c_str())) {
			leaveStr = gS1_StateVector[i].prefixStr + gS1_StateVector[i].stateStr + "";
		#endif
		} else {
			leaveStr = gS6_StateFunc_Table[i].leaveFunc;
		}

		entryStr = entryStr + ',';
		outFile.FilePrint(
			"\t{%-30s %-30s}, // %s%s" LF,
			entryStr.c_str(),
			leaveStr.c_str(),
			gS1_StateVector[i].prefixStr.c_str(),
			gS1_StateVector[i].stateStr.c_str());
	}

	outFile.FilePrint("};" LF LF);
}

static void _SMM3_S9_Print_StateChangeFunction(LibFileIoClass &outFile)
{
	// State change function
	outFile.FilePrint(
		"%s %s(%s nextState, %s *evtPara /* = NULL */)" LF \
		"{" LF \
		HT "int retVal = 0;" LF \
		HT "State_CB leaveCB = gState_CB_Table[%s].leave;" LF \
		HT "State_CB entryCB = gState_CB_Table[nextState].entry;" LF LF,
		_STR_StateTypedefName(),
		_STR_StateChangeFuncName(),
		_STR_StateTypedefName(),
		_STR_EventParaType(),
		_STR_StateGlobalVarName());

	outFile.FilePrint(
		HT "if (NULL != leaveCB) {" LF \
		HT HT "retVal = (*leaveCB)(nextState, evtPara);" LF \
/*		HT HT "if (retVal)" LF \        */
/*		HT HT HT "return retVal;" LF \  */
		HT "}" LF LF
	);

	outFile.FilePrint(
		HT "if (NULL != entryCB) {" LF \
		HT HT "retVal = (*entryCB)(nextState, evtPara);" LF \
/*		HT HT "if (retVal)" LF \        */
/*		HT HT HT "return retVal;" LF \  */
		HT "}" LF \
		LF \
		HT "switch (nextState) {" LF
	);

	for (u32 i = 0; i < gS1_StateVector.size(); i++) {
		switch (gS1_StateVector[i].stateType) {
			case SimpleState:
				break;
			case IntermediateState:
				outFile.FilePrint(
					HT HT "case %s: {" LF \
					HT HT HT "switch (retVal) {" LF ,
					_StateIndexToString_WithPrefix(i, "ERROR")
				);
				for (u32 j = 0; j < gS5_InterState_Table.size(); j++) {
					if (gS5_InterState_Table[j].fatherState == i) {
						for (u32 k = 0; k < gS5_InterState_Table[j].childState.size(); k++) {
							outFile.FilePrint(
								HT HT HT HT "case %s:" LF\
								HT HT HT HT HT "%s = nextState;" LF\
								HT HT HT HT HT "return %s(%s, evtPara);" LF LF ,
								gS5_InterState_Table[j].childStateExtEnumName[k].c_str(),
								_STR_StateGlobalVarName(),
								_STR_StateChangeFuncName(),
								_StateIndexToString_WithPrefix(gS5_InterState_Table[j].childState[k], "ERROR")
							);
						}
						break;
					}
				}
				outFile.FilePrint(
					HT HT HT "}" LF \
					HT HT "} break;" LF LF\
				);
				break;
			default:
				EXIT_LOC_IF(1);
				break;
		}
	}

	outFile.FilePrint(
		HT HT "default:" LF\
		HT HT HT "break;" LF\
		HT "}" LF\
		LF\
		HT "return nextState;" LF\
		"}" LF LF);
}

static void _SMM3_S9_Print_GlobalEventCB_Array(LibFileIoClass &outFile)
{
	outFile.FilePrint("Event_CB gGlobalEvent_CB_List[] = {" LF);

	for (u32 i = 0; i < gS2_EventVector.size(); i++) {
		if (gS2_EventVector[i].eventType == GlobalEvent) {
			if (0 == strcmp("NULL", gS2_EventVector[i].globalEventFunc.c_str())) {
				outFile.FilePrint(HT "NULL," LF);
			} else if (0 == strcmp("DEFAULT", gS2_EventVector[i].globalEventFunc.c_str())) {
				outFile.FilePrint(HT "%s%s_Function," LF,
					gS2_EventVector[i].prefixStr.c_str(),
					gS2_EventVector[i].eventStr.c_str());
			} else {
				outFile.FilePrint(HT "%s," LF,
					gS2_EventVector[i].globalEventFunc.c_str());
			}
		} else {
			//outFile.FilePrint(HT "NULL," LF);
		}
	}

	outFile.FilePrint("};" LF LF);
}

static void _SMM3_S9_Print_GlobalEventSection(LibFileIoClass &outFile)
{
	outFile.FilePrint(HT "switch (event) {" LF);

	for (u32 i = 0; i < gS2_EventVector.size(); i++) {
		if (gS2_EventVector[i].eventType == GlobalEvent) {
			outFile.FilePrint(
				HT HT "case %s%s:" LF,
				gS2_EventVector[i].prefixStr.c_str(),
				gS2_EventVector[i].eventStr.c_str());
		}
	}
	outFile.FilePrint(
		HT HT HT "if (NULL != gGlobalEvent_CB_List[event-%d]) {" LF \
		HT HT HT HT "return (*(gGlobalEvent_CB_List[event-%d]))(%s, evtPara);" LF \
		HT HT HT "} else {" LF \
		HT HT HT HT "return %s;" LF \
		HT HT HT "} break;" LF LF\
		HT HT "default:" LF\
		HT HT HT "break;" LF,
		gS2_SimpleEventTotalCount,
		gS2_SimpleEventTotalCount,
		_STR_StateGlobalVarName(),
		_STR_StateGlobalVarName());

	outFile.FilePrint(HT "}" LF LF);
}

static void _SMM3_S9_Print_StateChangeSection(LibFileIoClass &outFile)
{
	outFile.FilePrint(
		HT "Event_Pass_Type_t passType;" LF\
		HT "%s nextState;" LF\
		LF\
		HT "do {" LF\
		HT HT "passType  = gSE_Table[%s].cell[event].passType;" LF\
		HT HT "nextState = gSE_Table[%s].cell[event].nextState;" LF\
		LF\
		HT HT "if (NullPass == passType)" LF\
		HT HT HT "break;" LF\
		LF\
		HT HT "%s = %s(nextState);" LF\
		HT "} while (RepeatPass == passType);" LF\
		LF\
		HT "return %s;" LF,
		_STR_StateTypedefName(),
		_STR_StateGlobalVarName(),
		_STR_StateGlobalVarName(),
		_STR_StateGlobalVarName(),
		_STR_StateChangeFuncName(),
		_STR_StateGlobalVarName());

}

int SMM3_S9_Create_Core_File_Function(SMM3_t &nextState)
{
	LibFileIoClass outFile(gS0_Out_File_Source_C.c_str(), "w+b");

	outFile.FileOpen();

	_SMM3_S9_Print_Include_n_StateGlobalVariable(outFile);
	_SMM3_S9_Print_SE_Table(outFile);
	_SMM3_S9_Print_State_CB_Table(outFile);
	_SMM3_S9_Print_StateChangeFunction(outFile);
	_SMM3_S9_Print_GlobalEventCB_Array(outFile);

	outFile.FilePrint(
		"%s %s(%s event, %s *evtPara /* = NULL */)" LF \
		"{" LF,
		_STR_StateTypedefName(),
		_STR_StateMachineFuncName(),
		_STR_EventTypedefName(),
		_STR_EventParaType());

	_SMM3_S9_Print_GlobalEventSection(outFile);
	_SMM3_S9_Print_StateChangeSection(outFile);

	outFile.FilePrint("}" LF);
	
	nextState = SMM3_S10_Create_Function_File;
	return 0; // Error Code, true for error.
}

int SMM3_S10_Create_Function_File_Function(SMM3_t &nextState)
{
	LibFileIoClass outFile(_STR_FuncFileName(), "w+b");

	outFile.FileOpen();

	outFile.FilePrint(
		"\n" \
		"#include \"%s\"\n" \
		"\n",
		gS0_Out_File_Header_C.c_str());
	
	// state functions
	for (u32 i = 0; i < gS1_StateVector.size(); i++) {
		if (0 == strcmp(gS6_StateFunc_Table[i].entryFunc.c_str(), "DEFAULT")) {
			outFile.FilePrint(
				"int %s%s_Entry(%s oldState, %s *evtPara /* = NULL */)" LF\
				"{" LF\
				HT "return 0;" LF\
				"}" LF LF,
				gS1_StateVector[i].prefixStr.c_str(),
				gS1_StateVector[i].stateStr.c_str(),
				_STR_StateTypedefName(),
				_STR_EventParaType());
		}
		#if 0
		else if (0 == strcmp(gS6_StateFunc_Table[i].entryFunc.c_str(), "PURE")) {
			outFile.FilePrint(
				"int %s%s(%s oldState, %s *evtPara /* = NULL */)" LF\
				"{" LF\
				HT "return 0;" LF\
				"}" LF LF,
				gS1_StateVector[i].prefixStr.c_str(),
				gS1_StateVector[i].stateStr.c_str(),
				_STR_StateTypedefName(),
				_STR_EventParaType());
		}
		#endif

		if (0 == strcmp(gS6_StateFunc_Table[i].leaveFunc.c_str(), "DEFAULT")) {
			outFile.FilePrint(
				"int %s%s_Leave(%s newState, %s *evtPara /* = NULL */)" LF\
				"{" LF\
				HT "return 0;" LF\
				"}" LF LF,
				gS1_StateVector[i].prefixStr.c_str(),
				gS1_StateVector[i].stateStr.c_str(),
				_STR_StateTypedefName(),
				_STR_EventParaType());
		}
		#if 0
		else if (0 == strcmp(gS6_StateFunc_Table[i].leaveFunc.c_str(), "PURE")) {
			outFile.FilePrint(
				"int %s%s(%s newState, %s *evtPara /* = NULL */)" LF\
				"{" LF\
				HT "return 0;" LF\
				"}" LF LF,
				gS1_StateVector[i].prefixStr.c_str(),
				gS1_StateVector[i].stateStr.c_str(),
				_STR_StateTypedefName(),
				_STR_EventParaType());
		}
		#endif
	}

	for (u32 i = 0; i < gS2_EventVector.size(); i++) {
		if (gS2_EventVector[i].eventType == GlobalEvent) {
			if (0 == strcmp("NULL", gS2_EventVector[i].globalEventFunc.c_str())) {
				; // do nothing
			} else if (0 == strcmp("DEFAULT", gS2_EventVector[i].globalEventFunc.c_str())) {
				outFile.FilePrint(
					"%s %s%s_Function(%s &state, %s *evtPara /* = NULL */)" LF\
					"{" LF\
					HT "return state;" LF\
					"}" LF LF,
					_STR_StateTypedefName(),
					gS2_EventVector[i].prefixStr.c_str(),
					gS2_EventVector[i].eventStr.c_str(),
					_STR_StateTypedefName(),
					_STR_EventParaType());
			} else {
				; // do nothing
			}
		}
	}
	
	nextState = SMM3_S11_Exit;
	return 0; // Error Code, true for error.
}

std::string gExt_File_Name;

static void _Print_Ext_Header_Enum(LibFileIoClass &outFile)
{
	outFile.FilePrint("typedef enum {" LF);

	for (u32 i = 0; i < gS5_InterState_Table.size(); i++) {
		for (u32 j = 0; j < gS5_InterState_Table[i].childStateExtEnumName.size(); j++) {
			outFile.FilePrint(
				HT "%s," LF,
				gS5_InterState_Table[i].childStateExtEnumName[j].c_str());
		}
	}

	outFile.FilePrint("} %s_Return_Code_t;" LF LF, gExt_File_Name.c_str());
}

static void _Print_Ext_Header_FuncDeclare(LibFileIoClass &outFile)
{
	for (u32 i = 0; i < gS6_StateFunc_Table.size(); i++) {
		std::string entryStr;
		if (0 == strcmp("NULL", gS6_StateFunc_Table[i].entryFunc.c_str())) {
			//entryStr = "NULL";
		} else if (0 == strcmp("DEFAULT", gS6_StateFunc_Table[i].entryFunc.c_str())) {
			//entryStr = gS1_StateVector[i].prefixStr + gS1_StateVector[i].stateStr + "_Entry";
		} else {
			entryStr = gS6_StateFunc_Table[i].entryFunc;
			outFile.FilePrint(
				"int %s(%s state, %s *evtPara = NULL);" LF,
				entryStr.c_str(),
				_STR_StateTypedefName(),
				_STR_EventParaType());
		}

		std::string leaveStr;
		if (0 == strcmp("NULL", gS6_StateFunc_Table[i].leaveFunc.c_str())) {
			//leaveStr = "NULL";
		} else if (0 == strcmp("DEFAULT", gS6_StateFunc_Table[i].leaveFunc.c_str())) {
			//leaveStr = gS1_StateVector[i].prefixStr + gS1_StateVector[i].stateStr + "_Leave";
		} else {
			leaveStr = gS6_StateFunc_Table[i].leaveFunc;
			outFile.FilePrint(
				"int %s(%s state, %s *evtPara = NULL);" LF,
				leaveStr.c_str(),
				_STR_StateTypedefName(),
				_STR_EventParaType());
		}
	}

	// Global Event Function
	for (u32 i = 0; i < gS2_EventVector.size(); i++) {
		if (gS2_EventVector[i].eventType == GlobalEvent) {
			if (0 == strcmp("NULL", gS2_EventVector[i].globalEventFunc.c_str())) {
				//outFile.FilePrint(HT "NULL," LF);
			} else if (0 == strcmp("DEFAULT", gS2_EventVector[i].globalEventFunc.c_str())) {
				//outFile.FilePrint(HT "%s%s_Function," LF,
				//	gS2_EventVector[i].prefixStr.c_str(),
				//	gS2_EventVector[i].eventStr.c_str());
			} else {
				outFile.FilePrint(
					"%s %s(%s &state, %s *evtPara = NULL);" LF,
					_STR_StateTypedefName(),
					gS2_EventVector[i].globalEventFunc.c_str(),
					_STR_StateTypedefName(),
					_STR_EventParaType());
			}
		}
	}
}

static void _Print_Ext_Header(void)
{
	EXIT_CHK( rc, gS0_Ini_File_Obj.GetValueString("[ATTR]", "Ext_File_Name", gExt_File_Name) );

	std::string ext_File_Header_C;
	ext_File_Header_C = gExt_File_Name + ".hpp";

	LibFileIoClass outFile(ext_File_Header_C.c_str(), "w+b");

	outFile.FileOpen();

	// 1. avoid re-define
	LibStringClass redefineStr;
	redefineStr.Init(ext_File_Header_C.c_str());
	redefineStr.ReplaceSubString(".", "_");
	redefineStr.ToUpperCase();
	redefineStr.str = redefineStr.str.insert(0, "_");
	redefineStr.str = redefineStr.str.insert(redefineStr.str.length(), "_");

	// Avoid redefinition
	bool isRedefineTypeBefore;
	EXIT_CHK( rc, gS0_Ini_File_Obj.GetValueString("[ATTR]", "AVOID_REDEFINE_TYPE", gTempStr) );
	if (0 == strcmp(gTempStr.c_str(), "BEFORE")) {
		isRedefineTypeBefore = true;
	} else if (0 == strcmp(gTempStr.c_str(), "AFTER")) {
		isRedefineTypeBefore = false;
	} else {
		printf("AVOID_REDEFINE_TYPE must be \"BEFORE\" or \"AFTER\".\n");
		EXIT_LOC_IF(1);
	}
	// Avoid redefinition
	if (isRedefineTypeBefore) {
		outFile.FilePrint(
			"\n" \
			"#ifndef %s" LF \
			"#define %s" LF LF,
			redefineStr.CStr(),
			redefineStr.CStr());
	} else {
		outFile.FilePrint(
			"\n" \
			"#ifndef %s\n\n",
			redefineStr.CStr());
	}

	_Print_Ext_Header_Enum(outFile);
	_Print_Ext_Header_FuncDeclare(outFile);
	
	// Avoid redefinition
	if (isRedefineTypeBefore) {
		outFile.FilePrint(
			LF \
			"#endif //%s" LF LF,
			redefineStr.CStr());
	} else {
		outFile.FilePrint(
			LF \
			"#define %s" LF \
			"#endif //%s" LF LF,
			redefineStr.CStr(),
			redefineStr.CStr());
	}
}

static void _Print_Ext_Function(void)
{
	std::string ext_File_Function_C;
	ext_File_Function_C = gExt_File_Name + ".cpp";

	LibFileIoClass outFile(ext_File_Function_C.c_str(), "w+b");

	outFile.FileOpen();

	gTempStr = gExt_File_Name + ".hpp";
	outFile.FilePrint(
		"\n" \
		"#include \"%s\"" LF LF,
		gTempStr.c_str());

	for (u32 i = 0; i < gS6_StateFunc_Table.size(); i++) {
		std::string entryStr;
		if (0 == strcmp("NULL", gS6_StateFunc_Table[i].entryFunc.c_str())) {
			//entryStr = "NULL";
		} else if (0 == strcmp("DEFAULT", gS6_StateFunc_Table[i].entryFunc.c_str())) {
			//entryStr = gS1_StateVector[i].prefixStr + gS1_StateVector[i].stateStr + "_Entry";
		} else {
			entryStr = gS6_StateFunc_Table[i].entryFunc;
			outFile.FilePrint(
				"int %s(%s state, %s *evtPara /* = NULL */)" LF\
				"{" LF\
				HT "return 0;" LF\
				"}" LF LF,
				entryStr.c_str(),
				_STR_StateTypedefName(),
				_STR_EventParaType());
		}

		std::string leaveStr;
		if (0 == strcmp("NULL", gS6_StateFunc_Table[i].leaveFunc.c_str())) {
			//leaveStr = "NULL";
		} else if (0 == strcmp("DEFAULT", gS6_StateFunc_Table[i].leaveFunc.c_str())) {
			//leaveStr = gS1_StateVector[i].prefixStr + gS1_StateVector[i].stateStr + "_Leave";
		} else {
			leaveStr = gS6_StateFunc_Table[i].leaveFunc;
			outFile.FilePrint(
				"int %s(%s state, %s *evtPara /* = NULL */)" LF\
				"{" LF\
				HT "return 0;" LF\
				"}" LF LF,
				leaveStr.c_str(),
				_STR_StateTypedefName(),
				_STR_EventParaType());
		}
	}

	// Global Event Function
	for (u32 i = 0; i < gS2_EventVector.size(); i++) {
		if (gS2_EventVector[i].eventType == GlobalEvent) {
			if (0 == strcmp("NULL", gS2_EventVector[i].globalEventFunc.c_str())) {
				//outFile.FilePrint(HT "NULL," LF);
			} else if (0 == strcmp("DEFAULT", gS2_EventVector[i].globalEventFunc.c_str())) {
				//outFile.FilePrint(HT "%s%s_Function," LF,
				//	gS2_EventVector[i].prefixStr.c_str(),
				//	gS2_EventVector[i].eventStr.c_str());
			} else {
				outFile.FilePrint(
					"%s %s(%s &state, %s *evtPara /* = NULL */)" LF\
					"{" LF\
					HT "return state;" LF\
					"}" LF LF,
					_STR_StateTypedefName(),
					gS2_EventVector[i].globalEventFunc.c_str(),
					_STR_StateTypedefName(),
					_STR_EventParaType());
			}
		}
	}
}

int SMM3_S11_Exit_Function(SMM3_t &nextState)
{
	_Print_Ext_Header();
	_Print_Ext_Function();
	
	return 0; // Error Code, true for error.
}

