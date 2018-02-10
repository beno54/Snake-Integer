#include "Struct_Agent.h"

Struct_Agent::Struct_Agent(Grille* senseurs, int nb_game2Play)
{
    //ctor
    this->senseurs = senseurs;
    this->nb_game2Play = nb_game2Play;
}

Struct_Agent::~Struct_Agent()
{
    //dtor
}

