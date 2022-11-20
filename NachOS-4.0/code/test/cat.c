#include "syscall.h"
char filename[256];
#define MODE_READWRITE 0
#define MODE_READ 1
int main()
{
    int lengthStr = 0;
    int fileID;
    char buffer[100];
    int read;
    int length = 0;
    PrintString("Filename length must be <= 255\n");
    PrintString("Filename length: ");
    lengthStr = ReadNum();
    PrintString("Enter your filename: ");
    ReadString(filename, lengthStr);
    fileID = Open(filename, MODE_READ);
    if (fileID == -1)
    {
        PrintString("Failed to open file");
    }
    else
    {
        PrintString("File: ");
        PrintString(filename);
        PrintString(" open file successfully!\n");
        read = Read(buffer, 50, fileID);
        while (buffer[length] != '\0')
            length++;
        PrintString("File content: ");
        PrintString(buffer);
    }
    Halt();
    return 0;
}