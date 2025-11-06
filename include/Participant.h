#ifndef PARTICIPANT_H
#define PARTICIPANT_H

#pragma once
#include <iostream>
#include <string>

#include "Plateau.h"
using namespace std;


class Participant {
private:
    string pseudo;
public:
    explicit Participant(const string& p) : pseudo(move(p)) {}
    virtual ~Participant() = default;
    const string& getPseudo() const { return pseudo; }
    void setPseudo(string p) { pseudo = move(p); }
};

class Joueur : public Participant {
private:
    bool victoire{ false };
public:
    explicit Joueur(const string&, bool v = false) : Participant(p), victoire(v) {}
    bool aGagne() const noexcept { return victoire; }
    void setVictoire(bool v) noexcept { victoire = v; }
};

enum class Difficulte { Facile = 0, Moyen = 1, Difficile = 2 };

class IA : public Participant {
private:
    Difficulte difficulte{ Difficulte::Facile };
public:
    explicit IA(std::string p, Difficulte d = Difficulte::Facile): Participant(move(pseudo)), difficulte(d) {}
    Difficulte getDifficulte() const { return difficulte; }
    void setDifficulte(Difficulte d) { difficulte = d; }
};

#endif