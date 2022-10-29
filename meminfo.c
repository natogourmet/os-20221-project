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
  unsigned long m_buff;
  unsigned long m_cache;
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

int get_meminfo(struct memstat *mst)
{
  FILE *fd;
  char buff[128];
  fd = fopen("/proc/meminfo", "r");
  if (fd < 0)
    return -1;

  char helper[255];
  fgets(buff, sizeof(buff), fd);
  sscanf(buff, "%s %lu ", helper, &mst->m_total);
  fgets(buff, sizeof(buff), fd);
  sscanf(buff, "%s %lu ", helper, &mst->m_free);
  fgets(buff, sizeof(buff), fd);
  sscanf(buff, "%s %lu ", helper, &mst->m_avlbl);
  fgets(buff, sizeof(buff), fd);
  sscanf(buff, "%s %lu ", helper, &mst->m_buff);
  fgets(buff, sizeof(buff), fd);
  sscanf(buff, "%s %lu ", helper, &mst->m_cache);

  fclose(fd);
}

// void get_stats(struct memstat *st, int cpunum)
// {
//     FILE *fp = fopen("/proc/meminfo", "r");
//     fscanf(fp, "%d %d %d", &(st->m_total), &(st->m_free), &(st->m_avlbl));
//     fclose(fp);
//     return;
// }

void format_mstats(struct memstat *mst) {
  mst->m_total /= 1024;
  mst->m_free /= 1024;
  mst->m_avlbl /= 1024;
  mst->m_buff /= 1024;
  mst->m_cache /= 1024;
}

void getmeminfo(void)
{
  struct memstat mst;
  get_meminfo(&mst);
  format_mstats(&mst);
  unsigned long usedm = mst.m_avlbl - mst.m_free;
  printw("Mem(kB): %lu total,\t%lu free,\t%lu used,\t%lu buff/cache\n", (mst.m_total), (mst.m_free), usedm, (mst.m_buff + mst.m_cache));
}