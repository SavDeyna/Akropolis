#include "IllustreArchitecte.h"
#include <iostream>
#include <map>

IllustreArchitecte::IllustreArchitecte() {
    Tuile t(0, true); // id=0, tuile de départ
    prendreTuile(t);
}

void IllustreArchitecte::prendreTuile(const Tuile& t) {
    for (const auto& h : t.getDisposition()) {
        auto& info = infos[h.getTypeHexagone()]; // si le type n'existe pas encore, la map crée elle-même un enregistrement
        info.quartiers++;

        if (h.isPlace())
            info.places++;
    }
}

unsigned int IllustreArchitecte::calculerPoints() const {
    unsigned int points = 0;

    unsigned int niveau = (difficulte == Difficulte::Difficile) ? 2 : 1;

    for (const auto& [type, info] : infos) {
        if (type == TypeHexagone::Carriere)
            continue;

        unsigned int nbPlaces = std::min(info.places, IllustreArchitecte::maxPlaces(type));

        unsigned int nbQuartiers = std::min(info.quartiers, IllustreArchitecte::maxQuartiers(type));

        unsigned int e = IllustreArchitecte::etoiles(type);

        points += e * nbPlaces * nbQuartiers * niveau;
    }

    // Bonus Métagénès
    if (difficulte == Difficulte::Moyen) {
        auto it = infos.find(TypeHexagone::Carriere);
        if (it != infos.end())
            points += it->second.quartiers * 2;
    }

    points += pierres;

    return points;
}