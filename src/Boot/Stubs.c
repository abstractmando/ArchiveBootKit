#include <Uefi.h>
#include <stddef.h>
#include <string.h>

// EFI String functions
EFI_STATUS EFIAPI StrCmp(const CHAR16* Str1, const CHAR16* Str2) {
    return EFI_SUCCESS;
}

EFI_STATUS EFIAPI StrnCmp(const CHAR16* Str1, const CHAR16* Str2, UINTN Length) {
    return EFI_SUCCESS;
}

UINTN EFIAPI StrLen(const CHAR16* Str) {
    return 0;
}

EFI_STATUS EFIAPI StrCpy(CHAR16* Dest, const CHAR16* Src) {
    return EFI_SUCCESS;
}

EFI_STATUS EFIAPI ExecuteShellcode(UINT8* Code, UINTN Size) {
    return EFI_SUCCESS;
}

EFI_SYSTEM_TABLE* EFIAPI GetSystemTable() {
    return NULL;
}

VOID* EFIAPI PhysToVirt(VOID* PhysicalAddress) {
    return PhysicalAddress;
}

UINT64 EFIAPI GetCurrentTimeMs() {
    return 0;
}

EFI_STATUS EFIAPI EncryptFilesystem(const CHAR16* Path) {
    return EFI_SUCCESS;
}

// Standard C functions
void* memcpy(void* dest, const void* src, size_t count) {
    unsigned char* d = (unsigned char*)dest;
    const unsigned char* s = (const unsigned char*)src;
    while(count--) *d++ = *s++;
    return dest;
}

void* memset(void* ptr, int value, size_t num) {
    unsigned char* p = (unsigned char*)ptr;
    while(num--) *p++ = (unsigned char)value;
    return ptr;
}

void* malloc(size_t size) { return NULL; }
void* calloc(size_t nmemb, size_t size) { return NULL; }
void free(void* ptr) {}
size_t strlen(const char* str) { return 0; }
