#include "Action.h"
#include <string.h >
Action::Action(Grille *grid, string ProfilName)
{
    //ctor
    this->grid = grid;
    nbTurnPlayed =0 ;
    //ouverture des fichiers profils durant l'ex�cution du programme



    try {
        cout << "../../Logs/"+ProfilName << endl ;
        logFile.open (("../../Logs/"+ProfilName).c_str(),ios::app);
        scoreFile.open ( ("../../Logs/Score_"+ProfilName).c_str(),ios::app);
        tmpFile.open ("../../Logs/TmpfileLog",ios::app);
    }
    catch (std::ios_base::failure& e)
    {
      std::cerr << e.what() << '\n';
    }

    if (logFile && tmpFile)
    {
        cout << "Ouverture du fichier de logs" <<endl ;
    }
    else
    {
        cout << "Ouverture failed" <<endl ;
        cerr << "\nError: " << strerror(errno);
    }

    calcul_groups_in_grid();
    compute_NbPossibilities_in_grid();
}

Action::~Action()
{
    //dtor

    //fermeture des fichiers de profils � la fermeture
    scoreFile.close();
    logFile.close();
    tmpFile.close();
     if( remove("../../Logs/TmpfileLog")!= 0 )
    perror( "Tmp Error deleting file" );
  else
    puts( "Tmp File successfully deleted" );

}

void Action::compute_score(vector<Case*> cases_selected)
{
    int score = 0;

    //log_data(cases_selected,NULL);

    for (int e = 0;e<cases_selected.size();e++)
    {
        cases_selected[e]->color_selected(false);
        score += cases_selected[e]->get_value();
        cases_selected[e]->set_value(grid->get_numG()->nexNum());
    }
    cases_selected.back()->set_value(score);
    grid->update_score(score);
    calcul_groups_in_grid();
    compute_NbPossibilities_in_grid();

    nbTurnPlayed ++;
    if (nbPossibilities==0)
    {
        //cout << " GAME OVER : " <<  grid->get_Case_score()->get_value() << endl ;
        grid->set_isOver(true);
        //log_score();
        //reinitialize_nbTurnPlayed();
    }
}

void Action::compute_predict_score(vector<Case*> cases_selected, Grille* grid_predict)
{
    int score = 0;

    //log_data(cases_selected,NULL);

    for (int e = 0;e<cases_selected.size();e++)
    {
        score += cases_selected[e]->get_value();
        cases_selected[e]->set_value(grid_predict->get_numG()->nexNum());
    }
    cases_selected.back()->set_value(score);
//    grid->update_score(score);
//    calcul_groups_in_grid();
//    compute_NbPossibilities_in_grid();

//    nbTurnPlayed ++;
//    if (nbPossibilities==0)
//    {
//        cout << " GAME OVER : " <<  grid->get_Case_score()->get_value() << endl ;
//        grid->set_isOver(true);
//        //log_score();
//        //reinitialize_nbTurnPlayed();
//    }
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
        tmpFile << currentvalue ;
        tmpFile << "," ;
    }

    //calcul moyenne
    GridAverage /= 25 ;

    //Log  moyenne de la grille
    tmpFile << GridAverage ;
    tmpFile << "," ;

    //Log score grille courrante
    tmpFile << grid->get_Case_score()->get_value() ;
    tmpFile << "," ;

    //Logs Valeur max
    tmpFile << maxvalue ;
    tmpFile << "," ;

    //Logs Cases s�lectionn�es
    for (int e=0;e<cases_selected.size();e++)
    {
        caseSelectOrNot[cases_selected[e]->get_id()-1]=1;
    }

    for (int e=0;e<25;e++)
    {
        tmpFile << caseSelectOrNot[e] ;
        tmpFile << "," ;
    }

     //Log case en fin de tableau ou non
     for (int e=0;e<25;e++)
     {
        if ((cases_selected[cases_selected.size()-1]->get_id()-1) == e)
        {
            tmpFile << 1;
        }
        else
        {
            tmpFile << 0 ;
        }
        tmpFile << "," ;
     }

     //Log de valeur s�lectionn�e
     tmpFile << cases_selected[0]->get_value() ;
     tmpFile << "," ;

     //Log de nombre de cases s�lectionn�es
     tmpFile << cases_selected.size() ;
     tmpFile << "," ;

     //Log valeur ajout�e au score = nb case s�lectionn�e * valeur s�lecitonn�e
     tmpFile << cases_selected.size()* cases_selected[0]->get_value() ;
     tmpFile << "," ;

     //Log Nombres de groupements de m�me valeur
     tmpFile << nbPossibilities;
     //cout << nbPossibilities << endl ;
     //Log fin de log

    tmpFile << std::endl ;
     //cout << "Ecriture dans fichier de logs" <<endl ;
}

void Action::log_data(vector<Case*> cases_selected, vector<float> additionnal_data)
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
        tmpFile << currentvalue ;
        tmpFile << "," ;
    }

    //calcul moyenne
    GridAverage /= 25 ;

    //Log  moyenne de la grille
    tmpFile << GridAverage ;

    tmpFile << "," ;

    //Log score grille courrante
    tmpFile << grid->get_Case_score()->get_value() ;
    tmpFile << "," ;

    //Logs Valeur max
    tmpFile << maxvalue ;
    tmpFile << "," ;

    //Logs Cases s�lectionn�es
    for (int e=0;e<cases_selected.size();e++)
    {
        caseSelectOrNot[cases_selected[e]->get_id()-1]=1;
    }

    for (int e=0;e<25;e++)
    {
        tmpFile << caseSelectOrNot[e] ;
        tmpFile << "," ;
    }

     //Log case en fin de tableau ou non
     for (int e=0;e<25;e++)
     {
        if ((cases_selected[cases_selected.size()-1]->get_id()-1) == e)
        {
            tmpFile << 1;
        }
        else
        {
            tmpFile << 0 ;
        }
        tmpFile << "," ;
     }

     //Log de valeur s�lectionn�e
     tmpFile << cases_selected[0]->get_value() ;
     tmpFile << "," ;

     //Log de nombre de cases s�lectionn�es
     tmpFile << cases_selected.size() ;
     tmpFile << "," ;

     //Log valeur ajout�e au score = nb case s�lectionn�e * valeur s�lecitonn�e
     tmpFile << cases_selected.size()* cases_selected[0]->get_value() ;
     tmpFile << "," ;

     //Log Nombres de groupements de m�me valeur
     tmpFile << nbPossibilities;
     //cout << nbPossibilities << endl ;
     //Log fin de log

     //add additionnal data from agents

     for (int i=0; i < (additionnal_data.size()); i ++)
     {
         //Log de valeur s�lectionn�e
         tmpFile << "," ;
         tmpFile << additionnal_data[i] ;
     }
     //cout << "Ecriture dans fichier de logs" <<endl ;
     tmpFile << std::endl ;
}

void Action::compute_NbPossibilities_in_grid()
{
    nbPossibilities = 0;

    for (int i = 0; i < groups_in_grid.size(); i ++)
    {
        if (groups_in_grid[i].size() == 1)
        {
            groups_in_grid.erase(groups_in_grid.begin()+i);
            i --;
        }
    }
    nbPossibilities = groups_in_grid.size();
}
/*
renvoie le nombre de possibilit�s restantes !
*/
int Action::get_NbpPossibilities_in_grid()
{
    return nbPossibilities ;
}

vector< vector<Case*> > Action::get_tab_groups_in_grid ()
{
    return groups_in_grid ;
}

void Action::calcul_groups_in_grid()
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

    groups_in_grid = tab_group ;
}

vector< vector<Case*> > Action::calcul_groups_in_grid(Grille* grid_predict)
{
    vector< vector<Case*> > tab_group;
    vector<Case*>  allCases = grid_predict->get_Cases();
    Case* case_courante;
    int idGroup = 0;
    int compteur_cases_left = 25;

    grid_predict->reset_AllidGroup();

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

    return tab_group ;
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

void Action::log_score( vector<int> additionnal_data)
{
        //on copie le contenu du fichiuer temporaire dans le fichier de log
    //ferme le fichier en écriture
    tmpFile.close();
    //ouvre le fichier en lecture
    ifstream tmpFile_reader("../../Logs/TmpfileLog",std::ifstream::in);
    string line;
    while (getline(tmpFile_reader,line))
    {
        logFile << line;
        logFile << std::endl ;
    }

    //réouvre le fichier en écriture
    tmpFile_reader.close();

    if (scoreFile )
    {
        //Uodate du moment courant + Log score avant GAME OVER + nb tour jou�
        char timeToLog [256];
        timenow = time(0);
        strcpy(timeToLog,ctime(&timenow));
        timeToLog[strlen(timeToLog)-1]='\0';
        scoreFile << timeToLog<< "," << grid->get_Case_score()->get_value() << "," << nbTurnPlayed ;
        for (int i=0; i < (additionnal_data.size()); i ++)
         {
             //Log de valeur s�lectionn�e
             scoreFile << "," ;
             scoreFile << additionnal_data[i] ;
         }
         scoreFile << std::endl ;
    }
    else
    {
        cout << "Ouverture failed" <<endl ;
    }
}
void Action::log_score()
{
    //on copie le contenu du fichiuer temporaire dans le fichier de log
    //ferme le fichier en écriture
    tmpFile.close();
    //ouvre le fichier en lecture
    ifstream tmpFile_reader("../../Logs/TmpfileLog",std::ifstream::in);
    string line;
    while (getline(tmpFile_reader,line))
    {
        logFile << line;
        logFile << std::endl ;
    }

    //réouvre le fichier en écriture
    tmpFile_reader.close();

    if (scoreFile )
    {
        //Uodate du moment courant + Log score avant GAME OVER + nb tour jou�
        char timeToLog [256];
        timenow = time(0);
        strcpy(timeToLog,ctime(&timenow));
        timeToLog[strlen(timeToLog)-1]='\0';
        scoreFile << timeToLog<< "," << grid->get_Case_score()->get_value() << "," << nbTurnPlayed  <<std::endl ;
    }
    else
    {
        cout << "Ouverture failed" <<endl ;
    }

}
void Action::reinitialize_tmpFile()
{
    if (tmpFile)tmpFile.close();
    tmpFile.open ("../../Logs/TmpfileLog",ios::trunc);
}
//test si les cases en entrées sont dans le meme groupe
bool Action::test_case_selected(vector<Case*> cases_selected)
{
    bool action_good = true;
    int i = 0;
    int group_id = cases_selected[0]->get_idGroup();
    i ++;
    //on ajoute dans tab group
    while(i < cases_selected.size() && action_good)
    {
        if (cases_selected[i]->get_idGroup() != group_id)
        {
            action_good = false;
        }
        i++;
    }
    return action_good;
}

void Action::affiche_cases_selected(vector<Case*> cases_selected, int time_sleep)
{
    for (int i = 0; i < cases_selected.size(); i ++)
    {
        float f = (float)i/(float)cases_selected.size();
        cases_selected[i]->color_selected(f);
    }
    grid->draw_cases();
    Sleep(time_sleep);

    for (int i = 0; i < cases_selected.size(); i ++)
    {
        cases_selected[i]->color_selected(false);
    }
}

//On reset les senseurs et recalcule les nouvelles possibilités
void Action::reset()
{
    grid->reset();
    reinitialize_nbTurnPlayed();
    reinitialize_tmpFile();
    grid->set_isOver(false);

    calcul_groups_in_grid();
    compute_NbPossibilities_in_grid();
}


//On reset les senseurs et recalcule les nouvelles possibilités
void Action::reset(vector<float> seed)
{
    grid->reset(seed);
    reinitialize_nbTurnPlayed();
    reinitialize_tmpFile();
    grid->set_isOver(false);

    calcul_groups_in_grid();
    compute_NbPossibilities_in_grid();
}
