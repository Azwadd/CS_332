/* CS 332 - Operating Systems */
/* Lab 4 - Part 1 by Azwad Shameem */
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

int main() {
    execl("/bin/mv", "mv", "test2.txt", "test.txt", NULL);
}