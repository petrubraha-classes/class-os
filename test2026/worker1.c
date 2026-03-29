#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include "util.h"

int check_triangle(int number[4])
{
  if (number[0] <= 0 || number[1] <= 0 ||
      number[2] <= 0)
    return 0;

  if (number[0] + number[1] < number[2] ||
      number[1] + number[2] < number[0] ||
      number[2] + number[0] < number[1])
    return 0;
  return 1;
}

void send_w2(const int fd, void *shm_ptr, sem_t *sem)
{
  int number[4] = {0, 0, 0, 0};

  int bytes = 0;
  while (0 < (bytes = read(fd, number, 3 * sizeof(int))))
  {
    number[3] = check_triangle(number);
    memcpy(shm_ptr, number, sizeof(number));
    shm_ptr += sizeof(number);
  }

  // this can help me determine when to stop reading
  int termination[4] = {0, 0, 0, 0};
  memcpy(shm_ptr, termination, sizeof(termination));

  if (-1 == bytes)
    report_error(__LINE__, __FILE__, "read() failed in receive()");

  if (-1 == sem_post(sem))
    report_error(__LINE__, __FILE__, "sem_post() failed");
}

int main()
{
  // map
  int shm_fd = shm_open(SHMO_PATH, O_CREAT | O_RDWR, 0600);
  if (-1 == shm_fd)
    report_error(__LINE__, __FILE__, "shm_open() failed");

  if (-1 == ftruncate(shm_fd, SHMO_SIZE))
    report_error(__LINE__, __FILE__, "ftruncate() failed");

  void *shm_ptr = mmap(
      NULL, SHMO_SIZE,
      PROT_WRITE | PROT_READ,
      MAP_SHARED, shm_fd, 0);
  if (MAP_FAILED == shm_ptr)
    report_error(__LINE__, __FILE__, "mmap() failed");

  if (-1 == close(shm_fd))
    report_error(__LINE__, __FILE__, "close() failed");

  // semaphore
  sem_t *sem_send = sem_open(SEM0_PATH, O_CREAT | O_RDWR, 0600, 0);
  if (SEM_FAILED == sem_send)
    report_error(__LINE__, __FILE__, "sem_open() failed");

  //! transmission
  send_w2(STDIN_FILENO, shm_ptr, sem_send);

  // close
  if (-1 == munmap(shm_ptr, SHMO_SIZE))
    report_error(__LINE__, __FILE__, "munmap() failed");
  if (-1 == sem_close(sem_send))
    report_error(__LINE__, __FILE__, "sem_close() failed");
}
