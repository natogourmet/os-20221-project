
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

struct pstats
{
  int p_id;
  char p_comm[1000];
  char p_state;
  unsigned long p_size;
  unsigned long p_resident;
  unsigned long p_shared;
};

int main(void *args)
{
  DIR *d;
  struct dirent *dir;
  d = opendir("/proc/");
  if (d)
  {
    while ((dir = readdir(d)) != NULL)
    {
      char filename[1000];

      int isNumber = 1;
      char *pid = dir->d_name;
      for (int i = 0; i < strlen(pid); i++)
      {
        if (isdigit(pid[i]))
        {
        }
        else
          isNumber = 0;
      }
      if (isNumber == 0)
        continue;

      sprintf(filename, "/proc/%s/stat", dir->d_name);
      get_pid_info(dir->d_name);
      continue;

      char *buffer = 0;
      long length;
      FILE *f = fopen("/proc/stat", "rb");

      if (f)
      {
        fseek(f, 0, SEEK_END);
        length = ftell(f);
        fseek(f, 0, SEEK_SET);
        buffer = malloc(length);
        if (buffer)
        {
          fread(buffer, 1, length, f);
        }
        fclose(f);
      }

      if (buffer)
      {
        printf("Output es %s\n\n", buffer);
      }

      // FILE *f = fopen(filename, "r");

      // int unused;
      // char comm[1000];
      // char state;
      // int ppid;
      // fscanf(f, "%d %s %c %d", &unused, comm, &state, &ppid);
      // printf("comm = %s\n", comm);
      // printf("state = %c\n", state);
      // printf("parent pid = %d\n", ppid);
      // fclose(f);
      // printf("%s\n", dir->d_name);
    }
    closedir(d);
  }
  return (0);
}

void get_stats(struct pstats *st, char *pid)
{

  char filename[1000];
  sprintf(filename, "/proc/%s/stat", pid);

  int unused;
  char comm[1000];
  FILE *fp = fopen(filename, "r");
  fscanf(fp, "%d %s %c", &(st->p_id), (st->p_comm), &(st->p_state));
  // printf("%d\t%s\t\n", st->p_id, st->p_comm);

  return;
}

void get_mstats(struct pstats *st, int pid)
{
  char filename[1000];
  sprintf(filename, "/proc/%s/stat", pid);

  FILE *fp = fopen(filename, "r");
  fscanf(fp, "%ul %ul %ul", &(st->p_size), &(st->p_resident), &(st->p_shared));
  fclose(fp);
  return;
}

void format_stats(struct pstats *st) {
  st->p_resident = st->p_resident / 1000000;
  st->p_shared = st->p_shared / 1000000;
  st->p_size = st->p_size / 1000000;
}

void get_pid_info(char *pid)
{
  struct pstats st;
  get_stats(&st, pid);
  // get_mstats(&st, pid);
  // printf("%s\n", st.p_comm);
  // char data[2000];
  // sprintf(data, "%d\t%s\t%c\t%ul\t%ul\t%ul", st.p_id, st.p_comm, st.p_state, st.p_size, st.p_resident, st.p_shared);
  format_stats(&st);
  printf("%d\t%s\t%c\t%lu\t%lu\t%lu", st.p_id, st.p_comm, st.p_state, st.p_size, st.p_resident, st.p_shared);
  // printf("% s\n", data);
}