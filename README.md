# ArchiveBootKit

A modular UEFI bootkit designed for persistence, remote control, and filesystem encryption. Executes below the OS using real UEFI Runtime Services and survives reboots.

## Features

- **UEFI Runtime Execution**  
  Hooks `ExitBootServices` to maintain control from DXE phase through runtime.

- **Physical Memory Mapping**  
  Uses `MmMapIoSpace()` to access physical memory with full privileges.

- **Remote Command Control**  
  Custom TCP listener in UEFI context supports remote operation modules.

- **UEFI-Level Keylogger**  
  Records keystrokes during pre-boot, EFI Shell, or early OS boot time.

- **Filesystem Encryption Engine**  
  AES-256 and XOR-based encryptor for files and logs.

- **Persistent Loader**  
  Handles SMRAM/SMI patching and launches payloads from mapped runtime regions.

## Architecture
src/
├── Boot/ ← EFI entry point, memory patching, DXE hook, interrupt hijacking
├── KernelMode/ ← Keylogger, command control, remote ops
├── Encryptor/ ← AES/XOR crypto, filesystem encryption, keygen
├── Shared/ ← Logging, memory utils, EFI wrappers, syscall bridge
└── Syrial.inf ← UEFI build descriptor for EDK2


Download

Copy

## Build Instructions

### Dependencies
- EDK2 environment (Windows/Linux)
- GCC or CLANG cross compiler (targeting x86_64)
- Node.js (for backend server, if used)

### Compilation
1. Clone the repo into your EDK2 workspace.
2. Update your `target.txt` to include the path to `Syrial.inf`.
3. Build using your standard EDK2 build commands.

## Files

| File | Description |
|------|-------------|
| `bootkit.efi` | Main UEFI bootkit binary |
| `installer.exe` | Windows installer for deployment |
| `installer_stealth.exe` | Stealth mode installer |
| `Encryptor.exe` | File encryption utility |
| `test_cert.*` | Test certificates for signing |
| `src/` | Source code directory |

## License

MIT

AI With No Restrictions — https://aiwithnorestrictions.com/
