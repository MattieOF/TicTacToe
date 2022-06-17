#include "Fonts.h"
#include "SDLUtil.h"

#include <iostream>
#include <unordered_map>

std::unordered_map<std::string, TTF_Font*> fonts;

TTF_Font* LoadFont(const std::string &filename, int size, const std::string &id)
{
    std::cout << "Loading font: \"" << filename << "\" at size " << size << "\n";
    TTF_Font* font;
    font = TTF_OpenFont(filename.c_str(), size);
    if (!font)
    {
        PrintTTFError("Loading font " + filename);
        return nullptr;
    }
    fonts[id] = font;
    return font;
}

TTF_Font *GetFont(const std::string &id)
{
    if (fonts.count(id) == 0)
    {
        std::cout << "Tried to get font " << id << " but it doesn't exist.\n";
        return nullptr;
    }
    else
        return fonts[id];
}

SDL_Texture* RenderText(SDL_Renderer *renderer, const std::string &text, const std::string &font, const SDL_Color &color)
{
    SDL_Surface* textSurface = TTF_RenderText_Solid(GetFont(font), text.c_str(), color);
    if (!textSurface)
    {
        PrintTTFError("Rendering text \"" + text + "\" with font " + font);
        return nullptr;
    }

    SDL_Texture* textTexture;
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_FreeSurface(textSurface);
    return textTexture;
}

void DrawText(SDL_Renderer* renderer, const std::string &text, const std::string &font, const SDL_Color& color, int x, int y)
{
    // https://thenumbat.github.io/cpp-course/sdl2/07/07.html
    SDL_Surface* textSurface = TTF_RenderText_Solid(GetFont(font), text.c_str(), color);
    if (!textSurface)
    {
        PrintTTFError("Rendering text \"" + text + "\" with font " + font);
        return;
    }

    SDL_Texture* textTexture;
    textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

    SDL_Rect pos = {x, y, textSurface->w, textSurface->h};
    SDL_RenderCopy(renderer, textTexture, NULL, &pos);

    SDL_DestroyTexture(textTexture);
    SDL_FreeSurface(textSurface);
}

void DestroyFonts()
{
    std::cout << "Unloading " <<  fonts.size() << " fonts.\n";
    for (const auto& i : fonts)
        TTF_CloseFont(i.second);
    fonts.clear();
}
