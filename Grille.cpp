#include "Grille.h"

Grille::Grille(Vector2f posi, int taille)
{
    this->posi = posi;
    Vector2f posi_case = posi;
    this->taille = taille;
    int taille_case = taille/5;

    if (!font.loadFromFile("text/calibri.ttf"))
    {
        cout << "NOOOOOOOOOO" << endl;
    }

    for (int i = 0; i < 25; i++)
    {
        posi_case.x = posi.x + (i%5)*taille_case;
        posi_case.y = posi.y + (i/5)*taille_case;
        Case* new_case = new Case(posi_case, taille_case, font, i);
        cases.push_back(new_case);
    }
}

Grille::~Grille()
{
    //dtor
}

Case* Grille::get_Case(Vector2i posi)
{

}

void Grille::draw_cases(RenderWindow &win)
{
    for (int i = 0; i < cases.size(); i ++)
    {
        win.draw((cases[i]->get_background()));
        win.draw((cases[i]->get_text()));
    }
}
