/*
  Program: CS-synced_v2.c

  Functionality: solves the critical section problem -- synchronization is achieved using an unnamed semaphore.
  This program is supposed to be run as a SPMD job (i.e., multiple instances of it running in parallel).

  Remark: the solution based on a binary semaphore ensures the mutual exclusion property
  (possibly also the progress property, but not the bounded waiting property).
*/

#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>    /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#define handle_error(msg, cod) \
  {                            \
    perror(msg);               \
    exit(cod);                 \
  }

/* Define a structure that will be imposed on the shared memory object */
struct shm_struct
{
  unsigned long int shm_cnt_data;         /* Data value being shared */
  sem_t shm_semaphore;                    /* A POSIX unnamed semaphore */
  int shm_cnt_instances;                  /* How many instances of this program, run in paralel, are live now ? */
  int shm_bool_semaphore_was_initialized; /* It will be set after the sem_init call! */
};

struct shm_struct *shm_ptr;

void critical_section(int n)
{
  fprintf(stderr, "[PID: %d] In the Critical Section for the %dth time: starting to execute it...\n", getpid(), n);

  /* Doing some critical work, e.g. accessing a shared resource. */
  usleep(random() % 10000); // A random pause of up to 10 milliseconds to simulate some calculations performed here.

  /* Incrementing the shared data value. */
  ++shm_ptr->shm_cnt_data;
  // Attention: this operation is not atomic, thus it can leads to data races if it is not done in CS !!!

  fprintf(stderr, "[PID: %d] In the Critical Section for the %dth time: finished to execute it.\n", getpid(), n);
}

void remainder_section(int n)
{
  fprintf(stderr, "[PID: %d]\tOut of CS: starting to execute the remainder section for the %dth time...\n", getpid(), n);

  /* Doing some NON critical work, i.e. NOT accessing any shared resource. */
  usleep(random() % 20000); // A random pause of up to 20 milliseconds to simulate some calculations performed here.

  fprintf(stderr, "[PID: %d]\tOut of CS: finished to execute the remainder section for the %dth time.\n", getpid(), n);
}

int main(int argc, char *argv[])
{
  int nr_iterations, nlocal_exit_order, bool__first_at_start;
  const char *shm_name = "/shared_memory_obj1";

  /* Validate the required arguments in the command line. */
  if (argc < 2)
  {
    fprintf(stderr, "Usage: %s <number-of-iterations>\n", argv[0]);
    return 1;
  }
  if (1 != sscanf(argv[1], "%d", &nr_iterations))
  {
    fprintf(stderr, "Invalid argument: %s\n", argv[1]);
    return 2;
  }

  /* Initialize the random number generator. */
  srandom(getpid());

  bool__first_at_start = 1;
  /* Create the shared memory object... */
  int shm_fd = shm_open(shm_name, O_RDWR | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR); // permisiuni 0600
  if (-1 == shm_fd)
  {
    if (errno == EEXIST)
      bool__first_at_start = 0;
    else
      handle_error("Error at 1st shm_open", 3);
  }
  if (!bool__first_at_start)
  { // If I did not create the shared memory object, then I will just open it.
    /* Open the shared memory object... */
    shm_fd = shm_open(shm_name, O_RDWR, 0);
    if (-1 == shm_fd)
      handle_error("Error at 2nd shm_open", 3);
  }

  /* ... and set its size to the size of our structure. */
  if (-1 == ftruncate(shm_fd, sizeof(*shm_ptr)))
    handle_error("Error at ftruncate", 4);

  /* Map the object into the process's address space. */
  shm_ptr = mmap(NULL, sizeof(*shm_ptr), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
  // Basically, an entire page will be mapped, because sizeof(*shm_ptr) <= Page_Size
  if (MAP_FAILED == shm_ptr)
    handle_error("Error at mmap", 5);

  /* The file descriptor can now be closed. */
  if (-1 == close(shm_fd))
    handle_error("Error at close", 6);

  /* Initialize the unnamed semaphore as process-shared, with value 1, but only once! */
  // Otherwise, if the initialization is done multiple times, the mutual exclusion property will not hold !!!
  if (bool__first_at_start)
  { // If I created the shared memory object, then I will initialize the semaphore.
    if (-1 == sem_init(&shm_ptr->shm_semaphore, 1, 1))
      handle_error("Error at sem_init", 7);
    // The initial value of the semaphore is 1 (we are using a binary semaphore to ensure mutual exclusion).
    shm_ptr->shm_bool_semaphore_was_initialized = 1; // Set the flag for waking up all other instances.
  }
  else
  { // All other instances of the SPMD job will wait here for the semaphore to be initialized.
    do
    {
      usleep(1000);
    } while (!shm_ptr->shm_bool_semaphore_was_initialized);
  }

  /* The main loop of the program */
  for (int i = 1; i <= nr_iterations; i++)
  {
    /* Entry section:  wait(semaphore); */
    if (-1 == sem_wait(&shm_ptr->shm_semaphore))
      handle_error("Error at sem_wait", 8);

    if (i == 1)
    {
      /* Incrementing the shared counter of instances. */
      ++shm_ptr->shm_cnt_instances;
      // Attention: this operation is not atomic, thus it can leads to data races if it is not done in CS !!!
    }

    /* The critical section. */
    critical_section(i);

    if (i == nr_iterations)
    {
      /* Decrementing the shared counter of instances. */
      //-- shm_ptr->shm_cnt_instances;
      nlocal_exit_order = shm_ptr->shm_cnt_instances;
      shm_ptr->shm_cnt_instances = nlocal_exit_order - 1;
      // Attention: this operation is not atomic, thus it can leads to data races if is not done in CS !!!
    }

    /* Exit section:  signal(semaphore); */
    if (-1 == sem_post(&shm_ptr->shm_semaphore))
      handle_error("Error at sem_post", 9);

    /* The remainder (non-critical) section. */
    remainder_section(i);
  }

  fprintf(stderr, "[PID: %d]\t\tI finished my last iteration in CS on the %dth place (in reverse order).\n",
          getpid(), nlocal_exit_order);

  /* Destroy the unnamed semaphore, but only once (by the last instance of the SPMD job...) !
     And after destroying the unnamed semaphore, the memory in which it was located can be "deallocated", i.e.
     in this case the shared memory object can be destroyed, but only once (by the last instance of the SPMD job).
     And the final value of the shared data value is obtained by the last instance of the SPMD job at its end. */
  if (nlocal_exit_order == 1)
  {
    /* Destroy the unnamed semaphore. */
    if (-1 == sem_destroy(&shm_ptr->shm_semaphore))
      handle_error("Error at sem_destroy", 10);

    /* Delete the name of the shared memory object;
       the object itself will be destroyed after all the instances of the SPMD job will unmmap the object. */
    if (-1 == shm_unlink(shm_name))
      handle_error("Error at shm_unlink", 11);

    /* Print the final value of the shared data value (because you are the last process who incremented it !). */
    printf("[PID: %d] The final value of the shared counter is: %lu\n", getpid(), shm_ptr->shm_cnt_data);
  }

  if (-1 == munmap(shm_ptr, sizeof(*shm_ptr)))
    handle_error("Error at munmap", 12);
  // Note: this is not really necessary here, because unmapping is done automatically when the process is terminated.

  return 0;
}