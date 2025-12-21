#include "Partie.h"
#include "Sauvegarde.h"
#include <iostream>
#include <sstream>

#ifdef _WIN32
#ifndef NOMINMAX 
#define NOMINMAX
#endif
#define byte win_byte_override
#include <windows.h>
#endif

#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[]) {

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif


    std::cout<<"Lancement du jeu\n";
    string choix ="";
    do {
        std::cout<<"Veillez choisir un mode de lancement : \nTapez 1 : Mode Console\nTapez 2 : Mode Graphique\nTapez 0 : Sortie\n";
        std::cin>> choix;
    }while (choix !="1" && choix !="0" && choix !="2");


    //Mode console
    if (choix == "1"){
        std::cout<<"\nMode console\n\n";
        SauvegardeManager ManagerS;

        std::string choixn;
        
        std::cout<<"Voulez vous supprimer une sauvegarde?\nTapez 1 pour accepter\nTapez Autre chose pour continuer : ";
        std::cin>>choixn;

        unsigned int j = 0;
        vector<SauvegardeInfo> sauvegardes = ManagerS.getListeSauvegardes();

        if (choixn=="1"){
            
            std::string choix6 ="1";
            if (sauvegardes.size()== 0){
                std::cout<<"Aucune sauvegarde existante !\n\n";
            }

            while ((choix6=="1" || choix6=="2")&& sauvegardes.size()!=0){
                //On met sauvegarde ici pour le recharger à chaque fois
                std::cout<<"\nNombre total de sauvegarde : " << sauvegardes.size() <<"\n";
                std::cout<<"Celle-ci ?\nTapez 1 pour supprimer\nTapez 2 pour refuser\nTapez Autre pour arrêter\n\n";
                std::cout<<"Nom Sauvegarde : "<<sauvegardes[j].nomSauvegarde<<"\n";
                std::cout<<"Mode de jeu : "<<sauvegardes[j].nomMDJ<<"\n";
                std::cout<<"Tour : "<<sauvegardes[j].tour<<"\n";

                std::cin>>choix6;
                if (choix6 == "1"){
                    ManagerS.supprimerSauvegarde(j);
                    sauvegardes.erase(sauvegardes.begin() + j);
                    std::cout<<"Sauvegarde supprimée\n\n";
                }
                else{
                    //On incrémente pas lorsqu'on supprime
                    j= (j+1)%(sauvegardes.size());
                }
            }
        }

        std::string choix4;
        std::cout<<"\nChoisir le mode de chargement :\nTapez 1 pour charger une sauvegarde.\nTapez autre pour créer une nouvelle partie.\n";
        std::cin>>choix4;
        unsigned int i = 0;
        if (choix4 == "1" && !sauvegardes.empty()){
            std::string choix5 ="";
            
            while (choix5!="1"){
                std::cout<<"\nTapez 1 pour accepter\nTapez autre pour refuser\n\nCelle-ci ?\n";
                std::cout<<"Nom Sauvegarde : "<<sauvegardes[i].nomSauvegarde<<"\n";
                std::cout<<"Mode de jeu : "<<sauvegardes[i].nomMDJ<<"\n";
                std::cout<<"Tour : "<<sauvegardes[i].tour<<"\n";
                std::cin>>choix5;
                if (choix5 != "1"){
                    i= (i+1)%(sauvegardes.size());
                }
            }
        }
        if (sauvegardes.empty()) std::cout<<"Aucune sauvegarde existante, création d'une nouvelle partie";

        Partie& partie = (choix4 == "1" && !sauvegardes.empty()) ? ManagerS.chargerSauvegarde(i) : Partie::getInstance();

        if (choix4 !="1" || sauvegardes.empty()){

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

            std::cout << "Tuiles chargees et melangees !\n\n";
            

            

            //Chargement des participations :
            //Donne des tours, les cailloux de départs, l'ordre de passage
            
            for (unsigned int i = 0; i < partie.getNbParticipants(); i++) {
                stringstream f;
                f << "Joueur " << i+1;
                partie.addParticipation(f.str());
                std::cout << "Joueur " << i+1 << " initié : " << partie.getParticipants().back().getParticipant().getPseudo() << "\n";
            }
            

            //Ajout des variantes
            std::cout<<"\nChoississez les variantes à ajouter :\n\n";
            std::cout<<"Tapez 1 pour l'ajouter\nTapez autre pour ne pas ajouter\n\n";
            std::string choix8;
            std::map<Variante,std::string> Variantes = {{Variante::Habitations, "Habitations"},{Variante::Marches,"Marches"},{Variante::Casernes,"Casernes"},{Variante::Temples,"Temples"},{Variante::Jardins,"Jardins"}};
            for (const auto& [Key,Values] : Variantes){
                std::cout<<"Voulez vous ajouter cette variante : "<<Values<<"\n";
                
                std::cin>>choix8;
                if (choix8 == "1") partie.getMDJ().activerVariante(Key);
            }
        }
        std::cout<<"Test : "<<partie.getMDJ().getVariantes().size();


        //Déroulement des tours
        for (unsigned int i = 0; 11 ; i++){
            partie.debutTour();
            std::cout<< "\nDébut du tour "<<i+1<<"\n\n";


            for (unsigned int j = 1 ; j<=partie.getNbParticipants() ; j++){

                //On commence par trouver à qui c'est le tour
                unsigned int indice=0;
                bool trouve = false;
                while (indice <partie.getNbParticipants()){
                    if (partie.getParticipants()[indice].getOrdrePassage() == j) {
                    trouve = true;
                    break;
                    }
                    else indice ++;
                }
                if (!trouve){
                    std::cout<<"Pas de joueur avec l'indice "<<j<<".\n";
                    continue;
                }
                partie.getParticipants()[indice].Jouer(partie.getJeu());
            }
            partie.finTour();
            string choix3;
            std::cout<<"Voulez-vous sauvegarder?\nTapez 1 pour sauvegarder\nTapez Autre chose pour ne pas sauvegarder\n";
            std::cin>>choix3;
            if (choix3 == "1"){
                std::cout<<"Nom de la sauvegarde :\n";
                string nom ;
                std::cin>>nom;
                ManagerS.enregistrerSauvegarde(Sauvegarde(partie,nom));
            }


        }
        //On veut un gagnant :
        std::cout << "Fin de partie !\nLe gagnant est : "<< partie.getGagnant();

    }

    //Mode graphique
    else if (choix == "2"){
       QApplication a(argc, argv);

        // C'est ICI que l'on charge le style global si besoin
        MainWindow w;
        w.show();

        return a.exec(); // La boucle Qt prend le relais, std::cin ne fonctionnera plus
            
    }
    return 0; 
}
