/////////////////////////////////////////////////////////////////////////
// File Name       : srv.c                                             //
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
#include <pwd.h>
#include <arpa/inet.h>

#define MAX_BUF 20

////////////////////////////////////////////////////////////////////////////////////////////////////
// Function : int ip_match(char *first, char *second)                                             //
// ============================================================================================== //
// Input: x                                                                                       //
// Output: 1. int 1: matching success                                                             //
//         2. int 0: matching fail                                                                //
// ============================================================================================== //
// Purpose: IP ACCESS CONTROL, to match wild card characters                                      //
////////////////////////////////////////////////////////////////////////////////////////////////////
int ip_match(char *first, char *second)
{
	if (*first == '\0' && *second == '\0') return 1; // If we reach at the end of both strings, we are done
    /* Make sure that the characters after '*' are present in second string.
       Assume that the first string will not contain two consecutive '*' */
	if (*first == '*' && *(first+1) != '\0' && *second == '\0') return 0;
	if (*first == '?' || *first == *second) return ip_match(first+1, second+1); // If the first string contains '?', or current characters of both strings match
	/* If there is *, then there are two possibilities.
      1. We consider current character of second string.
      2. We ignore current character of second string. */
    if (*first == '*') return ip_match(first+1, second) || ip_match(first, second+1); 
	return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Function : int user_match(char *user, char *passwd)                                            //
// ============================================================================================== //
// Input: x                                                                                       //
// Output: 1. int 1: ID, PASSWORD CORRECT                                                         //
//         2. int 0: ID, PASSWORD ERROR                                                           //
// ============================================================================================== //
// Purpose: USER ID,PASSWORD matching                                                             //
////////////////////////////////////////////////////////////////////////////////////////////////////
int user_match(char *user, char *passwd) {
    FILE *fp;
    struct passwd *pw;
    fp = fopen("passwd", "r");

    /* 코드 작성 (hint: 인증 성공 시 return 1, 인증 실패 시 return 0  */
    memset((void *)&pw,0x00,sizeof(pw));
    while((pw = fgetpwent(fp)) != NULL) {
        if(!strcmp(user,pw->pw_name) && !strcmp(passwd,pw->pw_passwd)) // if user ID, PASSWORD equal, we are done
            return 1;
    }
    fclose(fp);
    return 0;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Function : int log_auth(int connfd)                                                            //
// ============================================================================================== //
// Input: x                                                                                       //
// Output: 1. int 1 : login success, authentication OK                                            //
//         2. int 0 login fail, check 3 times, if you fail 3 times, Disconnect                    //
// ============================================================================================== //
// Purpose: Log-in Authentication                                                                 //
////////////////////////////////////////////////////////////////////////////////////////////////////
int log_auth(int connfd)
{   
    char user[MAX_BUF], passwd[MAX_BUF];
    int n, count=1;
    while(1) {
        /* 코드 작성 (hint: username과 password를 client로부터 받는다) */
        /* Read User ID */
        if((n = read(connfd,user,MAX_BUF)) > 0) {
            if(user[strlen(user)-1] == '\n') user[strlen(user)-1] = '\0';
        }
        
        /* Read UserID's Password */
        if((n = read(connfd,passwd,MAX_BUF)) > 0) {
            if(passwd[strlen(passwd)-1] == '\n') passwd[strlen(passwd)-1] = '\0';
        }

        write(connfd, "OK", MAX_BUF); // send to "OK" message to client
        printf("** User is trying to log-in (%d/3) **\n",count);

        /* when ID,PASSWORD Authentication sucess */
        if((n = user_match(user, passwd)) == 1){
            /* 코드 작성 (hint: 인증 OK) */
            write(connfd, "OK", MAX_BUF);
            return 1;
        }

         /* when ID,PASSWORD Authentication fail */
        else if(n == 0){
            printf("** Log-in failed **\n");
            if(count >= 3) {
                /* 코드 작성 (hint: 3 times fail) */
                write(connfd,"DISCONNECTION", MAX_BUF); // send to "DISCONNECTION" message to client
                return 0;
            }
            write(connfd, "FAIL", MAX_BUF);
            count++; // add fail count
            continue;
        }
    }
    return 1;
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
    int listenfd, connfd;
    struct sockaddr_in servaddr, cliaddr;
    FILE *fp_checkIP; // FILE stream to check client’s IP

    /* 코드 작성 */
    int clilen;
    clilen = sizeof(cliaddr);
    /* argument count exception handling */
    if(argc !=2) {
        printf("Usage : %s [PORT_NUMBER]\n",argv[0]); // fixed argument format
        exit(0);
    }
    /* open socket */
    if((listenfd = socket(AF_INET,SOCK_STREAM, 0)) < 0) {
        perror("socket");
        exit(0);
    }

    int option = 1;
    /* prevent bind error after server terminated */
    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) { 
        perror("setsockopt");
        exit(0);
    }

    memset(&servaddr, '\0', sizeof(servaddr)); // initialize server socket information struct to zero 	 
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(atoi(argv[1]));
    
    /* bind socket */
    if(bind(listenfd, (struct sockaddr*)&servaddr, sizeof(servaddr)) < 0) {
        perror("bind");
        exit(0);
    }

    /* listen socket */
    if(listen(listenfd,5) < 0) {
        perror("listen");
        exit(0);
    }
    for(;;) {
        connfd = accept(listenfd, (struct sockaddr *) &cliaddr, &clilen) ;
        /* 코드 작성 (hint: Client의 IP가 접근 가능한지 확인) */
        printf("** Client is trying to connect **\n");
        printf(" - IP:   %s\n",inet_ntoa(cliaddr.sin_addr));
        printf(" - Port: %d\n",ntohs(cliaddr.sin_port));
        fp_checkIP = fopen("access.txt", "r"); // check what IP is access posible?
        char pattern[MAX_BUF];
        fgets(pattern,sizeof(pattern),fp_checkIP);
        if(pattern[strlen(pattern)-1] == '\n') pattern[strlen(pattern)-1] = '\0'; // remove newline
        
        char str_ip[INET_ADDRSTRLEN] = {0};
        inet_ntop(AF_INET,&(cliaddr.sin_addr),str_ip,INET_ADDRSTRLEN); // get it back after storing this IP address in sockaddr

        /* when ip pattern matched */
        if(ip_match(pattern,str_ip) == 1) {
            printf("** Client is connected **\n");
            write(connfd,"ACCEPTED",sizeof("ACCEPTED")); // send to "ACCEPTED" message to client
            bzero(pattern,sizeof(pattern));
        }

        /* when ip pattern not matched */
        else if(ip_match(pattern,str_ip) == 0) {
            printf("** It is NOT authenticated client **\n");
            write(connfd,"REJECTION",sizeof("REJECTION")); // send to "REJECTION" message to client
            bzero(pattern,sizeof(pattern));
            continue;
        }
        
        if(log_auth(connfd) == 0) { // if 3 times fail (ok : 1, fail : 0)
            printf("** Fail to log-in **\n");
            close(connfd);
            continue;
        }

        printf("** Success to log-in **\n");
        close(connfd);
        }
}