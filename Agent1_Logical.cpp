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
        vector< vector<Case*> > tab_group = action->get_tab_groups_in_grid();
        compute_possibilitiesInGrps();
        //cout << all_possibilities.size() << endl;

        int choix = rand() % all_possibilities.size();
        action->affiche_cases_selected(all_possibilities[choix], 0);
        action->compute_score(all_possibilities[choix]);

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
    vector< vector<Case*> > tab_group = action->get_tab_groups_in_grid();

    for (int i = 0; i < tab_group.size(); i ++)
    {
        for (int j = 0; j < tab_group[i].size(); j ++)
        {
            vector<Case*> racine;
            racine.push_back(tab_group[i][j]);
            deep_course(racine);
        }
    }
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
//        cout << "new chemin: " << endl;
//        for (int z = 0; z < vector_chemin.size(); z ++)
//        {
//            cout << vector_chemin[z]->get_id() << "-";
//        }
//        cout << endl;

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
