#include "Case.h"

Case::Case(Vector2f posi, int taille, Font& font, int valeur)
{
    this->posi_centre = posi;
    this->taille = taille;
    this->valeur = valeur;

    text.setFont(font);
    ostringstream ss;
    ss << valeur;
    text.setString(ss.str());

    sf::FloatRect textRect = text.getLocalBounds();

    text.setPosition(sf::Vector2f(posi.x + taille/2.0f, posi.y + taille/2.0f));
    text.setOrigin(textRect.left + textRect.width/2.0f,
               textRect.top  + textRect.height/2.0f);

    text.setColor(Color::Blue);
    background.setSize(Vector2f(taille, taille));
    background.setPosition(Vector2f(posi.x, posi.y));
    background.setOutlineThickness(1);
    background.setOutlineColor(Color::Blue);
}

Case::~Case()
{

}

void Case::set_value(int new_value)
{
    valeur = new_value;
}

int Case::get_value()
{
    return valeur;
}

Vector2f Case::get_posi()
{
    return posi_centre;
}

void Case::update_value(int new_value)
{
    valeur += new_value;
}

Text Case::get_text()
{
    return text;
}

RectangleShape Case::get_background()
{
    return background;
}
