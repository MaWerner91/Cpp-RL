#ifndef RL_QL_H
#define RL_QL_H

#include <gsl/gsl_spmatrix.h>
#include <gsl/gsl_rng.h>
#include <math.h>
#include <iostream>
#include <string>


class StateVector
{
    protected:
    
        int num_dimensions;
        int num_states_per_dim;
        int *vector;
        
    public:
    
        StateVector(int dims, int st_per_dim);
        ~StateVector();
        int map_to_int();
        virtual bool *get_possible_actions();
        virtual void apply_action();
        virtual int reward();
};


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
