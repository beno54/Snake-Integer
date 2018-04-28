#ifndef AGENT1B_LOGICAL_H
#define AGENT1B_LOGICAL_H
#include "Struct_Agent.h"

#include <Python.h>
#include "pyhelper.hpp"



class Agent1b_Logical: public Struct_Agent
{
    public:
        Agent1b_Logical(Grille* senseurs, int nb_game2Play, int decision_delay,string ProfilName, int mode);
        Agent1b_Logical();
        virtual ~Agent1b_Logical();
        void compute_decision(int mode,bool affichage);
        void compute_decision_predict(bool affichage);
        void compute_all_possibilities();
        vector< vector<Case*> > compute_all_predict_possibilities(Grille* grid_predict);
        bool has_games2Play();
        void deep_course(vector<Case*> v_casesCourante);
        void deep_course_predict(vector<Case*> v_casesCourante, Grille* grid_predict, vector< vector<Case*> > &all_predict_possibilities);
        float compute_predict_grid_cost(Grille* grid_predict);

        void compute_reward();

        void learn_coeff(int mode);

        float get_destination_reward_same_value();
        float get_destination_reward_multiple_value();
        float get_destination_base3_reward();
        float get_position_reward();
        void learn_cluster(string file_name_coeff);
        void test_copy();

    protected:
    private:

        vector< vector<Case*> > all_possibilities;
        vector< vector<Case*> > all_possibilities_cost;
        vector< float > destination_reward_same_value;
        vector< float > destination_base3_reward;
        vector< float > position_reward;
        vector<float> additionnal_data;
        Grille* copy_grid;
        vector<float> coefficients;

        int choix, mode, nb_game2Play_initial, score_total;

        CPyInstance hInstance;
};
#endif // AGENT1B_LOGICAL_H
