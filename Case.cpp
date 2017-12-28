#include "Case.h"

Case::Case(Vector2f posi, int taille, Font& font, int valeur, int id )
{
    this->posi_centre = posi;
    this->taille = taille;
    this->valeur = valeur;
    this->id = id ;
    text.setFont(font);
    ostringstream ss;
    ss << valeur;
    text.setString(ss.str());
    cout << ss.str() << endl;

    sf::FloatRect textRect = text.getLocalBounds();

    text.setPosition(sf::Vector2f(posi.x + taille/2.0f, posi.y + taille/2.0f));
    text.setOrigin(textRect.left + textRect.width/2.0f,
               textRect.top  + textRect.height/2.0f);

    text.setColor(Color::Blue);
    background.setSize(Vector2f(taille, taille));
    background.setPosition(Vector2f(posi.x, posi.y));
    background.setOutlineThickness(-1);
    background.setOutlineColor(Color::Blue);
}

Case::~Case()
{

}

void Case::set_value(int new_value)
{
    valeur = new_value;
    ostringstream ss;
    ss << valeur;
    text.setString(ss.str());

    sf::FloatRect textRect = text.getLocalBounds();
    text.setPosition(sf::Vector2f(posi_centre.x + taille/2.0f, posi_centre.y + taille/2.0f));
    text.setOrigin(textRect.left + textRect.width/2.0f,
               textRect.top  + textRect.height/2.0f);
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

void Case::color_selected(bool isSelected)
{
    if (isSelected)
    {
        background.setFillColor(Color::Red);
    }
    else
    {
        background.setFillColor(Color::White);
    }
}

int Case::get_id()
{
    return id ;
}
