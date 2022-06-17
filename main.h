#ifndef TICTACTOE_MAIN_H
#define TICTACTOE_MAIN_H

#include <SDL.h>
#include <SDL_image.h>
#include <string>

// Constant definitions
#define WINDOW_WIDTH 600
#define WINDOW_HEIGHT 600
#define WINDOW_SIZE Size{WINDOW_WIDTH, WINDOW_HEIGHT}
#define ROWS 3
#define COLUMNS 3

const int MagicSquareNaught[3][3] = {{2, 9, 4}, {7, 5, 3}, {6, 1, 8}};
const int MagicSquareCross[3][3] = {{4, 18, 8}, {14, 10, 6}, {12, 2, 16}};

// Type definitions
enum GameState { PLAYING, WON, TIE };

enum TileState { NONE, NAUGHT, CROSS };

enum MouseButtonState{NOT_PRESSED, PRESSED};
struct MouseState
{
    MouseButtonState leftButton, rightButton, middleButton;
};

// Game state
GameState currentState = PLAYING;
TileState winner = NONE;
bool isRunning = true;
float deltaTime = 0;
int fps = 0;

SDL_Window* window;
SDL_Renderer* renderer;

SDL_Surface* screenshotBuffer;

TileState currentPlayer = NAUGHT;
TileState grid[COLUMNS][ROWS];

// Input state
MouseState currentMouseState = {NOT_PRESSED, NOT_PRESSED, NOT_PRESSED};
MouseState prevMouseState = {NOT_PRESSED, NOT_PRESSED, NOT_PRESSED};

// Function declarations
int InitSDL();
int InitWindow(const std::string& title, int width, int height, bool resizeable);

int InitTTF();

void HandleInput(SDL_KeyboardEvent& key);
void HandleMouseInput(SDL_MouseButtonEvent& button, const MouseButtonState& newState);
bool IsMouseButtonDown(const uint8_t mouseButton);
bool IsMouseButtonPressed(const uint8_t mouseButton);
bool IsMouseButtonReleased(const uint8_t mouseButton);

bool GridFull();
void CheckForWinner(int x, int y);
void Winner(TileState winner);

void TakeScreenshot();
void SaveScreenshot();

void Reset();

int InitRenderer();
void PrepareRender();
void Render();
void RenderGame();
void PresentRender();

void Shutdown();

#endif //TICTACTOE_MAIN_H
