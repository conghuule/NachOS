/**************************************************************
 *
 * userprog/ksyscall.h
 *
 * Kernel interface for systemcalls
 *
 * by Marcus Voelp  (c) Universitaet Karlsruhe
 *
 **************************************************************/

#ifndef __USERPROG_KSYSCALL_H__
#define __USERPROG_KSYSCALL_H__

#include "kernel.h"
#include "synchconsole.h"
#include "machine.h"
#include "filesys.h"
#include "stdint.h"

#define INT32_MAX_DIV_10 214748364

void SysHalt()
{
  kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
  return op1 + op2;
}

bool isSpace(char ch)
{
  return ch == ' ' || ch == '\n' || ch == '\t';
}
int SysReadNum()
{
  int num = 0;
  bool isNegative = false;
  bool isFirstChar = true;
  bool isInt = true;

  char ch = kernel->synchConsoleIn->GetChar();
  while (isSpace(ch))
  {
    ch = kernel->synchConsoleIn->GetChar();
  }

  while (!isSpace(ch))
  {
    if (isInt)
    {
      if (isFirstChar && ch == '-')
      {
        isNegative = true;
        isFirstChar = false;
      }
      else if (isFirstChar && ch == '+')
      {
        isFirstChar = false;
        ch = kernel->synchConsoleIn->GetChar();
        continue;
      }
      else
      {
        if (ch >= '0' && ch <= '9')
        {
          if ((isNegative && (num < INT32_MAX_DIV_10 || (num == INT32_MAX_DIV_10 && ch <= '8'))) || (!isNegative && (num < INT32_MAX_DIV_10 || (num == INT32_MAX_DIV_10 && ch <= '7'))))
          {
            num = num * 10 + (ch - '0');
          }
          else
          {
            num = 0;
            isInt = false;
          }
        }
        else
        {
          num = 0;
          isInt = false;
        }

        if (isFirstChar)
        {
          isFirstChar = false;
        }
      }
    }
    ch = kernel->synchConsoleIn->GetChar();
  }
  return isNegative ? num * -1 : num;
}
void SysPrintNum(int num)
{
  if (num == 0)
  {
    kernel->synchConsoleOut->PutChar('0');
    return;
  }

  if (num < 0)
  {
    kernel->synchConsoleOut->PutChar('-');
    if (num == INT32_MIN)
    {
      for (int i = 0; i < 10; i++)
        kernel->synchConsoleOut->PutChar("2147483648"[i]);
      return;
    }
    else
    {
      num = -num;
    }
  }
  char arr[10];
  int i = 0;
  while (num)
  {
    arr[i++] = num % 10;
    num /= 10;
  }
  for (int j = i - 1; j >= 0; j--)
    kernel->synchConsoleOut->PutChar(arr[j] + '0');
}
char SysReadChar()
{
  return kernel->synchConsoleIn->GetChar();
}
void SysPrintChar(char ch)
{
  kernel->synchConsoleOut->PutChar(ch);
}
unsigned int SysRandomNum()
{
  RandomInit(time(0));
  return RandomNumber();
}
char *SysReadString(int length)
{
  char *buffer = new char[length + 1];
  for (int i = 0; i < length; i++)
  {
    buffer[i] = SysReadChar();
  }
  buffer[length] = '\0';
  return buffer;
}

void SysPrintString(char *buffer, int length)
{
  for (int i = 0; i < length; i++)
  {
    kernel->synchConsoleOut->PutChar(buffer[i]);
  }
}
#endif /* ! __USERPROG_KSYSCALL_H__ */
