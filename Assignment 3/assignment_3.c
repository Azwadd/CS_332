/* CS 332 - Operating Systems */
/* Assignment 3 */
/* TO COMPILE USE gcc assignment_3.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define threads 4
int n = 1, s = 1;

void *runner() {
    char array[5] = "North";
    printf("%s Tunbridge #%d farmer can cross the bridge\n", array, s);

    srand(time(NULL));
    printf("%s Tunbridge #%d is traveling on the bridge...\n", array, s);
    sleep(rand() % 3);

    printf("%s Tunbridge #%d farmer has left the bridge\n\n", array, s);
    s++;
    //pthread_mutex_unlock(&mutex1);
    pthread_exit(0);
}

int main() {
    pthread_t north[threads];
    pthread_t south[threads];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_mutex_t lock;
    pthread_mutex_init(&lock, NULL);

    pthread_create(&north[0], &attr, runner, NULL);
    pthread_join(north[0],NULL);
    /*for(int i = 0; i < threads; i++){
        pthread_create(&north[i], NULL, runn, NULL);
        pthread_create(&south[i], NULL, SouthFarmer, NULL);
    }*/
}
