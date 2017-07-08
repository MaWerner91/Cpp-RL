#include "RL_QL.h"
      
//using namespace  std;

StateVector::StateVector(int dims, int st_per_dim)
{
    num_dimensions = dims;
    num_states_per_dim = st_per_dim;
    vector = new int[num_dimensions];
}
        
        
StateVector::~StateVector()
{
    delete[] vector;
}
        
        
int StateVector::map_to_int()
{
    int state_key = 0;
    for(int i= 0; i < num_dimensions; i++)
        state_key = state_key + vector[i]*pow(num_states_per_dim, i);
                
    return(state_key);
}
        
        
// needs to be overloaded for the specific problem
// should return a bool vector, indicating if an action is possible or not
bool *StateVector::get_possible_actions(){}
void StateVector::apply_action(){}
int StateVector::reward(){}


ActionValueFunction::ActionValueFunction(int max_num_states, int max_num_actions, double alph, double gam)
{
    num_actions = max_num_actions;
    num_states = max_num_states;
    alpha = alph;
    gamma = gam;
    action_value_f = gsl_spmatrix_alloc(max_num_states, num_actions);
    gsl_spmatrix_set_zero(action_value_f);
}

        
ActionValueFunction::~ActionValueFunction()
{
    gsl_spmatrix_free(action_value_f);
}

        
void ActionValueFunction::set_value(int state, int action, double value)
{
    gsl_spmatrix_set(action_value_f, state, action, value);
}


void ActionValueFunction::write(string outfilename)
{
    FILE *outfile = fopen(outfilename.data(), "w");
    gsl_spmatrix_fwrite(outfile, action_value_f);
    fclose(outfile);
}


void ActionValueFunction::read(string infilename)
{
    FILE *infile = fopen(infilename.data(), "r");
    gsl_spmatrix_fread(infile, action_value_f);
    fclose(infile);
    
}

        
double ActionValueFunction::get_value(int state, int action)
{
    return(gsl_spmatrix_get(action_value_f, state, action));
}

        
double *ActionValueFunction::get_values_for_state(int state)
{
    double *row = new double[num_actions];
    for(int i = 0; i < num_actions; i++)
        row[i] = gsl_spmatrix_get(action_value_f, state, i);
    return(row);
}

        
int ActionValueFunction::get_num_actions()
{
    return(num_actions);
}

        
int ActionValueFunction::get_num_states()
{
    return(num_states);
}


int ActionValueFunction::get_num_nz()
{
    return(action_value_f->nz);
}


Policy::Policy()
{
    eps = 0.0;
    rng = gsl_rng_alloc(gsl_rng_mt19937);
}

        
Policy::Policy(double epsilon)
{
    eps = epsilon;
    rng = gsl_rng_alloc(gsl_rng_mt19937);
}

        
Policy::~Policy()
{
    gsl_rng_free(rng);
}

        
int Policy::get_action(StateVector *statevec, ActionValueFunction *avf)
{
    int return_action;
    int state = statevec->map_to_int();
    int num_actions = avf->get_num_actions();
    bool *possible_action = statevec->get_possible_actions();
    double *actionvalues = avf->get_values_for_state(state);
    double u1 = gsl_rng_uniform(rng);
    double u2 = gsl_rng_uniform(rng);
          
    // looking for maximal action values
    int max_actions_length = 0;
    int remaining_actions_length = 0;
    int *max_actions = new int[num_actions];
    int *remaining_actions = new int[num_actions];
    double max_value = 0.0;
            
    for(int i = 0; i < num_actions; i++)
    {
        if(actionvalues[i] >= max_value && possible_action[i] == true)
        {
            max_actions[max_actions_length++] = i;
            max_value = actionvalues[i];
            remaining_actions[remaining_actions_length++] = i;
        }
        else if(possible_action[i] == true)
            remaining_actions[remaining_actions_length++] = i;
    }
        
    // choose if greedy or not
    if(1-eps > u1 || remaining_actions_length < 1)
    {
        u2 = u2*max_actions_length;
        return_action = max_actions[(int) floor(u2)];
    }
    else
    {
        u2 = u2*remaining_actions_length;
        return_action = remaining_actions[(int) floor(u2)];
    }
            
    // free allocated memory
    delete[] max_actions;
    delete[] remaining_actions;
    delete[] actionvalues;
    delete[] possible_action;
    max_actions = NULL;
    remaining_actions = NULL;
    actionvalues = NULL;
    possible_action = NULL;
    
    return(return_action);
}


int Policy::get_max_action(StateVector *statevec, ActionValueFunction *avf)
{
    if(eps == 0.0)
        return(this->get_action(statevec, avf));
    else
    {
        double store_eps = eps;
        eps = 0.0;
        int a = this->get_action(statevec, avf);
        eps = store_eps;
        return(a);
    }
}
