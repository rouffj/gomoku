#ifndef GAMESTEP_HPP
#define GAMESTEP_HPP

#include <list>
#include <sys/types.h>
#include <limits.h>
#include "Board.hpp"

#define SCORE_ALIGN2 2
#define SCORE_ALIGN3 7
#define SCORE_ALIGN4 20

/*
template<int64_t B, int64_t N>
struct      Pow
{
    enum { value = B*Pow<B, N-1>::value };
};

template<int64_t B>
struct      Pow<B, 0>
{
    enum { value = 1 };
};

template<int64_t B, int64_t N>
int**       initPow()
{
    int**     values = new int*[B];
    for (int i = 0; i < B; ++i)
    {
        values[i] = new int[B];
        for (int j = 0; j < N; ++j)
        {
            int64_t tmp = Pow<i, j>::value;
            values[i][j] = (int)(tmp);
        }
    }
    return values;
}
*/

class GameStep
{
public:
    GameStep(Board& board, int playingColor, Coord const & played);
    ~GameStep();

    std::list<GameStep*>& getNextSteps();
    int getPlayingColor() const;
    Coord* getPlayed();
    int evaluate(int color);
    void cleanChilds();
    Board& getBoard();
    void addStone(int color, int value);
    int getNbStone(int color);
    int nbThreeMove(int color);
    bool isThreeMove(int direction, int color);

    GameStep* getFirstStep();
    GameStep* getNextStep(Coord const & coord);

    void doTakings();

private:
    int powa(int a, int b);
    int** initPow(int a, int b);
    int pow(int nb, int exp);
    void loadChilds();
    bool isIsolated(Coord& coord);

private:
    Board _board;
    int _playingColor;
    Coord _played;
    int _evaluationValue;
    int _nbStones[2];    // These two vars are for the number of eaten stones
    std::list<GameStep*> _nextSteps;
};

#endif // GAMESTEP_HPP
