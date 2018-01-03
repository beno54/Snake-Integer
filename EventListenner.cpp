#include "EventListenner.h"

using namespace sf;
/*
Constructeur qui re�oit la grille de jeu et la fen�tre en entr�e.
Cet objet sert � g�rer les �v�nement d� � la souris sur les diff�rents
objets pr�sents dans la fen�tres
*/
EventListenner::EventListenner(sf::RenderWindow *win, Grille *grid, Button* but_start,const char* ProfilName)
{
    this->grid = grid;
    this->win = win;
    this->but_start = but_start;
    this->isLeftPressed = false ;
    this->isButtonPressed = false;
    this->action = new Action(grid,ProfilName);
}

EventListenner::~EventListenner()
{
    //delete pointeurs d OBJETS (pas de variable) créés dans cette classe
    delete (action);
}

/*
Lancement de l'�coute des �v�nements :
- Bouton press� :
    on va capter la position de la souris pour savoir quelle case est s�lectionn�e.
    on remplit un tableau de case s�lectionn�es qui doivent avoir la m�me valeur.
    Chaque case de ce tableau doit �tre tangente � la case pr�c�dente dans le tableau.
- Bouton relach�:
    Si le tableau contient plus d'une case  on calcule le score pour la derni�re case
    et on reg�n�re des nombres entre 1 et 3 pour les cases.
*/
void EventListenner::listen ()
{

    if (Mouse::isButtonPressed(Mouse::Left))
    {
        bool CaseExist = false ;
        isLeftPressed = true ;
        Vector2i localPosition = Mouse::getPosition(*win);
        Case *casePointed;

        if (but_start->get_background().getGlobalBounds().contains(Vector2f(localPosition)) && !isButtonPressed)
        {
            isButtonPressed = true;
        }
        else
        {
            //Ajout de la case point�e par la souris dans le tableau s'il est vide
            if (!ptr_casesSelected.size())
            {
                casePointed = grid->get_Case_pointed(Vector2f(localPosition));
                if (casePointed)
                {
                    ptr_casesSelected.push_back(casePointed);
                    casePointed->color_selected(true);
                }
            }
            //Ajout de la case point�e par la souris dans le tableau s'il n'est PAS vide
            else
            {
                casePointed = grid->get_neighbourOfCase_pointed(Vector2f(localPosition),ptr_casesSelected.back());
                //Peut �tre qu'une case est point�e, mais si elle n'est pas voisine de la derni�re case
                // pr�sente dans le tableau de cases s�lectionn�es, alors la fonction ne retournera rien et
                // casePointed sera null
                if (casePointed)
                {
                    for (int e = 0;e<ptr_casesSelected.size();e++)
                    {
                        if (ptr_casesSelected[e]->get_id()== casePointed->get_id())
                        {
                            CaseExist = true;
                            //cout << "voisin touched " << endl ;
                            //cout << "voisin selected"<< caseSelected.back()->get_id() << endl;
                        }
                    }
                    //Ajout si pas pr�sente dans le tableau et si valeur identique
                    if (!CaseExist && (casePointed->get_value() == ptr_casesSelected[0]->get_value()))
                    {
                        ptr_casesSelected.push_back(casePointed);
                        casePointed->color_selected(true);
                    }
                    //si valeur pas identique, on regarde s'il n'y a pas u nretour en arri�re (avant derni�re case)
                    //dans quel cas on retire la derni�re case du tableau
                    else if ((ptr_casesSelected.size()-1 > 0)&& (casePointed->get_id()== ptr_casesSelected[ptr_casesSelected.size()-2]->get_id()))
                    {
                        ptr_casesSelected.back()->color_selected(false);
                        ptr_casesSelected.pop_back();
                    }
                }
            }
        }

    }

    if (isLeftPressed && !Mouse::isButtonPressed(Mouse::Left))
    {
        Vector2i localPosition = Mouse::getPosition(*win);
        if (but_start->get_background().getGlobalBounds().contains(Vector2f(localPosition)) && isButtonPressed)
        {
            grid->reset();
            isButtonPressed = false;
        }
        else if (ptr_casesSelected.size() != 0)
        {
            isLeftPressed = false ;

            if (ptr_casesSelected.size() > 1)
            {
                //log avant action d'écraser la grille
                action->log_data(ptr_casesSelected);
                action->compute_score(ptr_casesSelected);
                if (action->get_groups_in_grid()==0)
                {
                    cout << " GAME OVER " << endl ;
                }
            }
            else
            {
                ptr_casesSelected[0]->color_selected(false);
            }
            ptr_casesSelected.clear();
        }
    }
}
