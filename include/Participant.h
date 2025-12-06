#pragma once
#include <iostream>
#include <string>

using namespace std;


class Participant {
private:
    std::string pseudo;

public:
    explicit Participant(const std::string& p) : pseudo(p) {}
    virtual ~Participant() = default;
    const string& getPseudo() const { return pseudo; }
    
    void setPseudo(std::string p) { pseudo = std::move(p); }
    
};

class Joueur : public Participant {
private:
    bool victoire{ false };
public:
    explicit Joueur(const string& p) : Participant(p), victoire(false) {}
    bool aGagne() const noexcept { return victoire; }
    void setVictoire(bool v) noexcept { victoire = v; }
};

enum class Difficulte { Facile = 0, Moyen = 1, Difficile = 2 };

class IA : public Participant {
private:
    Difficulte difficulte{ Difficulte::Facile };
public:
    explicit IA(std::string p, Difficulte d = Difficulte::Facile): Participant(std::move(p)), difficulte(d) {}
    Difficulte getDifficulte() const { return difficulte; }
    void setDifficulte(Difficulte d) { difficulte = d; }
};
