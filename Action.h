#ifndef ACTION_H
#define ACTION_H

#include "Grille.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <thread>
#include <chrono>
#include <windows.h>

class Action
{
    public:
        Action(Grille *grid,string ProfilName);
        virtual ~Action();
        void compute_score(vector<Case*> cases_selected);
        void compute_predict_score(vector<Case*> cases_selected, Grille* grid_predict);
        void log_data(vector<Case*> cases_selected);
        void log_data(vector<Case*> cases_selected, vector<float> additionnal_data);
        void log_score();
        void log_score( vector<int> additionnal_data);
        //void log_score(vector<float> additionnal_data);

        int get_nbTurnPlayed();
        void set_nbTurnPlayed();
        void reinitialize_nbTurnPlayed();
        void reinitialize_tmpFile();

        void calcul_groups_in_grid ();
        void compute_NbPossibilities_in_grid();
        int get_NbpPossibilities_in_grid();
        vector< vector<Case*> > get_tab_groups_in_grid();
        bool test_case_selected(vector<Case*> cases_selected) ;
        void affiche_cases_selected(vector<Case*> cases_selected, int time_sleep);
        void reset();
        void reset(vector<float> seed);
    protected:
    private:
        Grille *grid;
        ofstream logFile, scoreFile, tmpFile ;
        int nbTurnPlayed,nbPossibilities;
        time_t timenow ;
        vector< vector<Case*> > groups_in_grid;

};

#endif // ACTION_H
