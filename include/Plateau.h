#pragma once
#include <map>
#include <iostream>
#include <vector>

#include "Hexagone.h"
#include "Tuile.h"

class Plateau {
private:
    std::map<HexagoneCoord, HexState> grille;

public:
    friend class Participation;
    Plateau() = default;

    // accès au hex
    const HexState* getHex(const HexagoneCoord& c) const;
    bool estOccupe(const HexagoneCoord& c) const;

    // voisins existants (sur la grille)
    std::vector<HexagoneCoord> getVoisins(const HexagoneCoord& c) const;

    // placer une tuile avec origine (coord absolue du centre ou repère)
    void placerTuile(const Tuile& t, const HexagoneCoord& origin);

    // affiche la grille
    void afficherPlateau() const;

    bool estVide() const { return grille.empty(); }

    //Verifier que l'on peut poser une tuile de 3 hexagones
    bool peutPoserTuile(const Tuile& t, const HexagoneCoord& origin) const;
};