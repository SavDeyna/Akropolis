#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <map>

#include "Tuile.h"
using namespace std;

class Plateau {
private:
    std::vector<Tuile> matrice_hexa; // first(key) : Hexagone ; second(value) : Tuile
public:
    Plateau() {
        Tuile tuile_depart(0, true); // tuile de départ automatique
        matrice_hexa.push_back(tuile_depart);
    }

    // Méthode pour ajouter une tuile (ne fait aucune vérification)
    void placerTuile(const Tuile& tuile);

    const Tuile* getTuile(const Hexagone& coord) const; // renvoie un pointeur vers la tuile qui recouvre l'hexagone donné
    std::vector<Hexagone> getVoisins(const Hexagone& c) const;
    void afficherPlateau();
};
