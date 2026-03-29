#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX 100

int main()
{
    char command[MAX] = "cut /etc/passwd";
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("error at forking\n");
        return 1;
    }

    if(pid == 0)
    {
        //system(command)
        
        char name[MAX], argument[MAX];
        char* args[MAX];
        size_t index_args = 0, index_argument = 0;

        // command index
        size_t index = 0;
        int  flag = 0;
        
        while(command[index])
        {
            // argument storing
            char temp = command[index];
            argument[index_argument++] = temp;
            argument[index_argument] = '\0';
            
            //
            if(command[index] == ' ')
            {
                if(flag == 0)
                {
                    strcpy(name, argument);
                    flag = 1;
                }
                else
                    strcpy(args[index_args++], argument);
                strcpy(argument, "\0");
                index_argument = 0;
            }
            index++;
        }

        args[index_args] = NULL;

        int status = execv(name, args);
        if(-1 != status)
            return 0;
        perror("error at execv");
        exit(1);
    }
    
    return 0;
}
