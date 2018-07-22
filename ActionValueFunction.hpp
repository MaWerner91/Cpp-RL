#include <gsl/gsl_spmatrix.h>
#include <gsl/gsl_rng.h>
#include <math.h>
#include <iostream>
#include <vector>
#include <string>

template <typename StateType>
class ActionValueFunction
{

public:
    
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

private:
    
    int num_actions;
    int num_states;
    gsl_spmatrix *action_value_f;
    double alpha;
    double gamma;

};

template <typename StateType>
ActionValueFunction<StateType>::ActionValueFunction(int max_num_states, int max_num_actions, double alph, double gam)
{
    num_actions = max_num_actions;
    num_states = max_num_states;
    alpha = alph;
    gamma = gam;
    action_value_f = gsl_spmatrix_alloc(max_num_states, num_actions);
    gsl_spmatrix_set_zero(action_value_f);
}

template <typename StateType>
ActionValueFunction<StateType>::~ActionValueFunction()
{
    gsl_spmatrix_free(action_value_f);
}

template <typename StateType>
void ActionValueFunction<StateType>::set_value(int state, int action, double value)
{
    gsl_spmatrix_set(action_value_f, state, action, value);
}

template <typename StateType>
void ActionValueFunction<StateType>::write(string outfilename)
{
    FILE *outfile = fopen(outfilename.data(), "w");
    gsl_spmatrix_fwrite(outfile, action_value_f);
    fclose(outfile);
}

template <typename StateType>
void ActionValueFunction<StateType>::read(string infilename)
{
    FILE *infile = fopen(infilename.data(), "r");
    gsl_spmatrix_fread(infile, action_value_f);
    fclose(infile);
    
}

template <typename StateType>
double ActionValueFunction<StateType>::get_value(int state, int action)
{
    return(gsl_spmatrix_get(action_value_f, state, action));
}

template <typename StateType>
double *ActionValueFunction<StateType>::get_values_for_state(int state)
{
    double *row = new double[num_actions];
    for(int i = 0; i < num_actions; i++)
        row[i] = gsl_spmatrix_get(action_value_f, state, i);
    return(row);
}

template <typename StateType>
int ActionValueFunction<StateType>::get_num_actions()
{
    return(num_actions);
}

template <typename StateType>
int ActionValueFunction<StateType>::get_num_states()
{
    return(num_states);
}

template <typename StateType>
int ActionValueFunction<StateType>::get_num_nz()
{
    return(action_value_f->nz);
}