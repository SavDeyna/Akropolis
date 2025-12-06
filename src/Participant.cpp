#include <vector>
#include <set>
#include <algorithm>
#include <iostream>

#include "Participant.h"
#include "Plateau.h"
#include "Tuile.h"
#include "Hexagone.h"

using namespace std;

bool Participant::placerTuile( Tuile& tuile) {
    Plateau& p = plateau;

    // On vérifie si au moins un hexagone voisin est occupé
    bool voisinTrouve = false;

    for (const auto& h : tuile.getDisposition()) {

        // Convertit un Hexagone → HexagoneCoord
        HexagoneCoord c { h.getQ(), h.getR(), h.getS() };

        // Récupère les voisins du plateau (coord absolues)
        auto voisins = p.getVoisins(c);

        for (const auto& v : voisins) {
            if (p.estOccupe(v)) {
                voisinTrouve = true;
                break;
            }
        }
        if (voisinTrouve) break;
    }

    if (!voisinTrouve && !p.estVide()) {
        std::cout << "Aucun voisin, placement refusé.\n";
        return false;
    }

    // Choix de l'origine de la tuile (ex: placer la première au centre)
    HexagoneCoord origin { 0,0,0 };

    p.placerTuile(tuile, origin);
    return true;
}

