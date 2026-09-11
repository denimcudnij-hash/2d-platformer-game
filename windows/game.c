#include <windows.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <xinput.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "xinput.lib")

// Constants
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define PLAYER_WIDTH 32
#define PLAYER_HEIGHT 48
#define GRAVITY 0.6f
#define JUMP_POWER 15.0f
#define MOVE_SPEED 5.0f
#define MAX_PLATFORMS 20
#define MAX_COINS 30
#define MAX_SPIKES 15
#define MAX_CHECKPOINTS 5

// Game states
#define GAME_STATE_PLAYING 0
#define GAME_STATE_FINISHED 1
#define GAME_STATE_DEAD 2

// Structures
typedef struct {
    float x, y;
    float vx, vy;
    int on_ground;
    int facing_right;
    int coins_collected;
    int checkpoints_passed;
} Player;

typedef struct {
    float x, y;
    float width, height;
    int moving;
    float move_speed;
    float move_range;
    float start_x;
} Platform;

typedef struct {
    float x, y;
    float width, height;
    int collected;
} Coin;

typedef struct {
    float x, y;
    float width, height;
} Spike;

typedef struct {
    float x, y;
    int reached;
} Checkpoint;

typedef struct {
    float x, y;
    float width, height;
} Goal;

typedef struct {
    Player player;
    Platform platforms[MAX_PLATFORMS];
    int platform_count;
    Coin coins[MAX_COINS];
    int coin_count;
    Spike spikes[MAX_SPIKES];
    int spike_count;
    Checkpoint checkpoints[MAX_CHECKPOINTS];
    int checkpoint_count;
    Goal goal;
    int state;
    int timer;
    int total_timer;
    int keys_pressed[256];
    HDC backbuffer;
    HBITMAP backbuffer_bitmap;
    HBITMAP old_bitmap;
} GameState;

// Global variables
GameState game;
HWND hwnd;
int game_running = 1;

// Function prototypes
void init_game();
void update_game();
void render_game();
void draw_rect(HDC hdc, int x, int y, int w, int h, int r, int g, int b);
void draw_text_win(HDC hdc, int x, int y, const char* text, int r, int g, int b);
void cleanup_game();

// Initialize game
void init_game() {
    // Player setup
    game.player.x = 50.0f;
    game.player.y = 400.0f;
    game.player.vx = 0.0f;
    game.player.vy = 0.0f;
    game.player.on_ground = 0;
    game.player.facing_right = 1;
    game.player.coins_collected = 0;
    game.player.checkpoints_passed = 0;

    game.state = GAME_STATE_PLAYING;
    game.timer = 0;
    game.total_timer = 0;

    // Initialize key state
    memset(game.keys_pressed, 0, sizeof(game.keys_pressed));

    // Platforms
    game.platform_count = 0;

    // Ground
    game.platforms[game.platform_count].x = 0;
    game.platforms[game.platform_count].y = 550;
    game.platforms[game.platform_count].width = 800;
    game.platforms[game.platform_count].height = 50;
    game.platforms[game.platform_count].moving = 0;
    game.platform_count++;

    // Static platforms
    game.platforms[game.platform_count].x = 150;
    game.platforms[game.platform_count].y = 480;
    game.platforms[game.platform_count].width = 150;
    game.platforms[game.platform_count].height = 20;
    game.platforms[game.platform_count].moving = 0;
    game.platform_count++;

    game.platforms[game.platform_count].x = 400;
    game.platforms[game.platform_count].y = 420;
    game.platforms[game.platform_count].width = 150;
    game.platforms[game.platform_count].height = 20;
    game.platforms[game.platform_count].moving = 0;
    game.platform_count++;

    game.platforms[game.platform_count].x = 650;
    game.platforms[game.platform_count].y = 360;
    game.platforms[game.platform_count].width = 150;
    game.platforms[game.platform_count].height = 20;
    game.platforms[game.platform_count].moving = 0;
    game.platform_count++;

    // Moving platform
    game.platforms[game.platform_count].x = 200;
    game.platforms[game.platform_count].y = 300;
    game.platforms[game.platform_count].width = 100;
    game.platforms[game.platform_count].height = 20;
    game.platforms[game.platform_count].moving = 1;
    game.platforms[game.platform_count].move_speed = 2.0f;
    game.platforms[game.platform_count].move_range = 150;
    game.platforms[game.platform_count].start_x = 200;
    game.platform_count++;

    // More static platforms
    game.platforms[game.platform_count].x = 500;
    game.platforms[game.platform_count].y = 250;
    game.platforms[game.platform_count].width = 150;
    game.platforms[game.platform_count].height = 20;
    game.platforms[game.platform_count].moving = 0;
    game.platform_count++;

    game.platforms[game.platform_count].x = 100;
    game.platforms[game.platform_count].y = 180;
    game.platforms[game.platform_count].width = 150;
    game.platforms[game.platform_count].height = 20;
    game.platforms[game.platform_count].moving = 0;
    game.platform_count++;

    // Coins
    game.coin_count = 0;
    game.coins[game.coin_count].x = 180; game.coins[game.coin_count].y = 450; game.coins[game.coin_count].width = 16; game.coins[game.coin_count].height = 16; game.coins[game.coin_count].collected = 0; game.coin_count++;
    game.coins[game.coin_count].x = 430; game.coins[game.coin_count].y = 390; game.coins[game.coin_count].width = 16; game.coins[game.coin_count].height = 16; game.coins[game.coin_count].collected = 0; game.coin_count++;
    game.coins[game.coin_count].x = 680; game.coins[game.coin_count].y = 330; game.coins[game.coin_count].width = 16; game.coins[game.coin_count].height = 16; game.coins[game.coin_count].collected = 0; game.coin_count++;
    game.coins[game.coin_count].x = 250; game.coins[game.coin_count].y = 270; game.coins[game.coin_count].width = 16; game.coins[game.coin_count].height = 16; game.coins[game.coin_count].collected = 0; game.coin_count++;
    game.coins[game.coin_count].x = 550; game.coins[game.coin_count].y = 220; game.coins[game.coin_count].width = 16; game.coins[game.coin_count].height = 16; game.coins[game.coin_count].collected = 0; game.coin_count++;
    game.coins[game.coin_count].x = 150; game.coins[game.coin_count].y = 150; game.coins[game.coin_count].width = 16; game.coins[game.coin_count].height = 16; game.coins[game.coin_count].collected = 0; game.coin_count++;

    // Spikes
    game.spike_count = 0;
    game.spikes[game.spike_count].x = 350; game.spikes[game.spike_count].y = 530; game.spikes[game.spike_count].width = 30; game.spikes[game.spike_count].height = 20; game.spike_count++;
    game.spikes[game.spike_count].x = 600; game.spikes[game.spike_count].y = 530; game.spikes[game.spike_count].width = 30; game.spikes[game.spike_count].height = 20; game.spike_count++;
    game.spikes[game.spike_count].x = 300; game.spikes[game.spike_count].y = 280; game.spikes[game.spike_count].width = 30; game.spikes[game.spike_count].height = 20; game.spike_count++;

    // Checkpoints
    game.checkpoint_count = 0;
    game.checkpoints[game.checkpoint_count].x = 150; game.checkpoints[game.checkpoint_count].y = 450; game.checkpoints[game.checkpoint_count].reached = 0; game.checkpoint_count++;
    game.checkpoints[game.checkpoint_count].x = 400; game.checkpoints[game.checkpoint_count].y = 390; game.checkpoints[game.checkpoint_count].reached = 0; game.checkpoint_count++;
    game.checkpoints[game.checkpoint_count].x = 650; game.checkpoints[game.checkpoint_count].y = 330; game.checkpoints[game.checkpoint_count].reached = 0; game.checkpoint_count++;

    // Goal
    game.goal.x = 700;
    game.goal.y = 100;
    game.goal.width = 60;
    game.goal.height = 60;
}

// Update game logic
void update_game() {
    if (game.state == GAME_STATE_FINISHED) {
        return;
    }

    game.total_timer++;
    game.timer = game.total_timer / 60; // Convert to seconds

    // Player horizontal movement
    game.player.vx = 0;
    if (game.keys_pressed['A'] || game.keys_pressed[VK_LEFT]) {
        game.player.vx = -MOVE_SPEED;
        game.player.facing_right = 0;
    }
    if (game.keys_pressed['D'] || game.keys_pressed[VK_RIGHT]) {
        game.player.vx = MOVE_SPEED;
        game.player.facing_right = 1;
    }

    // Apply gravity
    game.player.vy += GRAVITY;
    if (game.player.vy > 20.0f) {
        game.player.vy = 20.0f; // Terminal velocity
    }

    // Update player position
    game.player.x += game.player.vx;
    game.player.y += game.player.vy;

    // Screen bounds
    if (game.player.x < 0) game.player.x = 0;
    if (game.player.x + PLAYER_WIDTH > SCREEN_WIDTH) {
        game.player.x = SCREEN_WIDTH - PLAYER_WIDTH;
    }

    // Check if player fell off screen
    if (game.player.y > SCREEN_HEIGHT) {
        game.state = GAME_STATE_DEAD;
        game.player.x = 50.0f;
        game.player.y = 400.0f;
        game.player.vy = 0;
        game.player.coins_collected = 0;
        return;
    }

    // Reset on-ground flag
    game.player.on_ground = 0;

    // Collision detection with platforms
    for (int i = 0; i < game.platform_count; i++) {
        Platform* p = &game.platforms[i];

        // Update moving platforms
        if (p->moving) {
            p->x += p->move_speed;
            if (p->x < p->start_x || p->x + p->width > p->start_x + p->move_range) {
                p->move_speed *= -1;
            }
        }

        // Check collision
        if (game.player.x + PLAYER_WIDTH > p->x &&
            game.player.x < p->x + p->width &&
            game.player.y + PLAYER_HEIGHT > p->y &&
            game.player.y < p->y + p->height) {

            // Landing on top of platform
            if (game.player.vy > 0 && game.player.y + PLAYER_HEIGHT - game.player.vy <= p->y + 5) {
                game.player.y = p->y - PLAYER_HEIGHT;
                game.player.vy = 0;
                game.player.on_ground = 1;
            }
            // Hit bottom
            else if (game.player.vy < 0) {
                game.player.y = p->y + p->height;
                game.player.vy = 0;
            }
            // Hit sides
            else if (game.player.vx > 0) {
                game.player.x = p->x - PLAYER_WIDTH;
            } else if (game.player.vx < 0) {
                game.player.x = p->x + p->width;
            }
        }
    }

    // Coin collection
    for (int i = 0; i < game.coin_count; i++) {
        if (!game.coins[i].collected &&
            game.player.x + PLAYER_WIDTH > game.coins[i].x &&
            game.player.x < game.coins[i].x + game.coins[i].width &&
            game.player.y + PLAYER_HEIGHT > game.coins[i].y &&
            game.player.y < game.coins[i].y + game.coins[i].height) {
            game.coins[i].collected = 1;
            game.player.coins_collected++;

            // Vibration on coin collection
            XINPUT_VIBRATION vibration;
            vibration.wLeftMotorSpeed = 30000;
            vibration.wRightMotorSpeed = 30000;
            XInputSetState(0, &vibration);
        }
    }

    // Spike collision (death)
    for (int i = 0; i < game.spike_count; i++) {
        if (game.player.x + PLAYER_WIDTH > game.spikes[i].x &&
            game.player.x < game.spikes[i].x + game.spikes[i].width &&
            game.player.y + PLAYER_HEIGHT > game.spikes[i].y &&
            game.player.y < game.spikes[i].y + game.spikes[i].height) {
            game.state = GAME_STATE_DEAD;
            game.player.x = 50.0f;
            game.player.y = 400.0f;
            game.player.vy = 0;
            game.player.coins_collected = 0;
            return;
        }
    }

    // Checkpoint collision
    for (int i = 0; i < game.checkpoint_count; i++) {
        if (!game.checkpoints[i].reached &&
            game.player.x + PLAYER_WIDTH > game.checkpoints[i].x - 15 &&
            game.player.x < game.checkpoints[i].x + 15 &&
            game.player.y + PLAYER_HEIGHT > game.checkpoints[i].y - 15 &&
            game.player.y < game.checkpoints[i].y + 15) {
            game.checkpoints[i].reached = 1;
            game.player.checkpoints_passed++;

            // Vibration on checkpoint
            XINPUT_VIBRATION vibration;
            vibration.wLeftMotorSpeed = 20000;
            vibration.wRightMotorSpeed = 20000;
            XInputSetState(0, &vibration);
        }
    }

    // Goal collision
    if (game.player.x + PLAYER_WIDTH > game.goal.x &&
        game.player.x < game.goal.x + game.goal.width &&
        game.player.y + PLAYER_HEIGHT > game.goal.y &&
        game.player.y < game.goal.y + game.goal.height) {
        game.state = GAME_STATE_FINISHED;
    }

    // Jumping
    if ((game.keys_pressed['W'] || game.keys_pressed[VK_UP]) && game.player.on_ground) {
        game.player.vy = -JUMP_POWER;
        game.player.on_ground = 0;

        // Vibration on jump
        XINPUT_VIBRATION vibration;
        vibration.wLeftMotorSpeed = 15000;
        vibration.wRightMotorSpeed = 15000;
        XInputSetState(0, &vibration);
    }
}

// Render game
void render_game() {
    HDC hdc = GetDC(hwnd);
    HDC backbuffer = CreateCompatibleDC(hdc);
    HBITMAP backbuffer_bitmap = CreateCompatibleBitmap(hdc, SCREEN_WIDTH, SCREEN_HEIGHT);
    HBITMAP old_bitmap = SelectObject(backbuffer, backbuffer_bitmap);

    // Clear background (sky blue)
    HBRUSH bg_brush = CreateSolidBrush(RGB(135, 206, 235));
    RECT rect = {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    FillRect(backbuffer, &rect, bg_brush);
    DeleteObject(bg_brush);

    // Draw platforms
    for (int i = 0; i < game.platform_count; i++) {
        Platform* p = &game.platforms[i];
        if (p->moving) {
            draw_rect(backbuffer, (int)p->x, (int)p->y, (int)p->width, (int)p->height, 139, 69, 19); // Brown
        } else {
            draw_rect(backbuffer, (int)p->x, (int)p->y, (int)p->width, (int)p->height, 34, 139, 34); // Green
        }
    }

    // Draw coins
    for (int i = 0; i < game.coin_count; i++) {
        if (!game.coins[i].collected) {
            draw_rect(backbuffer, (int)game.coins[i].x, (int)game.coins[i].y, (int)game.coins[i].width, (int)game.coins[i].height, 255, 215, 0); // Gold
        }
    }

    // Draw spikes
    for (int i = 0; i < game.spike_count; i++) {
        draw_rect(backbuffer, (int)game.spikes[i].x, (int)game.spikes[i].y, (int)game.spikes[i].width, (int)game.spikes[i].height, 255, 0, 0); // Red
    }

    // Draw checkpoints
    for (int i = 0; i < game.checkpoint_count; i++) {
        int color = game.checkpoints[i].reached ? 0 : 255;
        draw_rect(backbuffer, (int)game.checkpoints[i].x - 15, (int)game.checkpoints[i].y - 15, 30, 30, 0, color, 0);
    }

    // Draw goal flag
    draw_rect(backbuffer, (int)game.goal.x, (int)game.goal.y, (int)game.goal.width, (int)game.goal.height, 255, 165, 0); // Orange

    // Draw player
    int player_color = game.state == GAME_STATE_DEAD ? 128 : 0;
    draw_rect(backbuffer, (int)game.player.x, (int)game.player.y, PLAYER_WIDTH, PLAYER_HEIGHT, player_color, 0, 255); // Blue

    // Draw UI
    char buffer[256];
    sprintf_s(buffer, sizeof(buffer), "Coins: %d", game.player.coins_collected);
    draw_text_win(backbuffer, 10, 10, buffer, 0, 0, 0);

    sprintf_s(buffer, sizeof(buffer), "Checkpoints: %d", game.player.checkpoints_passed);
    draw_text_win(backbuffer, 10, 30, buffer, 0, 0, 0);

    sprintf_s(buffer, sizeof(buffer), "Time: %ds", game.timer);
    draw_text_win(backbuffer, 10, 50, buffer, 0, 0, 0);

    if (game.state == GAME_STATE_FINISHED) {
        draw_text_win(backbuffer, 300, 300, "LEVEL COMPLETE!", 0, 128, 0);
        sprintf_s(buffer, sizeof(buffer), "Time: %ds | Coins: %d | Checkpoints: %d", game.timer, game.player.coins_collected, game.player.checkpoints_passed);
        draw_text_win(backbuffer, 200, 350, buffer, 0, 0, 0);
    }

    if (game.state == GAME_STATE_DEAD) {
        draw_text_win(backbuffer, 250, 300, "DEAD - Press R to restart", 255, 0, 0);
    }

    // Blit backbuffer to screen
    BitBlt(hdc, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, backbuffer, 0, 0, SRCCOPY);

    // Cleanup
    SelectObject(backbuffer, old_bitmap);
    DeleteObject(backbuffer_bitmap);
    DeleteDC(backbuffer);
    ReleaseDC(hwnd, hdc);
}

// Draw rectangle
void draw_rect(HDC hdc, int x, int y, int w, int h, int r, int g, int b) {
    HBRUSH brush = CreateSolidBrush(RGB(r, g, b));
    RECT rect = {x, y, x + w, y + h};
    FillRect(hdc, &rect, brush);
    DeleteObject(brush);
}

// Draw text
void draw_text_win(HDC hdc, int x, int y, const char* text, int r, int g, int b) {
    SetTextColor(hdc, RGB(r, g, b));
    SetBkMode(hdc, TRANSPARENT);
    TextOutA(hdc, x, y, text, strlen(text));
}

// Cleanup
void cleanup_game() {
    // Turn off vibration
    XINPUT_VIBRATION vibration;
    vibration.wLeftMotorSpeed = 0;
    vibration.wRightMotorSpeed = 0;
    XInputSetState(0, &vibration);
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_KEYDOWN:
        game.keys_pressed[wParam] = 1;
        if (wParam == 'R' && game.state == GAME_STATE_DEAD) {
            init_game();
        }
        break;
    case WM_KEYUP:
        game.keys_pressed[wParam] = 0;
        break;
    case WM_PAINT:
        render_game();
        ValidateRect(hWnd, NULL);
        break;
    case WM_DESTROY:
        game_running = 0;
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Main entry point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSA wc = {0};
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = "PlatformerGameClass";
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClassA(&wc);

    hwnd = CreateWindowA("PlatformerGameClass", "2D Platformer Game",
                         WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
                         SCREEN_WIDTH + 16, SCREEN_HEIGHT + 38, NULL, NULL, hInstance, NULL);

    if (!hwnd) {
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    init_game();

    MSG msg = {0};
    LARGE_INTEGER frequency, last_time, current_time;
    QueryPerformanceFrequency(&frequency);
    QueryPerformanceCounter(&last_time);

    // Game loop
    while (game_running) {
        QueryPerformanceCounter(&current_time);
        double delta_time = (double)(current_time.QuadPart - last_time.QuadPart) / frequency.QuadPart;

        // Aim for 60 FPS
        if (delta_time >= 1.0 / 60.0) {
            if (PeekMessageA(&msg, NULL, 0, 0, PM_REMOVE)) {
                if (msg.message == WM_QUIT) {
                    game_running = 0;
                    break;
                }
                TranslateMessage(&msg);
                DispatchMessageA(&msg);
            }

            update_game();
            render_game();

            last_time = current_time;
        }
    }

    cleanup_game();
    DestroyWindow(hwnd);

    return 0;
}
