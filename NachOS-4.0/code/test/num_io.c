#include "syscall.h"

int main()
{
    PrintNum(ReadNum());
    Halt();
    return 0;
}