
#include "SMM2.hpp"

SMM2_t gSMM2 = SMM2_S0_Read_INI;

int SMM2_StateMachine(void)
{
	int retVal;
	while (1) {
		switch (gSMM2) {
			case SMM2_S0_Read_INI: { // INIT STATE
				retVal = SMM2_S0_Read_INI_Function(gSMM2);
				if (retVal) {
					return retVal;
				}
			} break;

			case SMM2_S1_Extrace_States_From_Table: {
				retVal = SMM2_S1_Extrace_States_From_Table_Function(gSMM2);
				if (retVal) {
					return retVal;
				}
			} break;

			case SMM2_S2_Extrace_Events_From_Table: {
				retVal = SMM2_S2_Extrace_Events_From_Table_Function(gSMM2);
				if (retVal) {
					return retVal;
				}
			} break;

			case SMM2_S3_Make_Header_File: {
				retVal = SMM2_S3_Make_Header_File_Function(gSMM2);
				if (retVal) {
					return retVal;
				}
			} break;

			case SMM2_S4_Make_Function_File: {
				retVal = SMM2_S4_Make_Function_File_Function(gSMM2);
				if (retVal) {
					return retVal;
				}
			} break;

			case SMM2_S5_Make_Source_File_Start: {
				retVal = SMM2_S5_Make_Source_File_Start_Function(gSMM2);
				if (retVal) {
					return retVal;
				}
			} break;

			case SMM2_S6_Make_Source_File_Special_Event: {
				retVal = SMM2_S6_Make_Source_File_Special_Event_Function(gSMM2);
				if (retVal) {
					return retVal;
				}
			} break;

			case SMM2_S7_Make_Source_File_SE_SwitchCase: {
				retVal = SMM2_S7_Make_Source_File_SE_SwitchCase_Function(gSMM2);
				if (retVal) {
					return retVal;
				}
			} break;

			case SMM2_S8_Make_Source_File_End: {
				retVal = SMM2_S8_Make_Source_File_End_Function(gSMM2);
				if (retVal) {
					return retVal;
				}
			} break;

			case SMM2_S9_Exit: { // EXIT STATE
				retVal = SMM2_S9_Exit_Function(gSMM2);
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
