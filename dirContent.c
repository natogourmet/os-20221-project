
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

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
                if (isdigit(pid[i])){

                }
                else
                    isNumber = 0;
            }
            if (isNumber == 0)
                continue;

            sprintf(filename, "/proc/%s/stat", dir->d_name);

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
