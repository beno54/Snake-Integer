#include "Grille.h"

//on initie la grille, les cases et le score.
Grille::Grille(Vector2f posi, int taille)
{
    this->posi = posi;
    Vector2f posi_case = posi;
    this->taille = taille;
    int taille_case = taille/5;

    //on initie le numGénérator qui va générer les nombres aléatoires
    numG = new NumGenerator();

    if (!font.loadFromFile("text/calibri.ttf"))
    {
        cout << "Police de texte non chargee." << endl;
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

// on supprime tous les objets dynamiques contenu dans la grille
Grille::~Grille()
{
    for (int i=0;i<cases.size();i++)
    {
        delete cases[i];
    }
    delete numG;
    delete case_score;
}

//on renvoie la case pointée par la souris
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

//on renvoie la case pointée par la souris, en regardant uniquement les voisins de la dernière case sélectionnée
Case* Grille::get_Case(Vector2f posi,Case *caseSelected)
{
    int i  = (caseSelected->get_id()) ;
    vector<int> idVoisin;

    //Attention aux indices, indice dans le tab = id - 1
    //si la case est au bord a gauche, alors pas de voisin de gauche
    if (i%5 != 1)
    {
        idVoisin.push_back(i-2);
    }
    //idem droite
    if (i%5 != 0)
    {
        idVoisin.push_back(i);
    }
    //idem haut
    if (!(i <= 5))
    {
        idVoisin.push_back(i-6);
    }
    //idem bas
    if (!(i > 20))
    {
        idVoisin.push_back(i+4);
    }

    //on teste si la souris se trouve dans une case voisine
    for (int e=0;e<idVoisin.size();e++)
    {
        if (cases[idVoisin[e]]->get_background().getGlobalBounds().contains(posi))
        {
            return cases[idVoisin[e]] ;
        }
    }

    return NULL ;
}

//on dessine les cases (texte + background) et le score
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

//on renvoie le numGenerator pour obtenir les nombre aléatoires
NumGenerator* Grille::get_numG()
{
    return numG;
}

//permet de mettre a jour le score
void Grille::update_score(int valeur)
{
    case_score->update_value(valeur);
}

