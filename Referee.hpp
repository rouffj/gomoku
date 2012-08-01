#ifndef REFEREE_HPP
#define REFEREE_HPP

#include "Board.hpp"
#include "Options.hpp"
#include "Types.hpp"


class IPlayer;
class Game;

class Referee
{
public:
    Referee(Rules& rules);
    bool isValidMove(Board& board, Coord& coord, int color);
    bool isWinning(Board& board);
    bool isWinningMove(Board& board, Coord& coord);
    bool isWinningPlayer(IPlayer** players);
    bool isDoubleThreeMove(Board& board, Coord& coord, int color);
    bool isThreeMove(Board& board, Coord& coord, int color, int direction);

    void doTakings(Board& board, IPlayer** players, unsigned int turn);
    bool isAlignTakable(Board& board, Coord& coord, int direction, int left, int right);

private:
    Rules& _rules;
};

#endif // REFEREE_HPP
