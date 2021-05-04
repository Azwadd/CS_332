#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <signal.h>
#include "sem.h"
#define LOOPS 10
int lock, mutex_agent, mutex_tobacco, mutex_paper, mutex_matches;

int main() {
    // Create semaphores
    if ((lock = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1          ||
        (mutex_agent = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1   ||
        (mutex_matches = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1 ||
        (mutex_paper = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1   ||
        (mutex_tobacco = semget(IPC_PRIVATE, 1, 0666 | IPC_CREAT)) == -1) {
        printf("Error: semget() - [%s]\n", strerror(errno));
        return -1;
    } else {
        sem_create(lock, 1);
        sem_create(mutex_agent, 0);
        sem_create(mutex_matches, 0);
        sem_create(mutex_paper, 0);
        sem_create(mutex_tobacco, 0);
    }


    pid_t agent, smoker_tobacco, smoker_paper, smoker_matches;
    if ((agent = fork()) < 0) {
        printf("Error: fork() - [%s]\n", strerror(errno));
        return -1;
    } else if (agent == 0) {
        srand(time(NULL));
        for (int i = 0;i < LOOPS;i++) {
            P(lock);
            int x = rand()%3;
            if (x == 0) {
                printf("Agent puts tobacco and paper on the table.\n");
                V(mutex_matches);
            } else if (x == 1) {
                printf("Agent puts tobacco and matches on the table.\n");
                V(mutex_paper);
            } else if (x == 2) {
                printf("Agent puts paper and matches on the table.\n");
                V(mutex_tobacco);
            }
            V(lock);
            P(mutex_agent);
        }
        return 0;
    } else {
        if ((smoker_matches = fork()) < 0) {
            printf("Error: fork() - [%s]\n", strerror(errno));
            return -1;
        } else if (smoker_matches == 0) {

            for (int i = 0;i < LOOPS;i++) {
                P(mutex_matches);
                P(lock);
                printf("Smoker with the matches picks up tobacco and paper from the table.\n");
                V(mutex_agent);
                V(lock);
                printf("Smoker with the matches, smokes his cigarette.\n\n");
            }

        } else {
            if ((smoker_paper = fork()) < 0) {
                printf("Error: fork() - [%s]\n", strerror(errno));
                return -1;
            } else if (smoker_paper == 0) {

                for (int i = 0;i < LOOPS;i++) {
                    P(mutex_paper);
                    P(lock);
                    printf("Smoker with the paper takes the tobacco and matches from the table.\n");
                    V(mutex_agent);
                    V(lock);
                    printf("Smoker with the paper, smokes his cigarette.\n\n");
                }

            } else {
                if ((smoker_tobacco = fork()) < 0) {
                    printf("Error: fork() - [%s]\n", strerror(errno));
                    return -1;
                } else if (smoker_tobacco == 0) {

                    for (int i = 0;i < LOOPS;i++) {
                        P(mutex_tobacco);
                        P(lock);
                        printf("Smoker with the tobacco takes the paper and matches from the table.\n");
                        V(mutex_agent);
                        V(lock);
                        printf("Smoker with the tobacco, smokes his cigarette.\n\n");
                    }

                } else {
                    wait(&agent);
                    wait(&smoker_matches);
                    wait(&smoker_paper);
                    wait(&smoker_tobacco);
                    semkill(lock);
                    semkill(mutex_agent);
                    semkill(mutex_matches);
                    semkill(mutex_paper);
                    semkill(mutex_tobacco);
                    return 0;
                }
            }
        }
    }

}