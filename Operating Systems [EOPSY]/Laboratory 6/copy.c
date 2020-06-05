#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>

#define BUFFER 1024 //buffer size

void copy_read_write(int fd_from,   int fd_to) {
    char buffer[BUFFER];
    int in;
    size_t sizeOfReadFile;
    while((in = read(fd_from, &buffer, BUFFER)) > 0){

        if(write(fd_to, &buffer, BUFFER) < 0){
            printf("CRITICAL ERROR WHILE WRITING TO FILE!");
            return;
        }
        printf("Copied %d BYTES.", BUFFER);
    }
    printf("File copied. \n");
    close(fd_from);
    close(fd_to);
    return;
}
void copy_mmap(int fd_from, int fd_to) {
    char *src; //source  pointer
    char *dest;  //destination  pointer
    size_t filesize; //size of the source file
    filesize = lseek(fd_from, 0, SEEK_END); //repoisitoning the file offset of the open file description associated with the file descirptor (0). Offset is set to size of the file plus offset bytes.

    src = mmap(NULL, filesize, PROT_READ, MAP_PRIVATE, fd_from, 0); //create a mapping in virtual memory
    //PROT_READ Pages may be read
    //MAP_PRIVATE Create a private copy-on write mapping

    ftruncate(fd_to, filesize); //truncate file to filesize
    dest = mmap(NULL, filesize, PROT_READ | PROT_WRITE, MAP_SHARED, fd_to, 0);
    memcpy(dest, src, filesize);
    munmap(src, filesize);
    munmap(dest, filesize);

    close(fd_from);
    close(fd_to);

}


int main(int argc, char **argv){

    char inputChar; //argument char (m/h)
    int sourceFile;
    int destinationFile;
    bool isMapModeOn = false;
    while((inputChar = getopt(argc, argv, "mh")) != -1){ //-1 while no more arguments
        switch(inputChar){
        case 'm':
            printf("MMAP is used. \n");
            isMapModeOn = true;
            break;
        case 'h':
            printf("Hey, it seems like you are in trouble. Please follow the instruction: \n Pattern: copy [-m] <file_name> <new_file_name>   copy [-h] \n m - MMAP mode \n Without -m flag file is copied. \n");
            return 0;
            break;
        default:
            printf("shiet \n");
            return 0;
            break;

        }
    }

    if((sourceFile = open(argv[optind], O_RDONLY)) < 0) //open source file. Read only access. Negative value alert an error.
    {
        printf("OPEN SOURCE FILE FILED. GOOD BYE \n");
        return 1;
    }
    if((destinationFile = open(argv[optind + 1], O_RDWR | O_TRUNC | O_CREAT, 0666)) < 0) //open destination file. RW mode
    {
        printf("OPEN DESTINATION FILE FAILED. GOOD BYE \n");
        return 1;
    }

    if(isMapModeOn){
        copy_mmap(sourceFile, destinationFile);
    }else {
        copy_read_write(sourceFile, destinationFile);
    }

    printf("See you soon. \n");
    return 0;
}
