#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHM_SIZE 4096

int main() {
    pid_t pid;
    int shmid;
    key_t key = ftok(".", 'x');

    /* create a shared memory segment */
    if ((shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT)) < 0) {
        perror("shmget");
        exit(1);
    }

    /* fork a child process */
    if ((pid = fork()) < 0) {
        perror("fork");
    }

    /* parent process writes to the shared memory segment */
    if (pid > 0) {
        char *shmaddr;
        if ((shmaddr = (shmat(shmid, NULL, 0))) == (char *) -1) {
            perror("shmat");
            exit(1);
        }

        /* write to the shared memory segment */
        sprintf(shmaddr, "Hello, shared memory!");

        /* print the process ID */
        printf("This message is written by process %d\n", getpid());

        /* wait for the child process to exit */
        wait(NULL);

        /* detach from the shared memory segment */
        shmdt(shmaddr);
    }
    else {
        char *shmaddr;
        if ((shmaddr = (shmat(shmid, NULL, 0))) == (char *) -1) {
            perror("shmat");
        }

        /* read from the shared memory segment */
        printf("Message received from parent process: %s\n", shmaddr);

        /* detach from the shared memory segment */
        shmdt(shmaddr);
    }
    /* remove the shared memory segment */
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}
