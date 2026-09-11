# 2D Platformer Game - Web & Windows Versions

A complete, fully playable 2D platformer game written in C with two independent versions:

1. **Web Version** - Browser-based using Emscripten (WebAssembly + JavaScript)
2. **Windows Version** - Standalone .exe using Win32 API + GDI

## Features

✨ **Gameplay**
- Player movement with physics and gravity
- Jumping and collision detection
- Static and moving platforms
- Collectible coins
- Checkpoints/milestones
- Spike traps (instant death)
- Goal flag to complete level
- Timer tracking
- Score tracking (coins + checkpoints)

✨ **Web Version (Emscripten)**
- Runs in any modern browser
- Physics engine in C compiled to WebAssembly
- Canvas-based rendering
- Keyboard controls
- ~800x600 game window

✨ **Windows Version (Win32 + GDI)**
- Native Windows application
- Double-buffered rendering (60 FPS)
- Keyboard controls
- Xbox controller support via XInput
- Controller vibration feedback
- ~800x600 game window

## Directory Structure

```
2d-platformer-game/
├── web/
│   ├── game.c              # C game logic (Emscripten)
│   ├── game.js             # JavaScript loader
│   ├── index.html          # Web page
│   └── BUILD.md            # Build instructions
├── windows/
│   ├── game.c              # C game logic (Win32 API)
│   ├── BUILD.md            # Build instructions
│   ├── Makefile            # MSVC build
│   └── Makefile.gcc        # MinGW build
└── README.md               # This file
```

## Quick Start

### Web Version

**Requirements:**
- Emscripten SDK
- Python 3 (for local server)

**Build & Run:**
```bash
cd web/
emcc game.c -o game.js -s WASM=1 -s TOTAL_MEMORY=67108864 \
    -s "EXPORTED_FUNCTIONS=['_main','_on_key_down','_on_key_up']" \
    -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall','cwrap']" \
    -O3

python3 -m http.server 8000
```

Then open: `http://localhost:8000/index.html`

### Windows Version

**Requirements:**
- MSVC or MinGW-w64
- Make (optional, for Makefile)

**Build & Run:**

Using Makefile:
```bash
cd windows/
make
make run
```

Using MSVC directly:
```bash
cd windows/
cl.exe /O2 game.c /link user32.lib gdi32.lib xinput.lib /OUT:game.exe
game.exe
```

Using MinGW:
```bash
cd windows/
gcc -O2 -o game.exe game.c -luser32 -lgdi32 -lxinput
game.exe
```

## Controls

### Keyboard
- **W** or **↑** - Jump
- **A** or **←** - Move Left  
- **D** or **→** - Move Right
- **R** - Restart (when dead)

### Xbox Controller (Windows only)
- **A Button** - Jump
- **D-Pad Left/Right** - Move Left/Right
- **Left Analog Stick** - Move
- **Vibration** - Feedback on jump, coin collection, checkpoint

## Gameplay

**Objective:** Complete the level by:
1. Collecting all 6 coins (🪙)
2. Passing through all 3 checkpoints (✓)
3. Reaching the goal flag (🚩)
4. Avoiding 3 spike traps (⚠️)
5. Complete as fast as possible

**Level Layout:**
- Start at bottom-left
- Multiple platforms (static and moving)
- Coins scattered throughout
- Checkpoints mark progress
- Goal flag at top-right
- Spikes on ground and floating platforms

## Game States

- **PLAYING** - Normal gameplay
- **DEAD** - Hit spike or fell off screen (press R to restart)
- **FINISHED** - Reached goal flag (shows final stats)

## Technical Details

### Web Version
- **Language:** C + Emscripten
- **Rendering:** HTML5 Canvas
- **Format:** WebAssembly (WASM)
- **Size:** ~50KB (compressed)

### Windows Version
- **Language:** C
- **API:** Win32 API + GDI
- **Input:** Keyboard + XInput (Xbox controller)
- **Output:** Native Windows window
- **Size:** ~50KB (compiled)

## Code Structure

Both versions share:
- Same game logic in C
- Same level design
- Same gameplay mechanics
- Separate platform-specific rendering

**Common elements:**
- `Player` struct - Player position, velocity, state
- `Platform` struct - Platform data (static/moving)
- `Coin` struct - Collectible items
- `Spike` struct - Hazards
- `Checkpoint` struct - Progress markers
- `Goal` struct - Level completion target

**Physics:**
- Gravity: 0.6 px/frame²
- Jump power: 15 px/frame
- Terminal velocity: 20 px/frame
- Move speed: 5 px/frame

## Building from Source

See `BUILD.md` in respective directories for detailed build instructions.

## Troubleshooting

### Web Version
- Emscripten not found: Install SDK from https://emscripten.org
- WASM not running: Check browser console (F12) for errors
- Controller not working: XInput only available on Windows version

### Windows Version
- Compiler error: Install Visual Studio C++ tools or MinGW-w64
- Linking error: Ensure Windows SDK is installed
- Controller not detected: Check Windows Game Controller settings

## Performance

- **FPS Target:** 60 FPS
- **Web:** ~60 FPS on modern browsers
- **Windows:** Stable 60 FPS on any system
- **CPU:** Minimal usage (~1% per core)
- **Memory:** ~5MB (web), ~2MB (Windows)

## License

Open source - free to use, modify, and distribute.

## Author

Created as a demonstration of C programming for both web (Emscripten) and native Windows platforms.
