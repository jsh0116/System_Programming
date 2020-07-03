#include "apue.h"
#include "unp.h"
int main(int argc, char *argv[])
{
    int listenfd, connfd;
    pid_t childpid;
    socklen_t clilen;
    struct sockaddr_in servaddr, cliaddr;
    
    listenfd = Socket(AF_INET, SOCK_STREAM , 0);


}