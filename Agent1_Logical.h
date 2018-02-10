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
    protected:
    private:
        void deep_course(vector<Case*> v_casesCourante);
        vector< vector<Case*> > all_possibilities;
};

#endif // AGENT1_LOGICAL_H
