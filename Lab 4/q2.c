/* CS 332 - Operating Systems */
/* Lab 4 - Part 2 by Azwad Shameem */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#define chapters 5 // predefined by source.txt
#define hwPerChapter 2 // predefined by source.txt
#define students 10  // predefined by assignment instructions
#define quantityOfGrades chapters*hwPerChapter*students

int main() {
    int size = 0, grades[quantityOfGrades], bufferCount = chapters * hwPerChapter * 3 * students;
    char buffer[bufferCount];

    // Director Process - read grades and store it so that we can grade it later on //
    int fd = open("source.txt", O_RDONLY);
    if (fd == -1) { perror("Open() error with source file"); }
    // Read line by line of data and save values to the array - Note: Number of lines is deduced by number of students
    int read_count = read(fd, buffer, bufferCount);
    if (read_count == -1) { perror("Read() error with source file"); } // error checking
    // Convert all numbers in the string and store the values into the array
    char* token = strtok(buffer, " \n");
    while (token != NULL) {
        grades[size++] = atoi(token);
        token = strtok(NULL, " \n");
    }
    int error = close(fd);
    if (error == -1) { perror("Close() error closing source file"); } // error checking

    // Loop to create a manager per chapter
    for (int manager = 0, count = 0;manager < chapters;manager++, count += hwPerChapter) {
        pid_t pidManager = fork();
        if (pidManager == -1) {
            perror("Fork() Error");
            return -1;
        } else if (pidManager == 0) {
            printf("\nThis is the Manger #%d Process\n", manager+1);
            // Loop to create a worker per homeworks per chapter
            for (int worker = 0;worker < hwPerChapter;worker++, count++) {
                pid_t pidWorker = fork();
                if (pidWorker == -1) {
                    perror("Fork() Error");
                    return -1;
                } else if (pidWorker == 0) {
                    for (int i = count+chapters*hwPerChapter;i < size;i+=chapters*hwPerChapter) {
                        grades[count] += grades[i]; // essentially the way the
                    }
                    printf("This is the Worker #%d Process\n", count+1);
                    printf("The average for chapter %d, homework %d is %f\n", (int)count/2+1, count%2, (double)grades[count]/10);
                    return 0;
                } else {
                    wait(&pidWorker);
                }

            }
            return 0;
        } else {
            wait(&pidManager);
        }
    }
    return 0;
}