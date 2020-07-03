#include <stdio.h>
#include <unistd.h>
#define SIZE 200
void my_pwd(void);

int main()
{
    my_pwd();
}

void my_pwd(void) {
    char dirname[SIZE];
    if(getcwd(dirname, SIZE) == NULL)
        perror("getcwd error");
    else
        printf("%s\n", dirname);
}