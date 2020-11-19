#ifndef __MULTIHART_IP
#define __MULTIHART_IP

#include "ap_cint.h"
#include "debug_multihart_ip.h"

#define LOG_CODE_MEM_SIZE 13
//size in bytes
#define CODE_MEM_SIZE     (1<<LOG_CODE_MEM_SIZE)
#define LOG_DATA_MEM_SIZE 12
//size in bytes
#define DATA_MEM_SIZE     (1<<LOG_DATA_MEM_SIZE)
#define NB_REGISTER       32
#define NB_HART            4
#define NB_RUNNING_HART    4

#define RET            0x8067
#define NOP            0x13

#define RA             1

#define LOAD           0b00000
#define LOAD_FP        0b00001
#define CUSTOM_0       0b00010
#define MISC_MEM       0b00011
#define OP_IMM         0b00100
#define AUIPC          0b00101
#define OP_IMM_32      0b00110
#define RV48_0         0b00111
#define STORE          0b01000
#define STORE_FP       0b01001
#define CUSTOM_1       0b01010
#define AMO            0b01011
#define OP             0b01100
#define LUI            0b01101
#define OP_32          0b01110
#define RV_64          0b01111
#define MADD           0b10000
#define MSUB           0b10001
#define NMSUB          0b10010
#define NMADD          0b10011
#define OP_FP          0b10100
#define RESERVED_0     0b10101
#define CUSTOM_2_RV128 0b10110
#define RV48_1         0b10111
#define BRANCH         0b11000
#define JALR           0b11001
#define RESERVED_1     0b11010
#define JAL            0b11011
#define SYSTEM         0b11100
#define RESERVED_2     0b11101
#define CUSTOM_3_RV128 0b11110
#define RV80           0b11111

#define UNDEFINED_TYPE 0
#define R_TYPE         1
#define I_TYPE         2
#define S_TYPE         3
#define B_TYPE         4
#define U_TYPE         5
#define J_TYPE         6
#define OTHER_TYPE     7

#define BEQ            0
#define BNE            1
#define BLT            4
#define BGE            5
#define BLTU           6
#define BGEU           7

#define ADD            0
#define SUB            0
#define SLL            1
#define SLT            2
#define SLTU           3
#define XOR            4
#define SRL            5
#define SRA            5
#define OR             6
#define AND            7

#define ADDI           0
#define SLLI           1
#define SLTI           2
#define SLTIU          3
#define XORI           4
#define SRLI           5
#define SRAI           5
#define ORI            6
#define ANDI           7

#define ECALL          0
#define EBREAK         1

#define LB             0
#define LH             1
#define LW             2
#define LBU            4
#define LHU            5

#define SB             0
#define SH             1
#define SW             2

typedef unsigned int instruction_t;
typedef uint12 code_address_t;
typedef uint12 data_address_t;
typedef uint3  type_t;
typedef int20  immediate_t;
typedef int12  i_immediate_t;
typedef int12  s_immediate_t;
typedef int12  b_immediate_t;
typedef int20  u_immediate_t;
typedef int20  j_immediate_t;
typedef uint12 i_uimmediate_t;
typedef uint12 s_uimmediate_t;
typedef uint12 b_uimmediate_t;
typedef uint20 u_uimmediate_t;
typedef uint20 j_uimmediate_t;
typedef uint5  opcode_t;
typedef uint6  reg_num_p1_t;
typedef uint5  reg_num_t;
typedef uint3  func3_t;
typedef uint7  func7_t;
typedef uint1  bit_t;
typedef uint32 counter_t;
typedef uint3  hart_num_p1_t;
typedef uint2  hart_num_t;

typedef struct decoded_instruction_s{
  bit_t       inst_31;
  uint6       inst_30_25;
  uint4       inst_24_21;
  bit_t       inst_20;
  uint8       inst_19_12;
  uint4       inst_11_8;
  bit_t       inst_7;
  opcode_t    inst_6_2;
  opcode_t    opcode;
  uint2       inst_1_0;
  reg_num_t   rd;
  func3_t     func3;
  reg_num_t   rs1;
  reg_num_t   rs2;
  func7_t     func7;
  type_t      type;
  immediate_t imm;
  bit_t       is_rs1_reg;
  bit_t       is_rs2_reg;
  bit_t       is_r_type;
  bit_t       is_load;
  bit_t       is_store;
  bit_t       is_branch;
  bit_t       is_jal;
  bit_t       is_jalr;
  bit_t       is_ret;
  bit_t       is_lui;
  bit_t       is_op_imm;
  bit_t       is_system;
  bit_t       is_jump;
  bit_t       is_ctrl;
  bit_t       has_no_dest;
  uint2       opch;
  uint3       opcl;
} decoded_instruction_t;

typedef struct fetch_in_from_decode_s{
  bit_t          is_valid;
  hart_num_t     hart;
  code_address_t target_pc;
} fetch_in_from_decode_t;

typedef fetch_in_from_decode_t decode_out_to_fetch_t;

typedef struct fetch_in_from_execute_1_s{
  bit_t          is_valid;
  hart_num_t     hart;
  code_address_t target_pc;
} fetch_in_from_execute_1_t;

typedef fetch_in_from_execute_1_t execute_1_out_to_fetch_t;

typedef struct fetch_status_s{
  bit_t          is_full;
  code_address_t pc;
} fetch_status_t;

typedef struct decode_in_from_fetch_s{
  bit_t          is_valid;
  hart_num_t     hart;
  code_address_t pc;
  instruction_t  instruction;
} decode_in_from_fetch_t;

typedef decode_in_from_fetch_t fetch_out_to_decode_t;

typedef struct decode_status_s{
  bit_t          is_full;
  code_address_t pc;
  instruction_t  instruction;
} decode_status_t;

typedef struct issue_in_from_decode_s{
  bit_t          is_valid;
  hart_num_t     hart;
  code_address_t pc;
#ifndef __SYNTHESIS__
  instruction_t  instruction;
  code_address_t target_pc;
#endif
  decoded_instruction_t
                 decoded_instruction;
} issue_in_from_decode_t;

typedef issue_in_from_decode_t decode_out_to_issue_t;

typedef struct issue_status_s{
  bit_t          is_full;
  code_address_t pc;
#ifndef __SYNTHESIS__
  instruction_t  instruction;
  code_address_t target_pc;
#endif
  decoded_instruction_t
                 decoded_instruction;
} issue_status_t;

typedef struct execute_1_in_from_issue_s{
  bit_t          is_valid;
  hart_num_t     hart;
  int            rv1;
  int            rv2;
  code_address_t pc;
#ifndef __SYNTHESIS__
  instruction_t  instruction;
  code_address_t target_pc;
#endif
  decoded_instruction_t
                 decoded_instruction;
} execute_1_in_from_issue_t;

typedef execute_1_in_from_issue_t issue_out_to_execute_1_t;

typedef struct execute_1_status_s{
  bit_t          is_full;
  int            rv1;
  int            rv2;
  code_address_t pc;
  decoded_instruction_t
                 decoded_instruction;
#ifndef __SYNTHESIS__
  instruction_t  instruction;
  code_address_t target_pc;
#endif
} execute_1_status_t;

typedef struct execute_2_in_from_execute_1_s{
  bit_t          is_valid;
  hart_num_t     hart;
  reg_num_t      rd;
  bit_t          has_no_dest;
  bit_t          is_r_type;
  bit_t          is_op_imm;
  bit_t          is_ret;
  int            result1;
  int            result2;
#ifndef __SYNTHESIS__
  code_address_t pc;
  instruction_t  instruction;
  decoded_instruction_t
                 decoded_instruction;
  code_address_t target_pc;
#endif
} execute_2_in_from_execute_1_t;

typedef execute_2_in_from_execute_1_t execute_1_out_to_execute_2_t;

typedef struct execute_2_status_s{
  bit_t          is_full;
  reg_num_t      rd;
  bit_t          has_no_dest;
  bit_t          is_r_type;
  bit_t          is_op_imm;
  bit_t          is_ret;
  int            result1;
  int            result2;
#ifndef __SYNTHESIS__
  code_address_t pc;
  instruction_t  instruction;
  decoded_instruction_t
                 decoded_instruction;
  code_address_t target_pc;
#endif
} execute_2_status_t;

typedef struct mem_in_from_execute_1_s{
  bit_t          is_valid;
  hart_num_t     hart;
  uint2          a01;
  bit_t          a1;
  data_address_t a2;
  unsigned char  value_0;
  unsigned char  value_1;
  unsigned char  value_2;
  unsigned char  value_3;
  reg_num_t      rd;
  bit_t          has_no_dest;
  bit_t          is_load;
  bit_t          is_store;
  func3_t        func3;
#ifndef __SYNTHESIS__
  code_address_t pc;
  instruction_t  instruction;
  decoded_instruction_t
                 decoded_instruction;
#endif
} mem_in_from_execute_1_t;

typedef mem_in_from_execute_1_t execute_1_out_to_mem_t;

typedef struct mem_status_s{
  bit_t          is_full;
  uint2          a01;
  bit_t          a1;
  data_address_t a2;
  unsigned char  value_0;
  unsigned char  value_1;
  unsigned char  value_2;
  unsigned char  value_3;
  reg_num_t      rd;
  bit_t          has_no_dest;
  bit_t          is_load;
  bit_t          is_store;
  func3_t        func3;
#ifndef __SYNTHESIS__
  code_address_t pc;
  instruction_t  instruction;
  decoded_instruction_t
                 decoded_instruction;
#endif
} mem_status_t;

typedef struct wb_in_from_execute_2_s{
  bit_t          is_valid;
  hart_num_t     hart;
  int            result;
  reg_num_t      rd;
  bit_t          has_no_dest;
  bit_t          is_ret;
#ifndef __SYNTHESIS__
  code_address_t pc;
  instruction_t  instruction;
  decoded_instruction_t
                 decoded_instruction;
  code_address_t target_pc;
#endif
} wb_in_from_execute_2_t;

typedef wb_in_from_execute_2_t execute_2_out_to_wb_t;

typedef struct wb_in_from_mem_s{
  bit_t          is_valid;
  hart_num_t     hart;
  int            result;
  unsigned char  value_0;
  unsigned char  value_1;
  unsigned char  value_2;
  unsigned char  value_3;
  uint2          a01;
  bit_t          a1;
  func3_t        msize;
  reg_num_t      rd;
  bit_t          has_no_dest;
#ifndef __SYNTHESIS__
  code_address_t pc;
  instruction_t  instruction;
  decoded_instruction_t
                 decoded_instruction;
#endif
} wb_in_from_mem_t;

typedef wb_in_from_mem_t mem_out_to_wb_t;

typedef struct wb_status_s{
  bit_t          is_full;
  bit_t          has_exited;
  int            value;
  reg_num_t      rd;
  bit_t          has_no_dest;
  bit_t          is_ret;
#ifndef __SYNTHESIS__
  code_address_t pc;
  instruction_t  instruction;
  decoded_instruction_t
                 decoded_instruction;
  code_address_t target_pc;
#endif
} wb_status_t;

typedef struct lock_reg_in_from_issue_s{
  bit_t      is_valid;
  reg_num_t  destination;
  hart_num_t hart;
} lock_reg_in_from_issue_t;

typedef lock_reg_in_from_issue_t issue_out_to_lock_reg_t;

typedef struct unlock_reg_in_from_wb_s{
  bit_t      is_valid;
  reg_num_t  destination;
  hart_num_t hart;
} unlock_reg_in_from_wb_t;

typedef unlock_reg_in_from_wb_t wb_out_to_unlock_reg_t;

void multihart_ip(
  unsigned int   pc,
  unsigned int  *code_mem,
  unsigned char *data_mem_0,
  unsigned char *data_mem_1,
  unsigned char *data_mem_2,
  unsigned char *data_mem_3,
  unsigned int  *end_signal);

#endif
