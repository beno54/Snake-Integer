#include "Action.h"
#include <string.h >
Action::Action(Grille *grid, string ProfilName)
{
    //ctor
    this->grid = grid;
    nbTurnPlayed =0 ;
    //ouverture des fichiers profils durant l'ex�cution du programme
    logFile.open (("./Logs/"+ProfilName).c_str(),ios::app);
    scoreFile.open ( ("./Logs/Score_"+ProfilName).c_str(),ios::app);

    if (logFile )
    {
        cout << "Ouverture du fichier de logs" <<endl ;
    }
    else
    {
        cout << "Ouverture failed" <<endl ;
    }
}

Action::~Action()
{
    //dtor

    //fermeture des fichiers de profils � la fermeture
    logFile.close();
    cout << "Fermeture du fichier de logs" <<endl ;
}



void Action::compute_score(vector<Case*> cases_selected)
{
    int score = 0;

    for (int e = 0;e<cases_selected.size();e++)
    {
        cases_selected[e]->color_selected(false);
        score += cases_selected[e]->get_value();
        cases_selected[e]->set_value(grid->get_numG()->nexNum());
    }
    cases_selected.back()->set_value(score);
    grid->update_score(score);
}

void Action::log_data(vector<Case*> cases_selected)
{
    vector<Case*>  Cases = grid->get_Cases();
    double GridAverage = 0;
    int maxvalue = 0;
    int caseSelectOrNot[25]={0};

    for (int i=0;i<25;i++)
    {
        int currentvalue = Cases[i]->get_value();

        //test max value
        if (currentvalue>maxvalue) maxvalue = currentvalue;

        //calcul moyenne
        GridAverage += currentvalue ;
        //Logs Grid
        logFile << currentvalue ;
        logFile << ";" ;
    }

    //calcul moyenne
    GridAverage /= 25 ;

    //Log  moyenne de la grille
    logFile << GridAverage ;
    logFile << ";" ;

    //Log score grille courrante
    logFile << grid->get_Case_score()->get_value() ;
    logFile << ";" ;

    //Logs Valeur max
    logFile << maxvalue ;
    logFile << ";" ;

    //Logs Cases s�lectionn�es
    for (int e=0;e<cases_selected.size();e++)
    {
        caseSelectOrNot[cases_selected[e]->get_id()-1]=1;
    }

    for (int e=0;e<25;e++)
    {
        logFile << caseSelectOrNot[e] ;
        logFile << ";" ;
    }

     //Log case en fin de tableau ou non
     for (int e=0;e<25;e++)
     {
        if ((cases_selected[cases_selected.size()-1]->get_id()-1) == e)
        {
            logFile << 1;
        }
        else
        {
            logFile << 0 ;
        }
        logFile << ";" ;
     }

     //Log de valeur s�lectionn�e
     logFile << cases_selected[0]->get_value() ;
     logFile << ";" ;

     //Log de nombre de cases s�lectionn�es
     logFile << cases_selected.size() ;
     logFile << ";" ;

     //Log valeur ajout�e au score = nb case s�lectionn�e * valeur s�lecitonn�e
     logFile << cases_selected.size()* cases_selected[0]->get_value() ;
     logFile << ";" ;

     //Log Nombres de groupements de m�me valeur
     logFile << get_groups_in_grid();
     cout << "nb groupe " <<  get_groups_in_grid()<< endl ;

     //Log fin de log
     logFile << std::endl ;
     //cout << "Ecriture dans fichier de logs" <<endl ;
}

/*
renvoie le nombre de possibilit�s restantes !
*/
int Action::get_groups_in_grid()
{
    int NbPossiblite = 0;
    vector< vector<Case*> > tab_group = get_tab_groups_in_grid();


    for (int i = 0; i < tab_group.size(); i ++)
    {
        if (tab_group[i].size() > 1)
        {
            NbPossiblite ++;
        }
    }

    return NbPossiblite ;


//    int nb_group = 0 ;
//    for (int e = 0 ; e < 25 ; e++)
//    {
//       if (TabIdGroup[e]> nb_group) nb_group = TabIdGroup[e];
//    }
//    for (int e = 0 ; e < nb_group ; e++)
//    {
//        int cpt = 0 ;
//        for (int i = 0 ; i < 25 ; i++)
//        {
//           if (TabIdGroup[i] == e) cpt ++ ;
//        }
//        if (cpt > 1) NbPossiblite++;
//    }
}
/*
Compte le nombre de groupes (associes � chaques cases son groupe)
Compare avec le voisin du bas et celui de droite
*/

//int Action::get_groups_in_grid()
//{
//    vector<Case*>  Cases = grid->get_Cases();
//    int Nbgroup = 0 ;
//
//    vector<int> tab_group;
//    for (int i = 0; i < 40; i++)
//    {
//        tab_group.push_back(0);
//    }
//
//    for (int e = 0 ; e < 25 ; e++){Cases[e]->set_idGroup(0);}
//    for (int e = 0 ; e < 25 ; e++)
//    {
//        if ((e+1)%5 !=0)
//        {
//            if (Cases[e]->get_value() == Cases[e+1]->get_value())
//            {
//                if (Cases[e+1]->get_idGroup())
//                {
//                    Cases[e]->set_idGroup(Cases[e+1]->get_idGroup());
////                    int compteur = 1;
////                    if (Cases[e+1+compteur]->get_value() == Cases[e]->get_value() && (e+1+compteur)%5!=0)
////                    {
////                        compteur ++;
////                    }
////                    while(compteur > 0)
////                    {
////                        Cases[e+compteur-1]->set_idGroup(Cases[e+compteur]->get_idGroup());
////                        compteur --;
////                    }
//                }
//                else
//                    if (!Cases[e]->get_idGroup())
//                    {
//                        Nbgroup++;
//                        Cases[e]->set_idGroup(Nbgroup);
//                        Cases[e+1]->set_idGroup(Nbgroup);
//                    }
//                    else
//                    {
//                        Cases[e+1]->set_idGroup(Cases[e]->get_idGroup());
//                    }
//            }
//            else
//            {
//                //cas premiere case
//                if (!Cases[e]->get_idGroup())
//                {
//                    Nbgroup ++;
//                    Cases[e]->set_idGroup(Nbgroup);
//                }
//                //on check si voisin pas deja dans un groupe
//                if (!Cases[e+1]->get_idGroup())
//                {
//                    Nbgroup ++;
//                    Cases[e+1]->set_idGroup(Nbgroup);
//                    //cout << "voisin droite" << (e+1) << "prends groupe" << Cases[e+1]->get_idGroup() << endl;
//                }
//            }
//        }
//        if (((e+1)<20))
//        {
//            if (Cases[e]->get_value() == Cases[e+5]->get_value())
//            {
//                if (Cases[e+5]->get_idGroup())
//                {
//                    Cases[e]->set_idGroup(Cases[e+5]->get_idGroup());
//                }
//                else
//                    if (!Cases[e]->get_idGroup())
//                    {
//                        Cases[e]->set_idGroup(++Nbgroup);
//                        Cases[e+5]->set_idGroup(Nbgroup);
//                    }
//                    else
//                    {
//                        Cases[e+5]->set_idGroup(Cases[e]->get_idGroup());
//                        //cout << "voisin bas" << (e+1) << "prends groupe" << Cases[e]->get_idGroup() << endl;
//                    }
//            }
//            else
//            {
//                Cases[e+5]->set_idGroup(++Nbgroup);
//            }
//        }
//    }
//
//    for (int i = 0; i < 25; i++)
//    {
//        tab_group[(Cases[i]->get_idGroup())-1]++;
//    }
//
//    int compteur_grp_deleted = 0;
//    for (int i = 0; i < 40; i++)
//    {
//        if (tab_group[i] <= 1)
//        {
//            tab_group.erase (tab_group.begin()+i-compteur_grp_deleted);
//            compteur_grp_deleted ++;
//        }
//    }
//
//    return tab_group.size();
//}

vector< vector<Case*> > Action::get_tab_groups_in_grid()
{
    vector< vector<Case*> > tab_group;
    vector<Case*>  allCases = grid->get_Cases();
    Case* case_courante;
    int idGroup = 0;
    int compteur_cases_left = 25;

    grid->reset_AllidGroup();

    while (compteur_cases_left != 0)
    {
        //cout << "\nDebut groupe" << idGroup  << endl;
        idGroup ++;
        vector<Case*>  Sous_Cases_to_Test;
        vector<Case*>  new_groupe_cases;

        int i = 0;
        bool finded = false;
        //on ajoute dans tab group
        while(i < 25 && !finded)
        {
            if (allCases[i]->get_idGroup() == 0)
            {
                //ajout la 1ere case
                Sous_Cases_to_Test.push_back(allCases[i]);
                //cout << "ajout case" << allCases[i]->get_id()  << endl;
                allCases[i]->set_idGroup(idGroup);


                new_groupe_cases.push_back(allCases[i]);
                compteur_cases_left--;

                finded = true;
            }
            i++;
        }


        while (Sous_Cases_to_Test.size() != 0)
        {
            Case* case_courante = Sous_Cases_to_Test.front();
            int id_courant = case_courante->get_id();

            //voisin de droite ?
            if (id_courant%5 != 0 && allCases[id_courant]->get_idGroup() == 0 && case_courante->get_value() == allCases[id_courant]->get_value())
            {
                //ok donc rajout
                allCases[id_courant]->set_idGroup(idGroup);
                Sous_Cases_to_Test.push_back(allCases[id_courant]);
                compteur_cases_left--;
            }

            //voisin de gauche ?
            if (id_courant%5 != 1 && allCases[id_courant-2]->get_idGroup() == 0 && case_courante->get_value() == allCases[id_courant-2]->get_value())
            {
                //ok donc rajout
                allCases[id_courant-2]->set_idGroup(idGroup);
                Sous_Cases_to_Test.push_back(allCases[id_courant-2]);
                //cases_to_Test.erase (cases_to_Test.begin()+((id_courant-2)-compteur_grp_deleted));
                compteur_cases_left--;
            }

            //voisin du haut ?
            if (id_courant > 5 && allCases[id_courant-6]->get_idGroup() == 0 && case_courante->get_value() == allCases[id_courant-6]->get_value())
            {
                //ok donc rajout
                allCases[id_courant-6]->set_idGroup(idGroup);
                Sous_Cases_to_Test.push_back(allCases[id_courant-6]);
                //cases_to_Test.erase (cases_to_Test.begin()+((id_courant-6)-compteur_grp_deleted));
                compteur_cases_left--;
            }

            //voisin du bas ?
            if (id_courant <= 20 && allCases[id_courant+4]->get_idGroup() == 0 && case_courante->get_value() == allCases[id_courant+4]->get_value())
            {
                //ok donc rajout
                allCases[id_courant+4]->set_idGroup(idGroup);
                Sous_Cases_to_Test.push_back(allCases[id_courant+4]);
                //cases_to_Test.erase (cases_to_Test.begin()+((id_courant+4)-compteur_grp_deleted));
                compteur_cases_left--;
            }

            Sous_Cases_to_Test.erase(Sous_Cases_to_Test.begin());

            if (Sous_Cases_to_Test.size() != 0)
            {
                //on rajoute le suivant
                new_groupe_cases.push_back(Sous_Cases_to_Test.front());
                //cout << "ajout case" << Sous_Cases_to_Test.front()->get_id()  << endl;
            }
        }

        tab_group.push_back(new_groupe_cases);
    }

    return  tab_group ;
}


int Action::get_nbTurnPlayed()
{
    return nbTurnPlayed ;
}
void Action::reinitialize_nbTurnPlayed()
{
    nbTurnPlayed = 0 ;
}
void Action::set_nbTurnPlayed()
{
    nbTurnPlayed ++;
}
void Action::log_score()
{
    if (scoreFile )
    {
        //Uodate du moment courant + Log score avant GAME OVER + nb tour jou�
        char timeToLog [256];
        timenow = time(0);
        strcpy(timeToLog,ctime(&timenow));
        timeToLog[strlen(timeToLog)-1]='\0';
        scoreFile << timeToLog<< ";" << grid->get_Case_score()->get_value() << ";" << nbTurnPlayed << ";" <<std::endl ;
        scoreFile.close();
    }
    else
    {
        cout << "Ouverture failed" <<endl ;
    }

}
