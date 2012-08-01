#include <iostream>
#include <sstream>
#include <QLabel>
#include <QGraphicsEllipseItem>
#include <QPropertyAnimation>

#include "BoardView.hpp"
#include "Stone.hpp"

BoardView::BoardView(QWidget*&) :
        _gameInformations(0),
        _scene(this),
        _board(0),
        _lineSpacing(0),
        _marge(30)
{
    QSize viewSize = this->size();
    this->setRenderHint(QPainter::Antialiasing, true);
    this->_smallestSide = (viewSize.width() > viewSize.height()) ? viewSize.height() - this->_marge : viewSize.width() - this->_marge;
    this->_scene.setSceneRect(0, 0, this->_smallestSide, this->_smallestSide);
    this->setScene(&this->_scene);
}

BoardView::~BoardView()
{

}

void BoardView::setGameInformationsContainer(GameInformations* gameInfo)
{
    this->_gameInformations = gameInfo;
}

void BoardView::setBoard(Board* board)
{
    this->_board = board;
}

void BoardView::draw()
{
    if (this->_board != 0)
    {
        this->_scene.clear();
        this->drawGrid();
        this->drawStones();
    }
}

void BoardView::drawGrid()
{
    if (this->_board != 0)
    {
        this->_lineSpacing = this->_smallestSide / this->_board->getSize();
        unsigned int exceding = this->_smallestSide % this->_board->getSize();
        unsigned int coord = this->_lineSpacing / 2;
        unsigned int i = 0;
        while (i < this->_board->getSize())
        {
            this->_scene.addLine(0, coord, this->_smallestSide - exceding, coord);
            this->_scene.addLine(coord, 0, coord, this->_smallestSide - exceding);
            coord += this->_lineSpacing;
            i++;
        }
    }
    else
        std::cout << "drawGrid(): Warning: No board set in BoardView" << std::endl;
}

void BoardView::drawStones()
{
    if (this->_board != 0)
    {
        Coord* lastCoord = 0;
        QBrush lastBrush;
        for (unsigned int i = 0; i < this->_board->getSize(); i++)
        {
            for (unsigned int j = 0; j < this->_board->getSize(); j++)
            {
                QBrush brush;
                if (BoardCell::matchMask(*this->_board->getCell(i, j), WHITE))
                    brush = QBrush(Qt::white);
                else if (BoardCell::matchMask(*this->_board->getCell(i, j), BLACK))
                    brush = QBrush(Qt::black);
                if (!BoardCell::isEmpty(*this->_board->getCell(i, j)))
                {
                    Coord c = this->getCoord(i, j);
                    if (this->_board->getLastPlayed())
                    {
                        Coord lastCell = this->getCoord(this->_board->getLastPlayed()->x, this->_board->getLastPlayed()->y);
                        if (c.x == lastCell.x && c.y == lastCell.y)
                        {
                            lastCoord = new Coord(c.x, c.y);
                            lastBrush = brush;
                        }
                    }
                    if (!lastCoord || lastCoord->x != c.x || lastCoord->y != c.y)
                    {
                        this->_scene.addEllipse(c.x, c.y,
                                                this->_lineSpacing - this->_lineSpacing / 6,
                                                this->_lineSpacing - this->_lineSpacing / 6,
                                                QPen(Qt::black), brush);
                    }
                }
            }
        }
        if (lastCoord)
        {
            unsigned int size = this->_lineSpacing - this->_lineSpacing / 6;
            QRect startRect(lastCoord->x + 4, lastCoord->y + 4, size - 8, size - 8);
            QRect endRect(lastCoord->x, lastCoord->y, size, size);
            Stone* stone = new Stone();
            stone->setBrush(lastBrush);
            stone->setPen(QPen(Qt::black));
            this->_scene.addItem(stone);
            QPropertyAnimation* animation = new QPropertyAnimation(stone, "rect");
            animation->setDuration(200);
            animation->setStartValue(startRect);
            animation->setEndValue(endRect);
            animation->setEasingCurve(QEasingCurve::OutBounce);
            animation->start();
            delete lastCoord;
        }
    }
    else
        std::cout << "drawStones(): Warning: No board set in BoardView" << std::endl;
}

void BoardView::doError(Coord& lastCoord)
{
    lastCoord = this->getCoord(lastCoord.x, lastCoord.y);
    unsigned int size = this->_lineSpacing - this->_lineSpacing / 6;
    Stone* stone = new Stone();
    stone->setRect(lastCoord.x, lastCoord.y, size, size);
    stone->setBrush(QBrush(Qt::red));
    stone->setPen(QPen(Qt::black));
    stone->setOpacity(0.1);
    this->_scene.addItem(stone);
    QPropertyAnimation* animation = new QPropertyAnimation(stone, "opacity");
    animation->setDuration(200);
    animation->setStartValue(0.3);
    animation->setEndValue(0.0);
    animation->setEasingCurve(QEasingCurve::OutBounce);
    animation->start();
}

void BoardView::refresh()
{
    this->draw();
}

Coord BoardView::getCell(QMouseEvent const & trueCoord)
{
    return this->getCell(trueCoord.x(), trueCoord.y());
}

Coord BoardView::getCell(unsigned int trueX, unsigned int trueY)
{
    unsigned int x = (trueX - this->_marge / 2) / this->_lineSpacing;
    unsigned int y = (trueY - this->_marge / 2) / this->_lineSpacing;
    return Coord(x, y);
}

Coord BoardView::getCoord(unsigned int posX, unsigned int posY)
{
    unsigned int x = posX * this->_lineSpacing + this->_lineSpacing / 12;
    unsigned int y = posY * this->_lineSpacing + this->_lineSpacing / 12;
    return Coord(x, y);
}

void BoardView::resizeEvent(QResizeEvent*)
{
    QSize viewSize = this->size();
    this->_smallestSide = (viewSize.width() > viewSize.height()) ? viewSize.height() - this->_marge : viewSize.width() - this->_marge;
    this->_scene.setSceneRect(0, 0, viewSize.width() - this->_marge, viewSize.height() - this->_marge);
    this->_scene.clear();
    this->draw();
    if (this->_winner.size() > 0)
        this->setWinningText(this->_winner);
}

void BoardView::mousePressEvent(QMouseEvent* e)
{
    if (e->button() == Qt::LeftButton)
    {
        Coord c = this->getCell(*e);
        if (c.x < this->_board->getSize() && c.y < this->_board->getSize())
            this->_board->setLastPlayed(new Coord(c));
    }
}

std::string BoardView::_displayDirectionsInfos(Coord const & c, int direction)
{
    std::stringstream ss;
    int size = BoardCell::getAlignmentSize(*this->_board->getCell(c.x, c.y), direction);
    if (size > 0)
    {
        if (BoardCell::getAlignmentColor(*this->_board->getCell(c.x, c.y), direction) == BLACK)
            ss << "Black" << std::endl;
        else
            ss << "White" << std::endl;
        if (BoardCell::isAlignmentClosed(*this->_board->getCell(c.x, c.y), direction))
            ss << "close " << std::endl;
        else
            ss << "open" << std::endl;
        ss << size;
    }
    else
        ss << "None" << std::endl;
    return ss.str();
}

void BoardView::mouseMoveEvent(QMouseEvent* e)
{
    if (this->_gameInformations != 0)
    {
        Coord c = this->getCell(*e);
        if (c.x < this->_board->getSize() && c.y < this->_board->getSize())
        {
            // Coords
            std::stringstream str;
            str << c.x << "/" << c.y;
            this->_gameInformations->_ui.coordsLabel->setText(str.str().c_str());

            // Colors
            if (BoardCell::matchMask(*this->_board->getCell(c.x, c.y), WHITE))
                this->_gameInformations->_ui.stoneLabel->setText("White");
            else if (BoardCell::matchMask(*this->_board->getCell(c.x, c.y), BLACK))
                this->_gameInformations->_ui.stoneLabel->setText("Black");
            else
                this->_gameInformations->_ui.stoneLabel->setText("None");

            // Takable
            if (BoardCell::isTakable(*this->_board, c))
                this->_gameInformations->_ui.takableLabel->setText("Takable");
            else
                this->_gameInformations->_ui.takableLabel->setText("Not takable");

            // Directions
            this->_gameInformations->_ui.alignTopLeftLabel->setText(this->_displayDirectionsInfos(c, TOPLEFT).c_str());
            this->_gameInformations->_ui.alignTopLabel->setText(this->_displayDirectionsInfos(c, TOP).c_str());
            this->_gameInformations->_ui.alignTopRightLabel->setText(this->_displayDirectionsInfos(c, TOPRIGHT).c_str());
            this->_gameInformations->_ui.alignRightLabel->setText(this->_displayDirectionsInfos(c, RIGHT).c_str());
            this->_gameInformations->_ui.alignBottomRightLabel->setText(this->_displayDirectionsInfos(c, BOTTOMRIGHT).c_str());
            this->_gameInformations->_ui.alignBottomLabel->setText(this->_displayDirectionsInfos(c, BOTTOM).c_str());
            this->_gameInformations->_ui.alignBottomLeftLabel->setText(this->_displayDirectionsInfos(c, BOTTOMLEFT).c_str());
            this->_gameInformations->_ui.alignLeftLabel->setText(this->_displayDirectionsInfos(c, LEFT).c_str());
        }
    }
}

GameInformations& BoardView::getGameInfos()
{
    return *this->_gameInformations;
}

void BoardView::setWinningText(std::string const & text)
{
    this->_winner = text;
    this->getGameInfos()._ui.gameStatusLabel->setText(text.c_str());
    QGraphicsRectItem* rectItem = this->_scene.addRect(0, 0, this->_scene.width(), this->scene()->height(), QPen(Qt::white), QBrush(Qt::white));
    rectItem->setOpacity(0.8);
    QGraphicsTextItem* textItem = this->_scene.addText(text.c_str(), QFont("Ubuntu, Calibri", 24, 600));
    textItem->adjustSize();
    textItem->setPos(((this->_board->getSize() * this->_lineSpacing - textItem->textWidth()) / 2), 100);
}

void BoardView::resetWinner()
{
    this->_winner.clear();
}
