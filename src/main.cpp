#include "Partie.h"
#include "Participant.h"
#include "Hexagone.h"
#include "Tuile.h"

#include <iostream>

#ifdef _WIN32
#define NOMINMAX
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
        Partie partie;
        


        //Choix mdj
        partie.choixMDJ();


        //Chargement des tuiles
        partie.ChargerTuiles();
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
    }

    //Débug, pour test vos trucs
    else if (choix == "2"){
        //=== test d'initialisation d'un Plateau
        Plateau p1;
        p1.afficherPlateau();
        //===
        
    }
    else {
        return 0;
    }
    return 0;

    
    
}
