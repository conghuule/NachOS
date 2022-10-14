#include "syscall.h"

int main()
{
    unsigned int result = RandomNum();
    PrintNum(result);
    Halt();
    return 0;
}