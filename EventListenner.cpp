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
        Case *tmpCase;
        //Ajout dans le tableau s'il est vide (pas besoi nde vérifier que la valeur s'y trouve déjà)
        if (!caseSelected.size())
        {
            cout <<" vide " << endl ;
            tmpCase = grid->get_Case(Vector2f(localPosition));
            if (tmpCase)
            {
                caseSelected.push_back(tmpCase);
                cout << "case ajoutée " <<  tmpCase->get_id() << endl ;
            }
        }
        else
            {

                tmpCase = grid->get_Case(Vector2f(localPosition),caseSelected.back());
                if (tmpCase)
                {

                    for (int e = 0;e<caseSelected.size();e++)
                    {
                        if (caseSelected[e]->get_id()== tmpCase->get_id())
                        {
                            CaseExist = true;
                        }
                    }
                    if (!CaseExist)
                    {
                        caseSelected.push_back(tmpCase);
                        cout << "case ajoutée " <<  tmpCase->get_id() << endl ;
                    }

                }
            }
    }

    if (isLeftPressed && !Mouse::isButtonPressed(Mouse::Left))
    {
        isLeftPressed = false ;
        cout<<" msg" <<endl;
        caseSelected.clear();
    }
}
