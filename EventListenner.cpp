#include "EventListenner.h"

using namespace sf;
/*
Constructeur qui reçoit la grille de jeu et la fenêtre en entrée.
Cet objet sert à gérer les évènement dû à la souris sur les différents
objets présents dans la fenêtres
*/
EventListenner::EventListenner(sf::RenderWindow *win, Grille *grid)
{
    this->grid = grid;
    this->win = win;
    this->isLeftPressed = false ;
}

EventListenner::~EventListenner()
{
    //dtor
}

/*
Lancement de l'écoute des évènements :
- Bouton pressé :
    on va capter la position de la souris pour savoir quelle case est sélectionnée.
    on remplit un tableau de case sélectionnées qui doivent avoir la même valeur.
    Chaque case de ce tableau doit être tangente à la case précédente dans le tableau.
- Bouton relaché:
    Si le tableau contient plus d'une case  on calcule le score pour la dernière case
    et on regénère des nombres entre 1 et 3 pour les cases.
*/
void EventListenner::listen ()
{

    if (Mouse::isButtonPressed(Mouse::Left))
    {
        bool CaseExist = false ;
        isLeftPressed = true ;
        Vector2i localPosition = Mouse::getPosition(*win);
        Case *casePointed;

        //Ajout de la case pointée par la souris dans le tableau s'il est vide
        if (!ptr_casesSelected.size())
        {
            casePointed = grid->get_Case(Vector2f(localPosition));
            if (casePointed)
            {
                ptr_casesSelected.push_back(casePointed);
                casePointed->color_selected(true);
            }
        }
        //Ajout de la case pointée par la souris dans le tableau s'il n'est PAS vide
        else
        {
            casePointed = grid->get_Case(Vector2f(localPosition),ptr_casesSelected.back());
            //Peut être qu'une case est pointée, mais si elle n'est pas voisine de la dernière case
            // présente dans le tableau de cases sélectionnées, alors la fonction ne retournera rien et
            // casePointed sera null
            if (casePointed)
            {
                //test si  la case est déjà présente dans le tableau
                for (int e = ptr_casesSelected.size()-1;e>=0;e--)
                {
                    if (ptr_casesSelected[e]->get_id()== casePointed->get_id())
                    {
                        CaseExist = true;
                        break;
                    }
                }
                //Ajout si pas présente dans le tableau et si valeur identique
                if (!CaseExist && (casePointed->get_value() == ptr_casesSelected[0]->get_value()))
                {
                    ptr_casesSelected.push_back(casePointed);
                    casePointed->color_selected(true);
                }
                //si valeur pas identique, on regarde s'il n'y a pas u nretour en arrière (avant dernière case)
                //dans quel cas on retire la dernière case du tableau
                else if ((ptr_casesSelected.size()-1 > 0)&& (casePointed->get_id()== ptr_casesSelected[ptr_casesSelected.size()-2]->get_id()))
                {
                    ptr_casesSelected.back()->color_selected(false);
                    ptr_casesSelected.pop_back();
                }
            }
        }
    }

    if (isLeftPressed && !Mouse::isButtonPressed(Mouse::Left) && ptr_casesSelected.size() != 0)
    {
        int score = 0;
        isLeftPressed = false ;

        if (ptr_casesSelected.size() > 1)
        {
            for (int e = 0;e<ptr_casesSelected.size();e++)
            {
                ptr_casesSelected[e]->color_selected(false);
                score += ptr_casesSelected[e]->get_value();
                ptr_casesSelected[e]->set_value(grid->get_numG()->nexNum());
            }
            ptr_casesSelected.back()->set_value(score);
            grid->update_score(score);
        }
        else
        {
            ptr_casesSelected[0]->color_selected(false);
        }
        ptr_casesSelected.clear();
    }
}
