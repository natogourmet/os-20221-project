// #include "cpuLoad.h"
#include "./pstats.h"

int main(int argc, char const *argv[])
{
  printf("Init\n");
  // checkCPULoad();
  generate_psdata();
  return 0;
}
