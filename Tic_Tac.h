#ifndef TIC_TAC_H
#define TIC_TAC_H

#include <iostream>
#include <set>
#include "RL_QL.h"

class TicTacState : public StateVector
{
    private:
    
        int max_moves;
        
    public:
    
        TicTacState(int , int);
        bool *get_possible_actions();
        void apply_action(int, int);
        void print_state();
        int reward();
};

#endif
