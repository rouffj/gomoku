#include <iostream>
#ifdef WIN32
    #include "Windows.h"
#endif
#include <QtGui/QApplication>
#include <QtGui/QSplashScreen>

#include "Game.hpp"

void displaySplashScreen(int s)
{
    QSplashScreen screen;
    screen.setPixmap(QPixmap(":/images/splash.jpg"));
    screen.show();
#ifdef WIN32
    Sleep(s * 1000);
#else
    sleep(s);
#endif
    screen.hide();
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    // displaySplashScreen(3);

    Game game(a);
    game.initBoard();

    return a.exec();
}
