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

    if (Remove(filename) == -1)
    {
        PrintString("Failed to delete file");
        return -1;
    }
    else
    {
        PrintString("File: ");
        PrintString(filename);
        PrintString(" deleted successfully!\n");
    }
    Halt();
    return 0;
}