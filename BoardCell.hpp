#ifndef BOARDCELL_HPP
#define BOARDCELL_HPP

#include <iostream>
#include <bitset>
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

    static void             printBinary(std::string disp, uint64_t bin)
    {
        unsigned int val;
        uint64_t test = 1;

        test <<= 63;
        std::cout << disp;
        for (int j = 63; j >= 0; --j)
        {
            val = (unsigned int)((bin & test) >> j);
            std::cout << val;
            test = test >> 1;
        }
        std::cout << std::endl;
    }

    static uint64_t*        initAlignementsSizeMasks()
    {
        uint64_t*    tab = new uint64_t[8]; // 8 directions

        for(uint64_t i = 0; i < 8; ++i)
        {
            uint64_t    it = 7 - i;

            tab[it] = 0;
            tab[it] = ~tab[it];
            tab[it] = tab[it] << (64 - 5 * (i + 1));
            tab[it] = tab[it] >> (64 - 5 * (i + 1) + 2 + 5 * i);
            tab[it] = tab[it] << (42 - 5 * (i + 1));
        }
        return tab;
    }

    inline static int getAlignmentSize(uint64_t value, int direction)
    {
        static uint64_t*    values = initAlignementsSizeMasks();
        uint64_t            nr = (value & values[direction]) >> (2 + 5 * direction);
        //uint64_t r = value << (64 - 5 * (direction + 1));
        //r = r >> (64 - 5 * (direction + 1) + 2 + 5 * direction);
        return nr;
    }

    static uint64_t*        initAlignementsValueMasks()
    {
        uint64_t*    tab = new uint64_t[8]; // 8 directions

        for(uint64_t i = 0; i < 8; ++i)
        {
            uint64_t    it = 7 - i;

            tab[it] = 0;
            tab[it] = ~tab[it];
            tab[it] = tab[it] << (62 - 5 * (i + 1));
            tab[it] = tab[it] >> (62 - 5 * (i + 1) + 2 + 5 * i);
            tab[it] = tab[it] << (42 - 5 * (i + 1));
        }
        return tab;
    }

    static unsigned int getAlignmentValue(uint64_t value, int direction)
    {
        static uint64_t*    values = initAlignementsValueMasks();
        uint64_t            nr = (value & values[direction]) >> (2 + 5 * direction);
        //uint64_t r = value << (62 - 5 * (direction + 1));
        //r = r >> (62 - 5 * (direction + 1) + 2 + 5 * direction);
        return nr;
    }


    static uint64_t*        initAlignementsClosedMasks()
    {
        uint64_t*    tab = new uint64_t[8]; // 8 directions

        for(uint64_t i = 0; i < 8; ++i)
        {
            uint64_t    it = 7 - i;
            uint64_t mask = 1;
            mask = mask << (45 - 5 * (i + 1));
            tab[it] = mask;
        }
        return tab;
    }

    static bool isAlignmentClosed(uint64_t value, int direction)
    {
        static uint64_t*    values = initAlignementsClosedMasks();
        uint64_t            r = (value >> (5 * direction + 5)) & 1;
        //uint64_t            nr = (value & values[direction]) >> (5 + 5 * direction);
        return r;
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
