#include "EventListenner.h"  // Inclut SFML/Graphics
#include "Grille.h"

int main()
{
    // Create the main window
    sf::RenderWindow app(sf::VideoMode(1200, 800), "SFML window");
    sf::Font font;


    try
    {
        font.loadFromFile("text/calibri.ttf");
    }
    catch(exception e)
    {
        cout << "PAS LOAD" << endl;
    }

    Grille* ma_grille = new Grille(Vector2f (250, 100), 350);
    EventListenner Listenner (&app,ma_grille);

	// Start the game loop
    while (app.isOpen())
    {

        // Process events
        sf::Event event;
        Listenner.listen();
        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                app.close();
        }

        // Clear screen
        app.clear();

        // Draw the sprite
        ma_grille->draw_cases(app);

        // Update the window
        app.display();
    }

    //delete ma_grille;
    delete (ma_grille);

    return EXIT_SUCCESS;
}
