
#ifndef _SMM2_HPP_

typedef enum {
	SMM2_S0_Read_INI                       =  0, // 0x00, INIT STATE
	SMM2_S1_Extrace_States_From_Table      =  1, // 0x01, 
	SMM2_S2_Extrace_Events_From_Table      =  2, // 0x02, 
	SMM2_S3_Make_Header_File               =  3, // 0x03, 
	SMM2_S4_Make_Function_File             =  4, // 0x04, 
	SMM2_S5_Make_Source_File_Start         =  5, // 0x05, 
	SMM2_S6_Make_Source_File_Special_Event =  6, // 0x06, 
	SMM2_S7_Make_Source_File_SE_SwitchCase =  7, // 0x07, 
	SMM2_S8_Make_Source_File_End           =  8, // 0x08, 
	SMM2_S9_Exit                           =  9, // 0x09, EXIT STATE
} SMM2_t;

int SMM2_StateMachine(void);

int SMM2_S0_Read_INI_Function(SMM2_t &nextState);
int SMM2_S1_Extrace_States_From_Table_Function(SMM2_t &nextState);
int SMM2_S2_Extrace_Events_From_Table_Function(SMM2_t &nextState);
int SMM2_S3_Make_Header_File_Function(SMM2_t &nextState);
int SMM2_S4_Make_Function_File_Function(SMM2_t &nextState);
int SMM2_S5_Make_Source_File_Start_Function(SMM2_t &nextState);
int SMM2_S6_Make_Source_File_Special_Event_Function(SMM2_t &nextState);
int SMM2_S7_Make_Source_File_SE_SwitchCase_Function(SMM2_t &nextState);
int SMM2_S8_Make_Source_File_End_Function(SMM2_t &nextState);
int SMM2_S9_Exit_Function(SMM2_t &nextState);

#define _SMM2_HPP_
#endif //_SMM2_HPP_

