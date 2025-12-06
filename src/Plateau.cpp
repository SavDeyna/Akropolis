#include "Plateau.h"
#include <iostream>
#include <climits>
#include <algorithm>
#include <string>
#include <sstream>
#include <cmath>

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

void Plateau::dessinerPlateau(const int radius) const{
    vector<stringstream> lignes(2);
    for(int r = -radius; r <= radius; ++r){
        //reset des lignes
        for(size_t i = 0; i < 2; ++i) {
            lignes[i].str(""); // Clear the contents
            lignes[i].clear(); // Clear the flags
        }
        for(size_t j = 0; j < abs(r); ++j){
            lignes[0] << "     ";
            lignes[1] << "     ";
        }
        lignes[0] << "  ";
        lignes[1] << "  ";
        for(int q = -radius; q <= radius; ++q){
            int s = -q - r;
            if (s >= -radius && s <= radius) {
                auto it = grille.find(HexagoneCoord{q, r, s});
                const HexState * hexS = (it == grille.end()) ? nullptr : &it->second;
                string typeString = " ";
                string etoilesString = " ";
                if (hexS != nullptr) { 
                    // Si l'hexagone existe, on peut accéder à ses membres en toute sécurité
                    switch(hexS->type){
                        case TypeHexagone::Carriere: typeString = "C"; break;
                        case TypeHexagone::Caserne: typeString = "S"; break;
                        case TypeHexagone::Jardin: typeString = "J"; break;
                        case TypeHexagone::Temple: typeString = "T"; break;
                        case TypeHexagone::Marche: typeString = "M"; break;
                        case TypeHexagone::Habitation: typeString = "H"; break;
                        default: typeString = "?"; break;
                    }
                    etoilesString = std::to_string(hexS->place);
                }
                if (r == -radius) {
                    lignes[0] << "";
                    lignes[1] << "___";
                    lignes[1] << "       ";
                } else if (r + q == -radius) {
                    if (r == 0) {
                        lignes[0] << "      /   ";
                        lignes[1] << "     /    ";
                    } else {
                        lignes[0] << "      /   ";
                        lignes[1] << "  ___/    ";
                    }
                } else if (r + s == -radius) {
                    if (r == 0) {
                        lignes[0] << "\\         ";
                        lignes[1] << " \\        ";
                    } else {
                        lignes[0] << "\\         ";
                        lignes[1] << " \\___     ";
                    }
                } else {
                    lignes[0] << "\\ "<< typeString <<etoilesString <<"  /   ";
                    lignes[1] << " \\___/    ";
                }
            }
        }
        // quand on a fini une rangée d'hexagones, on peut l'afficher
        for(const auto& line : lignes){
            cout << line.str() << "\n";
        }
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