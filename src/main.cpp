#include "Partie.h"
#include "Participant.h"
#include "Hexagone.h"
#include "Tuile.h"

#include <iostream>

#ifdef _WIN32
#define NOMINMAX
#define byte win_byte_override
#include <windows.h>
#endif

void afficherPlateauConsole(const Plateau& p) {
    std::cout << "\n=== Contenu du plateau (HexState) ===\n";

    // Impossible d'accéder directement à p.grille car private → on ajoute une méthode utilitaire
    // En attendant, on la copie dans une fonction amie ou on ajoute un getter temporaire.
    // Si tu ne veux pas modifier Plateau, commente ce bloc.
}

int main() {

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    std::cout << "Lancement du jeu\n";

    Partie partie;
    partie.ChargerTuiles();
    partie.melangePioche();

    Joueur alice("Alice");

    std::cout << "\n=== Création des tuiles de test ===\n";

    // IMPORTANT : Hexagone(q, r, étoiles, type) → s = -(q+r)

    Hexagone h1(0, 0, 1, TypeHexagone::Caserne);
    Hexagone h2(1, -1, 1, TypeHexagone::Jardin);
    Hexagone h3(1, 0, 2, TypeHexagone::Temple);
    Tuile tuile1(1, {h1, h2, h3});

    Hexagone h4(0, 1, 1, TypeHexagone::Carriere);
    Hexagone h5(1, 0, 0, TypeHexagone::Marche);
    Hexagone h6(1, 1, 1, TypeHexagone::Habitation);
    Tuile tuile2(2, {h4, h5, h6});

    Hexagone h7(0, -1, 2, TypeHexagone::Temple);
    Hexagone h8(1, -2, 1, TypeHexagone::Jardin);
    Hexagone h9(1, -1, 0, TypeHexagone::Caserne);
    Tuile tuile3(3, {h7, h8, h9});

    std::cout << "Tuiles créées\n";

    // Placement avec origine (coordonnées absolues du plateau)
    Plateau& plateau = alice.getPlateau();

    std::cout << "\nPlacement tuile 1...\n";
    plateau.placerTuile(tuile1, HexagoneCoord{0, 0, 0});

    std::cout << "\nPlacement tuile 2...\n";
    plateau.placerTuile(tuile2, HexagoneCoord{3, 0, -3});

    std::cout << "\nPlacement tuile 3...\n";
    plateau.placerTuile(tuile3, HexagoneCoord{-2, 1, 1});

    std::cout << "\nPlateau du joueur " << alice.getPseudo() << " :\n";

    plateau.afficherPlateau();

    system("pause");
    return 0;
}
