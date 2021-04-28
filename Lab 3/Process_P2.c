// CS 332 - Operating Systems Lab //
// Lab 3 - Part 3 - Step 2 by Azwad Shameem //
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

/* Copy the contents of source.txt into destination1.txt and destination2.txt as
 * per the following procedure until you reach the end of the file:
 * 1. Read the next 50 characters from source.txt, and write to destination1.txt
 * 2. Then the next 100 characters are read from source.txt and written into destination2.txt. */
int main() {
	// First open all required files with the required mode
	int fd_source = open("source.txt", O_RDONLY);
	if (fd_source == -1) {
		printf("Open() for source.txt failed with error [%s]\n", strerror(errno));
		return -1;
	}
	int fd_dest1 = open("destination1.txt", O_WRONLY);
	if (fd_dest1 == -1) {
		printf("Open() for destination1.txt failed with error [%s]\n", strerror(errno));
		return -1;
	}
	int fd_dest2 = open("destination2.txt", O_WRONLY);
	if (fd_dest2 == -1) {
		printf("Open() for destination2.txt failed with error [%s]\n", strerror(errno));
		return -1;
	}

	// All required files are now opened, now we can read 50 + 100 characters and write them appropriately //
	char buffer[150]; // string buffer
	int readCount; // int to store return value for read()

	do {
        if ((readCount = read(fd_source, buffer, 50)) == -1) {
            printf("Read() from source.txt failed with error [%s]\n", strerror(errno));
            return -1;
        } else if (write(fd_dest1, buffer, readCount) == -1) {
            printf("Write() to destination1.txt failed with error [%s]\n", strerror(errno));
            return -1;
        } else if ((readCount = read(fd_source, buffer, 100)) == -1) {
            printf("Read() from source.txt failed with error [%s]\n", strerror(errno));
            return -1;
        } else if (write(fd_dest2, buffer, readCount) == -1) {
            printf("Write() to destination2.txt failed with error [%s]\n", strerror(errno));
            return -1;
        }
    } while (readCount > 0);

	if (close(fd_source) == -1 || close(fd_dest1) == -1 || close(fd_dest2) == -1) {
        printf("Close() for source.txt and/or destination1.txt and/or destination2.txt with error [%s]\n", strerror(errno));
        return -1;
	}
    return 0;
}