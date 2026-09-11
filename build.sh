#!/bin/bash
# Automated build script for 2D Platformer Game

set -e

echo "========================================"
echo "2D Platformer Game - Build Script"
echo "========================================"
echo ""

# Detect platform
if [[ "$OSTYPE" == "msys" || "$OSTYPE" == "cygwin" || "$OSTYPE" == "win32" ]]; then
    PLATFORM="windows"
elif [[ "$OSTYPE" == "linux-gnu"* ]]; then
    PLATFORM="linux"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    PLATFORM="macos"
else
    PLATFORM="unknown"
fi

echo "Detected platform: $PLATFORM"
echo ""

# Parse arguments
BUILD_TYPE="${1:-debug}"
VERSION="${2:-web}"

if [ "$VERSION" = "all" ]; then
    echo "Building both versions..."
    bash build.sh $BUILD_TYPE web
    echo ""
    bash build.sh $BUILD_TYPE windows
    exit 0
fi

echo "Build type: $BUILD_TYPE"
echo "Version: $VERSION"
echo ""

if [ "$VERSION" = "web" ]; then
    echo "Building WEB version (Emscripten)..."
    
    if ! command -v emcc &> /dev/null; then
        echo "ERROR: Emscripten not found!"
        echo "Install from: https://emscripten.org"
        exit 1
    fi
    
    cd web
    
    echo "Compiling game.c to WebAssembly..."
    emcc game.c -o game.js \
        -s WASM=1 \
        -s TOTAL_MEMORY=67108864 \
        -s "EXPORTED_FUNCTIONS=['_main','_on_key_down','_on_key_up']" \
        -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall','cwrap']" \
        -O3
    
    echo "✓ Web version built successfully!"
    echo "  Files: game.js, game.wasm, index.html"
    echo "  Run: python3 -m http.server 8000"
    echo "  Open: http://localhost:8000/index.html"
    
elif [ "$VERSION" = "windows" ]; then
    echo "Building WINDOWS version..."
    
    cd windows
    
    if [ "$PLATFORM" = "windows" ]; then
        if command -v cl.exe &> /dev/null; then
            echo "Using MSVC compiler..."
            cl.exe /O2 game.c /link user32.lib gdi32.lib xinput.lib /OUT:game.exe
            echo "✓ Windows version built successfully!"
            echo "  File: game.exe"
            echo "  Run: ./game.exe"
        elif command -v gcc &> /dev/null; then
            echo "Using MinGW compiler..."
            gcc -O2 -o game.exe game.c -luser32 -lgdi32 -lxinput
            echo "✓ Windows version built successfully!"
            echo "  File: game.exe"
            echo "  Run: ./game.exe"
        else
            echo "ERROR: No C compiler found!"
            exit 1
        fi
    else
        echo "Cross-compiling to Windows from $PLATFORM..."
        if command -v x86_64-w64-mingw32-gcc &> /dev/null; then
            x86_64-w64-mingw32-gcc -O2 -o game.exe game.c -luser32 -lgdi32 -lxinput
            echo "✓ Windows version cross-compiled successfully!"
            echo "  File: game.exe (Windows binary)"
        else
            echo "ERROR: MinGW cross-compiler not found!"
            echo "Install: sudo apt-get install mingw-w64"
            exit 1
        fi
    fi
    
else
    echo "ERROR: Unknown version '$VERSION'"
    echo "Usage: build.sh [debug|release] [web|windows|all]"
    exit 1
fi

echo ""
echo "========================================"
echo "Build complete!"
echo "========================================"
