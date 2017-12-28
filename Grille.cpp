#include "Grille.h"

Grille::Grille(Vector2f posi, int taille)
{
    this->posi = posi;
    Vector2f posi_case = posi;
    this->taille = taille;
    int taille_case = taille/5;
    numG = new NumGenerator();

    //case_score = new Case(Vector2f(0, 50), 50, font, 10, 30);

    if (!font.loadFromFile("text/calibri.ttf"))
    {
        cout << "NOOOOOOOOOO" << endl;
    }

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
    //delete case_score;
}

Case* Grille::get_Case(Vector2f posi)
{
    int i = 0 ;
    for (; i < cases.size();i++)
    {
        if (cases[i]->get_background().getGlobalBounds().contains(posi))
        {
            break;
        }
    }

    return cases[i] ;
}

Case* Grille::get_Case(Vector2f posi,Case *caseSelected)
{
    int i  = (caseSelected->get_id())-1 ;
    vector<int> idVoisin;

    if(!((i-1)<0))idVoisin.push_back(i-1);
    if(!((i+1)>24))idVoisin.push_back(i+1);
    if(!((i-5)<0))idVoisin.push_back(i-5);
    if(!((i+5)>24))idVoisin.push_back(i+5);

    for (int e=0;e<idVoisin.size();e++)
    {
        if (cases[idVoisin[e]]->get_background().getGlobalBounds().contains(posi))
        {
            return cases[idVoisin[e]] ;
        }
    }

    return 0 ;
}

void Grille::draw_cases(RenderWindow &win)
{
    for (int i = 0; i < cases.size(); i ++)
    {
        win.draw((cases[i]->get_background()));
        win.draw((cases[i]->get_text()));
    }
//    win.draw(case_score->get_background());
//    win.draw(case_score->get_text());
}

NumGenerator* Grille::get_numG()
{
    return numG;
}

