
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
        char command[9999];
        printf("*> ");
        scanf(" %[^\n]s", command);
        if (!strcmp(command, "quit")) { return 0; } // quit - close interpreter

        // turn the user input into a array that execvp can read
        int size = 2;
        char* arg[size]; // dynamic string array
        char* token = strtok(command, " ");
        while (token != NULL) {
            arg[size - 2] = token;
            size++;
            token = strtok(NULL, " ");
        }
        arg[size - 2] = NULL;

        pid_t pid = fork();
        if (pid == -1) {
            perror("Fork() Error");
        } else if (pid == 0) {
            // Child Process //
            execvp(arg[0], arg); // attempts executes command
            printf("Command could not be executed\n"); // only prints if error occurs in execvp
        } else {
            wait(&pid);
        }
    }
}