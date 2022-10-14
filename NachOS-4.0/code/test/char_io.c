#include "syscall.h"

int main()
{
    PrintChar(ReadChar());
    Halt();
    return 0;
}