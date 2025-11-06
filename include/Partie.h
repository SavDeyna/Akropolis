#ifndef PARTIE_H
#define PARTIE_H

#include "Participant.h"

class Partie{
    public:
        ~Partie(){};

        //Fonction qui charge les tuiles du fichier json Tom s'en occupe
        vector<Tuile> ChargerTuiles();

        //Méthode pour mélanger les tuiles d'une partie Tom s'en occupe
        void melangeTuile();

        //Méthode pour choisir l'ordre de passage 

        //Méthode pour mettre à jour les ordres de passage



    private:
        vector<Tuile> pioche ;
        vector<Tuile> defausse ;


};

#endif