#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MAX_LEN 256

void read_replica(const char *filename, char *replica) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    fgets(replica, MAX_LEN, file);
    fclose(file);
}

void write_replica(const char *filename, const char *replica) {
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    fprintf(file, "%s", replica);
    fclose(file);
}

int main() {
    char parent_replica[MAX_LEN];
    char child_replica[MAX_LEN];
    int flag_fd;

    flag_fd = open("flag.bin", O_RDWR);
    if (flag_fd < 0) {
        perror("Error opening flag file");
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("Fork failed");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process
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

            while (lseek(flag_fd, 0, SEEK_SET), read(flag_fd, parent_replica, 1) == 0);

            read_replica("replici-fiu.txt", child_replica);
            printf("Fiu: %s", child_replica);
        }
    }

    close(flag_fd);
    return 0;
}
