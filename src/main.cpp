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
        partie.melangePioche();

        std::cout << "Tuiles chargees et melangees !\n";
        

        

        //Chargement des participations :
        //Donne des tours, les cailloux de départs, l'ordre de passage
        
        for (unsigned int i = 0 ; i<partie.getNbParticipants();i++){
            std::cout<<"Pseudo du joueur n°"<<i+1<<" :\n";
            string p;
            Joueur j = Joueur(p) ;
            Participation parti(&j,i);
            partie.addParticipation(parti);
            std::cin.ignore();
        }

        //Déroulement des tours
        for (unsigned int i = 0; i<11 ; i++){
            partie.debutTour();


            for (unsigned int j = 1 ; j<=partie.getNbParticipants() ; j++){

                //On commence par trouver à qui c'est le tour
                unsigned int indice=0;
                bool trouve = false;
                while (indice <partie.getNbParticipants()){
                    if (partie.getParticipants()[indice].getOrdrePassage() == j) {
                    trouve = true;
                    }
                    else indice ++;
                }
                if (!trouve){
                    std::cout<<"Pas de joueur avec l'indice "<<j;
                    continue;
                }
                partie.getParticipants()[indice].Jouer(partie.getJeu());
            }
            partie.finTour();
        }
            



    }

    //Débug, pour test vos trucs
    else if (choix == "2"){
       
        
    }
    else {
        return 0;
    }
    return 0;

    
    
    
}
