#include "syscall.h"

char a[256];
int main()
{
    PrintString("String length: (<= 255):\n");
    ReadString(a, ReadNum());
    PrintString(a);
    return 0;
}