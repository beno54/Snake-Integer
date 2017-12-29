#ifndef BUTTON_H
#define BUTTON_H

#include "Grille.h"

class Button
{
    public:
        Button(int taille, Vector2f posi, Font& font, Grille* grid);
        virtual ~Button();
        Text get_text();
        RectangleShape get_background();
        void draw(RenderWindow &win);
    protected:
    private:
        Grille* grid;
        int taille;
        Vector2f posi_centre;
        Text text;
        RectangleShape background;
};

#endif // BUTTON_H
