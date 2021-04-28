// CS 332 - Operating Systems Lab //
// Lab 3 - Part 3 - Step 3 by Azwad Shameem //
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main() {
	pid_t pid = fork();
	if (pid == -1) {
		// error reporting //
		printf("\nChild 1 fork() failed with error [%s]\n",strerror(errno));
		return -1;
	} else if (pid == 0) {
		// Child 1 Process //
		execl("./p1", "./p1", NULL); // calls executable file of Process_P1.c
		printf("Child 1 execl() failed\n"); // error reporting for execl
		return 0;
	} else {
		// Parent Process //
		wait(&pid); // parent must wait for child 1 first
		if ((pid = fork()) == -1) {
			// error reporting //
			printf("\nfork() failed with error [%s]\n",strerror(errno));
			return -1;
		} else if (pid == 0) {
			// Child 2 Process //
			execl("./p2", "./p2", NULL); // calls executable file of Process_P2.c
			printf("Child 2 execl() failed\n"); // error report for execl
			return 0;
		} else {
			wait(&pid); // parent must wait for child 2
		}
	}

    return 0;
}