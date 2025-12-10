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

    std::vector<HexagoneCoord> coords;
    coords.reserve(3); // pour éviter une surallocation de mémoire

    for (const auto& h : depart.getDisposition()) {
        HexagoneCoord pos {
            origin.q + h.getQ(),
            origin.r + h.getR(),
            origin.s + h.getS()
        };

        HexState st;
        st.type = h.getTypeHexagone();
        st.hauteur = 1;
        st.id_tuile = depart.getId();
        st.place = h.isPlace();

        grille[pos] = st;
    }
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

bool Plateau::placerTuile(Tuile& t, const HexagoneCoord& origin, unsigned int& nbPierres) {
    std::vector<HexagoneCoord> coords;
    coords.reserve(3); // pour éviter une surallocation de mémoire

    for (const auto& h: t.getDisposition()) {
        // h.getQ() et getR() sont relatifs → on les additionne à origin
        HexagoneCoord pos {
            origin.q + h.getQ(),
            origin.r + h.getR(),
            origin.s + h.getS()
        };
        coords.push_back(pos);
    }

    // vérifions s'il y a déjà des hexa sur le plateau aux coords indiquées
    const HexState* hex1 = getHex(coords[0]);
    const HexState* hex2 = getHex(coords[1]);
    const HexState* hex3 = getHex(coords[2]);

    bool allEmpty = (!hex1 && !hex2 && !hex3);
    bool allPresent = (hex1 && hex2 && hex3);

    if (!allEmpty && !allPresent) {
        cout << "1" << endl;
        return false;
    }
    int h = 1;

    if (allPresent){
        if (!(hex1->hauteur == hex2->hauteur && hex2->hauteur == hex3->hauteur)) {
            cout << "2" << endl;
            return false;  // les hexa n'ont pas la même hauteur, la tuile ne peut pas être placée
        } 
        if (hex1->id_tuile == hex2->id_tuile && hex2->id_tuile == hex3->id_tuile) {
            cout << "3" << endl;
            return false; // la nouvelle tuile au niveau supérieur n'est pas à cheval sur au moins 2 tuiles différentes
        } 
        h = hex1->hauteur + 1;
        t.setHauteur(h);

        for (const auto* hex : {hex1, hex2, hex3}) {
            if (hex->type == TypeHexagone::Carriere)
                nbPierres++;
        }
    } else if (allEmpty) {
        if (h == 1){
            int i{0};
            vector<HexagoneCoord> voisins;
            while (i < 3) {
                voisins = getVoisins(coords[i]);
                if (!voisins.empty())
                    break; 
                i++;
            } 
            if (i == 3) {
                cout << "4" << endl;
                return false; // la nouvelle tuile n'est pas adjacente à une autre tuile de la Cité
            } 
        }
    } 
    
    while (true) {
        cout << "Rotation de la tuile ? (1 = gauche, 2 = droite, 0 = terminer) : ";
        int choix;
        cin >> choix;

        if (choix == 1) {
            t.tournerGauche();
            cout << "Coordonnées après rotation gauche :\n";
        } else if (choix == 2) {
            t.tournerDroite();
            cout << "Coordonnées après rotation droite :\n";
        } else if (choix == 0) {
            break;
        }
        if (choix == 1 || choix == 2) {
            const auto& disp = t.getDisposition();
            for (size_t i = 0; i < disp.size(); ++i) {
                const Hexagone& h = disp[i];
                cout << "Hex #" << i << " : (" 
                    << h.getQ() << "," << h.getR() << "," << h.getS() << ")\n";
            }
        }
    }

    for (size_t i = 0; i < coords.size(); ++i) {
        const Hexagone& hex10 = t.getDisposition()[i];
        const HexagoneCoord& position = coords[i];
        HexState st;

        st.type = hex10.getTypeHexagone();
        st.hauteur = h;
        st.id_tuile = t.getId();
        st.place = hex10.isPlace();
        grille[position] = st;
    }
    //=========
    return true;
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