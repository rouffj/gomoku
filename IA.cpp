#include <QTime>
#include <time.h>
#include "IA.hpp"
#include "GameStep.hpp"

AI::AI(Rules& rules, bool aiDebug) : _debugView(NULL), _toPlay(0), _color(0), _nbStone(0), _rules(rules)
{
    srand(time(NULL));
    std::cout << "AI construction" << std::endl;
    this->_debugView = new DebugAI(aiDebug, true, 200000);
    this->_debugToPlay = new DebugAI(true, false, 0);
}

AI::~AI()
{
    std::cout << "AI destruction" << std::endl;
    delete this->_debugView;
    delete this->_debugToPlay;
}

bool AI::play(Game& game, bool (Game::*callback)(Coord&))
{
    std::cout << "AI playing" << std::endl;
    this->_debugView->resetCnt();
    int nbStones = Board::getNbStones(game.getBoard());

    QTime timer;
    timer.start();
    GameStep* firstStep = new GameStep(game.getBoard(), (this->_color == WHITE) ? BLACK : WHITE, Coord(0, 0));
    firstStep->addStone(game.getPlayer(0)->getColor(), game.getPlayer(0)->getStone());
    firstStep->addStone(game.getPlayer(1)->getColor(), game.getPlayer(1)->getStone());
    this->_debugToPlay->_maxEverScore = -INFINITY;
    this->_debugToPlay->_minEverScore = INFINITY;
    int score = this->minimax(game, firstStep, this->_color, (game.getOptions().Difficulty + 1), -INFINITY, INFINITY);
    if (this->_toPlay == 0)
        this->_toPlay = new Coord(rand() % game.getBoard().getSize(), rand() % game.getBoard().getSize());
    std::cout << "playing in " << this->_toPlay->x << "/" << this->_toPlay->y << " with a score of " << score << " in " << timer.elapsed() << " ms" << std::endl;
    std::cout << "Minimax iterations : " << this->_debugView->getCnt() << ", nbr of sets : " << this->_debugView->addPerfMeasure(timer.elapsed(), nbStones) <<  std::endl;
    std::cout << "Max ever Score = " << this->_debugToPlay->_maxEverScore << ", Min ever score = " << this->_debugToPlay->_minEverScore << std::endl;
    game.getBoard().setLastPlayed(this->_toPlay);
    this->_toPlay = 0;
    delete firstStep;
    bool result = (game.*callback)(*game.getBoard().getLastPlayed());
    while (!result)
    {
        std::cout << "forbidden move. Playing random." << std::endl;
        this->_toPlay = new Coord(rand() % game.getBoard().getSize(), rand() % game.getBoard().getSize());
        game.getBoard().setLastPlayed(this->_toPlay);
        this->_toPlay = 0;
        result = (game.*callback)(*game.getBoard().getLastPlayed());
    }
    return true;
}

int AI::min(int a, int b)
{
    if (a <= b)
        return a;
    return b;
}

int AI::max(int a, int b)
{
    if (a >= b)
        return a;
    return b;
}

int AI::minimax(Game& game, GameStep* gamestep, int color, int depth, int alpha, int beta)
{
    game.catchEvents();
    this->_debugView->incCnt();
    if (this->_debugView->hasDebugWindow())
        this->_debugView->displayBoard(&gamestep->getBoard());
    int maxScore = (gamestep->getPlayingColor() == color) ? INFINITY : -INFINITY;
    // Fin de partie ?
    if (this->isWinning(gamestep->getBoard()))
    {
        if (this->_toPlay != 0)
            delete this->_toPlay;
        this->_toPlay = gamestep->getPlayed();
        std::cout << "One winning at " << this->_toPlay->x << ":" << this->_toPlay->y << std::endl;
        return maxScore;
    }
    // Feuille de la recursion
    if (depth == 0)
        return gamestep->evaluate(color);

    // Recursion sur les fils
    GameStep* nextStep = gamestep->getFirstStep();
    while (nextStep != 0)
    {
        int score = this->minimax(game, nextStep, color, depth - 1, alpha, beta);
/*      std::cout << "SCore is " << score << " at depth " << depth -1 << " maxSCore " << maxScore
                    << " alpha " << alpha << " beta " << beta << " x=" << nextStep->getPlayed()->x
                    << " y=" << nextStep->getPlayed()->y << std::endl;*/
        if (nextStep->getPlayingColor() == color)
        {
            if (score > maxScore)
            {
                /*
                std::cout << "New maxscore = " << score << ", old was " << maxScore
                          << " at depth " << depth << " for color " << color << std::endl;*/
                maxScore = score;
                if (this->_toPlay != 0)
                    delete this->_toPlay;
                this->_toPlay = nextStep->getPlayed();
                this->_debugToPlay->displayBoard(&nextStep->getBoard());
                if (maxScore > alpha)
                    alpha = maxScore;
                if (maxScore > beta)
                    break;
            }
        } else {
            if (score < maxScore)
            {/*
                std::cout << "New minscore = " << score << ", old was " << maxScore
                          << " at depth " << depth << " for color " << color << std::endl;*/
                maxScore = score;
                if (this->_toPlay != 0)
                    delete this->_toPlay;
                this->_debugToPlay->displayBoard(&nextStep->getBoard());
                this->_toPlay = nextStep->getPlayed();
                if (maxScore < beta)
                    beta = maxScore;
                if (maxScore > alpha)
                    break;
            }
        }
        GameStep* toDel = nextStep;
        nextStep = gamestep->getNextStep(*nextStep->getPlayed());
        delete toDel;
    }
    return maxScore;
}

bool AI::isWinningPlayer(GameStep& gamestep)
{
    return (gamestep.getNbStone(gamestep.getPlayingColor()) == 10);
}

bool AI::isWinning(Board& board)
{
    static uint64_t nonEmptyMask = BLACK|WHITE;
    for (unsigned int i = 0; i < board.getSize(); i++)
    {
        for (unsigned int j = 0; j < board.getSize(); j++)
        {
            Coord c(i, j);
            if ((*board.getCell(c))&nonEmptyMask)
                if (this->isWinningMove(board, c))
                    return true;
        }
    }
    return false;
}

bool AI::isWinningMove(Board & board, Coord& coord)
{
    for (int direction = TOPLEFT; direction < BOTTOMRIGHT; direction++)
    {
        int         leftSize = 0;
        int         rightSize = 0;
        uint64_t*   cell = board.getCell(coord.x, coord.y);

        //@todo: check if that 'if mask' is really needed or brings optimisation
        if (BoardCell::matchMask(*cell, BoardCell::getAlignmentColor(*cell, direction)))
            leftSize = BoardCell::getAlignmentSize(*cell, direction);
        if (BoardCell::matchMask(*cell, BoardCell::getAlignmentColor(*cell, board.getOppositeDirection(direction))))
            rightSize = BoardCell::getAlignmentSize(*cell, board.getOppositeDirection(direction));
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

bool AI::isAlignTakable(Board& board, Coord& coord, int direction, int left, int right)
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

int AI::getColor()
{
    return this->_color;
}

void AI::setColor(int color)
{
    this->_color = color;
}

void AI::addTakenStone(int nbStone)
{
    this->_nbStone += nbStone;
}

int AI::getStone()
{
    return this->_nbStone;
}
