
#include "Everything_App.hpp"

typedef enum {
  ARG_KEY_INPUT_FILE,
  ARG_KEY_OUTPUT_FILE,

  ARG_KEY_DUMP_LIST_FILE,
}ARG_KEY_t;
Option_Set_Big_t gBigOptionSet[] = {
	{ARG_KEY_INPUT_FILE,  true,  "-i", "--input", NULL, NULL},
	{ARG_KEY_OUTPUT_FILE, true,  "-o", "--output", NULL, NULL},
};
Option_Set_Small_t gSmallOptionSet[] = {
	{ARG_KEY_DUMP_LIST_FILE, 'l'},
};
ArgOptionSet gArgOption;


LibFileIoClass gInputFile;
LibFileIoClass gListFile;
LibFileIoClass gOutputFile;

static int _Assembler_1_CreateListFile_Pass_1_LineParser(void)
{
	DUMPS(gInputFile.lineStr);
	DUMPD(gInputFile.lineCount);
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
				EXIT_IF_ERROR(retVal);
			}
		}

		retVal = _Assembler_1_CreateListFile_Pass_1_LineParser();
		RETURN_IF(retVal);
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
	LibError_PrintErrorMessage(rc, true);
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


	int retVal = _Assembler_1_CreateListFile_Pass_1();
	if (retVal)
		return retVal;

	return 0;
}

