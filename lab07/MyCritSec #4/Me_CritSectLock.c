#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>

typedef struct
{
  int item_code;
  float item_stock;
} Produce;

void process_operation(int fd, int item_code, float item_stock)
{
  Produce p;
  int is_found = 0;
  off_t position = 0;
  struct flock lock;

  while (1)
  {
    lock.l_type = F_RDLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = position;
    lock.l_len = sizeof(Produce);

    if (fcntl(fd, F_SETLKW, &lock) < 0)
    {
      perror("fcntl - read lock");
      exit(2);
    }

    ssize_t bytes_read = pread(fd, &p, sizeof(Produce), position);
    if (bytes_read == 0)
    {
      lock.l_type = F_UNLCK;
      if (fcntl(fd, F_SETLKW, &lock) < 0)
      {
        perror("fcntl - unlock");
        exit(2);
      }
      break;
    }
    else if (bytes_read != sizeof(Produce))
    {
      perror("pread");
      exit(3);
    }

    if (p.item_code == item_code)
    {
      is_found = 1;

      lock.l_type = F_UNLCK;
      if (fcntl(fd, F_SETLKW, &lock) < 0)
      {
        perror("fcntl - unlock");
        exit(2);
      }
      break;
    }

    lock.l_type = F_UNLCK;
    if (fcntl(fd, F_SETLKW, &lock) < 0)
    {
      perror("fcntl - unlock");
      exit(2);
    }

    position += sizeof(Produce);
  }

  if (is_found)
  {
    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = position;
    lock.l_len = sizeof(Produce);

    if (fcntl(fd, F_SETLKW, &lock) < 0)
    {
      perror("fcntl - write lock");
      exit(2);
    }

    if (pread(fd, &p, sizeof(Produce), position) != sizeof(Produce))
    {
      perror("pread");
      exit(3);
    }

    if (item_stock < 0 && p.item_stock < -item_stock)
    {
      printf("[PID %d] ERROR: Insuficient item_stock for product with id %d (Current: %.2f, given quant: %.2f)\n", getpid(), item_code, p.item_stock, -item_stock);

      lock.l_type = F_UNLCK;
      fcntl(fd, F_SETLKW, &lock);
      return;
    }

    p.item_stock += item_stock;

    if (pwrite(fd, &p, sizeof(Produce), position) != sizeof(Produce))
    {
      perror("pwrite");
      exit(3);
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);

    printf("[PID %d] Updated product %d: New item_stock: %.2f\n", getpid(), item_code, p.item_stock);
  }
  else if (item_stock > 0)
  {
    p.item_code = item_code;
    p.item_stock = item_stock;

    off_t end_pos = lseek(fd, 0, SEEK_END);

    lock.l_type = F_WRLCK;
    lock.l_whence = SEEK_SET;
    lock.l_start = end_pos;
    lock.l_len = sizeof(Produce);
    if (fcntl(fd, F_SETLKW, &lock) < 0)
    {
      perror("fcntl - lock new entry");
      exit(2);
    }

    if (write(fd, &p, sizeof(Produce)) != sizeof(Produce))
    {
      perror("pwrite");
      exit(3);
    }

    lock.l_type = F_UNLCK;
    fcntl(fd, F_SETLKW, &lock);

    printf("[PID %d] Added product %d with item_stock %.2f\n", getpid(), item_code, item_stock);
  }
  else
  {
    printf("[PID %d] ERROR: Product %d doesn't exist and cannot be sold\n", getpid(), item_code);
    exit(1);
  }
}

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    fprintf(stderr, "USAGE: %s <Database> <Command File>\n", argv[0]);
    exit(1);
  }

  int fd = open(argv[1], O_RDWR);
  if (fd == -1)
  {
    perror("OPEN");
    exit(1);
  }

  FILE *all_instructions = fopen(argv[2], "r");
  if (!all_instructions)
  {
    perror("FOPEN");
    close(fd);
    exit(1);
  }

  int item_code = 0;
  char op = 0;
  float item_stock = 0.0;

  while (fscanf(all_instructions, "%d %c%f", &item_code, &op, &item_stock) == 3)
  {
    if (op == '-')
      item_stock = -item_stock;
    else if (op != '+')
    {
      fprintf(stderr, "Invalid operator %c \n", op);
      continue;
    }

    process_operation(fd, item_code, item_stock);
    sleep(5);
  }

  if (-1 == fclose(all_instructions))
    perror("FCLOSE");
  if (-1 == close(fd))
    perror("CLOSE");
  return EXIT_SUCCESS;
}
