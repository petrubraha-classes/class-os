#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// problems: last character, 

// gcc -Wall first_set/Octal2Binary_mmap.c -o first_set/first_problem
// ./first_set/first_problem first_set/first_file.txt

// one / two files given as parameters
// copy from one to another with specification

// in case of two files we will user 2 maps

char* result;

#define handle_error(message)\
    { perror(message); exit(EXIT_FAILURE); free(result); }
#define MAX 300


char* conversion(char ch)
{
    for(size_t i = 0; i < MAX; i++) result[i] = '\0';

    int number = ch - '0';
    while(number)
    {
        int remainder = number % 2;
        number /= 2;
        
        size_t n_result = strlen(result);
        if(n_result == MAX) handle_error("error at converting the numbers");
        for(size_t i = n_result; i; i--)
            result[i] = result[i - 1];

        result[0] = remainder + '0';
    }

    return result;
}

size_t processing(char* text, char* source)
{
    const char numbers[] = "01234567";
    size_t n_source = strlen(source), index_source = 0, index_text = 0;
    while(index_source < n_source)
    {
        if(strchr(numbers, source[index_source]))
        {
            char* temp = conversion(source[index_source]);
            strcat(text, temp);
            printf("text: %s and temp: %s\n", text, temp);
            index_text += strlen(temp);
        }

        else text[index_text++] = source[index_source];
        index_source++;
    }

    text[index_text] = '\0';
    return strlen(text);
}

int main(int argc, char** argv)
{
    // there can be one / two argument(s)
    if(argc < 2 || argc > 3)
        return EXIT_FAILURE;
    result = malloc(MAX);

    // open the file
    char* name_first_file = argv[1];
    int fd_first_file = open(name_first_file, O_RDWR);
    if(-1 == fd_first_file) handle_error("error at opening the first file");

    // determine the size of the file from which we want to read
    struct stat sd;
    if(-1 == fstat(fd_first_file, &sd)) handle_error("error at stat");
    size_t size_first_file = sd.st_size;

    // open the map && close fd
    char* first_map = mmap(NULL, size_first_file, PROT_READ | PROT_WRITE, MAP_SHARED, fd_first_file, 0);
    if(first_map == MAP_FAILED) handle_error("error at map creation");
    if(-1 == close(fd_first_file)) handle_error("error at closing the first file");

    // create buffer for the converted text
    char buffer[MAX]; strcpy(buffer, "\0");
    size_t size_buffer = processing(buffer, first_map);

    // one file
    if(argc == 2)
    {
        strcpy(first_map, buffer);
        if(-1 == msync(first_map, size_buffer ,MS_SYNC)) handle_error("error at changing the contents"); 
        if(-1 == munmap(first_map, size_first_file)) handle_error("error at closing the first map");
        return EXIT_SUCCESS;
    }

    // two files
    // close the first map, all the information that we need exists in buffer
    if(-1 == munmap(first_map, size_first_file)) handle_error("error at closing the first map");

    // open second file for writing
    char* name_second_file = argv[3];
    int fd_second_file = open(name_second_file, O_CREAT | O_WRONLY);
    if(-1 == fd_first_file) handle_error("error at opening the second file");
    
    // create the second map && close fd
    char* second_map = mmap(NULL, size_buffer, PROT_WRITE, MAP_SHARED, fd_second_file, 0);
    if(second_map == MAP_FAILED) handle_error("error at map creation");
    if(-1 == close(fd_second_file)) handle_error("error at closing the second file");
    
    // change content && close the second map
    strcpy(second_map, buffer);
    if(-1 == msync(second_map, size_buffer, MS_SYNC)) handle_error("error at changing the contents");
    if(-1 == munmap(second_map, size_buffer)) handle_error("error at closing the second map");
    return 0;
}
