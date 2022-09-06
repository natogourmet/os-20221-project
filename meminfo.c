#include <stdio.h>
#include <unistd.h>
#include <sys/sysinfo.h>

struct sysinfostruct
{
    long uptime;                                  /* Seconds since boot */
    unsigned long loads[3];                       /* 1, 5, and 15 minute load averages */
    unsigned long totalram;                       /* Total usable main memory size */
    unsigned long freeram;                        /* Available memory size */
    unsigned long sharedram;                      /* Amount of shared memory */
    unsigned long bufferram;                      /* Memory used by buffers */
    unsigned long totalswap;                      /* Total swap space size */
    unsigned long freeswap;                       /* swap space still available */
    unsigned short procs;                         /* Number of current processes */
    unsigned long totalhigh;                      /* Total high memory size */
    unsigned long freehigh;                       /* Available high memory size */
    unsigned int mem_unit;                        /* Memory unit size in bytes */
    char _f[20 - 2 * sizeof(long) - sizeof(int)]; /* Padding for libc5 */
};

struct memstat
{
    unsigned long m_total;
    unsigned long m_free;
    unsigned long m_avlbl;
};

typedef struct MEMPACKED
{
    char name1[20];
    unsigned long MemTotal;
    char name2[20];
    unsigned long MemFree;
    char name3[20];
    unsigned long Buffers;
    char name4[20];
    unsigned long Cached;

} MEM_OCCUPY;

int get_meminfo(MEM_OCCUPY *lpMemory)
{
    FILE *fd;
    char buff[128];
    fd = fopen("/proc/meminfo", "r");
    if (fd < 0)
        return -1;
    fgets(buff, sizeof(buff), fd);
    sscanf(buff, "%s %lu ", lpMemory->name1, &lpMemory->MemTotal);
    fgets(buff, sizeof(buff), fd);
    sscanf(buff, "%s %lu ", lpMemory->name2, &lpMemory->MemFree);
    fgets(buff, sizeof(buff), fd);
    sscanf(buff, "%s %lu ", lpMemory->name3, &lpMemory->Buffers);
    fgets(buff, sizeof(buff), fd);
    sscanf(buff, "%s %lu ", lpMemory->name4, &lpMemory->Cached);

    fclose(fd);
}

void get_stats(struct memstat *st, int cpunum)
{
    FILE *fp = fopen("/proc/meminfo", "r");
    fscanf(fp, "%d %d %d", &(st->m_total), &(st->m_free), &(st->m_avlbl));
    fclose(fp);
    return;
}

int main(void)
{
    struct MEMPACKED newMem;

    // struct memstat st;
    while (1)
    {
        sleep(1);
        get_meminfo(&newMem);

        // get_stats(&st, -1);
        printf("MemTotal: %d MemFree: %d MemAvailable: %d\n", (newMem.MemTotal), (newMem.MemFree), (newMem.Buffers));
    }
    return 0;
}