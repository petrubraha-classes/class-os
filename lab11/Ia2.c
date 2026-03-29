#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

const char command[] = "w -h | tr -s " " | cut -d" " -f1,8 | sort -t " " -k 1";

void process_creation(char* command, char** arguments, int fd_in, int fd_out)
{
    pid_t pid = fork();
    if(-1 == pid)
    {
        perror("error: can not fork.\n");
        exit(1);
    }
    if(pid) return;
    if(-1 != fd_in)
    {
        dup2(fd_in, 0);
        close(fd_in);
    }

    if(-1 != fd_out)
    {
        dup2(fd_out, 1);
        close(fd_out);
    }

    execvp(command, arguments);
    perror("error: can not execute.\n");
    exit(3);
}

int main()
{
    int one_two[2], two_three[2], three_four[2];

    if(-1 == pipe(one_two))
        exit(4);
    if(-1 == pipe(two_three))
        exit(5);
    if(-1 == pipe(three_four))
        exit(6);

    char* pipe1[]={"-h", NULL};
    process_creation("w", pipe1, -1, one_two[1]);
    close(one_two[1]);

    char* pipe2[]={"-s", " ", NULL};
    process_creation("tr", pipe2, one_two[0], two_three[1]);
    close(one_two[0]);
    close(two_three[1]);

    char* pipe3[]={"-d", " ", "-f1,8", NULL};
    process_creation("cut", pipe3, two_three[0], three_four[1]);
    close(two_three[0]);
    close(three_four[1]);

    char* pipe4[]={"-t", " ", "k", "1", NULL};
    process_creation("sort", pipe3, three_four[1], -1);
    close(three_four[0]);
    
    return 0;
}
