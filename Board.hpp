#ifndef BOARD_HPP
#define BOARD_HPP

#include <map>
#include <iostream>

#include "BoardCell.hpp"
#include "Types.hpp"

class Board
{
public:
    Board(unsigned int size = 19);
    Board(Board& cp);
    virtual ~Board();

    unsigned int getSize() const;
    uint64_t* getCell(unsigned int x, unsigned int y);
    uint64_t* getCell(Coord& coord);
    void removeStone(Coord* coord);
    Coord* getLastPlayed();
    void setLastPlayed(Coord* coord);
    void resetLastPlayed();
    void setError(bool error = true);
    bool getError();
    uint64_t** getBoard();

    void addStone(Coord* coord, int color);

    void dispatch(Coord* coord, int color);
    void dispatchTakings(Coord& from, int color);
    bool isValid(Coord const & coord);
    bool isBorder(Coord& coord);

    int getOppositeDirection(int direction);
    Coord getNextCell(Coord* from, int direction);

    static std::map<int, Coord*> createDirectionMap()
    {
        std::map<int, Coord*> map;
        map[TOPLEFT] = new Coord(-1, -1);
        map[TOP] = new Coord(0, -1);
        map[TOPRIGHT] = new Coord(1, -1);
        map[RIGHT] = new Coord(1, 0);
        map[BOTTOMRIGHT] = new Coord(1, 1);
        map[BOTTOM] = new Coord(0, 1);
        map[BOTTOMLEFT] = new Coord(-1, 1);
        map[LEFT] = new Coord(-1, 0);
        return map;
    }

    friend std::ostream& operator << (std::ostream& o, Board const & board);

private:
    void _dispatchThis(Coord& coord, int direction, int color, bool closed, int alignSize);
    void _dispatchClosedAlignment(Coord& coord, int direction);

private:
    unsigned int _size;
    uint64_t** _board;
    Coord* _lastPlayed;
    bool _error;
    static std::map<int, Coord*> _directionWays;
};

inline std::ostream& operator << (std::ostream& o, Board const & board)
{
    for (unsigned int i = 0; i < board._size; i++)
    {
        for (unsigned int j = 0; j < board._size; j++)
        {
            if (BoardCell::matchMask(board._board[j][i], WHITE))
                std::cout << " 1";
            else if (BoardCell::matchMask(board._board[j][i], BLACK))
                std::cout << " 2";
            else
                std::cout << " 0";
        }
        std::cout << std::endl;
    }
    return o;
}

#endif // BOARD_HPP
