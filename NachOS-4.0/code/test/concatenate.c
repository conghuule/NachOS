#include "syscall.h"
char filename1[256];
char filename2[256];
char filename3[256];
#define MODE_READWRITE 0
#define MODE_READ 1
int main()
{
    int lengthFileName1 = 0;
    int lengthFileName2 = 0;
    int lengthFileName3 = 0;
    int fileID;
    char buffer[100];
    char buffer2[100];
    char buffer3[100];
    int length = 0;
    int curPos = 0;
    // Read file 1
    PrintString("Filename length must be <= 255\n");
    PrintString("Filename 1 length: ");
    lengthFileName1 = ReadNum();
    PrintString("Enter your filename: ");
    ReadString(filename1, lengthFileName1);
    fileID = Open(filename1, MODE_READ);
    if (fileID == -1)
    {
        PrintString("Failed to open file");
        return -1;
    }
    else
    {

        Read(buffer, 50, fileID);
        while (buffer[length] != '\0')
            length++;
        Close(fileID);
    }

    // Write to file 3
    PrintString("Concatenate filename length: ");
    lengthFileName3 = ReadNum();
    PrintString("Enter your filename: ");
    ReadString(filename3, lengthFileName3);
    fileID = Open(filename3, MODE_READWRITE);
    if (fileID == -1)
    {
        PrintString("Failed to open file");
        return -1;
    }
    else
    {
        Write(buffer, length, fileID);
        Close(fileID);
    }
    curPos = length;

    // Read file 2
    PrintString("Filename 2 length: ");
    lengthFileName2 = ReadNum();
    PrintString("Enter your filename: ");
    ReadString(filename2, lengthFileName2);
    fileID = Open(filename2, MODE_READ);
    if (fileID == -1)
    {
        PrintString("Failed to open file");
        return -1;
    }
    else
    {
        Read(buffer2, 50, fileID);
        length = 0;
        while (buffer2[length] != '\0')
            length++;
        Close(fileID);
    }

    // Write to file 3
    fileID = Open(filename3, MODE_READWRITE);
    if (fileID == -1)
    {
        PrintString("Failed to open file");
        return -1;
    }
    else
    {
        Seek(curPos, fileID);
        Write(buffer2, length, fileID);
        Close(fileID);
    }
    Halt();
    return 0;
}