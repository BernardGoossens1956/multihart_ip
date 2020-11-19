#ifndef __SYNTHESIS__
#include <stdio.h>
#include "multihart_ip.h"
#include "print.h"
#include "immediate.h"

static void emulate_op(
  hart_num_t            hart,
  int                   reg_file[][NB_REGISTER],
  decoded_instruction_t d_i){
  if (d_i.rd != 0){
    print_reg_name(d_i.rd);
#ifdef SYMB_REG
    if (d_i.rd!=26 && d_i.rd!=27)
      printf(" ");
#else
    if (d_i.rd < 10)
      printf(" ");
#endif
    printf(" = ");
    if ((d_i.is_load                                 &&
        (d_i.func3  == LBU    || d_i.func3 == LHU))  ||
        (d_i.is_op_imm        && d_i.func3 == SLTIU) ||
        (d_i.opcode == OP     && d_i.func3 == SLTU ))
      printf("%16d (%8x)", (unsigned int)reg_file[hart][d_i.rd],
                           (unsigned int)reg_file[hart][d_i.rd]);
    else
      printf("%16d (%8x)", reg_file[hart][d_i.rd],
                           reg_file[hart][d_i.rd]);
  }
}
static void emulate_load(
  hart_num_t            hart,
  int                   reg_file[][NB_REGISTER],
  decoded_instruction_t d_i){
  unsigned int address = (int)(reg_file[hart][d_i.rs1]) +
                         (int)i_immediate(&d_i)         +
                              hart*DATA_MEM_SIZE/(4*NB_HART);
  emulate_op(hart, reg_file, d_i);
  printf("    (m[%8x])", address);
}
static void emulate_store(
  hart_num_t            hart,
  int                   reg_file[][NB_REGISTER],
  decoded_instruction_t d_i){
  unsigned int address = (int)(reg_file[hart][d_i.rs1]) +
                         (int)s_immediate(&d_i)         +
                              hart*DATA_MEM_SIZE/(4*NB_HART);
  printf("m[%8x]", address);
  printf(" = ");
  printf("%16d (%8x)", reg_file[hart][d_i.rs2],
                       reg_file[hart][d_i.rs2]);
}
void emulate(
  hart_num_t            hart,
  int                   reg_file[][NB_REGISTER],
  decoded_instruction_t d_i,
  code_address_t        next_pc){
  printf("      ");
  switch(d_i.type){
    case R_TYPE://OP
      emulate_op(hart, reg_file, d_i);
      break;
    case I_TYPE://JALR || OP_IMM || LOAD || SYSTEM
      if (d_i.is_jalr){
        emulate_op(hart, reg_file, d_i);
      }
      else if (d_i.is_op_imm)
        emulate_op(hart, reg_file, d_i);
      else if (d_i.is_load)
        emulate_load(hart, reg_file, d_i);
      break;
    case S_TYPE://STORE
      emulate_store(hart, reg_file, d_i);
      break;
    case B_TYPE://BRANCH
      break;
    case U_TYPE://LUI || AUIPC
      emulate_op(hart, reg_file, d_i);
      break;
    case J_TYPE://JAL
      emulate_op(hart, reg_file, d_i);
      break;
    default:
      break;
  }
  printf("\n");
}
#endif
