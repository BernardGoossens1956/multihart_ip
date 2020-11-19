#include <stdio.h>
#include "multihart_ip.h"
#include "print.h"

//#include "test_branch_dump.h"
//#include "test_jal_jalr_dump.h"
//#include "test_load_store_dump.h"
#include "test_lui_auipc_dump.h"
//#include "test_mem_dump.h"
//#include "test_op_dump.h"
//#include "test_op_imm_dump.h"
//#include "test_sum_dump.h"

unsigned char data_mem_0[DATA_MEM_SIZE/4];
unsigned char data_mem_1[DATA_MEM_SIZE/4];
unsigned char data_mem_2[DATA_MEM_SIZE/4];
unsigned char data_mem_3[DATA_MEM_SIZE/4];

int main() {
  unsigned int  i, h;
  unsigned int  pc;
  unsigned int  end_signal;
  unsigned char b0, b1, b2, b3;
  unsigned int  w, nbc, nbi;
  pc = 0;
  end_signal = 0;
  multihart_ip(pc,
             code_mem,
             data_mem_0,
             data_mem_1,
             data_mem_2,
             data_mem_3,
            &end_signal);
  for (h=0; h<NB_HART; h++){
    printf("\nhart %d register file dump\n", h);
    for (i=1; i<NB_REGISTER; i++){
      b0 = data_mem_0[i+h*DATA_MEM_SIZE/(4*NB_HART)];
      b1 = data_mem_1[i+h*DATA_MEM_SIZE/(4*NB_HART)];
      b2 = data_mem_2[i+h*DATA_MEM_SIZE/(4*NB_HART)];
      b3 = data_mem_3[i+h*DATA_MEM_SIZE/(4*NB_HART)];
      w = ((unsigned int)b3<<24) |
          ((unsigned int)b2<<16) |
          ((unsigned int)b1<< 8) |
           (unsigned int)b0;
      print_reg_name(i);
      printf(" ");
#ifdef SYMB_REG
      if (i!=26 && i!=27) printf(" ");
#else
      if (i<10) printf(" ");
#endif
      printf("= %16d (%8x)\n", w, (unsigned int)w);
    }
    printf("\nhart %d data memory dump (non null words)\n", h);
    for (i=NB_REGISTER; i<DATA_MEM_SIZE/(4*NB_HART); i++){
      b0 = data_mem_0[i+h*DATA_MEM_SIZE/(4*NB_HART)];
      b1 = data_mem_1[i+h*DATA_MEM_SIZE/(4*NB_HART)];
      b2 = data_mem_2[i+h*DATA_MEM_SIZE/(4*NB_HART)];
      b3 = data_mem_3[i+h*DATA_MEM_SIZE/(4*NB_HART)];
      w = ((unsigned int)b3<<24) |
          ((unsigned int)b2<<16) |
          ((unsigned int)b1<< 8) | 
           (unsigned int)b0;
      if (w != 0)
        printf("m[%4x] = %16d (%8x)\n",
                i+h*DATA_MEM_SIZE/(4*NB_HART),
                w, (unsigned int)w);
    }
  }
  nbc = ((unsigned int)data_mem_3[64]<<24) +
        ((unsigned int)data_mem_2[64]<<16) +
        ((unsigned int)data_mem_1[64]<< 8) +
        ((unsigned int)data_mem_0[64]);
  nbi = ((unsigned int)data_mem_3[65]<<24) +
        ((unsigned int)data_mem_2[65]<<16) +
        ((unsigned int)data_mem_1[65]<< 8) +
        ((unsigned int)data_mem_0[65]);
  printf("\n%d instructions run\
 in %d cycles (ipc = %2.2f)\n", nbi, nbc, ((float)nbi)/nbc);

  return 0;
}
