#ifndef __COMPUTE
#define __COMPUTE

#include "multihart_ip.h"

bit_t compute_branch_result(
  int rv1,
  int rv2,
  func3_t func3);
int compute_op_result(
  int rv1,
  int rv2,
  decoded_instruction_t d_i);
int compute_result(
  int rv1,
  code_address_t pc,
  decoded_instruction_t decoded_instruction);
code_address_t compute_next_pc(
  code_address_t pc,
  decoded_instruction_t decoded_instruction,
  int rv1);

#endif
