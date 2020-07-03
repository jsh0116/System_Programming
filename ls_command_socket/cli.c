/////////////////////////////////////////////////////////////////////////
// File Name       : cli.c                                             //
// Date            : 2020/05/16 ~ 2020/05/22                           // 
// OS              : Ubuntu 18.04.4 LTS                                // 
// Student Name    : Seung Hoon Jeong                                  //
// Student ID      : 2015707003                                        //
// ------------------------------------------------------------------- //
// Title : System Programming Assignment #3-1                          //
// Description : FTP command "ls" Implementation using socket (client) //
/////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <dirent.h>

#define MAX_BUFF 4096
#define RCV_BUFF 2048

///////////////////////////////////////////////////////////////////////
// Function : void process_result(char m_rcv_buff[RCV_BUFF])         //
// ================================================================= //
// Input: result_buff from server                                    //
// Output: Print result                                              //
// Purpose: Check final result                                       //
///////////////////////////////////////////////////////////////////////
void process_result(char m_rcv_buff[RCV_BUFF])
{
    printf("%s\n",m_rcv_buff);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Function : int main(int argc, char **argv)                                                     //
// ============================================================================================== //
// Input: argument on kernel, > ls, > quit                                                        //
// Output: 1.ls : File list result                                                                //
//         2.quit : Program quit                                                                  //
//                                                                                                //
// Purpose: Receiving FTP command "ls" and "quit", Implementation and Sending result using socket //
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    char buff[MAX_BUFF], cmd_buff[MAX_BUFF], rcv_buff[RCV_BUFF];
    int n;
    /////////////////////////////////////open socket and connect to server/////////////////////////////////////
    int sockfd;
    struct sockaddr_in servaddr;
    
    /* argument count exception handling */
    if(argc != 3) {
        printf("Usage :  %s [IP_ADDRESS] [PORT_NUMBER]\n", argv[0]);
        exit(0);
    }

    /* open socket */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket error");
        exit(1);
    }
    
    memset((char *)&servaddr, '\0', sizeof(servaddr)); // initialize server socket information struct to zero
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(atoi(argv[2])); // short data(port number) to network byte order
    inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

    /* connect socket */
    if(connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect");
        close(sockfd);
        exit(1);
    }
    
    for(;;) {
        printf("> ");
        if(fgets(cmd_buff,MAX_BUFF,stdin) == NULL) break; // input array string

        /* remove newline of cmd_buff */
        for(int i=0; cmd_buff[i] !=0; i++) {
            if(cmd_buff[i] == '\n') {
                cmd_buff[i] = 0;
                break;
            }
        }

        /* Command Exception Handling */
        if(!strcmp(cmd_buff, "ls") == 0 && !strcmp(cmd_buff, "quit") == 0) {
            printf("Please use only 'ls' or 'quit'\n");
            bzero(cmd_buff, sizeof(cmd_buff));
            continue;    
        }

        n = strlen(cmd_buff);
        /* send socket descriptor to server's buff */
        if(write(sockfd, cmd_buff, n) != n) {
            write(STDERR_FILENO, "write() error!!\n", sizeof("write() error!!\n"));
            exit(1);
        }
        
        /* receive from server's result_buff */
        if((n = read(sockfd, rcv_buff, RCV_BUFF-1)) < 0) {
            write(STDERR_FILENO, "read() error\n", sizeof("read() error\n"));
            exit(1);
        }
        
        rcv_buff[n] = '\0'; // null terminated
        
        if(!strcmp(rcv_buff, "quit")) {
            write(STDOUT_FILENO, "Program quit!!\n", sizeof("Program quit!!\n"));
            bzero(rcv_buff, sizeof(rcv_buff));
            exit(1);
        }
        process_result(rcv_buff); /*display ls (including options) command result */
        bzero(rcv_buff, sizeof(rcv_buff)); // initialize rcv_buff
        bzero(cmd_buff, sizeof(cmd_buff)); // initialize cmd_buff
    }
    close(sockfd);
    /////////////////////////////////////close socket and disconnect to server/////////////////////////////////////
    return 0;
}
