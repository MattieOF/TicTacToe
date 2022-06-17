#ifndef TICTACTOE_FONTS_H
#define TICTACTOE_FONTS_H

#include <SDL_ttf.h>
#include <string>

TTF_Font* LoadFont(const std::string& filename, int size, const std::string& id);
TTF_Font* GetFont(const std::string& id);
SDL_Texture* RenderText(SDL_Renderer* renderer, const std::string &text, const std::string &font, const SDL_Color& color);
void DrawText(SDL_Renderer* renderer, const std::string &text, const std::string &font, const SDL_Color& color, int x, int y);
void DestroyFonts();

#endif //TICTACTOE_FONTS_H
