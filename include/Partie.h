#ifndef PARTIE_H
#define PARTIE_H
#include "Participant.h"



class ModeDeJeu {
    public :
        const string& getnomMDJ() const {return nom;}
        unsigned int getNbrJoueur() const {return nbrJoueur;}
        const string& getDescription() const {return Description;}
        
    private:
        string nom;
        unsigned int nbrJoueur ;
        string Description;
};

class Partie{
    public:
        //Partie mode de Jeu
        void chargerMDJ();

        //Partie initialisation objet
        
        Partie() {};
        ~Partie() = default;



        //getter
        ModeDeJeu getMDJ() { return mdj; }
        int getTour() const { return tour; }
        Participant getParticipant(std::size_t i) const ;
        std::size_t getNbJoueurs() const noexcept { return nbParticipants; }
        void incTour() { tour += 1; }
        
       
        //Partie tuiles
        void melangePioche();
        void ChargerTuiles();

    private:
        //Partie mode de Jeu
        ModeDeJeu mdj;
        vector<Participant> participants;
        unsigned int nbParticipants{ 0 };

        //déroulement partie
        int tour{ 0 };
        
        //Partie tuiles
        vector<Tuile> pioche ;
        vector<Tuile> jeu;
        vector<Tuile> defausse ;

};
#endif
