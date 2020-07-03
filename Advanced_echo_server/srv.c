/////////////////////////////////////////////////////////////////////////
// File Name       : srv.c                                             //
// Date            : 2020/05/24 ~ 2020/05/28                           // 
// OS              : Ubuntu 18.04.4 LTS                                // 
// Student Name    : Seung Hoon Jeong                                  //
// Student ID      : 2015707003                                        //
// ------------------------------------------------------------------- //
// Title : System Programming Assignment #3-2                          //
// Description : Advanced echo Server                                  //
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

void sh_chld(int);
void sh_alrm(int);

////////////////////////////////////////////////////////////////////////////////////////////////////
// Function : int main(int argc, char **argv)                                                     //
// ============================================================================================== //
// Input: x                                                                                       //
// Output: 1.print client's information                                                           //
//         2.check child process created and terminated                                           //
// ============================================================================================== //
// Purpose: print server information                                                              //
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    char buff[BUF_SIZE];
    int n;
    struct sockaddr_in serv_addr, client_addr;
    int server_fd, client_fd;
    pid_t pid;
    int len;
    int port;
    int client_idx = 0;

    /* check argument count exception handling */
    if(argc !=2) {
        printf("Usage : %s [PORT_NUMBER]\n",argv[0]); // fixed argument format
        exit(0);
    }

    /* open socket */
    server_fd = socket(PF_INET, SOCK_STREAM, 0);

    /* prevent bind error after server terminated */
    int option = 1;
    if(setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) { 
        perror("setsockopt");
        exit(0);
    }

    memset(&serv_addr, 0, sizeof(serv_addr)); // initialize server socket information struct to zero
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    /* bind socket */
    bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));

    /* listen socket */
    listen(server_fd, 5);

    /* Applying signal handler(sh_alrm) for SIGALRM */
    signal(SIGALRM,sh_alrm);
    /* Applying signal handler(sh_chld) for SIGCHLD */
    signal(SIGCHLD,sh_chld);
    
    
    while(1) {
        len = sizeof(client_addr);
        /* accept socket descriptor */
	    client_fd = accept(server_fd, (struct sockaddr*)&client_addr, &len);
        //printf("new client connected..\n");
        if(client_idx == 5) {
            close(client_fd);
            continue;
        }
        client_idx++;

        /* 필요한 소스 삽입(fork() 이용) */
        if((pid = fork()) < 0) {
            perror("fork");
            close(client_fd);
            continue;
        }

        /* Child Process */
        else if(pid == 0) {
            close(server_fd);
            sleep(0.5);
            printf("Child Process ID: %d\n", getpid()); // print child process ID
            memset(buff,0x00,BUF_SIZE);
            while((len = read(client_fd, buff, BUF_SIZE)) != 0) {
		        /* when server received text "QUIT" */
                if(strcmp(buff, "QUIT\n") == 0){
                    close(client_fd); // connection close with client
                    //signal(SIGALRM,sh_alrm);
                    sh_alrm(port); // call SIGALRM, terminate child process 
                }
                else {
                    write(client_fd, buff, len); // send text to client
                    bzero(buff,sizeof(buff)); // buffer initialization
                    //continue;
                }
            }   
            close(client_fd);
            exit(0);
            
        }

        /* Parent Process */
        else {
            close(client_fd);
            /* print client's information */
            printf("==========Client info==========\n");
            printf("client IP: %s\n", inet_ntoa(client_addr.sin_addr)); // display IP by using inet_ntoa
            printf("\n");
            printf("client port: %d\n",ntohs(client_addr.sin_port)); // display Port number by using ntohs()
            printf("===============================\n");
            
        } 
    }
    close(server_fd);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Function : void sh_chld(int signum)                                                            //
// ============================================================================================== //
// Input: sigaction                                                                               //
// Output: print status of child process                                                          //
// ============================================================================================== //
// Purpose: Check status of child process                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////
void sh_chld(int signum) {
    printf("Status of Child process was changed.\n");
    wait(NULL);
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Function : void sh_alrm(int signum)                                                            //
// ============================================================================================== //
// Input: sigaction                                                                               //
// Output: print child process will be terminated                                                 //
// ============================================================================================== //
// Purpose: ready for terminating child process                                                   //
////////////////////////////////////////////////////////////////////////////////////////////////////
void sh_alrm(int signum) {
    printf("Child Process(PID : %d) will be terminated.\n", getpid());
    exit(1);
}
