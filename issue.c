#include "debug_multihart_ip.h"
#include "multihart_ip.h"
#ifndef __SYNTHESIS__
#ifdef DEBUG_PIPELINE
#include "stdio.h"
#endif
#endif

void init_issue_status(
  issue_status_t *issue_status,
  bit_t          *issue_status_is_full){
  hart_num_p1_t h1;
  hart_num_t    h;
  for (h1=0; h1<NB_HART; h1++){
    h = h1;
    issue_status[h].is_full = 0;
    issue_status_is_full[h] = 0;
  }
}
static void get_input(
  hart_num_t             hart,
  issue_in_from_decode_t issue_in_from_decode,
  issue_status_t        *issue_status){
#pragma HLS INLINE
  issue_status[hart].decoded_instruction =
    issue_in_from_decode.decoded_instruction;
  issue_status[hart].pc                  =
    issue_in_from_decode.pc;
#ifndef __SYNTHESIS__
  issue_status[hart].instruction         =
    issue_in_from_decode.instruction;
  issue_status[hart].target_pc           =
    issue_in_from_decode.target_pc;
#endif
}
static void stage_job(
  hart_num_t      hart,
  issue_status_t *issue_status,
  int             reg_file[][NB_REGISTER],
  int            *rv1,
  int            *rv2){
#pragma HLS INLINE
  *rv1 = reg_file[hart]
                 [issue_status[hart].decoded_instruction.rs1];
  *rv2 = reg_file[hart]
                 [issue_status[hart].decoded_instruction.rs2];
}
static void set_output(
  hart_num_t                hart,
  int                       rv1,
  int                       rv2,
  issue_status_t           *issue_status,
  issue_out_to_execute_1_t *issue_out_to_execute_1){
#pragma HLS INLINE
  issue_out_to_execute_1->hart                = hart;
  issue_out_to_execute_1->rv1                 = rv1;
  issue_out_to_execute_1->rv2                 = rv2;
  issue_out_to_execute_1->decoded_instruction =
    issue_status[hart].decoded_instruction;
  issue_out_to_execute_1->pc                  =
    issue_status[hart].pc;
#ifndef __SYNTHESIS__
  issue_out_to_execute_1->instruction         =
    issue_status[hart].instruction;
  issue_out_to_execute_1->target_pc           =
    issue_status[hart].target_pc;
#endif
}
static void get_hart(
  issue_status_t *issue_status,
  bit_t          *execute_1_status_is_full,
  bit_t           is_reg_computed[][NB_REGISTER],
  bit_t          *is_issuing,
  hart_num_t     *issuing_hart){
#pragma HLS INLINE
  hart_num_t h01, h23;
  bit_t      c01, c23;
  bit_t      c              [NB_HART];
  bit_t      is_locked_1    [NB_HART];
  bit_t      is_locked_2    [NB_HART];
  bit_t      wait_for_source[NB_HART];
  is_locked_1[0] =
    issue_status   [0].decoded_instruction.is_rs1_reg &&
    is_reg_computed[0]
                   [issue_status[0].decoded_instruction.rs1];
  is_locked_2[0] =
    issue_status   [0].decoded_instruction.is_rs2_reg &&
    is_reg_computed[0]
                   [issue_status[0].decoded_instruction.rs2];
  wait_for_source[0] = is_locked_1[0] || is_locked_2[0];
  is_locked_1[1] =
    issue_status   [1].decoded_instruction.is_rs1_reg &&
    is_reg_computed[1]
                   [issue_status[1].decoded_instruction.rs1];
  is_locked_2[1] =
    issue_status   [1].decoded_instruction.is_rs2_reg &&
    is_reg_computed[1]
                   [issue_status[1].decoded_instruction.rs2];
  wait_for_source[1] = is_locked_1[1] || is_locked_2[1];
  is_locked_1[2] =
    issue_status   [2].decoded_instruction.is_rs1_reg &&
    is_reg_computed[2]
                   [issue_status[2].decoded_instruction.rs1];
  is_locked_2[2] =
    issue_status   [2].decoded_instruction.is_rs2_reg &&
    is_reg_computed[2]
                   [issue_status[2].decoded_instruction.rs2];
  wait_for_source[2] = is_locked_1[2] || is_locked_2[2];
  is_locked_1[3] =
    issue_status   [3].decoded_instruction.is_rs1_reg &&
    is_reg_computed[3]
                   [issue_status[3].decoded_instruction.rs1];
  is_locked_2[3] =
    issue_status   [3].decoded_instruction.is_rs2_reg &&
    is_reg_computed[3]
                   [issue_status[3].decoded_instruction.rs2];
  wait_for_source[3] = is_locked_1[3] || is_locked_2[3];
  c[0] = (issue_status[0].is_full     &&
         !execute_1_status_is_full[0] &&
         !wait_for_source[0]);
  c[1] = (issue_status[1].is_full     &&
         !execute_1_status_is_full[1] &&
         !wait_for_source[1]);
  c[2] = (issue_status[2].is_full     &&
         !execute_1_status_is_full[2] &&
         !wait_for_source[2]);
  c[3] = (issue_status[3].is_full     &&
         !execute_1_status_is_full[3] &&
         !wait_for_source[3]);
  h01 = (c[0])?0:1;
  c01 = (c[0] || c[1]);
  h23 = (c[2])?2:3;
  c23 = (c[2] || c[3]);
  *issuing_hart = (c01)?h01:h23;
  *is_issuing   = (c01 || c23);
}
void issue(
  issue_in_from_decode_t    issue_in_from_decode,
  bit_t                    *execute_1_status_is_full,
  int                       reg_file       [][NB_REGISTER],
  bit_t                     is_reg_computed[][NB_REGISTER],
  issue_status_t           *issue_status,
  issue_out_to_execute_1_t *issue_out_to_execute_1,
  bit_t                    *issue_status_is_full,
  issue_out_to_lock_reg_t  *issue_out_to_lock_reg){
#pragma HLS INLINE off
  int        rv1, rv2;
  hart_num_t in_hart, issuing_hart;
  bit_t      is_issuing;
  get_hart(issue_status,
           execute_1_status_is_full,
           is_reg_computed,
          &is_issuing,
          &issuing_hart);
  issue_out_to_execute_1->is_valid = is_issuing;
  if (is_issuing){
    issue_status[issuing_hart].is_full = 0;
    issue_status_is_full[issuing_hart] = 0;
    stage_job(issuing_hart,
              issue_status,
              reg_file,
             &rv1,
             &rv2);
#ifndef __SYNTHESIS__
#ifdef DEBUG_PIPELINE
    printf("hart %d: issued   ", issuing_hart);
    printf("%04d\n",
            issue_status[issuing_hart].pc);
#endif
#ifdef DEBUG_DETAILED_PIPELINE
    printf("hart %d: rv1 %d, rv2 %d\n",
            issuing_hart, rv1, rv2);
#endif
#endif
    set_output(issuing_hart,
               rv1,
               rv2,
               issue_status,
               issue_out_to_execute_1);
  }
  issue_out_to_lock_reg->is_valid    =
    is_issuing &&
    issue_status[issuing_hart].decoded_instruction.rd != 0;
  issue_out_to_lock_reg->destination =
    issue_status[issuing_hart].decoded_instruction.rd;
  issue_out_to_lock_reg->hart        = issuing_hart;
  in_hart = issue_in_from_decode.hart;
  if (issue_in_from_decode.is_valid){
    issue_status[in_hart].is_full = 1;
    issue_status_is_full[in_hart] = 1;
    get_input(in_hart,
              issue_in_from_decode,
              issue_status);
  }
}
