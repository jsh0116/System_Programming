#include "apue.h"
#include <dirent.h>
#include <string.h>

int my_double_ls(const char *name) 
{
    struct dirent *d;
    DIR *dp;

    if((dp = opendir(name)) == NULL)
        return -1;
    while(d = readdir(dp)) {
        if(d->d_ino != 0)
            printf("%s\n",d->d_name);
    }

    rewinddir(dp);
    while(d = readdir(dp)) {
        if(d->d_ino != 0)
            printf("%s\n",d->d_name);
    }
    
    closedir(dp);
    return 0;
}


int main(int argc, char *argv[])
{
    char pathname[128];
    if(argc == 1)
        strcpy(pathname, ".");
    else if(argc > 2) {
        printf("Too many parameter..\n");
        exit(1);
    }
    else {
        strcpy(pathname,argv[1]);
    }
    if (my_double_ls(pathname) == -1) printf("Could not open the directory\n");
}
