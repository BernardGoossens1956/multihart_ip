#ifndef __UPDATE
#define __UPDATE
#include "multihart_ip.h"

void lock_unlock_reg(
  lock_reg_in_from_issue_t lock_reg_in_from_issue,
  unlock_reg_in_from_wb_t  unlock_reg_in_from_wb,
  bit_t                    is_reg_computed[][NB_REGISTER]);

#endif
