// CS 332 - Operating Systems Lab //
// Lab 2 - Part 1 by Azwad Shameem //
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

/* Write a program q1.c, and let the parent process produce two child processes.*/
int main() {
    pid_t pid = fork();  // creates child 1 //

    if (pid < 0) { // error checking in case fork does not create a child //
        perror("Forking error child 1");
        return -1;
    } else if (pid == 0) {
        printf("I am first child, my pid is: %d\n", getpid());  // the child 1 process prints out the desired string and its pid //
    } else {
        wait(&pid); // waits for child 1 to terminate //
        if ((pid = fork()) < 0) { // create child 2  and error checking in case fork does not create a child //
            perror("Forking Error with child 2");
            return -1;
        } else if (pid == 0) {
            printf("I am first second, my pid is: %d\n", getpid()); // child 2 prints out the desired string and its pid //
        } else {
            wait(&pid); // waits for child 2 to terminate //
            printf("Parent Process now Terminating.\n"); // prints parent terminating satement //
        }
    }
    return 0;
}