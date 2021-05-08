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
int lock, mutex_agent, mutex_tobacco, mutex_paper, mutex_matches, status;

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
            switch (rand()%3+1) {
                case 1:
                    printf("Agent puts tobacco and paper on the table.\n");
                    V(mutex_matches);
                    break;
                case 2:
                    printf("Agent puts tobacco and matches on the table.\n");
                    V(mutex_paper);
                    break;
                case 3:
                    printf("Agent puts paper and matches on the table.\n");
                    V(mutex_tobacco);
                    break;
            }
            V(lock);
            P(mutex_agent);
        }
        // We are done with the loops, so kill processes with infinite looping
    } else {
        if ((smoker_matches = fork()) < 0) {
            printf("Error: fork() - [%s]\n", strerror(errno));
            return -1;
        } else if (smoker_matches == 0) {

            while (1) {
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

                while (1) {
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

                    while(1) {
                        P(mutex_tobacco);
                        P(lock);
                        printf("Smoker with the tobacco takes the paper and matches from the table.\n");
                        V(mutex_agent);
                        V(lock);
                        printf("Smoker with the tobacco, smokes his cigarette.\n\n");
                    }

                } else {
                    waitpid(agent, &status, 0);
                    if (WIFEXITED(status)) {
                        // Since agent is done kill the infite loops
                        kill(smoker_matches, SIGINT);
                        kill(smoker_paper, SIGINT);
                        kill(smoker_tobacco, SIGINT);
                    }
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