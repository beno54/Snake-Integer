#include "agent2.h"

agent2::agent2(Grille* senseurs, int nb_game2Play, int decision_delay,string ProfilName, int mode): Struct_Agent(senseurs, nb_game2Play, decision_delay, ProfilName)
{
    //ctor
    srand (time(NULL));
    action = new Action(senseurs, ProfilName);
    this->mode = mode;

    score_total = 0;
    nb_game2Play_initial = nb_game2Play;

    /*Import du script*/
	CPyObject pName = PyUnicode_FromString("tettt");
	CPyObject pModule = PyImport_Import(pName);
}

agent2::agent2(): Struct_Agent()
{
    nb_game2Play_initial = 0 ;
}

agent2::~agent2()
{
    //dtor
    delete (action);
}


/*FONCTIONS DETECTION GAMEOVER*/
bool agent2::has_games2Play()
{
    bool monbool = true;
    if (nb_game2Play <= 0)
    {
        monbool = false;

    }
    return monbool;
}

void agent2::deep_course(vector<Case*> v_casesCourante)
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

void agent2::convert_grid2String()
{
    param = "";
    vector<Case*> cases = senseurs->get_Cases();
    for (int i=0;i<cases.size();i++)
    {
        param = param + to_string(cases[i]->get_value()) + ",";
    }
    param.pop_back();
}

void agent2::call_tensorflow()
{
    /*PYTHON START*/
    /*Import du script*/
	CPyObject pName = PyUnicode_FromString("tettt");
	CPyObject pModule = PyImport_Import(pName);

	if(pModule)
	{
	    /*Appel de la fonction de prediction*/
		CPyObject pFunc2 = PyObject_GetAttrString(pModule, "predict2model");

		/*init des param de la fonction (grille d'entree)*/
		const char *c = param.c_str();
		PyObject* pArgs = PyTuple_Pack(1, PyUnicode_FromString(c));

		if(pFunc2 && PyCallable_Check(pFunc2))
		{
		    /*Recuperation de la prediction (list de float)*/
		    CPyObject pValue = PyObject_CallObject(pFunc2, pArgs);

			if (pValue != NULL)
            {
                //compte le nombre d'élément de la liste
                int counter = (int) PyList_Size(pValue);
                //variables utilisées pour transposer la liste en float
                PyObject *ptemp, *objectsRepresentation ;
                char* a11;
                //reprendra la liste en float
                float temp[counter];


                ptemp = PyList_GetItem(pValue,0);
                objectsRepresentation = PyObject_Repr(ptemp);
                a11 = PyUnicode_AsUTF8(objectsRepresentation);
                temp[0]=(float)strtof(a11,NULL);

                /*On trie les predictions par ordre decroissant de probabilite*/
                /*une liste de valeur de proba et une liste d'indice de case*/
                sorted_cases_dest.clear();
                sorted_ind_dest.clear();

                sorted_cases_dest.push_back(temp[0]);
                sorted_ind_dest.push_back(0);

                /*Debut du tri*/
                for (int i = 0 ; i < 25 ; i++ )
                {
                    /*recuperation de la proba, from string to float*/
                    ptemp = PyList_GetItem(pValue,i);
                    objectsRepresentation = PyObject_Repr(ptemp);
                    a11 = PyUnicode_AsUTF8(objectsRepresentation);
                    temp[i]=(float)strtod(a11,NULL);

                    /*Insertion de la new proba dans la liste*/
                    for (int j = 0 ; j < sorted_cases_dest.size() ; j++ )
                    {
                        /*Si new proba + petit, on l'insere par la gauche*/
                        if (sorted_cases_dest[j] < temp[i])
                        {
                            sorted_cases_dest.insert(sorted_cases_dest.begin()+j, temp[i]);
                            sorted_ind_dest.insert(sorted_ind_dest.begin()+(j), i);
                            break;
                        }
                        /*Si new proba + grand, on l'insere par la droite*/
                        if (j == sorted_cases_dest.size()-1)
                        {
                            sorted_cases_dest.push_back(temp[i]);
                            sorted_ind_dest.push_back(i);
                            j = 45;
                        }
                    }
                }

                /*On trie les predictions par ordre decroissant de probabilite*/
                /*une liste de valeur de proba et une liste d'indice de case*/
                sorted_cases_nb.clear();
                sorted_ind_nb.clear();

                sorted_cases_nb.push_back(temp[0]);
                sorted_ind_nb.push_back(0);
                /*Debut du tri*/
                for (int i = 25 ; i < 50 ; i++ )
                {
                    /*recuperation de la proba, from string to float*/
                    ptemp = PyList_GetItem(pValue,i);
                    objectsRepresentation = PyObject_Repr(ptemp);
                    a11 = PyUnicode_AsUTF8(objectsRepresentation);
                    temp[i]=(float)strtod(a11,NULL);

                    /*Insertion de la new proba dans la liste*/
                    for (int j = 0 ; j < sorted_cases_nb.size() ; j++ )
                    {
                        /*Si new proba + petit, on l'insere par la gauche*/
                        if (sorted_cases_nb[j] < temp[i])
                        {
                            sorted_cases_nb.insert(sorted_cases_nb.begin()+j, temp[i]);
                            sorted_ind_nb.insert(sorted_ind_nb.begin()+(j), i-25);
                            j = 45;
                        }
                        /*Si new proba + grand, on l'insere par la droite*/
                        if (j == (sorted_cases_nb.size()-1))
                        {
                            sorted_cases_nb.push_back(temp[i]);
                            sorted_ind_nb.push_back(i-25);
                            j = 45;
                        }
                    }
                }
			}
		}
		else
		{
			printf("ERROR: function getInteger()\n");
		}

	}
	else
	{
		printf("ERROR: Module not imported\n");
	}

	/*PYTHON END*/
}

/*FONCTIONS PARCOURS DES POSS*/
void agent2::compute_all_possibilities()
{
    all_possibilities.clear();
    vector<Case*>  tab_group = senseurs->get_Cases();

    int indice = 0;
    vector<Case*> racine;
    racine.push_back(tab_group[sorted_ind_dest.front()]);
    deep_course(racine);

    while (all_possibilities.size() < 1)
    {
        racine.clear();
        racine.push_back(tab_group[sorted_ind_dest[indice]]);
        deep_course(racine);
        indice ++;
    }
}


//FONCTIONS  DE PRISE DE DECISION/
void agent2::compute_decision(int mode,bool affichage)
{
    if (!senseurs->get_isOver())
    {
        convert_grid2String();
        call_tensorflow();

        compute_all_possibilities();

        bool choice = false;
        /*Insertion de la new proba dans la liste*/
        for (int j = 0 ; j < sorted_ind_nb.size() && !choice; j++ )
        {
            /*Insertion de la new proba dans la liste*/
            for (int i = 0 ; i < all_possibilities.size() && !choice; i++ )
            {
                if ((all_possibilities[i].size()) == sorted_ind_nb[j])
                {
                    choice = true;
                    choix = i;
                }
            }
        }

        //choix = rand() % all_possibilities.size();

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
        //action->log_score(coefficients);
        action->reset();
        nb_game2Play --;
    }
}


/*FONCTIONS GET*/
