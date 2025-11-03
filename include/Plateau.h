#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

class Plateau {
private:
    int id; // inutile à mon avis car on a un seul plateau pour tous les joueurs
    vector<Tuile> matrice_hexa; // first(key) : Coordonnees ; second(value) : Tuile
public:
    Plateau(int id) : id(id) {}
    const Tuile* getTuile(const Coordonnees& coord) const; // renvoie un pointeur vers la tuile qui recouvre l'hexagone donné le cas échéant
    bool placerTuile(Tuile& tuile, const string& nom_joueur);
    vector<Coordonnees> getVoisins(const Coordonnees& c) const;
};