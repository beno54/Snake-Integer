#ifndef ACTION_H
#define ACTION_H

#include "Grille.h"
#include <iostream>
#include <fstream>
#include <string>

class Action
{
    public:
        Action(Grille *grid,const char* ProfilName);
        virtual ~Action();
        void compute_score(vector<Case*> cases_selected);
        void log_data(vector<Case*> cases_selected);
        int get_groups_in_grid();
        int* get_tab_groups_in_grid();
    protected:
    private:
        Grille *grid;
        ofstream LogFile ;
};

#endif // ACTION_H
