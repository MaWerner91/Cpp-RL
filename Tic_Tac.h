#ifndef TIC_TAC_H
#define TIC_TAC_H

#include <iostream>
#include <set>
#include "Tic_Tac.h"
#include "RL_QL.h"

using namespace std;

class TicTacState : public StateVector
{
    private:
    
        int max_moves;
        
    public:
    
        TicTacState(int dims, int st_per_dim);
        bool *get_possible_actions();
        void apply_action(int action, int token);
        void print_state();
        int reward();
};

#endif
