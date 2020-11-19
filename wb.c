#include "debug_multihart_ip.h"
#include "multihart_ip.h"
#ifndef __SYNTHESIS__
#ifdef DEBUG_PIPELINE
#include "stdio.h"
#endif
#endif

void init_wb_status(wb_status_t *wb_status,
                    bit_t       *wb_status_is_full){
  hart_num_p1_t h1;
  hart_num_t    h;
  for (h1=0; h1<NB_RUNNING_HART; h1++){
    h = h1;
    wb_status[h].is_full    = 0;
    wb_status[h].has_exited = 0;
    wb_status_is_full[h]    = 0;
  }
  for (h1=NB_RUNNING_HART; h1<NB_HART; h1++){
    h = h1;
    wb_status[h].is_full    = 0;
    wb_status[h].has_exited = 1;
    wb_status_is_full[h]    = 0;
  }
}
static void build_word(
  unsigned char b0,
  unsigned char b1,
  unsigned char b2,
  unsigned char b3,
  uint2         a01,
  bit_t         a1,
  func3_t       msize,
  int          *result){
#pragma HLS INLINE
  char           b;
  unsigned char  ub;
  short          h, h0, h1;
  unsigned short uh, uh0, uh1;
  int            w, ib, ih;
  unsigned int   iub, iuh;
  h0  = ((unsigned short)b1 <<  8) | (unsigned short)b0;
  uh0 = h0;
  h1  = ((unsigned short)b3 <<  8) | (unsigned short)b2;
  uh1 = h1;
  switch(a01){
    case 0b00: b = b0; break;
    case 0b01: b = b1; break;
    case 0b10: b = b2; break;
    case 0b11: b = b3; break;
  }
  ub  = b;
  ib  = b;
  iub = ub;
  h   = (a1)?h1:h0;
  uh  = h;
  ih  = h;
  iuh = uh;
  w = (uh1 << 16) | uh0;
  switch(msize){
    case LB:
      *result = ib;  break;
    case LH:
      *result = ih;  break;
    case LW:
      *result = w;   break;
    case 3:
      *result = 0;   break;
    case LBU:
      *result = iub; break;
    case LHU:
      *result = iuh; break;
    case 6:
    case 7:
      *result = 0;   break;
  }
}
static void get_input_from_mem_and_execute_2(
  wb_in_from_mem_t       wb_in_from_mem,
  wb_in_from_execute_2_t wb_in_from_execute_2,
  wb_status_t           *wb_status,
  bit_t                 *wb_status_is_full){
#pragma HLS INLINE
  hart_num_t hm, he;
  hm = wb_in_from_mem.hart;
  he = wb_in_from_execute_2.hart;
  wb_status[hm].rd                  =
    wb_in_from_mem.rd;
  wb_status[hm].has_no_dest         =
    wb_in_from_mem.has_no_dest;
  wb_status[hm].is_ret              = 0;
#ifndef __SYNTHESIS__
  wb_status[hm].pc                  =
    wb_in_from_mem.pc;
  wb_status[hm].instruction         =
    wb_in_from_mem.instruction;
  wb_status[hm].decoded_instruction =
    wb_in_from_mem.decoded_instruction;
#endif
  wb_status[he].value               =
    wb_in_from_execute_2.result;
  wb_status[he].rd                  =
    wb_in_from_execute_2.rd;
  wb_status[he].has_no_dest         =
    wb_in_from_execute_2.has_no_dest;
  wb_status[he].is_ret              =
    wb_in_from_execute_2.is_ret;
#ifndef __SYNTHESIS__
  wb_status[he].pc                  =
    wb_in_from_execute_2.pc;
  wb_status[he].instruction         =
    wb_in_from_execute_2.instruction;
  wb_status[he].decoded_instruction =
    wb_in_from_execute_2.decoded_instruction;
  wb_status[he].target_pc           =
    wb_in_from_execute_2.target_pc;
#endif
  wb_status[he].is_full = 1;
  wb_status_is_full[he] = 1;
}
static void get_input_from_mem(
  wb_in_from_mem_t wb_in_from_mem,
  wb_status_t     *wb_status){
#pragma HLS INLINE
  hart_num_t hart;
  hart = wb_in_from_mem.hart;
  wb_status[hart].rd                  =
    wb_in_from_mem.rd;
  wb_status[hart].has_no_dest         =
    wb_in_from_mem.has_no_dest;
  wb_status[hart].is_ret              = 0;
#ifndef __SYNTHESIS__
  wb_status[hart].pc                  =
    wb_in_from_mem.pc;
  wb_status[hart].instruction         =
    wb_in_from_mem.instruction;
  wb_status[hart].decoded_instruction =
    wb_in_from_mem.decoded_instruction;
#endif
}
static void get_input_from_execute_2(
  wb_in_from_execute_2_t wb_in_from_execute_2,
  wb_status_t           *wb_status){
#pragma HLS INLINE
  hart_num_t hart;
  hart = wb_in_from_execute_2.hart;
  wb_status[hart].value               =
    wb_in_from_execute_2.result;
  wb_status[hart].rd                  =
    wb_in_from_execute_2.rd;
  wb_status[hart].has_no_dest         =
    wb_in_from_execute_2.has_no_dest;
  wb_status[hart].is_ret              =
    wb_in_from_execute_2.is_ret;
#ifndef __SYNTHESIS__
  wb_status[hart].pc                  =
    wb_in_from_execute_2.pc;
  wb_status[hart].instruction         =
    wb_in_from_execute_2.instruction;
  wb_status[hart].decoded_instruction =
    wb_in_from_execute_2.decoded_instruction;
  wb_status[hart].target_pc           =
    wb_in_from_execute_2.target_pc;
#endif
}
static void stage_job(
  hart_num_t   hart,
  wb_status_t *wb_status,
  int          reg_file[][NB_REGISTER]){
#pragma HLS INLINE
  if (!wb_status[hart].has_no_dest)
    reg_file[hart][wb_status[hart].rd] =
      wb_status[hart].value;
  if (wb_status[hart].is_ret && wb_status[hart].value == 0)
    wb_status[hart].has_exited = 1;
}
static void get_hart(
  wb_status_t *wb_status,
  bit_t       *is_writing,
  hart_num_t  *writing_hart){
#pragma HLS INLINE
  hart_num_t h01, h23;
  bit_t      c01, c23;
  bit_t      c[NB_HART];
  c[0] = (wb_status[0].is_full);
  c[1] = (wb_status[1].is_full);
  c[2] = (wb_status[2].is_full);
  c[3] = (wb_status[3].is_full);
  h01 = (c[0])?0:1;
  c01 = (c[0] || c[1]);
  h23 = (c[2])?2:3;
  c23 = (c[2] || c[3]);
  *writing_hart = (c01)?h01:h23;
  *is_writing   = (c01 || c23);
}
void write_back(
  wb_in_from_execute_2_t  wb_in_from_execute_2,
  wb_in_from_mem_t        wb_in_from_mem,
  int                     reg_file       [][NB_REGISTER],
  wb_status_t            *wb_status,
  bit_t                  *wb_status_is_full,
  wb_out_to_unlock_reg_t *wb_out_to_unlock_reg){
#pragma HLS INLINE off
  hart_num_t writing_hart;
  bit_t      is_writing;
  uint2      input_source;
  int result;
  get_hart(wb_status,
          &is_writing,
          &writing_hart);
  if (is_writing){
    wb_status[writing_hart].is_full = 0;
    wb_status_is_full[writing_hart] = 0;
    stage_job(writing_hart, wb_status, reg_file);
#ifndef __SYNTHESIS__
#ifdef DEBUG_PIPELINE
    printf("hart %d: wb       ", writing_hart);
    printf("%04d\n", wb_status[writing_hart].pc);
    if (wb_status[writing_hart].rd != 0 ||
        wb_status[writing_hart].decoded_instruction.is_store)
      emulate(writing_hart,
              reg_file,
              wb_status[writing_hart].decoded_instruction,
              wb_status[writing_hart].target_pc);
#else
#ifdef DEBUG_FETCH
    printf("hart %d: %04d: %08x      ",
                    writing_hart,
      (unsigned int)wb_status[writing_hart].pc,
      (unsigned int)wb_status[writing_hart].instruction);
#ifndef DEBUG_DISASSEMBLE
    printf("\n");
#endif
#endif
#ifdef DEBUG_DISASSEMBLE
    disassemble(wb_status[writing_hart].pc,
                wb_status[writing_hart].instruction,
                wb_status[writing_hart].decoded_instruction);
#endif
#ifdef DEBUG_EMULATE
    if (wb_status[writing_hart].decoded_instruction.is_jalr ||
        wb_status[writing_hart].decoded_instruction.is_jal){
      printf("hart %d: ", writing_hart);
      printf("      pc  = %16d (%8x)\n",
        wb_status[writing_hart].target_pc,
        wb_status[writing_hart].target_pc);
    }
    if (wb_status[writing_hart].rd != 0 ||
        wb_status[writing_hart].decoded_instruction.is_store){
      printf("hart %d: ", writing_hart);
      emulate(writing_hart,
              reg_file,
              wb_status[writing_hart].decoded_instruction,
              wb_status[writing_hart].target_pc);
    }
#endif
#endif
#endif
  }
  wb_out_to_unlock_reg->is_valid    =
    is_writing &&
    wb_status[writing_hart].rd != 0;
  wb_out_to_unlock_reg->destination =
    wb_status[writing_hart].rd;
  wb_out_to_unlock_reg->hart        = writing_hart;
  build_word(wb_in_from_mem.value_0,
             wb_in_from_mem.value_1,
             wb_in_from_mem.value_2,
             wb_in_from_mem.value_3,
             wb_in_from_mem.a01,
             wb_in_from_mem.a1,
             wb_in_from_mem.msize,
            &result);
  input_source = (wb_in_from_mem.is_valid << 1) |
                  wb_in_from_execute_2.is_valid;
  switch (input_source){
    case 3:
    //(wb_in_from_mem.is_valid &&
    // wb_in_from_execute_2.is_valid)
    wb_status[wb_in_from_mem.hart].is_full = 1;
    wb_status_is_full[wb_in_from_mem.hart] = 1;
    get_input_from_mem_and_execute_2(
      wb_in_from_mem,
      wb_in_from_execute_2,
      wb_status,
      wb_status_is_full);
    wb_status[wb_in_from_mem.hart].value = result;
    break;
    case 2:
    //(wb_in_from_mem.is_valid &&
    //!wb_in_from_execute_2.is_valid)
    wb_status[wb_in_from_mem.hart].is_full = 1;
    wb_status_is_full[wb_in_from_mem.hart] = 1;
    get_input_from_mem(
      wb_in_from_mem,
      wb_status);
    wb_status[wb_in_from_mem.hart].value = result;
    break;
    case 1:
    //(!wb_in_from_mem.is_valid &&
    //  wb_in_from_execute_2.is_valid)
    wb_status[wb_in_from_execute_2.hart].is_full = 1;
    wb_status_is_full[wb_in_from_execute_2.hart] = 1;
    get_input_from_execute_2(
      wb_in_from_execute_2,
      wb_status);
  }
}
