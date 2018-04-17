#include "Agent1b_Logical.h"

Agent1b_Logical::Agent1b_Logical(Grille* senseurs, int nb_game2Play, int decision_delay,string ProfilName, int mode): Struct_Agent(senseurs, nb_game2Play, decision_delay, ProfilName)
{
    //ctor
    srand (time(NULL));
    action = new Action(senseurs, ProfilName);
    this->mode = mode;
    /*INITIALISATION*/
    for (int i = 0; i < 5; i++)
    {
        coefficients.push_back(0);
    }
    score_total = 0;
    nb_game2Play_initial = nb_game2Play;


    coefficients[0] = 15;
    coefficients[1] = 53;
    coefficients[2] = 7;
    coefficients[3] = 13;
    coefficients[4] = 12;



}

Agent1b_Logical::Agent1b_Logical(): Struct_Agent()
{
    nb_game2Play_initial = 0 ;
}

Agent1b_Logical::~Agent1b_Logical()
{
    //dtor
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


/*FONCTIONS MAJ PARAMETRES */

void  Agent1b_Logical::compute_reward()
{
    destination_reward_same_value.clear();
    destination_reward_multiple_value.clear();
    random_reward.clear();
    destination_base3_reward.clear();
    position_reward.clear();
    vector<float> probabilities ;
    int destvalue  ;

    for (int z = 0; z < all_possibilities.size(); z ++)
    {
        //calcul voisins autres cases que dest
        vector<Case*> voisins_no_dest;
         for (int x = 0; x < (all_possibilities[z].size()-1); x ++)
        {
            vector<Case*> tmp = senseurs->get_all_voisins(all_possibilities[z][x]);
            voisins_no_dest.insert (voisins_no_dest.end(),tmp.begin(),tmp.end());
        }
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
            for (int e = 0; e < voisins_no_dest.size(); e ++)
            {
                if (voisins_no_dest[e]->get_id()  == all_possibilities[z][i]->get_id())
                {
                    voisins_no_dest.erase(voisins_no_dest.begin()+e);
                    e--;
                }
            }
        }


        //calcul destination
        float reward = 0;
        float reward_multiple = 0 ;
        float reward_double_value  = 0 ;
        float  reward_random  = 0 ;
        for (int x = 0; x < voisins_dest.size(); x ++)
        {
            destvalue = (all_possibilities[z].size())*(all_possibilities[z].back())->get_value();

            if (voisins_dest[x]->get_value() == destvalue )
            {
                reward++;
            }
            else
            if( (voisins_dest[x]->get_value() % destvalue) == 0 )
            {
                reward_multiple++;
            }
        }

        //calcul random
        float Nb_voisins = voisins_no_dest.size()*1.0f;
        float  nb = 0 ;
        for (int e = 0; e < voisins_no_dest.size(); e ++)
        {
            if ((voisins_no_dest[e]->get_value() == 1)||(voisins_no_dest[e]->get_value() == 2)||(voisins_no_dest[e]->get_value() == 3))nb ++;
        }
        if (Nb_voisins) reward_random = float(nb/Nb_voisins) ;
        else {reward_random = 0;}



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
        random_reward.push_back(reward_random);
        destination_reward_same_value.push_back(reward/3.0f);
        destination_reward_multiple_value.push_back(reward_multiple/3.0f);
    }

}





/*FONCTIONS  DE PRISE DE DECISION*/
void Agent1b_Logical::compute_decision(int mode,bool affichage)
{
    if (!senseurs->get_isOver())
    {
        compute_all_possibilities();
        compute_possibilities_cost(mode);

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

void Agent1b_Logical::compute_possibilities_cost(int mode)
{
    additionnal_data.clear();

    choix = 0;
    float reward_best = 0;
    float reward  = 0 ;

    //MAJ DES Paramètres pour la grille courrante
    compute_reward(); // + random + base 3 + destination same + multiple dest + position

    for (int z = 0; z < all_possibilities.size(); z ++)
    {
        reward = coefficients[0]*destination_base3_reward[z]+coefficients[1]*destination_reward_same_value[z]+coefficients[2]*destination_reward_multiple_value[z]+coefficients[3]*position_reward[z]+coefficients[4]*random_reward[z] ;

        if (reward_best < reward)
        {
            reward_best = reward;
            choix = z;
        }
    }

    //log les DATAS de l'agent 1
    additionnal_data.push_back(destination_reward_same_value[choix]);
    additionnal_data.push_back(destination_reward_multiple_value[choix]);
    //additionnal_data.push_back(destination_reward_double_value[choix]);
    additionnal_data.push_back(random_reward[choix]);
    additionnal_data.push_back(destination_base3_reward[choix]);
    //additionnal_data.push_back((destination_4_reward[choix]);
    additionnal_data.push_back(position_reward[choix]);
    additionnal_data.push_back(reward_best);

}

void Agent1b_Logical::learn_coeff(int mode)
{
    ofstream logFile;

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
                           coefficients[4]=(MAX_COEFF-coefficients[0]-coefficients[1]-coefficients[2] -coefficients[3] -coefficients[4]);

                            cout << coefficients[0];
                            cout << coefficients[1];
                            cout << coefficients[2];
                            cout << coefficients[3];
                            cout << coefficients[4];


                            logFile.open (("../../Logs/Learning_"+ProfilName).c_str(),ios::app);

                            while (nb_game2Play > 0)
                            {
                                compute_decision(mode, false);
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
float Agent1b_Logical::get_destination_reward_multiple_value()
{
    return destination_reward_multiple_value[choix];
}

float Agent1b_Logical::get_random_reward()
{
    return random_reward[choix];
}
float Agent1b_Logical::get_destination_base3_reward()
{
    return destination_base3_reward[choix];
}

float Agent1b_Logical::get_position_reward()
{
    return position_reward[choix];
}
