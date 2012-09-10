#include "GameInformations.hpp"
#include <iostream>
GameInformations::GameInformations(QWidget*)
{
    this->_ui.setupUi(this);
    std::cout << "GInfo : " << this->width() << "x" << this->height() << std::endl;
}

GameInformations::~GameInformations()
{

}
