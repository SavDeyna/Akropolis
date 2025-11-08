
#include <iostream>
#include <string>
#include <vector>
#include <set>
#include "Plateau.h"


using namespace std;

// si au moins une coordonnée d'une tuile correspond à la coordonnée du paramètre alors on renvoie la tuile trouvée
const Tuile* Plateau::getTuile(const Hexagone& coord) const {
    for (const auto& t : matrice_hexa) {
        for (const auto& c : t.getDisposition()) {
            if (c.getQ() == coord.getQ() && c.getR() == coord.getR() && c.getS() == coord.getS()) {
                return &t;
            }
        }
    }
    return nullptr;
}

vector<Hexagone> Plateau::getVoisins(const Hexagone& c) const {
    //return {
    //    Hexagone(c.getQ()+1, c.getR()-1, c.getS()),
    //    Hexagone(c.getQ()+1, c.getR(), c.getS()-1),
    //    Hexagone(c.getQ(), c.getR()+1, c.getS()-1),
    //    Hexagone(c.getQ()-1, c.getR()+1, c.getS()),
    //    Hexagone(c.getQ()-1, c.getR(), c.getS()+1),
    //    Hexagone(c.getQ(), c.getR()-1, c.getS()+1)
    //};
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
            vector<Hexagone> voisins = getVoisins(c);
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

