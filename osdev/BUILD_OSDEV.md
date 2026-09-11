# Build instructions for osdev GCC environment

## Environment Setup for osdev GCC

If you have GCC configured for OS development, you can still build the Windows version using **cross-compilation**.

### Option 1: Using osdev GCC for x86-64 OS

Your osdev GCC toolchain is designed for bare-metal or OS-level development. The platformer game requires standard C library and system calls that won't be available in a bare-metal environment.

To use osdev GCC for this project, you need:

1. **Target-specific GCC** (e.g., `i686-elf-gcc` or `x86_64-elf-gcc`)
2. **Newlib or similar C library** compiled for your target
3. **Bootloader and kernel** (if building standalone OS)

### Option 2: Cross-compile for Windows (Recommended for osdev setup)

If you want to build a Windows binary from your osdev environment:

```bash
# Install MinGW cross-compiler
sudo apt-get install mingw-w64  # Ubuntu/Debian
sudo pacman -S mingw-w64-gcc    # Arch Linux
brew install mingw-w64          # macOS

# Cross-compile
cd windows/
x86_64-w64-mingw32-gcc -O2 -o game.exe game.c -luser32 -lgdi32 -lxinput

# Transfer game.exe to Windows and run
```

### Option 3: Use your osdev environment's native toolchain

If your osdev GCC uses Newlib:

```bash
# First, check your GCC target
${CC} --version

# If it's i686-elf-gcc or similar, you need a custom build
# You'll need to:
# 1. Implement syscalls for your OS
# 2. Link against your OS's libc
# 3. Create a bootable image
```

## Quick Build for osdev Setup

For quick testing with standard GCC:

```bash
# Build Windows version (cross-compile if needed)
cd windows/
gcc -O2 -o game.exe game.c -luser32 -lgdi32 -lxinput

# Or with osdev cross-compiler (if it supports Windows target)
x86_64-w64-mingw32-gcc -O2 -o game.exe game.c -luser32 -lgdi32 -lxinput
```

## Building for Your OS (Advanced)

If you want to port this game to your custom OS:

1. **Modify `windows/game.c`:**
   - Replace WinAPI calls with your OS's API
   - Replace XInput with your input system
   - Replace GDI with your graphics system

2. **Example structure:**
   ```c
   #ifdef __YOUR_OS__
   // Your OS-specific includes
   #else
   // Standard Windows includes
   #endif
   ```

3. **Recompile with your osdev toolchain:**
   ```bash
   ${CC} -O2 -o game game.c --target=your-os
   ```

## Testing

### Web Version (Linux/osdev host)
```bash
cd web/
emcc game.c -o game.js -s WASM=1 -O3
python3 -m http.server 8000
# Open http://localhost:8000/index.html
```

### Windows Version (on Windows or via cross-compilation)
```bash
cd windows/
# See options above for your setup
game.exe
```

## Your osdev GCC Configuration

Please share your GCC target:
```bash
${CC} --version
${CC} -dumpmachine
```

Then I can provide more specific build instructions for your environment!
