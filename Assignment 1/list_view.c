/* CS 332 - Operating Systems */
/* Assignment 1 by Azwad Shameem */
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

/* Your task is to create a system with one parent process and two child processes where
the children communicate using a pipe. In the below example the output from the ls -F command is piped to input of
the nl command. Use fork(), one form of exec() functions, so that the first child will perform ls -F and
pass the output to the second child using one direction pipe, so the second one can
perform nl on the list of current directory contents. Later the second child process will
print to the screen the result (see example below). The parent process must wait for its
both children.*/
int main() {
	pid_t pid, pids; 
	int p[2];   
	pipe(p); // pipe for communication //

	pid = fork();	// Creates child 1
	if (pid < 0) {
		perror("\nForking error with child 1");	// Error reporting if child 1 cannot be created //
		return -1;
	} else if (pid == 0) {
		// Child 1 Process //
		int e = dup2(p[1], 1);	// -1, which is stdout, now points to p[1] - meaning the stdout is now being put into the pipe for communication //
		if (e == -1) {	// dup2 equaling 1 means an error has happened so we print the error //
			perror("\ndup2() had an error in child 1");
			return -1;
		}
		e = execlp("ls", "bin/ls", "-F", NULL); // executes ls - F //
		if (e == -1) {	// execlp equaling -1 means an error has happened so we print the error //
			perror("\nexeclp() had an error in child 1");
			return -1;
		}	
	} else {
        // Parent process //
        wait(&pid); // parent process must wait for child 1 first before creating child 2

        // Create child 2 and give it a task //
		pids = fork();
		if (pids < 0) {
			perror("\nForking error with child 2");	// Error reporting if child 2 cannot be created //
			return -1;
		} else if (pids == 0) {
			// Child 2 Process //
			int e = close(p[1]);	// Since p[1] data has finished sending, we can close it now //
			if (e == -1) { 
				perror("\nclose() error with the pipe from child 2");
				return -1;
			}
			e = dup2(p[0], 0);	// 0, which is stdin, now points to p[0] - means the pipe communicates that the stdout from child 1 is to be stdin for child 2 //
			if (e == -1) {	// dup2 equaling -1 means an error has happened so we print the error //
				perror("\ndup2() had an error in child 2");
				return -1;
			}
			e = execlp("nl", "nl", NULL); // executes nl //
			if (e == -1) {	// excelp equaling -1 means an error has happened so we print the error //
				perror("\nexeclp() had an error in child 2");
				return -1;
			}		
		} else {	
			// Parent Process //
			int e = close(p[1]); // closes the pipe // 
			if (e == -1) { 
				perror("\nclose() error with the pipe[1] from parent");
				return -1;
			}
			e = close(p[0]); // closes the pipe //
			if (e == -1) { 
				perror("\nclose() error with the pipe[2] from parent");
				return -1;
			}

			// Parent waits for child 2 by waiting for two null terminations //
			wait(&pids);
		}	
	}
	return 0;
}