#include "debug_multihart_ip.h"
#include "multihart_ip.h"
#include "compute.h"
#ifndef __SYNTHESIS__
#ifdef DEBUG_PIPELINE
#include "stdio.h"
#endif
#endif

void init_execute_2_status(
  execute_2_status_t *execute_2_status,
  bit_t              *execute_2_status_is_full){
  hart_num_p1_t h1;
  hart_num_t    h;
  for (h1=0; h1<NB_HART; h1++){
    h = h1;
    execute_2_status[h].is_full = 0;
    execute_2_status_is_full[h] = 0;
  }
}
static void get_input(
  hart_num_t                    hart,
  execute_2_in_from_execute_1_t execute_2_in_from_execute_1,
  execute_2_status_t           *execute_2_status){
#pragma HLS INLINE
  execute_2_status[hart].result1             =
    execute_2_in_from_execute_1.result1;
  execute_2_status[hart].result2             =
    execute_2_in_from_execute_1.result2;
  execute_2_status[hart].rd                  =
    execute_2_in_from_execute_1.rd;
  execute_2_status[hart].has_no_dest         =
    execute_2_in_from_execute_1.has_no_dest;
  execute_2_status[hart].is_r_type           =
    execute_2_in_from_execute_1.is_r_type;
  execute_2_status[hart].is_op_imm           =
    execute_2_in_from_execute_1.is_op_imm;
  execute_2_status[hart].is_ret              =
    execute_2_in_from_execute_1.is_ret;
#ifndef __SYNTHESIS__
  execute_2_status[hart].pc                  =
    execute_2_in_from_execute_1.pc;
  execute_2_status[hart].instruction         =
    execute_2_in_from_execute_1.instruction;
  execute_2_status[hart].decoded_instruction =
    execute_2_in_from_execute_1.decoded_instruction;
  execute_2_status[hart].target_pc           =
  execute_2_in_from_execute_1.target_pc;
#endif
}
static void stage_job(
  hart_num_t          hart,
  execute_2_status_t *execute_2_status,
  int                *result){
#pragma HLS INLINE
  *result =
   (execute_2_status[hart].is_r_type ||
    execute_2_status[hart].is_op_imm)?
    execute_2_status[hart].result1   :
    execute_2_status[hart].result2;
}
static void set_output(
  hart_num_t             hart,
  int                    result,
  execute_2_status_t    *execute_2_status,
  execute_2_out_to_wb_t *execute_2_out_to_wb){
#pragma HLS INLINE
  execute_2_out_to_wb->hart                 = hart;
  execute_2_out_to_wb->result               =
    (execute_2_status[hart].is_ret) ?
     execute_2_status[hart].result2:
     result;
  execute_2_out_to_wb->rd                   =
    execute_2_status[hart].rd;
  execute_2_out_to_wb->has_no_dest          =
    execute_2_status[hart].has_no_dest;
  execute_2_out_to_wb->is_ret               =
    execute_2_status[hart].is_ret;
#ifndef __SYNTHESIS__
  execute_2_out_to_wb->pc                   =
    execute_2_status[hart].pc;
  execute_2_out_to_wb->instruction          =
    execute_2_status[hart].instruction;
  execute_2_out_to_wb->decoded_instruction  =
    execute_2_status[hart].decoded_instruction;
  execute_2_out_to_wb->target_pc            =
    execute_2_status[hart].target_pc;
#endif
}
static void get_hart(
  execute_2_status_t *execute_2_status,
  bit_t              *wb_status_is_full,
  bit_t              *is_executing,
  hart_num_t         *executing_hart){
#pragma HLS INLINE
  hart_num_t h01, h23;
  bit_t      c01, c23;
  bit_t      c[NB_HART];
  c[0] = (execute_2_status[0].is_full &&
         !wb_status_is_full[0]);
  c[1] = (execute_2_status[1].is_full &&
         !wb_status_is_full[1]);
  c[2] = (execute_2_status[2].is_full &&
         !wb_status_is_full[2]);
  c[3] = (execute_2_status[3].is_full &&
         !wb_status_is_full[3]);
  h01 = (c[0])?0:1;
  c01 = (c[0] || c[1]);
  h23 = (c[2])?2:3;
  c23 = (c[2] || c[3]);
  *executing_hart = (c01)?h01:h23;
  *is_executing   = (c01 || c23);
}
void execute_2(
  execute_2_in_from_execute_1_t execute_2_in_from_execute_1,
  bit_t                        *wb_status_is_full,
  execute_2_status_t           *execute_2_status,
  execute_2_out_to_wb_t        *execute_2_out_to_wb,
  bit_t                        *execute_2_status_is_full){
#pragma HLS INLINE off
  int        result;
  hart_num_t in_hart, executing_hart;
  bit_t      is_executing;
  get_hart(execute_2_status,
           wb_status_is_full,
          &is_executing,
          &executing_hart);
  execute_2_out_to_wb->is_valid = is_executing;
  if (is_executing){
    execute_2_status[executing_hart].is_full = 0;
    execute_2_status_is_full[executing_hart] = 0;
    stage_job(executing_hart,
              execute_2_status,
             &result);
#ifndef __SYNTHESIS__
#ifdef DEBUG_PIPELINE
    printf("hart %d: exe_ed_2 ", executing_hart);
    printf("%04d\n", execute_2_status[executing_hart].pc);
#endif
#ifdef DEBUG_DETAILED_PIPELINE
    printf("hart %d: result %d\n",
            executing_hart, result);
#endif
#endif
    set_output(executing_hart,
               result,
               execute_2_status,
               execute_2_out_to_wb);
  }
  in_hart = execute_2_in_from_execute_1.hart;
  if (execute_2_in_from_execute_1.is_valid){
    execute_2_status[in_hart].is_full = 1;
    execute_2_status_is_full[in_hart] = 1;
    get_input(in_hart,
              execute_2_in_from_execute_1,
              execute_2_status);
  }
}
