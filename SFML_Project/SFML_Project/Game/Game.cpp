#include "Game.h"
#include "TextureHandler.h"
#include <iostream>

Game::Game(sf::RenderTarget* renderTarget)
{
    m_font.loadFromFile("AGENCYB.TTF");
    m_winText.setFont(m_font);
    m_winText.setFillColor(sf::Color::White);
    m_isWon = false;
    m_moveNo = 0;
    m_teamTurn = 0;
    m_maxTeams = 2;
    m_renderTarget_p = renderTarget;
    m_size.x = 7;
    m_size.y = 6;
    m_percent = 0.8;
    auto size = renderTarget->getSize();
    float _min = std::min(size.x, size.y);
    m_tileSize = (_min * m_percent) / std::min(m_size.x, m_size.y);
    m_offset.x = size.x * 0.5f - ((m_tileSize * m_size.x) *0.5f);
    m_offset.y = size.y - m_size.y * m_tileSize;
    m_board = Vector<sf::RectangleShape>(m_size.x * m_size.y, m_size.x * m_size.y);
    m_pins = Vector<Pin>(m_size.x * m_size.y, m_size.x * m_size.y);
    m_teamColors.PushBack(sf::Color::Yellow);
    m_teamColors.PushBack(sf::Color::Red);
    TextureHandler::LoadTexture("Textures/Tile.png", "BOARD_TILE");

    sf::Texture* tex = TextureHandler::GetTexture("BOARD_TILE");

    m_markerPin.SetTeam(-1);
    m_markerPin.SetRadius(m_tileSize * 0.5f);
    m_markerPin.SetColor(m_teamColors[m_teamTurn]);
    for (int y = 0; y < m_size.y; y++)
    {
        for (int x = 0; x < m_size.x; x++)
        {
            int idx = x + y * m_size.x;
            m_board[idx].setTexture(tex);
            m_board[idx].setPosition(m_offset.x + x * m_tileSize,
                                     m_offset.y + y * m_tileSize);
            m_board[idx].setSize(sf::Vector2f(m_tileSize, m_tileSize));
            m_board[idx].setFillColor(sf::Color::Blue);
            m_pins[idx].SetTeam(-1);
            m_pins[idx].SetRadius(m_tileSize * 0.5f);
            m_pins[idx].SetPosition(m_offset.x + x * m_tileSize + m_tileSize * 0.5f,
                                    m_offset.y + y * m_tileSize + m_tileSize * 0.5f);
        };
    }
}

Game::~Game()
{
}

void Game::HandleEvent(const GameEvent& gameEvent)
{
    switch (gameEvent.Type)
    {
    case ResizeWindow:
    {
        sf::Vector2u newSize;
        memcpy(&newSize, &gameEvent.Envelope, sizeof(newSize));
        _resize(newSize);
        break;
    }
    case Exit:
    {
        m_isRunning = false;
        break;
    }
    default:
        break;
    }
}

void Game::StartGame()
{
    m_renderTarget_p->setActive(true);
    m_isRunning = true;
    sf::RenderWindow* wnd = dynamic_cast<sf::RenderWindow*>(m_renderTarget_p);

    Timer deltaTime;
    deltaTime.Start();
    while (m_isRunning)
    {
        float dt = deltaTime.Stop();

        sf::Vector2i mp = sf::Mouse::getPosition();

        if (wnd)
            mp = sf::Mouse::getPosition(*wnd);

        if (!m_isWon)
        {
            if (m_teamTurn == 0)
            {
                _handleMarkerPin(mp);
                _handleMousePress();
            }
            else
            {
                AI brain = {
                    100,
                    0,
                    50
                };


                _ai(brain);
            }

            _checkDraw();
        }
        
        m_renderTarget_p->clear();
            _draw();
        if (wnd) wnd->display();
    }
    m_closed = true;
    m_renderTarget_p->setActive(false);
}

bool Game::IsRunning()
{
    return !m_closed;
}

void Game::_draw()
{
    for (int y = 0; y < m_size.y; y++)
    {
        for (int x = 0; x < m_size.x; x++)
        {
            int idx = x + y * m_size.x;
            if (m_pins[idx].GetTeam() != -1)
                m_pins[idx].Draw(m_renderTarget_p);
        }
    }

    if (!m_isWon)
        m_renderTarget_p->draw(m_marker);

    for (int y = 0; y < m_size.y; y++)
    {
        for (int x = 0; x < m_size.x; x++)
        {
            int idx = x + y * m_size.x;
            m_renderTarget_p->draw(m_board[idx]);
        }
    }

    if (!m_isWon)
        m_markerPin.Draw(m_renderTarget_p);
    if (m_isWon)
        m_renderTarget_p->draw(m_winText);
}

void Game::_resize(const sf::Vector2u& newSize)
{
    auto size = newSize;
    std::cout << "size: " << size.x << " " << size.y << "\n";
    float _min = std::min(size.x, size.y);
    m_tileSize = (_min * m_percent) / std::min(m_size.x, m_size.y);
    m_offset.x = size.x * 0.5f - ((m_tileSize * m_size.x) * 0.5f);
    m_offset.y = size.y - m_size.y * m_tileSize;
    
    m_markerPin.SetRadius(m_tileSize * 0.5f);
    for (int y = 0; y < m_size.y; y++)
    {
        for (int x = 0; x < m_size.x; x++)
        {
            int idx = x + y * m_size.x;
            
            m_board[idx].setPosition(m_offset.x + x * m_tileSize,
                m_offset.y + y * m_tileSize);
            m_board[idx].setSize(sf::Vector2f(m_tileSize, m_tileSize));
            m_pins[idx].SetRadius(m_tileSize * 0.5f);
            m_pins[idx].SetPosition(m_offset.x + x * m_tileSize + m_tileSize * 0.5f,
                m_offset.y + y * m_tileSize + m_tileSize * 0.5f);
        }
    }
}

void Game::_handleMousePress()
{
    static bool mouseLPressedLastFrame = false;
    static bool mouseRPressedLastFrame = false;

    bool mouseL = sf::Mouse::isButtonPressed(sf::Mouse::Left);
    bool mouseR = sf::Mouse::isButtonPressed(sf::Mouse::Right);

    int xCoord = ((float)m_markerPin.GetPosition().x - m_offset.x) / m_tileSize;

    m_marker.setPosition(xCoord * m_tileSize + m_offset.x, m_offset.y);
    sf::Color col = m_markerPin.GetColor();
    col.a = 64;
    m_marker.setFillColor(col);

    bool isOk = m_pins[xCoord + 0 * m_size.x].GetTeam() == -1;

    if (isOk)
    {
        int at = 0;
        for (int y = 1; y < m_size.y; y++)
        {
            int idx = xCoord + y * m_size.x;
            if (m_pins[idx].GetTeam() == -1)
            {
                at = y;
            }
        }
        m_marker.setSize(sf::Vector2f(m_tileSize, m_tileSize * (at + 1)));
        if (!mouseLPressedLastFrame && mouseL)
        {
            m_pins[xCoord + at * m_size.x].SetTeam(m_teamTurn++);
            m_pins[xCoord + at * m_size.x].SetColor(m_markerPin.GetColor());
            m_moveNo++;

            _checkWin(xCoord, at);

            m_teamTurn %= m_maxTeams;
            m_markerPin.SetColor(m_teamColors[m_teamTurn]);
        }
    }

    mouseLPressedLastFrame = mouseL;
    mouseRPressedLastFrame = mouseR;
}

void Game::_handleMarkerPin(const sf::Vector2i& mp)
{
    float sizeX = m_renderTarget_p->getSize().x;
    int markerX = mp.x;
    markerX = std::max((float)markerX, m_offset.x + m_tileSize * 0.5f);
    markerX = std::min((float)markerX, sizeX - (m_offset.x + m_tileSize * 0.5f));
    m_markerPin.SetPosition(markerX, m_tileSize);
}

void Game::_checkWin(int x, int y)
{
    int team = m_pins[x + y * m_size.x].GetTeam();
    m_winText.setString("Player_" + std::to_string(m_teamTurn) + " Has Won!");
    m_winText.setFillColor(m_pins[x + y * m_size.x].GetColor());
    int _x = x;
    int _y = y;

    while (true)
    {
        if (_x == 0)
            break;
        if (m_pins[--_x + y * m_size.x].GetTeam() != team)
        {
            _x++;
            break;
        }
    }

    if (_x + 4 <= m_size.x)
    {
        bool isWon = true;
        for (int i = _x; i < _x + 4; i++)
        {
            if (m_pins[i + y * m_size.x].GetTeam() != team)
            {
                isWon = false;
                break;
            }
        }
        if (isWon)
        {
            m_isWon = true;
            return;
        }
    }
    
    while (true)
    {
        if (_y == 0)
            break;
        if (m_pins[x + --_y * m_size.x].GetTeam() != team)
        {
            _y++;
            break;
        }
    }

    if (_y + 4 <= m_size.y)
    {
        bool isWon = true;
        for (int i = _y; i < _y + 4; i++)
        {
            if (m_pins[x + i * m_size.x].GetTeam() != team)
            {
                isWon = false;
                break;
            }
        }
        if (isWon)
        {
            m_isWon = true;
            return;
        }
    }

    _x = x;
    _y = y;

    while (true)
    {
        if (_x == 0 || _y == 0)
            break;
        if (m_pins[--_x + --_y * m_size.x].GetTeam() != team)
        {
            _x++;
            _y++;
            break;
        }
    }

    if (_x + 4 <= m_size.x && _y + 4 <= m_size.y)
    {
        bool isWon = true;
        for (int i = 0; i < 4; i++)
        {
            if (m_pins[_x++ + _y++ * m_size.x].GetTeam() != team)
            {
                isWon = false;
                break;
            }
        }
        if (isWon)
        {
            m_isWon = true;
            return;
        }
    }

    _x = x;
    _y = y;

    while (true)
    {
        if (_x == 0 || _y == m_size.y - 1)
            break;
        if (m_pins[--_x + ++_y * m_size.x].GetTeam() != team)
        {
            _x++;
            _y--;
            break;
        }
    }

    if (_x + 4 <= m_size.x && _y - 4 >= 0)
    {
        bool isWon = true;
        for (int i = 0; i < 4; i++)
        {
            if (m_pins[_x++ + _y-- * m_size.x].GetTeam() != team)
            {
                isWon = false;
                break;
            }
        }
        if (isWon)
        {
            m_isWon = true;
            return;
        }
    }

}

void Game::_ai(const AI& brain)
{
    int xCoord = 0;
    if (rand() % 100 < brain.RandomChance)
    {
        bool isOk = false;
        while (!isOk)
        {
            xCoord = rand() % m_size.x;
            m_marker.setPosition(xCoord * m_tileSize + m_offset.x, m_offset.y);
            sf::Color col = m_markerPin.GetColor();
            col.a = 64;
            m_marker.setFillColor(col);
            isOk = m_pins[xCoord + 0 * m_size.x].GetTeam() == -1;
        }
    }
    else
    {
        AnalyzeResult ar = _analyzeBoard(m_pins, 1);



    }
   
    int at = 0;
    for (int y = 1; y < m_size.y; y++)
    {
        int idx = xCoord + y * m_size.x;
        if (m_pins[idx].GetTeam() == -1)
        {
            at = y;
        }
    }
    m_marker.setSize(sf::Vector2f(m_tileSize, m_tileSize * (at + 1)));
    m_pins[xCoord + at * m_size.x].SetTeam(m_teamTurn++);
    m_pins[xCoord + at * m_size.x].SetColor(m_markerPin.GetColor());
    m_moveNo++;

    _checkWin(xCoord, at);

    m_teamTurn %= m_maxTeams;
    m_markerPin.SetColor(m_teamColors[m_teamTurn]);
}

Game::AnalyzeResult Game::_analyzeBoard(Vector<Pin> board, int steps)
{
    AnalyzeResult ar;


    return ar;
}

void Game::_recursiveAnalyze(Vector<Pin>& board, int step, int maxStep)
{

}

void Game::_checkDraw()
{
    if (!m_isWon)
    {
        if (m_moveNo == m_pins.Size())
        {
            m_isWon = true;
            m_winText.setString("It's a draw!");
        }
    }
}
