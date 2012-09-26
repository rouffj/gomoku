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
private slots:
    void on_endGameTakingBox_clicked(bool checked);
    void on_doubleThreeBox_clicked(bool checked);
};

#endif // GAMEINFORMATIONS_H
