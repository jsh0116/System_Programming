/////////////////////////////////////////////////////////////////////////
// File Name       : srv.c                                            //
// Date            : 2020/05/16 ~ 2020/05/22                           // 
// OS              : Ubuntu 18.04.4 LTS                                // 
// Student Name    : Seung Hoon Jeong                                  //
// Student ID      : 2015707003                                        //
// ------------------------------------------------------------------- //
// Title : System Programming Assignment #3-1                          //
// Description : FTP command "ls" Implementation using socket (Server) //
/////////////////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h> 
#include <string.h> // bzero(), ...
#include <unistd.h> // STDOUT_FILENO, ...
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <dirent.h>
#define MAX_BUFF 4096
#define SEND_BUFF 2048

///////////////////////////////////////////////////////////////////////
// Function : void *client_info(struct sockaddr_in *clientaddr)      //
// ================================================================= //
// Input: Client address's address                                   //
// Output: Print client's Information                                //
// Purpose: Check client's IP ADDRESS and PORT                       //
///////////////////////////////////////////////////////////////////////
void *client_info(struct sockaddr_in *clientaddr)
{
    printf("==========Client info==========\n");
    printf("client IP: %s\n", inet_ntoa(clientaddr->sin_addr)); // display IP by using inet_ntoa
    printf("\n");
    printf("client port: %d\n",ntohs(clientaddr->sin_port)); // display Port number by using ntohs()
    printf("===============================\n");
}

////////////////////////////////////////////////////////////////////
// Function : static int select_files(const struct dirent *entry) //
// ===============================================================//
// Input: file pointer                                            //
// Output: 0 : fail                                               //
//         1 : success                                            //
// Purpose: choose file list name                                 //
////////////////////////////////////////////////////////////////////
static int select_files(const struct dirent *entry)
{
    if (entry->d_name[0] == '.')
        return 0;
    else
        return 1;
}

////////////////////////////////////////////////////////////////////////////////////////
// Function : void *cmd_process(char m_buff[MAX_BUFF], char m_result_buff[SEND_BUFF]) //
// ================================================================================== //
// Input: Client's cmd_buff and Server's result_buff                                  //
// Output: cmd process result('ls', 'quit')                                           //
// Purpose: Shell command implementation at Server                                    //
////////////////////////////////////////////////////////////////////////////////////////
void *cmd_process(char m_buff[MAX_BUFF], char m_result_buff[SEND_BUFF])
{
    DIR *dp = NULL;
    struct dirent *dirp = NULL;
    struct dirent **list = NULL;
    struct stat buf;
    char *temp = "\n";
    int n;
    ///////////////////////////////////////////insert command to buffer////////////////////////////////////////////////////
    if(!strcmp(m_buff, "ls")) { // ls command in buffer
        printf("%s\n",m_buff); // display command
        if((dp=opendir(".")) == NULL) { // open current directory
            printf("Can't Open this directory\n");
            exit(1);
        }
        else {
            while((dirp = readdir(dp)) != NULL) { // read directory
                if(dirp->d_ino == 0) continue; // skip if it doesn't have a i-nooe information
                if(!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name,"..")) //except current and parent directory
                    continue; 
            }
            /* scandir, alphasort file_list and insert to result_buff  */
            if((n = scandir(".", &list, select_files, alphasort)) < 0) {
                perror("scandir");
                exit(1);
            }
            for(int index=0; index<n; index++) {
                strcat(m_result_buff,list[index]->d_name);
                strcat(m_result_buff,temp);
                free(list[index]);
            }
            free(list);
            closedir(dp);
        }
    }

    else if(!strcmp(m_buff, "quit")) { // quit command in buffer
        strcpy(m_result_buff,m_buff); // buffer string copy to result buffer
        m_result_buff[strlen(m_result_buff)-1] == '\0'; // remove result buffer's newline
    }
    ///////////////////////////////////////////End of insert command////////////////////////////////////////////////////
}

////////////////////////////////////////////////////////////////////////////////////////////////////
// Function : int main(int argc, char **argv)                                                     //
// ============================================================================================== //
// Input: argument on kernel                                                                      //
// Output: 1.ls : ls command processing OK                                                        //
//         2.quit : quit command processing OK                                                    //
// Purpose: Receiving FTP command "ls" and "quit", Implementation and Sending result using socket //
////////////////////////////////////////////////////////////////////////////////////////////////////
int main(int argc, char **argv)
{
    char buff[MAX_BUFF], result_buff[SEND_BUFF];
    int n;
    /* open socket and listen */
    struct sockaddr_in servaddr, cliaddr;
    int listenfd, connfd, option = 1; 
    int clilen = sizeof(cliaddr);
    
    /* check argument count */
    if(argc !=2) {
        printf("Usage : %s [PORT_NUMBER]",argv[0]); // fixed argument format
        exit(0);
    }
    
    /* open socket */
    if((listenfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) { 
        perror("socket");
        exit(0);
    }
    
    /* prevent bind error after server terminated */
    if(setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option)) < 0) { 
        perror("setsockopt");
        exit(1);
    }

    memset((char *)&servaddr, '\0', sizeof(servaddr)); // initialize server socket information struct to zero 
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

        /* accept socket descriptor */
        if((connfd = accept(listenfd, (struct sockaddr *)&cliaddr, &clilen)) <0) {
            perror("accept");
            exit(0);
        }
        
        if(client_info(&cliaddr) < 0) /* display client ip and port */
            write(STDERR_FILENO, "client_info() err!!\n", sizeof("client_info() err!!\n"));
        
        while(1) {
            n = read(connfd, buff, MAX_BUFF); // read from client's cmd_buff
            buff[n] = '\0'; // null terminated
            bzero(result_buff,sizeof(result_buff)); // garbage terminated

            if(cmd_process(buff, result_buff) < 0) //command execute and result
            {
                write(STDERR_FILENO, "cmd_process() err!\n",sizeof("cmd_process() err!\n"));
                break;
            }

            write(connfd, result_buff, strlen(result_buff)); // put result_buffer in file descriptor
            result_buff[n] = '\0'; // null terminated
            
            if(!strcmp(result_buff, "quit")) // "quit" command on result_buff after cmd_process()
            {
                write(STDOUT_FILENO, "quit\n", sizeof("quit\n"));
                close(connfd); // connecting descriptor terminated
                break;
            }
        }

        /* for one more loop break */
        if(!strcmp(result_buff, "quit")) { 
            bzero(result_buff, sizeof(result_buff)); // initialize result_buff
            bzero(buff,sizeof(buff)); // initialize buff
            break;
        }
    }
    close(listenfd); // socket terminated
    return 0;
}
    

