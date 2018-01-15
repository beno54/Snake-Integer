#ifndef GRILLE_H
#define GRILLE_H

#include "Case.h"
#include "NumGenerator.h"

class Grille
{
    public:
        Grille(Vector2f posi, int taille);
        virtual ~Grille();
        Case* get_Case_pointed(Vector2f posi);
        Case* get_neighbourOfCase_pointed(Vector2f posi,Case *caseSelected);
        vector<Case*> get_Cases();
        Case* get_Case_score();
        void draw_cases(RenderWindow &win);
        NumGenerator* get_numG();
        void update_score(int valeur);
        void reset();
        void reset_AllidGroup();
        void set_isOver(bool resultat);
    protected:
    private:
        vector<Case*> cases;
        Font font;
        int taille;
        Vector2f posi;
        NumGenerator* numG;
        Case* case_score;
        Text gameOver;
        bool isOver ;
};

#endif // GRILLE_H
