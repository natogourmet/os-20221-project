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
    // start_color();
    // init_pair(1,COLOR_BLACK, COLOR_WHITE);
    // init_pair(2, COLOR_WHITE, COLOR_BLACK);
    printw("\n███████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████████\n");
    printw("\nPID\tSTATE\tPAGES\tVIRT\tP.RES\tRES\tP.SHR\tSHR\tCOMMAND\n");
    // wbkgd(stdscr, COLOR_PAIR(1));
    getpdata();
    // wbkgd(stdscr, COLOR_PAIR(2));

    refresh();
  }
  endwin();

  return 0;
}
