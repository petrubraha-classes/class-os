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
    const char *shm_name = "/shared_memory_obj1"; // The name of the POSIX shared memory object which will be created.

    /* Create a shared memory object with that name... */
    int shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0600); // Permissions 0600 = only read & write for the owner.
    if(-1 == shm_fd) handle_error("Error at shm_open",1);

    /* ... and set its size to the size of the message buffer. */
    if(-1 == ftruncate(shm_fd, Messages_MaxSize)) handle_error("Error at ftruncate",2);

    /* Map the object into the producer's address space. */
    void *shm_ptr = mmap(NULL, Messages_MaxSize, PROT_WRITE | PROT_READ, MAP_SHARED, shm_fd, 0);
    // Basically, an entire page will be mapped, because Messages_MaxSize <= Page_Size !
    if(MAP_FAILED == shm_ptr) handle_error("Error at mmap",3);

    /* The file descriptor can now be closed. */
    if(-1 == close(shm_fd)) handle_error("Error at close",4);

    /* Write a message into the shared memory object. */
    sprintf(shm_ptr,"<begin>Hello world! From the producer process, with PID: %d.</end>", getpid());

    /* Optional, output on stderr a notification about the message written into the shared memory object. */
    fprintf(stderr,"[Producer, PID: %d] I have written the following text in the shared memory region:\n%s\n",
        getpid(), (char *)shm_ptr );

    /* Eventually, we will destroy the object, but only after the consumer gets to read it. */

    // sleep(2); // Some pause sufficient for the consumer to read the object, but only if they are run in parallel !
    // if(-1 == shm_unlink(shm_name)) handle_error("Error at shm_unlink",5);
    /* But to be sure, is better to delete the object from commandline, after running the consumer as many times we want. */

    return 0;
}
