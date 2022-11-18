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

// ksyscall 2
bool SysCreateFile(char *filename)
{
  bool openSuccess;
  if (strlen(filename) == 0)
  {
    // Filename = "" return -1 in reg 2
    openSuccess = false;
  }
  else if (filename == NULL)
  {
    openSuccess = false;
  }
  else if (!kernel->fileSystem->Create(filename))
  {
    // Fail to create file
    openSuccess = false;
  }
  else
  {
    openSuccess = true;
  }

  return openSuccess;
}
bool SysOpen(char *filename, type)
{
  if (type != 0 && type != 1)
    return -1;
  int id = kernel->fileSystem->Open(filename, type);
  if (id == -1)
    return -1;
  return id;
}

int SysClose(int id) { return kernel->fileSystem->Close(id); }
int SysRead(char *buffer, int size, int fileID)
{
  if (fileID == 0)
  {
    return kernel->synchConsoleIn->GetString(buffer, size);
  }
  return kernel->fileSystem.Read(buffer, size, fileID);
}

int SysWrite(char *buffer, int size, int fileID)
{
  if (fileID == 1)
  {
    return kernel->synchConsoleOut->PutString(buffer, size);
  }
  return kernel->fileSystem->Write(buffer, size, fileID);
}

int SysSeek(int seekPos, int fileId)
{
  if (fileId <= 1)
  {
    return -1;
  }
  return kernel->fileSystem->Seek(seekPos, fileId);
}

int SysRemove(char *fileName)
{
  return kernel->fileSystem->Remove(fileName);
}
#endif /* ! __USERPROG_KSYSCALL_H__ */
