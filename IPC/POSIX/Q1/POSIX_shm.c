#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <fcntl.h>
#include <sys/mman.h>


#define SHM_SIZE 4096
#define SHM_NAME "/posix_shm"

int main() {
    pid_t pid;
    int shm_fd;
    char *shm_ptr;

    /* create shared memory object */
    shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        exit(EXIT_FAILURE);
    }

    /* ftruncate operation: set size of shared memory object */
    if (ftruncate(shm_fd, SHM_SIZE) == -1) {
        perror("ftruncate");
        exit(EXIT_FAILURE);
    }

    /* map shared memory object */
    shm_ptr = mmap(NULL, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm_ptr == MAP_FAILED) {
        perror("mmap");
        exit(EXIT_FAILURE);
    }

    /* fork a child process */
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {
        /* child process as producer */
        printf("Child process (producer) writing data to shared memory...\n");
        sprintf(shm_ptr, "Hello, shared memory from producer!");
        printf("Data writen by producer: %s\n", shm_ptr);

        /* unmap shared memory object */
        if (munmap(shm_ptr, SHM_SIZE) == -1) {
            perror("munmap");
            exit(EXIT_FAILURE);
        }
        close(shm_fd);

        exit(EXIT_SUCCESS);
    }
    else {
        /* parent process as consumer */
        wait(NULL);
        printf("Parent process (consumer) reading data from shared memory...\n");
        printf("Data read by consumer: %s\n", shm_ptr);

        /* unmap shared memory object */
        if (munmap(shm_ptr, SHM_SIZE) == -1) {
            perror("munmap");
            exit(EXIT_FAILURE);
        }
        close(shm_fd);

        /* delete shared memory object */
        if (shm_unlink(SHM_NAME) == -1) {
            perror("shm_unlink");
            exit(EXIT_FAILURE);
        }

        exit(EXIT_SUCCESS);
    }
    return 0;
}
