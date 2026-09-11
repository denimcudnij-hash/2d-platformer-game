# Build Windows version

## Requirements
- Microsoft Visual C++ Compiler (MSVC) or MinGW-w64
- Windows SDK (for XInput and GDI)
- Make (for Makefile)

## Build Instructions

### Option 1: Using MSVC (Recommended for Windows)

```bash
cd windows/
cl.exe /O2 game.c /link user32.lib gdi32.lib xinput.lib /OUT:game.exe
```

### Option 2: Using MinGW-w64

```bash
cd windows/
gcc -O2 -o game.exe game.c -luser32 -lgdi32 -lxinput
```

### Option 3: Using Makefile (Recommended)

```bash
cd windows/
make
```

Or for release build:
```bash
make release
```

Or clean build artifacts:
```bash
make clean
```

## Run the Game

```bash
./game.exe
```

## Controls
- **W** or **↑** - Jump
- **A** or **←** - Move Left
- **D** or **→** - Move Right
- **R** - Restart (when dead)

## XInput Controller Support
The game supports Xbox controllers via XInput:
- **A Button** - Jump
- **D-Pad Left/Right** - Move
- **Left Analog Stick** - Move
- **Vibration** - On jump, coin collection, and checkpoints

## Gameplay
- Collect coins (🪙) - 6 coins total
- Pass through checkpoints (✓) - 3 checkpoints
- Reach the goal flag (🚩)
- Avoid spikes (⚠️) - 3 spike traps
- Complete the level as fast as possible

## Features
- Physics-based movement with gravity
- Moving and static platforms
- Collision detection
- Score tracking (coins & checkpoints)
- Timer in seconds
- Game over and level complete states
- Controller vibration feedback
- Double-buffered rendering (no flicker)
- 60 FPS game loop

## Troubleshooting

### Compiler not found
- **MSVC**: Install Visual Studio with C++ tools
- **MinGW**: Download from https://www.mingw-w64.org/
- **Make**: Install from http://gnuwin32.sourceforge.net/packages/make.htm

### Linking errors
- Ensure Windows SDK is installed
- Check that xinput.lib, user32.lib, and gdi32.lib are in your system paths

### Controller not working
- Connect Xbox controller or compatible gamepad
- Test in Windows Game Controller settings
- Game supports up to 4 controllers (currently uses controller 0)
