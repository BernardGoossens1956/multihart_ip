#include "ap_cint.h"
#include "multihart_ip.h"

bit_t compute_branch_result(
  int rv1,
  int rv2,
  func3_t func3){
#pragma HLS INLINE
  bit_t result;
  switch(func3){
    case BEQ : result = (rv1 == rv2);
               break;
    case BNE : result = (rv1 != rv2);
               break;
    case 2   :
    case 3   : result = 0;
               break;
    case BLT : result = (rv1 <  rv2);
               break;
    case BGE : result = (rv1 >= rv2);
               break;
    case BLTU: result =
               ((unsigned int)rv1 <  (unsigned int)rv2);
               break;
    case BGEU: result =
               ((unsigned int)rv1 >= (unsigned int)rv2);
               break;
  }
  return result;
}
int compute_op_result(
  int rv1,
  int right,
  decoded_instruction_t d_i){
#pragma HLS INLINE
  bit_t f7_6   = d_i.func7>>5;
  uint5 shift;
  int result, rv2;
  if (d_i.is_r_type){
    rv2   = right;
    shift = rv2;
  }
  else{//I_TYPE
    rv2   = (int)d_i.imm;
    shift = ((uint5)d_i.inst_24_21)<<1 |
             (uint5)d_i.inst_20;
  }
  switch(d_i.func3){
    case ADD : if (d_i.is_r_type && f7_6)
                 result = rv1 - rv2;//SUB
               else
                 result = rv1 + rv2;
               break;
    case SLL : result = rv1 << shift;
               break;
    case SLT : result = rv1 < rv2;
               break;
    case SLTU: result = (unsigned int)rv1 < (unsigned int)rv2;
               break;
    case XOR : result = rv1 ^ rv2;
               break;
    case SRL : if (f7_6) result = rv1 >> shift;//SRA
               else result = (unsigned int)rv1 >> shift;
               break;
    case OR  : result = rv1 | rv2;
               break;
    case AND : result = rv1 & rv2;
               break;
  }
  return result;
}
int compute_result(
  int rv1,
  code_address_t pc,
  decoded_instruction_t decoded_instruction){
#pragma HLS INLINE
  code_address_t pc4 = pc + 4;
  int result;
  switch(decoded_instruction.type){
    case R_TYPE:
      result = 0;//computed in compute_op_result
      break;
    case I_TYPE:
      if (decoded_instruction.is_jalr)
        result = (unsigned int)pc4;
      else if (decoded_instruction.is_load)
        result = rv1 + (int)decoded_instruction.imm;
      else //if (decoded_instruction.is_op_imm)
        result = 0;//computed in compute_op_result
      //else
        //result = 0;//(d_i.opcode == SYSTEM)
      break;
    case S_TYPE:
      result = rv1 + (int)decoded_instruction.imm;
      break;
    case B_TYPE:
      result = 0;//computed in compute_branch_result
      break;
    case U_TYPE:
      if (decoded_instruction.is_lui)
        result = (unsigned int)
                 ((u_uimmediate_t)
                  (decoded_instruction.imm<<12));
      else//d_i.opcode == AUIPC
        result = (int)pc +
                 (int)
                 ((u_uimmediate_t)
                  (decoded_instruction.imm<<12));
      break;
    case J_TYPE:
      result = (unsigned int)pc4;
      break;
    default:
      result = 0;
      break;
  }
  return result;
}
code_address_t compute_next_pc(
  code_address_t pc,
  decoded_instruction_t decoded_instruction,
  int rv1){
#pragma HLS INLINE
  code_address_t next_pc;
  code_address_t j_b_target_pc = (pc +
           (code_address_t)
           (((int)decoded_instruction.imm<<1)));
  code_address_t i_target_pc = ((rv1 +
             (int)decoded_instruction.imm)&0xfffffffe);
  if (decoded_instruction.is_jalr)
    next_pc = i_target_pc;
  else
    next_pc = j_b_target_pc;
  return next_pc;
}
