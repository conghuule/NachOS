#include "syscall.h"

char filename[256];
int main()
{
    int lengthStr = 0;
    PrintString("Filename length must be <= 255\n");
    PrintString("Filename length: ");
    lengthStr = ReadNum();
    PrintString("Enter your filename: ");
    ReadString(filename, lengthStr);
    PrintString("\nYour filename: ");

    if (Create(filename) == -1)
    {
        PrintString("Failed to create file");
    }
    else
    {
        PrintString("File: ");
        PrintString(filename);
        PrintString(" create file successfully!\n");
    }
    Halt();
    return 0;
}