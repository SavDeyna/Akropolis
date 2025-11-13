#include "Partie.h"

#include <iostream>
#include <fstream>

#include <nlohmann/json.hpp>
using json = nlohmann::json;

#ifdef _WIN32
#define NOMINMAX
#define byte win_byte_override
#include <windows.h>
#endif


int main(){
    
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8); // pour que Windows affiche l’UTF-8
    #endif

    std::cout<<"Lancement du jeu\n";
    string choix ="";
    do {
        std::cout<<"Veillez choisir un mode de lancement : \nTapez 1 : Mode Console\nTapez 2 : Debug\nAutre : Sortie\n";
        std::cin>> choix;
    }while (choix !="1" && choix !="0" && choix !="2");
    if (choix == "1"){
        std::cout<<"Création des objets :\n";
        //Partie partie(nullptr);
        std::cout<<"Partie créée\n";
        //partie.ChargerTuiles();
        std::cout<<"Tuiles chargées\n";
        //partie.melangePioche();
        std::cout<<"    Mode de jeu\n";
        
        // Besoin d'une fonction qui va me créer des objets modedejeu pour chaque mode de jeu. Créer les méthodes associées nécesssaires
        //GameMode mode = ;

        //Boucle pour choisir le mode de jeu, détruire les autres mdj non utilisés

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
    }
    else if (choix == "2"){
        //script de test, généré par IA, utilisé pour débogger les méthodes

        Joueur alice("Alice");

        // Création de tuiles normales
        Hexagone h1(1, 0,-1, TypeHexagone::Caserne);
        Hexagone h2(2, -1, 0, TypeHexagone::Jardin);
        Hexagone h3(1, -1, 1, TypeHexagone::Temple);
        std::vector<Hexagone> dispo1 = {h1, h2, h3};
        Tuile tuile1(1, dispo1);

        std::cout << "Tuile 1 créée\n";

        Hexagone h4(2, -1, 1, TypeHexagone::Carriere);
        Hexagone h5(2, 0, 1, TypeHexagone::Marche);
        Hexagone h6(3, -1, 1, TypeHexagone::Habitation);
        std::vector<Hexagone> dispo2 = {h4, h5, h6};
        Tuile tuile2(2, dispo2);

        std::cout << "Tuile 2 créée\n";

        Hexagone h7(1,1,-2, TypeHexagone::Temple);
        Hexagone h8(2,0,-2, TypeHexagone::Jardin);
        Hexagone h9(2,1,-3, TypeHexagone::Caserne);
        std::vector<Hexagone> dispo3 = {h7, h8, h9};
        Tuile tuile3(3, dispo3);

        std::cout << "Tuile 3 créée\n";

        std::cout << "Tuiles créées\n";

        alice.getPlateau().afficherPlateau();

        if (alice.placerTuile(tuile1))
            std::cout << "Tuile 1 placée avec succès.\n";
        else
            std::cout << "Placement de la tuile 1 impossible.\n";

        if (alice.placerTuile(tuile2))
            std::cout << "Tuile 2 placée avec succès.\n";
        else
            std::cout << "Placement de la tuile 2 impossible.\n";
        
        if (alice.placerTuile(tuile3))
            std::cout << "Tuile 3 placée avec succès.\n";
        else
            std::cout << "Placement de la tuile 3 impossible.\n";

        std::cout << "\nPlateau du joueur " << alice.getPseudo() << " :" << std::endl;
        alice.getPlateau().afficherPlateau();

        system("pause");
    }

    

    
    return 0;
}
