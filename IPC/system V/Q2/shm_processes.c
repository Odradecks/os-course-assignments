#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>

#define SHM_SIZE 1024
#define NUM_CHILDREN 3

int main() {
    int shmid;
    key_t key = ftok(".", 'x'); 

    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(1);
    }

    for (int i = 0; i < NUM_CHILDREN; i++) {
        pid_t pid = fork();
        if (pid < 0) {
            perror("fork");
            exit(1);
        } else if (pid == 0) {
            char *shmaddr;
            if ((shmaddr = shmat(shmid, NULL, 0)) == (char *) -1) {
                perror("shmat");
                exit(1);
            }

            printf("Message received from parent process in child %d: %s\n", getpid(), shmaddr);

            shmdt(shmaddr);

            exit(0);
        }
    }

    char *shmaddr;
    if ((shmaddr = shmat(shmid, NULL, 0)) == (char *) -1) {
        perror("shmat");
        exit(1);
    }
    sprintf(shmaddr, "Hello, shared memory!");

    printf("This message is written by process %d.\n", getpid());

    for (int i = 0; i < NUM_CHILDREN; i++) {
        wait(NULL);
    }

    shmdt(shmaddr);

    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}

