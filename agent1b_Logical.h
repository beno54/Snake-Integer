#ifndef AGENT1B_LOGICAL_H
#define AGENT1B_LOGICAL_H
#include "Struct_Agent.h"

#include <Python.h>
#include "pyhelper.hpp"

#define MAX_COEFF 10

class Agent1b_Logical: public Struct_Agent
{
    public:
        Agent1b_Logical(Grille* senseurs, int nb_game2Play, int decision_delay,string ProfilName, int mode);
        Agent1b_Logical();
        virtual ~Agent1b_Logical();
        void compute_decision (int mode,bool affichage);
        void compute_all_possibilities();
        bool has_games2Play();
        void deep_course(vector<Case*> v_casesCourante);
        void compute_possibilities_cost(int option);

        void compute_reward();

        void learn_coeff(int mode);

        float get_destination_reward_same_value();
        float get_destination_reward_multiple_value();
        float get_random_reward();
        float get_destination_base3_reward();
        float get_position_reward();

    protected:
    private:

        vector< vector<Case*> > all_possibilities;
        vector< vector<Case*> > all_possibilities_cost;
        vector< float > destination_reward_same_value;
        vector< float > destination_reward_multiple_value;
        vector< float > random_reward;
        vector< float > destination_base3_reward;
        vector< float > position_reward;
        vector<float> additionnal_data;

        vector<int> coefficients;

        int choix, mode, nb_game2Play_initial, score_total;

        CPyInstance hInstance;
};
#endif // AGENT1B_LOGICAL_H
