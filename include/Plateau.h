#pragma once
#include <map>
#include <iostream>
#include <vector>

class Participation; // forward declaration nécessaire pour friend (selon notre ami le GPT)
class ModeDeJeu;     // <-- ajouter cette forward-declaration (toujours selon GPT je ne sais pas ce que ca fait)

#include "Hexagone.h"
#include "Tuile.h"

class Plateau {
private:
    std::map<HexagoneCoord, HexState> grille;
    unsigned int next_id = 1;

    // Methodes utilisées pour le calcul des points, ca permet une meilleur lisibilité du code d'avoir tout de séparé comme ca
    int calculValeurHabitations() const;
    int calculValeurMarches() const;
    int calculValeurCasernes() const;
    int calculValeurTemples() const;
    int calculValeurJardins() const;

    std::map<TypeHexagone,int> compterEtoiles() const;
    void appliquerVariantes(const ModeDeJeu& mdj, std::map<TypeHexagone,int>& valeurs, std::map<TypeHexagone,float>& multiplicateurs) const;

public:
    friend class Participation;

    Plateau();
    int generateId() {
        return next_id++; // renvoie l'ancienne valeur de next_id, puis l'incrémente
    }
    // accès au hex
    const HexState* getHex(const HexagoneCoord& c) const;
    bool estOccupe(const HexagoneCoord& c) const;

    // voisins existants (sur la grille)
    std::vector<HexagoneCoord> getVoisins(const HexagoneCoord& c) const;

    // placer une tuile avec origine (coord absolue du centre ou repère)
    bool placerTuile(Tuile& t, const HexagoneCoord& origin, unsigned int& nbPierres);

    // affiche la grille
    void afficherPlateau() const;

    // dessine le plateau en ASCII art (version compacte en tuiles collées)
    void dessinerPlateau(const int radius) const;

    bool estVide() const { return grille.empty(); }

    //Verifier que l'on peut poser une tuile de 3 hexagones
    bool peutPoserTuile(const Tuile& t, const HexagoneCoord& origin) const;

    // Calcul des points, déplacé de Participation
    unsigned int calculerPoints(const ModeDeJeu& mdj, unsigned int pierres) const;
};