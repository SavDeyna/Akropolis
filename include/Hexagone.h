#pragma once
#include <iostream>
#include <string>
#include <map>

enum class TypeHexagone {
    Carriere,     // permet de gagner des pierres
    Place,        // multiplicateur de points pour les quartiers
    Caserne,      // quartier rouge
    Jardin,       // quartier vert
    Temple,       // quartier violet
    Marche,       // quartier jaune
    Habitation    // quartier bleu
};

struct HexagoneCoord {
    int q, r, s;

    // Nécessaire pour std::map
    bool operator<(const HexagoneCoord& o) const {
        if (q != o.q) return q < o.q;
        if (r != o.r) return r < o.r;
        return s < o.s;
    }
};

struct HexState {
    TypeHexagone type;
    unsigned int etoiles;
    int hauteur;
    int id_tuile;            // l'id de la tuile dont cet hexagone provient
};

// L'hexagone d'une tuile, coordonnées relatives, à ne pas confondre avec les coordonnées du plateau
class Hexagone {
    int q, r;
    unsigned int etoiles;
    TypeHexagone type;

public:
    Hexagone(int q, int r, unsigned int e, TypeHexagone t)
        : q(q), r(r), etoiles(e), type(t) {}

    int getQ() const { return q; }
    int getR() const { return r; }
    int getS() const { return -(q + r); }

    unsigned int getEtoiles() const { return etoiles; }
    TypeHexagone getTypeHexagone() const { return type; }

    // opérateur nécessaire si on veut comparer/sort des Hexagones, à voir si utile par la suite
    bool operator<(const Hexagone& other) const;
};
