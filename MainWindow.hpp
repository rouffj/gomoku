#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QEvent>

#include "BoardView.hpp"
#include "NewGame.hpp"
#include "Options.hpp"

class Game;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
    Q_OBJECT

typedef void (Game::*RunCallback)();

public:
    MainWindow(Options& options, Game& game, RunCallback run, QWidget* parent = 0);
    ~MainWindow();

    BoardView& getBoardView() const;
    void refreshView();
    bool isClosed();

signals:

public slots:
    void openNewGameWindow();
    void newGameRequested();

protected:
    void changeEvent(QEvent* e);
    void closeEvent(QCloseEvent* e);

private:
    Ui::MainWindow* _ui;
    NewGame* _newGameWindow;
    bool _closed;
    Options& _options;
    Game& _game;
    RunCallback _run;
};

#endif // MAINWINDOW_H
