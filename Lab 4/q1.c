// CS 332 - Operating Systems Lab //
// Lab 4 - Part 1 by Azwad Shameem //
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main() {
    pid_t pid;

    printf("Welcome to the simple command interpreter, Please enter your command.\n* ");
    char cmd[256];
    char* args[2];
    args[1] = NULL;



    while (1) {
        scanf("%s%[^\n]", cmd, args[0]);
        if (strcmp(cmd, "quit") == 0) {
            printf("Exiting the simple command interpreter\n");
            break;
        }

        // Fork child to execute the command given by the user
        pid = fork();
        if (pid == -1) {
            perror("Fork() Error"); // error checking for fork
            return -1;
        } else if (pid == 0) {
            // Child Process //
            execvp(cmd, args); // executes the user command
            printf("Command execution error\n"); // only prints if command doesn't execute
        } else {
            // Parent Process //
            wait(&pid);
            memset(cmd, 0, sizeof(cmd));
            args[0] = " ";
            printf("* ");
        }
    }
    return 0;
}
