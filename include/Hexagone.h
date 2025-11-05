#pragma once
#include <iostream>
#include <string>
#include <map>

using namespace std;

enum class TypeHexagone {
    Carriere,
    Caserne,
    Jardin,
    Temple,
    Quartier,
    Marche
};

class Coordonnees {
    int q;
    int r;
    int s;
    TypeHexagone typehexagone ;
    public :
        Coordonnees(int q, int r, int s): q(q), r(r), s(s) {
            if ((q + r + s) != 0)
                throw invalid_argument("q+r+s n'est pas égal à 0");
        }
        bool operator<(const Coordonnees& other) const;
};