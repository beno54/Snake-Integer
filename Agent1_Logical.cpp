#include "Agent1_Logical.h"

Agent1_Logical::Agent1_Logical(Grille* senseurs, int nb_game2Play, int decision_delay,string ProfilName): Struct_Agent(senseurs, nb_game2Play, decision_delay, ProfilName)
{
    //ctor
    srand (time(NULL));
    action = new Action(senseurs, ProfilName);
}

Agent1_Logical::~Agent1_Logical()
{
    //dtor
    delete (action);
}


/*FONCTIONS DETECTION GAMEOVER*/
bool Agent1_Logical::has_games2Play()
{
    bool monbool = true;
    if (nb_game2Play <= 0)
    {
        monbool = false;
    }
    return monbool;
}


/*FONCTIONS PARCOURS DES POSS*/
void Agent1_Logical::compute_all_possibilities()
{
    all_possibilities.clear();
    vector<Case*>  tab_group = senseurs->get_Cases();

    for (int i = 0; i < tab_group.size(); i ++)
    {

            vector<Case*> racine;
            racine.push_back(tab_group[i]);
            deep_course(racine);
    }
//    cout<< all_possibilities.size()<<endl;
//    cout << "Possibilites : " << action->get_nbTurnPlayed() << endl ;
//    for (int z = 0; z < all_possibilities.size(); z ++)
//    {
//        for (int e = 0; e < all_possibilities[z].size(); e ++)
//        {
//            cout << all_possibilities[z][e]->get_id() << " ";
//        }
//        cout << endl;
//    }

}

void Agent1_Logical::deep_course(vector<Case*> v_casesCourante)
{
    vector<Case*> vector_chemin = v_casesCourante;

    vector<Case*> voisins = senseurs->get_voisins(vector_chemin.back());
    for (int k = 0; k < voisins.size(); k ++)
    {
        for (int z = 0; z < vector_chemin.size(); z ++)
        {
            if (voisins[k]->get_id() == vector_chemin[z]->get_id())
            {
                //on retire les voisins d�j� pr�sents dans le vector_casesCourante et on ajuste l'indice du vector voisins
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

void  Agent1_Logical::compute_destination_reward()
{
    destination_reward_same_value.clear();
    destination_reward_multiple_value.clear();
    destination_reward_double_value.clear();

    int destvalue  ;
    for (int z = 0; z < all_possibilities.size(); z ++)
    {
        vector<Case*> voisins = senseurs->get_all_voisins( all_possibilities[z].back());

        //retire voisins pr�sent dans le chemin

        for (int i =0; i < all_possibilities[z].size(); i++)
        {
            for (int e =0; e < voisins.size(); e++)
            {
                if(voisins[e]->get_id() == all_possibilities[z][i]->get_id())
                {
                    voisins.erase(voisins.begin()+ e);
                    e--;
                }
            }
        }

        float reward = 0;
        float reward_multiple = 0 ;
        float reward_double_value  = 0 ;

        for (int x = 0; x < voisins.size(); x ++)
        {
            destvalue = (all_possibilities[z].size())*(all_possibilities[z].back())->get_value();

            if (voisins[x]->get_value() == destvalue )
            {
                reward++;
            }
            else
            if( (voisins[x]->get_value() % destvalue) == 0 )
            {
                reward_multiple++;
            }
            if (voisins[x]->get_value() == destvalue*2 )
            {
                reward_double_value++;
            }
        }
        //coefficient entre 1 et 3

        destination_reward_same_value.push_back(reward/3.0f);
        destination_reward_multiple_value.push_back(reward_multiple/3.0f);
        destination_reward_double_value.push_back(reward_double_value/3.0f);
    }

}

void Agent1_Logical::compute_random_reward()
{
    random_reward.clear();

    for (int z = 0; z < all_possibilities.size(); z ++)
    {

        vector<Case*> voisins;
         for (int x = 0; x < (all_possibilities[z].size()-1); x ++)
        {
            vector<Case*> voisin_case = senseurs->get_all_voisins(all_possibilities[z][x]);
            for (int y = 0; y < voisin_case.size(); y ++)
            {
                bool suppression = false ;
                for (int w = 0; w < (all_possibilities[z].size()); w ++)
                {

                    if (voisin_case[y]->get_id()  == all_possibilities[z][w]->get_id())
                    {
                       suppression = true;
                    }

                }
                if (suppression == true )
                {
                    voisin_case.erase(voisin_case.begin()+y);
                    y--;
                }
            }
            voisins.insert(voisins.end(),voisin_case.begin(),voisin_case.end());
        }
        float Nb_voisins = voisins.size()*1.0f;
        float  nb = 0 ;
        for (int e = 0; e < voisins.size(); e ++)
        {
            if ((voisins[e]->get_value() == 1)||(voisins[e]->get_value() == 2)||(voisins[e]->get_value() == 3))nb ++;
        }

        float reward = float(nb/Nb_voisins) ;
        random_reward.push_back(reward);
    }
}

void Agent1_Logical::compute_destination_base3_reward()
{
    destination_base3_reward.clear();

    for (int z = 0; z < all_possibilities.size(); z ++)
    {
        float reward = 0;
        int number = (all_possibilities[z].back()->get_value())*(all_possibilities[z].size());
        while (number%2 == 0)
        {
            if (number/2 == 3)
            {
                reward = 1;
            }
            number = number/2;
        }
        destination_base3_reward.push_back(reward);
    }
}

void  Agent1_Logical::compute_destination_4_reward()
{
    destination_4_reward.clear();
    float reward ;
    for (int z = 0; z < all_possibilities.size(); z ++)
    {
        reward = 0;
        int number = (all_possibilities[z].back()->get_value())*(all_possibilities[z].size());
        if (number == 4)
        {
                reward = 1;
        }
        destination_4_reward.push_back(reward);
    }
}

void  Agent1_Logical::compute_position_reward()
{
    vector<Case*> cases = senseurs->get_Cases();

    position_reward.clear();
    float reward ;
    for (int z = 0; z < all_possibilities.size(); z ++)
    {
        int nb_voisins_tot = (senseurs->get_all_voisins(all_possibilities[z].back())).size();
        reward = 0 ;
       // cout << nb_voisins_tot << endl ;
        switch (nb_voisins_tot)
        {
            case 2 : reward = 1 ; break ;
            case 3 : reward = 0.5 ; break ;
        }
       // cout << "reward : " << reward << endl ;
        position_reward.push_back(reward);
    }
}


/*FONCTIONS  DE PRISE DE DECISION*/
void Agent1_Logical::compute_decision(int mode,bool affichage)
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
        action->log_score();
        action->reset();
        nb_game2Play --;
    }
}

void Agent1_Logical::compute_possibilities_cost(int mode)
{
    additionnal_data.clear();

    choix = 0;
    float reward_best = 0;
    float reward  = 0 ;


    //MAJ DES Paramètres pour la grille courrante
    compute_destination_reward();
    compute_random_reward();
    compute_destination_base3_reward();
    compute_destination_4_reward();
    compute_position_reward();

    switch(mode)
    {
        /*MODE RANDOM*/
        case 1:
            choix = rand() % all_possibilities.size();
            break;

        /*MODE FCT NOT OPTI*/
        case 2:
            for (int z = 0; z < all_possibilities.size(); z ++)
            {
                reward = destination_base3_reward[z]+destination_4_reward[z]+destination_reward_same_value[z]+destination_reward_multiple_value[z]+position_reward[z]+random_reward[z] ;

                if (reward_best < reward)
                {
                    reward_best = reward;
                    choix = z;
                }
            }
            break;

        /*MODE FCT COEFF FROM ESSAI ERROR*/
        case 3:
            for (int z = 0; z < all_possibilities.size(); z ++)
            {
                reward = 0.2*destination_base3_reward[z]+0.35*destination_reward_same_value[z]+0.25*destination_reward_multiple_value[z]+0.15*position_reward[z]+0.1*random_reward[z] ;

                if (reward_best < reward)
                {
                    reward_best = reward;
                    choix = z;
                }
            }
            break;

        /*MODE FCTION AVEC COEFF OPTI*/
        case 4:
            for (int z = 0; z < all_possibilities.size(); z ++)
            {
                reward = 0.2*destination_base3_reward[z]+0.35*destination_reward_same_value[z]+0.25*destination_reward_multiple_value[z]+0.15*position_reward[z]+0.1*random_reward[z] ;

                if (reward_best < reward)
                {
                    reward_best = reward;
                    choix = z;
                }
            }
            break;
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



//    cout << endl ;
//    cout << "the best is                     : " << reward_best                                  << endl;
//    cout << "NB VOISINS MULTIPLE  VALUE      : " << destination_reward_multiple_value[choix]     << endl;
//    cout << "NB VOISINS  SAME VALUE          : " << destination_reward_same_value[choix]         << endl;
//    cout << "NB 1-3 VOISINS                  : " << random_reward[choix]                         << endl;
//    cout << "BASE 3                          : " << destination_base3_reward[choix]              << endl;
//    cout << "POSITION REWARD                 : " << position_reward[choix]                       << endl;
//    cout << "MEAN RATION                     : " << senseurs->get_mean()/(all_possibilities[choix].size() *all_possibilities[choix][0]->get_value())        << endl;
   // cout << destination_reward_double_value[choix] << endl ;
    //cout << score_base6[choix] << endl ;

}


/*FONCTIONS GET*/

float Agent1_Logical::get_destination_reward_same_value()
{
    return destination_reward_same_value[choix];
}
float Agent1_Logical::get_destination_reward_multiple_value()
{
    return destination_reward_multiple_value[choix];
}
float Agent1_Logical::get_destination_reward_double_value()
{
    return destination_reward_double_value[choix];
}
float Agent1_Logical::get_random_reward()
{
    return random_reward[choix];
}
float Agent1_Logical::get_destination_base3_reward()
{
    return destination_base3_reward[choix];
}
float Agent1_Logical::get_destination_4_reward()
{
    return destination_4_reward[choix];
}
float Agent1_Logical::get_position_reward()
{
    return position_reward[choix];
}


