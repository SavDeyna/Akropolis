#include "Partie.h"

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
        string choix2 ="";
        do {
            std::cout<<"Veillez choisir un mode de chargement des tuiles : \nTapez 1 : Mode normal\nTapez 2 : Mode aléatoire\n";
            std::cin>> choix2;
        }while (choix2 !="1" && choix2 !="2");
        if (choix2 == "1"){
            partie.ChargerTuiles();
            partie.melangePioche();
        }
        else {
            partie.GenererTuilesAleatoires();
        }
        

        //Chargement des participations :
        //Donne des tours, les cailloux de départs, l'ordre de passage
        for (unsigned int i = 0 ; i<partie.getNbJoueurs();i++){
            std::cout<<"Pseudo du joueur n°"<<i+1<<" :\n";
            string p;
            std::cin >> p ;
            Joueur j(p) ;
            Participation parti(&j,i);
            parti.addPierres(i+1);
            partie.addParticipation(parti);
        }
        

        std::cout<<"    Partie\n";
        //Partie partie(mode);

        
        //partie.computeTurnOrder();
        //créer méthode tour suivant
        //méthode initialisation : 
            //donne un ordre de passage
            //donne le nbr de cailloux en fonction des ordres de passages

        std::cout<<"    Plateau\n";

            //priorité à créer le stockage du plateau

            //Méthode pour voir si une tuile(3 hexagones) peut être placée sur plateau à certaines coordonnées, passées en paramètre
            //Méthode pour placer, qui utilise la méthode de vérif au préalable

        std::cout<<"    Tuile\n";
            //Méthode de rotation de tuile


        system("pause"); // pour débug sur VS code
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
