#ifndef IPLAYER_H
#define IPLAYER_H

#include "Game.hpp"
#include "Board.hpp"

class IPlayer
{
public:
    virtual ~IPlayer()
    {
        std::cout << "IPlayer destruction" << std::endl;
    }
    virtual bool play(Game& game, bool (Game::*callback)(Coord&)) = 0;
    virtual int getColor() = 0;
    virtual void setColor(int color) = 0;
    virtual void addTakenStone(int nbStone) = 0;
    virtual int getStone() = 0;
};

#endif // IPLAYER_H
