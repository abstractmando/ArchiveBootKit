#include <Uefi.h>
#include "BotStartup.h"

extern void *gST;

EFI_STATUS EFIAPI BootEntry(IN EFI_HANDLE ImageHandle, IN EFI_SYSTEM_TABLE* SystemTable) {
    gST = SystemTable;
    return KernelMain(SystemTable);
}
