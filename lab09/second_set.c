#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void cp_third(int level, int order, pid_t parent_pid, int num_children) {
    for (int i = order; i < order + num_children; ++i) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("error fork");
            exit(EXIT_FAILURE);
        } else if (pid == 0) { // Child process
            printf("(%d,%d): PID=%d, PPID=%d\n", level, i, getpid(), parent_pid);
            exit(EXIT_SUCCESS);
        } else wait(NULL); // parent
    }
}

int main() 
{
    pid_t pid = getpid();
    printf("(1,1): PID=%d, ROOT_PROCESS\n", pid);

    pid_t child_pid = fork();

    if (child_pid == -1) 
    {
        perror("fork");
        exit(1);
    } 

    if (child_pid == 0) 
    { 
        printf("(%d,%d): PID=%d, PPID=%d\n", 2, 1, getpid(), pid);
        cp_third(3, 1, getpid(), 5);
        exit(EXIT_SUCCESS);
    }

    wait(NULL);

    child_pid = fork();

    if (child_pid == -1) 
    {
        perror("fork");
        exit(EXIT_FAILURE);
    } 

    if (child_pid == 0) 
    { 
        printf("(%d,%d): PID=%d, PPID=%d\n", 2, 2, getpid(), pid);
        cp_third(3, 6, getpid(), 5);
        exit(EXIT_SUCCESS);
    }

    wait(NULL);
    return 0;
}  