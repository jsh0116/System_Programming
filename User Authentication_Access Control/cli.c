/////////////////////////////////////////////////////////////////////////
// File Name       : cli.c                                             //
// Date            : 2020/06/05 ~ 2020/06/11                           // 
// OS              : Ubuntu 18.04.4 LTS                                // 
// Student Name    : Seung Hoon Jeong                                  //
// Student ID      : 2015707003                                        //
// ------------------------------------------------------------------- //
// Title : System Programming Assignment #4-1                          //
// Description : User Authentication & Access Control                  //
/////////////////////////////////////////////////////////////////////////

/* 필요한 header file 선언 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <netinet/in.h>

#define MAX_BUF 20 
#define CONT_PORT 20001

////////////////////////////////////////////////////////////////////////////////////////////////////
// Function : void log_in(int sockfd)                                                             //
// ============================================================================================== //
// Input: ID,PASSWORD                                                                             //
// Output: 1. login success : login check                                                         //
//         2. login fail : if you fail 3 times, relogin please                                    //
// ============================================================================================== //
// Purpose: Log in to server after IP ACCESS OK                                                   //
////////////////////////////////////////////////////////////////////////////////////////////////////
void log_in(int sockfd){ 
    int n; 
    char user[MAX_BUF], *passwd, buf[MAX_BUF]; 
    /* 코드 작성 (hint:  Check if the ip is acceptable ) */
    int count = 0;
    read(sockfd,buf,sizeof(buf));

    /* receives “REJECTION” from server */
    if (!strcmp(buf,"REJECTION")) {
        printf("** Connection refused **\n");
        close(sockfd);
        exit(0);
    }
    /* receives “ACCEPTED” from server */
    else if (!strcmp(buf,"ACCEPTED")) {
        printf("** It is connected to Server **\n");
        bzero(buf,sizeof(buf));
    }

    for(;;) {
	/* 코드 작성 (hint: pass username to server) */
	/////////////// Log-in to server ///////////////////////// 
        write(STDOUT_FILENO, "Input ID : ",sizeof("Input ID : ")); // Enter ID
        read(STDIN_FILENO, user, MAX_BUF);
        passwd = getpass("Input Password : "); // Enter Password (Password security operated)
        write(sockfd,user,sizeof(user));
        write(sockfd,passwd,sizeof(passwd)); 
        
        n = read(sockfd, buf, MAX_BUF); 
        buf[n] = '\0';
        if(!strcmp(buf, "OK")) { 
            n = read(sockfd, buf, MAX_BUF); 
            buf[n] = '\0'; 
            if(!strcmp(buf, "OK")) { 
                /* 코드 작성 (hint: login success) */
                printf("** User ‘[user_name]’ logged in **\n");
                break;
            } 
            else if(!strcmp(buf, "FAIL")) { 
                /* 코드 작성 (hint: login fail) */
                count++; // add count if you login fail
                printf("** Log-in failed **\n");
                if(count == 3) break; // if you login fail 3 times client connection refused
                else {
                    bzero(buf,sizeof(buf));
                    continue;
                }
            } 
            else{ // buf is “DISCONNECTION” 
                /* 코드 작성 (hint: three times fail) */
                printf("** Connection refused **\n");
                close(sockfd);
                exit(0);
            }
        }
	/////////////// Log-in End to server /////////////////////////
    }
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Function : int main(int argc, char **argv)                                                     //
// ============================================================================================== //
// Input: ./cli [IP_ADDRESS] [PORT_NUMBER]                                                        //
// Output: 1.success : IP access success and login success                                        //
//         2.fail : IP access fail or login fail                                                  //
// ============================================================================================== //
// Purpose: IP ACCESS and login to server                                                         //
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char *argv[]) 
{ 
    int sockfd, n, p_pid; 
    struct sockaddr_in servaddr;

    /* 코드 작성 */
    /* argument count exception handling */
    if(argc != 3) {
        printf("Usage :  %s [IP_ADDRESS] [PORT_NUMBER]\n", argv[0]);
        exit(0);
    }

    /* open socket */
    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(0);
    }

    memset(&servaddr, '\0', sizeof(servaddr)); // initialize server socket information struct to zero
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr(argv[1]); // short data(port number) to network byte order
    servaddr.sin_port = htons(atoi(argv[2]));

    /* connect to server */
    if(connect(sockfd,(struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("connect");
        exit(1);
    }

    log_in(sockfd); // User authentication after IP access control 
    close(sockfd); 
    return 0; 
}
