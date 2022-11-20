#include "syscall.h"
char filename1[256];
char filename2[256];
#define MODE_READWRITE 0
#define MODE_READ 1
int main()
{
    int lengthFileName1 = 0;
    int lengthFileName2 = 0;
    int fileID;
    char buffer[100];
    char buffer2[100];
    int length = 0;
    PrintString("Filename length must be <= 255\n");
    PrintString("Filename 1 length: ");
    lengthFileName1 = ReadNum();
    PrintString("Enter your filename: ");
    ReadString(filename1, lengthFileName1);
    fileID = Open(filename1, MODE_READ);
    if (fileID == -1)
    {
        PrintString("Failed to open file");
    }
    else
    {
        PrintString("File: ");
        PrintString(filename1);
        PrintString(" open file successfully!\n");
        Read(buffer, 50, fileID);
        while (buffer[length] != '\0')
            length++;
        Close(fileID);
    }
    PrintString("Filename length must be <= 255\n");
    PrintString("Filename 2 length: ");
    lengthFileName2 = ReadNum();
    PrintString("Enter your filename: ");
    ReadString(filename2, lengthFileName2);
    fileID = Open(filename2, MODE_READWRITE);
    if (fileID == -1)
    {
        PrintString("Failed to open file");
    }
    else
    {
        PrintString("File: ");
        PrintString(filename2);
        PrintString(" open file successfully!\n");
        Write(buffer, length, fileID);
        Close(fileID);
        fileID = Open(filename2, MODE_READ);
        Read(buffer2, length, fileID);
        PrintString("File content: ");
        PrintString(buffer2);
        PrintString("\n copy content successfully!\n");
    }
    Halt();
    return 0;
}