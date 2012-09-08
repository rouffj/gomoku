#ifndef DEBUGAI_H
#define DEBUGAI_H

#include "BoardView.hpp"

class DebugAI
{
    QWidget* _minimaxDialog;
    BoardView* _minimaxView;

public:
    DebugAI()
    {
        std::cout << "new debug AI" << std::endl;
        this->_minimaxDialog = static_cast<QWidget*>(new QDialog());
        this->_minimaxView = new BoardView(this->_minimaxDialog);
        this->_minimaxView->show();
    }

    virtual ~DebugAI()
    {
        if (this->_minimaxView != NULL)
            delete this->_minimaxView;
        if (this->_minimaxDialog != NULL)
            delete this->_minimaxDialog;
    }

    void    displayBoard(Board* board)
    {
        this->_minimaxView->setBoard(board);
        this->_minimaxView->draw();
        usleep(100000);
    }
};

#endif // DEBUGAI_H
