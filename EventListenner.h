#ifndef EVENTLISTENNER_H
#define EVENTLISTENNER_H
#include <SFML/Graphics.hpp>
#include "Button.h"
#include "Action.h"

class EventListenner
{
    public:
        EventListenner( sf::RenderWindow *win, Grille *grid, Button* but_start);
        virtual ~EventListenner();

        void listen ();
    protected:

    private:
        sf::RenderWindow *win;
        vector<Case*>ptr_casesSelected;
        Grille *grid;
        Button* but_start;
        bool isLeftPressed, isButtonPressed;
        Action* action;
};

#endif // EVENTLISTENNER_H
