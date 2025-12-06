#include "Participation.h"
#include "Plateau.h"
#include "Participant.h"
#include "Hexagone.h"

// ----- calcul des points -----
int Participation::calculerPoints() {

    /**
     * Calcule le score final du joueur selon les règles de Akropolis.
     *  1. Compter les hexagones de chaque type de quartier (5 types)
     *     dont la condition de placement est respectée.
     *  2. Faire la somme des coefficients (étoiles) des Places.
     *  3. Score = quartiers * coefficient.
     *  4. Ajouter 1 point par pierre possédée.
     */

    Plateau& plateau = participant->getPlateau();

    // Types de quartiers concernés
    const std::vector<TypeHexagone> quartiers = {
        TypeHexagone::Caserne,
        TypeHexagone::Jardin,
        TypeHexagone::Temple,
        TypeHexagone::Marche,
        TypeHexagone::Habitation
    };

    // Comptages
    std::map<TypeHexagone, int> nbQuartiers;
    int totalMultiplicateur = 0;

    // ⚠️ nécessite : friend class Participation; dans Plateau
    for (const auto& entry : plateau.grille) {
        const HexState& hex = entry.second;

        // --- Quartiers comptables ---
        if (hex.type == TypeHexagone::Caserne ||
            hex.type == TypeHexagone::Jardin ||
            hex.type == TypeHexagone::Temple ||
            hex.type == TypeHexagone::Marche ||
            hex.type == TypeHexagone::Habitation)
        {
            if (hex.hauteur >= 0) { // condition de placement
                nbQuartiers[hex.type]++;
            }
        }

        // --- Multiplicateur des Places ---
        if (hex.type == TypeHexagone::Place) {
            totalMultiplicateur += hex.etoiles;
        }
    }

    // --- Score total par type ---
    int total = 0;
    for (TypeHexagone t : quartiers) {
        total += nbQuartiers[t] * totalMultiplicateur;
    }

    // --- Ajout des pierres ---
    nbrPierre = Participation::getPierres();
    total += nbrPierre;

    nbrPoints = total;
    return nbrPoints;
}
