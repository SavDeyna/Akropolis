#pragma once
#include "Participation.h"
#include "IllustreArchitecte.h"
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
        unsigned int getNbJoueur() const {return nbJoueur;}
        unsigned int getNbIA() const {return nbIA;}
        const string& getDescription() const {return description;}
        ModeDeJeu(const string& nom, unsigned int nbJoueur, unsigned int nbIA, const string& description) : nom(nom), nbJoueur(nbJoueur),nbIA(nbIA), description(description) {};

        //Constructeur nécessaire pour initié partie sans aucun mdj
        ModeDeJeu() : nom("Indéfini"), nbJoueur(0), nbIA(0), description("") {}
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

        // IllustreArchitecte
        bool estSoloArchitecte() const { return soloArchitecte; }
        void activerSoloArchitecte() { soloArchitecte = true; }
        void desactiverSoloArchitecte() { soloArchitecte = false; }
        //Pour la sauvegarde
        std::string ToStringVariente() const;

    private:
        string nom;
        unsigned int nbJoueur ;
        unsigned int nbIA ;
        string description;

        // Variantes
        std::set<Variante> variantes;

        // IllustreArchitecte
        bool soloArchitecte{false};
};


class Partie{
    public:
        friend class SauvegardeManager;
        static Partie& getInstance() {
            // Initialisation statique locale 
            static Partie instance;
            return instance;
        }
        //Partie mode de Jeu
        void choixMDJ(); // l'utilisateur choisit un mode de jeu

        ModeDeJeu getMDJ() const {return mdj;}
        ModeDeJeu& getMDJ() {return mdj;}
        
        void chargerDepuisSauvegarde(unsigned int t,std::vector<Participation>&& p,const ModeDeJeu& m,std::vector<Tuile>&& pi, std::vector<unique_ptr<Participant>>&& j) ;


        //Setter
        void SetNbParticipants();
        ~Partie() = default;


        //getter
        int getTour() const { return tour; }
        std::vector<Participation>& getParticipants() { return participants; }
        std::vector<Tuile>& getPioche() { return pioche; }
        //Accesseur pour la sauvegarde
        std::vector<Tuile> getPioche() const{ return pioche; }
        unsigned int getNbParticipants() const noexcept { return nbParticipants; }
        vector<Tuile>& getJeu(){return jeu;};
        
        
        friend class Sauvegarde;
       
        //Partie tuiles
        void melangePioche();
        void ChargerTuiles();
        void GenererTuilesAleatoires(unsigned int n = 61);

        //Methodes pour la participation
        void addParticipation(string pseudo);
        

        //Retourne le pseudo du gagnant
        string getGagnant();


        //Prépare le jeu grâce à la pioche
        void debutTour();
        //Met à jour le tour, l'ordre de passage, le score des participations, vide le jeu.
        void finTour();

        // IllustreArchitecte
    
        bool estModeSoloArchitecte() const {
            return mdj.estSoloArchitecte();
        }

        void donnerPierresArchitecte(unsigned int nb);
        void jouerTourArchitecte();
        
    private:
        IllustreArchitecte architecte;

        void calculerScoresFinDePartie();
        
        //Constructeur pour charger une partie depuis une sauvegarde.
        Partie(unsigned int tour, vector<Participation>&& participants , ModeDeJeu mdj, vector<Tuile>&& pioche);

        Partie() {}; // init private => singleton
        Partie(const Partie&) = delete; // copie interdite
        Partie& operator=(const Partie&) = delete; // affectation interdite

        //Partie mode de jeu
        ModeDeJeu mdj;
        void SetMdj(const ModeDeJeu& m){mdj=m;} // choisir le mode de jeu

        //Partie participants
        std::vector<Participation> participants;
        unsigned int nbParticipants{ 0 };

        //déroulement partie
        unsigned int tour{ 1 };
        
        

        //Partie tuiles
        vector<Tuile> pioche ;
        vector<Tuile> jeu;

        //On stocke également les joueurs pour éviter qu'il soit supprimé après leur création dans le main
        std::vector<std::unique_ptr<Participant>> joueurs;

};
