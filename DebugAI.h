#ifndef DEBUGAI_H
#define DEBUGAI_H

#include <map>
#include <iostream>
#include <fstream>

#include "BoardView.hpp"

typedef struct   s_perf
{
    int  _time;
    int  _cnt;
    int  _stones;
}                perf;

class DebugAI
{
    QWidget*    _minimaxDialog;
    BoardView*  _minimaxView;
    int         _minimaxCnt;
    std::vector<perf>    _perfs;
    bool        _savePerfs;
    int         _waitTime;

public:

    int         _maxEverScore;
    int         _minEverScore;

    DebugAI(bool hasDebugWindow, bool savePerfs, int waitTime) : _minimaxDialog(NULL), _minimaxView(NULL)
    {
        std::cout << "new debug AI" << std::endl;
        if (hasDebugWindow)
        {
            this->_minimaxDialog = static_cast<QWidget*>(new QDialog());
            this->_minimaxView = new BoardView(this->_minimaxDialog);
            this->_minimaxView->draw();
            this->_minimaxView->show();
        }
        this->_minimaxCnt = 0;
        this->_perfs.clear();
        this->_savePerfs = savePerfs;
        this->_waitTime = waitTime;
    }

    virtual ~DebugAI()
    {
        if (this->_minimaxView != NULL)
            delete this->_minimaxView;
        if (this->_minimaxDialog != NULL)
            delete this->_minimaxDialog;

        if (this->_savePerfs)
        {
            std::ofstream outputFile;
            std::cout << "DEBUG AI DESTRUCTION!" << std::endl;
            outputFile.open("programData.txt");
            if (!outputFile.is_open())
                std::cout << "Cant open file !!!!!" << std::endl;
            outputFile << "# X Y\n";
            for (int i = 0; i < this->_perfs.size(); ++i)
            {/*
                outputFile << i;
                outputFile << " ";*/
                outputFile << (this->_perfs[i])._cnt;
                outputFile << " ";
                outputFile << (this->_perfs[i])._time;
                outputFile << " ";
                outputFile << (this->_perfs[i])._stones;
                outputFile << "\n";
            }
            outputFile.close();
        }
        this->_perfs.clear();
    }

    bool    hasDebugWindow()
    {
        return (this->_minimaxDialog != NULL);
    }

    void    displayBoard(Board* board)
    {
        this->_minimaxView->setBoard(board);
        this->_minimaxView->draw();
        if (this->_waitTime)
            usleep(this->_waitTime);
    }

    void    resetCnt()
    {
        this->_minimaxCnt = 0;
    }

    void    incCnt()
    {
        ++this->_minimaxCnt;
    }

    int     getCnt()
    {
        return this->_minimaxCnt;
    }

    int     addPerfMeasure(int time)
    {
        perf    nPerf;
        nPerf._cnt = this->_minimaxCnt;
        nPerf._time = time;
        this->_perfs.push_back(nPerf);
        return this->_perfs.size();
    }

    int     addPerfMeasure(int time, int nbStones)
    {
        perf    nPerf;
        nPerf._cnt = this->_minimaxCnt;
        nPerf._time = time;
        nPerf._stones = nbStones;
        this->_perfs.push_back(nPerf);
        return this->_perfs.size();
    }
};

#endif // DEBUGAI_H
