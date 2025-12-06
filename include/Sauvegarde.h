#ifndef SAUVEGARDE_H
#define SAUVEGARDE_H
#include "Partie.h"

class Sauvegarde ;

struct SauvegardeInfo{
    unsigned int tour ;
    string nomSauvegarde ;
    string nomMDJ;
    unsigned int index ;
};

class SauvegardeManager{

        
    public :
        vector<SauvegardeInfo> getListeSauvegardes();
        Partie chargerSauvegarde(unsigned int id);
        void enregistrerSauvegarde(const Sauvegarde& s);
};

class Sauvegarde{
    public:
        Sauvegarde(Partie p,const string& nomsauvegarde);//Va permettre de créer une nouvelle sauvegarde

        ~Sauvegarde() = default ;
        friend class SauvegardeManager;

    private:

        string nom;
        ModeDeJeu mdj;
        vector<Participant> participants;
        unsigned int nbParticipants;
        unsigned int tour ;
        vector<Tuile> pioche ;
        //La sauvegarde n'a pas de jeu (il est vide avant un tour)

};
#endif
