#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include "sem.h"
#define LOOPS 1
int mutex_agent, mutex_lock, mutex_tobacco, mutex_paper, mutex_matches;

int main() {
    if ((mutex_agent = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1   ||
        (mutex_lock = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1    ||
        (mutex_tobacco = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1 ||
        (mutex_paper = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1   ||
        (mutex_matches = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1) {
        printf("Error: semget() - [%s]\n", strerror(errno));
        return -1;
    } else {
        sem_create(mutex_agent, 1);
        sem_create(mutex_lock, 1);
        sem_create(mutex_tobacco, 1);
        sem_create(mutex_paper, 1);
        sem_create(mutex_matches, 1);
    }


    pid_t agent, smoker_tobacco, smoker_paper, smoker_matches;
    if ((agent = fork()) == -1 || (smoker_tobacco = fork()) == -1 || (smoker_paper = fork()) == -1 || (smoker_matches = fork()) == -1) {
        printf("Error: fork() - [%s]\n", strerror(errno));
        return -1;
    } else if (agent == 0) {
        for (int i = 0; i < LOOPS;i++) {
            P(mutex_lock);
            srand(time(NULL));
            switch (rand()%3) {
                case 0:
                    printf("Agent puts tobacco and paper on table\n");
                    V(smoker_matches);
                    break;
                case 1:
                    printf("Agent puts tobacco and match on table\n");
                    V(smoker_paper);
                    break;
                case 2:
                    printf("Agent puts match and paper on table\n");
                    V(smoker_tobacco);
                    break;
            }
            V(mutex_lock);
            V(mutex_agent);
        }
    } else if (smoker_tobacco == 0){

        for (int i = 0;i < LOOPS;i++) {
            P(mutex_tobacco);  // Sleep right away
            P(mutex_lock);
            printf("Smoker picks up paper and matches\n");
            V(mutex_agent);
            V(mutex_lock);
        }

    } else if (smoker_paper == 0) {

        for (int i = 0;i < LOOPS;i++) {
            P(mutex_paper);  // Sleep right away
            P(mutex_lock);
            printf("Smoker picks up tobacco and matches\n");
            V(mutex_agent);
            V(mutex_lock);
        }

    } else if (smoker_matches == 0) {

        for (int i = 0;i < LOOPS;i++) {
            P(mutex_matches);  // Sleep right away
            P(mutex_lock);
            printf("Smoker picks up tobacco and paper\n");
            V(mutex_agent);
            V(mutex_lock);
        }

    } else {
        wait(&agent);
        wait(&smoker_tobacco);
        wait(&smoker_paper);
        wait(&smoker_matches);
    }
}