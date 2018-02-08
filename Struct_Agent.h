#ifndef STRUCT_AGENT_H
#define STRUCT_AGENT_H
#include "Grille.h"

class Struct_Agent
{
    public:
        Struct_Agent();
        virtual ~Struct_Agent();
        virtual vector<Case*> compute_decision (Grille senseurs) = 0;
    protected:

    private:

};

#endif // STRUCT_AGENT_H
