#include <Base.h>
#include <Library/BaseLib.h>
#include <Library/MemoryAllocationLib.h>
#include <string.h>
#include <stdlib.h>

// Variabili globali UEFI come puntatori generici
void *gST = NULL;
void *gBS = NULL;
void *gImageHandle = NULL;
void *gRT = NULL;

// GUIDs come array di byte grezzi
const unsigned char gEfiFileInfoGuid[] = {
    0x91, 0x9C, 0x06, 0x23, 0x70, 0xAD, 0xD2, 0x4E,
    0xBB, 0xDA, 0x21, 0x6B, 0x69, 0xE5, 0x6E, 0x04
};

const unsigned char gEfiSimpleFileSystemProtocolGuid[] = {
    0xA8, 0x1D, 0xFB, 0x8F, 0x1A, 0x14, 0x3C, 0xD4,
    0x8E, 0x4D, 0x62, 0xAE, 0x0D, 0x8B, 0x7B, 0x8B
};

const unsigned char gEfiLoadedImageProtocolGuid[] = {
    0x5B, 0x1B, 0x41, 0x86, 0xFA, 0x27, 0xDA, 0x11,
    0xAC, 0xC6, 0x85, 0xA7, 0x0B, 0xD1, 0x9D, 0xA3
};

// Implementazioni standard per le funzioni di memoria UEFI
void *CopyMem(void *Destination, const void *Source, UINTN Length) {
    return memcpy(Destination, Source, Length);
}

void *SetMem(void *Buffer, UINTN Length, UINT8 Value) {
    return memset(Buffer, Value, Length);
}

void *AllocatePool(UINTN Size) {
    return malloc(Size);
}

void *AllocateZeroPool(UINTN Size) {
    return calloc(1, Size);
}

void FreePool(void *Buffer) {
    free(Buffer);
}

// Funzioni XOR se non sono già definite altrove
void XOR_EncryptBuffer(UINT8 *Buffer, UINTN BufferSize, UINT8 Key) {
    for (UINTN i = 0; i < BufferSize; i++) {
        Buffer[i] ^= Key;
    }
}

void XOR_DecryptBuffer(UINT8 *Buffer, UINTN BufferSize, UINT8 Key) {
    XOR_EncryptBuffer(Buffer, BufferSize, Key);
}
