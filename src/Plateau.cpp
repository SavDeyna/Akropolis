#include "Plateau.h"
#include <iostream>

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

bool Plateau::placerTuile(Tuile& t, const HexagoneCoord& origin) {
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
