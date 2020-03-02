#include "Pin.h"

Pin::Pin(int team, float x, float y, float rad)
{
    m_teamId = team;
    m_shape.setPosition(x, y);
    m_shape.setRadius(rad);
    m_shape.setOrigin(sf::Vector2f(rad, rad));
    m_shape.setFillColor(sf::Color::White);
}

void Pin::SetColor(const sf::Color& col)
{
    m_shape.setFillColor(col);
}

const sf::Color& Pin::GetColor() const
{
    return m_shape.getFillColor();
}

void Pin::SetPosition(float x, float y)
{
    m_shape.setPosition(x, y);
}

const sf::Vector2f& Pin::GetPosition() const
{
    return m_shape.getPosition();
}

void Pin::SetRadius(float rad)
{
    m_shape.setRadius(rad);
    m_shape.setOrigin(sf::Vector2f(rad, rad));
}

float Pin::GetRadius()
{
    return m_shape.getRadius();
}

void Pin::SetTexture(sf::Texture* tex)
{
    m_shape.setTexture(tex);
}

void Pin::Draw(sf::RenderTarget* renderTarget)
{
    renderTarget->draw(m_shape);
}
