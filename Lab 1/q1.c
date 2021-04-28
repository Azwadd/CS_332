// CS 332 - Operating Systems Lab //
// Lab 1 - Part 1 by Azwad Shameem //
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

/* Question 1. Write a C program to implement a command called printcontent
 * that takes a file(text) as an argument and displays its contents.Report an appropriate message if the file does not
 * exist or can’t be opened(i.e.the file doesn't have read permission).You are to use open(), read(),write() and close() system calls.*/
int main (int argc, char* argv[]) {
	// This makes sure that user is utilizing the command in the right way
	if(argc < 2) {
		printf("Improper Usage. Please use this format --> './a.out filename'\n");      
		return -1;
	} else if (argc > 2) {
		printf("Too many arguments, please only use one file at a time with the format --> './a.out filename'\n");      
		return -1;
	}

	errno = 0;   
	int fd = open(argv[1], O_RDONLY);  
	if(fd == -1) {
		printf("Open() failed with error [%s]\n",strerror(errno));      
		return -1;
	} else {
		/* open() succeeded, now one can do read operations on the     
		file opened since we opened it in read-only mode. Also once done with processing, the file needs to be closed.*/	
		printf("Open() Successful\n\n");  
		char* output;		// buffer to put the content of the file
		int c_read;			// counter for read()	
		int c_write;		// counter for write()

		while ((c_read = read(fd, output, 1024)) > 0) {
			if (c_read == -1) { perror("read"); return -1; }	// this makes sure that if read has an error, it will report the erorr 
			c_write = write(1, output, c_read);
			if (c_write == -1) { perror("write"); return -1; }	// this makes sure that if write has an error, it will report the error
		}
		printf("\n");
		close(fd);	// since we are done writing close the file 
	}
	return 0;		// no issues so returned with 0 to show everything was a success
}