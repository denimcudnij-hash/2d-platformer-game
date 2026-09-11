# Complete Guide: Building 2D Platformer with osdev GCC

## Your osdev GCC Setup

Since you're using GCC configured for OS development, here's how to build this game:

## Option 1: Web Version (Easiest - Works Everywhere)

### Prerequisites
```bash
# Install Emscripten
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh

# Or on your system
sudo apt-get install emscripten  # Ubuntu/Debian
brew install emscripten          # macOS
```

### Build
```bash
cd web/
emcc game.c -o game.js \
    -s WASM=1 \
    -s TOTAL_MEMORY=67108864 \
    -s "EXPORTED_FUNCTIONS=['_main','_on_key_down','_on_key_up']" \
    -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall','cwrap']" \
    -O3

# Run local server
python3 -m http.server 8000

# Open http://localhost:8000/index.html
```

**Result:** Runs in any browser, cross-platform ✅

---

## Option 2: Windows Version from osdev Environment

### If your osdev GCC targets Windows (x86_64-w64-mingw32)

```bash
# Check your compiler target
gcc --version
gcc -dumpmachine

# If output is "x86_64-w64-mingw32", you can compile directly:
cd windows/
gcc -O2 -o game.exe game.c -luser32 -lgdi32 -lxinput
```

### If your osdev GCC targets something else (i686-elf, riscv64, etc.)

You need the MinGW cross-compiler:

```bash
# Install MinGW cross-compiler
sudo apt-get install mingw-w64          # Ubuntu/Debian
sudo pacman -S mingw-w64-gcc            # Arch Linux
brew install mingw-w64                  # macOS

# Check if installed
x86_64-w64-mingw32-gcc --version

# Cross-compile
cd windows/
x86_64-w64-mingw32-gcc -O2 -o game.exe game.c -luser32 -lgdi32 -lxinput

# Now transfer game.exe to Windows and run it
```

---

## Option 3: Using Build Scripts

### On Linux/macOS with osdev setup

```bash
# Make scripts executable
chmod +x build.sh

# Build web version
./build.sh debug web

# Build Windows version (cross-compile)
./build.sh release windows

# Build both
./build.sh release all
```

### On Windows

```bash
build.bat debug web
build.bat release windows
build.bat release all
```

---

## Option 4: Using Makefile (osdev Recommended)

We've included an osdev-specific Makefile:

```bash
cd osdev/

# See available targets
make help

# Build web version
make web

# Build Windows version (native if cross-compiler installed)
make windows

# Cross-compile explicitly
make windows-cross

# Build both
make all

# Clean
make clean
```

---

## Detailed Build Instructions by Platform

### Linux with osdev i686-elf-gcc

```bash
# Check your setup
i686-elf-gcc --version
i686-elf-gcc -dumpmachine  # Output: i686-elf

# Web version (easiest)
cd web/
emcc game.c -o game.js -s WASM=1 -O3
# Open in browser

# Windows version (cross-compile)
cd windows/
x86_64-w64-mingw32-gcc -O2 -o game.exe game.c -luser32 -lgdi32 -lxinput
# Run on Windows or Wine
```

### macOS with osdev GCC

```bash
# Install tools if needed
brew install gcc emscripten mingw-w64

# Web version
cd web/
emcc game.c -o game.js -s WASM=1 -O3
open index.html  # Opens in default browser

# Windows version
cd windows/
x86_64-w64-mingw32-gcc -O2 -o game.exe game.c -luser32 -lgdi32 -lxinput
# Transfer .exe to Windows machine
```

### Windows (with osdev/MinGW-w64 GCC)

```bash
# Check compiler
gcc --version
gcc -dumpmachine  # Output: x86_64-w64-mingw32

# Web version
cd web
emcc game.c -o game.js -s WASM=1 -O3
start index.html  # Opens in default browser

# Windows version
cd windows
gcc -O2 -o game.exe game.c -luser32 -lgdi32 -lxinput
game.exe  # Run immediately
```

---

## Troubleshooting

### Problem: "gcc: command not found"

```bash
# If using osdev GCC with specific target
which i686-elf-gcc
which x86_64-elf-gcc
# Use the full path or add to PATH
export PATH=$PATH:/path/to/osdev/gcc/bin
```

### Problem: "emcc: command not found"

```bash
# Activate Emscripten
source /path/to/emsdk/emsdk_env.sh
# Or install system-wide
sudo apt-get install emscripten
```

### Problem: "x86_64-w64-mingw32-gcc not found"

```bash
# Install MinGW cross-compiler
sudo apt-get install mingw-w64
# Verify
which x86_64-w64-mingw32-gcc
```

### Problem: Linking errors on Windows version

```bash
# Make sure Windows SDK libraries are available
# If cross-compiling, they should be included with mingw-w64

# On Windows native, try with MSVC instead
cl.exe /O2 game.c /link user32.lib gdi32.lib xinput.lib /OUT:game.exe
```

### Problem: WebAssembly not working

```bash
# Check browser console (F12)
# Make sure WASM mime type is correct
# Test local server:
python3 -m http.server 8000
# Don't use file:// protocol
```

---

## Quick Start Commands by Goal

### "I want to play the game now"

```bash
# Web version (instant, cross-platform)
cd web/
emcc game.c -o game.js -s WASM=1 -O3
python3 -m http.server 8000
# Open http://localhost:8000/index.html
```

### "I want a Windows .exe"

```bash
# Native Windows compilation
cd windows/
gcc -O2 -o game.exe game.c -luser32 -lgdi32 -lxinput
game.exe

# Or cross-compile from Linux
cd windows/
x86_64-w64-mingw32-gcc -O2 -o game.exe game.c -luser32 -lgdi32 -lxinput
```

### "I want to modify the game"

```bash
# Both versions share the same core game logic
# Edit either web/game.c or windows/game.c
# The logic is identical, only rendering differs

# For physics/gameplay:
vim web/game.c      # Edit the physics/logic
vim windows/game.c  # Copy changes here too

# Then recompile both versions
```

---

## File Structure

```
2d-platformer-game/
├── README.md                    # Main documentation
├── build.sh                     # Linux/macOS build script
├── build.bat                    # Windows build script
│
├── web/
│   ├── game.c                  # Game logic (Emscripten)
│   ├── game.js                 # JS loader (generated)
│   ├── index.html              # Web page
│   └── BUILD.md                # Web build details
│
├── windows/
│   ├── game.c                  # Game logic (Win32 API)
│   ├── Makefile                # MSVC build
│   ├── Makefile.gcc            # MinGW build
│   └── BUILD.md                # Windows build details
│
└── osdev/
    ├── Makefile                # osdev-specific build
    └── BUILD_OSDEV.md          # This file
```

---

## Your osdev GCC Configuration

To get personalized build instructions, run:

```bash
gcc --version
gcc -dumpmachine
gcc -print-file-name=libc.a
```

Share the output and I can provide exact commands for your setup!

---

## Performance Notes

**Web Version:**
- Runs in browser (no compilation needed each time)
- WebAssembly ensures near-native performance
- ~60 FPS on modern browsers
- Cross-platform (Windows/Mac/Linux/mobile)

**Windows Version:**
- True native Windows executable
- Full performance on any Windows PC
- Xbox controller support
- Vibration feedback
- ~60 FPS guaranteed

---

## Next Steps

1. **Try web version first** (easiest)
2. **Build Windows version** (with cross-compiler if needed)
3. **Modify game.c** (same logic in both versions)
4. **Rebuild with `make all`** or build scripts

Have fun! 🎮
