#include "Action.h"

Action::Action(Grille *grid)
{
    //ctor
    this->grid = grid;
}

Action::~Action()
{
    //dtor
}

void Action::compute_score(vector<Case*> cases_selected)
{
    int score = 0;

    for (int e = 0;e<cases_selected.size();e++)
    {
        cases_selected[e]->color_selected(false);
        score += cases_selected[e]->get_value();
        cases_selected[e]->set_value(grid->get_numG()->nexNum());
    }
    cases_selected.back()->set_value(score);
    grid->update_score(score);
}
