#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

struct pstats
{
    unsigned long p_id;
    char* p_comm;
    char p_state;
    unsigned long p_size;
    unsigned long p_resident;
    unsigned long p_shared;
};

void get_stats(struct pstats *st, int pid)
{
    char* filename;
    sprintf(filename, "/proc/%d/stat", pid);

    FILE *fp = fopen(filename, "r");
    char cpun[255];
    fscanf(fp, "%d %s %c", cpun, &(st->p_id), &(st->p_comm), &(st->p_state));
    fclose(fp);
    return;
}

void get_mstats(struct pstats *st, int pid)
{
    char* filename;
    sprintf(filename, "/proc/%d/statm", pid);
    
    FILE *fp = fopen(filename, "r");
    char cpun[255];
    fscanf(fp, "%ul %ul %ul", cpun, &(st->p_size), &(st->p_resident), &(st->p_shared));
    fclose(fp);
    return;
}

int is_number(char* str)
{
    int i;
    for (i = 0; i < strlen(str); i++)
    {
        if (!isdigit(str[i]))
        {
            return 0;
        }
    }
}

void generate_psdata() {
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

            int pid = atoi(dir->d_name);
            get_pid_info(pid);
        }
        closedir(d);
    }
}

int get_pid_info(int pid)
{
    struct pstats st;
    get_stats(&st, pid);
    get_mstats(&st, pid);
    printf("%d\t%s\t%c\t%ul\t%ul\t%ul\n", st.p_id, st.p_comm, st.p_state, st.p_size, st.p_resident, st.p_shared);
    return 0;
}