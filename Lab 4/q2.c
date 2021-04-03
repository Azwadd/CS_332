
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <string.h>
#define chapters 2
#define hwPerChapter 2
#define students 10
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
            // Loop to create a worker per homeworks per chapter
            for (int worker = 0;worker < hwPerChapter;worker++, count++) {
                pid_t pidWorker = fork();
                if (pidWorker == -1) {
                    perror("Fork() Error");
                    return -1;
                } else if (pidWorker == 0) {
                    printf("yes");
                    for (int i = count+4;i < size;i+=4) {
                        grades[count] += grades[i];
                    }
                    printf("%d ", grades[count]);
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