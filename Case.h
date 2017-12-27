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
        Case(Vector2f posi, int taille, Font& font, int valeur, int id);
        virtual ~Case();
        void set_value(int new_value);
        int get_value();
        void update_value(int new_value);
        Text get_text();
        RectangleShape get_background();
        void draw();
        int get_id();
    protected:
        Vector2f get_posi();
        int valeur, taille;
        Vector2f posi_centre;
        Text text;
        RectangleShape background;
    private:
        int id ;
};

#endif // CASE_H
