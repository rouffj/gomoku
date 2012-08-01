#ifndef BOARDCELL_HPP
#define BOARDCELL_HPP

#include <iostream>
#include "Types.hpp"

class Board;

class BoardCell
{
public:
    static void setMask(uint64_t* value, uint64_t mask)
    {
        *value |= mask;
    }

    static bool matchMask(uint64_t value, uint64_t mask)
    {
        return ((value & mask) == mask);
    }

    static bool isEmpty(uint64_t value)
    {
        return !(BoardCell::matchMask(value, WHITE) || BoardCell::matchMask(value, BLACK));
    }

    static int getColor(uint64_t value)
    {
        if (BoardCell::matchMask(value, WHITE))
            return WHITE;
        else if (BoardCell::matchMask(value, BLACK))
            return BLACK;
        else
            return EMPTY;
    }


    static int getAlignmentColor(uint64_t value, int direction)
    {
        return (((value >> (5 * direction + 6)) & 1) == 0) ? WHITE : BLACK;
    }

    static int getAlignmentSize(uint64_t value, int direction)
    {
        uint64_t r = value << (64 - 5 * (direction + 1));
        r = r >> (64 - 5 * (direction + 1) + 2 + 5 * direction);
        return r;
    }
    
    static unsigned int getAlignmentValue(uint64_t value, int direction)
    {
        uint64_t r = value << (62 - 5 * (direction + 1));
        r = r >> (62 - 5 * (direction + 1) + 2 + 5 * direction);
        return r;
    }

    static bool isAlignmentClosed(uint64_t value, int direction)
    {
        return (value >> (5 * direction + 5)) & 1;
    }

    static bool matchAlignement(uint64_t value, uint64_t mask, int direction)
    {
        return (BoardCell::getAlignmentValue(value, direction) == mask);
    }
    
    static void setAlignment(uint64_t* value, int direction, int color, bool closed, int size)
    {
        if (size < 8)
        {
            uint64_t newValue = size;
            if (closed)
                newValue += 8;
            if (color == BLACK)
                newValue += 16;
            newValue = newValue << (2 + 5 * direction);
            uint64_t oldValue = BoardCell::getAlignmentValue(*value, direction);
            oldValue = oldValue << (2 + 5 * direction);
            *value ^= oldValue;
            *value |= newValue;
        }
    }

    static bool isTakable(Board& board, Coord& cell);
};

#endif // BOARDCELL_HPP
