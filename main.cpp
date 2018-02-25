#include "EventListenner.h"  // Inclut SFML/Graphics
#include "Agent1_Logical.h"
#include "Button.h"
#include "string.h"

#define AFFICHAGE
#define DECISION_DELAY_MS 100
#define NB_GAME 2


int main (int argc, char* argv[])
{
    // Create the main window
    sf::RenderWindow app(sf::VideoMode(1200, 800), "SFML window");
    app.setFramerateLimit(20);
    sf::Font font;
    int option_lancement = 1 ;
    vector<float> seeds;
    Grille* ma_grille;

    string ProfilName;
    if (argc > 1)
    {
        ProfilName =  string(argv[1]) + ".csv";
        if (argc > 2)
        {
            option_lancement = atoi(argv[2]);

            cout << "option= " << option_lancement << endl;
        }
        if (argc > 5)
        {
            seeds.push_back(atof(argv[3]));
            seeds.push_back(atof(argv[4]));
            seeds.push_back(atof(argv[5]));

            cout << "seed1= " << seeds[0] << "seed2= " << seeds[1] << "seed3= " << seeds[2] << endl;
        }
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
    if (argc > 5)
    {
        ma_grille = new Grille(Vector2f (250, 100), 450, &app, seeds);
    }
    else
    {
        ma_grille = new Grille(Vector2f (250, 100), 450, &app);
    }

    cout << "profil= " << ProfilName << "bonsoir" << endl;

    Button* but_start = new Button(100, Vector2f (800, 100), font, ma_grille);
    cout << (ProfilName).c_str() << endl;
    EventListenner* Listenner = new  EventListenner(&app,ma_grille, but_start,ProfilName);
    //Grille + Nb parties à jouer + delay entre chaques choix
#ifdef DECISION_DELAY_MS
    Agent1_Logical* agent1 = new Agent1_Logical(ma_grille, NB_GAME, DECISION_DELAY_MS);
#else
    Agent1_Logical* agent1 = new Agent1_Logical(ma_grille, NB_GAME, 0);
#endif

#ifdef AFFICHAGE
    // Clear screen
    app.clear(sf::Color(242, 223, 202));

    // Draw the grille + cases + score

    ma_grille->draw_cases();
    but_start->draw(app);

    // Update the window
    app.display();
#endif

	// Start the game loop
    while (app.isOpen() && agent1->has_games2Play())
    {
        sf::Event event;
#ifdef AFFICHAGE
      //   Clear screen
        app.clear(sf::Color(242, 223, 202));

        // Draw the grille + cases + score
        ma_grille->draw_cases();
        but_start->draw(app);
#endif
        //on d�tecte le clique de souris et on fait les actions correspondantes
        //Listenner->listen();

        switch (option_lancement)
        {
            case 0 : Listenner->listen();break;
            case 1 : agent1->compute_decision(1);break;
            default : agent1->compute_decision(1);break;
        }

        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                app.close();
        }

#ifdef AFFICHAGE
        // Update the window
        app.display();
#endif
    }

    //delete ma_grille;
    delete (ma_grille);
    delete (but_start);
    delete (Listenner);

    return EXIT_SUCCESS;
}
