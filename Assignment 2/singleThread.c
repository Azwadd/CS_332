/* CS 332 - Operating Systems */
/* Assignment 2 - singleThread */
/* TO COMPILE USE gcc singleThread.c -lpthread -o single */
#include <stdio.h> 
#include <stdlib.h> 
#include <pthread.h> 
#include <time.h> 
#define total_threads 1
#define range 100000
int i;
int divisors; 
int number;

struct parameters {
    int start;
    int end;
} parameters_default = {0,range};

void *run(void *param) { 
    struct parameters *args = param;
    // Find integer with the highest number of divisors within range given
    for (i = args->start;i  <= args->end;i++) {
        int count = 0;
        for (int j = 1;  j <= i;  j++ ) {
            if (i % j == 0) { ++count; }  // if divisor is found increment count             
        }
        if (count > divisors) { // if count is higher than the current highest number of divisors 
            divisors = count;   // make count the next highest number of divisors
            number = i;         // save the number with the highest number of dicisors
        }
    }
    pthread_exit(0);
} 

/* Develop a multithreading program that can find the integer in the range 1 to 100000 that has the largest
number of divisors. It should print that integer as well as the time spent to finish the calculation. You
can use any language (e.g. C, Java, or Python), and Any API library.*/
int main() {   
    // variables to initalize
    struct parameters param = parameters_default; // sets default parameters when calling runner()
    // setting up the global variables for the task
    divisors = 0; 
    number = 0;

    // Start of Single Thread // 
    printf("The task is to find the number with most divisors\nThe range of the numbers are 1-%d\n\nExecuting the task with %d thread\n", range, total_threads); 
    pthread_t thread;
    pthread_attr_t attr;
    pthread_attr_init(&attr); 

    clock_t start = clock(); // task timer starts and starts the task
    pthread_create(&thread, &attr, run, (void*)&param); 
    pthread_join(thread, NULL);
    clock_t finish = clock(); // task completed and the task time is saved

    printf("The number with the highest number of divisors is %d with %d divisors\n", number, divisors);
    printf("This task took %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC));
    // END of Single Thread //


    return 0;
}