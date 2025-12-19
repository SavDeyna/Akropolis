#pragma once
#include "Hexagone.h"
#include "Tuile.h"
#include "Plateau.h"
#include <iostream>
#include <map>

enum class Difficulte { 
    Facile, 
    Moyen, 
    Difficile 
};

struct InfosQuartier {
    unsigned int quartiers = 0;
    unsigned int places = 0;
};

class IllustreArchitecte {
private:
    unsigned int pierres{2};
    std::map<TypeHexagone, InfosQuartier> infos;

public:
    IllustreArchitecte();
// pierres
    unsigned int getPierres() const { 
        return pierres; 
    }
    void addPierres(unsigned int p) { 
        pierres += p; 
    }
    void depenserPierres(unsigned int p) {
        if (pierres >= p) 
            pierres -= p;
    }

// tuiles
    void prendreTuile(const Tuile& t); 

// score
    unsigned int etoiles(const TypeHexagone& t) const {
        switch (t) {
            case TypeHexagone::Habitation: return 1;
            case TypeHexagone::Marche: return 2;
            case TypeHexagone::Caserne: return 2;
            case TypeHexagone::Temple: return 2;
            case TypeHexagone::Jardin: return 3;
            default: return 0;
        }
    }

    unsigned int maxPlaces(const TypeHexagone& t) const {
        switch (t) {
            case TypeHexagone::Habitation: return 5;
            case TypeHexagone::Marche: return 4;
            case TypeHexagone::Caserne: return 4;
            case TypeHexagone::Temple: return 4;
            case TypeHexagone::Jardin: return 3;
            default: return 0;
        }
    }

    unsigned int maxQuartiers(const TypeHexagone t) const {
        switch (t) {
            case TypeHexagone::Habitation: return 18;
            case TypeHexagone::Marche: return 12;
            case TypeHexagone::Caserne: return 10;
            case TypeHexagone::Temple: return 8;
            case TypeHexagone::Jardin: return 6;
            default: return 0;
        }
    }

    unsigned int calculerPoints(Difficulte d) const; // retourne des points
};