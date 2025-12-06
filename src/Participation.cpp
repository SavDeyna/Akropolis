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

    const std::vector<TypeHexagone> quartiers = {
        TypeHexagone::Caserne,
        TypeHexagone::Jardin,
        TypeHexagone::Temple,
        TypeHexagone::Marche,
        TypeHexagone::Habitation
    };

    // Nouveau : on stocke la somme des valeurs pondérées
    std::map<TypeHexagone, int> sommeValeursQuartiers;

    int totalMultiplicateur = 0;

    for (const auto& entry : plateau.grille) {
        const HexState& hex = entry.second;

        // ----------- QUARTIERS -----------
        if (hex.type == TypeHexagone::Caserne ||
            hex.type == TypeHexagone::Jardin ||
            hex.type == TypeHexagone::Temple ||
            hex.type == TypeHexagone::Marche ||
            hex.type == TypeHexagone::Habitation)
        {
            // valeur = hauteur + 1
            int valeur = hex.hauteur + 1;
            sommeValeursQuartiers[hex.type] += valeur;
        }

        // ----------- PLACES -----------
        if (hex.type == TypeHexagone::Place) {
            totalMultiplicateur += hex.etoiles;
        }
    }

    // ---- CALCUL SCORE FINAL ----
    int total = 0;

    for (auto t : quartiers) {
        int valeurQuartiers = sommeValeursQuartiers[t];
        total += valeurQuartiers * totalMultiplicateur;
    }

    // Ajout des pierres
    nbrPierre = participant->getPierres();
    total += nbrPierre;

    nbrPoints = total;
    return nbrPoints;
}
