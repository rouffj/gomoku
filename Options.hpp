#ifndef OPTIONS_H
#define OPTIONS_H

#include <iostream>

typedef struct s_Rules
{
    s_Rules() :
        DoubleThree(false),
        EndGameTaking(false) {}

    bool DoubleThree;
    bool EndGameTaking;
} Rules;

typedef struct s_Options
{
    s_Options() :
        PvP(true),
        PvAi(false),
        AivAi(false),
        Difficulty(0),
        AiPlayFirst(false) { }

    void toString()
    {
        std::cout << "[struct Options]" << std::endl << std::boolalpha
                  << "PvP " << this->PvP << std::endl
                  << "PvAi " << this->PvAi << std::endl
                  << "AivAi " << this->AivAi << std::endl
                  << "Difficulty " << this->Difficulty << std::endl
                  << "Ai play first " << this->AiPlayFirst << std::endl
                  << "Double three " << this->rules.DoubleThree << std::endl
                  << "End game taking " << this->rules.EndGameTaking << std::endl
                  << "[End struct Options]" << std::endl;
    }

    bool PvP;
    bool PvAi;
    bool AivAi;
    int Difficulty;
    bool AiPlayFirst;
    Rules rules;
} Options;

#endif // OPTIONS_H
