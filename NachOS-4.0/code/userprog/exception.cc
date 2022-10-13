// exception.cc
//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1996 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "main.h"
#include "syscall.h"
#include "ksyscall.h"
//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	For system calls, the following is the calling convention:
//
// 	system call code -- r2
//		arg1 -- r4
//		arg2 -- r5
//		arg3 -- r6
//		arg4 -- r7
//
//	The result of the system call, if any, must be put back into r2.
//
// If you are handling a system call, don't forget to increment the pc
// before returning. (Or else you'll loop making the same system call forever!)
//
//	"which" is the kind of exception.  The list of possible exceptions
//	is in machine.h.
//----------------------------------------------------------------------
void increase_PC()
{
	/* set previous programm counter (debugging only)*/
	kernel->machine->WriteRegister(PrevPCReg, kernel->machine->ReadRegister(PCReg));

	/* set programm counter to next instruction (all Instructions are 4 byte wide)*/
	kernel->machine->WriteRegister(PCReg, kernel->machine->ReadRegister(PCReg) + 4);

	/* set next programm counter for brach execution */
	kernel->machine->WriteRegister(NextPCReg, kernel->machine->ReadRegister(PCReg) + 4);
}

char *User2System(int virtAddr, int limit)
{
	int index;
	int oneChar;
	char *kernelBuf = NULL;
	kernelBuf = new char[limit + 1]; // need for terminal string
	if (kernelBuf == NULL)
		return kernelBuf;
	memset(kernelBuf, 0, limit + 1);
	for (index = 0; index < limit; index++)
	{
		kernel->machine->ReadMem(virtAddr + index, 1, &oneChar);
		kernelBuf[index] = (char)oneChar;
		if (oneChar == 0)
			break;
	}
	return kernelBuf;
}

void System2User(int virtAddr, int len, char *buffer)
{
	if (len > 0)
	{
		int i = 0;
		int oneChar = 0;
		do
		{
			oneChar = (int)buffer[i];
			kernel->machine->WriteMem(virtAddr + i, 1, oneChar);
			i++;
		} while (i < len && oneChar != 0);
	}
}

void handleHalt()
{
	DEBUG(dbgSys, "Shutdown, initiated by user program.\n");
	SysHalt();
	ASSERTNOTREACHED();
}

void handleAdd()
{
	DEBUG(dbgSys, "Add " << kernel->machine->ReadRegister(4) << " + " << kernel->machine->ReadRegister(5) << "\n");

	/* Process SysAdd Systemcall*/
	int result;
	result = SysAdd(/* int op1 */ (int)kernel->machine->ReadRegister(4),
					/* int op2 */ (int)kernel->machine->ReadRegister(5));

	DEBUG(dbgSys, "Add returning with " << result << "\n");
	/* Prepare Result */
	kernel->machine->WriteRegister(2, (int)result);

	increase_PC();
}
void handleReadNum()
{
	int num = 0;
	num = SysReadNum();							 // system read integer number
	kernel->machine->WriteRegister(2, (int)num); // write the return value to register 2

	increase_PC();
}
void handlePrintNum()
{
	int num = (int)kernel->machine->ReadRegister(4);
	SysPrintNum(num);

	increase_PC();
}
void handleReadChar()
{
	char ch = SysReadChar();
	kernel->machine->WriteRegister(2, ch);

	increase_PC();
}
void handlePrintChar()
{
	char ch = (char)kernel->machine->ReadRegister(4);
	SysPrintChar(ch);

	increase_PC();
}
void handleRandomNum()
{
	unsigned int res = 0;
	res = SysRandomNum();
	kernel->machine->WriteRegister(2, res);

	increase_PC();
}

#define MAX_READ_STRING_LENGTH 255
char _stringBuffer[MAX_READ_STRING_LENGTH];
void handleReadString()
{
	int memPtr = kernel->machine->ReadRegister(4); // read address of C-string
	int length = kernel->machine->ReadRegister(5); // read length of C-string
	if (length > MAX_READ_STRING_LENGTH)
	{ // avoid allocating large memory
		DEBUG(dbgSys, "String length exceeds " << MAX_READ_STRING_LENGTH);
		SysHalt();
	}
	char *buffer = SysReadString(length);
	System2User(memPtr, length, buffer);
	delete[] buffer;

	increase_PC();
}
void handlePrintString()
{
	int memPtr = kernel->machine->ReadRegister(4); // read address of C-string
	char *buffer = User2System(memPtr, MAX_READ_STRING_LENGTH + 1);

	SysPrintString(buffer, strlen(buffer));
	delete[] buffer;

	increase_PC();
}

void ExceptionHandler(ExceptionType which)
{
	int type = kernel->machine->ReadRegister(2);

	DEBUG(dbgSys, "Received Exception " << which << " type: " << type << "\n");

	switch (which)
	{
	case NoException:
		kernel->interrupt->setStatus(SystemMode);
		DEBUG(dbgSys, "Returned control to system.\n");
		return;
	case PageFaultException:	// No valid translation found
	case ReadOnlyException:		// Write attempted to page marked
								// "read-only"
	case BusErrorException:		// Translation resulted in an
								// invalid physical address
	case AddressErrorException: // Unaligned reference or one that
								// was beyond the end of the
								// address space
	case OverflowException:		// Integer overflow in add or sub.
	case IllegalInstrException: // Unimplemented or reserved instr.
	case NumExceptionTypes:
		cerr << "Error" << which << ". Halting Nachos\n";
		SysHalt();
		ASSERTNOTREACHED();
	case SyscallException:
		switch (type)
		{
		case SC_Halt:
			handleHalt();
			return;

		case SC_Add:
			handleAdd();
			return;
		case SC_ReadNum:
			handleReadNum();
			return;
		case SC_PrintNum:
			handlePrintNum();
			return;
		case SC_ReadChar:
			handleReadChar();
			return;
		case SC_PrintChar:
			handlePrintChar();
			return;
		case SC_RandomNum:
			handleRandomNum();
			return;
		case SC_ReadString:
			handleReadString();
			return;
		case SC_PrintString:
			handlePrintString();
			return;
		default:
			cerr << "Unexpected system call " << type << "\n";
			break;
		}
		break;
	default:
		cerr << "Unexpected user mode exception" << (int)which << "\n";
		break;
	}
	ASSERTNOTREACHED();
}
