#ifndef RL_QL_H
#define RL_QL_H

#include <gsl/gsl_spmatrix.h>
#include <gsl/gsl_rng.h>
#include <math.h>
#include <iostream>
#include <string>

using namespace std;

class StateVector
{
    protected:
    
        int num_dimensions;
        int num_states_per_dim;
        int *vector;
        
    public:
    
        StateVector(int, int);
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
        ActionValueFunction(int, int, double, double);
        ~ActionValueFunction();
        void set_value(int, int, double);
        void write(string);
        void read(string);
        double get_value(int, int);
        double *get_values_for_state(int);
        int get_num_actions();
        int get_num_states();
};


class Policy
{
    private:
    
        double eps;
        gsl_rng *rng;
        
    public:
    
        Policy();
        Policy(double);
        ~Policy();
        int get_action(StateVector *, ActionValueFunction *);
        int get_max_action(StateVector *, ActionValueFunction *);
};
#endif
