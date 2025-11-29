#pragma once
#include <iostream>
#include <string>

#include "Plateau.h"
using namespace std;


class Participant {
private:
    std::string pseudo;
    unsigned int pierres{0};
    Plateau plateau;

public:
    explicit Participant(const std::string& p) : pseudo(p), plateau() {}
    virtual ~Participant() = default;

    const std::string& getPseudo() const { return pseudo; }
    void setPseudo(std::string p) { pseudo = std::move(p); }

    Plateau& getPlateau() { return plateau; }

    bool placerTuile(const Tuile& tuile);
};

class Joueur : public Participant {
private:
    bool victoire{ false };
public:
    explicit Joueur(const string& p, bool v = false) : Participant(p), victoire(v) {}
    bool aGagne() const noexcept { return victoire; }
    void setVictoire(bool v) noexcept { victoire = v; }
};

enum class Difficulte { Facile = 0, Moyen = 1, Difficile = 2 };

class IA : public Participant {
private:
    Difficulte difficulte{ Difficulte::Facile };
public:
    explicit IA(std::string p, Difficulte d = Difficulte::Facile): Participant(move(p)), difficulte(d) {}
    Difficulte getDifficulte() const { return difficulte; }
    void setDifficulte(Difficulte d) { difficulte = d; }
};
