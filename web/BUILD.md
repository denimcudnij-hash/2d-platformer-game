# Build WebAssembly version

## Requirements
- Emscripten SDK (https://emscripten.org)
- Node.js (for testing locally)

## Build Instructions

### 1. Install Emscripten (if not installed)
```bash
git clone https://github.com/emscripten-core/emsdk.git
cd emsdk
./emsdk install latest
./emsdk activate latest
source ./emsdk_env.sh  # On Windows: emsdk_env.bat
```

### 2. Compile C to WebAssembly
```bash
cd web/
emcc game.c -o game.js -s WASM=1 -s TOTAL_MEMORY=67108864 \
    -s "EXPORTED_FUNCTIONS=['_main','_on_key_down','_on_key_up']" \
    -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall','cwrap']" \
    -O3
```

### 3. Run locally with Python
```bash
python3 -m http.server 8000
```

Then open `http://localhost:8000/index.html` in your browser.

### 4. Deploy to web server
Copy the following files to your web server:
- `index.html`
- `game.js`
- `game.wasm` (generated during compilation)

## Controls
- **W** or **⬆️** - Jump
- **A** or **⬅️** - Move Left
- **D** or **➡️** - Move Right
- **R** - Restart (when dead)

## Gameplay
- Collect coins (🪙)
- Pass through checkpoints (✓)
- Reach the goal flag (🚩)
- Avoid spikes (⚠️)
- Complete the level as fast as possible

## Features
- Physics-based movement with gravity
- Moving platforms
- Collision detection
- Score tracking (coins & checkpoints)
- Timer
- Game over and level complete states

## Build Output Files
After compilation, you'll have:
- `game.js` - JavaScript glue code (generated)
- `game.wasm` - Binary WebAssembly module (generated)
- `index.html` - HTML page (already created)
- `game.js` - Game logic loader (already created)