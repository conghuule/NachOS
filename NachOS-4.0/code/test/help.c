#include "syscall.h"

int main()
{
    PrintString("Teammate: \n");
    PrintString("\t20120294 - Le Cong Huu\n");
    PrintString("\t20120312 - Le Tan Kiet\n\n");

    PrintString("Ascii: At code/build.linux, run './nachos -x ../test/ascii' to print Ascii characters\n\n");

    PrintString("Sort: At code/build.linux, run './nachos -x ../test/bubble_sort' to start\n");
    PrintString("\tInput: \n");
    PrintString("\t\t- Array need sort (char*)\n");
    PrintString("\t\t- Length of array (int)\n");
    PrintString("\t\t- Order (1: increase, 2: decrease) (int)\n");
    PrintString("\tOutput:  \n");
    PrintString("\t\t- Sorted array (char*)\n");
}