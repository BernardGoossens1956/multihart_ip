#ifndef __TEST_OP_DUMP
#define __TEST_OP_DUMP

#include "multihart_ip.h"

/*
       .globl  main
main:
       li      x3,0
       sw      ra,128(x3)
       li      a0,13
       li      a4,12
       li      a1,7
       li      t0,28
       li      t6,-10
       li      s2,2022
       add     a2,a1,zero
       and     a3,a2,a0
       or      a5,a3,a4
       xor     a6,a5,t0
       sub     a6,a6,a1
       sltu    a7,a6,a0
       sll     t1,a6,t0
       slt     t2,t1,t6
       sltu    t3,t1,s2
       srl     t4,t1,t0
       sra     t5,t1,t0
       jal     dump_reg
       lw      ra,128(x3)
       ret
dump_reg:
       sw      x0,   0(x3)
       sw      x1,   4(x3)
       sw      x2,   8(x3)
       sw      x3,  12(x3)
       sw      x4,  16(x3)
       sw      x5,  20(x3)
       sw      x6,  24(x3)
       sw      x7,  28(x3)
       sw      x8,  32(x3)
       sw      x9,  36(x3)
       sw      x10, 40(x3)
       sw      x11, 44(x3)
       sw      x12, 48(x3)
       sw      x13, 52(x3)
       sw      x14, 56(x3)
       sw      x15, 60(x3)
       sw      x16, 64(x3)
       sw      x17, 68(x3)
       sw      x18, 72(x3)
       sw      x19, 76(x3)
       sw      x20, 80(x3)
       sw      x21, 84(x3)
       sw      x22, 88(x3)
       sw      x23, 92(x3)
       sw      x24, 96(x3)
       sw      x25,100(x3)
       sw      x26,104(x3)
       sw      x27,108(x3)
       sw      x28,112(x3)
       sw      x29,116(x3)
       sw      x30,120(x3)
       sw      x31,124(x3)
       ret
*/

instruction_t code_mem[CODE_MEM_SIZE/sizeof(unsigned int)] = {
  0x00000193,
  0x0811a023,
  0x00d00513,
  0x00c00713,
  0x00700593,
  0x01c00293,
  0xff600f93,
  0x7e600913,
  0x00058633,
  0x00a676b3,
  0x00e6e7b3,
  0x0057c833,
  0x40b80833,
  0x00a838b3,
  0x00581333,
  0x01f323b3,
  0x01233e33,
  0x00535eb3,
  0x40535f33,
  0x00c000ef,
  0x0801a083,
  0x00008067,
  0x0001a023,
  0x0011a223,
  0x0021a423,
  0x0031a623,
  0x0041a823,
  0x0051aa23,
  0x0061ac23,
  0x0071ae23,
  0x0281a023,
  0x0291a223,
  0x02a1a423,
  0x02b1a623,
  0x02c1a823,
  0x02d1aa23,
  0x02e1ac23,
  0x02f1ae23,
  0x0501a023,
  0x0511a223,
  0x0521a423,
  0x0531a623,
  0x0541a823,
  0x0551aa23,
  0x0561ac23,
  0x0571ae23,
  0x0781a023,
  0x0791a223,
  0x07a1a423,
  0x07b1a623,
  0x07c1a823,
  0x07d1aa23,
  0x07e1ac23,
  0x07f1ae23,
  0x00008067
};
#endif
