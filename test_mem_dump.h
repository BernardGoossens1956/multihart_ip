#ifndef __TEST_MEM_DUMP
#define __TEST_MEM_DUMP

#include "multihart_ip.h"

/*
       .globl  main
main:
       li      x3,0
       sw      ra,128(x3)
       li      a0,0
       li      a1,0
       li      a2,132
       addi    a3,a2,40
.L1:
       addi    a1,a1,1
       sw      a1,0(a2)
       addi    a2,a2,4
       bne     a2,a3,.L1
       li      a1,0
       li      a2,132
.L2:
       lw      a4,0(a2)
       addi    a2,a2,4
       add     a0,a0,a4
       bne     a2,a3,.L2
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

instruction_t code_mem[CODE_MEM_SIZE/sizeof(int)] = {
  0x00000193,
  0x0811a023,
  0x00000513,
  0x00000593,
  0x08400613,
  0x02860693,
  0x00158593,
  0x00b62023,
  0x00460613,
  0xfed61ae3,
  0x00000593,
  0x08400613,
  0x00062703,
  0x00460613,
  0x00e50533,
  0xfed61ae3,
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
