#ifndef AGENT1_LOGICAL_H
#define AGENT1_LOGICAL_H
#include "Struct_Agent.h"

class Agent1_Logical: public Struct_Agent
{
    public:
        Agent1_Logical(Grille* senseurs, int nb_game2Play);
        virtual ~Agent1_Logical();
        void compute_decision ();
        void compute_possibilitiesInGrps();
        bool has_games2Play();
        void deep_course(vector<Case*> v_casesCourante);
        void compute_possibilities_cost();
        vector< float >  compute_destination_reward();
        vector< float >  compute_random_reward();
        vector< int >compute_destination_base6_reward();

    protected:
    private:

        vector< vector<Case*> > all_possibilities;
        vector< vector<Case*> > all_possibilities_cost;
};

#endif // AGENT1_LOGICAL_H
