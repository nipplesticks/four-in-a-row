#ifndef _TEXTURE_HANDLER_H_
#define _TEXTURE_HANDLER_H_

#include <SFML/Graphics.hpp>
#include <string>
#include <unordered_map>

class TextureHandler
{
public:
    static bool LoadTexture(const std::string & path, const std::string & key);
    static void UnloadTexture(const std::string& key);
    static sf::Texture* GetTexture(const std::string& key);

private:
    TextureHandler() {};
    ~TextureHandler() {};

    static std::unordered_map<std::string, sf::Texture> Textures;
};

#endif