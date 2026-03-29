#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <signal.h>
#include <fcntl.h>
#include <time.h>

int *shared_memory, n;

pid_t process_creation()
{
    pid_t pid = fork();
    if (-1 == pid)
    {
        perror("error: can not fork.\n");
        exit(1);
    }

    return pid;
}

int *integers_generation(const int n)
{
    srand(time(0));
    int *array = malloc(n * sizeof(int));
    for (int i = 0; i < n; i++)
        array[i] = rand();
    return array;
}

int convert_char(char *array)
{
    if (array[0] == '-')
        return 0;

    int sum = 0;
    size_t i = 0;
    while (array[i])
    {
        sum = sum * 10 + (array[i] - '0');
        i++;
    }
    return sum;
}

void handler1(int sig)
{
    int sum = 0;
    for (int i = 0; i < n; i++)
        sum ^= shared_memory[i];
    printf("XOR sum = %d.\n", sum);
    kill(getpid(), SIGUSR1);
}

void handler2(int sig)
{
    for (int i = 0; i < n; i++)
    {
        for (int j = i + 1; j < n; j++)
        {
            if (shared_memory[i] < shared_memory[j])
            {
                int aux = shared_memory[i];
                shared_memory[i] = shared_memory[j];
                shared_memory[j] = aux;
            }
        }
    }

    printf("Sorted array\n");
    for (int i = 0; i < n; i++)
        printf("%d ", shared_memory[i]);
    printf("\n");
}

int main(int argc, char **argv)
{
    if (argc == 1)
        return 1;
    n = convert_char(argv[1]);

    // mapping
    size_t size = n * sizeof(int);
    shared_memory = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_PRIVATE, -1, 0);
    if (shared_memory == MAP_FAILED)
    {
        perror("error: can not map");
        exit(1);
    }

    // two child_processes
    pid_t pid1 = process_creation();
    if (pid1 == 0)
    {
        pid_t pid2 = process_creation();
        if (pid2 == 0)
        {
            signal(SIGUSR2, handler2);
            signal(SIGUSR1, handler2);
            pause();
            exit(0);
        }

        signal(SIGUSR2, handler1);
        pause();
        exit(0);
    }

    // parent
    shared_memory = integers_generation(n);
    if (-1 == kill(pid1, SIGUSR2) || -1 == kill(1, SIGUSR2))
    {
        perror("error: can not send signal.\n");
        exit(5);
    }

    wait(NULL);
    wait(NULL);

    // delete map
    if (-1 == munmap(shared_memory, size))
    {
        perror("error: can not delete map.\n");
        exit(100);
    }
    return 0;
}
