
#include "Everything_App.hpp"

typedef enum {
	ARG_KEY_INPUT_FILE,
	ARG_KEY_OUTPUT_FILE,

	ARG_KEY_DUMP_LIST_FILE,
}ARG_KEY_t;
Option_Set_Big_t gBigOptionSet[] = {
	{ARG_KEY_INPUT_FILE,  ARG_BY_SPACE,  "-i", "--input", NULL, NULL},
	{ARG_KEY_OUTPUT_FILE, ARG_BY_SPACE,  "-o", "--output", NULL, NULL},
};
Option_Set_Small_t gSmallOptionSet[] = {
	{ARG_KEY_DUMP_LIST_FILE, 'l'},
};
ArgOptionSet gArgOption;


LibFileIoClass gInputFile;
LibFileIoClass gListFile;
LibFileIoClass gOutputFile;
AsmLists gAsmLists;

static int _Assembler_1_CreateListFile_Pass_1_LineParser(void)
{
	//DUMPS(gInputFile.lineStr);
	//DUMPD(gInputFile.lineCount);
	gAsmLists.AddLine(gInputFile.lineStr);
	return 0;
}

static int _Assembler_1_CreateListFile_Pass_1(void)
{
	int retVal;

	//gInputFile.EnableFileDbgMsg();
	retVal = gInputFile.FileOpenForRead();
	RETURN_IF(retVal);
	retVal = gListFile.FileOpenForRead();
	RETURN_IF(retVal);

	while (1) {
		retVal = gInputFile.GetLine();

		if (retVal) {
			if (retVal == RC_FILE_REACH_EOF) {
				break;
			} else {
				EXIT_LOC_IF(retVal);
			}
		}

		retVal = _Assembler_1_CreateListFile_Pass_1_LineParser();
		RETURN_IF(retVal);
	}

	gAsmLists.Dump();
	
	return 0;
}

static int _Assembler_2_CreateObjFile(void)
{
	RETURN_CHK(rc, gOutputFile.FileOpenForRead());

	for (u32 i = 0; i < gAsmLists.lineVec.size(); i++) {
		
		fprintf(gOutputFile.fp, "%06X\n", gAsmLists.lineVec[i].machineCode);
	}
	
	return 0;
}

void Assembler_PrintUsage(void)
{
	printf("Usage:\n");
	printf("  as.exe -[options] [input file]\n\n");
	printf("Synopsis:\n");
	printf("  1.Output file will use the same name as \"input file\".\n");
}

int Assembler_Main(int argc, char *argv[])
{
	gArgOption.Init(ARRAY_AND_SIZE(gBigOptionSet), ARRAY_AND_SIZE(gSmallOptionSet), 1);

	rc = gArgOption.StartParsing(argc, argv);
	LibError_PrintErrorMessage(rc, false);
	RETURN_IF(rc);

	//gArgOption.Dump();

	const char *inputFile;
	if (gArgOption.CheckArgByKey(ARG_KEY_INPUT_FILE, &inputFile)) {

	} else {
		if (gArgOption.standAloneArgs.size()>0) {
			inputFile = gArgOption.standAloneArgs[0].c_str();
		} else {
			printf("Error!! No input file.\n\n");
			Assembler_PrintUsage();
			exit(1);
		}
	}
	DUMPS(inputFile);
	gInputFile.openMode = "r+b";
	gInputFile.fileName = inputFile;

	LibStringClass str;
	str.Init(inputFile);
	str.ReplaceExtension('.', "_list.txt");
	DUMPS(str.CStr());
	gListFile.openMode = "w+b";
	gListFile.fileName = str.CStr();
	
	str.Init(inputFile);
	str.ReplaceExtension('.', "_obj.txt");
	DUMPS(str.CStr());
	gOutputFile.openMode = "w+b";
	gOutputFile.fileName = str.CStr();


	int retVal;

	RETURN_CHK(retVal, _Assembler_1_CreateListFile_Pass_1());

	RETURN_CHK(retVal, _Assembler_2_CreateObjFile());

	return 0;
}

