#ifndef TYPES_HPP
#define TYPES_HPP

// BoardCell type
typedef unsigned long long uint64_t;

// MASKS
#define EMPTY 0
#define WHITE 1
#define BLACK 2

#define ALIGN_1_OPEN_WHITE      1
#define ALIGN_1_CLOSED_WHITE    9
#define ALIGN_1_OPEN_BLACK      17
#define ALIGN_1_CLOSED_BLACK    25
#define ALIGN_2_OPEN_WHITE      2
#define ALIGN_2_CLOSED_WHITE    10
#define ALIGN_2_OPEN_BLACK      18
#define ALIGN_2_CLOSED_BLACK    26
#define ALIGN_3_OPEN_WHITE      3
#define ALIGN_3_CLOSED_WHITE    11
#define ALIGN_3_OPEN_BLACK      19
#define ALIGN_3_CLOSED_BLACK    27
#define ALIGN_4_OPEN_WHITE      4
#define ALIGN_4_CLOSED_WHITE    12
#define ALIGN_4_OPEN_BLACK      20
#define ALIGN_4_CLOSED_BLACK    28

// DIRECTIONS
#define TOPLEFT 0
#define TOP 1
#define TOPRIGHT 2
#define RIGHT 3
#define BOTTOMRIGHT 4
#define BOTTOM 5
#define BOTTOMLEFT 6
#define LEFT 7

typedef struct s_Coord
{
    s_Coord(unsigned int px, unsigned int py) : x(px), y(py) {}
    s_Coord(s_Coord const & cp) : x(cp.x), y(cp.y) {}

    unsigned int x;
    unsigned int y;
} Coord;

#endif // TYPES_HPP
