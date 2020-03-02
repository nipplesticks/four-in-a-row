#include "TextureHandler.h"
std::unordered_map<std::string, sf::Texture> TextureHandler::Textures;

bool TextureHandler::LoadTexture(const std::string& path, const std::string& key)
{
    Textures.insert(std::make_pair(key, sf::Texture()));
    if (!Textures.at(key).loadFromFile(path))
    {
        UnloadTexture(key);
        return false;
    }
    return true;
}

void TextureHandler::UnloadTexture(const std::string& key)
{
    Textures.erase(key);
}

sf::Texture* TextureHandler::GetTexture(const std::string& key)
{
    return &Textures.at(key);
}
