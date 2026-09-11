// Game canvas and context
let canvas;
let ctx;

// Module for Emscripten
let Module = {
    canvas: null,
    onRuntimeInitialized: onEmscriptenReady
};

// Called when Emscripten is ready
function onEmscriptenReady() {
    console.log('Emscripten initialized');
    Module.canvas = canvas;
    
    // Initialize game
    Module._main();
    
    // Show game, hide loading
    document.getElementById('loadingContainer').style.display = 'none';
    document.getElementById('gameContainer').style.display = 'block';
}

// Initialize canvas and listeners
function initializeGame() {
    canvas = document.getElementById('gameCanvas');
    ctx = canvas.getContext('2d');

    // Keyboard event listeners
    document.addEventListener('keydown', function(e) {
        const key = e.key.charCodeAt(0);
        Module._on_key_down(key);
    });

    document.addEventListener('keyup', function(e) {
        const key = e.key.charCodeAt(0);
        Module._on_key_up(key);
    });

    console.log('Game initialized');
}

// Draw rectangle function (called from C code via EM_ASM)
function drawRect(x, y, w, h, r, g, b) {
    ctx.fillStyle = `rgb(${r},${g},${b})`;
    ctx.fillRect(x, y, w, h);
}

// Draw text function (called from C code via EM_ASM)
function drawText(x, text, r, g, b, y) {
    ctx.fillStyle = `rgb(${r},${g},${b})`;
    ctx.font = 'bold 16px Arial';
    ctx.fillText(text, x, y);
}

// Initialize when page loads
window.addEventListener('load', initializeGame);

// Handle visibility to pause game when tab is not focused
document.addEventListener('visibilitychange', function() {
    if (document.hidden) {
        console.log('Game paused (tab not visible)');
    } else {
        console.log('Game resumed');
    }
});
