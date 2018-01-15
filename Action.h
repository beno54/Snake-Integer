#ifndef ACTION_H
#define ACTION_H

#include "Grille.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

class Action
{
    public:
        Action(Grille *grid,string ProfilName);
        virtual ~Action();
        void compute_score(vector<Case*> cases_selected);
        void log_data(vector<Case*> cases_selected);
        void log_score();
        int get_groups_in_grid();
        vector< vector<Case*> > get_tab_groups_in_grid();
        int get_nbTurnPlayed();
        void set_nbTurnPlayed();
        void reinitialize_nbTurnPlayed();
        void reinitialize_tmpFile();
    protected:
    private:
        Grille *grid;
        ofstream logFile, scoreFile, tmpFile ;
        int nbTurnPlayed;
        time_t timenow ;
};

#endif // ACTION_H
