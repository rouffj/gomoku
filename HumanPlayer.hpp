#ifndef HUMANPLAYER_HPP7
#define HUMANPLAYER_HPP

#include "IPlayer.hpp"

class HumanPlayer : public IPlayer
{
public:
    HumanPlayer(Rules& rules);
    virtual ~HumanPlayer()
    {
        std::cout << "Player destruction" << std::endl;
    }

    bool play(Game& game, bool (Game::*)(Coord&));
    int getColor();
    void setColor(int color);
    void addTakenStone(int nbStone);
    int getStone();

private:
    int _color;
    int _nbStone;
    Rules& _rules;
};

#endif // HUMANPLAYER_HPP
