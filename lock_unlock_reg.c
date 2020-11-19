#include "multihart_ip.h"

void lock_unlock_reg(
  lock_reg_in_from_issue_t lock_reg_in_from_issue,
  unlock_reg_in_from_wb_t  unlock_reg_in_from_wb,
  bit_t                    is_reg_computed[][NB_REGISTER]){
#pragma HLS INLINE off
  uint6 r;
  for (r=1; r<NB_REGISTER; r++){
    if (unlock_reg_in_from_wb.is_valid &&
        unlock_reg_in_from_wb.destination == r){
      is_reg_computed[unlock_reg_in_from_wb.hart][r] = 0;
#ifndef __SYNTHESIS__
#ifdef DEBUG_DETAILED_PIPELINE
      printf("hart %d: unlocked register %d\n", unlock_reg_in_from_wb.hart, r);
#endif
#endif
    }
    if (lock_reg_in_from_issue.is_valid &&
        lock_reg_in_from_issue.destination == r){
      is_reg_computed[lock_reg_in_from_issue.hart][r] = 1;
#ifndef __SYNTHESIS__
#ifdef DEBUG_DETAILED_PIPELINE
      printf("hart %d: locked register %d\n", lock_reg_in_from_issue.hart, r);
#endif
#endif
    }
  }
}
