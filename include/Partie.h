#pragma once

#include "Participant.h"

class Partie{
    public:
        ~Partie(){};

        //Fonction qui charge les tuiles du fichier json
        void ChargerTuiles();

        //Méthode pour mélanger les tuiles d'une partie Tom s'en occupe
        void melangePioche();

        //Méthode pour choisir l'ordre de passage 

        //Méthode pour mettre à jour les ordres de passage
    


    private:
        vector<Tuile> pioche ;
        vector<Tuile> jeu;
        vector<Tuile> defausse ;

};