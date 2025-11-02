#pragma once
#include <iostream>
#include <string>

using namespace std;

class Participant {
private:
    string pseudo;
public:
    Participant(const string& p) : pseudo(p) {}
    string getPseudo() const { return pseudo; }
    void setPseudo(string p) { pseudo = p; }
};

class Joueur : public Participant {
private:
    bool victoire;
public:
    Joueur(const string&) : Participant(p) {}
};

class IA : public Participant {
private:
    int difficulte;
public:
    IA(const string&) : Participant(p) {}
};