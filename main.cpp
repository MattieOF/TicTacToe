#include "SDLUtil.h"
#include "main.h"
#include "Textures.h"
#include "Fonts.h"

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>
#include <ctime>
#include <sstream>

int main(int argc, char *argv[]) {
    if (InitSDL()) return 1;
    std::cout << "TicTacToe on " << SDL_GetPlatform() << std::endl;
    if (InitWindow("TicTacToe", WINDOW_WIDTH, WINDOW_HEIGHT, false)) return 1;
    if (InitRenderer()) return 1;
    if (InitTTF()) return 1;

//    LoadTexture(renderer, "Resources/Textures/catgirl.png", "catgirl");
//    LoadTexture(renderer, "Resources/Textures/flynn.png", "flynn");
//    LoadTexture(renderer, "Resources/Textures/lmao.png", "lmao");
    LoadTexture(renderer, "Resources/Textures/Cross.png", "cross");
    LoadTexture(renderer, "Resources/Textures/Naught.png", "naught");

    LoadFont("Resources/Fonts/OpenSans-Regular.ttf", 35, "opensans");
    LoadFont("Resources/Fonts/OpenSans-Regular.ttf", 20, "opensansmed");
    LoadFont("Resources/Fonts/OpenSans-Regular.ttf", 10, "opensanssmall");

    // Pre-rendered text
    AddTexture("titletext", RenderText(renderer, "TicTacToe", "opensans", SDL_Color{0, 0, 0}));
    AddTexture("naughtwins", RenderText(renderer, "NAUGHT WINS!", "opensans", SDL_Color{0, 0, 0}));
    AddTexture("crosswins", RenderText(renderer, "CROSS WINS!", "opensans", SDL_Color{0, 0, 0}));
    AddTexture("tie", RenderText(renderer, "TIE!", "opensans", SDL_Color{0, 0, 0}));
    AddTexture("restart", RenderText(renderer, "Press R to restart", "opensansmed", SDL_Color{0, 0, 0}));
    AddTexture("ss", RenderText(renderer, "Press S to screenshot board", "opensansmed", SDL_Color{0, 0, 0}));
    AddTexture("quit", RenderText(renderer, "Press ESC to exit the game", "opensansmed", SDL_Color{0, 0, 0}));

    while (isRunning)
    {
        uint64_t start = SDL_GetPerformanceCounter();

        prevMouseState = currentMouseState;
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
                case SDL_KEYDOWN:
                    HandleInput(event.key);
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    HandleMouseInput(event.button, PRESSED);
                    break;
                case SDL_MOUSEBUTTONUP:
                    HandleMouseInput(event.button, NOT_PRESSED);
                    break;
                case SDL_QUIT:
                    isRunning = false;
                    break;
                default:
                    break;
            }
        }

        PrepareRender();
        Render();
        PresentRender();

        uint64_t end = SDL_GetPerformanceCounter();
        deltaTime = (end - start) / (float)SDL_GetPerformanceFrequency();
    }

    Shutdown();

    std::cin.get();
    return 0;
}

int InitSDL()
{
    if (SDL_Init(SDL_INIT_EVERYTHING))
    {
        PrintSDLError("Initialising SDL");
        return 1;
    } else return 0;
}

int InitWindow(const std::string &title, int width, int height, bool resizeable)
{
    window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              width, height, SDL_WINDOW_SHOWN | (resizeable ? SDL_WINDOW_RESIZABLE : 0));
    if (!window)
    {
        PrintSDLError("Creating SDL window");
        Shutdown();
        return 1;
    } else return 0;
}

int InitRenderer()
{
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "best");

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        PrintSDLError("Creating SDL renderer");
        Shutdown();
        return 1;
    } else return 0;
}

void PrepareRender()
{
    // SDL_SetRenderDrawColor(renderer, 160, 32, 240, SDL_ALPHA_OPAQUE);
    SDL_SetRenderDrawColor(renderer, 225, 225, 225, SDL_ALPHA_OPAQUE);
    SDL_RenderClear(renderer);
}

void Render()
{
    switch (currentState)
    {
        case PLAYING:
            RenderGame();
            break;
        case WON:
            RenderTextureCenter(renderer, winner == NAUGHT ? "naughtwins" : "crosswins", 0, 0, WINDOW_SIZE);
            RenderTextureCenter(renderer, "restart", 0, 40, WINDOW_SIZE);
            RenderTextureCenter(renderer, "ss", 0, 60, WINDOW_SIZE);
            RenderTextureCenter(renderer, "quit", 0, 80, WINDOW_SIZE);
            break;
        case TIE:
            RenderTextureCenter(renderer, "tie", 0, 0, WINDOW_SIZE);
            RenderTextureCenter(renderer, "restart", 0, 40, WINDOW_SIZE);
            RenderTextureCenter(renderer, "ss", 0, 60, WINDOW_SIZE);
            RenderTextureCenter(renderer, "quit", 0, 80, WINDOW_SIZE);
            break;
    }

    // This should refresh the FPS once per second
    static float fpsRefreshTimer = 0;
    fpsRefreshTimer += deltaTime;
    if (fpsRefreshTimer >= 1)
    {
        fps = (int) (1.0f / deltaTime);
        fpsRefreshTimer = 0;
    }
    DrawText(renderer, "FPS: " + std::to_string(fps), "opensanssmall", SDL_Color{0, 0, 0, 255}, 1, 1);
}

void PresentRender()
{
    SDL_RenderPresent(renderer);
}

void HandleInput(SDL_KeyboardEvent &key)
{
    switch(key.keysym.scancode)
    {
        case SDL_SCANCODE_ESCAPE:
            isRunning = false;
            break;
        case SDL_SCANCODE_R:
            Reset();
            break;
        case SDL_SCANCODE_S:
            SaveScreenshot();
            break;
        default:
            break;
    }
}



void Shutdown()
{
    std::cout << "Quitting...\n";
    DestroyTextures();
    SDL_FreeSurface(screenshotBuffer);
    DestroyFonts();
    if (renderer) SDL_DestroyRenderer(renderer);
    if (window) SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();
}

int InitTTF()
{
    if (TTF_Init())
    {
        PrintTTFError("Initialising TTF");
        return 1;
    } else return 0;
}

void Reset()
{
    for (auto & i : grid)
        for (auto & j : i)
            j = NONE;

    currentPlayer = NAUGHT;
    currentState = PLAYING;
}

void HandleMouseInput(SDL_MouseButtonEvent &button, const MouseButtonState& newState)
{
    if (button.button == SDL_BUTTON_LEFT)
        currentMouseState.leftButton = newState;
    else if (button.button == SDL_BUTTON_RIGHT)
        currentMouseState.rightButton = newState;
    else if (button.button == SDL_BUTTON_MIDDLE)
        currentMouseState.middleButton = newState;
}

// Input functions
// Messy code that could be much better, but I didn't really want to write an
// input system today
// TODO: Maybe clean this up
bool IsMouseButtonDown(const uint8_t mouseButton)
{
    switch(mouseButton)
    {
        case SDL_BUTTON_LEFT:
            return currentMouseState.leftButton == PRESSED && prevMouseState.leftButton == NOT_PRESSED;
        case SDL_BUTTON_RIGHT:
            return currentMouseState.rightButton == PRESSED && prevMouseState.rightButton == NOT_PRESSED;
        case SDL_BUTTON_MIDDLE:
            return currentMouseState.middleButton == PRESSED && prevMouseState.middleButton == NOT_PRESSED;
        default:
            return false;
    }
}

bool IsMouseButtonPressed(const uint8_t mouseButton)
{
    switch(mouseButton)
    {
        case SDL_BUTTON_LEFT:
            return currentMouseState.leftButton == PRESSED;
        case SDL_BUTTON_RIGHT:
            return currentMouseState.rightButton == PRESSED;
        case SDL_BUTTON_MIDDLE:
            return currentMouseState.middleButton == PRESSED;
        default:
            return false;
    }
}

bool IsMouseButtonReleased(const uint8_t mouseButton)
{
    switch(mouseButton)
    {
        case SDL_BUTTON_LEFT:
            return currentMouseState.leftButton == NOT_PRESSED && prevMouseState.leftButton == PRESSED;
        case SDL_BUTTON_RIGHT:
            return currentMouseState.rightButton == NOT_PRESSED && prevMouseState.rightButton == PRESSED;
        case SDL_BUTTON_MIDDLE:
            return currentMouseState.middleButton == NOT_PRESSED && prevMouseState.middleButton == PRESSED;
        default:
            return false;
    }
}

void CheckForWinner(int x, int y)
{
    // Check horizontal
    int sum = 0;
    for (int i = 0; i < ROWS; i++)
    {
        if (grid[x][i] == CROSS) sum += MagicSquareCross[x][i];
        else if (grid[x][i] == NAUGHT) sum += MagicSquareNaught[x][i];
    }
    if (sum == 15)
    {
        Winner(NAUGHT);
        return;
    } else if (sum == 30)
    {
        Winner(CROSS);
        return;
    }

    // Check vertical
    sum = 0;
    for (int i = 0; i < COLUMNS; i++)
    {
        if (grid[i][y] == CROSS) sum += MagicSquareCross[i][y];
        else if (grid[i][y] == NAUGHT) sum += MagicSquareNaught[i][y];
    }
    if (sum == 15)
    {
        Winner(NAUGHT);
        return;
    } else if (sum == 30)
    {
        Winner(CROSS);
        return;
    }

    // Check horizontals
    sum = 0;
    for (int i = 0; i < COLUMNS; i++)
    {
        if (grid[i][i] == CROSS) sum += MagicSquareCross[i][i];
        else if (grid[i][i] == NAUGHT) sum += MagicSquareNaught[i][i];
    }
    if (sum == 15)
    {
        Winner(NAUGHT);
        return;
    } else if (sum == 30)
    {
        Winner(CROSS);
        return;
    }

    sum = 0;
    int j = 0;
    for (int i = COLUMNS - 1; i >= 0; i--)
    {
        if (grid[i][j] == CROSS) sum += MagicSquareCross[i][j];
        else if (grid[i][j] == NAUGHT) sum += MagicSquareNaught[i][j];
        j++;
    }
    if (sum == 15)
    {
        Winner(NAUGHT);
        return;
    } else if (sum == 30)
    {
        Winner(CROSS);
        return;
    }
}

void Winner(TileState winner)
{
    if (winner == CROSS)
        std::cout << "Cross wins.\n";
    else
        std::cout << "Naught wins.\n";

    ::winner = winner;
    currentState = WON;
}

bool GridFull()
{
    for (auto & i : grid)
        for (auto & j : i)
            if (j == NONE) return false;

    return true;
}

void RenderGame()
{
    bool shouldScreenshot = false;

    SDL_Point mousePos;
    SDL_GetMouseState(&mousePos.x, &mousePos.y);

    SDL_FRect currentRect;
    float sizeX = WINDOW_WIDTH - 50, sizeY = WINDOW_HEIGHT - 30;
    float xStep = (sizeX - 50) / COLUMNS;
    float yStep = (sizeY - 30) / ROWS;

    currentRect.x = 50;
    currentRect.y = 50;
    currentRect.w = xStep;
    currentRect.h = yStep;
    for (int i = 0; i < COLUMNS; ++i)
    {
        for (int j = 0; j < ROWS; ++j)
        {
            SDL_Rect rect = {(int)currentRect.x, (int)currentRect.y, (int)currentRect.w, (int)currentRect.h};
            if (SDL_PointInRect(&mousePos, &rect))
            {
                if (IsMouseButtonDown(SDL_BUTTON_LEFT) && grid[i][j] == NONE)
                {
                    // Set value on grid to be current player
                    grid[i][j] = currentPlayer;

                    CheckForWinner(i, j);

                    // If playing but the grid is full, it's a tie
                    if (currentState == PLAYING && GridFull())
                        currentState = TIE;

                    if (currentState == WON) shouldScreenshot = true;

                    // Flip current player
                    currentPlayer = (currentPlayer == NAUGHT ? CROSS : NAUGHT);
                }

                SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
                SDL_RenderFillRectF(renderer, &currentRect);
            }

            // Magic square debug
            // DrawText(renderer, std::to_string(MagicSquare[i][j]), "opensans", SDL_Color{0, 0, 0, 255}, rect.x, rect.y);

            if (grid[i][j] != NONE)
                SDL_RenderCopy(renderer, GetTexture(grid[i][j] == NAUGHT ? "naught" : "cross"), nullptr, &rect);

            SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
            SDL_RenderDrawRectF(renderer, &currentRect);
            currentRect.y += yStep;
        }
        currentRect.y = 50;
        currentRect.x += xStep;

        RenderTexture(renderer, "titletext", 50, 0);

        if (shouldScreenshot) TakeScreenshot();
    }
}

void TakeScreenshot()
{
    // https://stackoverflow.com/questions/22315980/sdl2-c-taking-a-screenshot
    if (screenshotBuffer != nullptr) SDL_FreeSurface(screenshotBuffer);
    screenshotBuffer = SDL_CreateRGBSurface(0, WINDOW_WIDTH, WINDOW_HEIGHT, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
    SDL_RenderReadPixels(renderer, nullptr, SDL_PIXELFORMAT_ARGB8888, screenshotBuffer->pixels, screenshotBuffer->pitch);
}

void SaveScreenshot()
{
    time_t now = time(nullptr);
    tm *localTime = localtime(&now);
    std::stringstream ss;
    ss << (1900 + localTime->tm_year) << localTime->tm_mon << localTime->tm_mday << "_" << localTime->tm_hour << localTime->tm_min << localTime->tm_sec;

    std::cout << "Saving screenshot of board as " << ss.str() << ".png\n";
    IMG_SavePNG(screenshotBuffer, (ss.str() + ".png").c_str());
}
