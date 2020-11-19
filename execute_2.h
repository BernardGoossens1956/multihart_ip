#ifndef __EXECUTE_2
#define __EXECUTE_2

#include "multihart_ip.h"

void init_execute_2_status(
  execute_2_status_t *execute_2_status,
  bit_t              *execute_2_status_is_full);
void execute_2(
  execute_2_in_from_execute_1_t execute_2_in_from_execute_1,
  bit_t                        *wb_status_is_full,
  execute_2_status_t           *execute_2_status,
  execute_2_out_to_wb_t        *execute_2_out_to_wb,
  bit_t                        *execute_2_status_is_full);

#endif
