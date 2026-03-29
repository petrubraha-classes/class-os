
#include <stdio.h>
#include <stdlib.h>

#include <fcntl.h>
#include <unistd.h>
#include <sys/syscall.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>

void print_error(const char *const message)
{
  fprintf(stderr, "error: %s.\n", message);
  exit(EXIT_FAILURE);
}

void same_file(
    const int fd,
    const char ch0, const char ch1)
{
  struct stat file_data;
  if (-1 == fstat(fd, &file_data))
    print_error("fstat() failed()");

  void *addr =
      mmap(NULL, file_data.st_size,
           PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  void *final = addr + file_data.st_size;
  if (MAP_FAILED == addr)
    print_error("mmap() failed");

  // process
  for (size_t i = 0; addr != final; addr++)
    if (ch0 == *(char *)addr)
      *(char *)addr = ch1;

  // close the job
  if (-1 == msync(addr, file_data.st_size, MS_SYNC))
    print_error("msync() failed");
  if (-1 == munmap(addr, file_data.st_size))
    print_error("munmap() failed");
}

// file file char char
int main(int argc, char *argv[])
{
  if (4 != argc && 5 != argc)
    print_error("wrong number of arguments");

  char *path0 = argv[1];
  char *path1 = argv[1];
  char ch0 = *argv[2], ch1 = *argv[3];
  if (5 == argc)
  {
    path1 = argv[2];
    ch0 = *argv[3];
    ch1 = *argv[4];
  }

  if (-1 == access(path0, R_OK))
    print_error("unable to read from the first file");

  // ask confirmation
  if (-1 != access(path0, F_OK))
  {
    char decision = 'N';
    printf("do you want to overwrite the file? [Y/N]\n");
    scanf("%c", &decision);
    if ('N' != decision && 'Y' != decision)
      print_error("invalid input");
    if ('N' == decision)
    {
      printf("the file won't be overwritten.\n");
      exit(EXIT_SUCCESS);
    }
  }

  int fd0 = -1;
  int fd1 = -1;
  if (0 == strcmp(path0, path1))
    fd0 = fd1 = open(path0, O_RDWR);
  else
    fd0 = open(path0, O_RDONLY), fd1 = open(path1, O_WRONLY | O_CREAT);
  if (-1 == fd0 || -1 == fd1)
    print_error("open() failed");

  // edge case
  if (fd0 == fd1)
  {
    same_file(fd0, ch0, ch1);
    exit(EXIT_SUCCESS);
  }

  struct stat file_data;
  if (-1 == fstat(fd0, &file_data))
    print_error("fstat() failed()");

  void *add0 = mmap(NULL, file_data.st_size, PROT_READ, MAP_PRIVATE, fd0, 0);
  void *add1 = mmap(NULL, file_data.st_size, PROT_WRITE, MAP_SHARED, fd1, 0);
  if (MAP_FAILED == add0 || MAP_FAILED == add1)
    print_error("mmap() failed");

  // process
  void *final = add0 + file_data.st_size;
  for (; add0 != final; add0++, add1++)
    if (ch0 == *(char *)add0)
      *(char *)add1 = ch1;
    else
      *(char *)add1 = *(char *)add0;

  // close the job
  if (-1 == msync(add1, file_data.st_size, MS_SYNC))
    print_error("msync() failed");
  if (-1 == munmap(add0, file_data.st_size))
    print_error("munmap() failed");
  if (-1 == munmap(add1, file_data.st_size))
    print_error("munmap() failed");
  return EXIT_SUCCESS;
}
