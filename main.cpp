#include "EventListenner.h"  // Inclut SFML/Graphics
#include "Agent1b_Logical.h"
#include "Button.h"
#include "string.h"
//#include "c_api.h"



int main (int argc, char* argv[])
{
    // Create the main window
    sf::RenderWindow app(sf::VideoMode(1200, 800), "SNAKE INTEGER");
    app.setFramerateLimit(20);
    sf::Font font;
    //default value to launch game. 0 is human, 1 is agent 1
    int option_lancement = 4;
    vector<float> seeds;
    Grille* ma_grille;

    //veriables propre à l'agent
    int affichage = 1 ;
    int nb_game = 100;
    int ms_delay = 1000; // ms

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

    //on cr?e la grille et le EventListener
    if (argc > 6)
    {
        ma_grille = new Grille(Vector2f (250, 100), 450, &app, seeds);
    }
    else
    {
        ma_grille = new Grille(Vector2f (250, 100), 450, &app);
    }

//    std::string command2 = "py";
//    system(command2.c_str());
//
//    command2 = "import SuperNNSnakeInteger_b";
//    system(command2.c_str());

//    std::string filename = "SuperNNSnakeInteger_b.py train ModelNN Agent1_to_train.csv";
//    std::string command = "SuperNNSnakeInteger_b.train_with_model('ModelNN', '1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5')";
//    std::string command = "py SuperNNSnakeInteger_b.train_with_model('ModelNN', 'Agent1_to_train.csv')";

//    std::string command = "py SuperNNSnakeInteger_b.py predict ModelNN 1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5,1,2,3,4,5";
//    system(command.c_str());

//    ///Test d'existance du fichier
//    ifstream fichier("log.py");
//
//    if (fichier.fail()) {
//        cout << "Fichier de script introuvable : " << "log.py" <<"\n";
//        return 0;
//    }
//
//    ///Lance le script
//    cout << "\n--- Execution du script : " << "log.py" <<" ---\n";
//
//    // Ouvre le script python a executer
//    FILE* pyFile = fopen("log.py", "r");
//
//    Py_Initialize();
//
//
//    PyRun_SimpleString("");
//    // Execute le script
//    PyRun_AnyFile(pyFile, "log.py");
//
//    Py_Finalize();


    EventListenner* Listenner;
    Agent1b_Logical* agent1;
    Button* but_start = new Button(100, Vector2f (800, 100), font, ma_grille);
    switch (option_lancement)
    {
        // case 0 appelle qd meme agent 1 constructeur vide pour initialiser son nbgamtoplay à 0
        case 0 :        Listenner = new  EventListenner(&app,ma_grille, but_start,ProfilName);agent1=new Agent1b_Logical(); break;
        case 1 ... 4 :  agent1 = new Agent1b_Logical(ma_grille, nb_game, ms_delay,ProfilName, option_lancement); break;
        case 10 :  agent1 = new Agent1b_Logical(ma_grille, nb_game, ms_delay,ProfilName, option_lancement); break;
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
    while (app.isOpen() && (agent1->has_games2Play() || option_lancement == 0))
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

//        switch (option_lancement)
//        {
//            case 0 : Listenner->listen();break;
//            case 1 : agent1->compute_decision(1,affichage);break;
//            case 2 : agent1->compute_decision(2,affichage);break;
//            case 3 : agent1->compute_decision(3,affichage);break;
//            case 4 : agent1->compute_decision(4,affichage);break;
//            case 10 : agent1->learn_coeff(4);break;
//        }
        agent1->test_copy();


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
