#ifndef GAMEINFORMATIONS_H
#define GAMEINFORMATIONS_H

#include <QWidget>

#include "ui_GameInfos.h"

class GameInformations : public QWidget
{
    Q_OBJECT

public:
    GameInformations(QWidget* parent = 0);
    ~GameInformations();

public:
    Ui::gameInformationsGB _ui;
};

#endif // GAMEINFORMATIONS_H
