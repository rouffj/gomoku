#ifndef GAMESTEP_HPP
#define GAMESTEP_HPP

#include <list>
#include "Board.hpp"

#define SCORE_ALIGN2 2
#define SCORE_ALIGN3 7
#define SCORE_ALIGN4 20

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
    int pow(int nb, int exp);
    void loadChilds();
    bool isIsolated(Coord& coord);

private:
    Board _board;
    int _playingColor;
    Coord _played;
    int _evaluationValue;
    int _nbStones[2];
    std::list<GameStep*> _nextSteps;
};

#endif // GAMESTEP_HPP
