#pragma once
#include <map>
#include <vector>
#include "Participant.h"
#include "Plateau.h"

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
    unsigned int ordrePassage;        // Ordre de jeu dans la partie
    unsigned int pierres{1};
    Plateau plateau;
public:
    // ----- Constructeur -----
    Participation(Participant& p, unsigned int ordre)
        : participant(&p), ordrePassage(ordre){}

    // Construction pour la classe sauvegarde
    Participation(Participant& participant, unsigned int ordre, unsigned int points, unsigned int cailloux , const Plateau& plateau)
        : participant(&participant), nbPoints(points), ordrePassage(ordre), pierres(cailloux), plateau(plateau){}
    // ----- Getters -----
    Participant& getParticipant() const { return *participant; }
    unsigned int getPoints() const { return nbPoints; }
    unsigned int getPierres() const { return pierres; }
    unsigned getOrdrePassage() const { return ordrePassage; }
    Plateau& getPlateau() { return plateau; }

    //Ce plateau sert à la lecture pour sauvegarder une partie
    const Plateau& getPlateau() const { return plateau; }

    // ----- Setters -----
    void setOrdrePassage(int o) { ordrePassage = o; }
    void addPierres(unsigned int i) {pierres+=i;}
    void setPierres(unsigned int p) { pierres = p; }
    void prochainOrdrePassage(unsigned int nbParticipants);
    
    // Stocker les points après leur décompte
    void setNbPoints(unsigned int pts) { nbPoints = pts;}

    void Jouer(vector<Tuile>& jeu);
};
