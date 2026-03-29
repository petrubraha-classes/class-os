#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define ERROR_CODE -1
#define ERROR_STRING_MAX 30
#define STRING_MAX 4096

int main(int argc, char *argv[])
{
  // argument validation
  if (5 != argc)
  {
    write(STDOUT_FILENO,
          "error: wrong number of arguments.\n",
          strlen("error: wrong number of arguments.\n"));
    return ERROR_CODE;
  }

  int flag = access(argv[1], R_OK);
  if (0 != flag)
  {
    write(STDOUT_FILENO,
          "error: can not read from the first file.\n",
          strlen("error: can not read from the first file.\n"));
    return ERROR_CODE;
  }

  flag = access(argv[2], W_OK);
  if (0 == flag)
  {
    // send confirmation
    int bytes =
        write(STDOUT_FILENO,
              "confirmation to override: (y/n)\n",
              strlen("confirmation to override: (y/n)\n"));
    if (-1 == bytes)
    {
      write(STDOUT_FILENO,
            "error: can not print confirmation.\n",
            strlen("error: can not print confirmation.\n"));
      return ERROR_CODE;
    }

    char decision = 'y';
    bytes = read(STDIN_FILENO, &decision, sizeof(decision));
    if (-1 == bytes)
    {
      write(STDOUT_FILENO,
            "error: can not read confirmation.\n",
            strlen("error: can not read confirmation.\n"));
      return ERROR_CODE;
    }

    if ('n' == decision)
    {
      write(STDOUT_FILENO,
            "user rejected overwriting.\n",
            strlen("user rejected overwriting.\n"));
      return 0;
    }
  }

  // the file does not exist OR
  // the user accepted overwriting
  const int fd0 = open(argv[1], O_RDONLY),
            fd1 = open(argv[2], O_RDWR | O_CREAT, 0777);
  if (-1 == fd0 || -1 == fd1)
  {
    write(STDOUT_FILENO,
          "error: file openning failed.\n",
          strlen("error: file openning failed.\n"));
    return ERROR_CODE;
  }

  // get the parameters
  const char ch0 = *argv[3], ch1 = *argv[4];

  char buffer[STRING_MAX];
  for (int bytes = read(fd0, buffer, STRING_MAX); bytes;
       bytes = read(fd0, buffer, STRING_MAX))
  {

    // validate the read
    if (-1 == bytes)
    {
      write(STDOUT_FILENO,
            "error: read() failed.\n",
            strlen("error: read() failed.\n"));
      return ERROR_CODE;
    }

    // do the operation
    for (int b = 0; b < bytes; b++)
      if (ch0 == buffer[b])
        buffer[b] = ch1;

    // validate the write
    int bytes_output = write(fd1, buffer, bytes);
    if (bytes_output != bytes)
    {
      write(STDOUT_FILENO,
            "error: write() failed.\n",
            strlen("error: write() failed.\n"));
      return ERROR_CODE;
    }
  }
}
