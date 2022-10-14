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

void SysHalt()
{
  kernel->interrupt->Halt();
}

int SysAdd(int op1, int op2)
{
  return op1 + op2;
}
bool isDigit(char ch)
{
  if (ch >= '0' && ch <= '9')
  {
    return true;
  }
  return false;
}
bool isWhitespaceCharacter(char ch)
{
  if (ch == ' ' || ch == '\n' || ch == '\t')
  {
    return true;
  };
  return false;
}
int SysReadNum()
{
  int num = 0;
  bool isNegative = false;
  bool isNumber = true;
  char ch = kernel->synchConsoleIn->GetChar();
  // Skip Whitespace Character
  while (isWhitespaceCharacter(ch))
  {
    ch = kernel->synchConsoleIn->GetChar();
  }
  if (ch == '-')
  {
    isNegative = true;
    ch = kernel->synchConsoleIn->GetChar();
    if (isWhitespaceCharacter(ch))
    {
      return 0;
    }
  }

  if (!isDigit(ch))
  {
    isNumber = false;
  }
  while (!isWhitespaceCharacter(ch))
  {
    num = num * 10 + ch - '0';
    ch = kernel->synchConsoleIn->GetChar();
    if (!isDigit(ch) && !isWhitespaceCharacter(ch))
    {
      isNumber = false;
    }
  }
  if (!isNumber)
  {
    return 0;
  }
  if (isNegative)
  {
    num = -num;
  }
  return num;
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
    num = -num;
  }
  char arr[10];
  int i = 0;
  while (num != 0)
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
