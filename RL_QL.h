#ifndef RL_QL_H
#define RL_QL_H

#include <gsl/gsl_spmatrix.h>
#include <gsl/gsl_rng.h>
#include <math.h>
#include <iostream>
#include <string>

using namespace std;

/**
    StateVector:
    Base class representing a state in the RL problem.
    
    Parameters:
    
        dims: (int) dimensionality of state space
        st_per_dim: (int) discretization of state space along each axes
        
    Methods:
        
        int map_to_int() :
            maps state to unique integer for identification
        
            Parameters:
                None
            
            Returns:
                state_key: (int) unique number of state
            
        get_possible_actions() :
            virtual method - needs to be adapted for the specific problem.
            Should return an array of possible moves in the current state.
            
        apply_action() :
            virtual method - needs to be adapted for the specific problem.
            Should change vector according to the chosen action.            
            
        reward() :
            virtual method - needs to be adapted for the specific problem.
            Should return the reward in the given state.
**/
class StateVector
{
    protected:
    
        int num_dimensions;
        int num_states_per_dim; // TODO: make this variable for each dimension separately
        int *vector;            // vector representing the state TODO: make this a template
        
    public:
    
        StateVector(int dims, int st_per_dim);
        ~StateVector();
        int map_to_int();
        virtual bool *get_possible_actions();
        virtual void apply_action();
        virtual int reward();
};


/**
    ActionValueFunction:
    Class representing the Q-function.
    
    Parameters:
    
        max_num_states: (int) estimated number of states in the problem. Does not need to be exact
        max_num_actions: (int) maximal number of possible actions in any state
        alph: (double) learning rate alpha for Q-Learning
        gam: (double) discount factor gamma for Q-Learning
    
    Member variables:
    
        alpha: (double) learning rate
        gamma: (double) discount factor
            
    Methods:
        
        set_value:
        Sets Q-function value for a given state-action pair
        
            Parameters:
                state: (int) number of state
                action: (int) number of action
                value: (double) value for this state-action pair
                
            Returns:
                nothing
                
        write:
        Writes sparse matrix to a binary file for storage
        
            Parameters:
                outfilename: (string) filename of outfile
                
            Returns:
                nothing
        
        read:
        Reads sparse matrix from a binary file
        
            Parameters:
                infilename: (string) filename of the infile
                
            Returns:
                nothing
                
        get_value:
        Getter method for the Q-value of a given state-action pair
        
            Parameters:
                state: (int) number of state
                action: (int) number of action
                
            Returns:
                value: (double) Q-value of state-action pair
                
        get_values_for_state:
        Getter method for the Q-values of all actions in state
        
            Parameters:
                state: (int) number of state
                
            Returns:
                values: (double *) array of all Q-values
                
        get_num_actions:
        Getter method for the maximal number of actions used for initialisation
        
            Parameters:
                None
                
            Returns:
                num_actions: (int) number of actions
                
        get_num_states:
        Getter methof for the maximal number of states used for initialisation
        
            Parametes:
                None
            
            Returns_
                num_states: (int) number of states
                
        get_num_nz:
        Getter function for the number of non-zero entries in Q-function
        
            Parameters:
                None
                
            Returns:
                num_nz: (int) number of non-zero entries   
**/
class ActionValueFunction
{
    private:
    
        int num_actions;
        int num_states;
        gsl_spmatrix *action_value_f;
        
    public:
    
        double alpha;
        double gamma;
        ActionValueFunction(int max_num_states, int max_num_actions, double alph, double gam);
        ~ActionValueFunction();
        void set_value(int state, int action, double value);
        void write(string outfilename);
        void read(string infilename);
        double get_value(int state, int action);
        double *get_values_for_state(int state);
        int get_num_actions();
        int get_num_states();
        int get_num_nz();
};


/**
    Policy:
    Class representing the policy used by the agent
    
    Parameters:
    
        eps: (double) (optional) epsilon for epsilon-greedy policy
        
    Methods:
    
        get_action:
        Returns the action to perform in the given state with the given Q-function, using epsilon-greedy policy
        
            Parameters:
                statevec: (StateVector *) pointer to the current state
                avf: (ActionValueFunction *) pointer to current Q-function
                
            Returns:
                action: (int) number of action
                
        get_max_action:
        Returns action with maximal value in the given state
        
            Parameters:
                statevec: (StateVector *) pointer to the current state
                avf: (ActionValueFunction *) pointer to current Q-function
                
            Returns:
                action: (int) number of action
**/
class Policy
{
    private:
    
        double eps;
        gsl_rng *rng;
        
    public:
    
        Policy();
        Policy(double epsilon);
        ~Policy();
        int get_action(StateVector *statevec, ActionValueFunction *avf);
        int get_max_action(StateVector *statevec, ActionValueFunction *avf);
};
#endif
