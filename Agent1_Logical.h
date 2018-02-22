#ifndef AGENT1_LOGICAL_H
#define AGENT1_LOGICAL_H
#include "Struct_Agent.h"

class Agent1_Logical: public Struct_Agent
{
    public:
        Agent1_Logical(Grille* senseurs, int nb_game2Play, int decision_delay);
        virtual ~Agent1_Logical();
        void compute_decision ();
        void compute_possibilitiesInGrps();
        bool has_games2Play();
        void deep_course(vector<Case*> v_casesCourante);
        int compute_possibilities_cost();
        void compute_destination_reward();
        void compute_random_reward();
        void compute_destination_base3_reward();
        void compute_destination_4_reward();
        void compute_position_reward();

        void get_destination_reward_same_value();
        void get_destination_reward_multiple_value();
        void get_destination_reward_double_value();
        void get_random_reward();
        void get_destination_base3_reward();
        void get_destination_4_reward();
        void get_position_reward();

    protected:
    private:

        vector< vector<Case*> > all_possibilities;
        vector< vector<Case*> > all_possibilities_cost;
        vector< float > destination_reward_same_value;
        vector< float > destination_reward_multiple_value;
        vector< float > destination_reward_double_value;
        vector< float > random_reward;
        vector< int > destination_base3_reward;
        vector< int > destination_4_reward;
        vector< int > position_reward;
        int choix;
};

#endif // AGENT1_LOGICAL_H
