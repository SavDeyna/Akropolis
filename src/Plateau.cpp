#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include "Plateau.h"
#include "Tuile.h"
#include "Coordonnees.h"

using namespace std;

// si au moins une coordonnée d'une tuile correspond à la coordonnée du paramètre alors on renvoie la tuile trouvée
const Tuile* Plateau::getTuile(const Coordonnees& coord) const {
    for (const auto& t : matrice_hexa) {
        for (const auto& c : t.getDisposition()) {
            if (c.q == coord.q && c.r == coord.r && c.s == coord.s) {
                return &t;
            }
        }
    }
    return nullptr;
}

vector<Coordonnees> Plateau::getVoisins(const Coordonnees& c) const {
    return {
        Coordonnees(c.q+1, c.r-1, c.s),
        Coordonnees(c.q+1, c.r, c.s-1),
        Coordonnees(c.q, c.r+1, c.s-1),
        Coordonnees(c.q-1, c.r+1, c.s),
        Coordonnees(c.q-1, c.r, c.s+1),
        Coordonnees(c.q, c.r-1, c.s+1)
    };
}

bool Plateau::placerTuile(Tuile& tuile, const string& nom_joueur) {
    const auto& coords = tuile.getDisposition();
    set<const Tuile*> coveredTuiles;
    int maxHauteur = 0;

    // on vérifie si la nouvelle tuile chevauche d'autres tuiles (leurs hexas)
    for (const auto& c : coords) {
        const Tuile* t = getTuile(c);
        if (t) {
            coveredTuiles.insert(t);
            maxHauteur = max(maxHauteur, t->getHauteur());
        }
    }

    // si finalement la nouvelle tuile chevauche d'autres tuiles on doit vérifier si les règles du jeu sont satisfaites
    if (coveredTuiles.size() == 1) return false; // on ne peut pas recouvrir une seule tuile complètement, il faut recouvrir au moins 2 tuiles différentes
    // sinon on vérifie qu'au moins un hexa de la nouvelle tuile touche une tuile du même joueur
    if (coveredTuiles.empty()) {
        bool touche = false;
        for (const auto& c : coords) {
            vector<Coordonnees> voisins = getVoisins(c);
            for (auto& v : voisins) {
                const Tuile* t = getTuile(v);
                if (t && t->getNomJoueur() == nom_joueur) {
                    touche = true;
                    break;
                }
            }
        }
        if (!touche) return false;
    } else {
        // on vérifie que toutes les tuiles chevauchées appartiennent au même joueur
        for (auto t : coveredTuiles) {
            if (t->getNomJoueur() != nom_joueur) return false;
        }
    }
    tuile.setHauteur(maxHauteur + 1);
    matrice_hexa.push_back(tuile);
    return true;
}

