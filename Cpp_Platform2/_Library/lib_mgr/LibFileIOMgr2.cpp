
#include "Everything_Lib_Mgr.hpp"

void LibFileIO2_Demo_INI_File(void)
{
	INI_File_Field_t fields[] = {
		{"", INI_VAR_HEX, NULL},
		{"", INI_VAR_HEX, NULL},
		{"", INI_VAR_HEX, NULL},
		{"", INI_VAR_HEX, NULL},
	};
	
	LibFileIoClass file("test.ini", "r+b");
	LibStringClass str;
	
	EXIT_CHK( rc, file.FileOpenForRead() );
	
	while ( 0 == file.GetLine() ) {
		str.Init(file.lineStr);
		str.RemoveRestString("//");
		str.RemoveRestString(";");
		str.RemoveEmptyPrefixChar();
		str.RemoveEmptyPostfixChar();
		if (str.Size()) {
			str.InsertBefore("=", " ");
			str.InsertAfter("=", " ");
			str.Split();
			const char *valueStr;
			int retVal;
			retVal = str.FindValueStr("Max_Size", "=", valueStr);
			DUMPD(retVal);
			if (retVal == 0) {
				DUMPS(valueStr);
				u32 val = LibString_HexStringToU32(valueStr);
				DUMPD(val);
				DUMPX(val);
			} else {
				LibError_PrintExtErrorMessage("ExtError:");
			}
			DUMPS(str.CStr());
			DUMPD(str.Size());
			DUMPD(file.lineCount);
			PRINT_NEXT_LINE;
		}
	}
}