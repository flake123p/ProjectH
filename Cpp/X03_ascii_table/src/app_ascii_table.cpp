
#include <stdio.h>
#include <string.h>

#include "My_Basics.hpp"

#include "LibArgParser.hpp"
#include "LibString.hpp"

#include "app_ascii_table.hpp"

void PrintUsage(void)
{
	printf("Usage:\n");
	printf("  atbl -[options]\n");
	printf("Options:\n");
	printf("  -a : Print all.\n");
	printf("  -c : Print control characters.\n");
	printf("  -p : Print printable characters.\n");
}

int AppAsciiTable(int argc, char *argv[])
{
	LibArgParser_ClearCharOption();
	LibArgParser_CharOptionParserEx(argc, argv, "acp");

	if (NOT(LibArgParser_DoesCharOptionExist())) {
		PrintUsage();
		return 1;
	}
	
	if (LibArgParser_CheckCharOption('a')) {
		LibString_DumpControlChar();
		LibString_DumpPrintableChar();
		return 0;
	}

	if (LibArgParser_CheckCharOption('c')) {
		LibString_DumpControlChar();
		//no return here
	}

	if (LibArgParser_CheckCharOption('p')) {
		LibString_DumpPrintableChar();
		//no return here
	}

	return 0;
}
