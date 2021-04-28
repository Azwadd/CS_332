// CS 332 - Operating Systems Lab //
// Lab 1 - Part 2 by Azwad Shameem //
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
/* Question 2. The cp command copies the source file specified by the SourceFile parameter to the destination file specified
 * by the DestinationFile parameter. Write a C program that mimics the cp command using open() systemcall to open source.txt
 * file in read-only mode and copy the contents of it to destination.txt using read() and write() system calls.*/
int main (int argc, char* argv[]) {
	if (argc < 3) { 	
		printf("Improper usage. Please use format --> './a.out SourceFile DestinationFile'\n");      
		return -1;
	} else if (argc > 3) {
		printf("Too many arguments. Please use format --> './a.out SourceFile DestinationFile'\n");  
		return -1;
	}

	errno = 0;   
	int fd = open(argv[1], O_RDONLY); // argv[1] is assumed as the source file
	if(fd == -1) {	// Checks if opening the source file in read only mode has an error if it does utilize strerror(errno) to report
		printf("\nOpen() for source file has failed with error [%s]\n", strerror(errno));      
		return -1;
	} 
	int fd2 = open(argv[2], O_WRONLY); // argv[2] is assumed as destination file
	if(fd2 == -1) {	// Checks if opening the destination file in write only mode has an error if it does utilize strerror(errno) to report
		printf("Open() for destination file has failed with error [%s]\n", strerror(errno));      
		return -1;
	}

	// The file has opened properly so now we must copy the source file's contents into the destination file
	printf("Open() Successful\n\n");
	printf("Now copying %s to %s\n\n", argv[1], argv[2]);  
	char* output;			// buffer to put the content of the file
	int c_read, c_write;	// counter for read() and counter for write()
	while ((c_read = read(fd, output, 1024)) > 0) {
		if (c_read == -1) { perror("read"); return -1; }	// this makes sure that if read has an error, it will report the erorr 
		c_write = write(fd2, output, c_read);
		if (c_write == -1) { perror("write"); return -1; }	// this makes sure that if write has an error, it will report the error
	}
	close(fd);
	close(fd2);
	printf("Copying completed\n");
	return 0;
}