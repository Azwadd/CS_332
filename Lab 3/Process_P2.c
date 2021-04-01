// CS 332 - Operating Systems Lab //
// Lab 3 - Part 3 - Step 2 by Azwad Shameem //
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

/* Copy the contents of source.txt into destination1.txt and destination2.txt as
per the following procedure until you reach the end of the file:
1. Read the next 50 characters from source.txt, and write to destination1.txt
2. Then the next 100 characters are read from source.txt and written into destination2.txt.*/
int main() {
	// First open all required files in correct state, otherwise report the error and return -1 //
	int fd_read = open("source.txt", O_RDONLY);
	if (fd_read == -1) {
		printf("Open() for source.txt failed with error [%s]\n", strerror(errno));
		return -1;
	}
	int fd_write1 = open("destination1.txt", O_WRONLY);
	if (fd_write1 == -1) {
		printf("Open() for destination1.txt failed with error [%s]\n", strerror(errno));
		return -1;
	}
	int fd_write2 = open("destination2.txt", O_WRONLY);
	if (fd_write2 == -1) {
		printf("Open() for destination2.txt failed with error [%s]\n", strerror(errno));
		return -1;
	}

	// All required files are now opened, now we can read 50 + 100 characters and write them apropriately //
	char *destination; // string buffer
	int reads; // int to store return value for read()
	int writes; // int to store return value for write()

	do {
		reads = read(fd_read, destination, 50);
		if (reads == -1) {
			printf("read(50 characters from source.txt) failed with error [%s]\n", strerror(errno)); // error reporting for read
		}
		writes = write(fd_write1, destination, reads);
		if (writes == -1) {
			printf("write(destination1.txt) failed with error [%s]\n", strerror(errno)); // error reporting for write
		}
		reads = read(fd_read, destination, 100);
		if (reads == -1) {
			printf("read(100 characters from source.txt) failed with error [%s]\n", strerror(errno)); // error reporting for read
		}
	    writes = write(fd_write2, destination, reads);
	    if (writes == -1) {
			printf("write(destination2.txt) failed with error [%s]\n", strerror(errno)); // error reporting for write
		}
	} while (reads > 0);
	
	close(fd_read);
	close(fd_write1);
	close(fd_write2);
    return 0;
}