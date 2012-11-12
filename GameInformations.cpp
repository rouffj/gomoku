#include "GameInformations.hpp"
#include <iostream>
GameInformations::GameInformations(QWidget*)
{
    this->_ui.setupUi(this);
}

GameInformations::~GameInformations()
{

}

void GameInformations::on_endGameTakingBox_clicked(bool checked)
{
    this->_ui.endGameTakingBox->setChecked(!this->_ui.endGameTakingBox->isChecked());
}

void GameInformations::on_doubleThreeBox_clicked(bool checked)
{
    this->_ui.doubleThreeBox->setChecked(!this->_ui.doubleThreeBox->isChecked());
}
