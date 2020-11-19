#include "multihart_ip.h"

void mem_load(
  unsigned char *data_mem_0,
  unsigned char *data_mem_1,
  unsigned char *data_mem_2,
  unsigned char *data_mem_3,
  data_address_t a2,
  unsigned char *b0,
  unsigned char *b1,
  unsigned char *b2,
  unsigned char *b3){
#pragma HLS INLINE
  *b0 = *(data_mem_0 + a2);
  *b1 = *(data_mem_1 + a2);
  *b2 = *(data_mem_2 + a2);
  *b3 = *(data_mem_3 + a2);
}
void mem_store(
  unsigned char *data_mem_0,
  unsigned char *data_mem_1,
  unsigned char *data_mem_2,
  unsigned char *data_mem_3,
  uint2          a01,
  bit_t          a1,
  data_address_t a2,
  unsigned char  rv2_0,
  unsigned char  rv2_1,
  unsigned char  rv2_2,
  unsigned char  rv2_3,
  uint2          msize){
#pragma HLS INLINE
  switch(msize){
    case SB:
      switch(a01){
        case 0b00:
          *(data_mem_0 + a2) = rv2_0;
          break;
        case 0b01:
          *(data_mem_1 + a2) = rv2_0;
          break;
        case 0b10:
          *(data_mem_2 + a2) = rv2_0;
          break;
        case 0b11:
          *(data_mem_3 + a2) = rv2_0;
          break;
      }
      break;
    case SH:
      if (a1 == 0){
        *(data_mem_0 + a2) = rv2_0;
        *(data_mem_1 + a2) = rv2_1;
      }
      else{
        *(data_mem_2 + a2) = rv2_0;
        *(data_mem_3 + a2) = rv2_1;
      }
      break;
    case SW:
      *(data_mem_0 + a2) = rv2_0;
      *(data_mem_1 + a2) = rv2_1;
      *(data_mem_2 + a2) = rv2_2;
      *(data_mem_3 + a2) = rv2_3;
      break;
    case 3:
      break;
  }
}
