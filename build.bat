@echo off
REM Automated build script for 2D Platformer Game (Windows batch)

echo ========================================
echo 2D Platformer Game - Build Script
echo ========================================
echo.

setlocal enabledelayedexpansion

set BUILD_TYPE=%1
set VERSION=%2

if "%BUILD_TYPE%"==" " set BUILD_TYPE=debug
if "%VERSION%"=="" set VERSION=web

echo Build type: %BUILD_TYPE%
echo Version: %VERSION%
echo.

if "%VERSION%"=="all" (
    echo Building both versions...
    call build.bat %BUILD_TYPE% web
    call build.bat %BUILD_TYPE% windows
    goto :EOF
)

if "%VERSION%"=="web" (
    echo Building WEB version (Emscripten)...
    
    where emcc >nul 2>nul
    if errorlevel 1 (
        echo ERROR: Emscripten not found!
        echo Install from: https://emscripten.org
        exit /b 1
    )
    
    cd web
    
    echo Compiling game.c to WebAssembly...
    call emcc game.c -o game.js ^
        -s WASM=1 ^
        -s TOTAL_MEMORY=67108864 ^
        -s "EXPORTED_FUNCTIONS=['_main','_on_key_down','_on_key_up']" ^
        -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall','cwrap']" ^
        -O3
    
    echo Web version built successfully!
    echo Files: game.js, game.wasm, index.html
    echo Run: python -m http.server 8000
    echo Open: http://localhost:8000/index.html
) else if "%VERSION%"=="windows" (
    echo Building WINDOWS version...
    
    cd windows
    
    where cl.exe >nul 2>nul
    if errorlevel 1 (
        echo Using MinGW compiler...
        gcc -O2 -o game.exe game.c -luser32 -lgdi32 -lxinput
    ) else (
        echo Using MSVC compiler...
        cl.exe /O2 game.c /link user32.lib gdi32.lib xinput.lib /OUT:game.exe
    )
    
    echo Windows version built successfully!
    echo File: game.exe
    echo Run: game.exe
) else (
    echo ERROR: Unknown version '%VERSION%'
    echo Usage: build.bat [debug^|release] [web^|windows^|all]
    exit /b 1
)

echo.
echo ========================================
echo Build complete!
echo ========================================
