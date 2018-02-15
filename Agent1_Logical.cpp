#include "Agent1_Logical.h"

Agent1_Logical::Agent1_Logical(Grille* senseurs, int nb_game2Play): Struct_Agent(senseurs, nb_game2Play)
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
        compute_possibilitiesInGrps();
        vector<float> nbvoisinssamevalue = compute_destination_reward();
        vector<float> randomscore = compute_random_reward ();
        vector<int> score_base6 = compute_destination_base6_reward ();

        vector<float> rewards;
        for (int z = 0; z < all_possibilities.size(); z ++)
        {
            float reward = nbvoisinssamevalue[z] + randomscore[z] + score_base6[z];
            rewards.push_back(reward);
            //cout << "same value: " << nbvoisinssamevalue[z] << ", random: " << randomscore[z] << ", total :" << rewards[z] << endl;
        }
        float reward_best = 0;
        int choix = 0;
        for (int z = 0; z < rewards.size(); z ++)
        {
            if (reward_best < rewards[z])
            {
                reward_best = rewards[z];
                choix = z;
            }
        }
        cout << "the best is " << reward_best << endl;
        action->affiche_cases_selected(all_possibilities[choix], 0);
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
    //cout << "all_possibilities = " << all_possibilities.size() << endl;
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

vector< float >   Agent1_Logical::compute_random_reward()
{
    vector< float > random_reward;

    for (int z = 0; z < all_possibilities.size(); z ++)
    {
          float nb_1 = 0;
          float nb_2 = 0 ;
          float nb_3 = 0 ;
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
        for (int z = 0; z < voisins.size(); z ++)
        {
            if (voisins[z]->get_value() == 1)nb_1++;
            else if (voisins[z]->get_value() == 2)nb_2++;
            else if (voisins[z]->get_value() == 3)nb_3++;
        }
//        cout << "voisin size: " << voisins.size() << endl;
//        cout << nb_1 << nb_2 << nb_3 << endl;
//        cout << (nb_1/Nb_voisins) << "  " << (nb_2/Nb_voisins) << "  " << (nb_3/Nb_voisins) << endl;
        float reward = (nb_1/Nb_voisins)+(nb_2/Nb_voisins)+(nb_3/Nb_voisins) ;
//        cout << reward << endl;
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

void Agent1_Logical::compute_possibilities_cost()
{

}
