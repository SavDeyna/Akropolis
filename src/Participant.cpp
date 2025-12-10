#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

#include "Participant.h"

using namespace std;

void IA::Jouer() const {

}

void Joueur::Jouer() const {

}
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

