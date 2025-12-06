#include "Participation.h"

// ----- calcul des points -----
unsigned int Participation::calculerPoints() {

    /**
     * Calcule le score final du joueur selon les règles de Akropolis.
     *  1. Compter les hexagones de chaque type de quartier (5 types)
     *     dont la condition de placement est respectée.
     *  2. Faire la somme des coefficients (étoiles) des Places.
     *  3. Score = quartiers * coefficient.
     *  4. Ajouter 1 point par pierre possédée.
     */


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

    //Tableau des points : 
    map<TypeHexagone,int> tabpoints={
        {TypeHexagone::Caserne, 2},
        {TypeHexagone::Jardin, 3},
        {TypeHexagone::Temple, 2},
        {TypeHexagone::Marche , 2},
        {TypeHexagone::Habitation , 1}
    };

    // nécessite : friend class Participation; dans Plateau
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
        if (hex.place) {
            totalMultiplicateur += tabpoints[hex.type];
        }
    }

    // --- Score total par type ---
    int total = 0;
    for (TypeHexagone t : quartiers) {
        total += nbQuartiers[t] * totalMultiplicateur;
    }

    // --- Ajout des pierres ---
    pierres = Participation::getPierres();
    total += pierres;

    nbPoints = total;
    return nbPoints;
}

bool Participation::placerTuile(const Tuile& tuile) {
    Plateau& p = plateau;

    // On vérifie si au moins un hexagone voisin est occupé
    bool voisinTrouve = false;

    for (const auto& h : tuile.getDisposition()) {

        // Convertit un Hexagone → HexagoneCoord
        HexagoneCoord c { h.getQ(), h.getR(), h.getS() };

        // Récupère les voisins du plateau (coord absolues)
        auto voisins = p.getVoisins(c);

        for (const auto& v : voisins) {
            if (p.estOccupe(v)) {
                voisinTrouve = true;
                break;
            }
        }
        if (voisinTrouve) break;
    }

    if (!voisinTrouve && !p.estVide()) {
        std::cout << "Aucun voisin, placement refusé.\n";
        return false;
    }

    // Choix de l'origine de la tuile (ex: placer la première au centre)
    HexagoneCoord origin { 0,0,0 };

    p.placerTuile(tuile, origin);
    return true;
}


