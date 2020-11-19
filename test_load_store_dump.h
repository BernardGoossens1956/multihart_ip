#ifndef __TEST_LOAD_STORE_DUMP
#define __TEST_LOAD_STORE_DUMP

#include "multihart_ip.h"

/*
       .globl  main
main:
       li      x3,0
       sw      ra,128(x3)
       li      t0,1
       li      t1,2
       li      t2,-3
       li      t3,-4
       li      a0,132
       sw      t0,0(a0)
       addi    a0,a0,4
       sh      t1,0(a0)
       sh      t0,2(a0)
       addi    a0,a0,4
       sb      t3,0(a0)
       sb      t2,1(a0)
       sb      t1,2(a0)
       sb      t0,3(a0)
       lb      a1,0(a0)
       lb      a2,1(a0)
       lb      a3,2(a0)
       lb      a4,3(a0)
       lbu     a5,0(a0)
       lbu     a6,1(a0)
       lbu     a7,2(a0)
       addi    a0,a0,-4
       lh      s0,2(a0)
       lh      s1,0(a0)
       lhu     s2,4(a0)
       lhu     s3,6(a0)
       addi    a0,a0,-4
       lw      s4,8(a0)
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
  0x00100293,
  0x00200313,
  0xffd00393,
  0xffc00e13,
  0x08400513,
  0x00552023,
  0x00450513,
  0x00651023,
  0x00551123,
  0x00450513,
  0x01c50023,
  0x007500a3,
  0x00650123,
  0x005501a3,
  0x00050583,
  0x00150603,
  0x00250683,
  0x00350703,
  0x00054783,
  0x00154803,
  0x00254883,
  0xffc50513,
  0x00251403,
  0x00051483,
  0x00455903,
  0x00655983,
  0xffc50513,
  0x00852a03,
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

