#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#define CYLINDERS 10000
#define REQUESTS 1000

int FCFS(int disk[], int head);
int SSTF(int disk[], int head);
int SCAN(int disk[], int head);
int CSCAN(int disk[], int head);
int LOOK(int disk[], int head);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Incorrect Usage! Please enter the starting position as a parameter when running on the command line.\n");
        return -1;
    } else if (argc > 2) {
        printf("Please enter a single parameter, the starting position only.\n");
        return -1;
    }
    int disk[REQUESTS], head = atoi(argv[1]);
    if (head < 0) { printf("Incorrect parameter, starting position should be 0-999");  return -1; }
    srand(time(NULL)); // randomizes rand()
    for(int i = 0; i< REQUESTS;i++) { disk[i] = rand() % CYLINDERS; } // Disk has cylinders from 0-9999

    printf("< Algorithm  - Head Movement >");
    printf("\n< 1. FCFS    - %d >", FCFS(disk, head));
    printf("\n< 2. SSTF    - %d >", SSTF(disk, head));
    printf("\n< 3. SCAN    - %d >", SCAN(disk, head));
    printf("\n< 4. CSCAN   - %d >", CSCAN(disk, head));
    printf("\n< 5. LOOK    - %d >\n", LOOK(disk, head));
    return 0;
}

/* First Come First Serve - get the elements in order from the head */
int FCFS(int disk[], int head) {
    int headMovement = 0, current = disk[head];
    // Add the Seek distance from head for every element in order from head to the end
    for (int i = head;i < REQUESTS;i++) {
        headMovement += abs(disk[i] - current);
        current = disk[i];
    }
    // Now since head to the end has been calculated in order
    // We can now take care of everything before the head
    for (int i = 0;i < head;i++) {
        headMovement += abs(disk[i] - current);
        current = disk[i];
    }
    return headMovement;
}

/* Shortest Seek Time First Algorithm - repeatedly find closest elements*/
int SSTF(int disk[], int head) {
    int headMovement = 0, position = 0;
    bool visited[REQUESTS] = {false}; // Bool array to check if element has already been visited - prevents repeat of same element
    visited[head] = true; // Since we start from the head it should automatically be set to true

    // Loop non-stop, until there is no more elements to find shortest distance
    while (true) {

        // Current = 10000 is just a placeholder, obviously max distance from any point in this cylinder is 9999
        // thus current is assured to be changed since we are finding minimum distance
        // current is supposed to represent minimum distance from the current head pointer on the disk
        int current = CYLINDERS;
        // Check all elements if their have a shorter distance from the current head element
        for (int i = 0;i < REQUESTS;i++) {
            if (current >= abs(disk[head] - disk[i]) && !visited[i]){ // ONLY for elements not already visited!
                current = abs(disk[head] - disk[i]);
                position = i; // save the element location where the current distance from head is minimum
            }
        }

        // We assume that since position has not changed, all elements have been visited
        if (head == position) {
            return headMovement; // so we can just return the summation of the distances
        } else {
            headMovement += abs(disk[head] - disk[position]); // Add the distance from head to the closest element
            visited[position] = true; // Set this position to true so that we do not check it again
            head = position; // this position is now the new head
        }
    }
}
/* SCAN algorithm - First check all elements less than head and reach zero,
 * then check all the elements that are greater than head */
int SCAN(int disk[], int head) {
    int headMovement = 0, position = 0, curr_ptr = head; // curr_ptr is the head that changes per iteration
    bool toZero = true, fromZero = true;
    while (true) {

        // Current = 10000 is just a placeholder, obviously max distance from any point in this cylinder is 9999
        // thus current is assured to be changed since we are finding minimum distance
        // current is supposed to represent minimum distance from the current head pointer on the disk
        int current = CYLINDERS;
        // For all elements less than the head, all the way to zero, reaching zero
        if (toZero) {
            for (int i = 0; i < REQUESTS; i++) {
                if (disk[curr_ptr] > disk[i] && current >= abs(disk[curr_ptr] - disk[i])) {
                    current = abs(disk[curr_ptr] - disk[i]);
                    position = i; // saves position of closest smaller element than current head
                }
            }

            // if the position does not change, assume all elements that satisfy the requirements have been checked
            if (curr_ptr == position) {
                // For scan the requirements are to be less than head, and reach to zero
                // disk[curr_ptr] is the smallest element less than head
                headMovement += disk[curr_ptr]; // Therefore, we can add it's distance from 0, to simulate that we reached zero
                curr_ptr = head; // Set the curr_ptr to head so that we can now find elements greater than the head
                toZero = false; // toZero is false because we have reached zero and now we need to do the other part
            } else {
                // General case - found closest element smaller than the head
                headMovement += abs(disk[curr_ptr] - disk[position]); // add it's distance to the total
                curr_ptr = position; // this element is now the new head
            }

        } else {
            // For all elements greater than the head
            for (int i = 0; i < REQUESTS; i++) {
                if (disk[curr_ptr] < disk[i] && current >= abs(disk[curr_ptr] - disk[i])) {
                    current = abs(disk[curr_ptr] - disk[i]);
                    position = i; // saves position of elements greater than the head
                }
            }

            // Assume since position has not changed, there is no great element
            if (curr_ptr == position) {
                return headMovement; // therefore we have found all the elements and we can return
            } else if (fromZero) {
                // According to the SCAN algorithm we must go from 0 to the closest element greater than head
                headMovement += disk[position]; // we add the distance from 0 to this element to the total distance
                curr_ptr = position; // this element is the new head
                fromZero = false; // fromZero is set to false because we have added the distance from 0 to the closest greater element
            } else {
                // General case - Found greater element than the head
                headMovement += abs(disk[curr_ptr] - disk[position]); // add its distance tot he total
                curr_ptr = position; // this element is greater than the head
            }

        }
    }
}
/* C-SCAN algorithm - */
int CSCAN(int disk[], int head) {
    int headMovement = 0, position = 0, curr_ptr = head;
    bool toMax = true, fromMax = true;
    while (true) {

        // Current = 10000 is just a placeholder, obviously max distance from any point in this cylinder is 9999
        // thus current is assured to be changed since we are finding minimum distance
        // current is supposed to represent minimum distance from the current head pointer on the disk
        int current = CYLINDERS;
        // C-SCAN goes to the highest element in the disk first
        if (toMax) {
            for(int i = 0;i < REQUESTS;i++) {
                // if element is greater than the head and is also the closest element, save it
                if (disk[head] < disk[i] && disk[curr_ptr] < disk[i]  && current >= abs(disk[curr_ptr] - disk[i])){
                    current = abs(disk[curr_ptr] - disk[i]);
                    position = i; // saves position of element
                }
            }

            // Assuming there is no other element to check
            if (curr_ptr == position) {
                // For C-SCAN disk pointer goes from highest element to the maximum edge on the disk
                headMovement += abs(disk[curr_ptr] - CYLINDERS-1); // Add distance from highest element to max on disk
                curr_ptr = head; // reset curr_ptr to the original head
                toMax = false; // toMax is false because we accomplish this part of the task, so we won't repeat bc its now false
            } else {
                // General case - add distance from head to closest point to distance
                headMovement += abs(disk[curr_ptr] - disk[position]);
                curr_ptr = position; // this element is now the new head
            }

        } else {

            // Maximum edge has been reached, for C-SCAN algo, we now need to go to zero edge
            if (fromMax) {
                headMovement += CYLINDERS-1; // CYLINDERS-1 is distance from one edge to another so now we have reached zero
                // Find closest element to zero edge
                for (int i = 0;i < REQUESTS;i++) {
                    if (current >= abs(0 - disk[i]) && disk[i] < disk[head]){
                        current = abs(0 - disk[i]);
                        position = i; // save its element
                    }
                }
                headMovement += disk[position]; // add distance from 0 to the point to the total distance
                curr_ptr = position; // this point is the new head
                fromMax = false; // set to false so we don't repeat this

            } else {
                // For all elements greater than curr_ptr and less than original head
                for(int i = 0;i < REQUESTS;i++) {
                    // Find point closest to curr_ptr
                    if (disk[curr_ptr] < disk[i] && disk[head] > disk[i] && current >= abs(disk[curr_ptr] - disk[i])){
                        current = abs(disk[curr_ptr] - disk[i]);
                        position = i; // save its location
                    }
                }

                // Assume if position hasn't changed there is no more point to check
                if (curr_ptr == position) {
                    return headMovement; // just return result
                } else {
                    // General case - closest point found
                    headMovement += abs(disk[curr_ptr] - disk[position]); // add its distance
                    curr_ptr = position; // this point is now the head
                }
            }
        }
    }
}

int LOOK(int disk[], int head) {
    int headMovement = 0, position = 0, curr_ptr = head, edge_ptr;
    bool toMax = true, fromMax = true;

    while (true) {
        // Current = 10000 is just a placeholder, obviously max distance from any point in this cylinder is 9999
        // thus current is assured to be changed since we are finding minimum distance
        // current is supposed to represent minimum distance from the current head pointer on the disk
        int current = CYLINDERS;
        // LOOK first goes to max
        if (toMax) {
            for(int i = 0;i < REQUESTS;i++) {
                // closest point to head point and greater than head point is saved
                if (disk[curr_ptr] < disk[i] && current >= abs(disk[curr_ptr] - disk[i])){
                    current = abs(disk[curr_ptr] - disk[i]);
                    position = i; // save position
                }
            }
            // Assume if position does not change there is no higher position
            if (curr_ptr == position) {
                // UNLIKE SCAN, LOOK does not reach the edge!
                edge_ptr = curr_ptr; // save location of highest point in edge_ptr
                curr_ptr = head; // curr_ptr is now the head
                toMax = false; // set to false to prevent repeat
            } else {
                // General case - found greater element save its distance
                headMovement += abs(disk[curr_ptr] - disk[position]);
                curr_ptr = position; // element is new head
            }

        } else {
            // For all elements less than the head
            for(int i = 0;i < REQUESTS;i++) {
                // find closest element and save its position as long as its less then the original head
                if (disk[curr_ptr] > disk[i] && current >= abs(disk[curr_ptr] - disk[i])) {
                    current = abs(disk[curr_ptr] - disk[i]);
                    position = i; // save position
                }
            }
            // Assume if position changed all elements has been visited
            if (curr_ptr == position) {
                return headMovement; // just return because everything has been visited
            } else if (fromMax) {
                // For look we go from the max point to the closest point less than the original head
                headMovement += abs(disk[edge_ptr]-disk[position]); // add distance of max to closest element of the original head
                curr_ptr = position; // curr_ptr is now the new head being checked closest to
                fromMax = false; // set to false to prevent repeat
            } else {
                // General case - found closest element
                headMovement += abs(disk[curr_ptr] - disk[position]); // add its distance to the toal distance
                curr_ptr = position; // element is the new head
            }
        }
    }
}
