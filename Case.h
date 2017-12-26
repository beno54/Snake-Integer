#ifndef CASE_H
#define CASE_H

#include <SFML/Graphics.hpp>

#include <string>
#include <sstream>
#include <iostream>

using namespace sf;
using namespace std;

class Case
{
    public:
        Case(Vector2f posi, int taille, Font& font, int valeur);
        virtual ~Case();
        void set_value(int new_value);
        int get_value();
        Vector2f get_posi();
        void update_value(int new_value);
        Text get_text();
        RectangleShape get_background();
        void draw();
    protected:
        int valeur, taille;
        Vector2f posi_centre;
        Text text;
        RectangleShape background;
    private:
};

#endif // CASE_H
