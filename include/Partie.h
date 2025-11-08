#ifndef PARTIE_H
#define PARTIE_H
#include "Participant.h"
#include <array>

enum class Variante {
    Facile,
    Moyen,
    Difficile
};

class Partie{
    public:
        explicit Partie(std::initializer_list<const Joueur*> js, Variante v) {}
        Partie(const Joueur& j1, Variante v): Partie({ &j1 }, v) {}
        Partie(const Joueur& j1, const Joueur& j2, Variante v): Partie({ &j1, &j2 }, v) {}
        Partie(const Joueur& j1, const Joueur& j2, const Joueur& j3, Variante v): Partie({ &j1, &j2, &j3 }, v) {}
        Partie(const Joueur& j1, const Joueur& j2, const Joueur& j3, const Joueur& j4, Variante v): Partie({ &j1, &j2, &j3, &j4 }, v) {}
        ~Partie() = default;
        int getTour() const { return tour; }
        const Joueur* getJoueur(std::size_t i) const {
            if (i >= nbJoueurs) throw std::out_of_range("Index de joueur invalide");
            return joueurs[i];
        }
        std::size_t getNbJoueurs() const noexcept { return nbJoueurs; }
        void incTour() { tour += 1; }
        Variante getVariante() { return variante; }
    private:
        Variante variante;
        std::array<const Joueur*, 4> joueurs{};
        int nbJoueurs{ 0 };
        int tour{ 0 };


};

#endif