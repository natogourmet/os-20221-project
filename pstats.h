#include <stdio.h>


struct pstats
{
    unsigned long p_id;
    char* p_comm;
    char p_state;
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