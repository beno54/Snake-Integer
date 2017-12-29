#ifndef ACTION_H
#define ACTION_H

#include "Grille.h"

class Action
{
    public:
        Action(Grille *grid);
        virtual ~Action();
        void compute_score(vector<Case*> cases_selected);
    protected:
    private:
        Grille *grid;
};

#endif // ACTION_H
