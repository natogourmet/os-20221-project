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
}

void get_mstats(struct pstats *st, int pid)
{
  char *filename;
  sprintf(filename, "/proc/%d/statm", pid);

  FILE *fp = fopen(filename, "r");
  char cpun[255];
  fscanf(fp, "%ul %ul %ul", cpun, &(st->p_size), &(st->p_resident), &(st->p_shared));
  fclose(fp);
  return;
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
      if (atoi(dir->d_name) < 2000 || atoi(dir->d_name) > 10000) {
        continue;
      }
      sprintf(filename, "/proc/%s/stat", dir->d_name);
      get_pid_info(dir->d_name);
    }
    closedir(d);
  }
  return (0);
}

int get_pid_info(char *pid)
{
  struct pstats *st = (struct st*)malloc(sizeof(struct pstats));

  get_stats(st, pid);
  printf("%d\t%s\t%c\t%ul\t%ul\t%ul\n", st->p_id, st->p_comm, st->p_state, st->p_size, st->p_resident, st->p_shared);
  // get_mstats(&st, pid);
  // free(st);
  // printf("%d\t%s\t%c\t%ul\t%ul\t%ul\n", st.p_id, st.p_comm, st.p_state, st.p_size, st.p_resident, st.p_shared);
  return 0;
}

// int main(void)
// {
//   printf("Hello im trying to execute myself, pls C oniichan");
//   generate_psdata();
// }