#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
    FILE* input_child = fopen("replici-fiu.txt", "r");
    FILE* input_parent = fopen("replici-parinte.txt", "r");
    if(input_child == NULL || input_parent == NULL)
        exit(1);

    char* text_child, * text_parent;
    
    int flag_fd;
    flag_fd = open("flag.bin", O_RDONLY);
    if (flag_fd < 0){
        perror("error opening flag file");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } 
    
    // process 1 parent
    // process 2 child
    
    //while(true) // no more text
    

    /*else if (pid == 0) {
        while (1) {
            // Wait for parent to write its replica
            while (lseek(flag_fd, 0, SEEK_SET), read(flag_fd, child_replica, 1) == 0)
                ;

            // Read parent's replica
            read_replica("replici-parinte.txt", parent_replica);
            printf("Parinte: %s", parent_replica);

            // Write child's replica
            printf("Fiu: ");
            fgets(child_replica, MAX_LEN, stdin);
            write_replica("replici-fiu.txt", child_replica);

            // Set flag to signal parent
            lseek(flag_fd, 0, SEEK_SET);
            write(flag_fd, "1", 1);
        }
    } else {
        // Parent process
        while (1) {
            // Write parent's replica
            printf("Parinte: ");
            fgets(parent_replica, MAX_LEN, stdin);
            write_replica("replici-parinte.txt", parent_replica);

            // Set flag to signal child
            lseek(flag_fd, 0, SEEK_SET);
            write(flag_fd, "1", 1);

            // Wait for child to write its replica
            while (lseek(flag_fd, 0, SEEK_SET), read(flag_fd, parent_replica, 1) == 0)
                ;

            // Read child's replica
            read_replica("replici-fiu.txt", child_replica);
            printf("Fiu: %s", child_replica);
        }
    }


*/
    close(flag_fd);
    return 0;
}
