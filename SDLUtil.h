#ifndef TICTACTOE_SDLUTIL_H
#define TICTACTOE_SDLUTIL_H

#include <string>
#include <SDL.h>

struct Size
{
    int x;
    int y;
};

void PrintSDLError(const std::string& functionName);
void PrintSDLError(const std::string& functionName, const std::string& message);
void PrintTTFError(const std::string& functionName);
void PrintTTFError(const std::string& functionName, const std::string& message);

void RenderTexture(SDL_Renderer* renderer, const std::string& texture, int x, int y, int width, int height);
void RenderTexture(SDL_Renderer* renderer, const std::string& texture, int x, int y);
void RenderTextureCenter(SDL_Renderer* renderer, const std::string& texture, int xOffset, int yOffset, const Size& windowSize);

#endif //TICTACTOE_SDLUTIL_H
