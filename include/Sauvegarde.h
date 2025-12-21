#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H
#pragma once
#include "Partie.h"
#include "Participation.h"
#include "Tuile.h"

class Sauvegarde ;

struct SauvegardeInfo{
    unsigned int tour ;
    string nomSauvegarde ;
    string nomMDJ;
    unsigned int index ;
};

class SauvegardeManager{

        
    public :
        SauvegardeManager(){};
        vector<SauvegardeInfo> getListeSauvegardes();
        Partie& chargerSauvegarde(unsigned int id);
        void enregistrerSauvegarde(const Sauvegarde& s);
        void supprimerSauvegarde(unsigned int id);
};

class Sauvegarde{
    public:
        Sauvegarde(const Partie& p,const string& nomsauvegarde);

        ~Sauvegarde() = default ;
        friend class SauvegardeManager;

    private:
        IllustreArchitecte architecte;
        string nom;
        ModeDeJeu mdj;
        vector<Participation> participants;
        unsigned int nbParticipants;
        unsigned int tour ;
        vector<Tuile> pioche ;
        //La sauvegarde n'a pas de jeu (il est vide avant un tour)

};
#endif
