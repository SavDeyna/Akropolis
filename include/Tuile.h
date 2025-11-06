#ifndef TUILE_H
#define TUILE_H

#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>

#include <Hexagone.h>
using namespace std;

class Tuile { 
private:
    string nom_joueur;
    int id_tuile;
    int hauteur; 
    vector<Hexagone> disposition; // le tableau qui contient 3 hexagones
    bool formeValide() const;
    // en coordonnées cubiques (pour un hexagone) la distance est calculée comme le nb de pas entre deux hexas : ( |distance_q| + |distance_r| + |distance_s| ) / 2
    int distance(const Hexagone& a, const Hexagone& b) const {
        return (abs(a.getQ() - b.getQ()) + abs(a.getR() - b.getR()) + abs(a.getS() - b.getS())) / 2;
    }
public:
    Tuile(int id, const vector<Hexagone>& d) : id_tuile(id), disposition(d), hauteur(0) {
        if (disposition.size() != 3)
            throw invalid_argument("Une tuile doit avoir 3 hexagones");
        if (!formeValide())
            throw invalid_argument("La forme de la tuile n’est pas valide");
    }
    const vector<Hexagone>& getDisposition() const { return disposition; }
    int getHauteur() const { return hauteur; }
    void setHauteur(int h) { hauteur = h; }
    string getNomJoueur() const { return nom_joueur; }
};

#endif