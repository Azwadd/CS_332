#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
const char* t2 = "t2.txt";
const char* t3 = "t3.txt";
const char* path = "path.txt";
const char* tree = "tree.txt";
const char* logs = "log.txt";

int main() {
    int pipefd[2];
    pipe(pipefd);
    pid_t pid = fork();
    if (pid == -1) {
        perror("Error with fork()");
        return -1;
    } else if (pid == 0) {
        close(pipefd[0]);    // close reading end in the child
        dup2(pipefd[1], 1);  // send stdout to the pipe
        dup2(pipefd[1], 2);  // send stderr to the pipe
        close(pipefd[1]);    // this descriptor is no longer needed
        execl("/bin/pwd", "pwd", NULL);
        printf("Error with pwd");
    } else {
        wait(&pid);
        close(pipefd[1]); // close the write end of the pipe in the parent
        int fd = open(t2, O_RDWR);
        if(fd == -1) {
            printf("Open() failed with error [%s]\n", strerror(errno));
            return -1;
        }
        char buffer[1024];
        int fd_read = 1, fd_write, fd_write2;
        while (fd_read > 0) {
            if ((fd_read = read(pipefd[0], buffer, 1024)) == -1) {
                printf("Read() failed with error [%s]\n", strerror(errno));
                return -1;
            }
            if ((fd_write = write(STDOUT_FILENO, buffer, fd_read)) == -1) {
                printf("Write() failed with error [%s]\n", strerror(errno));
                return -1;
            }
            if ((fd_write2 = write(fd, buffer, fd_read)) == -1) {
                printf("Write() failed with error [%s]\n", strerror(errno));
                return -1;
            }
        }
        if (close(fd) == -1) {
            printf("Close() failed with error [%s]\n", strerror(errno));
            return -1;
        }
        if (rename(t2, path) != 0) {
            printf("Error: unable to rename the file");
            return -1;
        }

        int fd_tree = open(tree, O_RDONLY);
        int fd_path = open(path, O_RDONLY);
        int fd_t3 = open(t3, O_RDWR);
        if (fd_tree == -1 || fd_path == -1 || fd_t3 == -1) {
            printf("Open() failed with error [%s]\n", strerror(errno));
            unlink(path);
            return -1;
        }

        fd_read = 1;
        while (fd_read > 0) {
            if ((fd_read = read(fd_tree, buffer, 1024)) == -1) {
                printf("Read() failed with error [%s]\n", strerror(errno));
                return -1;
            }
            if ((fd_write = write(fd_t3, buffer, fd_read)) == -1) {
                printf("Write() failed with error [%s]\n", strerror(errno));
                return -1;
            }
        }

        fd_read = 1;
        while (fd_read > 0) {
            if ((fd_read = read(fd_path, buffer, 1024)) == -1) {
                printf("Read() failed with error [%s]\n", strerror(errno));
                return -1;
            }
            if ((fd_write = write(fd_t3, buffer, fd_read)) == -1) {
                printf("Write() failed with error [%s]\n", strerror(errno));
                return -1;
            }
        }
        close(fd_tree);
        close(fd_path);
        close(fd_t3);

        if (rename(t3, logs) != 0) {
            printf("Error: unable to rename the file");
            return -1;
        }

        unlink(tree);
        unlink(path);
    }
    return 0;
}