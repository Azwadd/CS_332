// CS 332 - Operating Systems Lab //
// Lab 1 - Part 3 by Azwad Shameem //
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
/*3. Repeat part 2 (by writing a new C program) as per the following procedure:
(a) Read the next 50 characters from source.txt, and among characters read,
replace each character ’5’ with character ‘A’’ and all characters are then written in destination.txt
(b) Write characters "XYZ" into file destination.txt
(c) Repeat the previous steps until the end of file source.txt. The last read step may not have 50 characters.*/
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
	int fd2 = open(argv[2], O_RDWR); // argv[2] is assumed as destination file
	if(fd2 == -1) {	// Checks if opening the destination file in write only mode has an error if it does utilize strerror(errno) to report
		printf("Open() for destination file has failed with error [%s]\n", strerror(errno));      
		return -1;
	}

	// File opened properly, so now we can read from source and write to destination following the procedure listed in the question 
	printf("Open() Successful\n");
	printf("Now following the procedure to copy the edited version of every 50 characters from %s to %s and add 'XYZ' each time.\n", argv[1], argv[2]);  
	char output[50];				// buffer to put the content of the file
	int c_read, c_write, c_write2;	// counter for read() and counter for write()
	while ((c_read = read(fd, output, 50)) > 0) {
		if (c_read == -1) { perror("read"); return -1; }	// this makes sure that if read has an error, it will report the erorr 

		for (int i = 0;i < strlen(output);i++) {
			if (output[i] == '5') { output[i] = 'A'; }		// for every 5 in the 50 characters of the file replace it with a A
		}
		c_write = write(fd2, output, c_read);				// now write the edited version of the 50 characters into the file
		c_write2 = write(fd2, "XYZ", 3);					// following the procedure we must also add 3 characters of "XYZ" per each 50 read

		if (c_write == -1) { perror("write"); return -1; }	// this makes sure that if write has an error, it will report the error
		if (c_write2 == -1) { perror("write2"); return -1; }	// this makes sure that if write has an error, it will report the error

	}
	close(fd);
	close(fd2);
	printf("Copying completed\n");
	return 0;
}