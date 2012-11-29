#include "HumanPlayer.hpp"

HumanPlayer::HumanPlayer(Rules& rules) : _color(0), _nbStone(0), _rules(rules)
{
    std::cout << "Player creation" << std::endl;
}

bool HumanPlayer::play(Game& game, bool (Game::*callback)(Coord&))
{
    while (game.getBoard().getLastPlayed() == 0)
    {
        usleep(1);
        game.catchEvents();
        if (game.getWindow().isClosed())
            return (true);
    }
    return (game.*callback)(*game.getBoard().getLastPlayed());
}

int HumanPlayer::getColor()
{
    return (this->_color);
}

void HumanPlayer::setColor(int color)
{
    this->_color = color;
}

void HumanPlayer::addTakenStone(int nbStone)
{
    this->_nbStone += nbStone;
}

int HumanPlayer::getStone()
{
    return this->_nbStone;
}
