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

//Bien faire les setter / getter 

int main(){
    
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8); // pour que Windows affiche l’UTF-8
    #endif

    std::cout<<"Lancement du jeu\n";
    
    std::cout<<"Création des objets :\n";
    Partie partie;
    partie.ChargerTuiles();
    partie.melangePioche();
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
    
    try {
        // --- Création du plateau ---
        Plateau plateau(1);

        // --- Création de quelques hexagones ---
        // (coordonnées cubiques q, r, s = -q-r)
        Hexagone h1(0, 0, 1, TypeHexagone::Caserne);
        Hexagone h2(1, -1, 1, TypeHexagone::Jardin);
        Hexagone h3(1, 0, 1, TypeHexagone::Temple);

        // --- Création d'une tuile (3 hexagones contigus) ---
        vector<Hexagone> dispo1 = { h1, h2, h3 };
        Tuile tuile1(1, dispo1);

        // --- Création d’une autre tuile pour tester le placement multiple ---
        Hexagone h4(2, -1, 1, TypeHexagone::Carriere);
        Hexagone h5(2, 0, 1, TypeHexagone::Quartier);
        Hexagone h6(3, -1, 1, TypeHexagone::Jardin);
        vector<Hexagone> dispo2 = { h4, h5, h6 };
        Tuile tuile2(2, dispo2);

        // --- Placement des tuiles sur le plateau ---
        plateau.placerTuile(tuile1, "Alice");
        plateau.placerTuile(tuile2, "Bob");

        // --- Affichage du plateau ---
        plateau.afficherPlateau();
    }
    catch (const exception& e) {
        cerr << "Erreur : " << e.what() << endl;
    }


    system("pause");
    return 0;
}
