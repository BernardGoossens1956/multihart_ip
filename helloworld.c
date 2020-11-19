#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"

unsigned int *pc       =
  (unsigned int *)(0x40000000
               +   0x10   );
unsigned int *end  =
  (unsigned int *)(0x40000000
               +   0x18);
char         *data_mem_0 =
  (        char *)(0x40000000
                 +   0x400  );
char         *data_mem_1 =
  (        char *)(0x40000000
                 +   0x800  );
char         *data_mem_2 =
  (        char *)(0x40000000
                 +   0xc00  );
char         *data_mem_3 =
  (        char *)(0x40000000
                 +   0x1000 );
unsigned int *code_mem =
  (unsigned int *)(0x40000000
               +   0x2000 );

unsigned int code_memory[128]=
{
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

int main()
{
  unsigned int i;
  char c0, c1, c2, c3;
  int w;
  *pc = 0;
  *end = 0;
  for (i=0; i<128; i++)
    code_mem[i] = code_memory[i];

  init_platform();

  while ((*end)!=1);

  print("register file dump\n\r");
  for (i=1; i<32; i++){
    c0 = data_mem_0[i];
    c1 = data_mem_1[i];
    c2 = data_mem_2[i];
    c3 = data_mem_3[i];
    w = (c3<<24) | (c2<<16) | (c1<<8) | c0;
    print("x[");
    xil_printf("%2d", i);
    print("] = ");
    xil_printf("%10d", w);
    print("\n\r");
  }
  print("data memory dump (non null words)\n\r");
  for (i=32; i<128; i++){
    c0 = data_mem_0[i];
    c1 = data_mem_1[i];
    c2 = data_mem_2[i];
    c3 = data_mem_3[i];
    w = (c3<<24) | (c2<<16) | (c1<<8) | c0;
    if (w != 0){
      print("m[");
      xil_printf("%2d", i);
      print("] = ");
      xil_printf("%10d", w);
      print("\n\r");
    }
  }

  cleanup_platform();
  return 0;
}
