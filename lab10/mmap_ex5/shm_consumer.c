#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <fcntl.h>           /* For O_* constants */
#include <stdio.h>
#include <stdlib.h>

#define handle_error(msg,cod) \
    { perror(msg); exit(cod); }

int main()
{
    // const int Page_Size = 4096;
    const int Messages_MaxSize = 100;
    const char *shm_name = "/shared_memory_obj1"; // Same name as in the producer program.

    // sleep(1); // Some pause required for the producer to create the object, but only if they are run in parallel !

    /* Open shared memory object */
    int shm_fd = shm_open(shm_name, O_RDONLY, 0);
    if(-1 == shm_fd) handle_error("Error at shm_open",1);

    // It is not necessary to do the truncate operation here too!

    /* Map the object into the consumer's address space. */
    char *start_addr = mmap(NULL, Messages_MaxSize, PROT_READ, MAP_SHARED, shm_fd, 0);
    // Basically, an entire page will be mapped, because Messages_MaxSize <= Page_Size !
    if(MAP_FAILED == start_addr) handle_error("Error at mmap",3);

    /* The file descriptor can now be closed. */
    if(-1 == close(shm_fd)) handle_error("Error at close",4);

    /* Read a message from the shared memory object. */
    if(! (start_addr[0] && start_addr[1] && start_addr[2]) )
        printf("[Consumer, PID: %d] In the shared memory there are only zeros (at least at the first 3 addresses) !\n",
            getpid()); // This can happen only if the consumer reads the memory before the producer writes it!
    else
        printf("[Consumer, PID: %d] I have read the following text from the shared memory region:\n%s\n",
            getpid(), start_addr);

    /* Eventually, we can destroy the object now, but only if it was not destroyed in the producer,
       because it will give an error if it is destroyed 2 times ! */

    // if(-1 == shm_unlink(shm_name)) handle_error("Error at shm_unlink",5);

    /* But to be sure, is better to delete the object from commandline, after running the consumer as many times we want. */

    return 0;
}

//named