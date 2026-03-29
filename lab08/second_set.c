#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main()
{
    pid_t pid = fork();
    int status;

    if(-1 == pid)
    {
        perror("error at fork");
        exit(1);
    }
    else if(pid==0)
    {
        char* args[]={"find", getenv("HOME"), "-name", ".c", "-or", "-name", ".sh", "printf", "%p : %k KB\n", NULL};
        int status = execvp("/usr/bin/find", args);
        if(status != -1)
            return 0;
        perror("error at execvp");
        exit(10);
    }
    
    if(wait(&status) == -1)
    {
        perror("error at wait");
        exit(1);
    }

    if(WIFEXITED(status) && (WEXITSTATUS(status) == 0))
    {
        printf("Comanda find a fost executata...");
        return 0;
    }
    
    printf("Comanda find nu a putut fi executata...");
    return 1;
}