#include "syscall.h"

int main()
{
    int a[100];
    int length, order;
    int i;
    PrintString("Enter length of array (0 <= length <= 100): ");
    length = ReadNum();
    while (length < 0 || length > 100)
    {
        PrintString("Please enter valid length (0 <= length <= 100)\n");
        PrintString("Enter length of array (0 <= length <= 100): ");
        length = ReadNum();
    };
    for (i = 0; i < length; i++)
    {
        PrintString("Enter a[");
        PrintNum(i);
        PrintString("]: ");
        a[i] = ReadNum();
    }

    PrintString("Enter sort order (1: increase, 2: decrease): ");
    order = ReadNum();
    while (order != 1 && order != 2)
    {
        PrintString("Please enter sort order 1 or 2\n");
        PrintString("Enter sort order (1: increase, 2: decrease): ");
        order = ReadNum();
    };

    if (order == 1)
    {
        int temp;
        int m = 0;
        for (m = 0; m < length - 1; m++)
        {
            int k;
            for (k = 0; k < length - m - 1; k++)
            {
                if (a[k] > a[k + 1])
                {
                    temp = a[k];
                    a[k] = a[k + 1];
                    a[k + 1] = temp;
                }
            }
        }
    }
    else if (order == 2)
    {
        int temp;
        int m = 0;
        for (m = 0; m < length - 1; m++)
        {
            int k;
            for (k = 0; k < length - m - 1; k++)
            {
                if (a[k] < a[k + 1])
                {
                    temp = a[k];
                    a[k] = a[k + 1];
                    a[k + 1] = temp;
                }
            }
        }
    }

    PrintString("Sorted array: ");
    i = 0;
    for (i = 0; i < length; i++)
    {
        PrintNum(a[i]);
        PrintChar(' ');
    }
    Halt();
}
