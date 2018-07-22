#ifndef TIC_TAC_PLAY_H
#define TIC_TAC_PLAY_H

#include <iostream>
#include "Tic_Tac.h"
#include "RL_QL.h"

class PvComp
{
    private:
    
        ActionValueFunction *avf;
        Policy *pi;
        TicTacState *state;
        bool terminal_state;
        
    public:
    
        PvComp(std::string avf_file, double eps);
        ~PvComp();
        void comp_move();
        void p_move();
        bool check_terminal();
};

#endif
