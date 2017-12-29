#ifndef GRILLE_H
#define GRILLE_H

#include "Case.h"
#include "NumGenerator.h"

class Grille
{
    public:
        Grille(Vector2f posi, int taille);
        virtual ~Grille();
        Case* get_Case(Vector2f posi);
        Case* get_Case(Vector2f posi,Case *caseSelected);
        void draw_cases(RenderWindow &win);
        NumGenerator* get_numG();
        void update_score(int valeur);
        void reset();
    protected:
    private:
        vector<Case*> cases;
        Font font;
        int taille;
        Vector2f posi;
        NumGenerator* numG;
        Case* case_score;
};

#endif // GRILLE_H
