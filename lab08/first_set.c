#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    pid_t pid = fork();
    if(pid == -1)
    {
        perror("error at forking\n");
        return 1;
    }

    if(pid == 0)
    {
        char* args[]={"cut", "--fields", "1,3,4", "-d:", "--output-delimiter= -- ", "/etc/passwd", NULL};
        int status = execv("/usr/bin/cut", args);
	    if(-1 != status)
            return 0;
        perror("error at execv");
        exit(10);
    }
    
    int stat;
    if(waitpid(pid, &stat, 0) == -1)
    {
        perror("error at waitpid");
        exit(1);
    }

    if(WIFEXITED(stat) && (WEXITSTATUS(stat)==0)){
        printf("Comanda cut a fost executata ...");
        return 0;
    }
    
    printf("Comanda cut nu a putut fi executata...");
    return 1;
}
