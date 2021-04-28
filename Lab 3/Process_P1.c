// CS 332 - Operating Systems Lab //
// Lab 3 - Part 3 - Step 1 by Azwad Shameem //
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

/* Create two files namely, destination1.txt and destination2.txt with read, write and execute permissions. */
int main() {
	int fd1, fd2;
	if ((fd1 = open("destination1.txt", O_RDWR | O_CREAT, 0777)) == -1) {
		printf("\nopen(destination1.txt) failed with error [%s]\n",strerror(errno)); // error checking for open
	}
	if ((fd2 = open("destination2.txt", O_RDWR | O_CREAT, 0777)) == -1) {
		printf("\nopen(destination2.txt) failed with error [%s]\n",strerror(errno)); // error checking for open
	}

	if (close(fd1) == -1 || close(fd2) == -1) {
		printf("\nClose() destination1.txt and/or destination 2.txt failed with error [%s]\n",strerror(errno));
	}

    return 0;
}