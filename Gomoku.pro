# -------------------------------------------------
# Project created by QtCreator 2011-06-04T03:49:29
# -------------------------------------------------
QT += svg \
    testlib
TARGET = Gomoku
TEMPLATE = app
SOURCES += main.cpp \
    MainWindow.cpp \
    Board.cpp \
    BoardCell.cpp \
    BoardView.cpp \
    Game.cpp \
    HumanPlayer.cpp \
    Referee.cpp \
    GameInformations.cpp \
    NewGame.cpp \
    IA.cpp \
    GameStep.cpp
HEADERS += Board.hpp \
    BoardCell.hpp \
    Types.hpp \
    BoardView.hpp \
    Game.hpp \
    HumanPlayer.hpp \
    Referee.hpp \
    NewGame.hpp \
    MainWindow.hpp \
    IPlayer.hpp \
    GameInformations.hpp \
    Options.hpp \
    Stone.hpp \
    IA.hpp \
    GameStep.hpp
FORMS += MainWindow.ui \
    GameInfos.ui \
    NewGame.ui
RESOURCES += Images.qrc