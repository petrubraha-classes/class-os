#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void process_creation(const char* cmd, char*const argv[], int fd_in, int fd_out)
{
    pid_t pid=fork();
    if(pid==-1)
    {
        perror("error: can not fork.\n");
        exit(1);
    }
    if (pid) return;
    
    if(fd_in!=-1)
    {
        dup2(fd_in, STDIN_FILENO);
        close(fd_in);
    }

    if(fd_out!=-1)
    {
        dup2(fd_out, STDOUT_FILENO);
        close(fd_out);
    }

    execvp(cmd, argv);
    perror("ERROR Executing the command\n");
    exit (2);
}

int main()
{
    int p1[2], p2[2], p3[2];

    if(pipe(p1)==-1 || pipe(p2)==-1 || pipe(p3)==-1)
    {
        perror("error: can not create pipe.\n");
        exit(3);
    }

    char* ps[]={"ps", "-eo", "user,comm,pid", "--no-headers", NULL};
    process_creation("ps", ps, -1, p1[1]);
    close(p1[1]);

    char* tr[]={"tr", "-s", " ", NULL};
    process_creation("tr", tr, p1[0], p2[1]);
    close(p1[0]);
    close(p2[1]);

    char* cut[]={"cut", "-d", " ", "-f1,2,3", "--output-delimiter=:", NULL};
    process_creation("cut", cut, p2[0], p3[1]);
    close(p2[0]);
    close(p3[1]);

    char* sort[]={"sort", "-k2", "-t:", NULL};
    process_creation("sort", sort, p3[0], -1);
    close(p3[0]);

    return 0;
}
