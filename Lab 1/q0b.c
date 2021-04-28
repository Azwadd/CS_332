// CS 332 - Operating Systems Lab //
// Lab 1 - Part 0 - B by Azwad Shameem //
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

/* Question 0. Part (b) Write a C program where open system call creates a new file (say, destination.txt and then opens it.
 * (Hint: use the bitwise OR flag) */
int main (int argc, char* argv[]) {
	if (argc < 2) {
		printf("Improper usage. Use the format --> S'./create filename'\n");
		return -1;
	} else if (argc > 2) {
		printf("Too many arguments, please create one file at a time. Use format --> './access filepath'\n");
		return -1;
	}
	errno = 0;	
	printf("Creating file %s\n", argv[1]);
	int fd = open(argv[1], O_RDWR | O_CREAT, 0777);  // assume argv[1] is the destination file due to input requirements
	if(fd == -1) {      
		printf("Open() failed with error [%s]\n", strerror(errno));  // if fd is equal to -1 then an error happened and now we will report that error using strerror and errno
		return -1;													  // now since we have reported the error return -1 to show that the program has ended with an error since the file was not created
	}
	printf("File %s has been created\n", argv[1]);
	close(fd);	// no error has happened so we can close the file
	return 0;	// return 0 because program has created the file successfully
}