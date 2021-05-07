#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define CYLINDERS 10000
#define REQUESTS 1000

int FCFS(int disk[], int start);
int SSTF(int disk[], int start);
int SCAN(int disk[], int start);
int CSCAN(int disk[], int start);
int LOOK(int disk[], int start);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Incorrect Usage! Please enter the starting position as a parameter when running on the command line.\n");
        return -1;
    } else if (argc > 2) {
        printf("Please enter a single parameter, the starting position only.\n");
        return -1;
    }
    int disk[REQUESTS], start = atoi(argv[1]);
    //srand(time(NULL));
    for(int i = 0; i< REQUESTS;i++) { disk[i] = rand()%CYLINDERS; } // Disk has cylinders from 0-9999

    printf("< Algorithm - Head Movement >");
    printf("\n< 1. FCFS      - %d >", FCFS(disk, start));
    qsort(disk, sizeof(disk)/sizeof(*disk), sizeof(*disk), compare);
    printf("\n< 2. SSTF      - %d >", SSTF(disk, start));
    printf("\n< 3. SCAN      - %d >", SCAN(disk, start));
    printf("\n< 4. C-SCAN    - %d >", CSCAN(disk, start));
    printf("\n< 5. LOOK      - %d >", LOOK(disk, start));
    return 0;
}

int compare(const void *x, const void *y) { // comparator function for quick sort
    int a = *((int*) x);
    int b = *((int*) y);
    return a>b ? 1 : -1;
}

int FCFS(int disk[], int start) {
    int headMovement = 0, current = disk[start];
    for (int i = start;i < REQUESTS;i++) {
        headMovement += abs(disk[i] - current);
        current = disk[i];
    }
    for (int i = 0;i < start;i++) {
        headMovement += abs(disk[i] - current);
        current = disk[i];
    }
    return headMovement;
}

int SSTF(int disk[], int start) {
    int headMovement, current = disk[start], head = start - 1, tail = start + 1;
    for (int i = REQUESTS - 2; i >= 0; i--) {
        if ((abs(disk[head]) - current) <= (abs(disk[tail]) - current)) {
            headMovement += abs(disk[head] - current);
            current = disk[head];
            head--;
        } else {
            headMovement += abs(disk[tail] - current);
            current = disk[tail];
            tail++;
        }
    }
    return headMovement;
}

int SCAN(int disk[], int start) {
    int headMovement = 0, current = disk[start];
    for (int i = start-1;i >= 0;i--) {
        headMovement += abs(disk[i] - current);
        current = disk[i];
    }
    headMovement += current;
    current = 0;
    for (int i = start+1;i < REQUESTS;i++) {
        headMovement += abs(disk[i] - current);
        current = disk[i];
    }
    return headMovement;
}

int CSCAN(int disk[], int start) {
    int headMovement = 0, current = disk[start];
    for (int i = start;i < REQUESTS;i++) {
        headMovement += abs(disk[i] - current);
        current = disk[i];
    }
    headMovement += (9999 - current + 9999);
    current = 0;
    for (int i = 0;i < start;i++) {
        headMovement += abs(disk[i] - current);
        current = disk[i];
    }
    return headMovement;
}

int LOOK(int disk[], int start) {
    int headMovement = 0, current = disk[start];
    for (int i = start+1;i < REQUESTS;i++) {
        headMovement += abs(disk[i] - current);
        current = disk[i];
    }
    for (int i = start-1;i >= 0;i--) {
        headMovement += abs(disk[0] - current);
        current = disk[i];
    }
    return headMovement;
}
