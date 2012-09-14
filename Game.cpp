#include <sstream>
#include <QTime>

#include "HumanPlayer.hpp"
#include "IA.hpp"
#include "Game.hpp"

Game::Game(QApplication& app) :
        _app(app),
        _window(0),
        _board(0),
        _referee(_options.rules),
        _turn(0),
        _endGame(false)
{
    this->_players[0] = 0;
    this->_players[1] = 0;
    this->_window = new MainWindow(_options, *this, &Game::run);
}

Game::~Game()
{
    if (this->_players[0])
        delete this->_players[0];
    if (this->_players[1])
        delete this->_players[1];
    if (this->_window)
        delete this->_window;
    if (this->_board)
        delete this->_board;
}

void Game::initBoard(unsigned int boardSize)
{
    this->_board = new Board(boardSize);
    this->_window->show();
    this->_window->getBoardView().setBoard(this->_board);
    this->_window->getBoardView().draw();
    this->_window->getBoardView().setDisabled(true);
}

void Game::run()
{
    while (this->_board)
    {
        this->restart();
        this->_options.toString();
        QTime time;
        while (42)
        {
            if (!this->_endGame)
            {
                bool played = false;
                std::stringstream turnTimeStream;
                turnTimeStream << "Turn time : ";
                time.restart();
                while (!played)
                {
                    this->_board->resetLastPlayed();
                    played = this->_players[this->_turn % 2]->play(*this, &Game::played);
                }
                turnTimeStream << time.elapsed() << " ms";
                this->_window->getBoardView().getGameInfos()._ui.turnTimeLabel->setText(turnTimeStream.str().c_str());
                std::stringstream timeStream;
                timeStream << "Propagation time : ";
                time.restart();
                if (this->_window->isClosed())
                    break;
                this->_board->addStone(this->_board->getLastPlayed(), this->_players[this->_turn % 2]->getColor());
                this->_referee.doTakings(*this->_board, this->_players, this->_turn);
                this->_window->getBoardView().draw();
                if (this->_referee.isWinning(*this->_board)
                    || this->_referee.isWinningPlayer(this->_players))
                    {
                    std::stringstream ss;
                    ss << "Winner : Player " << this->_turn % 2 + 1;
                    this->getWindow().getBoardView().setWinningText(ss.str());
                    this->_window->getBoardView().setDisabled(true);
                    this->_endGame = true;
                }
                timeStream << time.elapsed() << " ms";
                this->_window->getBoardView().getGameInfos()._ui.timeLabel->setText(timeStream.str().c_str());
                this->_turn++;
            }
            this->_refreshInfos();
            this->_app.processEvents();
            if (this->_window->isClosed())
                break;
        }
    }
}

bool Game::played(Coord& coord)
{
    if (!this->_referee.isValidMove(*this->_board, coord, this->_players[this->_turn % 2]->getColor()))
    {
        this->_window->getBoardView().doError(coord);
        return false;
    }
    return true;
}

void Game::catchEvents()
{
    this->_app.processEvents();
}

Board& Game::getBoard()
{
    return *this->_board;
}

void Game::setEnd(bool end)
{
    this->_endGame = end;
}

MainWindow& Game::getWindow()
{
    return (*this->_window);
}

IPlayer* Game::getPlayer(int index)
{
    if (index < 2)
        return this->_players[index];
    return 0;
}

int Game::getTurn()
{
    return this->_turn;
}

Options& Game::getOptions()
{
    return this->_options;
}

void Game::stop()
{
    if (this->_board)
    {
        delete this->_board;
        this->_board = 0;
    }
    else
        std::cerr << "Warning: there is no board" << std::endl;
}

void Game::restart()
{
    std::cout << "Game is starting..." << std::endl;
    this->_window->getBoardView().getGameInfos()._ui.gameStatusLabel->setText("Game started");
    this->_window->getBoardView().resetWinner();
    if (this->_board)
    {
        delete this->_board;
        this->_turn = 0;
        this->initBoard();
        this->_window->getBoardView().setDisabled(false);
    }
    else
        std::cerr << "Warning: there is no board" << std::endl;
    if (this->_players[0])
        delete this->_players[0];
    if (this->_players[1])
        delete this->_players[1];
    this->_players[0] = (this->_options.PvP || (this->_options.PvAi && !this->_options.AiPlayFirst)) ? (IPlayer*) new HumanPlayer(this->_options.rules) : (IPlayer*) new AI(this->_options.rules, this->_options.AiDebug);
    this->_players[0]->setColor(WHITE);
    this->_players[1] = (this->_options.PvP || (this->_options.PvAi && this->_options.AiPlayFirst)) ? (IPlayer*) new HumanPlayer(this->_options.rules) : (IPlayer*) new AI(this->_options.rules, this->_options.AiDebug);
    this->_players[1]->setColor(BLACK);
}

void Game::_refreshInfos()
{
    GameInformations& gi = this->_window->getBoardView().getGameInfos();
    std::ostringstream turn;
    turn << "Turn : " << this->_turn + 1;
    gi._ui.turnLabel->setText(turn.str().c_str());
    if (this->_options.PvP)
    {
        gi._ui.playerOneLabel->setText("Player 1 : Human (white)");
        gi._ui.playerTwoLabel->setText("Player 2 : Human (black)");
    }
    else if (this->_options.PvAi)
    {
        if (this->_options.AiPlayFirst)
        {
            gi._ui.playerOneLabel->setText("Player 1 : A.I. (white)");
            gi._ui.playerTwoLabel->setText("Player 2 : Human (black)");
        }
        else
        {
            gi._ui.playerOneLabel->setText("Player 1 : Human (white)");
            gi._ui.playerTwoLabel->setText("Player 2 : A.I. (black)");
        }
    }
    else
    {
        gi._ui.playerOneLabel->setText("Player 1 : A.I. (white)");
        gi._ui.playerTwoLabel->setText("Player 2 : A.I. (black)");
    }
    std::stringstream ss1;
    ss1 <<  gi._ui.playerOneLabel->text().toStdString();
    ss1 << " - " << this->_players[0]->getStone() << " stones";
    gi._ui.playerOneLabel->setText(ss1.str().c_str());
    std::stringstream ss2;
    ss2 <<  gi._ui.playerTwoLabel->text().toStdString();
    ss2 << " - " << this->_players[1]->getStone() << " stones";
    gi._ui.playerTwoLabel->setText(ss2.str().c_str());
    if (this->_options.PvAi || this->_options.AivAi)
    {
        if (this->_options.Difficulty == 0)
            gi._ui.difficultyLabel->setText("Difficulty : Easy");
        else if (this->_options.Difficulty == 1)
            gi._ui.difficultyLabel->setText("Difficulty : Medium");
        else
            gi._ui.difficultyLabel->setText("Difficulty : Hard");
    }
    if (this->_options.rules.DoubleThree)
        gi._ui.doubleThreeLabel->setText("Doubre three opt. : on");
    else
        gi._ui.doubleThreeLabel->setText("Doubre three opt. : off");
    if (this->_options.rules.EndGameTaking)
        gi._ui.endGameTakingLabel->setText("End game taking opt. : on");
    else
        gi._ui.endGameTakingLabel->setText("End game taking opt. : off");
    if (this->_turn % 2 == 0)
    {
        gi._ui.playerOneLabel->setFont(QFont("Ubuntu, Calibri", 11, 600, false));
        gi._ui.playerTwoLabel->setFont(QFont("Ubuntu, Calibri", 11, 0, false));
    }
    else
    {
        gi._ui.playerOneLabel->setFont(QFont("Ubuntu, Calibri", 11, 0, false));
        gi._ui.playerTwoLabel->setFont(QFont("Ubuntu, Calibri", 11, 600, false));
    }
}
