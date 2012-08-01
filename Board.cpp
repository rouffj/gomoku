#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "Board.hpp"

std::map<int, Coord*> Board::_directionWays =  Board::createDirectionMap();

Board::Board(unsigned int size) :
        _size(size),
        _lastPlayed(0),
        _error(false)
{
    this->_board = new uint64_t*[this->_size];
    for (unsigned int i = 0; i < this->_size; i++)
    {
        this->_board[i] = new uint64_t[this->_size];
        for (unsigned int j = 0; j < this->_size; j++)
            this->_board[i][j] = 0x8000000000000000;
    }
}

Board::Board(Board& cp) :
        _size(cp.getSize()),
        _lastPlayed(0),
        _error(false)
{
    this->_board = new uint64_t*[this->_size];
    for (unsigned int i = 0; i < this->_size; i++)
    {
        this->_board[i] = new uint64_t[this->_size];
        for (unsigned int j = 0; j < this->_size; j++)
        {
            this->_board[i][j] = *cp.getCell(i, j);
        }
    }
}

Board::~Board()
{
    for (unsigned int i = 0; i < this->_size; i++)
        delete [] this->_board[i];
    delete [] this->_board;
    if (this->_lastPlayed)
        delete this->_lastPlayed;
}

uint64_t** Board::getBoard()
{
    return this->_board;
}

void Board::addStone(Coord* coord, int color)
{
    BoardCell::setMask(&this->_board[coord->x][coord->y], color);
    this->dispatch(coord, color);
}

void Board::removeStone(Coord* coord)
{
    this->_board[coord->x][coord->y] = this->_board[coord->x][coord->y] >> 2 << 2;
}

int Board::getOppositeDirection(int direction)
{
    return (direction + 4) % 8;
}

Coord Board::getNextCell(Coord* from, int direction)
{
    return Coord(from->x + this->_directionWays[direction]->x, from->y + this->_directionWays[direction]->y);
}

void Board::dispatch(Coord* from, int color)
{
    for (int direction = TOPLEFT; direction <= LEFT; direction++)
    {
        Coord nextCoord = this->getNextCell(from, direction);
        if (nextCoord.x < this->_size && nextCoord.y < this->_size)
        {
            // Tant qu'on est sur le plateau
            if (BoardCell::getAlignmentSize(this->_board[from->x][from->y], this->getOppositeDirection(direction)) > 0)
            {
                if (BoardCell::getAlignmentColor(this->_board[from->x][from->y], this->getOppositeDirection(direction)) == color)
                {
                    // on prolonge un alignement existant
                    int alignSize = BoardCell::getAlignmentSize(this->_board[from->x][from->y], this->getOppositeDirection(direction)) + 1;
                    bool closed = BoardCell::isAlignmentClosed(this->_board[from->x][from->y], this->getOppositeDirection(direction));
                    this->_dispatchThis(nextCoord, direction, color, closed, alignSize);
                } else {
                    // on cree un alignement de 1 ferme
                    this->_dispatchThis(nextCoord, direction, color, true, 1);
                }
            } else {
                // Pas d'alignement
                bool closed = false;
                if (this->isBorder(*from) && !this->isValid(this->getNextCell(from, this->getOppositeDirection(direction))))
                    closed = true;
                this->_dispatchThis(nextCoord, direction, color, closed, 1);
            }
        }
    }
}

void Board::dispatchTakings(Coord& from, int color)
{
    for (int direction = TOPLEFT; direction <= LEFT; direction++)
    {
        Coord coord = this->getNextCell(&from, direction);
        if (this->isValid(coord))
        {
            if (BoardCell::isEmpty(*this->getCell(coord)))
            {
                BoardCell::setAlignment(this->getCell(coord), this->getOppositeDirection(direction), 0, 0, 0);
            } else {
                BoardCell::setAlignment(this->getCell(coord), this->getOppositeDirection(direction), 0, 0, 0);
                int newColor = BoardCell::getColor(*this->getCell(coord));
                coord = this->getNextCell(&coord, direction);
                this->_dispatchThis(coord, direction, newColor, false, 1);
            }
        }
    }
}

bool Board::isBorder(Coord& coord)
{
    return (coord.x == 0 || coord.x == (this->_size - 1)
            || coord.y == 0 || coord.y == (this->_size - 1));
}

void Board::_dispatchThis(Coord& coord, int direction, int color, bool closed, int alignSize)
{
    if (this->isValid(coord))
    {
        BoardCell::setAlignment(&this->_board[coord.x][coord.y], this->getOppositeDirection(direction), color, closed, alignSize);
        while (this->isValid(coord) && !BoardCell::isEmpty(this->_board[coord.x][coord.y]))
        {
            if (BoardCell::matchMask(this->_board[coord.x][coord.y], color))
            {
                alignSize++;
                coord = this->getNextCell(&coord, direction);
                if (this->isValid(coord))
                    BoardCell::setAlignment(&this->_board[coord.x][coord.y], this->getOppositeDirection(direction), color, closed, alignSize);
            } else {
                while (this->isValid(coord)
                    && !BoardCell::matchMask(this->_board[coord.x][coord.y], color)
                    && !BoardCell::isEmpty(this->_board[coord.x][coord.y]))
                    {
                    coord = this->getNextCell(&coord, direction);
                    this->_dispatchClosedAlignment(coord, direction);
                }
                break;
            }
        }
    }
}

void Board::_dispatchClosedAlignment(Coord& coord, int direction)
{
    if (this->isValid(coord))
    {
        int newAlignSize = BoardCell::getAlignmentSize(this->_board[coord.x][coord.y], this->getOppositeDirection(direction));
        int newColor = BoardCell::getAlignmentColor(this->_board[coord.x][coord.y], this->getOppositeDirection(direction));
        BoardCell::setAlignment(&this->_board[coord.x][coord.y], this->getOppositeDirection(direction), newColor, true, newAlignSize);
    }
}

bool Board::isValid(Coord const & coord)
{
    return (coord.x < this->_size && coord.y < this->_size);
}

void Board::resetLastPlayed()
{
    if (this->_lastPlayed != 0)
    {
        delete this->_lastPlayed;
        this->_lastPlayed = 0;
    }
}

unsigned int Board::getSize() const
{
    return this->_size;
}

uint64_t* Board::getCell(Coord& coord)
{
    return this->getCell(coord.x, coord.y);
}

uint64_t* Board::getCell(unsigned int x, unsigned int y)
{
    assert(x < this->_size && y < this->_size);
    return &this->_board[x][y];
}

Coord* Board::getLastPlayed()
{
    return this->_lastPlayed;
}

void Board::setLastPlayed(Coord* coord)
{
    this->_lastPlayed = coord;
}

void Board::setError(bool error)
{
    this->_error = error;
}

bool Board::getError()
{
    return this->_error;
}
