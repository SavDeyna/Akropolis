#include "Plateau.h"
#include <iostream>

using namespace std;


Plateau::Plateau() {
    Tuile depart(0, true);
    HexagoneCoord origin{0, 0, 0};
    placerTuile(depart, origin);
}

const HexState* Plateau::getHex(const HexagoneCoord& c) const {
    auto it = grille.find(c);
    return (it == grille.end()) ? nullptr : &it->second;
}

bool Plateau::estOccupe(const HexagoneCoord& c) const {
    return grille.find(c) != grille.end();
}

vector<HexagoneCoord> Plateau::getVoisins(const HexagoneCoord& c) const {
    static const int d[6][3] = {
        {+1,-1,0}, {+1,0,-1}, {0,+1,-1},
        {-1,+1,0}, {-1,0,+1}, {0,-1,+1}
    };
    vector<HexagoneCoord> v;
    for (auto& x : d) {
        HexagoneCoord nc{c.q + x[0], c.r + x[1], c.s + x[2]};
        if (grille.find(nc) != grille.end())
            v.push_back(nc);
    }
    return v;
}

void Plateau::placerTuile(const Tuile& t, const HexagoneCoord& origin) {
    for (const auto& h : t.getDisposition()) {
        // h.getQ() et getR() sont relatifs → on les additionne à origin
        HexagoneCoord pos {
            origin.q + h.getQ(),
            origin.r + h.getR(),
            origin.s + h.getS()
        };
        HexState st;
        st.type = h.getTypeHexagone();
        st.hauteur = t.getHauteur();
        st.id_tuile = t.getId();
        grille[pos] = st;
    }
}

void Plateau::afficherPlateau() const {
    std::cout << "=== Grille du plateau ===\n";
    if (grille.empty()) {
        std::cout << "(vide)\n";
        return;
    }
    for (const auto& [coord, st] : grille) {
        std::cout << "Hex (" << coord.q << "," << coord.r << "," << coord.s << ")"
                  << " type=" << static_cast<int>(st.type)
                  << " hauteur=" << st.hauteur
                  << " id_tuile=\"" << st.id_tuile << "\"\n";
    }
}

    bool Plateau::peutPoserTuile(const Tuile& t, const HexagoneCoord& origin) const {
    const auto& hexs = t.getDisposition();

    for (const Hexagone& h : hexs) {

        // Conversion coordonnées relatives → absolues
        HexagoneCoord abs{
            origin.q + h.getQ(),
            origin.r + h.getR(),
            origin.s + h.getS()
        };

        // 1) Vérifier si la case est déjà occupée
        if (estOccupe(abs)) {
            return false; // un hexagone de la tuile overlap → interdit
        }

        // 2) (Optionnel) Vérifier contraintes de hauteur
        // Tu peux adapter selon les règles Akropolis
        // Exemple : ne pas dépasser une hauteur max, etc.
        // Ici on n'impose rien
    }

    // 3) Vérifier qu'au moins un hexagone touche une tuile existante si le plateau n'est pas vide
    if (!estVide()) {
        bool touche = false;

        for (const Hexagone& h : hexs) {
            HexagoneCoord abs{
                origin.q + h.getQ(),
                origin.r + h.getR(),
                origin.s + h.getS()
            };

            // on regarde les voisins existants
            for (const auto& v : getVoisins(abs)) {
                if (estOccupe(v)) {
                    touche = true;
                    break;
                }
            }
            if (touche) break;
        }

        if (!touche) return false;  // la tuile doit être connectée
    }

    return true; // tout est OK → la tuile peut être posée
}