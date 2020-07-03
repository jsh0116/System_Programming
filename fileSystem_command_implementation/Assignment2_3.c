/////////////////////////////////////////////////////////////////////////
// File Name       : Assignment2_3.c                                   //                           
// Date            : 2020/04/26                                        //                           
// OS              : Ubuntu 18.04.4 LTS                                //         
// Student Name    : Seung Hoon Jeong                                  //                 
// Student ID      : 2015707003                                        //                     
// ------------------------------------------------------------------- //
// Title : System Programming Assignment #2-3                          //
// Description : file System command implementation                    //             
/////////////////////////////////////////////////////////////////////////

#include <unistd.h>
/* Add header file if you need */
#include <sys/stat.h> // for mode authority
#include <sys/types.h>
#include <stdio.h>
#include <string.h> // for strcmp function
#include <stdlib.h>
#include <dirent.h> // for opendir(), readdir(), closedir()
#include <errno.h> // for exception handling
#include <getopt.h>
#include <memory.h>

int main(int argc, char **argv)
{
    char getStr[128];
    while(1)
    {
        /* Write your codes */
        int c, i;
        int status;
        mode_t mode; // folder authority
        char u,g,o,a; // for arithmetic shift

        int new_argc = 2;
        char **new_argv;
        
        DIR *d = NULL;
        struct dirent *file = NULL;
        struct stat buf;
        char *src, *dest;
        
        printf(">> ");
/////////////// argument input //////////////////////////////////////////////
        if(fgets(getStr,128,stdin) == NULL) break; //getStr array에 입력받기
        if(getStr[strlen(getStr)-1] == '\n')
            getStr[strlen(getStr)-1] = '\0'; // 개행문자제거
        
        char *cmd;
        cmd = strtok(getStr, " "); // array parsing
        
        new_argv = (char **) malloc(sizeof(char*) *10);
        for (int j = 0; j < 10; j++)
            *(new_argv + j) = (char*) malloc(sizeof(char) * 10); // core dumped 방지
        
        if (!strcmp(getStr, "pwd")) {
            cmd = strtok(NULL, "\n"); // for argument count check
            if(getcwd(getStr, 128) == NULL)
                perror("getcwd error");
            else {
                if (cmd != NULL) {
                    printf("argument count error\n");
                    continue;
            }
                printf("%s\n",getcwd(getStr,128)); // current directory print
            } 
                
            
            
        }

        else if (!strcmp(getStr, "cd")) {
            cmd = strtok(NULL, "\n");
            if(chdir(cmd) != 0) // directory change
                perror("chdir error");
            
        }
        
        else if (!strcmp(getStr, "mkdir")) {
            optind = 1;
            
            while(cmd != NULL){
                new_argv[0] = NULL;
                cmd = strtok(NULL, " ");
                i = optind;
                while(i < new_argc) {
                    new_argv[i] = cmd;
                    cmd = strtok(NULL, " ");
                    if(cmd == NULL) break; // mkdir 이후 입력되는 인자 new_argv에 저장
                    i++;
                    new_argc++; // counting하면서 배열 크기 결정
                }
            }
            
            while((c = getopt(new_argc, new_argv, "m:")) != -1) {
                switch (c)
                {
                case 'm':
                    a = new_argv[optind-1][0]-'0';
                    u = new_argv[optind-1][1]-'0';
                    g = new_argv[optind-1][2]-'0';
                    o = new_argv[optind-1][3]-'0'; // shift 연산(8진수)
                    mode = o;
                    mode |= g << 3;
                    mode |= u << 6;
                    mode |= a << 9; // 연산 후 mode에 저장
                    
                    umask(0000);
                    if(*new_argv[optind] == '\0' )  { //파일 권한 내용 이후 인자 정보가 없을 때
                        for(i = optind-3; i > 0; --i) {
                            if(stat(new_argv[i],&buf) == 0) perror("stat error");
                            status = mkdir(new_argv[i],mode); // 8진수로 shift 시켜 저장한 권한 참조하여 입력 디렉토리 생성
                            if(status != 0)
                                perror("mkdir error");
                    }

                }
                    else {
                        for(i = optind; i < new_argc; ++i) { // 권한 내용 이후 입력 디렉토리 제목 인자 입력될 때
                            status = mkdir(new_argv[i],mode);
                            if(status != 0)
                                perror("mkdir error");
                    }
                }
                    
                    
                    break;
                }
            }
        }

        else if (!strcmp(getStr, "rmdir")) {
            cmd = strtok(NULL, "\n");
            if(stat(cmd,&buf) != 0) perror("stat error"); // directory stat check
            if(S_ISDIR(buf.st_mode)){ // check is this directory?
                if((d = opendir(cmd)) == NULL) // Please exit when User can't open directory 
                    perror("Failed to open this directory");
                if(chdir(cmd) != 0)
                    perror("chdir error");
                while((file = readdir(d)) != NULL) {
                    if (strcmp(file->d_name, ".") == 0 || strcmp(file->d_name, "..")) // pass current directory and parent directory 
                        continue;
                }
                closedir(d);
                chdir(".."); // parent directory 이동
                if(rmdir(cmd) != 0) // 이동 후 삭제
                    perror("rmdir error");
            }
            else // check when is this regular file?
                unlink(cmd); // remove using unlink
        }

        else if (!strcmp(getStr, "rename")) {
            cmd = strtok(NULL, "\n"); // 'rename' parsing
            src = strtok(cmd, " "); // memorized old cmd name
            dest = strtok(NULL, "\n"); // memorized new cmd name
            if(cmd == NULL || src == NULL | dest == NULL) {
                printf("argument count error");
                continue;
            }
            if(rename(src,dest) != 0)
                perror("rename error");
        }
        else if (!strcmp(getStr, "exit")) {
            cmd = strtok(NULL, "\n");
            if (cmd != NULL) {
                printf("argument count error\n"); // exit 이 외로 추가가 입력됬을 때
                continue;
            }
                
            else break;
        }
            
        else {
            printf("Please Use 5 command {'pwd','cd','mkdir','rmdir','rename', 'exit'}\n");
            continue;
        }
/////////////////////////////////////////Analysis End/////////////////////////////////////////////////            
    }
        return 0;
}