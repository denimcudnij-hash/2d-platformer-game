@echo off
REM Automated build script for 2D Platformer Game (Windows batch)

echo ========================================
echo 2D Platformer Game - Build Script
echo ========================================
echo.

setlocal enabledelayedexpansion

set BUILD_TYPE=%1
set VERSION=%2

if "%BUILD_TYPE%"=="" set BUILD_TYPE=debug
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
    echo Building WEB version - Emscripten
    where emcc >nul 2>nul
    if errorlevel 1 (
        echo ERROR: Emscripten not found!
        exit /b 1
    )
    cd web
    echo Compiling game.c to WebAssembly...
    emcc game.c -o game.js -s WASM=1 -s TOTAL_MEMORY=67108864 -O3
    echo Web version built successfully!
    cd ..
) else if "%VERSION%"=="windows" (
    echo Building WINDOWS version...
    cd windows
    where cl.exe >nul 2>nul
    if not errorlevel 1 (
        echo Using MSVC compiler...
        cl.exe /O2 game.c /link user32.lib gdi32.lib xinput.lib /OUT:game.exe
        if not errorlevel 1 (
            echo Windows version built successfully!
            cd ..
            goto :EOF
        )
    )
    where gcc >nul 2>nul
    if errorlevel 1 (
        echo ERROR: Neither MSVC nor MinGW found!
        cd ..
        exit /b 1
    )
    echo Using MinGW compiler...
    gcc -O2 -o game.exe game.c -luser32 -lgdi32 -lxinput
    if errorlevel 1 (
        echo ERROR: Compilation failed!
        cd ..
        exit /b 1
    )
    echo Windows version built successfully!
    cd ..
) else (
    echo ERROR: Unknown version %VERSION%
    exit /b 1
)

echo.
echo ========================================
echo Build complete - windows/game.exe
echo ========================================
