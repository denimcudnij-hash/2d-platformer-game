# Makefile for osdev environments
# Supports both native and cross-compilation

.PHONY: all web windows windows-cross clean help

# Detect compiler
CC ?= gcc
TARGET := $(shell $(CC) -dumpmachine)

# Build flags
CFLAGS_COMMON = -Wall -Wextra -O2
CFLAGS_WEB = -s WASM=1
LDFLAGS_WINDOWS = -luser32 -lgdi32 -lxinput

help:
	@echo "2D Platformer Game - osdev Build"
	@echo ""
	@echo "Targets:"
	@echo "  make web              - Build web version (requires Emscripten)"
	@echo "  make windows          - Build Windows version (native or cross-compile)"
	@echo "  make windows-cross    - Cross-compile for Windows from Linux"
	@echo "  make clean            - Clean build artifacts"
	@echo ""
	@echo "Detected target: $(TARGET)"
	@echo "Compiler: $(CC)"

# Web version
web:
	@echo "Building web version..."
	@cd web && emcc game.c -o game.js \
		-s WASM=1 \
		-s TOTAL_MEMORY=67108864 \
		-s "EXPORTED_FUNCTIONS=['_main','_on_key_down','_on_key_up']" \
		-s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall','cwrap']" \
		-O3
	@echo "✓ Web version complete: web/game.wasm"

# Windows version (native or cross-compile)
windows: check-os
ifeq ($(filter %mingw%, $(TARGET)), %mingw%)
	@echo "Building Windows version (native)..."
	@cd windows && $(CC) $(CFLAGS_COMMON) -o game.exe game.c $(LDFLAGS_WINDOWS)
else ifneq ($(filter %-w64-mingw32, $(TARGET)), )
	@echo "Building Windows version (cross-compiled)..."
	@cd windows && $(CC) $(CFLAGS_COMMON) -o game.exe game.c $(LDFLAGS_WINDOWS)
else
	@echo "WARNING: Not a Windows target ($(TARGET))"
	@echo "Attempting anyway..."
	@cd windows && $(CC) $(CFLAGS_COMMON) -o game.exe game.c $(LDFLAGS_WINDOWS)
endif
	@echo "✓ Windows version complete: windows/game.exe"

# Cross-compile for Windows (explicit)
windows-cross:
	@echo "Cross-compiling for Windows from $(TARGET)..."
	@command -v x86_64-w64-mingw32-gcc >/dev/null 2>&1 || \
		(echo "ERROR: mingw-w64 cross-compiler not found"; exit 1)
	@cd windows && x86_64-w64-mingw32-gcc $(CFLAGS_COMMON) -o game.exe game.c $(LDFLAGS_WINDOWS)
	@echo "✓ Windows version cross-compiled: windows/game.exe"

check-os:
	@echo "Target system: $(TARGET)"

# Clean
clean:
	@echo "Cleaning..."
	@rm -f web/game.js web/game.wasm web/*.map
	@rm -f windows/game.exe windows/game.o
	@echo "✓ Clean complete"

all: web windows
	@echo ""
	@echo "Build complete! Both versions ready."
