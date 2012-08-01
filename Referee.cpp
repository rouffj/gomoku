#include "Referee.hpp"
#include "Game.hpp"
#include "IPlayer.hpp"

Referee::Referee(Rules& rules) : _rules(rules)
{

}

bool Referee::isValidMove(Board & board, Coord& coord, int color)
{
    if (BoardCell::matchMask(*board.getCell(coord.x, coord.y), WHITE) || BoardCell::matchMask(*board.getCell(coord.x, coord.y), BLACK))
        return false;
    if (this->_rules.DoubleThree && this->isDoubleThreeMove(board, coord, color))
    {
        board.setError();
        return false;
    }
    board.setError(false);
    return true;
}

bool Referee::isDoubleThreeMove(Board & board, Coord& coord, int color)
{
    int nbDouble3 = 0;
    for (int direction = TOPLEFT; direction < BOTTOMRIGHT; direction++)
    {
        if (this->isThreeMove(board, coord, color, direction)
            || this->isThreeMove(board, coord, color, board.getOppositeDirection(direction)))
            nbDouble3++;
    }
    if (nbDouble3 >= 2)
        return true;
    return false;
}

bool Referee::isThreeMove(Board& board, Coord& coord, int color, int direction)
{
    int oppositeDirection = board.getOppositeDirection(direction);
    int oppositeAlignSize = BoardCell::getAlignmentSize(*board.getCell(coord), oppositeDirection);
    Coord nextCell = board.getNextCell(&coord, direction);
    if (oppositeAlignSize == 0)
    {
        // .CXX.
        if ((color == BLACK && BoardCell::matchAlignement(*board.getCell(coord), ALIGN_2_OPEN_BLACK, direction))
            || (color == WHITE && BoardCell::matchAlignement(*board.getCell(coord), ALIGN_2_OPEN_WHITE, direction)))
            return true;
        if (board.isValid(nextCell))
        {
            // .C.XX.
            if (BoardCell::getAlignmentSize(*board.getCell(coord), direction) == 0
                && ((color == BLACK && BoardCell::matchAlignement(*board.getCell(nextCell), ALIGN_2_OPEN_BLACK, direction))
                || (color == WHITE && BoardCell::matchAlignement(*board.getCell(nextCell), ALIGN_2_OPEN_WHITE, direction))))
                return true;
            // .CX.X.
            Coord nextNextCell = board.getNextCell(&nextCell, direction);
            if (board.isValid(nextNextCell)
                && ((color == BLACK && BoardCell::matchAlignement(*board.getCell(coord), ALIGN_1_OPEN_BLACK, direction))
                    || (color == WHITE && BoardCell::matchAlignement(*board.getCell(coord), ALIGN_1_OPEN_WHITE, direction)))
                && ((color == BLACK && BoardCell::matchAlignement(*board.getCell(nextNextCell), ALIGN_1_OPEN_BLACK, direction))
                    || (color == WHITE && BoardCell::matchAlignement(*board.getCell(nextNextCell), ALIGN_1_OPEN_WHITE, direction))))
                return true;
        }
    } else if (oppositeAlignSize == 1
               && BoardCell::getAlignmentColor(*board.getCell(coord), oppositeDirection) == color
               && !BoardCell::isAlignmentClosed(*board.getCell(coord), oppositeDirection)) {
        // .XCX.
        if ((color == BLACK && BoardCell::matchAlignement(*board.getCell(coord), ALIGN_1_OPEN_BLACK, direction))
            || (color == WHITE && BoardCell::matchAlignement(*board.getCell(coord), ALIGN_1_OPEN_WHITE, direction)))
            return true;
        // .XC.X.
        if (board.isValid(nextCell)
            && BoardCell::getAlignmentSize(*board.getCell(coord), direction) == 0
            && ((color == BLACK && BoardCell::matchAlignement(*board.getCell(nextCell), ALIGN_1_OPEN_BLACK, direction))
            || (color == WHITE && BoardCell::matchAlignement(*board.getCell(nextCell), ALIGN_1_OPEN_WHITE, direction))))
            return true;
    }
    return false;
}

void Referee::doTakings(Board& board, IPlayer** players, unsigned int turn)
{
    for (int direction = TOPLEFT; direction <= LEFT; direction++)
    {
        if ((players[(turn + 1) % 2]->getColor() == BLACK && BoardCell::matchAlignement(*board.getCell(*board.getLastPlayed()), ALIGN_2_CLOSED_BLACK, direction))
            || (players[(turn + 1) % 2]->getColor() == WHITE && BoardCell::matchAlignement(*board.getCell(*board.getLastPlayed()), ALIGN_2_CLOSED_WHITE, direction)))
            {
            Coord toRemove1 = board.getNextCell(board.getLastPlayed(), direction);
            Coord toRemove2 = board.getNextCell(&toRemove1, direction);
            Coord nextStone = board.getNextCell(&toRemove2, direction);
            if (board.isValid(nextStone))
            {
                board.removeStone(&toRemove1);
                board.removeStone(&toRemove2);
                players[turn % 2]->addTakenStone(2);
                board.dispatchTakings(toRemove1, players[(turn + 1) % 2]->getColor());
                board.dispatchTakings(toRemove2, players[(turn + 1) % 2]->getColor());
            }
        }
    }
}

bool Referee::isWinning(Board & board)
{
    for (unsigned int i = 0; i < board.getSize(); i++)
    {
        for (unsigned int j = 0; j < board.getSize(); j++)
        {
            Coord c(i, j);
            if (this->isWinningMove(board, c))
                return true;
        }
    }
    return false;
}

bool Referee::isWinningMove(Board & board, Coord& coord)
{
    for (int direction = TOPLEFT; direction < BOTTOMRIGHT; direction++)
    {
        int leftSize = 0;
        int rightSize = 0;
        if (BoardCell::matchMask(*board.getCell(coord.x, coord.y), BoardCell::getAlignmentColor(*board.getCell(coord.x, coord.y), direction)))
            leftSize = BoardCell::getAlignmentSize(*board.getCell(coord.x, coord.y), direction);
        if (BoardCell::matchMask(*board.getCell(coord.x, coord.y), BoardCell::getAlignmentColor(*board.getCell(coord.x, coord.y), board.getOppositeDirection(direction))))
            rightSize = BoardCell::getAlignmentSize(*board.getCell(coord.x, coord.y), board.getOppositeDirection(direction));
        if (leftSize + rightSize + 1 >= 5)
        {
            if (!this->_rules.EndGameTaking)
                return true;
            else if (this->_rules.EndGameTaking && !this->isAlignTakable(board, coord, direction, leftSize, rightSize))
                return true;
        }
    }
    return false;
}

bool Referee::isAlignTakable(Board& board, Coord& coord, int direction, int left, int right)
{
    if (BoardCell::isTakable(board, coord))
        return true;
    Coord curCoord = board.getNextCell(&coord, direction);
    int i = 0;
    while (i < left && board.isValid(curCoord))
    {
        if (BoardCell::isTakable(board, curCoord))
            return true;
        curCoord = board.getNextCell(&curCoord, direction);
        i++;
    }
    int oppositeDirection = (direction + 4) % 8;
    curCoord = board.getNextCell(&coord, oppositeDirection);
    i = 0;
    while (i < right && board.isValid(curCoord))
    {
        if (BoardCell::isTakable(board, curCoord))
            return true;
        curCoord = board.getNextCell(&curCoord, oppositeDirection);
        i++;
    }
    return false;
}

bool Referee::isWinningPlayer(IPlayer** players)
{
    return (players[0]->getStone() == 10 || players[1]->getStone() == 10);
}