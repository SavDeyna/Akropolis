#pragma once
#include <map>
#include <vector>
#include "Participant.h"
#include "Plateau.h"
#include "Hexagone.h"

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
    int nbrPoints{0};           // Score final
    int nbrPierre{0};           // Nombre de pierres à la fin de la partie
    int ordrePassage{0};        // Ordre de jeu dans la partie

public:
    // ----- Constructeur -----
    Participation(Participant* p, int ordre = 0)
        : participant(p), ordrePassage(ordre)
    {}


    // ----- Getters -----
    Participant* getParticipant() const { return participant; }
    int getPoints() const { return nbrPoints; }
    int getPierres() const { return nbrPierre; }
    int getOrdrePassage() const { return ordrePassage; }

    // ----- Setters -----
    void setOrdrePassage(int o) { ordrePassage = o; }


    //--------------------
    int calculerPoints();
};
