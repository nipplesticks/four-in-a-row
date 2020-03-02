#ifndef _GAME_H_
#define _GAME_H_
#include <SFML/Graphics.hpp>
#include "../Utility/Timer.h"
#include "../Utility/vector.h"
#include "Pin/Pin.h"

using namespace Container::Vector;

const size_t DATA_SIZE = 256;

struct Data
{
    char buffer[DATA_SIZE];
};

enum EventType
{
    ResizeWindow,
    Exit,
    Undefined
};

struct GameEvent
{
    EventType Type = Undefined;
    Data Envelope;
};

class Game
{
public:
    Game(sf::RenderTarget * renderTarget);
    ~Game();

    void HandleEvent(const GameEvent& gameEvent);
    void StartGame();
    bool IsRunning();

private:
    struct AI
    {
        int RandomChance; // 0 - 100 percent chance for a random move
        int Dumbness; // 0 - 100 percent of a dumb move
        int Agressivness; // 0 - 100 focus on defence or win
    };
    struct Move
    {
        int value;
        sf::Vector2i Idx;
    };
    struct AnalyzeResult
    {
        Vector<Move> Agressive;
        Vector<Move> Deffensive;
    };

private:
    void _draw();
    void _resize(const sf::Vector2u & newSize);
    void _handleMousePress();
    void _handleMarkerPin(const sf::Vector2i & mp);
    void _checkWin(int x, int y);
    void _checkDraw();
private:
    void _ai(const AI & brain);
    AnalyzeResult _analyzeBoard(Vector<Pin> board, int steps);
    void _recursiveAnalyze(Vector<Pin>& board, int step, int maxStep);
private:
    sf::RenderTarget* m_renderTarget_p;

    bool m_isRunning = false;
    bool m_closed = true;

    sf::Vector2i m_size;
    sf::Vector2f m_offset;
    float m_tileSize;
    float m_percent;

    Vector<sf::RectangleShape> m_board;
    Vector<Pin> m_pins;
    Pin m_markerPin;
    sf::RectangleShape m_marker;

    int m_teamTurn;
    int m_maxTeams;
    Vector<sf::Color> m_teamColors;
    bool m_isWon;
    int m_moveNo;

    sf::Text m_winText;
    sf::Font m_font;
};

#endif