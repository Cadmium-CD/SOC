#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include "do_block_hal.h"
#define MAP_SIZE 524288UL //consider block size 100 27 1000
#define MAP_MASK (MAP_MASK -1)

int do_block_hal (int lda,int ldb, int ldc, int M, int N, int K, int* A, int* B, int* C)
{
  volatile void *base;
  void *flag_address;  //store special address to control run/stop
  int finish_flag; //flag 1 means computation done

  //predefine address
  unsigned long addr0;
  addr0 = 0x40000000ul;
  //Memory map
  int fd = open("/dev/mem", O_RDWR|O_SYNC);
  if(!fd)
    {
      printf("Unable to open /dev/mem.  Ensure it exists (major=1, minor=1)\n");
      return -1;
    }
  Drv_initialize();

  //Drv_checkready(); //?????

  //Input data
  Drv_inputdata1(lda);
  Drv_inputdata1(ldb);
  Drv_inputdata1(ldc);
  Drv_inputdata1(M);
  Drv_inputdata1(N);
  Drv_inputdata1(K);
  Drv_inputdata2(*A, M, K, lda);
  Drv_inputdata2(*B, K, N, ldb);
  Drv_inputdata2(*C, M, N, ldc);

  //Run accelerator
  Drv_startfunction();

  //Wait until finish
  finish_flag = Drv_waitfunction();

  //outdata data and ummap
  if (finish_flag)
  {
    Drv_outputdata(*C, M, N, ldc);
    Drv_stopfunction();
  }

  return finish_flag;

}

void Drv_initialize(void)
{
  //Open memory as a file

    if((base == MAP_FAILED))
    {
      printf("mapping failed\n");
      fflush(stdout);
    }

  base = (unsigned int *)mmap(NULL, MAP_SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, addr0 & ~MAP_MASK);
  flag_address = base;
}

void Drv_inputdata1(int data1)
{
  (int*) base += 4;
  *（int*） base = data1;
}

void Drv_inputdata2(int *data2, int height, int width, int ld)
{
  int i,j;
  for (i = 0; i<height; ++i)
    for (j = 0; j<width; ++j)
    {
        (int*) base += 4;
        *（int*） base = data2[i*ld + j];
    }
}

void Drv_startfunction()
{
  *(int*) flag_address = 1;
}

int Drv_waitfunction()
{
  int result;
  do{
    result = *(int*) flag_address;
  } while (result) //continue waiting until result is 0
  return 1;
}

void Drv_outputdata(*data, int height, int width, int ld)
{
  int* temp_data[height*width];
  int i,j;
  memcpy(temp_data, (int*)(base-(height*width*4)+4),sizeof(temp_data)); //copy data back
  // cut data to subblock
  for (i = 0; i<height; ++i)
    for (j = 0; j<width; ++j)
    {
      data[i*ld + j]=temp_data[i*height+j];
    }
}

void Drv_stopfunction()
{
    munmap((void*)flag_address, MAP_SIZE);
    close(fd);
}
