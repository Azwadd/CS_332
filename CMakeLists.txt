cmake_minimum_required(VERSION 3.17)
project(CS_332 C)

set(CMAKE_C_STANDARD 11)

include_directories(.)
include_directories("Assignment 1")
include_directories("Assignment 2")
include_directories("Lab 1")
include_directories("Lab 2")
include_directories("Lab 3")
include_directories("Lab 4")

add_executable(CS_332
        "Assignment 1/assignment_1.c"
        "Assignment 2/multiThread.c"
        "Assignment 2/singleThread.c"
        "Lab 1/q0a.c"
        "Lab 1/q0b.c"
        "Lab 1/q1.c"
        "Lab 1/q2.c"
        "Lab 1/q3.c"
        "Lab 2/processes.c"
        "Lab 2/q2.c"
        "Lab 3/Parent_Process.c"
        "Lab 3/Process_P1.c"
        "Lab 3/Process_P2.c"
        "Lab 4/q1.c")
