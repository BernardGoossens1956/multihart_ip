#include "debug_multihart_ip.h"
#include "multihart_ip.h"
#include "new_cycle.h"
#include "fetch.h"
#include "decode.h"
#include "issue.h"
#include "execute_1.h"
#include "execute_2.h"
#include "mem_access.h"
#include "wb.h"
#include "lock_unlock_reg.h"
#ifndef __SYNTHESIS__
#ifdef DEBUG_PIPELINE
#include <stdio.h>
#endif
#endif

static void init_file(
  int    reg_file       [][NB_REGISTER],
  bit_t  is_reg_computed[][NB_REGISTER]){
#pragma HLS INLINE off
  hart_num_p1_t h1;
  hart_num_t    h;
  reg_num_p1_t  r1;
  reg_num_t     r;
  for (h1=0; h1<NB_HART; h1++){
    h = h1;
    for (r1=0; r1<NB_REGISTER; r1++){
      r = r1;
      is_reg_computed[h][r] = 0;
      reg_file       [h][r] = 0;
    }
  }
}
static void stat_and_exit_cond(
  execute_1_in_from_issue_t execute_1_in_from_issue,
  wb_status_t              *wb_status,
  counter_t                *nbi,
  counter_t                *nbc,
  bit_t                    *exit_cond){
#pragma HLS INLINE off
  *nbc = *nbc + 1;
  if (execute_1_in_from_issue.is_valid)
    *nbi = *nbi + 1;
  *exit_cond =
    wb_status[0].has_exited &&
    wb_status[1].has_exited &&
    wb_status[2].has_exited &&
    wb_status[3].has_exited;
}
void multihart_ip(
  unsigned int  pc,
  unsigned int  code_mem
                [CODE_MEM_SIZE/sizeof(instruction_t)],
  unsigned char data_mem_0[DATA_MEM_SIZE/sizeof(int)],
  unsigned char data_mem_1[DATA_MEM_SIZE/sizeof(int)],
  unsigned char data_mem_2[DATA_MEM_SIZE/sizeof(int)],
  unsigned char data_mem_3[DATA_MEM_SIZE/sizeof(int)],
  unsigned int *end_signal){
#pragma HLS INTERFACE s_axilite port=pc
#pragma HLS INTERFACE s_axilite port=code_mem
#pragma HLS INTERFACE s_axilite port=data_mem_0
#pragma HLS INTERFACE s_axilite port=data_mem_1
#pragma HLS INTERFACE s_axilite port=data_mem_2
#pragma HLS INTERFACE s_axilite port=data_mem_3
#pragma HLS INTERFACE s_axilite port=end_signal
#pragma HLS INTERFACE ap_ctrl_none port=return
  int   reg_file       [NB_HART][NB_REGISTER];
#pragma HLS ARRAY_PARTITION variable=reg_file\
  dim=0 complete
  bit_t is_reg_computed[NB_HART][NB_REGISTER];
#pragma HLS ARRAY_PARTITION variable=is_reg_computed\
  dim=0 complete
  fetch_in_from_decode_t        fetch_in_from_decode;
  fetch_in_from_execute_1_t     fetch_in_from_execute_1;
  fetch_status_t
    fetch_status            [NB_HART];
#pragma HLS ARRAY_PARTITION variable=fetch_status\
  dim=1 complete
  fetch_out_to_decode_t         fetch_out_to_decode;
  decode_in_from_fetch_t        decode_in_from_fetch;
  bit_t
    decode_status_is_full   [NB_HART];
  decode_status_t
    decode_status           [NB_HART];
#pragma HLS ARRAY_PARTITION variable=decode_status\
  dim=1 complete
  decode_out_to_fetch_t         decode_out_to_fetch;
  decode_out_to_issue_t         decode_out_to_issue;
  issue_in_from_decode_t        issue_in_from_decode;
  bit_t
    issue_status_is_full    [NB_HART];
  issue_status_t
    issue_status            [NB_HART];
#pragma HLS ARRAY_PARTITION variable=issue_status\
  dim=1 complete
  issue_out_to_execute_1_t      issue_out_to_execute_1;
  issue_out_to_lock_reg_t       issue_out_to_lock_reg;
  execute_1_in_from_issue_t     execute_1_in_from_issue;
  bit_t
    execute_1_status_is_full[NB_HART];
  execute_1_status_t
    execute_1_status        [NB_HART];
#pragma HLS ARRAY_PARTITION variable=execute_1_status\
  dim=1 complete
  execute_1_out_to_fetch_t      execute_1_out_to_fetch;
  execute_1_out_to_mem_t        execute_1_out_to_mem;
  execute_1_out_to_execute_2_t  execute_1_out_to_execute_2;
  execute_2_in_from_execute_1_t execute_2_in_from_execute_1;
  bit_t
    execute_2_status_is_full[NB_HART];
  execute_2_status_t
    execute_2_status        [NB_HART];
#pragma HLS ARRAY_PARTITION variable=execute_2_status\
  dim=1 complete
  execute_2_out_to_wb_t         execute_2_out_to_wb;
  mem_in_from_execute_1_t       mem_in_from_execute_1;
  bit_t
    mem_status_is_full      [NB_HART];
  mem_status_t
    mem_status              [NB_HART];
#pragma HLS ARRAY_PARTITION variable=mem_status\
  dim=1 complete
  mem_out_to_wb_t               mem_out_to_wb;
  wb_in_from_execute_2_t        wb_in_from_execute_2;
  wb_in_from_mem_t              wb_in_from_mem;
  bit_t
    wb_status_is_full       [NB_HART];
  wb_status_t
    wb_status               [NB_HART];
#pragma HLS ARRAY_PARTITION variable=wb_status\
  dim=1 complete
  wb_out_to_unlock_reg_t        wb_out_to_unlock_reg;
  lock_reg_in_from_issue_t      lock_reg_in_from_issue;
  unlock_reg_in_from_wb_t       unlock_reg_in_from_wb;
  counter_t                     nbi;
  counter_t                     nbc;
  bit_t                         exit_cond;
  init_fetch_status    (fetch_status, pc);
  init_decode_status   (decode_status,
                        decode_status_is_full);
  init_issue_status    (issue_status,
                        issue_status_is_full);
  init_execute_1_status(execute_1_status,
                        execute_1_status_is_full);
  init_execute_2_status(execute_2_status,
                        execute_2_status_is_full);
  init_mem_status      (mem_status,
                        mem_status_is_full);
  init_wb_status       (wb_status,
                        wb_status_is_full);
  init_file            (reg_file,
                        is_reg_computed);
  fetch_out_to_decode.is_valid        = 0;
  decode_out_to_fetch.is_valid        = 0;
  decode_out_to_issue.is_valid        = 0;
  issue_out_to_execute_1.is_valid     = 0;
  issue_out_to_lock_reg.is_valid      = 0;
  execute_1_out_to_fetch.is_valid     = 0;
  execute_1_out_to_mem.is_valid       = 0;
  execute_1_out_to_execute_2.is_valid = 0;
  execute_2_out_to_wb.is_valid        = 0;
  mem_out_to_wb.is_valid              = 0;
  wb_out_to_unlock_reg.is_valid       = 0;
  nbi                                 = 0;
  nbc                                 = 0;
  exit_cond                           = 0;
  //int iter;
  //for (iter=0; iter<1024; iter++){
  do {
#pragma HLS PIPELINE II=3
#ifndef __SYNTHESIS__
#ifdef DEBUG_PIPELINE
    printf("==============================================\n");
    printf("cycle %d\n", (unsigned int)nbc);
#endif
#endif
    new_cycle(fetch_out_to_decode,
              decode_out_to_fetch,
              decode_out_to_issue,
              issue_out_to_execute_1,
              issue_out_to_lock_reg,
              execute_1_out_to_fetch,
              execute_1_out_to_execute_2,
              execute_1_out_to_mem,
              execute_2_out_to_wb,
              mem_out_to_wb,
              wb_out_to_unlock_reg,
             &fetch_in_from_decode,
             &fetch_in_from_execute_1,
             &decode_in_from_fetch,
             &issue_in_from_decode,
             &execute_1_in_from_issue,
             &execute_2_in_from_execute_1,
             &mem_in_from_execute_1,
             &wb_in_from_execute_2,
             &wb_in_from_mem,
             &lock_reg_in_from_issue,
             &unlock_reg_in_from_wb);
    stat_and_exit_cond(execute_1_in_from_issue,
                       wb_status,
                      &nbi,
                      &nbc,
                      &exit_cond);
    fetch(fetch_in_from_decode,
          fetch_in_from_execute_1,
          decode_status_is_full,
          code_mem,
          fetch_status,
         &fetch_out_to_decode);
    decode(decode_in_from_fetch,
           issue_status_is_full,
           decode_status,
          &decode_out_to_fetch,
          &decode_out_to_issue,
           decode_status_is_full);
    issue(issue_in_from_decode,
          execute_1_status_is_full,
          reg_file,
          is_reg_computed,
          issue_status,
         &issue_out_to_execute_1,
          issue_status_is_full,
         &issue_out_to_lock_reg);
    execute_1(execute_1_in_from_issue,
              execute_2_status_is_full,
              mem_status_is_full,
#ifndef __SYNTHESIS__
              reg_file,
#endif
              execute_1_status,
             &execute_1_out_to_fetch,
             &execute_1_out_to_mem,
             &execute_1_out_to_execute_2,
              execute_1_status_is_full);
    execute_2(execute_2_in_from_execute_1,
              wb_status_is_full,
              execute_2_status,
             &execute_2_out_to_wb,
              execute_2_status_is_full);
    mem_access(mem_in_from_execute_1,
               wb_status_is_full,
               data_mem_0,
               data_mem_1,
               data_mem_2,
               data_mem_3,
               mem_status,
              &mem_out_to_wb,
               mem_status_is_full);
    write_back(wb_in_from_execute_2,
               wb_in_from_mem,
               reg_file,
               wb_status,
               wb_status_is_full,
              &wb_out_to_unlock_reg);
    lock_unlock_reg(lock_reg_in_from_issue,
                    unlock_reg_in_from_wb,
                    is_reg_computed);
    //if (exit_cond) break;
  } while (!exit_cond);
#ifndef __SYNTHESIS__
#ifdef DEBUG_PIPELINE
  printf("==============================================\n");
#endif
#endif
  data_mem_0[64] = nbc;
  data_mem_1[64] = nbc>>8;
  data_mem_2[64] = nbc>>16;
  data_mem_3[64] = nbc>>24;
  data_mem_0[65] = nbi;
  data_mem_1[65] = nbi>>8;
  data_mem_2[65] = nbi>>16;
  data_mem_3[65] = nbi>>24;
  *end_signal    = 1;
}
