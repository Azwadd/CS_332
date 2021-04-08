/* CS 332 - Operating Systems */
/* Assignment 3 */
/* TO COMPILE USE gcc assignment_3.c */
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
    //pthread_mutex_lock(&lock);
    struct parameters *args = params;
    args->c++;
    printf("%s#%d farmer can cross the bridge\n", args->bridge, args->c);

    srand(time(NULL));
    printf("%s#%d is traveling on the bridge...\n", args->bridge, args->c);
    sleep(rand() % 3);

    printf("%s#%d farmer has left the bridge\n\n", args->bridge, args->c);
    //pthread_mutex_unlock(&lock);
    pthread_exit(0);
}

int main() {
    pthread_t north[threads];
    pthread_t south[threads];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_mutex_init(&lock, NULL);
    struct parameters params[2];
    params[0].c = 0;
    params[0].bridge = "North Tunbridge ";
    params[1].c = 0;
    params[1].bridge = "South Tunbridge ";

    for(int i = 0; i < threads;i++) {
        pthread_create(&north[i], &attr, runner, (void *)&params[0]);
        pthread_create(&south[i], &attr, runner, (void *)&params[1]);
    }

    for (int i = 0; i < threads;i++) {
        pthread_join(north[i],NULL);
        pthread_join(south[i],NULL);

    }

    pthread_mutex_destroy(&lock);
}