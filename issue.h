#ifndef __ISSUE
#define __ISSUE

#include "multihart_ip.h"

void init_issue_status(
  issue_status_t *issue_status,
  bit_t          *issue_status_is_full);
void issue(
  issue_in_from_decode_t    issue_in_from_decode,
  bit_t                    *execute_1_status_is_full,
  int                       reg_file       [][NB_REGISTER],
  bit_t                     is_reg_computed[][NB_REGISTER],
  issue_status_t           *issue_status,
  issue_out_to_execute_1_t *issue_out_to_execute_1,
  bit_t                    *issue_status_is_full,
  issue_out_to_lock_reg_t  *issue_out_to_lock_reg);

#endif
