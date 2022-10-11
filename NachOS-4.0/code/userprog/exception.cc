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
/*
	Copy buffer from User space to Kernel memory space
	Input:  User space address, length of buffer
	Output: Buffer
*/
char *User2System(int virtualAddr, int limit)
{
	int idx;
	int oneChar;
	char *kernelBuffer = NULL;
	kernelBuffer = new char[limit + 1];
	if (kernelBuffer == NULL)
		return kernelBuffer;
	// printf("Copying User to Kernel");
	for (idx = 0; idx < limit; ++idx)
	{
		kernel->machine->ReadMem(virtualAddr + idx, 1, &oneChar);
		kernelBuffer[idx] = oneChar;
		if (oneChar == 0)
			break;
	}
	return kernelBuffer;
}

/*
	Copy buffer from Kernel space to User memory space
	Input:  - address in user space
			- limit of buffer
			- buffer kernel memory
	Output: number of bytes copied
*/
int System2User(int virtualAddr, int len, char *buffer)
{
	if (len < 0)
		return -1;
	if (len == 0)
		return 0;
	int i = 0, oneChar = 0;
	do
	{
		oneChar = (int)buffer[i];
		kernel->machine->WriteMem(virtualAddr + i, 1, oneChar);
		++i;
	} while (i < len && oneChar != 0);
	return i;
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
}
void handleReadNum()
{
	int num = 0;
	num = SysReadNum();							 // system read integer number
	kernel->machine->WriteRegister(2, (int)num); // write the return value to register 2
}
void handlePrintNum()
{
	int num = (int)kernel->machine->ReadRegister(4);
	SysPrintNum(num);
}
void handleReadChar()
{
}
void handlePrintChar()
{
}
void handleRandomNum()
{
	unsigned int res = 0;
	res = SysRandomNum();
	kernel->machine->WriteRegister(2, res);
}
void handleReadString()
{
}
void handlePrintString()
{
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
			DEBUG(dbgSys, "Shutdown, initiated by user program.\n");

			SysHalt();

			ASSERTNOTREACHED();
			break;

		case SC_Add:
			handleAdd();
			/* Modify return point */
			increase_PC();

			return;

			ASSERTNOTREACHED();

			break;
		case SC_ReadNum:
			handleReadNum();
			increase_PC();
			return;

			ASSERTNOTREACHED();
			break;
		case SC_PrintNum:
			handlePrintNum();
			increase_PC();
			return;

			ASSERTNOTREACHED();
			break;
		case SC_ReadChar:
			handleReadChar();
			increase_PC();
			return;

			ASSERTNOTREACHED();
			break;
		case SC_PrintChar:
			handlePrintChar();
			increase_PC();
			return;

			ASSERTNOTREACHED();
			break;
		case SC_RandomNum:
			handleRandomNum();
			increase_PC();
			return;

			ASSERTNOTREACHED();
			break;
		case SC_ReadString:
			handleReadString();
			increase_PC();
			return;

			ASSERTNOTREACHED();
			break;
		case SC_PrintString:
			handlePrintString();
			increase_PC();
			return;

			ASSERTNOTREACHED();
			break;
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
