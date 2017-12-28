#include "Grille.h"

Grille::Grille(Vector2f posi, int taille)
{
    this->posi = posi;
    Vector2f posi_case = posi;
    this->taille = taille;
    int taille_case = taille/5;
    numG = new NumGenerator();

    if (!font.loadFromFile("text/calibri.ttf"))
    {
        cout << "NOOOOOOOOOO" << endl;
    }

    case_score = new Case(Vector2f(800, 200), taille_case, font, 0, 30);

    for (int i = 0; i < 25; i++)
    {
        posi_case.x = posi.x + (i%5)*taille_case;
        posi_case.y = posi.y + (i/5)*taille_case;
        Case* new_case = new Case(posi_case, taille_case, font, numG->nexNum(),(i+1));
        cases.push_back(new_case);
    }
}

Grille::~Grille()
{
    for (int i=0;i<cases.size();i++)
    {
        delete cases[i];
    }
    delete numG;
    delete case_score;
}

Case* Grille::get_Case(Vector2f posi)
{
    int i = 0 ;
    for (; i < cases.size();i++)
    {
        if (cases[i]->get_background().getGlobalBounds().contains(posi))
        {
            return cases[i] ;
        }
    }

    return NULL;
}

Case* Grille::get_Case(Vector2f posi,Case *caseSelected)
{
    int i  = (caseSelected->get_id()) ;
    vector<int> idVoisin;

    //Attention aux indices, indice dans le tab = id - 1
    //si pas case bord gauche, alors on peut regarder le voisin de gauche
    if (i%5 != 1)
    {
        idVoisin.push_back(i-2);
    }
    //si pas case bord droite, alors on peut regarder le voisin de droite
    if (i%5 != 0)
    {
        idVoisin.push_back(i);
    }
    //si pas case bord haut, alors on peut regarder le voisin de haut
    if (!(i <= 5))
    {
        idVoisin.push_back(i-6);
    }
    //si pas case bord bas, alors on peut regarder le voisin de bas
    if (!(i >= 20))
    {
        idVoisin.push_back(i+4);
    }

    for (int e=0;e<idVoisin.size();e++)
    {
        if (cases[idVoisin[e]]->get_background().getGlobalBounds().contains(posi))
        {
            return cases[idVoisin[e]] ;
        }
    }

    return NULL ;
}

void Grille::draw_cases(RenderWindow &win)
{
    for (int i = 0; i < cases.size(); i ++)
    {
        win.draw((cases[i]->get_background()));
        win.draw((cases[i]->get_text()));
    }
    win.draw(case_score->get_background());
    win.draw(case_score->get_text());
}

NumGenerator* Grille::get_numG()
{
    return numG;
}

void Grille::update_score(int valeur)
{
    case_score->update_value(valeur);
}

