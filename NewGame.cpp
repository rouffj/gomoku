#include <iostream>

#include "NewGame.hpp"

NewGame::NewGame(Options& options, QWidget* parent) :
    QDialog(parent),
    _options(options)
{
    this->_ui.setupUi(this);
   // connect(this->_ui.radioPvP, SIGNAL(clicked()), this, SLOT(checkPvP()));
   // connect(this->_ui.radioPvAI, SIGNAL(clicked()), this, SLOT(checkPvAI()));
   // connect(this->_ui.radioAIvAI, SIGNAL(clicked()), this, SLOT(checkAIvAI()));
    connect(this->_ui.comboGameType, SIGNAL(currentIndexChanged(int)), this, SLOT(setOptions()));
    connect(this->_ui.comboDificulty, SIGNAL(currentIndexChanged(int)), this, SLOT(chooseDifficulty()));
    connect(this->_ui.buttonBox, SIGNAL(accepted()), this, SLOT(validation()));
    connect(this->_ui.buttonBox, SIGNAL(rejected()), this, SLOT(abort()));
    connect(this, SIGNAL(startNewGame()), parent, SLOT(newGameRequested()));
}

NewGame::~NewGame()
{

}

void NewGame::setOptions()
{
    bool    ai_in_game = this->_ui.comboGameType->currentIndex() > 0;
    this->_ui.checkFirstPlayer->setEnabled(ai_in_game);
    this->_ui.comboDificulty->setEnabled(ai_in_game);
    this->_ui.minimaxDebug->setEnabled(ai_in_game);
    if (ai_in_game)
    {
        this->_ui.firstPlayerLabel->setStyleSheet("color: white;");
        this->_ui.debugLabel->setStyleSheet("color: white;");
    }
    else
    {
        this->_ui.firstPlayerLabel->setStyleSheet("color: grey;");
        this->_ui.debugLabel->setStyleSheet("color: grey;");
    }
}

void NewGame::chooseDifficulty()
{
    std::cout << "Difficulty chosen" << std::endl;
}

void NewGame::validation()
{
    std::cout << "Validation" << std::endl;
    this->_options.PvP = false;
    this->_options.PvAi = false;
    this->_options.AivAi = false;
    switch (this->_ui.comboGameType->currentIndex())
    {
        case 0:
            this->_options.PvP = true;
                break;
        case 1:
            this->_options.PvAi = true;
            break;
        case 2:
            this->_options.AivAi = true;
            break;
    }
    this->_options.Difficulty = this->_ui.comboDificulty->currentIndex();
    this->_options.AiDebug = this->_ui.minimaxDebug->isChecked();
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
