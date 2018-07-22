#include <iostream>
#include <memory>
#include "TicTacPlay.h"

using namespace std;

int main(int argc, char *argv[])
{
    int int_input;
    string str_input;
    unique_ptr<PvComp> pvc;
    
    cout << "Choose:" << endl;
    cout << "\t1 - Player vs. Computer" << endl;
    cout << "\t2 - Player vs. Player" << endl;
    
    try
    {
        cin >> int_input;
        if(int_input != 1 && int_input != 2)
            throw 20;
    }
    catch(...)
    {
        cout << "Invalid input!" << endl;
    }
    
    if(int_input == 1)
    {
        cout << "Which Q-function should be used?" << endl;
        while(true)
        {
            try
            {
                cin >> str_input;
                pvc.reset(new PvComp(str_input, 0.1));
                break;
            }
            catch(...){}
        }
        
        while(!pvc->check_terminal())
        {
            pvc->p_move();
            pvc->comp_move();
        }
    }
    
    return(0);
}
