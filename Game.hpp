#ifndef GAME_HPP
#define GAME_HPP

#include <QtGui/QApplication>

#include "MainWindow.hpp"
#include "Board.hpp"
#include "Types.hpp"
#include "Referee.hpp"
#include "Options.hpp"

class IPlayer;

class Game
{
public:
    Game(QApplication& app);
    ~Game();

    void initBoard(unsigned int boardSize = 19);
    void run();
    bool played(Coord& coord);
    void catchEvents();
    MainWindow & getWindow();

    Board& getBoard();
    Options& getOptions();
    IPlayer* getPlayer(int index);
    int getTurn();
    void setEnd(bool end);

    void stop();
    void restart();

private:
    void _refreshInfos();

private:
    Options _options;
    QApplication& _app;
    MainWindow* _window;
    Board* _board;
    IPlayer* _players[2];
    Referee _referee;
    unsigned int _turn;
    bool _endGame;
};

#endif // GAME_HPP
