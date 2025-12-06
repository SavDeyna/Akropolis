#pragma once
#include "Participation.h"
#include <set>
#include <string>

// Les 5 variantes du jeu
enum class Variante {
    Habitations,
    Marches,
    Casernes,
    Temples,
    Jardins
};

class ModeDeJeu {
    public :
        const string& getnomMDJ() const {return nom;}
        unsigned int getNbrJoueur() const {return nbrJoueur;}
        unsigned int getNbrIA() const {return nbrIA;}
        const string& getDescription() const {return description;}
        ModeDeJeu(const string& n, unsigned int j, unsigned int i, const string& d) : nom(n), nbrJoueur(j),nbrIA(i), description(d) {};

        //Constructeur nécessaire pour initié partie sans aucun mdj
        ModeDeJeu() : nom("Indéfini"), nbrJoueur(0), nbrIA(0), description("") {}
        ~ModeDeJeu() = default;

        // Variantes
        bool varianteActive(Variante v) const {
            return variantes.count(v) > 0;
        }

        void activerVariante(Variante v) {
            variantes.insert(v);
        }

        const std::set<Variante>& getVariantes() const {
            return variantes;
        }

    private:
        string nom;
        unsigned int nbrJoueur ;
        unsigned int nbrIA ;
        string description;

        // Variantes
        std::set<Variante> variantes;
};


class Partie{
    public:
        //Partie mode de Jeu
        void choixMDJ(); // l'utilisateur choisit un mode de jeu
        ModeDeJeu getMDJ() const {return mdj;}

        //Partie initialisation objet
        
        Partie() {} ;
        //Constructeur pour charger une partie depuis une sauvegarde.
        Partie(unsigned int tour, vector<Participation> participants , ModeDeJeu mdj, vector<Tuile> pioche);
        ~Partie() = default;



        //getter

        int getTour() const { return tour; }
        Participation getParticipant(std::size_t i) const ;
        std::size_t getNbJoueurs() const noexcept { return nbParticipants; }
        void incTour() { tour += 1; }
        
        friend class Sauvegarde;
       
        //Partie tuiles
        void melangePioche();
        void ChargerTuiles();
        void GenererTuilesAleatoires(unsigned int n = 61);

        //Methodes pour la participation
        void initialiserParticipations();
        void calculerScoresFinDePartie();
        Participation& getGagnant();

    private:
        //Partie mode de Jeu
        ModeDeJeu mdj;
        void SetMdj(const ModeDeJeu& m){mdj=m;}

        //Partie participants
        vector<Participation> participants;
        unsigned int nbParticipants{ 0 };

        //déroulement partie
        unsigned int tour{ 0 };
        
        

        //Partie tuiles
        vector<Tuile> pioche ;
        vector<Tuile> jeu;

};
