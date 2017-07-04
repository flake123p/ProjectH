
#include "zztest.hpp"

SFS_TEST_t gSFS_Test = SFS_S0_1EST;

int SFS_Test_StateMachine(void)
{
	int retVal;
	while (1) {
		switch (gSFS_Test) {
		case SFS_S0_1EST: { // INIT STATE
			retVal = SFS_S0_1EST_Function(&gSFS_Test);
			if (retVal) {
				return retVal;
		} break;

		case SFS_S1_2EST: {
			retVal = SFS_S1_2EST_Function(&gSFS_Test);
			if (retVal) {
				return retVal;
		} break;

		case SFS_S2_3EST: {
			retVal = SFS_S2_3EST_Function(&gSFS_Test);
			if (retVal) {
				return retVal;
		} break;

		case SFS_S3_4EST: { // EXIT STATE
			retVal = SFS_S3_4EST_Function(&gSFS_Test);
			if (retVal) {
				return retVal;
			goto END_OF_WHILE_1;
		} break;
	}
END_OF_WHILE_1:
	;

	return 0;
}
