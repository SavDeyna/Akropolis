#ifndef PLATEAU_H
#define PLATEAU_H

#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Tuile.h"
using namespace std;

class Plateau {
private:
    int id; // inutile à mon avis car on a un seul plateau pour tous les joueurs
    vector<Tuile> matrice_hexa; // first(key) : Hexagone ; second(value) : Tuile
public:
    Plateau(int id) : id(id) {}
    const Tuile* getTuile(const Hexagone& coord) const; // renvoie un pointeur vers la tuile qui recouvre l'hexagone donné le cas échéant
    bool placerTuile(Tuile& tuile, const string& nom_joueur);
    vector<Hexagone> getVoisins(const Hexagone& c) const;
};

#endif