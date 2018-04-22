#include "Agent1b_Logical.h"

Agent1b_Logical::Agent1b_Logical(Grille* senseurs, int nb_game2Play, int decision_delay,string ProfilName, int mode): Struct_Agent(senseurs, nb_game2Play, decision_delay, ProfilName)
{
    //ctor
    srand (time(NULL));
    action = new Action(senseurs, ProfilName);
    this->mode = mode;
    /*INITIALISATION*/
    for (int i = 0; i < 6; i++)
    {
        coefficients.push_back(0);
    }
    score_total = 0;
    nb_game2Play_initial = nb_game2Play;

    //coef present
    coefficients[0] = 15;
    coefficients[1] = 24;
    coefficients[2] = 16;
    coefficients[3] = 22;

    //coef futur
    coefficients[4] = 8;
    coefficients[5] = 15;


    copy_grid = new Grille(Vector2f (250, 100), 450, NULL);
}

Agent1b_Logical::Agent1b_Logical(): Struct_Agent()
{
    nb_game2Play_initial = 0 ;
}

Agent1b_Logical::~Agent1b_Logical()
{
    //dtor
    delete(copy_grid);
    delete (action);
}


/*FONCTIONS DETECTION GAMEOVER*/
bool Agent1b_Logical::has_games2Play()
{
    bool monbool = true;
    if (nb_game2Play <= 0)
    {
        monbool = false;

    }
    return monbool;
}


/*FONCTIONS PARCOURS DES POSS*/
void Agent1b_Logical::compute_all_possibilities()
{
    all_possibilities.clear();
    vector<Case*>  tab_group = senseurs->get_Cases();

    for (int i = 0; i < tab_group.size(); i ++)
    {
            vector<Case*> racine;
            racine.push_back(tab_group[i]);
            deep_course(racine);
    }
}

/*FONCTIONS PARCOURS DES POSS*/
vector< vector<Case*> > Agent1b_Logical::compute_all_predict_possibilities(Grille* grid_predict)
{
    vector< vector<Case*> > all_predict_possibilities;
    vector<Case*>  tab_group = grid_predict->get_Cases();

    for (int i = 0; i < tab_group.size(); i ++)
    {
            vector<Case*> racine;
            racine.push_back(tab_group[i]);
            deep_course_predict(racine, grid_predict, all_predict_possibilities);
    }

    return all_predict_possibilities;
}

void Agent1b_Logical::deep_course(vector<Case*> v_casesCourante)
{
    vector<Case*> vector_chemin = v_casesCourante;
    vector<Case*> voisins = senseurs->get_voisins(vector_chemin.back());

    for (int k = 0; k < voisins.size(); k ++)
    {
        for (int z = 0; z < vector_chemin.size(); z ++)
        {
            if (voisins[k]->get_id() == vector_chemin[z]->get_id())
            {
                //on retire les voisins d?j? pr?sents dans le vector_casesCourante et on ajuste l'indice du vector voisins
                voisins.erase(voisins.begin()+k);
                z = 26;
                k --;
            }
        }
    }

    for (int k = 0; k < voisins.size(); k ++)
    {
        vector_chemin.push_back(voisins[k]);

        all_possibilities.push_back(vector_chemin);
        deep_course(vector_chemin);
        vector_chemin.pop_back();
    }
}

void Agent1b_Logical::deep_course_predict(vector<Case*> v_casesCourante, Grille* grid_predict, vector< vector<Case*> > &all_predict_possibilities)
{
    vector<Case*> vector_chemin = v_casesCourante;
    vector<Case*> voisins = grid_predict->get_voisins(vector_chemin.back());

    for (int k = 0; k < voisins.size(); k ++)
    {
        for (int z = 0; z < vector_chemin.size(); z ++)
        {
            if (voisins[k]->get_id() == vector_chemin[z]->get_id())
            {
                //on retire les voisins d?j? pr?sents dans le vector_casesCourante et on ajuste l'indice du vector voisins
                voisins.erase(voisins.begin()+k);
                z = 26;
                k --;
            }
        }
    }

    for (int k = 0; k < voisins.size(); k ++)
    {
        vector_chemin.push_back(voisins[k]);

        all_predict_possibilities.push_back(vector_chemin);
        deep_course_predict(vector_chemin, grid_predict, all_predict_possibilities);
        vector_chemin.pop_back();
    }
}


//FONCTIONS  DE PRISE DE DECISION/
void Agent1b_Logical::compute_decision(int mode,bool affichage)
{
    if (!senseurs->get_isOver())
    {
        compute_all_possibilities();
        //compute_possibilities_cost(mode);

        if (action->test_case_selected(all_possibilities[choix]) == true)
        {
            if (affichage == true)
            {
                action->affiche_cases_selected(all_possibilities[choix], decision_delay);
            }

            action->log_data(all_possibilities[choix], additionnal_data);

            action->compute_score(all_possibilities[choix]);
        }
    }
    else
    {
        score_total += senseurs->get_Case_score()->get_value();
        action->log_score(coefficients);
        action->reset();
        nb_game2Play --;
    }
}

/*FONCTIONS MAJ PARAMETRES */

void  Agent1b_Logical::compute_reward()
{
    destination_reward_same_value.clear();

    destination_base3_reward.clear();
    position_reward.clear();
    vector<float> probabilities ;
    int destvalue  ;

    for (int z = 0; z < all_possibilities.size(); z ++)
    {
        //calcul voisins dest
        vector<Case*> voisins_dest = senseurs->get_all_voisins( all_possibilities[z].back());

        //calcul position param
        int nb_voisins_tot = voisins_dest.size();
        float reward_position = 0 ;
        switch (nb_voisins_tot)
        {
            case 2 : reward_position = 1 ; break ;
            case 3 : reward_position = 0.5 ; break ;
        }

        //retire voisins present dans le chemin
        for (int i =0; i < all_possibilities[z].size(); i++)
        {
            for (int e =0; e < voisins_dest.size(); e++)
            {
                if(voisins_dest[e]->get_id() == all_possibilities[z][i]->get_id())
                {
                    voisins_dest.erase(voisins_dest.begin()+ e);
                    e--;
                }
            }
        }
        //calcul destination
        float reward = 0;
        for (int x = 0; x < voisins_dest.size(); x ++)
        {
            destvalue = (all_possibilities[z].size())*(all_possibilities[z].back())->get_value();

            if (voisins_dest[x]->get_value() == destvalue )
            {
                reward++;
            }
        }

        //calcul param base 3
        float reward_b3 = 0;
        int number = (all_possibilities[z].back()->get_value())*(all_possibilities[z].size());
        while (number%2 == 0)
        {
            if (number/2 == 3)
            {
                reward_b3 = 1;
            }
            number = number/2;
        }

        //coefficient entre 1 et 3
        destination_base3_reward.push_back(reward_b3);
        position_reward.push_back(reward_position);
        destination_reward_same_value.push_back(reward/3.0f);

    }

}


/*FONCTIONS  DE PRISE DE DECISION*/
void Agent1b_Logical::compute_decision_predict(bool affichage)
{
    if (!senseurs->get_isOver())
    {
        compute_all_possibilities();
        test_copy();

        if (action->test_case_selected(all_possibilities[choix]) == true)
        {
            if (affichage == true)
            {
                action->affiche_cases_selected(all_possibilities[choix], decision_delay);
            }

            //action->log_data(all_possibilities[choix], additionnal_data);

            action->compute_score(all_possibilities[choix]);
        }
    }
    else
    {
        score_total += senseurs->get_Case_score()->get_value();
        action->log_score(coefficients);
        action->reset();
        nb_game2Play --;
    }
}


float Agent1b_Logical::compute_predict_grid_cost(Grille* grid_predict)
{
    float reward_total = 0;

    vector< vector<Case*> > groups = action->calcul_groups_in_grid(grid_predict);

    for (int i = 0; i < groups.size(); i ++)
    {
        if (groups[i].size() == 1)
        {
            groups.erase(groups.begin()+i);
            i --;
        }
    }
    vector<Case*> all_cases = grid_predict->get_Cases();

    //nbre de groupes par grille
    float reward_nbG = groups.size()/12.0f;
    //nbre de cases moyen  par groupe
    float average_group_size = 0;
    //nbre de cases appartenant à un groupe
    float nb_cases = 0;

    for (int i=0; i<groups.size(); i++)
    {
        average_group_size += groups[i].size();
    }
    nb_cases = average_group_size/25.0f;

    average_group_size /= (groups.size()*25.0f);

    return (coefficients[3]*nb_cases + coefficients[4]*reward_nbG+ coefficients[5]*average_group_size);
}

void Agent1b_Logical::learn_coeff(int mode)
{
    ofstream logFile;

    vector<vector<float> > seeds;
    for (int i=0; i < 100; i ++)
    {
        seeds.push_back({i*5+2, i*3-2, i*20-4});
    }

    coefficients[0]=0;
    coefficients[1]=0;
    coefficients[2]=0;
    coefficients[3]=0;
    coefficients[4]=0;
    coefficients[5]=0;



    logFile.open (("../../Logs/Learning_"+ProfilName).c_str(),ios::app);

    logFile << "Base3" << ",";
    logFile << "sameValue" << ",";
    logFile << "multiple" << ",";
    logFile << "position" << ",";
    logFile << "random" << ",";
    logFile << "moyenne" << endl;

    logFile.close ();

    /*INCREMENTATION*/
    for ( ;(MAX_COEFF-coefficients[0]) >=0 ;coefficients[0]++)
    {
          for ( ;(MAX_COEFF-coefficients[0]-coefficients[1])>=0; coefficients[1]++)
         {
               for ( ;(MAX_COEFF-coefficients[0]-coefficients[1]-coefficients[2] ) >=0; coefficients[2]++)
               {
                       for ( ;(MAX_COEFF-coefficients[0]-coefficients[1]-coefficients[2] -coefficients[3])  >=0; coefficients[3]++)
                       {
                            for ( ;(MAX_COEFF-coefficients[0]-coefficients[1]-coefficients[2] -coefficients[3]-coefficients[4])  >=0; coefficients[4]++)
                        {
                             coefficients[5]=(MAX_COEFF-coefficients[0]-coefficients[1]-coefficients[2] -coefficients[3] -coefficients[4]-coefficients[5]);

                            cout << coefficients[0];
                            cout << coefficients[1];
                            cout << coefficients[2];
                            cout << coefficients[3];
                            cout << coefficients[4];
                            cout << coefficients[5];

                            logFile.open (("../../Logs/Learning_"+ProfilName).c_str(),ios::app);


                            action->reset(seeds[nb_game2Play]);

                            int all_game2Play = 0;
                            while (nb_game2Play > 0)
                            {
                                all_game2Play = nb_game2Play;
                                compute_decision_predict( false);
                                if ((all_game2Play != nb_game2Play) && nb_game2Play)
                                {
                                    action->reset(seeds[nb_game2Play]);
                                }
                            }
                            cout << " , score: " << (float)(score_total/(nb_game2Play_initial *1.0f)) << endl;
                            for (int i = 0; i < coefficients.size(); i++)
                            {
                                logFile << coefficients[i] << ",";
                            }
                            logFile << (float)(score_total/(nb_game2Play_initial *1.0f)) << endl ;
                            logFile.close ();

                            score_total = 0;
                            nb_game2Play = nb_game2Play_initial;
                            coefficients[5]=0;

                        }
                          coefficients[4]=0;
                       }
                       coefficients[3]=0;
               }
               coefficients[2]=0;
          }
          coefficients[1]=0;
    }
    nb_game2Play = 0;


}


/*FONCTIONS GET*/

float Agent1b_Logical::get_destination_reward_same_value()
{
    return destination_reward_same_value[choix];
}

float Agent1b_Logical::get_destination_base3_reward()
{
    return destination_base3_reward[choix];
}

float Agent1b_Logical::get_position_reward()
{
    return position_reward[choix];
}

void Agent1b_Logical::test_copy()
{
    choix = 0;
    float reward_best = 0.0f;
    float score_grid  = 0.0f;
    float score_current_poss  = 0.0f;

    compute_reward();

    for (int j = 0; j < all_possibilities.size(); j ++)
    {
        copy_grid->copy_grille(senseurs);

        vector<Case*> copy_cases_selected;
        for (int k = 0; k < all_possibilities[j].size(); k ++)
        {
            /*
             * Copy case selected from all possibilities, using the copy of the case
             */
          copy_cases_selected.push_back(copy_grid->get_Cases()[all_possibilities[j][k]->get_id()-1]);
        }
        action->compute_predict_score(copy_cases_selected, copy_grid);

        //vector< vector<Case*> > all_possibilities_predit = compute_all_predict_possibilities(copy_grid);

        score_grid = compute_predict_grid_cost(copy_grid);

//        cout << "BASE3 :    "   << destination_base3_reward[j]          << endl;
//        cout << "SAMEVAL :  "   << destination_reward_same_value[j]     << endl;
//        cout << "MULTVAL :  "   << destination_reward_multiple_value[j] << endl;
//        cout << "POS :      "   << position_reward[j]                   << endl;
//        cout << "FUTUR :    "   << score_grid                           << endl;
//        system("pause");

//   score_current_poss = coefficients[0]*destination_base3_reward[j]+coefficients[1]*destination_reward_same_value[j]+coefficients[3]*position_reward[j]+score_grid +coefficients[2]*destination_reward_multiple_value[j];
        score_current_poss = coefficients[0]*destination_base3_reward[j]+coefficients[1]*destination_reward_same_value[j]+coefficients[2]*position_reward[j]+score_grid ; //random_reward[j]+s

        if (reward_best < score_current_poss)
        {
            reward_best = score_current_poss;
            choix = j;
        }
    }

}
