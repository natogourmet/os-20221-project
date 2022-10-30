#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

struct pstats
{
  int p_id;
  char p_comm[1000];
  char p_state;
  unsigned long p_pages;
  unsigned long p_size;
  unsigned long p_resident;
  unsigned long p_shared;
};

void get_stats(struct pstats *st, char *pid)
{
  char filename[1000];
  sprintf(filename, "/proc/%s/stat", pid);
  FILE *fp = fopen(filename, "r");
  fscanf(fp, "%d %s %c", &(st->p_id), (st->p_comm), &(st->p_state));
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

void format_pstats(struct pstats *st)
{
  st->p_pages = st->p_size;
  st->p_size *= 4;
  // st->p_size /= 1024;
  st->p_resident *= 4;
  // st->p_resident /= 1024;
  st->p_shared *= 4;
  // st->p_shared /= 1024;
}

int get_pid_info(char *pid)
{
  struct pstats *st = (struct st *)malloc(sizeof(struct pstats));

  get_stats(st, pid);
  get_mstats(st, pid);
  format_pstats(st);
  printw(
      "%d\t%c\t%lu\t%lu\t%lu\t%lu\t%lu\t%lu\t%s\n",
      st->p_id,
      st->p_state,
      st->p_pages,
      st->p_size,
      st->p_resident / 4,
      st->p_resident,
      st->p_shared / 4,
      st->p_shared,
      st->p_comm);
  // free(st);
  // printf("%d\t%s\t%c\t%ul\t%ul\t%ul\n", st.p_id, st.p_comm, st.p_state, st.p_size, st.p_resident, st.p_shared);
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
      if (atoi(dir->d_name) < 1500 || atoi(dir->d_name) > 2000)
      {
        continue;
      }
      sprintf(filename, "/proc/%s/stat", dir->d_name);
      get_pid_info(dir->d_name);
    }
    closedir(d);
  }
  return (0);
}
