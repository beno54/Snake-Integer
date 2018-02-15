#include "Grille.h"

//on initie la grille, les cases et le score.
Grille::Grille(Vector2f posi, int taille, RenderWindow* win)
{
    window = win;
    this->posi = posi;
    Vector2f posi_case = posi;
    this->taille = taille;
    int taille_case = taille/5;
    isOver = false;
    //on initie le numGénérator qui va générer les nombres aléatoires
    numG = new NumGenerator();

    if (!font.loadFromFile("text/calibri.ttf"))
    {
        cout << "Police de texte non chargee." << endl;
    }

    case_score = new Case(Vector2f(800, 350), taille_case, font, 0, 30);

    for (int i = 0; i < 25; i++)
    {
        posi_case.x = posi.x + (i%5)*taille_case;
        posi_case.y = posi.y + (i/5)*taille_case;
        Case* new_case = new Case(posi_case, taille_case, font, numG->nexNum(),(i+1));
        cases.push_back(new_case);
    }

    //Gameover message
    gameOver.setFont(font);

    //conversion int to string
    gameOver.setString("GAME OVER");
    gameOver.setCharacterSize(64);
    //on met l'origine au centre de la case

    sf::FloatRect textRect = gameOver.getLocalBounds();

    gameOver.setPosition(sf::Vector2f(posi.x + taille/2.0f, posi.y + taille/2.0f));
    gameOver.setOrigin(textRect.left + textRect.width/2.0f,
               textRect.top  + textRect.height/2.0f);
    gameOver.setColor(Color::Red);
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
Case* Grille::get_Case_pointed(Vector2f posi)
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
Case* Grille::get_neighbourOfCase_pointed(Vector2f posi,Case *caseSelected)
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
void Grille::draw_cases()
{
    for (int i = 0; i < cases.size(); i ++)
    {
        window->draw((cases[i]->get_background()));
        window->draw((cases[i]->get_text()));
    }
    window->draw(case_score->get_background());
    window->draw(case_score->get_text());
    if (isOver)
    {
        window->draw(gameOver);
    }
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

void Grille::reset()
{
    for (int i = 0; i < 25; i++)
    {
        cases[i]->set_value(numG->nexNum());
    }
    case_score->set_value(0);
}

vector<Case*> Grille::get_Cases()
{
    return cases ;
}
Case* Grille::get_Case_score()
{
    return case_score ;
}

void Grille::reset_AllidGroup()
{
    for (int i = 0; i < 25; i++)
    {
        cases[i]->set_idGroup(0);
    }
}

void Grille::set_isOver(bool resultat)
{
    isOver = resultat;
}

bool Grille::get_isOver()
{
    return isOver;
}

vector<Case*> Grille::get_voisins(Case* case_courante)
{
    int id_courante = case_courante->get_id();
    vector<Case*> voisins;

    //voisin de droite ?
    if (id_courante%5 != 0 && case_courante->get_value() == cases[id_courante]->get_value())
    {
        //ok donc rajout
        voisins.push_back(cases[id_courante]);
    }

    //voisin de gauche ?
    if (id_courante%5 != 1 && case_courante->get_value() == cases[id_courante-2]->get_value())
    {
        //ok donc rajout
        voisins.push_back(cases[id_courante-2]);
    }

    //voisin du haut ?
    if (id_courante > 5 && case_courante->get_value() == cases[id_courante-6]->get_value())
    {
        //ok donc rajout
        voisins.push_back(cases[id_courante-6]);
    }

    //voisin du bas ?
    if (id_courante <= 20 && case_courante->get_value() == cases[id_courante+4]->get_value())
    {
        //ok donc rajout
        voisins.push_back(cases[id_courante+4]);
    }

    return voisins;
}

vector<Case*> Grille::get_all_voisins(Case* case_courante)
{
    int id_courante = case_courante->get_id();
    vector<Case*> voisins;

    //voisin de droite ?
    if (id_courante%5 != 0 )
    {
        //ok donc rajout
        voisins.push_back(cases[id_courante]);
    }

    //voisin de gauche ?
    if (id_courante%5 != 1 )
    {
        //ok donc rajout
        voisins.push_back(cases[id_courante-2]);
    }

    //voisin du haut ?
    if (id_courante > 5 )
    {
        //ok donc rajout
        voisins.push_back(cases[id_courante-6]);
    }

    //voisin du bas ?
    if (id_courante <= 20)
    {
        //ok donc rajout
        voisins.push_back(cases[id_courante+4]);
    }

    return voisins;
}
