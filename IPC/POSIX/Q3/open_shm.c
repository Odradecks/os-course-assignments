#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>

int main() {
    int fd;
    char *mapped_file;
    struct stat file_stat;

    /* open file */
    fd = open("example.txt", O_RDONLY);
    if (fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }

    /* get file information */
    if (fstat(fd, &file_stat) == -1) {
        perror("fstat");
        exit(EXIT_FAILURE);
    }

    /* map file into memory */
    mapped_file = mmap(NULL, file_stat.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
    if (mapped_file == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    /* read file content from memory */
    printf("File content:\n%s\n", mapped_file);

    /* unmap memory */
    if (munmap(mapped_file, file_stat.st_size) == -1) {
        perror("munmap");
        exit(EXIT_FAILURE);
    }

    /* close file */
    if (close(fd) == -1) {
        perror("close");
        exit(EXIT_FAILURE);
    }

    return 0;
}

