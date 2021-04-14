#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "sem.h"

int main() {
    int sem, key, nsems, flags;
    sem = semget(key, nsems, flags | 0600);
    sem_create(sem,1);
    pid_t pid = fork();
    if (pid == -1) {
        return -1;
    } else if (pid == 0) {
        P(sem);
        printf("pid %d about to wait\n", getpid());

        printf("pid %d done w\n", getpid());
        V(sem);
    } else {
        if ((pid = fork()) == -1) {
            return -1;
        } else if (pid == 0) {
            P(sem);
            printf("pid %d about to waits\n", getpid());
            printf("pid %d done ws\n", getpid());
            V(sem);
        } else {
            wait(&pid);
            wait(&pid);
            semkill(sem);
        }
    }
}