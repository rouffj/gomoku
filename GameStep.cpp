#include "GameStep.hpp"

GameStep::GameStep(Board& baseBoard, int playingColor, Coord const & played) :
        _board(baseBoard),
        _playingColor(playingColor),
        _played(played),
        _evaluationValue(-1)
{
    this->_nbStones[0] = 0;
    this->_nbStones[1] = 0;
}

GameStep::~GameStep()
{
    std::list<GameStep*>::iterator ite = this->_nextSteps.end();
    for (std::list<GameStep*>::iterator it = this->_nextSteps.begin(); it != ite; it++)
        delete *it;
}

int         GameStep::powa(int a, int b)
{
    if (b == 0)
        return 1;
    return a * powa(a, b - 1);
}

int**       GameStep::initPow(int a, int b)
{
    int**     values = new int*[a];
    for (int i = 0; i < a; ++i)
    {
        values[i] = new int[a];
        for (int j = 0; j < b; ++j)
        {
            int tmp = powa(i, j);
            values[i][j] = tmp;
        }
    }
    return values;
}

int GameStep::pow(int nb, int exp)
{    
    static int**   values = initPow(SCORE_ALIGN4 * 2, LEFT + 1);
    return values[nb][exp];
}

int GameStep::evaluate(int color)
{
    int nbAlign2 = 0;
    int nbAlign2Adv = 0;
    int nbAlign3 = 0;
    int nbAlign3Adv = 0;
    int nbAlign4 = 0;
    int nbAlign4Adv = 0;
    int score = 0;

    score += this->getNbStone(color) * this->getNbStone(color) * this->getNbStone(color);
    score -= this->getNbStone((color == WHITE) ? BLACK : WHITE) * this->getNbStone((color == WHITE) ? BLACK : WHITE) * this->getNbStone((color == WHITE) ? BLACK : WHITE);
    for (unsigned int i = 0; i < this->_board.getSize(); i++)
    {
        for (unsigned int j = 0; j < this->_board.getSize(); j++)
        {
            nbAlign2 = 0;
            nbAlign3 = 0;
            nbAlign4 = 0;
            nbAlign2Adv = 0;
            nbAlign3Adv = 0;
            nbAlign4Adv = 0;
            for (int direction = TOPLEFT; direction <= LEFT; direction++)
            {
                int size = BoardCell::getAlignmentSize(*this->_board.getCell(i, j), direction);
                if (size > 1 && !BoardCell::isAlignmentClosed(*this->_board.getCell(i, j), direction))
                {
                    if (size == 2)
                    {
                        if (BoardCell::getAlignmentColor(*this->_board.getCell(i, j), direction) == color)
                            nbAlign2++;
                        else
                            nbAlign2Adv++;
                    } else if (size == 3) {
                        if (BoardCell::getAlignmentColor(*this->_board.getCell(i, j), direction) == color)
                            nbAlign3++;
                        else
                            nbAlign3Adv++;
                    } else if (size == 4) {
                        if (BoardCell::getAlignmentColor(*this->_board.getCell(i, j), direction) == color)
                            nbAlign4++;
                        else
                            nbAlign4Adv++;
                    }
                }
            }
            score += this->pow(SCORE_ALIGN2, nbAlign2);
            score += this->pow(SCORE_ALIGN3, nbAlign3);
            score += this->pow(SCORE_ALIGN4, nbAlign4);
            score -= this->pow(SCORE_ALIGN2, nbAlign2Adv);
            score -= this->pow(SCORE_ALIGN3 + SCORE_ALIGN3 / 2, nbAlign3Adv);
            score -= this->pow(SCORE_ALIGN4 + SCORE_ALIGN4 / 2, nbAlign4Adv);
        }
    }
    return (score);
}

int GameStep::getPlayingColor() const
{
    return this->_playingColor;
}

GameStep* GameStep::getFirstStep()
{
    for (unsigned int i = 0; i < this->_board.getSize(); i++)
    {
        for (unsigned int j = 0; j < this->_board.getSize(); j++)
        {
            Coord c(i, j);
            if (BoardCell::isEmpty(*this->_board.getCell(c)) && !this->isIsolated(c))
            {
                Board newBoard(this->_board);
                int color = (this->_playingColor == WHITE) ? BLACK : WHITE;
                newBoard.addStone(&c, color);
                return new GameStep(newBoard, color, c);
            }
        }
    }
    return 0;
}

GameStep* GameStep::getNextStep(Coord const & coord)
{
    unsigned int i = coord.x;
    unsigned int j;
    bool flag = true;

    while (i < this->_board.getSize())
    {
        if (flag)
        {
            j = coord.y + 1;
            flag = false;
        }
        else
            j = 0;
        while (j < this->_board.getSize())
        {
            Coord c(i, j);
            if (BoardCell::isEmpty(*this->_board.getCell(c)) && !this->isIsolated(c))
            {
                Board newBoard(this->_board);
                int color = (this->_playingColor == WHITE) ? BLACK : WHITE;
                newBoard.addStone(&c, color);
                return new GameStep(newBoard, color, c);
            }
            j++;
        }
        i++;
    }
    return 0;
}

std::list<GameStep*>& GameStep::getNextSteps()
{
    if (this->_nextSteps.empty())
        this->loadChilds();
    return this->_nextSteps;
}

void GameStep::cleanChilds()
{
    std::list<GameStep*>::iterator ite = this->_nextSteps.end();
    for (std::list<GameStep*>::iterator it = this->_nextSteps.begin(); it != ite; ++it)
        delete *it;
    this->_nextSteps.clear();
}

Coord* GameStep::getPlayed()
{
    return new Coord(this->_played);
}

Board& GameStep::getBoard()
{
    return this->_board;
}

void GameStep::loadChilds()
{
    for (unsigned int i = 0; i < this->_board.getSize(); i++)
    {
        for (unsigned int j = 0; j < this->_board.getSize(); j++)
        {
            Coord c(i, j);
            if (BoardCell::isEmpty(*this->_board.getCell(c)) && !this->isIsolated(c))
            {
                Board newBoard(this->_board);
                int color = (this->_playingColor == WHITE) ? BLACK : WHITE;
                newBoard.addStone(&c, color);
                this->_nextSteps.push_back(new GameStep(newBoard, color, c));
            }
        }
    }
}

void GameStep::addStone(int color, int value)
{
    this->_nbStones[color - 1] += value;
}

int GameStep::getNbStone(int color)
{
    return this->_nbStones[color - 1];
}

bool GameStep::isIsolated(Coord& coord)
{
    for (int direction = TOPLEFT; direction <= LEFT; direction++)
    {
        Coord c1 = this->_board.getNextCell(&coord, direction);
        Coord c2 = this->_board.getNextCell(&c1, direction);
        if ((this->_board.isValid(c1) && !BoardCell::isEmpty(*this->_board.getCell(c1)))
            || (this->_board.isValid(c2) && !BoardCell::isEmpty(*this->_board.getCell(c2))))
            return false;
    }
    return true;
}
