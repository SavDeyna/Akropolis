#ifndef HEXAGONE_H
#define HEXAGONE_H

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

class Hexagone {
    int q;
    int r;
    TypeHexagone typehexagone ;
    unsigned int etoiles ;

    public :
        Hexagone(int q, int r, unsigned int e): q(q), r(r),  etoiles(e) {}
        bool operator<(const Hexagone& other) const;
        int getQ() const{return q;};
        int getR() const{return r;};
        int getS() const{return -getQ()-getR();};
};

#endif