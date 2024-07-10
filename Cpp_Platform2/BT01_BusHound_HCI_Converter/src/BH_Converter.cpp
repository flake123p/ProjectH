
#include "BH_Converter.hpp"

BH_CONVERTER_t gBH_Converter = BHC_S0_Init;

int BH_Converter_StateMachine(void)
{
	int retVal;
	while (1) {
		switch (gBH_Converter) {
			case BHC_S0_Init: { // INIT STATE
				retVal = BHC_S0_Init_Function(&gBH_Converter);
				if (retVal) {
					return retVal;
				}
			} break;

			case BHC_S1_Cut_Useless_Header: {
				retVal = BHC_S1_Cut_Useless_Header_Function(&gBH_Converter);
				if (retVal) {
					return retVal;
				}
			} break;

			case BHC_S2_Judge_Is_First_Or_Continue: {
				retVal = BHC_S2_Judge_Is_First_Or_Continue_Function(&gBH_Converter);
				if (retVal) {
					return retVal;
				}
			} break;

			case BHC_S3_Parsing_First_Line_Section: {
				retVal = BHC_S3_Parsing_First_Line_Section_Function(&gBH_Converter);
				if (retVal) {
					return retVal;
				}
			} break;

			case BHC_S4_Parsing_Continue_Line_Section: {
				retVal = BHC_S4_Parsing_Continue_Line_Section_Function(&gBH_Converter);
				if (retVal) {
					return retVal;
				}
			} break;

			case BHC_S5_Exit: { // EXIT STATE
				retVal = BHC_S5_Exit_Function(&gBH_Converter);
				if (retVal) {
					return retVal;
				}
				goto END_OF_WHILE_1;
			} break;
		}
	}
END_OF_WHILE_1:
	;

	return 0;
}
