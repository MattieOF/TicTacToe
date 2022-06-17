#ifndef TICTACTOE_TEXTURES_H
#define TICTACTOE_TEXTURES_H

#include <SDL.h>
#include <string>

SDL_Texture* LoadTexture(SDL_Renderer* renderer, const std::string& filename, const std::string& id);
SDL_Texture* GetTexture(const std::string& id);
void AddTexture(const std::string& id, SDL_Texture* texture);
void UnloadTexture(const std::string& id);
void DestroyTextures();

#endif //TICTACTOE_TEXTURES_H
