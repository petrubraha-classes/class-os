/* Să se scrie un program C care primește de la linia de comandă numele unui fișier și afișează numărul total de apariții pentru fiecare caracter ce apare în acel fișier.
Cerință: se vor utiliza apelurile de sistem din API-ul POSIX pentru accesarea fișierului. */

#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

#define MAX_STRING 5
#define ASCII_SIZE 256

int freq[ASCII_SIZE];

int main(int argc, char *argv[])
{
  if (2 != argc)
  {
    fprintf(stderr, "wrong number of arguments.\n");
    return 1;
  }

  char *path = argv[1];
  int fd = open(path, O_RDONLY);
  if (-1 == fd)
  {
    perror("open() failed");
    return 2;
  }

  char buffer[MAX_STRING];
  int bytes = 1;
  while ((bytes = read(fd, buffer, MAX_STRING) > 0))
    for (size_t i = 0; i < MAX_STRING; i++)
      freq[buffer[i]]++;

  if (-1 == bytes)
  {
    perror("read() failed");
    return 3;
  }

  for (size_t i = 0; i < ASCII_SIZE; i++)
    if (freq[i])
      printf("codul ascii: %c apare de %d ori.\n", (char)i, freq[i]);
  return 0;
}
