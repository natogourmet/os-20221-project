#include <stdio.h>
#include <unistd.h>

unsigned sleep(unsigned sec);
double totald = 1;

struct cpustat
{
  unsigned long t_user;
  unsigned long t_nice;
  unsigned long t_system;
  unsigned long t_idle;
  unsigned long t_iowait;
  unsigned long t_irq;
  unsigned long t_softirq;
};

void skip_lines(FILE *fp, int numlines)
{
  int cnt = 0;
  char ch;
  while ((cnt < numlines) && ((ch = getc(fp)) != EOF))
  {
    if (ch == '\n')
      cnt++;
  }
  return;
}

void get_cpustats(struct cpustat *st, int cpunum)
{
  FILE *fp = fopen("/proc/stat", "r");
  int lskip = cpunum + 1;
  skip_lines(fp, lskip);
  char cpun[255];
  fscanf(fp, "%s %lu %lu %lu %lu %lu %lu %lu", cpun, &(st->t_user), &(st->t_nice),
         &(st->t_system), &(st->t_idle), &(st->t_iowait), &(st->t_irq),
         &(st->t_softirq));
  fclose(fp);
  return;
}

void print_stats(struct cpustat *st, char *name)
{
  printw("%s: %lu %lu %lu %lu %lu %lu %lu\n", name, (st->t_user), (st->t_nice),
         (st->t_system), (st->t_idle), (st->t_iowait), (st->t_irq),
         (st->t_softirq));
}

double calculate_load(struct cpustat *prev, struct cpustat *cur)
{
  int idle_prev = (prev->t_idle) + (prev->t_iowait);
  int idle_cur = (cur->t_idle) + (cur->t_iowait);

  int nidle_prev = (prev->t_user) + (prev->t_nice) + (prev->t_system) + (prev->t_irq) + (prev->t_softirq);
  int nidle_cur = (cur->t_user) + (cur->t_nice) + (cur->t_system) + (cur->t_irq) + (cur->t_softirq);

  int total_prev = idle_prev + nidle_prev;
  int total_cur = idle_cur + nidle_cur;

  totald = (double)total_cur - (double)total_prev;
  double idled = (double)idle_cur - (double)idle_prev;

  double cpu_perc = (1000 * (totald - idled) / totald + 1) / 10;

  return cpu_perc;
}

float calculate_usage(unsigned long prev, unsigned long cur)
{
  return ((float)(cur - prev) / totald) * 100;
}

int getcpuinfo(void)
{
  struct cpustat st0_0, st0_1;
  get_cpustats(&st0_0, -1);
  sleep(1);
  get_cpustats(&st0_1, -1);
  // system("clear");

  printw("%%CPU usage: %.2lf%%\t", calculate_load(&st0_0, &st0_1));
  printw("user: %.2f\t", calculate_usage(st0_0.t_user, st0_1.t_user));
  printw("user(nice): %.2f\t", calculate_usage(st0_0.t_nice, st0_1.t_nice));
  printw("kernel: %.2f\t", calculate_usage(st0_0.t_system, st0_1.t_system));
  printw("idle: %.2f\t", calculate_usage(st0_0.t_idle, st0_1.t_idle));
  printw("iowait: %.2f\t", calculate_usage(st0_0.t_iowait, st0_1.t_iowait));
  printw("irq: %.2f\t", calculate_usage(st0_0.t_irq, st0_1.t_irq));
  printw("softirq: %.2f", calculate_usage(st0_0.t_softirq, st0_1.t_softirq));
  printw("\n");

  return 0;
}
