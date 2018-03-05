#include "Struct_Agent.h"

Struct_Agent::Struct_Agent(Grille* senseurs, int nb_game2Play,int decision_delay,string ProfilName)
{
    //ctor
    this->senseurs = senseurs;
    this->nb_game2Play = nb_game2Play;
    this->decision_delay =decision_delay;
    this->ProfilName =  ProfilName;
}

Struct_Agent::~Struct_Agent()
{
    //dtor
}

