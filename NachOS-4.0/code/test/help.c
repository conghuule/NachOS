#include "syscall.h"

int main()
{
    PrintString("My Team: \n");
    PrintString("20120219 - Nguyen Minh Tri\n");
    PrintString("20120312 - Ngo Thanh Luc\n\n");
    PrintString("How to run my program: \n");
    PrintString("\tASCII: At directory /nachos/NachOS-4.0/code/test, run '../build.linux/nachos -x ascii' to see ASCII Table\n\n");
    PrintString("\tBubble Sort: At directory /nachos/NachOS-4.0/code/test, run './nachos -x ../test/bubble_sort'\n");
    PrintString("\tInput:  \n");
    PrintString("\t\t- Enter array want to sort (char*)\n");
    PrintString("\t\t- Length of array\n");
    PrintString("\t\t- Order (1: Increase, 2: Decrease)\n");
    PrintString("\tOutput:  \n");
    PrintString("\t\t- Sorted array (char*)\n");
    Halt();
}