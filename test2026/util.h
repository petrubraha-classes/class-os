#ifndef __UTIL__
#define __UTIL__

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

void report_error(
    const int line, const char *const file,
    const char *const mess)
{
  printf("line: %d, file: %s, error: %s - %s.\n",
         line, file, mess, strerror(errno));
  exit(EXIT_FAILURE);
}

#define STRG_SIZE 1024

const char SEM0_PATH[20] = "sem_send";
const char SEM1_PATH[20] = "sem_recv";
const char SEM2_PATH[20] = "sem_intr";
const int SHMO_SIZE = 1024 * 4 * sizeof(int);
const char SHMO_PATH[20] = "mem_shrd";
const char FIFO_PATH[20] = "fifo2025";
const char WRK1_PATH[30] = "worker1";

#endif
