#ifndef AGENT2_H
#define AGENT2_H
#include "Struct_Agent.h"

#include <Python.h>
#include "pyhelper.hpp"



class agent2: public Struct_Agent
{
    public:
        agent2(Grille* senseurs, int nb_game2Play, int decision_delay,string ProfilName, int mode);
        agent2();
        virtual ~agent2();
        void compute_decision(int mode,bool affichage);
        void convert_grid2String();
        void call_tensorflow();
        void compute_all_possibilities();
        bool has_games2Play();
        void deep_course(vector<Case*> v_casesCourante);
        void deep_course_predict(vector<Case*> v_casesCourante, Grille* grid_predict, vector< vector<Case*> > &all_predict_possibilities);

    protected:
    private:

        vector<float> additionnal_data;
        vector<float> seeds;
        string param;
        vector< vector<Case*> > all_possibilities;

        int choix, mode, nb_game2Play_initial, score_total;
        vector<float > sorted_cases_dest;
        vector<int > sorted_ind_dest;

        vector<float > sorted_cases_nb;
        vector<int > sorted_ind_nb;

        CPyInstance hInstance;
        CPyObject pName;
        CPyObject pModule;
};
#endif // AGENT2_H

