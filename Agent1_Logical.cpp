#include "Agent1_Logical.h"

Agent1_Logical::Agent1_Logical(Grille* senseurs, int nb_game2Play, int decision_delay): Struct_Agent(senseurs, nb_game2Play, decision_delay)
{
    //ctor
    srand (time(NULL));
    string profil = "agent1.csv";
    action = new Action(senseurs, profil);
}

Agent1_Logical::~Agent1_Logical()
{
    //dtor
}

void Agent1_Logical::compute_decision()
{
    if (!senseurs->get_isOver())
    {
        int choix = 0 ;

        compute_possibilitiesInGrps();
        choix = compute_possibilities_cost();
        action->affiche_cases_selected(all_possibilities[choix], decision_delay);
        action->compute_score(all_possibilities[choix]);




//cout << all_possibilities.size() << endl;

//        int choix = rand() % all_possibilities.size();
//        action->affiche_cases_selected(all_possibilities[choix], 3000);
//        cout << nbvoisinssamevalue[choix] <<endl;
//        cout << randomscore[choix] <<endl;
//        action->compute_score(all_possibilities[choix]);

//        int choix = rand() % tab_group.size();
//        action->affiche_cases_selected(tab_group[choix], 1000000);
//        action->compute_score(tab_group[choix]);
    }
    else
    {
        action->reset();
        nb_game2Play --;
    }
}

void Agent1_Logical::compute_possibilitiesInGrps()
{
    all_possibilities.clear();
    vector<Case*>  tab_group = senseurs->get_Cases();

    for (int i = 0; i < tab_group.size(); i ++)
    {
            vector<Case*> racine;
            racine.push_back(tab_group[i]);
            deep_course(racine);
    }

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
                //on retire les voisins déjà présents dans le vector_casesCourante et on ajuste l'indice du vector voisins
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

bool Agent1_Logical::has_games2Play()
{
    bool monbool = true;
    if (nb_game2Play <= 0)
    {
        monbool = false;
    }
    return monbool;
}

vector< float >  Agent1_Logical::compute_destination_reward()
{
    vector< float > destination_reward;

    for (int z = 0; z < all_possibilities.size(); z ++)
    {
        vector<Case*> voisins = senseurs->get_all_voisins( all_possibilities[z].back());
        float reward = 0;
        for (int x = 0; x < voisins.size(); x ++)
        {
            if (voisins[x]->get_value() == (all_possibilities[z].size()*all_possibilities[z].back()->get_value()))
            {
                 reward++;
            }
        }
        //coefficient entre 1 et 3
        destination_reward.push_back(reward/3.0f);
    }
    return destination_reward;
}

vector< float >  Agent1_Logical::compute_random_reward()
{
    vector< float > random_reward;

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

    return random_reward;
}

vector< int >  Agent1_Logical::compute_destination_base6_reward()
{
    vector< int > destination_base6_reward;

    for (int z = 0; z < all_possibilities.size(); z ++)
    {
        float reward = 0;
        int number = (all_possibilities[z].front()->get_value())*(all_possibilities[z].size());
        while (number%2 == 0)
        {
            if (number == 6)
            {
                reward = 1;
            }

            number = number/2;
        }
        destination_base6_reward.push_back(reward);
    }
    return destination_base6_reward;
}

vector< int >  Agent1_Logical::compute_destination_4_reward()
{
    vector< int > destination_4_reward;
    float reward ;
    for (int z = 0; z < all_possibilities.size(); z ++)
    {
        reward = 0;
        int number = (all_possibilities[z].front()->get_value())*(all_possibilities[z].size());
        if (number == 4)
        {
                reward = 1;
        }
        destination_4_reward.push_back(reward);
    }
    return destination_4_reward;
}

vector< int >  Agent1_Logical::compute_position_reward()
{
    vector<Case*> cases = senseurs->get_Cases();

    vector< int > position_reward;
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
    return position_reward;
}


int Agent1_Logical::compute_possibilities_cost()
{
    vector<float> nbvoisinssamevalue = compute_destination_reward();
    vector<float> randomscore = compute_random_reward ();
    vector<int> score_base6 = compute_destination_base6_reward ();
    vector<int> score_4 = compute_destination_4_reward();
    vector<int> position_reward = compute_position_reward();
    int choix = 0;
    float reward_best = 0;
    float reward  = 0 ;

    for (int z = 0; z < all_possibilities.size(); z ++)
    {
        float taille = all_possibilities[z].size();
        float destvalue = taille * all_possibilities[z][0]->get_value();
        reward = 1*nbvoisinssamevalue[z]+1*randomscore[z]+1*position_reward[z] + senseurs->get_mean()/destvalue;
        if (score_base6[z])
        {
           reward += 4*score_base6[z];
        }
        else
            {
                reward += 1*score_4[z];
            }

        if (reward_best < reward)
        {
            reward_best = reward;
            choix = z;
        }
        //cout << "same value: " << nbvoisinssamevalue[z] << ", random: " << randomscore[z] << ", total :" << rewards[z] << endl;
    }
   /* cout << endl ;
    cout << "the best is            : " << reward_best                           << endl;
    cout << "NB VOISINS SAME VALUE  : " << nbvoisinssamevalue[choix]             << endl;
    cout << "NB 1-3 VOISINS         : " << randomscore[choix]                    << endl;
    cout << "BASE 6                 : " << score_base6[choix]                    << endl;
    cout << "POSITION REWARD        : " << position_reward[choix]                << endl;
    cout << "MEAN RATION            : " << senseurs->get_mean()/(all_possibilities[choix].size() *all_possibilities[choix][0]->get_value())        << endl;
    */return choix ;
}


