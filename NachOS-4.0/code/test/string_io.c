#include "syscall.h"

char string[256];
int main()
{
    int lengthStr = 0;
    PrintString("String length must be <= 255\n");
    PrintString("String length: ");
    lengthStr = ReadNum();
    PrintString("Enter your string: ");
    ReadString(string, lengthStr);
    PrintString("\nYour string: ");
    PrintString(string);
    PrintString("\n");
    Halt();
    return 0;
}