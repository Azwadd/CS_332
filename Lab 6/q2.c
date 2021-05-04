#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <time.h>
#define LOOPS 10	// Use loops to decide how many iterations you desire
// Global Variables
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER, mutex = PTHREAD_MUTEX_INITIALIZER,
mutex_matches = PTHREAD_MUTEX_INITIALIZER, mutex_paper = PTHREAD_MUTEX_INITIALIZER, mutex_tobacco = PTHREAD_MUTEX_INITIALIZER;
pthread_t threadAgent, threadSmokerMatches, threadSmokerPaper, threadSmokerTobacco;

void *agent();
void *smokerMatches();
void *smokerPaper();
void *smokerTobacco();

int main() {
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    pthread_mutex_lock(&mutex);
    pthread_mutex_lock(&mutex_matches);
    pthread_mutex_lock(&mutex_paper);
    pthread_mutex_lock(&mutex_tobacco);

    pthread_create(&threadAgent, &attr, agent, NULL);
    pthread_create(&threadSmokerMatches, &attr, smokerMatches, NULL);
    pthread_create(&threadSmokerPaper, &attr, smokerPaper, NULL);
    pthread_create(&threadSmokerTobacco, &attr, smokerTobacco, NULL);

    pthread_join(threadAgent, NULL);
    pthread_join(threadSmokerMatches, NULL);
    pthread_join(threadSmokerPaper, NULL);
    pthread_join(threadSmokerTobacco, NULL);

    return 0;
}

void *agent() {
    srand(time(NULL));
    for (int i = 0;i < LOOPS;i++) {
        pthread_mutex_lock(&lock);
        switch (rand() % 3 + 1) {
            case 1:
                printf("Agent puts tobacco and paper on the table.\n");
                pthread_mutex_unlock(&mutex_matches);
                break;
            case 2:
                printf("Agent puts tobacco and matches on the table.\n");
                pthread_mutex_unlock(&mutex_paper);
                break;
            case 3:
                printf("Agent puts paper and matches on the table.\n");
                pthread_mutex_unlock(&mutex_tobacco);
                break;
        }
        pthread_mutex_unlock(&lock);
        pthread_mutex_lock(&mutex);
    }
    // Kill the infinite looping threads because desired loop has been completed in agent
    pthread_kill(threadSmokerMatches, SIGINT);
    pthread_kill(threadSmokerPaper, SIGINT);
    pthread_kill(threadSmokerTobacco, SIGINT);
    pthread_exit(0);
}

void *smokerMatches() {
    while (true) {
        pthread_mutex_lock(&mutex_matches);
        pthread_mutex_lock(&lock);
        printf("Smoker with the matches picks up tobacco and paper from the table.\n");
        pthread_mutex_unlock(&mutex);
        pthread_mutex_unlock(&lock);
        printf("Smoker with the matches, smokes his cigarette.\n\n");
    }
}

void *smokerPaper() {
    while (true) {
        pthread_mutex_lock(&mutex_paper);
        pthread_mutex_lock(&lock);
        printf("Smoker with the paper takes the tobacco and matches from the table.\n");
        pthread_mutex_unlock(&mutex);
        pthread_mutex_unlock(&lock);
        printf("Smoker with the paper, smokes his cigarette.\n\n");
    }
}

void *smokerTobacco() {
    while (true) {
        pthread_mutex_lock(&mutex_tobacco);
        pthread_mutex_lock(&lock);
        printf("Smoker with the tobacco takes the paper and matches from the table.\n");
        pthread_mutex_unlock(&mutex);
        pthread_mutex_unlock(&lock);
        printf("Smoker with the tobacco, smokes his cigarette.\n\n");
    }
}