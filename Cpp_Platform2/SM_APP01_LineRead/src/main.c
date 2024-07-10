
#include "Everything_App.hpp"

#include "LineReadMachine.hpp"

int main(int argc, char *argv[])
{
	LRM_EVT_EvtPara_t evtPara = {"BH_Out.txt"};
	//evtPara.inputFileName = "BH_Out.txt";
	
	EXIT_CHK( rc, LRM_StateMachine(LRM_EVT_E4_SetInputFile, &evtPara) );
	EXIT_CHK( rc, LRM_StateMachine(LRM_EVT_E3_GetLineObj, &evtPara) );

	while (LRM_EC_End_Of_File != LRM_StateMachine(LRM_EVT_E1_ReadLine)) {
		DUMPS((*(evtPara.lineObj)).lineStr);
	}

	return 0;
}

