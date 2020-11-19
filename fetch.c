#include "debug_multihart_ip.h"
#include "multihart_ip.h"
#ifndef __SYNTHESIS__
#ifdef DEBUG_PIPELINE
#include <stdio.h>
#endif
#endif

void init_fetch_status(fetch_status_t *fetch_status,
                       code_address_t  pc){
  hart_num_p1_t h1;
  hart_num_t    h;
  for (h1=0; h1<NB_RUNNING_HART; h1++){
    h = h1;
    fetch_status[h].is_full = 1;
    fetch_status[h].pc      = pc;
  }
  for (h1=NB_RUNNING_HART; h1<NB_HART; h1++){
    h = h1;
    fetch_status[h].is_full = 0;
  }
}
static void get_input_from_decode(
  fetch_in_from_decode_t fetch_in_from_decode,
  fetch_status_t        *fetch_status){
#pragma HLS INLINE
  fetch_status[fetch_in_from_decode.hart].pc =
    fetch_in_from_decode.target_pc;
}
static void get_input_from_execute_1(
  fetch_in_from_execute_1_t fetch_in_from_execute_1,
  fetch_status_t           *fetch_status){
#pragma HLS INLINE
  fetch_status[fetch_in_from_execute_1.hart].pc =
    fetch_in_from_execute_1.target_pc;
}
static void stage_job(
  hart_num_t      hart,
  fetch_status_t *fetch_status,
  unsigned int   *code_mem,
  instruction_t  *instruction){
#pragma HLS INLINE
  *instruction =
    code_mem[fetch_status[hart].pc >> 2];
}
static void set_output(
  hart_num_t             hart,
  instruction_t          instruction,
  fetch_status_t        *fetch_status,
  fetch_out_to_decode_t *fetch_out_to_decode){
#pragma HLS INLINE
  fetch_out_to_decode->hart        = hart;
  fetch_out_to_decode->pc          =
    fetch_status[hart].pc;
  fetch_out_to_decode->instruction =
    instruction;
}
void get_hart(
  fetch_status_t *fetch_status,
  bit_t          *decode_status_is_full,
  bit_t          *is_fetching,
  hart_num_t     *fetching_hart){
#pragma HLS INLINE
  hart_num_t h01, h23;
  bit_t      c01, c23;
  bit_t      c[NB_HART];
  c[0] = (fetch_status[0].is_full &&
         !decode_status_is_full[0]);
  c[1] = (fetch_status[1].is_full &&
         !decode_status_is_full[1]);
  c[2] = (fetch_status[2].is_full &&
         !decode_status_is_full[2]);
  c[3] = (fetch_status[3].is_full &&
         !decode_status_is_full[3]);
  h01 = (c[0])?0:1;
  c01 = (c[0] || c[1]);
  h23 = (c[2])?2:3;
  c23 = (c[2] || c[3]);
  *fetching_hart = (c01)?h01:h23;
  *is_fetching   = (c01 || c23);
}
void fetch(
  fetch_in_from_decode_t    fetch_in_from_decode,
  fetch_in_from_execute_1_t fetch_in_from_execute_1,
  bit_t                    *decode_status_is_full,
  unsigned int             *code_mem,
  fetch_status_t           *fetch_status,
  fetch_out_to_decode_t    *fetch_out_to_decode){
#pragma HLS INLINE off
  instruction_t instruction;
  hart_num_t    fetching_hart;
  bit_t         is_fetching;
  get_hart(fetch_status,
           decode_status_is_full,
          &is_fetching,
          &fetching_hart);
  fetch_out_to_decode->is_valid = is_fetching;
  if (is_fetching){
    fetch_status[fetching_hart].is_full = 0;
    stage_job(fetching_hart,
              fetch_status,
              code_mem,
             &instruction);
#ifndef __SYNTHESIS__
#ifdef DEBUG_PIPELINE
    printf("hart %d: fetched  ", fetching_hart);
    printf("%04d: %08x      \n",
      fetch_status[fetching_hart].pc,
      instruction);
#endif
#endif
    set_output(fetching_hart,
               instruction,
               fetch_status,
               fetch_out_to_decode);
  }
  if (fetch_in_from_decode.is_valid){
    fetch_status[fetch_in_from_decode.hart].is_full = 1;
    get_input_from_decode(fetch_in_from_decode,
                          fetch_status);
  }
  if (fetch_in_from_execute_1.is_valid){
    fetch_status[fetch_in_from_execute_1.hart].is_full = 1;
    get_input_from_execute_1(fetch_in_from_execute_1,
                             fetch_status);
  }
}
