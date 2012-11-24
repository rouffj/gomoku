#ifndef AI_HPP
#define AI_HPP

#include "IPlayer.hpp"
#include "GameStep.hpp"
#include "DebugAI.h"

#define INFINITY 1000000

class AI : public IPlayer
{
public:
    AI(Rules& rules, bool aiDebug);
    virtual ~AI();

    bool play(Game& game, bool (Game::*)(Coord&));

    int getColor();
    void setColor(int color);
    void addTakenStone(int nbStone);
    int getStone();
    bool isWinningPlayer(GameStep& gamestep);

private:
    int min(int a, int b);
    int max(int a, int b);
    bool isAlignTakable(Board& board, Coord& coord, int direction, int left, int right);
    bool isWinning(Board & board);
    bool isWinningMove(Board & board, Coord& coord);
    int minimax(Game& game, GameStep* gamestep, int color, int depth, int alpha, int beta);

private:
    DebugAI* _debugView;
    Coord* _toPlay;
    int _color;
    int _nbStone;
    Rules& _rules;
};

#endif // IA_HPP
