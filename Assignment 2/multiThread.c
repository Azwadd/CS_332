/* CS 332 - Operating Systems */
/* Assignment 2 - Multithreading */
/* TO COMPILE USE gcc multiThread.c -lpthread -o multi */
#include <stdio.h> 
#include <pthread.h>
#include <time.h> 
#define total_threads 4
#define range 100000
int i;
int divisors; 
int number;

// struct to pass two integers into each thread
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
            number = i;         // save the number with the highest number of divisors
        }
    }
    pthread_exit(0);
} 

/*Develop a multithreading program that can find the integer in the range 1 to 100000 that has the largest
number of divisors. It should print that integer as well as the time spent to finish the calculation. You
can use any language (e.g. C, Java, or Python), and Any API library.*/
int main() {   
    // Setting up the params to split the task //
    divisors = 0; 
    number = 0;  
    struct parameters params[total_threads]; // stores parameters for each individual thread
    for (i = 0;i < total_threads;i++) {
        params[i].start = ((range * i) / total_threads);    // this will allow the task to be split 
        params[i].end = ((range/total_threads) * (i+1));   // 0-2500 / 2500 - 5000 / 5000 - 7500 / 7500 - 1000
    }

    // Start of Multithreading  //
    printf("The task is to find the number with most divisors\nThe range of the numbers are 1-%d\n\nExecuting the task with %d thread\n", range, total_threads); 
    pthread_t threads[total_threads];
    pthread_attr_t attr; 
    pthread_attr_init(&attr);

    clock_t start = clock(); // task timer starts and starts the task
    for (int k = 0;k < total_threads;k++) { pthread_create(&threads[k], &attr, run, (void *)&params[k]); }
    for (int k = 0;k < total_threads;k++) { pthread_join(threads[k], NULL); }
    clock_t finish = clock(); // task completed and the task time is saved


    printf("The number with the highest number of divisors is %d with %d divisors\n", number, divisors);
    printf("This task took %f seconds\n", ((double)(finish - start) / CLOCKS_PER_SEC));
    // END of Multithreading //

    return 0;
}