#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

int main() {
    pid_t pid;
    int status;

    // 1. 创建子进程并输出当前路径
    pid = fork();
    if (pid < 0) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) { // 子进程
        char cwd[1024];
        if (getcwd(cwd, sizeof(cwd)) != NULL) {
            printf("子进程所在路径: %s\n", cwd);
        } else {
            perror("getcwd() error");
            exit(EXIT_FAILURE);
        }
        exit(EXIT_SUCCESS);
    } else { // 父进程
        // 等待子进程完成输出当前路径的操作
        waitpid(pid, &status, 0);

        // 在当前目录下新建名为os的文件夹
        if (mkdir("os", 0777) == -1) {
            perror("mkdir");
            exit(EXIT_FAILURE);
        }
	printf("创建成功。\n");
	printf("子进程PID：%d\n", pid);
	printf("父进程PID：%d\n", getpid());
        // 将本程序源码文件拷贝到新建的os文件夹
        char src_file[] = "func_practice.c";
        char dest_file[] = "os/func_practice.c";
        int fd_src, fd_dest;
        ssize_t bytes_read;
        char buffer[4096];

        fd_src = open(src_file, O_RDONLY);
        if (fd_src == -1) {
            perror("open source file");
            exit(EXIT_FAILURE);
        }

        fd_dest = open(dest_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
        if (fd_dest == -1) {
            perror("open destination file");
            exit(EXIT_FAILURE);
        }

        while ((bytes_read = read(fd_src, buffer, sizeof(buffer))) > 0) {
            if (write(fd_dest, buffer, bytes_read) != bytes_read) {
                perror("write");
                exit(EXIT_FAILURE);
            }
        }

        if (bytes_read == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        if (close(fd_src) == -1 || close(fd_dest) == -1) {
            perror("close");
            exit(EXIT_FAILURE);
        }
    }

    return 0;
}

