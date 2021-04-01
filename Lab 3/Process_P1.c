// CS 332 - Operating Systems Lab //
// Lab 3 - Part 3 - Step 1 by Azwad Shameem //
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>

/*Create two files namely, ​destination1.txt ​and ​destination2.txt ​with read, write and execute permissions.*/
int main() {
	int fd1 = open("destination1.txt", O_RDWR | O_CREAT, 0777); // created destination1.txt with all permissions using 777
	if (fd1 == -1) {
		printf("\nopen(destination1.txt) failed with error [%s]\n",strerror(errno)); // error checking for open
	}
	int fd2 = open("destination2.txt", O_RDWR | O_CREAT, 0777); // created destination2.txt with all permissions using 777
	if (fd1 == -1) {
		printf("\nopen(destination2.txt) failed with error [%s]\n",strerror(errno)); // error checking for open
	}
	fd1 = close(fd1); // since destination1.txt has been created we can now close 
	if (fd1 == -1) {
		printf("\nclose(destination1.txt) failed with error [%s]\n",strerror(errno)); // error checking for close
	}
	fd2 = close(fd2); // since destination2.txt has been created we can now close
	if (fd2 == -1) {
		printf("\nclose(destination2.txt) failed with error [%s]\n",strerror(errno)); // eror checking for close
	}
    return 0;
}