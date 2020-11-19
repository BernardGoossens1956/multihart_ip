#include "debug_multihart_ip.h"
#include "multihart_ip.h"
#include "mem.h"
#ifndef __SYNTHESIS__
#ifdef DEBUG_PIPELINE
#include "stdio.h"
#endif
#endif

void init_mem_status(mem_status_t *mem_status,
                     bit_t        *mem_status_is_full){
  hart_num_p1_t h1;
  hart_num_t    h;
  for (h1=0; h1<NB_HART; h1++){
    h = h1;
    mem_status[h].is_full = 0;
    mem_status_is_full[h] = 0;
  }
}
static void get_input(
  hart_num_t              hart,
  mem_in_from_execute_1_t mem_in_from_execute_1,
  mem_status_t           *mem_status){
#pragma HLS INLINE
  mem_status[hart].value_0             =
    mem_in_from_execute_1.value_0;
  mem_status[hart].value_1             =
    mem_in_from_execute_1.value_1;
  mem_status[hart].value_2             =
    mem_in_from_execute_1.value_2;
  mem_status[hart].value_3             =
    mem_in_from_execute_1.value_3;
  mem_status[hart].a01                 =
    mem_in_from_execute_1.a01;
  mem_status[hart].a1                  =
    mem_in_from_execute_1.a1;
  mem_status[hart].a2                  =
    mem_in_from_execute_1.a2;
  mem_status[hart].rd                  =
    mem_in_from_execute_1.rd;
  mem_status[hart].has_no_dest         =
    mem_in_from_execute_1.has_no_dest;
  mem_status[hart].is_load             =
    mem_in_from_execute_1.is_load;
  mem_status[hart].is_store            =
    mem_in_from_execute_1.is_store;
  mem_status[hart].func3               =
    mem_in_from_execute_1.func3;
#ifndef __SYNTHESIS__
  mem_status[hart].pc                  =
    mem_in_from_execute_1.pc;
  mem_status[hart].instruction         =
    mem_in_from_execute_1.instruction;
  mem_status[hart].decoded_instruction =
    mem_in_from_execute_1.decoded_instruction;
#endif
}
static void stage_job(
  hart_num_t     hart,
  uint2          a01,
  bit_t          a1,
  data_address_t a2,
  func3_t        msize,
  mem_status_t  *mem_status,
  unsigned char *data_mem_0,
  unsigned char *data_mem_1,
  unsigned char *data_mem_2,
  unsigned char *data_mem_3){
#pragma HLS INLINE off
  if (mem_status[hart].is_load)
    mem_load(
        data_mem_0,
        data_mem_1,
        data_mem_2,
        data_mem_3,
        a2,
       &(mem_status[hart].value_0),
       &(mem_status[hart].value_1),
       &(mem_status[hart].value_2),
       &(mem_status[hart].value_3));
  else
    mem_store(
      data_mem_0,
      data_mem_1,
      data_mem_2,
      data_mem_3,
      a01,
      a1,
      a2,
      mem_status[hart].value_0,
      mem_status[hart].value_1,
      mem_status[hart].value_2,
      mem_status[hart].value_3,
      (uint2)msize);
}
static void set_output(
  hart_num_t       hart,
  uint2            a01,
  bit_t            a1,
  func3_t          msize,
  mem_status_t    *mem_status,
  mem_out_to_wb_t *mem_out_to_wb){
#pragma HLS INLINE
  mem_out_to_wb->hart                = hart;
  mem_out_to_wb->value_0             =
    mem_status[hart].value_0;
  mem_out_to_wb->value_1             =
    mem_status[hart].value_1;
  mem_out_to_wb->value_2             =
    mem_status[hart].value_2;
  mem_out_to_wb->value_3             =
    mem_status[hart].value_3;
  mem_out_to_wb->a01                 =
    a01;
  mem_out_to_wb->a1                  =
    a1;
  mem_out_to_wb->msize               =
    msize;
  mem_out_to_wb->rd                  =
    mem_status[hart].rd;
  mem_out_to_wb->has_no_dest         =
    mem_status[hart].has_no_dest;
#ifndef __SYNTHESIS__
  mem_out_to_wb->pc                  =
    mem_status[hart].pc;
  mem_out_to_wb->instruction         =
    mem_status[hart].instruction;
  mem_out_to_wb->decoded_instruction =
    mem_status[hart].decoded_instruction;
#endif
}
static void get_hart(
  mem_status_t *mem_status,
  bit_t        *wb_status_is_full,
  bit_t        *is_accessing_mem,
  hart_num_t   *mem_accessing_hart){
#pragma HLS INLINE
  hart_num_t h01, h23;
  bit_t      c01, c23;
  bit_t      c[NB_HART];
  c[0] = (mem_status[0].is_full && !wb_status_is_full[0]);
  c[1] = (mem_status[1].is_full && !wb_status_is_full[1]);
  c[2] = (mem_status[2].is_full && !wb_status_is_full[2]);
  c[3] = (mem_status[3].is_full && !wb_status_is_full[3]);
  h01 = (c[0])?0:1;
  c01 = (c[0] || c[1]);
  h23 = (c[2])?2:3;
  c23 = (c[2] || c[3]);
  *mem_accessing_hart = (c01)?h01:h23;
  *is_accessing_mem   = (c01 || c23);
}
void mem_access(
  mem_in_from_execute_1_t mem_in_from_execute_1,
  bit_t                  *wb_status_is_full,
  unsigned char          *data_mem_0,
  unsigned char          *data_mem_1,
  unsigned char          *data_mem_2,
  unsigned char          *data_mem_3,
  mem_status_t           *mem_status,
  mem_out_to_wb_t        *mem_out_to_wb,
  bit_t                  *mem_status_is_full){
#pragma HLS INLINE off
  bit_t          a1;
  uint2          a01;
  data_address_t a2;
  func3_t        msize;
  hart_num_t     in_hart, mem_accessing_hart;
  bit_t          is_accessing_mem;
  get_hart(mem_status,
           wb_status_is_full,
          &is_accessing_mem,
          &mem_accessing_hart);
  in_hart    = mem_in_from_execute_1.hart;
  if (mem_in_from_execute_1.is_valid){
    mem_status[in_hart].is_full = 1;
    mem_status_is_full[in_hart] = 1;
    get_input(in_hart,
              mem_in_from_execute_1,
              mem_status);
  }
  mem_out_to_wb->is_valid =
#ifdef __SYNTHESIS__
   !mem_status[mem_accessing_hart].is_store &&
#endif
    is_accessing_mem;
  msize =  mem_status[mem_accessing_hart].func3;
  a01   =  mem_status[mem_accessing_hart].a01;
  a1    =  mem_status[mem_accessing_hart].a1;
  a2    =  mem_status[mem_accessing_hart].a2;
  msize =  mem_status[mem_accessing_hart].func3;
  if (is_accessing_mem){
    mem_status[mem_accessing_hart].is_full = 0;
    mem_status_is_full[mem_accessing_hart] = 0;
    stage_job(mem_accessing_hart,
              a01,
              a1,
              a2,
              msize,
              mem_status,
              data_mem_0,
              data_mem_1,
              data_mem_2,
              data_mem_3);
#ifndef __SYNTHESIS__
#ifdef DEBUG_PIPELINE
    printf("hart %d: mem      ", mem_accessing_hart);
    printf("%04d\n", mem_status[mem_accessing_hart].pc);
#endif
#ifdef DEBUG_DETAILED_PIPELINE
    printf("hart %d: %c, a2 %4x, a1 %d, a01 %d, msize %c,\
 v0 %d, v1 %d, v2 %d, v3 %d\n", mem_accessing_hart,
           (mem_status[mem_accessing_hart].is_load)?'L':'S',
            a2 << 2, a1, a01,
          ((msize&3)==0)?'B':((msize&3)==1)?'H':'W',
            mem_status[mem_accessing_hart].value_0,
            mem_status[mem_accessing_hart].value_1,
            mem_status[mem_accessing_hart].value_2,
            mem_status[mem_accessing_hart].value_3);
#endif
#endif
    set_output(mem_accessing_hart,
               a01,
               a1,
               msize,
               mem_status,
               mem_out_to_wb);
  }
}
