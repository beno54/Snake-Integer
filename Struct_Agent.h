#ifndef STRUCT_AGENT_H
#define STRUCT_AGENT_H
#include "Action.h"

class Struct_Agent
{
    public:
        Struct_Agent(Grille* senseurs, int nb_game2Play, int decision_delay);
        virtual ~Struct_Agent();
        virtual void compute_decision () = 0;
    protected:
        Action* action;
        Grille* senseurs;
        int nb_game2Play;
        int decision_delay;
    private:
};

#endif // STRUCT_AGENT_H
