#include <SFML/Graphics.hpp>
#include "Game/Game.h"
#include "Utility/Vector.h"
#include <thread>

int main()
{
    sf::RenderWindow wnd(sf::VideoMode(1234, 567), "Four in a row");
    wnd.setActive(false);
    Game game(&wnd);

    std::thread gameThread(&Game::StartGame, &game);

    while (wnd.isOpen())
    {
        sf::Event e;
        while (wnd.pollEvent(e))
        {
            GameEvent ge;
            switch (e.type)
            {
            case sf::Event::Closed:
            {
                ge.Type = EventType::Exit;
                game.HandleEvent(ge);
                while (game.IsRunning());
                wnd.setActive(true);
                wnd.close();
                break;
            }
            case sf::Event::Resized:
            {
                ge.Type = EventType::ResizeWindow;
                sf::Vector2u size(e.size.width, e.size.height);
                memcpy(&ge.Envelope, &size, sizeof(size));
                sf::View v = wnd.getView();
                v.setSize(size.x, size.y);
                v.setCenter(size.x * 0.5f, size.y * 0.5f);
                wnd.setView(v);
                break;
            }
            case sf::Event::LostFocus:
            {

                break;
            }
            case sf::Event::GainedFocus:
            {
                break;
            }
            case sf::Event::TextEntered:
            {
                break;
            }
            case sf::Event::KeyPressed:
            {
                break;
            }
            case sf::Event::KeyReleased:
            {
                break;
            }
            case sf::Event::MouseWheelMoved:
            {
                break;
            }
            case sf::Event::MouseWheelScrolled:
            {
                break;
            }
            case sf::Event::MouseButtonPressed:
            {
                break;
            }
            case sf::Event::MouseButtonReleased:
            {
                break;
            }
            case sf::Event::MouseMoved:
            {
                break;
            }
            case sf::Event::MouseEntered:
            {
                break;
            }
            case sf::Event::MouseLeft:
            {
                break;
            }
            case sf::Event::JoystickButtonPressed:
            {
                break;
            }
            case sf::Event::JoystickButtonReleased:
            {
                break;
            }
            case sf::Event::JoystickMoved:
            {
                break;
            }
            case sf::Event::JoystickConnected:
            {
                break;
            }
            case sf::Event::JoystickDisconnected:
            {
                break;
            }
            case sf::Event::TouchBegan:
            {
                break;
            }
            case sf::Event::TouchMoved:
            {
                break;
            }
            case sf::Event::TouchEnded:
            {
                break;
            }
            case sf::Event::SensorChanged:
            {
                break;
            }
            default:
            {
                break;
            }
            }
            game.HandleEvent(ge);
        }
    }

    gameThread.join();

    return 0;
}