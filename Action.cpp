#include "Action.h"

Action::Action(Grille *grid, const char* ProfilName)
{
    //ctor
    this->grid = grid;
    //ouverture des fichiers profils durant l'exécution du programme
    LogFile.open (ProfilName,ios::app);
    cout << "Ouverture du fichier de logs" <<endl ;
}

Action::~Action()
{
    //dtor

    //fermeture des fichiers de profils à la fermeture
    LogFile.close();
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
        LogFile << currentvalue ;
        LogFile << ";" ;
    }

    //calcul moyenne
    GridAverage /= 25 ;

    //Log  moyenne de la grille
    LogFile << GridAverage ;
    LogFile << ";" ;

    //Log score grille courrante
    LogFile << grid->get_Case_score()->get_value() ;
    LogFile << ";" ;

    //Logs Valeur max
    LogFile << maxvalue ;
    LogFile << ";" ;

    //Logs Cases sélectionnées
    for (int e=0;e<cases_selected.size();e++)
    {
        caseSelectOrNot[cases_selected[e]->get_id()-1]=1;
    }

    for (int e=0;e<25;e++)
    {
        LogFile << caseSelectOrNot[e] ;
        LogFile << ";" ;
    }

     //Log case en fin de tableau ou non
     for (int e=0;e<25;e++)
     {
        if ((cases_selected[cases_selected.size()-1]->get_id()-1) == e)
        {
            LogFile << 1;
        }
        else
        {
            LogFile << 0 ;
        }
        LogFile << ";" ;
     }

     //Log de valeur sélectionnée
     LogFile << cases_selected[0]->get_value() ;
     LogFile << ";" ;

     //Log de nombre de cases sélectionnées
     LogFile << cases_selected.size() ;
     LogFile << ";" ;

     //Log valeur ajoutée au score = nb case sélectionnée * valeur sélecitonnée
     LogFile << cases_selected.size()* cases_selected[0]->get_value() ;
     LogFile << ";" ;

     //Log Nombres de groupements de même valeur
     LogFile << get_groups_in_grid();
     cout << "nb groupe " <<  get_groups_in_grid()<< endl ;

     //Log fin de log
     LogFile << std::endl ;
     cout << "Ecriture dans fichier de logs" <<endl ;
}

/*
renvoie le nombre de possibilités restantes !
*/
int Action::get_groups_in_grid()
{

    int *TabIdGroup = get_tab_groups_in_grid();
    int NbPossiblite = 0;

    int nb_group = 0 ;
    for (int e = 0 ; e < 25 ; e++)
    {
       if (TabIdGroup[e]> nb_group) nb_group = TabIdGroup[e];
    }
    for (int e = 0 ; e < nb_group ; e++)
    {
        int cpt = 0 ;
        for (int i = 0 ; i < 25 ; i++)
        {
           if (TabIdGroup[i] == e) cpt ++ ;
        }
        if (cpt > 1) NbPossiblite++;
    }
    return NbPossiblite ;
}
/*
Compte le nombre de groupes (associes à chaques cases son groupe)
Compare avec le voisin du bas et celui de droite
*/
int* Action::get_tab_groups_in_grid()
{
    vector<Case*>  Cases = grid->get_Cases();
    int Nbgroup = 1 ;
    int *TabIdGroup= new int [25];
    for (int e = 0 ; e < 25 ; e++){TabIdGroup[e]=0;}

    for (int e = 0 ; e < 25 ; e++)
    {
        if (TabIdGroup[e]==0)TabIdGroup[e]= Nbgroup++;

        //Test si il y a voisin à droite et si la même valeur
        if ( ((e+1)%5 !=0) && Cases[e]->get_value() == Cases[e+1]->get_value() &&  TabIdGroup[e]!= TabIdGroup[e+1])
            if (TabIdGroup[e+1] != 0 )
            {
                Nbgroup--;
                TabIdGroup[e]=TabIdGroup[e+1];
            }
            else TabIdGroup[e+1]=TabIdGroup[e];

        //Test si il y a voisin en bas et si la même valeur
        if ( !((e+1)>20) && Cases[e]->get_value() == Cases[e+5]->get_value() &&  TabIdGroup[e]!= TabIdGroup[e+5])
             if (TabIdGroup[e+5] != 0 )
            {
                Nbgroup--;
                TabIdGroup[e]=TabIdGroup[e+5];
            }
            else TabIdGroup[e+5]=TabIdGroup[e];
    }
    return  TabIdGroup ;
}
