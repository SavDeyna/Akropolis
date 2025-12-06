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

    // dessine le plateau en ASCII art (version compacte en tuiles collées)
    void dessinerPlateau(const int radius) const;

    bool estVide() const { return grille.empty(); }
};