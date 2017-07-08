
#ifndef _BH_CONVERTER_HPP_

typedef enum {
	BHC_S0_Init                          =  0, // 0x00, INIT STATE
	BHC_S1_Cut_Useless_Header            =  1, // 0x01, 
	BHC_S2_Judge_Is_First_Or_Continue    =  2, // 0x02, 
	BHC_S3_Parsing_First_Line_Section    =  3, // 0x03, 
	BHC_S4_Parsing_Continue_Line_Section =  4, // 0x04, 
	BHC_S5_Exit                          =  5, // 0x05, EXIT STATE
} BH_CONVERTER_t;

int BH_Converter_StateMachine(void);

int BHC_S0_Init_Function(BH_CONVERTER_t *state);
int BHC_S1_Cut_Useless_Header_Function(BH_CONVERTER_t *state);
int BHC_S2_Judge_Is_First_Or_Continue_Function(BH_CONVERTER_t *state);
int BHC_S3_Parsing_First_Line_Section_Function(BH_CONVERTER_t *state);
int BHC_S4_Parsing_Continue_Line_Section_Function(BH_CONVERTER_t *state);
int BHC_S5_Exit_Function(BH_CONVERTER_t *state);

#define _BH_CONVERTER_HPP_
#endif //_BH_CONVERTER_HPP_
