#ifndef PARTIE_H
#define PARTIE_H
#include <string>
#include <vector>
#include "Participant.h"
#include "Tuile.h"


class ModeDeJeu {
    public :
        const string& getnomMDJ() const {return nom;}
        unsigned int getNbJoueur() const {return nbJoueur;}
        unsigned int getNbrIA() const {return nbIA;}
        const string& getDescription() const {return description;}
        ModeDeJeu(const string& n, unsigned int j, unsigned int i, const string& d) : nom(n), nbJoueur(j),nbIA(i), description(d) {};

        //Constructeur nécessaire pour initié partie sans aucun mdj
        ModeDeJeu() : nom("Indéfini"), nbJoueur(0), nbIA(0), description("") {}
        ~ModeDeJeu() = default;

    private:
        string nom;
        unsigned int nbJoueur ;
        unsigned int nbIA ;
        string description;
};

class Partie{
    public:

        static Partie& getInstance() {
            // Initialisation statique locale (la plus simple et thread-safe depuis C++11)
            static Partie instance;
            return instance;
        }

        ~Partie() = default;

        //init de la partie bout par bout
        void choixMDJ(); // l'utilisateur choisit un mode de jeu
        void initParticipants(); // initialiser les participants de la partie selon le mdj

        //getter
        const ModeDeJeu& getMDJ() const { return mdj; }
        int getTour() const { return tour; }
        std::vector<Participant>& getParticipants() { return participants; }
        std::vector<Tuile>& getPioche() { return pioche; }
        Participant getParticipant(std::size_t i) const ;
        std::size_t getNbParticipants() const noexcept { return nbParticipants; }
        void incTour() { tour += 1; }
        
       
        //Partie tuiles
        void melangePioche();
        void ChargerTuiles();
    

    private:
        Partie() {}; // init private => singleton
        Partie(const Partie&) = delete; // copie interdite
        Partie& operator=(const Partie&) = delete; // affectation interdite

        //Partie mode de jeu
        ModeDeJeu mdj;
        void SetMdj(const ModeDeJeu& m){mdj=m;} // choisir le mode de jeu

        //Partie participants
        std::vector<Participant> participants;
        unsigned int nbParticipants{ 0 };

        //déroulement partie
        int tour{ 0 };
        
        

        //Partie tuiles
        std::vector<Tuile> pioche ;
        std::vector<Tuile> jeu;
        std::vector<Tuile> defausse ;

};
#endif
