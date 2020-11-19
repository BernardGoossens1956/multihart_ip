#include "multihart_ip.h"

i_immediate_t i_immediate(
  decoded_instruction_t *decoded_instruction){
#pragma HLS INLINE
 return (((i_immediate_t)decoded_instruction->inst_31   <<11) |
         ((i_immediate_t)decoded_instruction->inst_30_25<< 5) |
         ((i_immediate_t)decoded_instruction->inst_24_21<< 1) |
         ((i_immediate_t)decoded_instruction->inst_20       ));
}
s_immediate_t s_immediate(
  decoded_instruction_t *decoded_instruction){
#pragma HLS INLINE
 return (((s_immediate_t)decoded_instruction->inst_31   <<11) |
         ((s_immediate_t)decoded_instruction->inst_30_25<< 5) |
         ((s_immediate_t)decoded_instruction->inst_11_8 << 1) |
         ((s_immediate_t)decoded_instruction->inst_7        ));
}
b_immediate_t b_immediate(
  decoded_instruction_t *decoded_instruction){
#pragma HLS INLINE
 return (((b_immediate_t)decoded_instruction->inst_31   <<11) |
         ((b_immediate_t)decoded_instruction->inst_7    <<10) |
         ((b_immediate_t)decoded_instruction->inst_30_25<< 4) |
         ((b_immediate_t)decoded_instruction->inst_11_8     ));
}
u_immediate_t u_immediate(
  decoded_instruction_t *decoded_instruction){
#pragma HLS INLINE
 return (((u_immediate_t)decoded_instruction->inst_31   <<19) |
         ((u_immediate_t)decoded_instruction->inst_30_25<<13) |
         ((u_immediate_t)decoded_instruction->inst_24_21<< 9) |
         ((u_immediate_t)decoded_instruction->inst_20   << 8) |
         ((u_immediate_t)decoded_instruction->inst_19_12    ));
}
j_immediate_t j_immediate(
  decoded_instruction_t *decoded_instruction){
#pragma HLS INLINE
 return (((j_immediate_t)decoded_instruction->inst_31   <<19) |
         ((j_immediate_t)decoded_instruction->inst_19_12<<11) |
         ((j_immediate_t)decoded_instruction->inst_20   <<10) |
         ((j_immediate_t)decoded_instruction->inst_30_25<< 4) |
         ((j_immediate_t)decoded_instruction->inst_24_21    ));
}
