#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 4096        /* Define the size of buffer. */
/*************************************************************************************************
 * Q: The file descriptors are only integer variables. How do they map to the corresponding file?
 * A: I guess, the file descriptor is the corresponding file's index. When open file "a", the
 *    value of the file descriptor is that file's index.
 *************************************************************************************************/
int main() {
    int src_fd, dst_fd;         /* source file's file descriptor and destination file's file descriptor */
    char buffer[BUFFER_SIZE];   /* A buffer of reading and writing files. */
    ssize_t num_read;           /* The number of bytes read. */

    /* Open the source file. The path of the source file can be changed. */
    src_fd = open("/Users/zarathustra/CLionProjects/copy_files/src", O_RDONLY); /* Set "read only" (Why doesn't the relative path work here?). */
    if (src_fd == -1) {
        perror("open src");     /* error */
        exit(EXIT_FAILURE);
    }

    /* Create the destination file. */
    dst_fd = open("/Users/zarathustra/CLionProjects/copy_files/dst", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (dst_fd == -1) {
        perror("open dst");
        exit(EXIT_FAILURE);
    }

    /* Read content from the source file and write into the destination file. */
    while ((num_read = read(src_fd, buffer, BUFFER_SIZE)) > 0) {    /* The syscall read() reads data from the file corresponding to file descriptor "src_fd" and stores it in buffer. */
        if (write(dst_fd, buffer, num_read) != num_read) {  /* Check. */
            perror("write");
            exit(EXIT_FAILURE);
        }
    }

    /* Check if it read successfully or not. */
    if (num_read == -1) {
        perror("read");
        exit(EXIT_FAILURE);
    }

    /* Close the fd. */
    if (close(src_fd) == -1) {
        perror("close src");
        exit(EXIT_FAILURE);
    }
    if(close(dst_fd) == -1) {
        perror("close dst");
        exit(EXIT_FAILURE);
    }

    printf("File copied successfully.\n");

    return 0;
}