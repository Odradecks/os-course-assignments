#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int num = 0;

int main() {
    pid_t pid, pid_got;
    pid = fork();
    if (pid < 0) {
        printf("fork failed!\n");
        exit(-1);
    } else if (pid == 0) {
        // 子进程
        pid_got = getppid(); // 获取父进程的进程ID
        printf("Child (PID: %d) waiting for parent (PID: %d) to complete...\n", getpid(), pid_got);
        waitpid(pid_got, NULL, 0); // 等待指定进程ID（父进程）结束
        num += 1;
        printf("Child (PID: %d) Begin: num = %d\n", getpid(), num);
    } else {
        // 父进程
        printf("Parent (PID: %d) Begin\n", getpid());
        num += 2;
        printf("Parent (PID: %d) Complete: num = %d\n", getpid(), num);
    }
    return 0;
}

