
#ifndef _ZZTEST_HPP_

typedef enum {
	SFS_S0_1EST, // INIT STATE
	SFS_S1_2EST, //
	SFS_S2_3EST, //
	SFS_S3_4EST, // EXIT STATE
} SFS_TEST_t;

int SFS_Test_StateMachine(void);

int SFS_S0_1EST_Function(SFS_TEST_t *state);
int SFS_S1_2EST_Function(SFS_TEST_t *state);
int SFS_S2_3EST_Function(SFS_TEST_t *state);
int SFS_S3_4EST_Function(SFS_TEST_t *state);

#define _ZZTEST_HPP_
#endif //_ZZTEST_HPP_
