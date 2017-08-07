
#ifndef _SMM3_HPP_

typedef enum {
	SMM3_S0_Read_All_INI_Data     =  0, // 0x00, INIT STATE
	SMM3_S1_Extract_States        =  1, // 0x01, 
	SMM3_S2_Extract_Events        =  2, // 0x02, 
	SMM3_S3_Extract_GlobalEvents  =  3, // 0x03, 
	SMM3_S4_Make_SE_Table         =  4, // 0x04, 
	SMM3_S5_Make_InterState_Table =  5, // 0x05, 
	SMM3_S6_Make_StateFunc_Table  =  6, // 0x06, 
	SMM3_S7_Make_TestCase_Table   =  7, // 0x07, 
	SMM3_S8_Create_Header_File    =  8, // 0x08, 
	SMM3_S9_Create_Core_File      =  9, // 0x09, 
	SMM3_S10_Create_Function_File = 10, // 0x0a, 
	SMM3_S11_Exit                 = 11, // 0x0b, EXIT STATE
} SMM3_t;

int SMM3_StateMachine(void);

int SMM3_S0_Read_All_INI_Data_Function(SMM3_t &nextState);
int SMM3_S1_Extract_States_Function(SMM3_t &nextState);
int SMM3_S2_Extract_Events_Function(SMM3_t &nextState);
int SMM3_S3_Extract_GlobalEvents_Function(SMM3_t &nextState);
int SMM3_S4_Make_SE_Table_Function(SMM3_t &nextState);
int SMM3_S5_Make_InterState_Table_Function(SMM3_t &nextState);
int SMM3_S6_Make_StateFunc_Table_Function(SMM3_t &nextState);
int SMM3_S7_Make_TestCase_Table_Function(SMM3_t &nextState);
int SMM3_S8_Create_Header_File_Function(SMM3_t &nextState);
int SMM3_S9_Create_Core_File_Function(SMM3_t &nextState);
int SMM3_S10_Create_Function_File_Function(SMM3_t &nextState);
int SMM3_S11_Exit_Function(SMM3_t &nextState);

#define _SMM3_HPP_
#endif //_SMM3_HPP_

