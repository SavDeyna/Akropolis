
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

#include "Tuile.h"

using namespace std;

 inline std::string typeHexagoneToString(TypeHexagone type) {
        switch (type) {
            case TypeHexagone::Caserne:   return "Caserne";
            case TypeHexagone::Carriere:  return "Carrière";
            case TypeHexagone::Habitation:return "Habitation";
            case TypeHexagone::Temple:    return "Temple";
            case TypeHexagone::Jardin:    return "Jardin";
            case TypeHexagone::Marche:    return "Marché";
            default:                      return "Inconnu";
        }
    }
    
bool Tuile::formeValide() const {
    // Ne valider que les tuiles "normales" à 3 hexagones
    if (is_depart) return true; // départ toujours valide par construction

    if (disposition.size() != 3) {
        std::cerr << "FormeValide attendu 3 hexagones, en a " << disposition.size() << std::endl;
        return false;
    }

    int dist01 = distance(disposition[0], disposition[1]);
    int dist12 = distance(disposition[1], disposition[2]);
    int dist02 = distance(disposition[0], disposition[2]);

    return (dist01 == 1 && dist12 == 1 && dist02 == 1);
}

void Tuile::afficherTuile() const {
    std::cout << "\n=== Tuile #" << id_tuile;
    if (is_depart) std::cout << " (Tuile de départ)";
    std::cout << " ===" << std::endl;

    std::cout << "Hauteur: " << hauteur 
              << " | Joueur: " << (nom_joueur.empty() ? "(aucun)" : nom_joueur)
              << " | Nb Hex: " << disposition.size() << std::endl;

    for (size_t i = 0; i < disposition.size(); ++i) {
        const Hexagone& h = disposition[i];
        std::cout << "  - Hex #" << i 
                  << "  q=" << std::setw(2) << h.getQ()
                  << "  r=" << std::setw(2) << h.getR()
                  << "  s=" << std::setw(2) << h.getS()
                  << "  type=" << typeHexagoneToString(h.getTypeHexagone())
                  << std::endl;
    }
}

void Tuile::tournerDroite() {
    int pq = disposition[0].getQ();
    int pr = disposition[0].getR();
    int ps = disposition[0].getS();

    for (auto& h : disposition) {
        int q = h.getQ() - pq;
        int r = h.getR() - pr;
        int s = h.getS() - ps;

        int rq = -s;
        int rr = -q;
        int rs = -r;

        h.setQ(rq + pq);
        h.setR(rr + pr);
        h.setS(rs + ps);
    }
}

void Tuile::tournerGauche() {
    int pq = disposition[0].getQ();
    int pr = disposition[0].getR();
    int ps = disposition[0].getS();

    for (auto& h : disposition) {
        int q = h.getQ() - pq;
        int r = h.getR() - pr;
        int s = h.getS() - ps;

        int rq = -r;
        int rr = -s;
        int rs = -q;

        h.setQ(rq + pq);
        h.setR(rr + pr);
        h.setS(rs + ps);
    }
}
