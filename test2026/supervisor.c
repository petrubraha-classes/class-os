//! 0 is treated as the end of the file

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <string.h>
#include <semaphore.h>
#include "util.h"

void send(const char *const path, const int fd)
{
  FILE *file = fopen(path, "r");
  if (NULL == file)
    report_error(__LINE__, __FILE__, "error: open() failed");

  // read file
  for (char line[STRG_SIZE] = ""; fgets(line, STRG_SIZE, file);)
  {
    int number[3] = {0, 0, 0};
    char *buffer = strtok(line, " \n");
    number[0] = atoi(buffer);
    buffer = strtok(NULL, " \n");
    number[1] = atoi(buffer);
    buffer = strtok(NULL, " \n");
    number[2] = atoi(buffer);

    if (-1 == write(fd, &number, sizeof(number)))
      report_error(__LINE__, __FILE__, "error: write() failed");
  }

  if (-1 == fclose(file))
    report_error(__LINE__, __FILE__, "error: write() failed");
}

void recv(const int fifo, sem_t *sem)
{
  if (-1 == sem_wait(sem))
    report_error(__LINE__, __FILE__, "error: sem_wait() failed");

  int number[5] = {0, 0, 0, 0, 0}, bytes = 0;
  int count = 0, count_invalid = 0;
  while (0 < (bytes = read(fifo, number, sizeof(number))))
  {
    count++;
    if (0 == number[3])
    {
      count_invalid++;
      fprintf(stderr, "tripleta a,b,c nu poate reprezenta"
                      " lungimile laturilor unui triunghi.\n");
    }
    else
      fprintf(stderr, "tipleta a,b,c"
                      " reprezintă lungimile laturilor unui triunghi"
                      " ce are perimetrul %d și aria %d.\n",
              number[3], number[4]);
  }

  if (-1 == bytes)
    report_error(__LINE__, __FILE__, "read() failed in receive()");

  fprintf(stderr, "variante totale: %d; variante invalide: %d.\n",
          count, count_invalid);
}

int main(int argc, char *argv[])
{
  // input validation
  if (2 != argc)
    report_error(__LINE__, __FILE__, "wrong number of arguments");

  int pipe_anonymous[2] = {0, 0};
  if (-1 == pipe(pipe_anonymous))
    report_error(__LINE__, __FILE__, "pipe() failed");

  pid_t pid = fork();
  if (-1 == pid)
    report_error(__LINE__, __FILE__, "fork() failed");

  // child
  if (0 == pid)
  {
    if (-1 == close(pipe_anonymous[1]))
      report_error(__LINE__, __FILE__, "child close() failed");
    if (-1 == dup2(pipe_anonymous[0], STDIN_FILENO))
      report_error(__LINE__, __FILE__, "parent close() failed");

    execl(WRK1_PATH, WRK1_PATH, NULL);
    report_error(__LINE__, __FILE__, "child execl() failed");
  }

  // parent
  else
  {
    if (-1 == close(pipe_anonymous[0]))
      report_error(__LINE__, __FILE__, "parent close(0) failed");

    if (-1 == mkfifo(FIFO_PATH, 0600) && EEXIST != errno)
      report_error(__LINE__, __FILE__, "mkfifo() failed");
    int fifo = open(FIFO_PATH, O_RDONLY);
    if (-1 == fifo)
      report_error(__LINE__, __FILE__, "parent open() failed");

    sem_t *sem_recv = sem_open(SEM1_PATH, O_CREAT | O_RDWR, 0600, 0);
    if (SEM_FAILED == sem_recv)
      report_error(__LINE__, __FILE__, "sem_open() failed");

    //! transmission
    send(argv[1], pipe_anonymous[1]);
    if (-1 == close(pipe_anonymous[1]))
      report_error(__LINE__, __FILE__, "parent close(1) failed");

    recv(fifo, sem_recv);

    // close
    if (-1 == wait(NULL))
      report_error(__LINE__, __FILE__, "wait() failed");

    if (-1 == close(fifo))
      report_error(__LINE__, __FILE__, "parent close() failed");
    if (-1 == sem_close(sem_recv))
      report_error(__LINE__, __FILE__, "sem_close() failed");
    if (-1 == sem_unlink(SEM1_PATH))
      report_error(__LINE__, __FILE__, "sem_unlink() failed");
  }
}
