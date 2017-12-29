#include "Button.h"

Button::Button(int taille, Vector2f posi, Font& font, Grille* grid)
{
    //ctor
    this->grid = grid;
    this->posi_centre = posi;
    this->taille = taille;
    text.setFont(font);

    text.setString("Restart");
    text.setColor(Color::White);
    //on met l'origine au centre de la case
    sf::FloatRect textRect = text.getLocalBounds();

    text.setPosition(sf::Vector2f(posi.x + taille/2.0f, posi.y + taille/2.0f));
    text.setOrigin(textRect.left + textRect.width/2.0f,
               textRect.top  + textRect.height/2.0f);

    background.setSize(Vector2f(taille, taille));
    background.setPosition(Vector2f(posi.x, posi.y));
    background.setOutlineThickness(-1);
    background.setOutlineColor(Color::White);
    background.setFillColor(Color::Blue);
}

Button::~Button()
{
    //dtor
}

Text Button::get_text()
{
    return text;
}

RectangleShape Button::get_background()
{
    return background;
}

//on dessine le bouton (texte + background)
void Button::draw(RenderWindow &win)
{
    win.draw(background);
    win.draw(text);
}
