#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
  if (3 != argc)
  {
    fprintf(stderr, "error: wrong number of arguments");
    exit(EXIT_FAILURE);
  }

  char *file = argv[1];
  int N = atoi(argv[2]);

  if (-1 == access(file, W_OK))
  {
    fprintf(stderr, "error: invalid file");
    exit(EXIT_FAILURE);
  }

  if (N <= 0)
  {
    fprintf(stderr, "error: invalid n");
    exit(EXIT_FAILURE);
  }

  // file N
  pid_t pid = 0;
  if (-1 == (pid = fork()))
  {
    fprintf(stderr, "error: fork() failed");
    exit(EXIT_FAILURE);
  }

  if (0 == pid)
  {
    // asteapta SIGUSR2
    // printf("primt");
    // printf("gata");
  }
  else
  {
    int fd = open(file, O_WRONLY);
    if (-1 == fd)
    {
      fprintf(stderr, "error - parent: open() failed");
      exit(EXIT_FAILURE);
    }

    // alarm 5 sec - no input
    // if(no input in 5 * N secunde)
    //   close
    //   kill(pid, SIGUSR2);
    //   printf(bad stuff);
    //   wait for SIGCHLD
    //   exit(1);
    // if(ctrl d)
    //   close
    //   kill(pid, SIGUSR2);
    //   wait for SIGCHLD
    //   exit(0)
    // Parinte: sfarsit executie!"
  }

  return EXIT_SUCCESS;
}
