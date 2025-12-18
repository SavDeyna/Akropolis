#pragma once
#include <iostream>
#include <string>
#include <map>

enum class TypeHexagone {
    Carriere,     // permet de gagner des pierres
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
    //Pour la sauvegarde
    std::string toString() const ;
    HexagoneCoord fromString(const std::string& str);
};

struct HexState {
    TypeHexagone type;
    int hauteur;
    int id_tuile;            // l'id de la tuile dont cet hexagone provient
    bool place;

    //Pour la sauvegarde
    std::string toString() const ;
    HexState fromString(const std::string& str);
};

// L'hexagone d'une tuile, coordonnées relatives, à ne pas confondre avec les coordonnées du plateau
class Hexagone {
    int q, r;
    TypeHexagone type;
    bool place;

public:
    Hexagone(int q, int r, TypeHexagone t, bool p = false)
        : q(q), r(r), type(t), place(p) {}

    int getQ() const { return q; }
    int getR() const { return r; }
    int getS() const { return -(q + r); }

    // Accesseurs
    TypeHexagone getTypeHexagone() const { return type; }
    bool isPlace() const { return place; }

    // opérateur nécessaire si on veut comparer/sort des Hexagones, à voir si utile par la suite
    bool operator<(const Hexagone& other) const;

    // Setters
    void setQ(int nq) { q = nq; }
    void setR(int nr) { r = nr; }
    void setS(int ns) { r = -q - ns; } // q + r + s = 0
    void setType(TypeHexagone t) { type = t; }
    void setPlace(bool p) { place = p; }
};
