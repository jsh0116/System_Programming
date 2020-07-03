/////////////////////////////////////////////////////////////////////////
// File Name       : cli.c                                             //
// Date            : 2020/05/24 ~ 2020/05/28                           // 
// OS              : Ubuntu 18.04.4 LTS                                // 
// Student Name    : Seung Hoon Jeong                                  //
// Student ID      : 2015707003                                        //
// ------------------------------------------------------------------- //
// Title : System Programming Assignment #3-1                          //
// Description : Advanced echo Client                                  //
/////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <signal.h>

#define BUF_SIZE 256

////////////////////////////////////////////////////////////////////////////////////////////////////
// Function : int main(int argc, char **argv)                                                     //
// ============================================================================================== //
// Input: argument on kernel,> [TEXT] or > QUIT                                                   //
// Output: 1.from server:[TEXT]                                                                   //
//         2.quit : Program quit                                                                  //
// ============================================================================================== //      
// Purpose: Send text and print Receiving data                                                    //
//          if you input 'QUIT', client connection close after child process end                  // 
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    char buff[BUF_SIZE];
    int n;
    int sockfd;
    struct sockaddr_in serv_addr;
    
    /* argument count exception handling */
    if(argc != 3) {
        printf("Usage :  %s [IP_ADDRESS] [PORT_NUMBER]\n", argv[0]);
        exit(0);
    }

    /* open socket */
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&serv_addr, 0, sizeof(serv_addr)); // initialize server socket information struct to zero
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2])); // short data(port number) to network byte order
    /* connect socket */
    connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    
    while(1) {
	///////////////////text input///////////////////
        write(STDOUT_FILENO,"> ", 2);
        read(STDIN_FILENO, buff, BUF_SIZE);
	/* remove newline of cmd_buff */
        if(buff[strlen(buff) - 1] == '\n')
            buff[strlen(buff) - 1] == '\0';
        n = strlen(buff);
        write(sockfd, buff, BUF_SIZE); // send text to server
        n = read(sockfd, buff, BUF_SIZE); // receive text
        //printf("%d\n",n);
	/* if you receive "QUIT", n = 0 */
        if(n == 0) {
            bzero(buff,sizeof(buff));
            sleep(1); // wait for child process terminated
            break;
        }
        else if(n < 0) {
            perror("read");
            exit(0);
        }
        printf("from server: %s", buff);
        bzero(buff,sizeof(buff)); // buffer initialization
        //////////////////text result////////////////////
        
    }
    close(sockfd);
    return 0;
}
