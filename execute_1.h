#ifndef __EXECUTE_1
#define __EXECUTE_1

#include "multihart_ip.h"

void init_execute_1_status(
  execute_1_status_t *execute_1_status,
  bit_t              *execute_1_status_is_full);
void execute_1(
  execute_1_in_from_issue_t     execute_1_in_from_issue,
  bit_t                        *execute_2_status_is_full,
  bit_t                        *mem_status_is_full,
#ifndef __SYNTHESIS__
  int                           reg_file[][NB_REGISTER],
#endif
  execute_1_status_t           *execute_1_status,
  execute_1_out_to_fetch_t     *execute_1_out_to_fetch,
  execute_1_out_to_mem_t       *execute_1_out_to_mem,
  execute_1_out_to_execute_2_t *execute_1_out_to_execute_2,
  bit_t                        *execute_1_status_is_full);

#endif
