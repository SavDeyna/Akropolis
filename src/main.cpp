#include "Partie.h"

#include <iostream>

#ifdef _WIN32
#ifndef NOMINMAX 
#define NOMINMAX
#endif
#define byte win_byte_override
#include <windows.h>
#endif


int main() {

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif
    std::cout<<"Lancement du jeu\n";
    string choix ="";
    do {
        std::cout<<"Veillez choisir un mode de lancement : \nTapez 1 : Mode Console\nTapez 2 : Debug\nAutre : Sortie\n";
        std::cin>> choix;
    }while (choix !="1" && choix !="0" && choix !="2");

    //Mode console
    if (choix == "1"){
        std::cout<<"Mode console\n\n";
        Partie& partie = Partie::getInstance(); // init du singleton partie avec getInstance

        //Choix mdj
        partie.choixMDJ();
        partie.initParticipants();

        //Chargement des tuiles
        string choix2 ="";
        do {
            std::cout<<"Veillez choisir un mode de chargement des tuiles : \nTapez 1 : Mode normal\nTapez 2 : Mode aléatoire\n";
            std::cin>> choix2;
        }while (choix2 !="1" && choix2 !="2");
        if (choix2 == "1"){
            partie.ChargerTuiles();
        }
        else {
            partie.GenererTuilesAleatoires();
        }
        partie.melangePioche();

        

        std::cout<<"    Participant(s)\n";

        //Construire les objets participants en fonction du nbr de joueurs dans objet mode de jeu 
        //partie.initParticipants();

        //Mettre leur plateau associé, initié avec 1 case neutre à définir, 3 cases cailloux

        std::cout<<"    Partie\n";
        //Partie partie(mode);

        //Méthode pour choisir au hasard l'ordre des participants: les joueurs réels commencent toujours, donner l'ordre des participants actuels, changer de 1 l'ordre des participants(le premier devient le dernier)
        //partie.computeTurnOrder();
        //créer méthode tour suivant
        //méthode initialisation : 
            //donne un ordre de passage
            //donne le nbr de cailloux en fonction des ordres de passages

        std::cout<<"    Plateau\n";

            //priorité à créer le stockage du plateau

            //Méthode pour voir si une tuile(3 hexagones) peut être placée sur plateau à certaines coordonnées, passées en paramètre
            //Méthode pour placer, qui utilise la méthode de vérif au préalable
            //méthode calcul de points

        std::cout<<"    Tuile\n";
            //Méthode de rotation de tuile
            //méthode de chargement des 61 tuiles
            //méthode de tirage de x tuiles parmis les 61, sans remise (faire par exemple un tableau ou on retire des éléments petits à petit)
            //méthode de chargement des 61 pièces stockées

            //non prioritaire : création aléatoire des 61 tuiles (proba à déterminer)

        system("pause"); // pour débug sur VS code
    }

    //Débug, pour test vos trucs
    else if (choix == "2"){
        std::cout << "\n=== MODE DEBUG - TEST PLATEAU ===\n\n";

    // Créer un plateau
    Plateau plateau;
    std::cout << "Plateau créé (vide)\n";
    plateau.afficherPlateau();

    // Créer une tuile de départ
    std::cout << "\nCréation d'une tuile de départ...\n";
    Tuile tuileDepart(0, true);
    std::cout << "Tuile de départ créée\n";

    // Placer la tuile au centre du plateau
    HexagoneCoord origin{0, 0, 0};
    plateau.placerTuile(tuileDepart, origin);
    std::cout << "Tuile placée à l'origine (0,0,0)\n";

    // Afficher le plateau
    std::cout << "\nAffichage du plateau après placement:\n";
    plateau.afficherPlateau();

    // Afficher avec le dessin complet
    std::cout << "\nDessin du plateau complet (avec hauteurs):\n";
    plateau.dessinerPlateau(3);

    // Test avec une deuxième tuile (créée manuellement)
    std::cout << "\nCréation d'une deuxième tuile...\n";
    std::vector<Hexagone> disposition2{
        Hexagone(0, 0, 1, TypeHexagone::Jardin),
        Hexagone(1, 0, 1, TypeHexagone::Jardin),
        Hexagone(1, -1, 0, TypeHexagone::Jardin)
    };
    Tuile tuile2(1, disposition2, false);
    tuile2.setHauteur(1);  // hauteur 1

    // Placer la deuxième tuile à côté
    HexagoneCoord origin2{2, 0, -2};
    plateau.placerTuile(tuile2, origin2);
    std::cout << "Tuile 2 placée à (2,0,-2) avec hauteur 1\n";

    // Affichage final
    std::cout << "\nAffichage final du plateau:\n";
    plateau.afficherPlateau();
    plateau.dessinerPlateau(4);
      
    std::cout << "Appuyez sur Entrée pour quitter..." << std::endl;
    std::cin.get(); // Attend l'appui sur la touche Entrée
        
    }
    else {
        return 0;
    }
    return 0;

    
    
    
}
