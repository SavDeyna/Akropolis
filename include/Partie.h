#pragma once

#include <vector>
#include "Tuile.h"

class Partie {
public:
    ~Partie() = default;

    // Fonction qui charge les tuiles du fichier json
    void ChargerTuiles();

    // Méthode pour mélanger les tuiles d'une partie
    void melangePioche();

    // à compléter : ordre de passage...
private:
    std::vector<Tuile> pioche ;
    std::vector<Tuile> jeu;
    std::vector<Tuile> defausse ;
};
