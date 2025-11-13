#include <vector>
#include <set>
#include <algorithm>
#include "Plateau.h"
#include "Tuile.h"
#include "Hexagone.h"
#include "Participant.h"

bool Participant::placerTuile(Tuile& tuile) {
    auto& p = getPlateau();
    const auto& coords = tuile.getDisposition();
    std::set<const Tuile*> coveredTuiles;
    int maxHauteur = 0;

    cout<<"Fonction initalisée\n";
    // Vérifie chevauchement
    for (const auto& c : coords) {
        const Tuile* t = p.getTuile(c);
        if (t) {
            coveredTuiles.insert(t);
            maxHauteur = std::max(maxHauteur, t->getHauteur());
        }
    }
    cout<<"Chevauchement vérifié\n";

    // Si on recouvre une seule tuile : interdit
    if (coveredTuiles.size() == 1) return false;

    cout<<"début boucles\n";
    // Si on ne recouvre aucune tuile, vérifier qu'au moins un hexagone touche une tuile existante
    if (coveredTuiles.empty()) {
        bool touche = false;
        for (const auto& c : coords) {
            auto voisins = p.getVoisins(c);
            for (auto& v : voisins) {
                const Tuile* t = p.getTuile(v);
                if (t) { // une tuile du participant touche
                    touche = true;
                    break;
                }
            }
            if (touche) break;
        }
        if (!touche) return false;
    }

    // Si chevauche plusieurs tuiles, pas besoin de vérifier le joueur (plateau privé)

    cout<<"après boucles\n";

    tuile.setHauteur(maxHauteur + 1);
    p.placerTuile(tuile); // ajoute la tuile au plateau
    return true;
}