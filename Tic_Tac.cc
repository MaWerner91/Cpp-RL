#include "Tic_Tac.h"

TicTacState::TicTacState(int dims, int st_per_dim) : StateVector(dims, st_per_dim)
{
    max_moves = 9;
    for(int i = 0; i < num_dimensions; i++)
        vector[i] = 0;
}


bool *TicTacState::get_possible_actions()
{
    bool *possible_actions = new bool[num_dimensions];
    
    for(int i = 0; i < num_dimensions; i++)
    {
        if(vector[i] == 0)
            possible_actions[i] = true;
        else
            possible_actions[i] = false;
    }
    
    return(possible_actions);
}


void TicTacState::apply_action(int action, int token)
{
    vector[action] = token;
}


void TicTacState::print_state()
{
    for(int i = 0; i < 3; i++)
    {
        for(int j = 0; j < 3; j++)
        {
            std::cout << vector[3*i + j] << "  ";
        }
        std::cout << std::endl;
    } 
}


int TicTacState::reward()
{
    int reward = 1;
    bool *possible_actions = NULL;
    
    if(vector[0] == vector[3] && vector[0] == vector[6] && vector[0] != 0)
        reward = 3;
    else if(vector[1] == vector[4] && vector[1] == vector[7] && vector[1] != 0)
        reward = 3;
    else if(vector[2] == vector[5] && vector[2] == vector[8] && vector[2] != 0)
        reward = 3;
    else if(vector[0] == vector[1] && vector[0] == vector[2] && vector[0] != 0)
        reward = 3;
    else if(vector[3] == vector[4] && vector[3] == vector[5] && vector[3] != 0)    
        reward = 3;
    else if(vector[6] == vector[7] && vector[6] == vector[8] && vector[6] != 0)    
        reward = 3;
    else if(vector[0] == vector[4] && vector[0] == vector[8] && vector[0] != 0)
        reward = 3;
    else if(vector[2] == vector[4] && vector[2] == vector[6] && vector[2] != 0)
        reward = 3;
    else
    {
        possible_actions = this->get_possible_actions();
        for(int i = 0; i < max_moves; i++)
        {
            if(possible_actions[i] == true)
            {
                reward = 0;
                break;
            }
        }
    }
    
    delete[] possible_actions;
    possible_actions = NULL;
    return(reward);
}
