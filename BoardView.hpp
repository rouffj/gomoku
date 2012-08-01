#ifndef BOARDVIEW_HPP
#define BOARDVIEW_HPP

#include <QGraphicsView>
#include <QMouseEvent>
#include <QGroupBox>
#include <QApplication>
#include <QGraphicsTextItem>
#include <QGraphicsRectItem>

#include "GameInformations.hpp"
#include "Board.hpp"
#include "Types.hpp"

namespace Ui { class MainWindow; }

class BoardView : public QGraphicsView
{
public:
    BoardView(QWidget*& widget);
    virtual ~BoardView();

    void setBoard(Board* board);
    void setGameInformationsContainer(GameInformations* gameInfo);

    void draw();
    void drawGrid();
    void drawStones();
    void refresh();
    void resetWinner();

    Coord getCell(QMouseEvent const & trueCoord);
    Coord getCell(unsigned int trueX, unsigned int trueY);
    Coord getCoord(unsigned int posX, unsigned int posY);
    GameInformations& getGameInfos();
    void setWinningText(std::string const & text);

    void doError(Coord& lastCoord);

protected:
    virtual void resizeEvent(QResizeEvent* e);
    virtual void mousePressEvent(QMouseEvent* e);
    virtual void mouseMoveEvent(QMouseEvent* e);

private:
    std::string _displayDirectionsInfos(Coord const & c, int direction);

private:
    GameInformations* _gameInformations;
    QGraphicsScene _scene;
    Board* _board;
    unsigned int _smallestSide;
    unsigned int _lineSpacing;
    unsigned int _marge;
    std::string _winner;
};

#endif // BOARDVIEW_HPP
