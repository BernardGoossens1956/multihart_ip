#include "debug_multihart_ip.h"
#include "multihart_ip.h"
#include "type.h"
#include "immediate.h"
#ifndef __SYNTHESIS__
#ifdef DEBUG_PIPELINE
#include <stdio.h>
#include "disassemble.h"
#endif
#endif

static void decode_instruction(
  instruction_t instruction,
  decoded_instruction_t *decoded_instruction){
#pragma HLS INLINE
  decoded_instruction->inst_31    = (instruction >> 31);
  decoded_instruction->inst_30_25 = (instruction >> 25);
  decoded_instruction->inst_24_21 = (instruction >> 21);
  decoded_instruction->inst_20    = (instruction >> 20);
  decoded_instruction->inst_19_12 = (instruction >> 12);
  decoded_instruction->inst_11_8  = (instruction >>  8);
  decoded_instruction->inst_7     = (instruction >>  7);
  decoded_instruction->inst_6_2   = (instruction >>  2);
  decoded_instruction->opcode     = (instruction >>  2);
  decoded_instruction->inst_1_0   = (instruction      );
  decoded_instruction->rd         = (instruction >>  7);
  decoded_instruction->func3      = (instruction >> 12);
  decoded_instruction->rs1        = (instruction >> 15);
  decoded_instruction->rs2        = (instruction >> 20);
  decoded_instruction->func7      = (instruction >> 25);
  decoded_instruction->is_rs1_reg =
                  (decoded_instruction->opcode != JAL   &&
                   decoded_instruction->opcode != LUI   &&
                   decoded_instruction->opcode != AUIPC &&
                   decoded_instruction->rs1    != 0);
  decoded_instruction->is_rs2_reg =
                  (decoded_instruction->opcode != OP_IMM &&
                   decoded_instruction->opcode != LOAD   &&
                   decoded_instruction->opcode != JAL    &&
                   decoded_instruction->opcode != JALR   &&
                   decoded_instruction->opcode != LUI    &&                   
                   decoded_instruction->opcode != AUIPC  &&
                   decoded_instruction->rs2    != 0);
  decoded_instruction->is_load    =
                  (decoded_instruction->opcode == LOAD);
  decoded_instruction->is_store   =
                  (decoded_instruction->opcode == STORE);
  decoded_instruction->is_branch  =
                  (decoded_instruction->opcode == BRANCH);
  decoded_instruction->is_jal     =
                  (decoded_instruction->opcode == JAL);
  decoded_instruction->is_jalr    =
                  (decoded_instruction->opcode == JALR);
  decoded_instruction->is_ret     = (instruction == RET);
  decoded_instruction->is_lui     =
                  (decoded_instruction->opcode == LUI);
  decoded_instruction->is_op_imm  =
                  (decoded_instruction->opcode == OP_IMM);
  decoded_instruction->is_system  =
                  (decoded_instruction->opcode == SYSTEM);
  decoded_instruction->is_jump    =
                  (decoded_instruction->is_jal ||
                   decoded_instruction->is_jalr);
  decoded_instruction->is_ctrl    =
                  (decoded_instruction->is_branch ||
                   decoded_instruction->is_jump);
  decoded_instruction->has_no_dest=
                  (decoded_instruction->is_branch ||
                   decoded_instruction->is_store  ||
                  (decoded_instruction->rd == 0));
  decoded_instruction->opch       =
                   decoded_instruction->opcode>>3;
  decoded_instruction->opcl       =
                   decoded_instruction->opcode;
  decoded_instruction->type       =
              type(decoded_instruction->opch,
                   decoded_instruction->opcl);
  switch(decoded_instruction->type){
    case UNDEFINED_TYPE: decoded_instruction->imm = 0;
                         break;
    case R_TYPE:         decoded_instruction->imm = 0;
                         break;
    case I_TYPE:         decoded_instruction->imm =
                           i_immediate(decoded_instruction);
                         break;
    case S_TYPE:         decoded_instruction->imm =
                           s_immediate(decoded_instruction);
                         break;
    case B_TYPE:         decoded_instruction->imm =
                           b_immediate(decoded_instruction);
                         break;
    case U_TYPE:         decoded_instruction->imm =
                           u_immediate(decoded_instruction);
                         break;
    case J_TYPE:         decoded_instruction->imm =
                           j_immediate(decoded_instruction);
                         break;
    case OTHER_TYPE:     decoded_instruction->imm = 0;
                         break;
  }
  decoded_instruction->is_r_type  =
                  (decoded_instruction->type   == R_TYPE);
}
void init_decode_status(
  decode_status_t *decode_status,
  bit_t           *decode_status_is_full){
  hart_num_p1_t h1;
  hart_num_t    h;
  for (h1=0; h1<NB_HART; h1++){
    h = h1;
    decode_status[h].is_full = 0;
    decode_status_is_full[h] = 0;
  }
}
static void get_input(
  hart_num_t             hart,
  decode_in_from_fetch_t decode_in_from_fetch,
  decode_status_t       *decode_status){
#pragma HLS INLINE
  decode_status[hart].pc          =
    decode_in_from_fetch.pc;
  decode_status[hart].instruction =
    decode_in_from_fetch.instruction;
}
static void stage_job(
  hart_num_t             hart,
  decode_status_t       *decode_status,
  decoded_instruction_t *decoded_instruction,
  code_address_t        *target_pc){
#pragma HLS INLINE
  decode_instruction(decode_status[hart].instruction,
                     decoded_instruction);
  *target_pc = decode_status[hart].pc +
    ((decoded_instruction->is_jal)   ?
     (decoded_instruction->imm << 1) :
      4);
}
static void set_output(
  hart_num_t             hart,
  decoded_instruction_t  decoded_instruction,
  code_address_t         target_pc,
  decode_status_t       *decode_status,
  decode_out_to_fetch_t *decode_out_to_fetch,
  decode_out_to_issue_t *decode_out_to_issue){
#pragma HLS INLINE
  decode_out_to_fetch->hart                = hart;
  decode_out_to_fetch->target_pc           =
    target_pc;
  decode_out_to_issue->hart                = hart;
  decode_out_to_issue->pc                  =
    decode_status[hart].pc;
  decode_out_to_issue->decoded_instruction =
    decoded_instruction;
  if (decoded_instruction.has_no_dest)
    decode_out_to_issue->decoded_instruction.rd = 0;
#ifndef __SYNTHESIS__
  decode_out_to_issue->instruction         =
    decode_status[hart].instruction;
  decode_out_to_issue->target_pc           =
    target_pc;
#endif
}
static void get_hart(
  decode_status_t *decode_status,
  bit_t           *issue_status_is_full,
  bit_t           *is_decoding,
  hart_num_t      *decoding_hart){
#pragma HLS INLINE
  hart_num_t h01, h23;
  bit_t      c01, c23;
  bit_t      c[NB_HART];
  c[0] = (decode_status[0].is_full &&
         !issue_status_is_full[0]);
  c[1] = (decode_status[1].is_full &&
         !issue_status_is_full[1]);
  c[2] = (decode_status[2].is_full &&
         !issue_status_is_full[2]);
  c[3] = (decode_status[3].is_full &&
         !issue_status_is_full[3]);
  h01 = (c[0])?0:1;
  c01 = (c[0] || c[1]);
  h23 = (c[2])?2:3;
  c23 = (c[2] || c[3]);
  *decoding_hart = (c01)?h01:h23;
  *is_decoding   = (c01 || c23);
}
void decode(
  decode_in_from_fetch_t decode_in_from_fetch,
  bit_t                 *issue_status_is_full,
  decode_status_t       *decode_status,
  decode_out_to_fetch_t *decode_out_to_fetch,
  decode_out_to_issue_t *decode_out_to_issue,
  bit_t                 *decode_status_is_full){
#pragma HLS INLINE off
  bit_t                 wait_for_target_address;
  bit_t                 is_valid;
  decoded_instruction_t decoded_instruction;
  code_address_t        target_pc;
  hart_num_t            in_hart, decoding_hart;
  bit_t                 is_decoding;
  get_hart(decode_status,
           issue_status_is_full,
          &is_decoding,
          &decoding_hart);
  decode_out_to_issue->is_valid = is_decoding;
  if (is_decoding){
    decode_status[decoding_hart].is_full = 0;
    decode_status_is_full[decoding_hart] = 0;
    stage_job(decoding_hart,
              decode_status,
             &decoded_instruction,
             &target_pc);
#ifndef __SYNTHESIS__
#ifdef DEBUG_PIPELINE
    printf("hart %d: decoded  %04d: ",
            decoding_hart,
            decode_status[decoding_hart].pc);
    disassemble(
      decode_status[decoding_hart].pc,
      decode_status[decoding_hart].instruction,
      decoded_instruction);
    if (decoded_instruction.is_jal)
      printf("      pc  = %16d (%8x)\n", (int)target_pc,
                                (unsigned int)target_pc);
#endif
#ifdef DEBUG_DETAILED_PIPELINE
    printf("hart %d: target pc %d\n",
            decoding_hart, target_pc);
#endif
#endif
    set_output(decoding_hart,
               decoded_instruction,
               target_pc,
               decode_status,
               decode_out_to_fetch,
               decode_out_to_issue);
  }
  wait_for_target_address       =
    is_decoding                   &&
   (decoded_instruction.is_branch ||
    decoded_instruction.is_jalr);
  decode_out_to_fetch->is_valid =
    is_decoding &&
   !wait_for_target_address;
  in_hart = decode_in_from_fetch.hart;
  if (decode_in_from_fetch.is_valid){
    decode_status[in_hart].is_full = 1;
    decode_status_is_full[in_hart] = 1;
    get_input(in_hart,
              decode_in_from_fetch,
              decode_status);
  }
}
