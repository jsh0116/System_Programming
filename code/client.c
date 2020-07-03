#include "apue.h"
#include "unp.h"
#include <dirent.h>
#include <time.h>
int main(int argc, char *argv[])
{
    int sockfd, n;
    char recvline[MAXLINE + 1];
    struct sockaddr_in servaddr;
    
    if (argc != 2)
        err_quit("Usage a.out <IPADDRESS>");

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
        err_sys("socket error!");
    
    bzero(&servaddr,sizeof(servaddr));
    servaddr.sin_family = AF_INET;  
    servaddr.sind_addr.s_addr = htonl(INADDR_ANY); 
    servaddr.sin_port = htons(13);

}