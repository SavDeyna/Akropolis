#include <iostream>
#include <string>
#include <vector>
#include <set>
#include "Plateau.h"
#include "Tuile.h"

#include <algorithm>


using namespace std;

// si au moins une coordonnée d'une tuile correspond à la coordonnée du paramètre alors on renvoie la tuile trouvée
const Tuile* Plateau::getTuile(const Hexagone& coord) const {
    for (const auto& t : matrice_hexa) {
        for (const auto& c : t.getDisposition()) {
            if (c.getQ() == coord.getQ() && c.getR() == coord.getR() && c.getS() == coord.getS()) {
                return &t;
            }
        }
    }
    return nullptr;
}

vector<Hexagone> Plateau::getVoisins(const Hexagone& c) const {
    // Déplacements pour les 6 voisins dans un système axial (q,r)
    static const int dq[6] = { +1, +1,  0, -1, -1,  0 };
    static const int dr[6] = {  0, -1, -1,  0, +1, +1 };

    vector<Hexagone> voisins;
    voisins.reserve(6);

    for (int i = 0; i < 6; ++i) {
        int nq = c.getQ() + dq[i];
        int nr = c.getR() + dr[i];

        // Crée un hexagone qui permet de rechercher la tuile
        // Les valeurs type et étoiles sont factices ici
        Hexagone probe(nq, nr, 0u, TypeHexagone::Carriere);

        const Tuile* tuile = getTuile(probe); // renvoie nullptr si pas de tuile
        if (tuile != nullptr) {
            // Cherche l’hexagone exact dans la tuile
            const auto& disposition = tuile->getDisposition();
            auto it = std::find_if(disposition.begin(), disposition.end(),
                [nq, nr](const Hexagone& h) { return h.getQ() == nq && h.getR() == nr; });

            if (it != disposition.end()) {
                voisins.push_back(*it);
            }
        }
    }

    return voisins;
}

void Plateau::placerTuile(const Tuile& tuile) {
    matrice_hexa.push_back(tuile);
}

void Plateau::afficherPlateau() {
    std::cout << "=== État actuel du plateau ===" << std::endl;

    if (matrice_hexa.empty()) { 
        std::cout << "Aucune tuile placée." << std::endl;
        return;
    }

    for (const auto& tuile : matrice_hexa) {
        // On utilise directement la méthode d'affichage de la tuile
        tuile.afficherTuile();
    }
}

