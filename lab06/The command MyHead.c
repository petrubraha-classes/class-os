#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#define DEFAULT_PARAMETER 10
#define ERROR_CODE -1

void head_line(const int fd, const int n, const char *const path)
{
  if (0 == n)
  {
    printf("\n");
    return;
  }

  struct stat st;
  if (-1 == stat(path, &st))
  {
    perror("stat");
    exit(1);
  }

  char *buffer = (char *)malloc(st.st_size);
  int cnt = 0;

  if (-1 == read(fd, buffer, st.st_size))
  {
    perror("read");
    exit(ERROR_CODE);
  }

  size_t newline_index = 0;
  size_t *newline_positions = (size_t *)malloc(sizeof(size_t) * st.st_size);
  for (size_t i = 0; i < st.st_size; i++)
    if ('\n' == buffer[i])
      newline_positions[newline_index++] = i;

  // positive edge case
  if (n > newline_index)
  {
    printf("%s\n", buffer);
    return;
  }

  // negative - edge case
  if (n < 0 && -1 * (size_t)n > newline_index)
  {
    printf("\n");
    return;
  }

  if (n > 0)
    write(STDOUT_FILENO, buffer, newline_positions[n - 1]);
  else
    write(STDOUT_FILENO, buffer, newline_positions[newline_index + n - 1]);

  printf("\n");
}

void head_bytes(int fd, int n)
{
  char buffer;
  int cnt = 0;

  while (cnt < n && read(fd, &buffer, 1) > 0)
  {
    cnt++;
    if (-1 == write(STDOUT_FILENO, &buffer, 1))
    {
      perror("write");
      exit(ERROR_CODE);
    }
  }
  printf("\n");
}

int main(int argc, char *argv[])
{
  if (4 != argc && 2 != argc)
  {
    perror("USAGE: <option> <number> <filename>\n");
    return ERROR_CODE;
  }

  if (2 == argc)
  {
    int fd = open(argv[1], O_RDONLY, 0644);
    if (-1 == fd)
    {
      perror("open");
      return ERROR_CODE;
    }

    head_line(fd, 1, argv[1]);
    close(fd);
    return 0;
  }

  // if (4 == argc)
  char *option = argv[1];
  int n = atoi(argv[2]);
  int fd = open(argv[3], O_RDONLY, 0644);
  if (-1 == fd)
  {
    perror("open");
    return ERROR_CODE;
  }

  if (strncmp(option, "-n", 2) == 0)
    head_line(fd, n, argv[1]);
  else if (strncmp(option, "-c", 2) == 0)
    head_bytes(fd, n);
  else
  {
    printf("UNKNOWN PARAMETER: %s", option);
    return ERROR_CODE;
  }

  close(fd);
}
