#include <iostream>
#include "RL_QL.h"
#include "Tic_Tac.h"
#include "TicTacPlay.h"

using namespace std;

PvComp::PvComp(string avf_file, double eps)
{
    avf = new ActionValueFunction(pow(3,9), 9, 0.0, 0.0);
    avf->read(avf_file);
    state = new TicTacState(9, 3);
    pi = new Policy(eps);
    terminal_state = false;  
}


PvComp::~PvComp()
{
    delete avf;
    delete state;
    delete pi;
}


void PvComp::comp_move()
{
    int a = pi->get_action(state, avf);
    state->apply_action(a, 2);
    
    if(state->reward() != 0)
        terminal_state = true;
}


void PvComp::p_move()
{
    bool *possible_actions = state->get_possible_actions();
    int choice;
    
    cout << "Your move: " << endl;
    while(true)
    {
        try
        {
            cin >> choice;
            if(possible_actions[choice] == false)
                throw "This choice is impossible!";
            else
                break;
        }
        catch(...)
        {
            cout << "Invalid input!" << endl;
        }
    }
    
    state->apply_action(choice, 1);
    if(state->reward() != 0)
        terminal_state = true;
}


bool PvComp::check_terminal()
{
    return(terminal_state);
}
