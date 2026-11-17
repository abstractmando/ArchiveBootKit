#ifndef SYRIAL_SHARED_MEMTOOLS_H
#define SYRIAL_SHARED_MEMTOOLS_H

#include <Uefi.h>

VOID* MapPhysicalMemory(EFI_PHYSICAL_ADDRESS Address, UINTN Size);
VOID  UnmapMemory(VOID* VirtualAddress, UINTN Size);

BOOLEAN ReadPhysMem(EFI_PHYSICAL_ADDRESS Address, VOID* Buffer, UINTN Size);
BOOLEAN WritePhysMem(EFI_PHYSICAL_ADDRESS Address, CONST VOID* Buffer, UINTN Size);

VOID* FindPattern(VOID* Base, UINTN Size, CONST UINT8* Pattern, CONST CHAR8* Mask);

#endif
VOID* PhysToVirt(UINT64 PhysAddr);

// EFI memory mapping functions
VOID* MmMapIoSpace(UINT64 PhysicalAddress, UINTN NumberOfBytes, BOOLEAN Cacheable);
VOID  MmUnmapIoSpace(VOID* VirtualAddress, UINTN NumberOfBytes);

// Memory management functions
VOID* AllocatePool(UINTN Size);
VOID* AllocateZeroPool(UINTN Size);
VOID  FreePool(VOID* Buffer);

// String/memory functions
UINTN StrLen(const CHAR16* Str);
INTN  StrCmp(const CHAR16* Str1, const CHAR16* Str2);
VOID* MemCpy(VOID* Dest, const VOID* Src, UINTN Length);
