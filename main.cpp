#include "EventListenner.h"  // Inclut SFML/Graphics
#include "Agent1b_Logical.h"
#include "Button.h"
#include "string.h"
#include "agent2.h"
//#include "c_api.h"



int main (int argc, char* argv[])
{
    // Create the main window
    sf::RenderWindow app(sf::VideoMode(1200, 800), "SNAKE INTEGER");
    app.setFramerateLimit(20);
    sf::Font font;
    //default value to launch game. 0 is human, 1 is agent 1
    int option_lancement = 20;
    vector<float> seeds;
    Grille* ma_grille;

    //veriables propre à l'agent
    int affichage = 1 ;
    int nb_game = 50;
    int ms_delay = 0; // ms

    if (affichage ==0)ms_delay==0;

    string ProfilName;
    if (argc > 1)
    {
        ProfilName =  string(argv[1]) + ".csv";
        cout << ProfilName << endl;
        if (argc > 2)
        {
            option_lancement = atoi(argv[2]);
            cout << "option = " << option_lancement << endl;
            if (option_lancement>0)
            {
                affichage = atoi(argv[3]);
                cout << "affichage = " << affichage << endl;
                ms_delay  = atoi(argv[4]);
                cout << "ms_delay = " << ms_delay << endl;
                nb_game   = atoi(argv[5]);
                cout << "nb_game = " << nb_game << endl;

                if (affichage ==0) ms_delay = 0 ;
            }
        }
        if (argc > 6)
        {
            seeds.push_back(atof(argv[6]));
            seeds.push_back(atof(argv[7]));
            seeds.push_back(atof(argv[8]));

            cout << "seed1= " << seeds[0] << "seed2= " << seeds[1] << "seed3= " << seeds[2] << endl;
        }
    }
    else
    {
        ProfilName = "agent2.csv";
    }

    try
    {
        font.loadFromFile("text/calibri.ttf");
    }
    catch(exception e)
    {
        cout << "Police de texte non chargee." << endl;
    }

    //on cr?e la grille et le EventListener
    if (argc > 6)
    {
        ma_grille = new Grille(Vector2f (250, 100), 450, &app, seeds);
    }
    else
    {
        ma_grille = new Grille(Vector2f (250, 100), 450, &app);
    }


    EventListenner* Listenner = NULL;
    Agent1b_Logical* agent1 = NULL;
    agent2* my_agent2 = NULL;
    Button* but_start = new Button(100, Vector2f (800, 100), font, ma_grille);
    switch (option_lancement)
    {
        // case 0 appelle qd meme agent 1 constructeur vide pour initialiser son nbgamtoplay à 0
        case 0 :        Listenner = new  EventListenner(&app,ma_grille, but_start,ProfilName);agent1=new Agent1b_Logical(); break;
        case 1 ... 15 :  agent1 = new Agent1b_Logical(ma_grille, nb_game, ms_delay,ProfilName, option_lancement); break;
        case 20 ... 30 :  my_agent2 = new agent2(ma_grille, nb_game, ms_delay,ProfilName, option_lancement); break;
    }


    cout << (ProfilName).c_str() << endl;

    //Grille + Nb parties à jouer + delay entre chaques choix


    if (affichage)
    {

        // Clear screen
        app.clear(sf::Color(242, 223, 202));

        // Draw the grille + cases + score

        ma_grille->draw_cases();
        but_start->draw(app);

        // Update the window
        app.display();
    }



	// Start the game loop
    while (app.isOpen())
    {
        sf::Event event;
        if (affichage)
        {
            //   Clear screen
            app.clear(sf::Color(242, 223, 202));

            // Draw the grille + cases + score
            ma_grille->draw_cases();
            but_start->draw(app);
        }
        //on d?tecte le clique de souris et on fait les actions correspondantes
        //Listenner->listen();

        switch (option_lancement)
        {
            case 0 : Listenner->listen();break;
            case 1 : agent1->compute_decision(1,affichage);break;
            case 2 : agent1->compute_decision(2,affichage);break;
            case 3 : agent1->compute_decision(3,affichage);break;
            case 4 : agent1->learn_coeff(4);break;
            case 11 : agent1->learn_cluster("../../Logs/Learn_coeff/Learning_agent1_10games_newPop5.csv"); break ;
            case 12 : agent1->compute_decision_predict(affichage);break ;
            case 20 : my_agent2->compute_decision(0, affichage);break ;
        }



        while (app.pollEvent(event))
        {
            // Close window : exit
            if (event.type == sf::Event::Closed)
                app.close();
        }

        if (affichage)
        {
            // Update the window
            app.display();
        }

        if (my_agent2!=NULL)
        {
            if (!my_agent2->has_games2Play())
            {
                break;
            }
        }
        if (agent1!=NULL)
        {
            if (!agent1->has_games2Play())
            {
                break;
            }
        }
    }

    //delete ma_grille;
    delete (ma_grille);
    delete (but_start);

    switch (option_lancement)
        {
            case 0 : delete (Listenner);;break;
            case 1 ... 4 : delete (agent1);break;
            case 10 :  delete (agent1);break;
        }


    return EXIT_SUCCESS;
}
