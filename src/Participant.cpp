#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

#include "Participant.h"
#include "Plateau.h"
#include "Tuile.h"
#include "Hexagone.h"

using namespace std;

bool Participant::placerTuile(Tuile& tuile, const HexagoneCoord& origin) {
    Plateau& p = plateau;

    bool placementReussi = p.placerTuile(tuile, origin, pierres);
    if (plateau.placerTuile(tuile, origin, pierres)) {
        setPierres(pierres);
    } else {
        std::cerr << "Erreur : impossible de placer la tuile à cet endroit.\n";
        return false;
    }

    return true;
}

