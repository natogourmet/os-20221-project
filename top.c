#include "./cpuinfo.h"
#include "./meminfo.c"
#include "./pstats.c"

int main(int argc, char const *argv[])
{
  while (1)
  {
    getcpuinfo();
    getmeminfo();
    printf("\n███████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████\n");
    getpdata();
  }
  return 0;

  // printf("Init\n");
  // // checkCPULoad();
  // return 0;
}
