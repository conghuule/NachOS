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
void SysHalt()
{
  kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
  return op1 + op2;
}
// Check character is a number or not
bool isDigit(char ch)
{
  if (ch >= '0' && ch <= '9')
    return true;
  return false;
}
int SysReadNum()
{
  int num = 0;
  char ch;
  bool isNegative = false;
  bool isNumber = true;
  // Ignore space and end line, input while
  while (ch == ' ' || ch == '\n')
  {
    ch = kernel->synchConsoleIn->GetChar();
  }
  // Input number is a negative number
  if (ch == '-')
  {
    isNegative = true;
    ch = kernel->synchConsoleIn->GetChar();
    if (ch == '\n' || ch == ' ')
    {
      return 0;
    }
  }
  if (!isDigit(ch))
  {
    isNumber = false;
  }
  while (ch != '\n' && ch != ' ')
  {
    if (isDigit(ch))
    {
      num = num * 10 + ch - '0';
    }
    ch = kernel->synchConsoleIn->GetChar();
    if (!isDigit(ch))
    {
      isNumber = false;
    }
  }

  if (!isNumber) // If not a number
    return 0;

  if (isNegative) // Change to negative
    num = -num;
  return num;
}
void SysPrintNum(int op1)
{
  // Print Zero
  if (op1 == 0)
  {
    kernel->synchConsoleOut->PutChar('0');
    return;
  }

  // Print Negative Number
  if (op1 < 0)
  {
    kernel->synchConsoleOut->PutChar('-');
    op1 = -op1;
  }
  // Min and Max of Int32 have 10 digits
  char arr[10];
  int i = 0;
  int r = 0;

  // Change number to char array
  while (op1 != 0)
  {
    r = op1 % 10;
    arr[i] = r;
    i++;
    op1 = op1 / 10;
  }

  // Print digits
  for (int j = i - 1; j > -1; --j)
  {
    kernel->synchConsoleOut->PutChar('0' + arr[j]);
  }
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

void PrintString(char *buffer, int length)
{
  for (int i = 0; i < length; i++)
  {
    kernel->synchConsoleOut->PutChar(buffer[i]);
  }
}
#endif /* ! __USERPROG_KSYSCALL_H__ */
