#include <iostream>

#include "MainWindow.hpp"
#include "ui_MainWindow.h"
#include "GameInformations.hpp"
#include "ui_GameInfos.h"
#include "Game.hpp"

MainWindow::MainWindow(Options& options, Game& game, RunCallback run, QWidget* parent) :
        QMainWindow(parent),
        _ui(new Ui::MainWindow),
        _closed(false),
        _options(options),
        _game(game),
        _run(run)
{
    this->_ui->setupUi(this);
    GameInformations* gb = new GameInformations();
    this->_ui->horizontalLayout->addWidget(gb, 100);
    this->_ui->graphicsView->setGameInformationsContainer(gb);
    this->_newGameWindow = new NewGame(this->_options, this);
    connect(this->_ui->actionNewGame, SIGNAL(activated()), this, SLOT(openNewGameWindow()));
}

MainWindow::~MainWindow()
{
    delete this->_ui;
}

void MainWindow::changeEvent(QEvent* e)
{
    QMainWindow::changeEvent(e);
    switch (e->type())
    {
    case QEvent::LanguageChange:
        this->_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

bool MainWindow::isClosed()
{
    return this->_closed;
}

BoardView& MainWindow::getBoardView() const
{
    return *this->_ui->graphicsView;
}

void MainWindow::refreshView()
{
    this->_ui->graphicsView->refresh();
}

void MainWindow::closeEvent(QCloseEvent*)
{
    this->_closed = true;
    this->_game.stop();
}

void MainWindow::openNewGameWindow()
{
    this->_newGameWindow->show();
}

void MainWindow::newGameRequested()
{
    std::cout << "New game requested" << std::endl;
    this->_game.setEnd(false);
    (this->_game.*this->_run)();
}
