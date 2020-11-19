#ifndef __MEM
#define __MEM

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
  unsigned char *b3);
void mem_store(
  unsigned char *data_mem_0,
  unsigned char *data_mem_1,
  unsigned char *data_mem_2,
  unsigned char *data_mem_3,
  uint2 a01,
  bit_t a1,
  data_address_t a2,
  unsigned char rv2_0,
  unsigned char rv2_1,
  unsigned char rv2_2,
  unsigned char rv2_3,
  uint2 msize);

#endif
