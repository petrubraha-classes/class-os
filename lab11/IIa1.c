#include <stdlib.h>

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#define max_len 100

#define handle_eror(msg)\
    { perror(msg); exit(EXIT_FAILURE); }

void first_manipulation(char* text)
{   
    const char letters[] = "0123456789abcdefABCDEF";
    size_t i = 0;
    while(text[i])
    {
        if(strchr(letters, text[i]) == 0)
            strcpy(text + i, text + i + 1);
        else
        {
            if(text[i] > 'a' && text[i] < 'f')
                text[i] -= 32;
            i++;
        }
    }
}

int main()
{
    const char file_name[] = "input_data.txt";
    char text[max_len] = "\0";
    
    // open the file
    int fd_file = open(file_name, O_RDONLY);
    if(-1 == fd_file) handle_eror("open function failed");
    
    // determine the size of the file
    struct stat sb;
    if(-1 == fstat(fd_file, &sb)) handle_eror("fstat function failed");

    // read from the file in text
    if(-1 == read(fd_file, text, sb.st_size)) handle_eror("read function failed");

    // open anonymous pipe
    int parent_child1[2];
    if(-1 == pipe(parent_child1)) handle_eror("pipe function failed");

    // write to the channel 
    if(-1 == write(parent_child1[1], text, strlen(text)))
        handle_eror("write function failed");

    // creating child processes
    pid_t pid1 = -1, pid2 = -1;
    pid1 = fork();
    if(-1 == pid1) handle_eror("fork function failed");

    // inside first child process
    if(pid1 == 0)
    {
        // read from the first pipe and change
        char new_text[max_len] = "\0";
        read(parent_child1[0], new_text, strlen(text));
        first_manipulation(new_text);
        
        // create fifo
        // open to write fifo
        // send
        
        // create third process
        pid2 = fork();
        if(-1 == pid2) handle_eror("fork function failed");

        // inside second child process
        if(pid2 == 0)
        {
            // open to read fifo
            // create third pipe

            // create statistics.txt

            // write to pipe the number of distinct letters
        }

        // back to the first child process
    }

    // back to parent process
    close(parent_child1[0]);
    
    // read from third pipe and print

    // when should i close the pipes?
    close(parent_child1[1]);
    return 0;
}

// https://edu.info.uaic.ro/sisteme-de-operare/SO/labs/C/lab12.html#sec_2a