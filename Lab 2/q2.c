#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>

int main() {
    int a = 10, b =25, fq = 0, fr = 0;
    fq= fork(); // fork a child - call it process Q //
        if (fq == 0) {  // child successfully forked //
        a = a + b;
        // prints a, b, c and pid of process //
        printf("Process Q - a = %d b = %d pid = %d\n", a, b, getpid());
        fr = fork();    // fork another child - call it process R //
        if (fr != 0) {
            b = b + 15;
            // prints a, b, c and pid of process //
            printf("Parent of Process R - a = %d b = %d pid = %d\n", a, b, getpid());
        } else {
            // prints a, b, c and pid of process //
            a = (a * b) + 20;
            printf("Process R - a = %d b = %d pid = %d\n", a, b, getpid());
        }
    } else {
        b = a + b - 5;
        // prints a, b, c and pid of process //
        printf("Parent Process - a = %d b = %d pid = %d\n", a, b, getpid());
    }
    return 0;
}