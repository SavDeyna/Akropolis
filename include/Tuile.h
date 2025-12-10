#pragma once
#include <vector>
#include <string>
#include <cmath>
#include <stdexcept>
#include <iostream>
#include "Hexagone.h"

class Tuile {
private:
    bool is_depart{false};

    bool formeValide() const;
    int distance(const Hexagone& a, const Hexagone& b) const {
        return (std::abs(a.getQ() - b.getQ()) +
                std::abs(a.getR() - b.getR()) +
                std::abs(a.getS() - b.getS())) / 2;
    }

protected:
    std::vector<Hexagone> disposition;
    int hauteur{1}; // 1er niveau par défaut
    std::string nom_joueur;
    int id_tuile;

public:
    // Constructeur standard
    Tuile(int id, const std::vector<Hexagone>& d, bool tuileDepart = false)
        :  is_depart(tuileDepart),disposition(d), id_tuile(id)
    {
        if (disposition.size() < 3)
            throw std::invalid_argument("Une tuile doit avoir au moins 3 hexagones");

        if (!is_depart && disposition.size() == 3 && !formeValide())
            throw std::invalid_argument("La forme de la tuile n’est pas valide");
    }

    // Constructeur pratique pour la tuile de départ
    Tuile(int id, bool tuileDepart) : is_depart(tuileDepart), id_tuile(id)  {
        if (tuileDepart) {
            disposition = {
                Hexagone(0, 0, TypeHexagone::Habitation, true), // "true" car cet hexagone est une place
                Hexagone(1, 0, TypeHexagone::Carriere),
                Hexagone(-1, 1, TypeHexagone::Carriere),
                Hexagone(0, -1, TypeHexagone::Carriere)
            };
        } else {
            throw std::invalid_argument("Tuile(id, bool): mettre true pour la tuile de départ.");
        }
        hauteur = 0;
    }

    void afficherTuile() const;

    // Accesseurs
    const std::vector<Hexagone>& getDisposition() const { return disposition; }
    int getHauteur() const { return hauteur; }
    void setHauteur(int h) { hauteur = h; }
    std::string getNomJoueur() const { return nom_joueur; }
    bool estTuileDepart() const { return is_depart; }
    int getId() const { return id_tuile; }
    
    // Rotation
    void tournerDroite(); 
    void tournerGauche();
};

