#ifndef __CMN_LE_LM_MAS_H__
#define __CMN_LE_LM_MAS_H__

#ifdef DFS_SIM_ON
  #include "cmn_le_lm_conn_state.h"
#else
  #include "common/le_lm/cmn_le_lm_adv.h"
#endif

void lc_mas_handle_conn_indXX(Bt_Dev_Info_t *dev_from_ini);

#endif //#define __CMN_LE_LM_MAS_H__

