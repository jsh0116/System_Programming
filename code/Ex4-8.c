#include "apue.h"
#include <fcntl.h>

int main(int argc, char *argv[]) {
    if(argc != 2)
        err_quit("usage: Ex4-8 <pathname>");
    if(access(argv[1],R_OK)<0)
        err_ret("access error for %s", argv[1]);
    else
        printf("Read access OK\n");
    exit(0);
}