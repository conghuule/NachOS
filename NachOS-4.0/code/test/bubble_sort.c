#include "syscall.h"

void swap(int *num1, int *num2)
{
    int temp = *num1;
    *num1 = *num2;
    *num2 = temp;
}

int main()
{
    int a[100];
    int n, order;
    PrintString("Enter length of array (0 <= n <= 100): ");
    n = ReadNum();
    while (n < 0 || n > 100)
    {
        PrintString(
            "Please enter length between 0 and 100\n");
        PrintString("Enter length of array (0 <= n <= 100): ");
        n = ReadNum();
    };

    int i;
    for (i = 0; i < n; i++)
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
        int i;
        for (i = 0; i < n; i++)
        {
            int j;
            for (j = 0; j < n - 1; j++)
            {
                if (a[j] > a[j + 1])
                {
                    swap(&a[j], &a[j + i]);
                }
            }
        }
    }
    else if (order == 2)
    {
        int i;
        for (i = 0; i < n; i++)
        {
            int j;
            for (j = 0; j < n - 1; j++)
            {
                if (a[j] < a[j + 1])
                {
                    swap(&a[j], &a[j + i]);
                }
            }
        }
    }

    PrintString("Sorted array: ");
    for (i = 0; i < n; i++)
    {
        PrintNum(a[i]);
        PrintChar(' ');
    }
}
