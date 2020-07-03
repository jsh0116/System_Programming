#include "apue.h"

int main(void) {
    struct stat statbuf;
    /* turn on group execute and turn-off group-write */
    if(stat("foo", &statbuf) < 0)
        err_sys("stat error for foo");
    if(chmod("foo", (statbuf.st_mode & ~S_IWGRP) | S_IXGRP) < 0)
        err_sys("chmod error for foo");
    
    /* set absolute mode to "rw-r--r--" */
    if (chmod("bar", S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP) < 0)
        err_sys("chmod error for bar");

    exit(0);
}   