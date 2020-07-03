#include "apue.h"
#include <fcntl.h>
#include <utime.h>

int main(int argc, char *argv[])
{
    int i,fd;
    struct stat statbuf;
    struct utimbuf timebuf;

    for(i=1;i<argc;i++) {

        if(stat(argv[i],&statbuf) < 0) {
            err_sys("%s : stat error", argv[i]);
            continue;
        }

        if((fd = open(argv[i], O_RDWR | O_TRUNC)) < 0) {
            err_ret("%s: open error", argv[i]);
            continue;
        }

        close(fd);
        timebuf.actime = ;
    }
}