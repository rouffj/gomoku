#include <iostream>

#include "NewGame.hpp"

NewGame::NewGame(Options& options, QWidget* parent) :
    QDialog(parent),
    _options(options)
{
    this->_ui.setupUi(this);
    connect(this->_ui.radioPvP, SIGNAL(clicked()), this, SLOT(checkPvP()));
    connect(this->_ui.radioPvAI, SIGNAL(clicked()), this, SLOT(checkPvAI()));
    connect(this->_ui.radioAIvAI, SIGNAL(clicked()), this, SLOT(checkAIvAI()));
    connect(this->_ui.comboDificulty, SIGNAL(currentIndexChanged(int)), this, SLOT(chooseDifficulty()));
    connect(this->_ui.buttonBox, SIGNAL(accepted()), this, SLOT(validation()));
    connect(this->_ui.buttonBox, SIGNAL(rejected()), this, SLOT(abort()));
    connect(this, SIGNAL(startNewGame()), parent, SLOT(newGameRequested()));
}

NewGame::~NewGame()
{

}

void NewGame::checkPvP()
{
    std::cout << "Pvp clicked" << std::endl;
    this->_ui.comboDificulty->setDisabled(true);
    this->_ui.checkFirstPlayer->setDisabled(true);
}

void NewGame::checkPvAI()
{
    std::cout << "PvAI clicked" << std::endl;
    this->_ui.comboDificulty->setDisabled(false);
    this->_ui.checkFirstPlayer->setDisabled(false);
}

void NewGame::checkAIvAI()
{
    std::cout << "AIvAI clicked" << std::endl;
    this->_ui.comboDificulty->setDisabled(false);
    this->_ui.checkFirstPlayer->setDisabled(true);
}

void NewGame::chooseDifficulty()
{
    std::cout << "Difficulty chosen" << std::endl;
}

void NewGame::validation()
{
    std::cout << "Validation" << std::endl;
    this->_options.PvP = this->_ui.radioPvP->isChecked();
    this->_options.PvAi = this->_ui.radioPvAI->isChecked();
    this->_options.AivAi = this->_ui.radioAIvAI->isChecked();
    this->_options.Difficulty = this->_ui.comboDificulty->currentIndex() + 1;
    this->_options.AiPlayFirst = this->_ui.checkFirstPlayer->isChecked();
    this->_options.rules.DoubleThree = this->_ui.checkDoubleThree->isChecked();
    this->_options.rules.EndGameTaking = this->_ui.checkEndGameTaking->isChecked();
    this->setHidden(true);
    emit startNewGame();
}

void NewGame::abort()
{
    std::cout << "Abort" << std::endl;
    this->setHidden(true);
}
