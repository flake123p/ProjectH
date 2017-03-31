
#include "Everything_App.hpp"

LibFileIoClass gInputFile;

static int _Assembler_Pass_1_LineParser(void)
{
	return 0;
}

static int _Assembler_Pass_1(void)
{
	int retVal;

	//gInputFile.EnableFileDbgMsg();
	retVal = gInputFile.FileOpenForRead();
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

		DUMPS(gInputFile.lineStr);

		retVal = _Assembler_Pass_1_LineParser();
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
	switch (argc) {
		case 1:
			printf("Error, arguments are too few. (Only:%d)\n", argc);
			Assembler_PrintUsage();
			return 1;
		break;

		case 2:
			#if 0
			if (LibArgParser_CharOptionParserEx(argc, argv, "x")) {
				printf("Error, arguments are too few. (Only:%d, No Inpu File.)\n", argc);
				Assembler_PrintUsage();
				return 1;
			}
			#endif
			gInputFile.openMode = "r+b";
			gInputFile.fileName = argv[1];
		break;

		case 3:
			gInputFile.openMode = "r+b";
			gInputFile.fileName = argv[2];
		break;

		default:
			printf("Error, arguments are too much. (Only:%d)\n", argc);
			Assembler_PrintUsage();
			return 1;
		break;
	}

	int retVal;
	
	retVal = _Assembler_Pass_1();
	if (retVal)
		return retVal;

	return 0;
}

