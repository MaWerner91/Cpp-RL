#include "Tic_Tac.h"
#include <iostream>
#include <set>
#include <string>
#include <climits>

using namespace std;

// TODO: write names of variables in header files

int main(int argc, char *argv[])
{
    int a;
    int old_a[2];
    int token;
    int other_token;
    int old_state[2];
    int max_epoches;
    int max_moves = 9;
    bool write_flag = false;
    double Q;
    double eps;
    double alpha1;
    double alpha2;
    double gamma1;
    double gamma2;
    double draw_rate = 0.0;
    double win_rate[2] = {0, 0};
    set<int> visited_states[2];            // set to keep track which states have been visited
    string outfilename;
    ActionValueFunction *avf[2];
    
    // set default parameters
    max_epoches = 1000;
    alpha1 = 0.9;
    alpha2 = 0.9;
    gamma1 = 0.9;
    gamma2 = 0.9;
    eps = 0.0;
    //outfilename = string("outfilename.out");
    
    // read parameters
    if(argc > 1)
    {
        try
        {
            if(argc % 2 != 1)
                throw "Parameters missing?";
            for(int i = 1; i < argc; i++)
            {
                string str = string(argv[i]);
                if(str.compare("-a1") == 0)
                    alpha1 = atof(argv[i+1]);
                if(str.compare("-a2") == 0)
                    alpha2 = atof(argv[i+1]);
                if(str.compare("-g1") == 0)
                    gamma1 = atof(argv[i+1]);
                if(str.compare("-g2") == 0)
                    gamma2 = atof(argv[i+1]);
                if(str.compare("-e") == 0)
                    max_epoches = atoi(argv[i+1]);
                if(str.compare("-eps") == 0)
                    eps = atof(argv[i+1]);
                if(str.compare("-o") == 0)
                {
                    outfilename = argv[i+1];
                    write_flag = true;
                }
            }
        }
        catch(...)
        {
            cout << "Some error occured while reading parameters! Use default instead." << endl;
        }
    }
    
    avf[0] = new ActionValueFunction(pow(3, 9), 9, alpha1, gamma1);
    avf[1] = new ActionValueFunction(pow(3, 9), 9, alpha2, gamma2);
    Policy pi = Policy(eps);
    
    for(int epoche_count = 0; epoche_count < max_epoches; epoche_count++)
    {
        TicTacState *state = new TicTacState(9, 3);
        int reward = 0;
        
        if(epoche_count == max_epoches-1)
            cout << endl << "Last epoche played" << endl << endl;
        else
            cout << "\r" << ((double) epoche_count)/max_epoches*100.0 << " percent done!" << flush; 
        
        for(int move_count = 0; move_count < max_moves; move_count++)
        {
            // check if state has been visited before
            token = (epoche_count + move_count) % 2;
            visited_states[token].insert(state->map_to_int());
            
            // 1. get action according to policy and apply it
            a = pi.get_action(state, avf[token]);
            old_state[token] = state->map_to_int();
            state->apply_action(a, token+1);
            
            if(move_count > 0)
                old_a[token] = a;
            
            // the last epoche is printed ti console
            if(epoche_count == max_epoches-1)
            {
                cout << endl << "State " << state->map_to_int() << endl;
                state->print_state();
                cout << endl;
            }
            
            // 2. observe reward
            reward = state->reward();
            
            if(reward == 3)
            {
                Q = avf[token]->get_value(old_state[token], old_a[token]);
                Q = Q + avf[token]->alpha*(reward - Q);
                avf[token]->set_value(old_state[token], old_a[token], Q);
                
                //other_token = (token+1) % 2;
                //avf[other_token]->set_value(old_state[other_token], old_a[other_token], -1.0);
                
                win_rate[token] = win_rate[token] + 1.0/max_epoches;                
                break;
            }
            else if(reward == 1)
            {
                avf[token]->set_value(state->map_to_int(), a, 1.0);
                
                other_token = (token+1) % 2;
                avf[other_token]->set_value(old_state[token], old_a[other_token], 1.0);
                
                draw_rate = draw_rate + 1.0/max_epoches;
                break;
            }
            else
            {
                // if state is not terminal, update Q-function
                if(move_count < 1)
                    continue;
                    
                // get action with maximal value in state
                a = pi.get_max_action(state, avf[token]);
                Q = avf[token]->get_value(old_state[token], old_a[token]);
                Q = Q + avf[token]->alpha*(reward + avf[token]->gamma*avf[token]->get_value(state->map_to_int(), a) - Q);
                avf[token]->set_value(old_state[token], old_a[token], Q);
            }
        }
        delete state;
    }
    
    cout << "-----------------------------------------------" << endl << endl;
    cout << "Win-rate Player 1: " << win_rate[0] << endl;
    cout << "Win-rate Player 2: " << win_rate[1] << endl;
    cout << "Draw-rate: " << draw_rate << endl << endl;
    cout << "Training epoches played: " << max_epoches << endl << endl;
    cout << "Player 1:" << endl;
    cout << "\talpha = " << avf[0]->alpha << "\t(learning rate)" << endl;
    cout << "\tgamma = " << avf[0]->gamma << "\t(discount factor)" << endl << endl;;
    cout << "Player 2:" << endl;
    cout << "\talpha = " << avf[1]->alpha << "\t(learning rate)" << endl;
    cout << "\tgamma = " << avf[1]->gamma << "\t(discount factor)" << endl << endl;
    cout << "Player 1 has visited " << visited_states[0].size() << " states." << endl;
    cout << "Player 2 has visited " << visited_states[1].size() << " states." << endl << endl;
    cout << "Used " << eps << "-greedy Policy" << endl << endl;
    cout << "-----------------------------------------------" << endl << endl;
    
    if(write_flag)
        avf[0]->write(outfilename);
    
    delete avf[0];
    delete avf[1];
    
    return(0);
}