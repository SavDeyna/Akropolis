#include "Partie.h"

Partie::Partie(std::initializer_list<const Joueur*> js, Variante v)
    : variante(v)
{
    if (js.size() == 0 || js.size() > 4)
        throw std::invalid_argument("Une partie doit avoir entre 1 et 4 joueurs.");

    // Remplit le tableau et compte
    nbJoueurs = 0;
    for (const Joueur* p : js) {
        joueurs[nbJoueurs++] = p;  // on stocke tel quel (non-owner)
    }
    // Pad le reste avec nullptr
    for (std::size_t i = nbJoueurs; i < joueurs.size(); ++i) {
        joueurs[i] = nullptr;
    }
}
