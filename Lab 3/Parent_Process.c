// CS 332 - Operating Systems Lab //
// Lab 3 - Part 3 - Step 3 by Azwad Shameem //
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main() {
	pid_t pid1, pid2;
	pid1 = fork();
	if (pid1 == -1) { 
		// error reporting //
		printf("\nChild 1 fork() failed with error [%s]\n",strerror(errno));
		return -1;
	} else if (pid1 == 0) {
		// Child 1 Process //
		execl("./p1", "./p1", NULL); // calls executable file of Process_P1.c
		printf("Child 1 execl() failed\n"); // error reporting for execl
	} else {
		// Parent Process //
		wait(&pid1); // parent must wait for child 1 first
		pid2 = fork(); // child 2 created

		if (pid2 == -1) { 
			// error reporting //
			printf("\nfork() failed with error [%s]\n",strerror(errno));
			return -1;
		} else if (pid2 == 0) { 
			// Child 2 Process //
			execl("./p2", "./p2", NULL); // calls executable file of Process_P2.c
			printf("Child 2 execl() failed\n"); // error report for execl
		} else {
			wait(&pid2); // parent must wait for child 2
		}
	}

    return 0;
}