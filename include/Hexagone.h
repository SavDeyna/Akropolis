#pragma once
#include <iostream>
#include <string>
#include <map>

using namespace std;

enum class TypeHexagone {
    Carriere,     // permet de gagner des pierres
    Place,        // multiplicateur de points pour les quartiers
    Caserne,      // quartier rouge
    Jardin,       // quartier vert
    Temple,       // quartier violet
    Marche,       // quartier jaune
    Habitation    // quartier bleu
};

class Hexagone {
    int q;
    int r;
    unsigned int etoiles ;
    TypeHexagone typehexagone ;
    

    public :
        Hexagone(int q, int r, unsigned int e, TypeHexagone t): q(q), r(r),  etoiles(e) , typehexagone(t) {}
        bool operator<(const Hexagone& other) const;
        int getQ() const{return q;};
        int getR() const{return r;};
        int getS() const{return -(q+r);};
        TypeHexagone getTypeHexagone() const{return typehexagone;};
        unsigned int getEtoiles() const{return etoiles;};
};