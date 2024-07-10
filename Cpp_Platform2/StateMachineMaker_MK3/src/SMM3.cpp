
#include "SMM3.hpp"

SMM3_t gSMM3 = SMM3_S0_Read_All_INI_Data;

int SMM3_StateMachine(void)
{
	int retVal;
	while (1) {
		switch (gSMM3) {
			case SMM3_S0_Read_All_INI_Data: { // INIT STATE
				retVal = SMM3_S0_Read_All_INI_Data_Function(gSMM3);
				if (retVal) {
					return retVal;
				}
			} break;

			case SMM3_S1_Extract_States: {
				retVal = SMM3_S1_Extract_States_Function(gSMM3);
				if (retVal) {
					return retVal;
				}
			} break;

			case SMM3_S2_Extract_Events: {
				retVal = SMM3_S2_Extract_Events_Function(gSMM3);
				if (retVal) {
					return retVal;
				}
			} break;

			case SMM3_S3_Extract_GlobalEvents: {
				retVal = SMM3_S3_Extract_GlobalEvents_Function(gSMM3);
				if (retVal) {
					return retVal;
				}
			} break;

			case SMM3_S4_Make_SE_Table: {
				retVal = SMM3_S4_Make_SE_Table_Function(gSMM3);
				if (retVal) {
					return retVal;
				}
			} break;

			case SMM3_S5_Make_InterState_Table: {
				retVal = SMM3_S5_Make_InterState_Table_Function(gSMM3);
				if (retVal) {
					return retVal;
				}
			} break;

			case SMM3_S6_Make_StateFunc_Table: {
				retVal = SMM3_S6_Make_StateFunc_Table_Function(gSMM3);
				if (retVal) {
					return retVal;
				}
			} break;

			case SMM3_S7_Make_TestCase_Table: {
				retVal = SMM3_S7_Make_TestCase_Table_Function(gSMM3);
				if (retVal) {
					return retVal;
				}
			} break;

			case SMM3_S8_Create_Header_File: {
				retVal = SMM3_S8_Create_Header_File_Function(gSMM3);
				if (retVal) {
					return retVal;
				}
			} break;

			case SMM3_S9_Create_Core_File: {
				retVal = SMM3_S9_Create_Core_File_Function(gSMM3);
				if (retVal) {
					return retVal;
				}
			} break;

			case SMM3_S10_Create_Function_File: {
				retVal = SMM3_S10_Create_Function_File_Function(gSMM3);
				if (retVal) {
					return retVal;
				}
			} break;

			case SMM3_S11_Exit: { // EXIT STATE
				retVal = SMM3_S11_Exit_Function(gSMM3);
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
