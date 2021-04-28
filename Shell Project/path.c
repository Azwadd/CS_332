/* CS 332 - Operating Systems
 * Shell - Path by Azwad Shameem */
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

/* The path command
 Task 1: This new command will get the path of the current directory
 Task 2: Then print the output of task 1 to the terminal and t2.txt and change the name of text file to path.txt
 Task 3: Concatenate the content of tree.txt and path.txt into t3.txt and change the last to log.txt.
 Task 4: Finally, delete tree.txt and path.txt.*/
int main() {
    // Initialize the pipe
    int pipefd[2];
    pipe(pipefd);

    pid_t pid = fork();
    if (pid == -1) { // Error checking
        printf("Fork() failed with error [%s]\n", strerror(errno));
        return -1;
    } else if (pid == 0) {
        // Child Process // - Task 1 get the current working directory
        close(pipefd[0]);                // Close the other side of the pipe so we don't lose data
        dup2(pipefd[1], STDOUT_FILENO);  // Send STDOUT to the pipe
        dup2(pipefd[1], STDERR_FILENO);  // Send STDERR to the pipe
        //close(pipefd[1]);
        execl("/bin/pwd", "pwd", NULL);
        printf("Pwd() failed with error [%s]\n", strerror(errno));
    } else {
        wait(&pid);
        // Parent Process //
        close(pipefd[1]); // close the write end of the pipe in the parent
        // Task 1 complete

        // Task 2 - Now we want to read the output and write it to terminal and file //
        int fd = open(t2, O_WRONLY); // open file in write mode so we can write to it
        if (fd == -1) { // Error checking
            printf("Open() for t2.txt failed with error [%s]\n", strerror(errno));
            return -1;
        } else {

            // Initialize variables needed to read from pipe and write to terminal and file
            char buffer[1024]; // Buffer to save content
            int fd_read;       // Integer to check if we need to keep reading the content in the pipe
            do {
                if ((fd_read = read(pipefd[0], buffer, 1024)) == -1) { // reads from pipe
                    printf("Read() for pipe failed with error [%s]\n", strerror(errno));
                    return -1; // If fail to read from pipe return -1
                }
                if (write(STDOUT_FILENO, buffer, fd_read) == -1) { // write to terminal
                    printf("Write() to terminal failed with error [%s]\n", strerror(errno));
                    return -1; // If fail to write to terminal return -1
                }
                if (write(fd, buffer, fd_read) == -1) { // write to file
                    printf("Write() to t2.txt failed with error [%s]\n", strerror(errno));
                    return -1; // If fail to write to file return -1
                }
            } while (fd_read > 0);

            if (close(fd) == -1) { // Close file since we are done
                printf("Close() for t2.txt failed with error [%s]\n", strerror(errno));
                return -1; // If fail to close return -1
            }
            if (rename(t2, path) != 0) { // rename t2.txt to path.txt
                printf("Error: unable to rename the file");
                return -1; // return -1 if can't rename
            } // Task 2 complete

            // Task 3 - Concatenate tree.txt and path.txt into t3.txt and rename t3.txt to log.txt
            int fd_tree = open(tree, O_RDONLY); // Opening tree.txt in read only mode
            int fd_path = open(path, O_RDONLY); // Opening path.txt in read only mode
            int fd_t3 = open(t3, O_WRONLY);     // Opening t3.txt in write only mode
            if (fd_tree == -1 || fd_path == -1 || fd_t3 == -1) { // Error checking for all the files
                printf("Open() for tree.txt and/or path.txt and/or t3.txt failed with error [%s]\n", strerror(errno));
                rename(path, t2); // Rename path.txt to t2.txt since operation failed
                return -1;
            } else {

                do {
                    if ((fd_read = read(fd_tree, buffer, 1024)) == -1) { // Reads 1024 characters and stores it into buffer
                        printf("Read() for tree.txt failed with error [%s]\n", strerror(errno));
                        return -1; // If fail to read tree.txt return -1
                    }
                    if (write(fd_t3, buffer, fd_read) == -1) { // Writes the amount of characters read into t3.txt
                        printf("Write() to t3.txt failed with error [%s]\n", strerror(errno));
                        return -1; // If fail to write return -1
                    }
                } while (fd_read > 0);

                do {
                    if ((fd_read = read(fd_path, buffer, 1024)) == -1) {
                        printf("Read() for path.txt failed with error [%s]\n", strerror(errno));
                        return -1;
                    }
                    if (write(fd_t3, buffer, fd_read) == -1) {
                        printf("Write() to t3.txt failed with error [%s]\n", strerror(errno));
                        return -1;
                    }
                } while (fd_read > 0);

                if (close(fd_tree) == -1 || close(fd_path) == -1 || close(fd_t3) == -1) {
                    printf("Close() for tree.txt and/or path.txt and/or t3.txt failed with error [%s]\n",
                           strerror(errno));
                }

                if (rename(t3, logs) != 0) { // rename t3.txt to log.txt
                    printf("Error: unable to rename the file");
                    return -1; // If fail to rename return -1
                } // Task 3 Complete

                // Task 4 - delete tree.txt and path.txt
                if (unlink(tree) == -1 || unlink(path) == -1) {
                    printf("Unlink() for tree.txt and/or path.txt failed with error [%s]\n", strerror(errno));
                    return -1; // If fail to delete return -1
                } // Task 4 complete
            }
        }
    }
    return 0;
}