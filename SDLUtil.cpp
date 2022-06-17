#include "SDLUtil.h"
#include "Textures.h"

#include <SDL.h>
#include <SDL_ttf.h>
#include <iostream>

void PrintSDLError(const std::string &functionName)
{
    std::cout << "SDL Error: " << functionName << ": " << SDL_GetError() << "\n";
}

void PrintSDLError(const std::string &functionName, const std::string &message)
{
    PrintSDLError(functionName);
    std::cout << message << "\n";
}

void PrintTTFError(const std::string &functionName)
{
    std::cout << "TTF Error: " << functionName << ": " << TTF_GetError() << "\n";
}

void PrintTTFError(const std::string &functionName, const std::string &message)
{
    PrintTTFError(functionName);
    std::cout << message << "\n";
}

void RenderTexture(SDL_Renderer *renderer, const std::string &texture, int x, int y, int width, int height)
{
    static SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = width;
    rect.h = height;
    SDL_RenderCopy(renderer, GetTexture(texture), NULL, &rect);
}

void RenderTexture(SDL_Renderer *renderer, const std::string &texture, int x, int y)
{
    static SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    SDL_Texture* tex = GetTexture(texture);
    SDL_QueryTexture(tex, NULL, NULL, &rect.w, &rect.h);
    SDL_RenderCopy(renderer, tex, NULL, &rect);
}

void RenderTextureCenter(SDL_Renderer *renderer, const std::string &texture, int xOffset, int yOffset, const Size& windowSize)
{
    int w, h;
    SDL_QueryTexture(GetTexture(texture), NULL, NULL, &w, &h);
    RenderTexture(renderer, texture, ((windowSize.x / 2) - (w / 2)) + xOffset, ((windowSize.y / 2) - (h / 2)) + yOffset);
}
