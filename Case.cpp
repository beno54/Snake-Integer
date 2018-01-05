#include "Case.h"

// on init la case (position, taille, valeur, id, text et background)
Case::Case(Vector2f posi, int taille, Font& font, int valeur, int id )
{
    this->posi_centre = posi;
    this->taille = taille;
    this->valeur = valeur;
    this->id = id ;
    text.setFont(font);

    //conversion int to string
    ostringstream ss;
    ss << valeur;
    text.setString(ss.str());

    //on met l'origine au centre de la case
    sf::FloatRect textRect = text.getLocalBounds();

    text.setPosition(sf::Vector2f(posi.x + taille/2.0f, posi.y + taille/2.0f));
    text.setOrigin(textRect.left + textRect.width/2.0f,
               textRect.top  + textRect.height/2.0f);

    text.setColor(Color::White);
    background.setSize(Vector2f(taille, taille));
    background.setPosition(Vector2f(posi.x, posi.y));
    background.setOutlineThickness(-1);
    background.setOutlineColor(Color::White);
    background.setFillColor(sf::Color((valeur*51)%255, (valeur*41)%255, (valeur*28)%255));
}

Case::~Case()
{

}

//on change la valeur de la case, en changeant aussi le texte correspondant et le background
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

    background.setFillColor(sf::Color((valeur*51)%255, (valeur*41)%255, (valeur*28)%255));
}

int Case::get_value()
{
    return valeur;
}

Vector2f Case::get_posi()
{
    return posi_centre;
}

int Case::get_idGroup()
{
    return idGroup;
}

void Case::set_idGroup(int newValue)
{
    idGroup = newValue;
}

//on incrémente la valeur de la case, en changeant aussi le texte correspondant et le background
void Case::update_value(int new_value)
{
    valeur += new_value;

    ostringstream ss;
    ss << valeur;
    text.setString(ss.str());

    sf::FloatRect textRect = text.getLocalBounds();
    text.setPosition(sf::Vector2f(posi_centre.x + taille/2.0f, posi_centre.y + taille/2.0f));
    text.setOrigin(textRect.left + textRect.width/2.0f,
               textRect.top  + textRect.height/2.0f);

    background.setFillColor(sf::Color((valeur*51)%255, (valeur*41)%255, (valeur*28)%255));
}

Text Case::get_text()
{
    return text;
}

RectangleShape Case::get_background()
{
    return background;
}

//on (dé)selectionne une case, en changeant sa couleur (rouge = selected)
void Case::color_selected(bool isSelected)
{
    if (isSelected)
    {
        background.setFillColor(Color::Red);
    }
    else
    {
        background.setFillColor(sf::Color((valeur*51)%255, (valeur*41)%255, (valeur*28)%255));
    }
}

int Case::get_id()
{
    return id ;
}
