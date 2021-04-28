// CS 332 - Operating Systems Lab //
// Lab 1 - Part 0 - A by Azwad Shameem //
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

/* Question 0. Part (a) Extend code snippet 1 to check for read and write access permissions of a given file */
int main (int argc, char* argv[]) {	
	// Note - argv[1] must be the filepath or the program will not work properly
	if (argc < 2) {
		printf("Improper usage. Use the format --> './access filepath' Where filepath is the location of the file.\n");
		return -1;
	} else if (argc > 2) {
		printf("Too many arguments, please use the command in proper format. --> './access filepath' Where filepath is the location of the file.\n");
		return -1;
	}
	// Checking file existence -------------> Note: argv[1] must be the filepath
	errno = 0;
	int returnval = access(argv[1], F_OK);
	if (returnval == 0) {
		printf ("%s exists\n", argv[1]);
	} else {
		if (errno == ENOENT) {      
			printf ("%s does not exist\n", argv[1]);
		} else if (errno == EACCES) { 
			printf ("%s is not accessible\n", argv[1]);
		}
		return 0;
	}

	// Checking read access ...  
	returnval = access(argv[1], R_OK);		// uses returnval to store the return value for access when checking for read access
	if (returnval == 0) {							
		printf ("%s has read access\n", argv[1]);		// return value is 0 the file has been accessed and has read permissions
	} else {
		printf("Open() failed with error [%s]\n",strerror(errno));	// return value is not 0, some error occurred which means access sets errno so we can call strerror(errno) to display the error
	}

	// Checking  write access ... 
	returnval = access(argv[1], W_OK);		// uses returnval to store the return value for access when checking for write access
	if (returnval == 0) {
		printf ("%s has write access\n", argv[1]);	// return value is 0 so the file has been accessed and has write permissions
	} else {
		printf("Open() failed with error [%s]\n",strerror(errno));	// return value is not 0, some error has occurred and access has set up errno which means we can call strerror(errno) to display the error
	}
	return 0;
}