#include "Board.hpp"
#include "BoardCell.hpp"

bool BoardCell::isTakable(Board& board, Coord& cell)
{
    for (int direction = TOPLEFT; direction <= LEFT; direction++)
    {
        int closed = BoardCell::isAlignmentClosed(*board.getCell(cell), direction);
        int oppositeDirection = (direction + 4) % 8;
        int color = BoardCell::getColor(*board.getCell(cell));
        if (BoardCell::getAlignmentColor(*board.getCell(cell), direction) == color
            && BoardCell::getAlignmentSize(*board.getCell(cell), direction) == 1)
        {
            Coord c = board.getNextCell(&cell, direction);
            c = board.getNextCell(&c, direction);
            Coord c2 = board.getNextCell(&cell, oppositeDirection);
            if (closed && board.isValid(c) && BoardCell::getAlignmentSize(*board.getCell(cell), oppositeDirection) == 0 && board.isValid(c2))
                return true;
            if (!closed && BoardCell::getAlignmentColor(*board.getCell(cell), oppositeDirection) != color
                && BoardCell::getAlignmentSize(*board.getCell(cell), oppositeDirection) > 0)
                return true;
        }
    }
    return false;
}
