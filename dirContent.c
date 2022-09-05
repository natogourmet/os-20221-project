
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>

int main(void)
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
                    printf("The string contains int\n");
                else
                    isNumber = 0;
            }
            if (isNumber == 0) continue;

            
            sprintf(filename, "/proc/%s/stat", dir->d_name);
            FILE *f = fopen(filename, "r");

            int unused;
            char comm[1000];
            char state;
            int ppid;
            fscanf(f, "%d %s %c %d", &unused, comm, &state, &ppid);
            printf("comm = %s\n", comm);
            printf("state = %c\n", state);
            printf("parent pid = %d\n", ppid);
            fclose(f);
            printf("%s\n", dir->d_name);
        }
        closedir(d);
    }
    return (0);
}
