#include "debug_multihart_ip.h"
#include "multihart_ip.h"
#include "compute.h"
#ifndef __SYNTHESIS__
#ifdef DEBUG_PIPELINE
#include "stdio.h"
#endif
#endif

void init_execute_1_status(
  execute_1_status_t *execute_1_status,
  bit_t              *execute_1_status_is_full){
  hart_num_p1_t h1;
  hart_num_t    h;
  for (h1=0; h1<NB_HART; h1++){
    h = h1;
    execute_1_status[h].is_full = 0;
    execute_1_status_is_full[h] = 0;
  }
}
static void get_input(
  hart_num_t                hart,
  execute_1_in_from_issue_t execute_1_in_from_issue,
  execute_1_status_t       *execute_1_status){
#pragma HLS INLINE
  execute_1_status[hart].rv1                 =
    execute_1_in_from_issue.rv1;
  execute_1_status[hart].rv2                 =
    execute_1_in_from_issue.rv2;
  execute_1_status[hart].pc                  =
    execute_1_in_from_issue.pc;
  execute_1_status[hart].decoded_instruction =
    execute_1_in_from_issue.decoded_instruction;
#ifndef __SYNTHESIS__
  execute_1_status[hart].target_pc           =
    execute_1_in_from_issue.target_pc;
  execute_1_status[hart].instruction         =
    execute_1_in_from_issue.instruction;
#endif
}
static void compute(
  hart_num_t          hart,
  execute_1_status_t *execute_1_status,
  bit_t              *bcond,
  int                *result1,
  int                *result2,
  code_address_t     *target_pc){
#pragma HLS INLINE
  *bcond     = compute_branch_result(
    execute_1_status[hart].rv1,
    execute_1_status[hart].rv2,
    execute_1_status[hart].decoded_instruction.func3);
  *result1   = compute_op_result(
    execute_1_status[hart].rv1,
    execute_1_status[hart].rv2,
    execute_1_status[hart].decoded_instruction);
  *result2   = compute_result(
    execute_1_status[hart].rv1,
    execute_1_status[hart].pc,
    execute_1_status[hart].decoded_instruction);
  *target_pc = compute_next_pc(
    execute_1_status[hart].pc,
    execute_1_status[hart].decoded_instruction,
    execute_1_status[hart].rv1);
}
static void stage_job(
  hart_num_t          hart,
  execute_1_status_t *execute_1_status,
  bit_t               bcond,
  code_address_t      tpc,
  code_address_t     *target_pc){
#pragma HLS INLINE
  if (!execute_1_status[hart].decoded_instruction.is_jal)
    *target_pc =
     (bcond ||
      execute_1_status[hart].decoded_instruction.is_jalr) ?
      tpc                                      :
      execute_1_status[hart].pc + 4;
}
static void set_output(
  hart_num_t                    hart,
  int                           result1,
  int                           result2,
  code_address_t                target_pc,
  execute_1_status_t           *execute_1_status,
  execute_1_out_to_fetch_t     *execute_1_out_to_fetch,
  execute_1_out_to_mem_t       *execute_1_out_to_mem,
  execute_1_out_to_execute_2_t *execute_1_out_to_execute_2){
#pragma HLS INLINE
  execute_1_out_to_fetch->hart              = hart;
  execute_1_out_to_fetch->target_pc         =
    target_pc;
  execute_1_out_to_mem->hart                = hart;
  execute_1_out_to_mem->a01                 =
    result2;
  execute_1_out_to_mem->a1                  =
   (result2 >> 1);
  execute_1_out_to_mem->a2                  =  
   (result2 >> 2) + hart * DATA_MEM_SIZE/(4*NB_HART);
  execute_1_out_to_mem->value_0             =
    execute_1_status[hart].rv2;
  execute_1_out_to_mem->value_1             =
    execute_1_status[hart].rv2 >> 8;
  execute_1_out_to_mem->value_2             =
    execute_1_status[hart].rv2 >> 16;
  execute_1_out_to_mem->value_3             =
    execute_1_status[hart].rv2 >> 24;
  execute_1_out_to_mem->rd                  =
    execute_1_status[hart].decoded_instruction.rd;
  execute_1_out_to_mem->has_no_dest         =
    execute_1_status[hart].decoded_instruction.has_no_dest;
  execute_1_out_to_mem->is_load             =
    execute_1_status[hart].decoded_instruction.is_load;
  execute_1_out_to_mem->is_store            =
    execute_1_status[hart].decoded_instruction.is_store;
  execute_1_out_to_mem->func3               =
    execute_1_status[hart].decoded_instruction.func3;
#ifndef __SYNTHESIS__
  execute_1_out_to_mem->pc                  =
    execute_1_status[hart].pc;
  execute_1_out_to_mem->instruction         =
    execute_1_status[hart].instruction;
  execute_1_out_to_mem->decoded_instruction =
    execute_1_status[hart].decoded_instruction;
#endif
  execute_1_out_to_execute_2->hart                = hart;
  execute_1_out_to_execute_2->result1             =
    result1;
  execute_1_out_to_execute_2->result2             =
   (execute_1_status[hart].decoded_instruction.is_ret)?
    target_pc                                    :
    result2;
  execute_1_out_to_execute_2->rd                  =
    execute_1_status[hart].decoded_instruction.rd;
  execute_1_out_to_execute_2->has_no_dest         =
    execute_1_status[hart].decoded_instruction.has_no_dest;
  execute_1_out_to_execute_2->is_r_type           =
    execute_1_status[hart].decoded_instruction.is_r_type;
  execute_1_out_to_execute_2->is_op_imm           =
    execute_1_status[hart].decoded_instruction.is_op_imm;
  execute_1_out_to_execute_2->is_ret              =
    execute_1_status[hart].decoded_instruction.is_ret;
#ifndef __SYNTHESIS__
  execute_1_out_to_execute_2->pc                  =
    execute_1_status[hart].pc;
  execute_1_out_to_execute_2->instruction         =
    execute_1_status[hart].instruction;
  execute_1_out_to_execute_2->decoded_instruction =
    execute_1_status[hart].decoded_instruction;
  execute_1_out_to_execute_2->target_pc           =
   (execute_1_status[hart].decoded_instruction.is_jal)?
    execute_1_status[hart].target_pc:
    target_pc;
#endif
}
static void get_hart(
  execute_1_status_t *execute_1_status,
  bit_t              *execute_2_status_is_full,
  bit_t              *mem_status_is_full,
  bit_t              *is_executing,
  hart_num_t         *executing_hart){
#pragma HLS INLINE
  hart_num_t h01, h23;
  bit_t      c01, c23;
  bit_t      c        [NB_HART];
  bit_t      is_branch[NB_HART];
  is_branch[0] =
    execute_1_status[0].decoded_instruction.is_branch;
  is_branch[1] =
    execute_1_status[1].decoded_instruction.is_branch;
  is_branch[2] = 
    execute_1_status[2].decoded_instruction.is_branch;
  is_branch[3] = 
    execute_1_status[3].decoded_instruction.is_branch;
  c[0] = (execute_1_status[0].is_full &&
         (is_branch[0]                ||
        (!mem_status_is_full[0]       &&
         !execute_2_status_is_full[0])));
  c[1] = (execute_1_status[1].is_full &&
         (is_branch[1]                ||
        (!mem_status_is_full[1]       &&
         !execute_2_status_is_full[1])));
  c[2] = (execute_1_status[2].is_full &&
         (is_branch[2]                ||
        (!mem_status_is_full[2]       &&
         !execute_2_status_is_full[2])));
  c[3] = (execute_1_status[3].is_full &&
         (is_branch[3]                ||
        (!mem_status_is_full[3]       &&
         !execute_2_status_is_full[3])));
  h01 = (c[0])?0:1;
  c01 = (c[0] || c[1]);
  h23 = (c[2])?2:3;
  c23 = (c[2] || c[3]);
  *executing_hart = (c01)?h01:h23;
  *is_executing   = (c01 || c23);
}
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
  bit_t                        *execute_1_status_is_full){
#pragma HLS INLINE off
  bit_t          bcond;
  int            result1, result2;
  code_address_t tpc, target_pc;
  hart_num_t     in_hart, executing_hart;
  bit_t          is_executing;
  get_hart(execute_1_status,
           execute_2_status_is_full,
           mem_status_is_full,
          &is_executing,
          &executing_hart);
  compute(
    executing_hart,
    execute_1_status,
   &bcond,
   &result1,
   &result2,
   &tpc);
  execute_1_out_to_fetch->is_valid =
    is_executing              &&
   (execute_1_status[executing_hart].\
decoded_instruction.is_branch ||
   (execute_1_status[executing_hart].\
decoded_instruction.is_jalr   &&
  (!execute_1_status[executing_hart].\
decoded_instruction.is_ret    ||
    tpc != 0)));
  execute_1_out_to_mem->is_valid   =
    is_executing              &&
   (execute_1_status[executing_hart].\
decoded_instruction.is_load   ||
    execute_1_status[executing_hart].\
decoded_instruction.is_store);
  execute_1_out_to_execute_2->is_valid =
    is_executing              &&
   !execute_1_status[executing_hart].\
decoded_instruction.is_load   &&
   !execute_1_status[executing_hart].\
decoded_instruction.is_store  &&
   !execute_1_status[executing_hart].\
decoded_instruction.is_branch;
  if (is_executing){
    execute_1_status[executing_hart].is_full = 0;
    execute_1_status_is_full[executing_hart] = 0;
    stage_job(executing_hart,
              execute_1_status,
              bcond,
              tpc,
             &target_pc);
#ifndef __SYNTHESIS__
#ifdef DEBUG_PIPELINE
    printf("hart %d: exe_ed_1 ", executing_hart);
    printf("%04d\n", execute_1_status[executing_hart].pc);
    if (execute_1_status[executing_hart].decoded_instruction.is_branch ||
        execute_1_status[executing_hart].decoded_instruction.is_jalr)
      printf("      pc  = %16d (%8x)\n", (int)target_pc,
                                (unsigned int)target_pc);
#else
    if (execute_1_status[executing_hart].decoded_instruction.is_branch){
#ifdef DEBUG_FETCH
      printf("hart %d: ", executing_hart);
      printf("%04d: %08x      ",
        (unsigned int)execute_1_status[executing_hart].pc,
        (unsigned int)execute_1_status[executing_hart].instruction);
#ifndef DEBUG_DISASSEMBLE
      printf("\n");
#endif
#endif
#ifdef DEBUG_DISASSEMBLE
      disassemble(execute_1_status[executing_hart].pc,
                  execute_1_status[executing_hart].instruction,
                  execute_1_status[executing_hart].decoded_instruction);
#endif
#ifdef DEBUG_EMULATE
      printf("hart %d: ", executing_hart);
      printf("      pc  = %16d (%8x)\n", (int)target_pc,
                                (unsigned int)target_pc);
#endif
    }
#endif
#ifdef DEBUG_DETAILED_PIPELINE
    printf("hart %d: result1 %d, result2 %d, target pc %d\n",
            executing_hart, result1, result2, target_pc);
#endif
#endif
    set_output(executing_hart,
               result1,
               result2,
               target_pc,
               execute_1_status,
               execute_1_out_to_fetch,
               execute_1_out_to_mem,
               execute_1_out_to_execute_2);
  }
  in_hart = execute_1_in_from_issue.hart;
  if (execute_1_in_from_issue.is_valid){
    execute_1_status[in_hart].is_full = 1;
    execute_1_status_is_full[in_hart] = 1;
    get_input(in_hart,
              execute_1_in_from_issue,
              execute_1_status);
  }
}
