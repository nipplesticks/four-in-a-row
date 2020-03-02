#ifndef _PIN_H_
#define _PIN_H_

#include <SFML/Graphics.hpp>

class Pin
{
public:
    Pin(int team = 0, float x = 0, float y = 0, float rad = 0);
    ~Pin() {}

    void SetColor(const sf::Color& col);
    const sf::Color& GetColor() const;

    void SetTeam(int team) { m_teamId = team; }
    int GetTeam() const { return m_teamId; }

    void SetPosition(float x, float y);
    const sf::Vector2f & GetPosition() const;

    void SetRadius(float rad);
    float GetRadius();

    void SetTexture(sf::Texture* tex);

    void Draw(sf::RenderTarget * renderTarget);
private:
    sf::CircleShape m_shape;
    int m_teamId;
};


#endif