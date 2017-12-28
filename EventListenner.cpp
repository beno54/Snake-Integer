#include "EventListenner.h"

using namespace sf;
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

void EventListenner::listen ()
{

    if (Mouse::isButtonPressed(Mouse::Left))
    {
        isLeftPressed = true ;
        bool CaseExist = false ;
        Vector2i localPosition = Mouse::getPosition(*win);
        Case *casePointed;
        //Ajout dans le tableau s'il est vide (pas besoi nde vérifier que la valeur s'y trouve déjà)
        if (!ptr_casesSelected.size())
        {
            //cout <<" vide " << endl ;
            casePointed = grid->get_Case(Vector2f(localPosition));
            if (casePointed)
            {
                ptr_casesSelected.push_back(casePointed);
                casePointed->color_selected(true);
                //cout << "case ajoutée " <<  casePointed->get_id() << endl ;
            }
        }
        else
        {
            casePointed = grid->get_Case(Vector2f(localPosition),ptr_casesSelected.back());
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
                if (!CaseExist && (casePointed->get_value() == ptr_casesSelected[0]->get_value()))
                {
                    ptr_casesSelected.push_back(casePointed);
                    casePointed->color_selected(true);
                    //cout << "case ajoutée " <<  casePointed->get_id() << endl;
                }
                else if (ptr_casesSelected.size()-1 > 0)
                {
                    if (casePointed->get_id()== ptr_casesSelected[ptr_casesSelected.size()-2]->get_id())
                    {
                        //cout << "Retour arriere" << endl;
                        ptr_casesSelected.back()->color_selected(false);
                        ptr_casesSelected.pop_back();
                    }
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

        //cout<<" unselected" <<endl;
        ptr_casesSelected.clear();
    }
}
