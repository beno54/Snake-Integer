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
        int get_value();
        int get_id();
        Text get_text();
        RectangleShape get_background();
        void set_value(int new_value);
        void update_value(int new_value);
        void draw();
        void color_selected(bool isSelected);


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
