#ifndef GRILLE_H
#define GRILLE_H

#include "Case.h"
#include "NumGenerator.h"

class Grille
{
    public:
        Grille(Vector2f posi, int taille, RenderWindow* win);
        virtual ~Grille();
        Case* get_Case_pointed(Vector2f posi);
        Case* get_neighbourOfCase_pointed(Vector2f posi,Case *caseSelected);
        vector<Case*> get_Cases();
        Case* get_Case_score();
        void draw_cases();
        NumGenerator* get_numG();
        void update_score(int valeur);
        void reset();
        void reset_AllidGroup();
        void set_isOver(bool resultat);
        bool get_isOver();
        float get_mean();
        vector<Case*> get_voisins(Case* case_courante);
        vector<Case*> get_all_voisins(Case* case_courante);
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
        RenderWindow* window;
};

#endif // GRILLE_H
