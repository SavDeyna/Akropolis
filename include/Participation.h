#pragma once
#include <map>
#include <vector>
#include "Participant.h"
#include "Plateau.h"
#include "Partie.h"

/**
 * Classe représentant la participation d’un joueur (Participant)
 * dans une partie. Elle stocke :
 *   - le nombre de points calculé,
 *   - le nombre de pierres possédées (copié depuis Participant),
 *   - l’ordre de passage du joueur.
 */
class Participation {
private:
    Participant* participant;   // Le joueur associé à cette participation
    unsigned int nbPoints{0};           // Score
    unsigned int ordrePassage{0};        // Ordre de jeu dans la partie
    unsigned int pierres{0};
    Plateau plateau;
public:
    // ----- Constructeur -----
    Participation(Participant* p, unsigned int ordre)
        : participant(p), ordrePassage(ordre)
    {}


    // ----- Getters -----
    Participant* getParticipant() const { return participant; }
    unsigned getPoints() const { return nbPoints; }
    unsigned int getPierres() const { return pierres; }
    unsigned getOrdrePassage() const { return ordrePassage; }
    Plateau& getPlateau() { return plateau; }

    // ----- Setters -----
    void setOrdrePassage(int o) { ordrePassage = o; }
    bool placerTuile(const Tuile& tuile);
    // Stocker les points après leur décompte
    void Participation::setNbPoints(unsigned int pts) { nbPoints = pts;}
};
