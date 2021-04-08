/* CS 332 - Operating Systems */
/* Lab 4 - Part 1 by Azwad Shameem */
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

/* Recall: In Task 3, we worked with exec() system calls for specific commands such as date, and ls.
Write a special simple command interpreter that takes command and its arguments. This interpreter
is a program where the main process creates a child process to execute the command using exec()
family functions. After executing the command, it asks for a new command input (i.e., parent wait for
child). The interpreter program will get terminated when the user enters quit. */
int main(int argc, char *argv[]) {
    while (1) {
        char cmd[1000];
        printf("*> ");
        scanf(" %[^\n]s", cmd);
        if (strcmp(cmd, "quit") == 0) {
            return 0;  // quit - close interpreter
        } else {
            // turn the user input into a array that execvp can read
            int size = 1;
            char *arg[size]; // dynamic string array
            char *token = strtok(cmd, " ");
            while (token != NULL) {
                arg[size - 1] = token;
                size++;
                token = strtok(NULL, " ");
            }
            arg[size - 1] = NULL; // last element should be set to null

            pid_t pid = fork();
            if (pid == -1) {
                perror("Fork() Error\n");
                printf("The current command will be skipped because the child process failed to be created.\n");
            } else if (pid == 0) {
                // Child Process //
                execvp(arg[0], arg); // attempts executes command
                printf("Command could not be executed\n"); // only prints if error occurs in execvp
                return 0;
            } else {
                wait(&pid);
                memset(arg, 0, size);
                memset(cmd, 0, 1000);
            }
        }
    }
}