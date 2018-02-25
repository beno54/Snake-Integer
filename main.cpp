#include "EventListenner.h"  // Inclut SFML/Graphics
#include "Agent1_Logical.h"
#include "Button.h"
#include "string.h"

//#define AFFICHAGE
//#define DECISION_DELAY_MS 100
#define NB_GAME 100


int main (int argc, char* argv[])
{
    // Create the main window
    sf::RenderWindow app(sf::VideoMode(1200, 800), "SNAKE INTEGER");
    app.setFramerateLimit(20);
    sf::Font font;
    //default value to launch game. 0 is human, 1 is agent 1
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



    EventListenner* Listenner;
    Agent1_Logical* agent1;
    Button* but_start = new Button(100, Vector2f (800, 100), font, ma_grille);
    switch (option_lancement)
    {
        case 0 :    Listenner = new  EventListenner(&app,ma_grille, but_start,ProfilName);

                    break;
        case 1 ... 4 :
            #ifdef DECISION_DELAY_MS
                agent1 = new Agent1_Logical(ma_grille, NB_GAME, DECISION_DELAY_MS);
            #else
                 agent1 = new Agent1_Logical(ma_grille, NB_GAME, 0);
            #endif
            break;
        default :
            #ifdef DECISION_DELAY_MS
                agent1 = new Agent1_Logical(ma_grille, NB_GAME, DECISION_DELAY_MS);
            #else
                 agent1 = new Agent1_Logical(ma_grille, NB_GAME, 0);
            #endif
            break;
    }


    cout << (ProfilName).c_str() << endl;

    //Grille + Nb parties à jouer + delay entre chaques choix


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
            case 2 : agent1->compute_decision(2);break;
            case 3 : agent1->compute_decision(3);break;
            case 4 : agent1->compute_decision(4);break;
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

    switch (option_lancement)
        {
            case 0 : delete (Listenner);;break;
            case 1 ... 4 : delete (agent1);break;
        }


    return EXIT_SUCCESS;
}
