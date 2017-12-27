#ifndef EVENTLISTENNER_H
#define EVENTLISTENNER_H
#include <SFML/Graphics.hpp>
#include "Grille.h"
class EventListenner
{
    public:
        EventListenner( sf::RenderWindow *win, Grille *grid);
        virtual ~EventListenner();
        void listen ();
    protected:

    private:
        sf::RenderWindow *win;
        vector<Case*>caseSelected;
        Grille *grid;
        bool isLeftPressed;
};

#endif // EVENTLISTENNER_H
