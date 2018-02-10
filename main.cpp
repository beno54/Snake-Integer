#include "EventListenner.h"  // Inclut SFML/Graphics
#include "Agent1_Logical.h"
#include "Button.h"
#include "string.h"
int main (int argc, char* argv[])
{
    // Create the main window
    sf::RenderWindow app(sf::VideoMode(1200, 800), "SFML window");
    app.setFramerateLimit(20);
    sf::Font font;

    string ProfilName;
    if (argc > 1)
    {
        ProfilName =  string(argv[1]) + ".csv";
    }
    else
    {
        ProfilName = "ProfilLogs.csv";
    }

    try
    {
        font.loadFromFile("text/calibri.ttf");
    }
    catch(exception e)
    {
        cout << "Police de texte non chargee." << endl;
    }

    //on cr�e la grille et le EventListener
    Grille* ma_grille = new Grille(Vector2f (250, 100), 450, &app);
    Button* but_start = new Button(100, Vector2f (800, 100), font, ma_grille);
    cout << (ProfilName).c_str() << endl;
    EventListenner* Listenner = new  EventListenner(&app,ma_grille, but_start,ProfilName);
    Agent1_Logical* agent1 = new Agent1_Logical(ma_grille, 100);

    // Clear screen
    app.clear(sf::Color(242, 223, 202));

    // Draw the grille + cases + score
    ma_grille->draw_cases();
    but_start->draw(app);
    // Update the window
    app.display();

	// Start the game loop
    while (app.isOpen() && agent1->has_games2Play())
    {
        sf::Event event;

        // Clear screen
        app.clear(sf::Color(242, 223, 202));

        // Draw the grille + cases + score
        ma_grille->draw_cases();
        but_start->draw(app);

        //on d�tecte le clique de souris et on fait les actions correspondantes
        //Listenner->listen();

        agent1->compute_decision();

        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                app.close();
        }


        // Update the window
        app.display();

    }

    //delete ma_grille;
    delete (ma_grille);
    delete (but_start);
    delete (Listenner);

    return EXIT_SUCCESS;
}
