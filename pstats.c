#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define TICKS_PER_SEC sysconf(_SC_CLK_TCK)
struct pstats
{
  int p_id;
  char p_comm[1000];
  char p_state;
  unsigned long p_size;
  unsigned long p_resident;
  unsigned long p_shared;
  unsigned long utime;
  unsigned long ktime;
  unsigned long long start_time;
};

float get_process_elapsed_time(float pstart_time)
{
  float uptime;
  FILE *fp = fopen("/proc/uptime", "r");
  fscanf(fp, "%f", &uptime);
  fclose(fp);

  return uptime - pstart_time;
}

float calculate_cpu_usage(struct pstats *st)
{
  float elapsed_time = get_process_elapsed_time(st->start_time);
  return ((st->utime + st->ktime) * 100 / elapsed_time);
}

void get_stats(struct pstats *st, char *pid)
{
  char filename[1000];
  sprintf(filename, "/proc/%s/stat", pid);
  FILE *fp = fopen(filename, "r");
  fscanf(
      fp,
      "%d %s %c %*d %*d %*d %*d %*d %*u %*lu %*lu %*lu %*lu %lu %lu %*ld %*ld %*ld %*ld %*ld %*ld %llu",
      &(st->p_id),
      (st->p_comm),
      &(st->p_state),
      &(st->utime),
      &(st->ktime),
      &(st->start_time));
  fclose(fp);
}

void get_mstats(struct pstats *st, char *pid)
{
  char filename[1000];
  sprintf(filename, "/proc/%s/statm", pid);
  FILE *fp = fopen(filename, "r");
  fscanf(fp, "%lu %lu %lu", &(st->p_size), &(st->p_resident), &(st->p_shared));
  fclose(fp);
}

int is_number(char *str)
{
  int i;
  for (i = 0; i < strlen(str); i++)
  {
    if (!isdigit(str[i]))
    {
      return 0;
    }
  }
  return 1;
}

// void format_stats(struct pstats *st) {
//   st.
// }

void format_pstats(struct pstats *st)
{
  st->p_size *= 4;
  // st->p_size /= 1024;
  st->p_resident *= 4;
  // st->p_resident /= 1024;
  st->p_shared *= 4;
  // st->p_shared /= 1024;
  st->utime /= TICKS_PER_SEC;
  st->ktime /= TICKS_PER_SEC;
  st->start_time /= TICKS_PER_SEC;
}

int get_pid_info(char *pid)
{
  struct pstats *st = (struct st *)malloc(sizeof(struct pstats));

  get_stats(st, pid);
  get_mstats(st, pid);
  format_pstats(st);

  float cpu_usage = calculate_cpu_usage(st);

  printw("%d\t%c\t%lu\t%lu\t%lu\t%f%%\t%s\n", st->p_id, st->p_state, st->p_size, st->p_resident, st->p_shared, cpu_usage, st->p_comm);

  free(st);
  return 0;
}

int getpdata()
{
  DIR *d;
  struct dirent *dir;
  d = opendir("/proc/");
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
      char filename[1000];

      if (!is_number(dir->d_name))
      {
        continue;
      }
      // if (atoi(dir->d_name) < 1500 || atoi(dir->d_name) > 2000)
      // {
      //   continue;
      // }
      sprintf(filename, "/proc/%s/stat", dir->d_name);
      get_pid_info(dir->d_name);
    }
    closedir(d);
  }
  return (0);
}

// int main(void)
// {
//   printf("Hello im trying to execute myself, pls C oniichan");
//   generate_psdata();
// }