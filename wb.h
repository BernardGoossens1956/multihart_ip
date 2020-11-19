#ifndef __WB
#define __WB

#include "multihart_ip.h"

void init_wb_status(wb_status_t *wb_status,
                    bit_t       *wb_status_is_full);
void write_back(
  wb_in_from_execute_2_t  wb_in_from_execute_2,
  wb_in_from_mem_t        wb_in_from_mem,
  int                     reg_file       [][NB_REGISTER],
  wb_status_t            *wb_status,
  bit_t                  *wb_status_is_full,
  wb_out_to_unlock_reg_t *wb_out_to_unlock_reg);

#endif
