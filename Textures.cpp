#include "Textures.h"
#include "SDLUtil.h"

#include <unordered_map>
#include <SDL_image.h>
#include <iostream>

std::unordered_map<std::string, SDL_Texture*> textures;

SDL_Texture* LoadTexture(SDL_Renderer* renderer, const std::string &filename, const std::string &id)
{
    std::cout << "Loading texture: " << filename << "\n";

    if (textures.count(id) != 0)
    {
        std::cout << "Tried to load texture " << filename << " with id " << id << " but there is already a texture with that id.\n";
        return nullptr;
    }

    SDL_Texture* texture = IMG_LoadTexture(renderer, filename.c_str());

    if (!texture)
    {
        PrintSDLError("Loading texture");
        return nullptr;
    }

    textures[id] = texture;
    return texture;
}

void DestroyTextures()
{
    std::cout << "Unloading " <<  textures.size() << " textures.\n";
    for (const auto& i : textures)
        SDL_DestroyTexture(i.second);
    textures.clear();
}

SDL_Texture* GetTexture(const std::string &id)
{
    if (textures.count(id) == 0)
    {
        std::cout << "Tried to get texture " << id << " but it doesn't exist.\n";
        return nullptr;
    }
    else
        return textures[id];
}

void AddTexture(const std::string &id, SDL_Texture *texture)
{
    if (textures.count(id) != 0)
    {
        std::cout << "Tried to add a texture with id " << id << " but one already exists with that name.\n";
        return;
    }
    else
        textures[id] = texture;
}

void UnloadTexture(const std::string &id)
{
    std::cout << "Unloading texture with id " << id << "\n";
    if (textures.count(id))
    {
        SDL_DestroyTexture(textures[id]);
        textures.erase(id);
    }
    else
        std::cout << "Tried to remove texture with id " << id << " but it doesn't exist.\n";
}
