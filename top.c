#include <ncurses.h>
#include "./cpuinfo.h"
#include "./meminfo.c"
#include "./pstats.c"
int main(int argc, char const *argv[])
{
  initscr();

  while (1)
  {
    clear();
    getcpuinfo();
    getmeminfo();
    getpdata();
    refresh();
    // printw("\n███████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████\n");
  }
  endwin();

  return 0;

  // printf("Init\n");
  // // checkCPULoad();
  // return 0;
}
