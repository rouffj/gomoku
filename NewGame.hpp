#ifndef NEWGAME_HPP
#define NEWGAME_HPP

#include <QWidget>
#include <QDialog>

#include "NewGame.hpp"
#include "ui_NewGame.h"
#include "Options.hpp"

class NewGame : public QDialog
{
    Q_OBJECT
public:
    NewGame(Options& options, QWidget *parent = 0);
    ~NewGame();

signals:
    void startNewGame();

public slots:
    void setOptions();
    void chooseDifficulty();
    void validation();
    void abort();

private:
    Ui::NewGameDialog _ui;
    Options& _options;
};

#endif // NEWGAME_HPP
