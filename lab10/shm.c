#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int main()
{
  int shm = shm_open("dada", O_CREAT | O_RDWR, 0600);
  if (-1 == shm)
    perror("shm_open()");

  if (ftruncate(shm, 6))
    perror("ftruncate()");

  void *map = mmap(
      NULL, 6, PROT_READ | PROT_WRITE,
      MAP_SHARED, shm, 0);
  if (MAP_FAILED == map)
    perror("mmap()");

  if (close(shm) == -1)
    perror("close()");

  // memcpy(map, "dada", 5);
  printf("%s\n", map);

  if (munmap(map, 6))
    perror("munmap()");

  // if (shm_unlink("dada") == -1)
  // perror("shm_unlink()");
}
