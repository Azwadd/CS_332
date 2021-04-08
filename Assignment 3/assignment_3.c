/* CS 332 - Operating Systems */
/* Assignment 3 by Azwad Shameem */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define threads 4
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

struct parameters {
    int c;
    char* bridge;
};

void *runner(void *params) {

    // Comment out the lock if you wish to test without the lock
    pthread_mutex_lock(&lock); // Locks the thread

    struct parameters *args = params;
    args->c++; // Keeps track of which bridge
    printf("%s#%d farmer can cross the bridge\n", args->bridge, args->c);

    srand(time(NULL)); // this allows for rand() to change and not stay the same
    printf("%s#%d is traveling on the bridge...\n", args->bridge, args->c);
    sleep(rand() % 3); // Sleep anywhere up to 3 seconds

    printf("%s#%d farmer has left the bridge\n\n", args->bridge, args->c);

    // Comment without the lock if you wish to test without the lock
    pthread_mutex_unlock(&lock); // Unlocks the thread
    pthread_exit(0);
}

int main() {
    pthread_t north[threads]; // 4 threads for north
    pthread_t south[threads]; // 4 threads for south
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_mutex_init(&lock, NULL); // initializes the mutex lock
    struct parameters params[2]; // this is the information to run either north or south and keep count of which bridge
    params[0].c = 0;
    params[0].bridge = "North Tunbridge ";
    params[1].c = 0;
    params[1].bridge = "South Tunbridge ";

    for(int i = 0; i < threads;i++) { // creates a north and south bridge for each thread
        pthread_create(&north[i], &attr, runner, (void *)&params[0]);
        pthread_create(&south[i], &attr, runner, (void *)&params[1]);
    }

    for (int i = 0; i < threads;i++) { // Waits for the thread to stop
        pthread_join(north[i], NULL);
        pthread_join(south[i], NULL);

    }

    pthread_mutex_destroy(&lock); // frees the lock since we no longer need it
}