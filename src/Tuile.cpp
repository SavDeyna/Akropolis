
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>

#include "Tuile.h"

using namespace std;

// IllustreArchitecte
bool Tuile::possedePlace() const {
    for (const auto& h : disposition) {
        if (h.isPlace())
            return true;
    }
    return false;
}

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

void Tuile::afficherTuile(HexagoneCoord Origine) const {
    std::cout << "\n=== Tuile #" << id_tuile;
    std::cout << " ===" << std::endl;

    const std::map<TypeHexagone, std::string> Conversion = {
        {TypeHexagone::Carriere,   "C"},
        {TypeHexagone::Caserne,    "S"},
        {TypeHexagone::Jardin,     "J"},
        {TypeHexagone::Temple,     "T"},
        {TypeHexagone::Marche,     "M"},
        {TypeHexagone::Habitation, "H"}
    };
    
    std::string t0, t1,t2;
    auto it= Conversion.find(disposition[0].getTypeHexagone());
    if (it !=Conversion.end()){
        t0=it->second;
    }
    it= Conversion.find(disposition[1].getTypeHexagone());
    if (it !=Conversion.end()){
        t1=it->second;
    }
    it= Conversion.find(disposition[2].getTypeHexagone());
    if (it !=Conversion.end()){
        t2=it->second;
    }
    //Affichage des tuiles en fonction de leur orientation
    if (orientation == 1){
        std::cout <<"\n/ "<<disposition[1].isPlace()<< t1<<"  \\ / "<<disposition[2].isPlace()<<t2<<"  \\\n";
        std::cout <<"\\ "<<abs(disposition[1].getQ()+Origine.q)<<abs(disposition[1].getR()+Origine.r)<<abs(disposition[1].getS()+Origine.s)<<" / \\ "<<abs(disposition[2].getQ()+Origine.q)<<abs(disposition[2].getR()+Origine.r)<<abs(disposition[2].getS()+Origine.s)<<" /\n";
    
        std::cout <<"    / "<<disposition[0].isPlace()<<t0<<"  \\\n";
        std::cout <<"    \\ "<<abs(disposition[0].getQ()+Origine.q)<<abs(disposition[0].getR()+Origine.r)<<abs(disposition[0].getS()+Origine.s)<<" /\n\n";
    }
    else {
        std::cout <<"\n    / "<<disposition[2].isPlace()<<t2<<"  \\\n";
        std::cout <<"    \\ "<<abs(disposition[2].getQ()+Origine.q)<<abs(disposition[2].getR()+Origine.r)<<abs(disposition[2].getS()+Origine.s)<<" /\n";
        std::cout <<"/ "<<disposition[1].isPlace()<< t1<<"  \\ / "<<disposition[0].isPlace()<<t0<<"  \\\n";
        std::cout <<"\\ "<<abs(disposition[1].getQ()+Origine.q)<<abs(disposition[1].getR()+Origine.r)<<abs(disposition[1].getS()+Origine.s)<<" / \\ "<<abs(disposition[0].getQ()+Origine.q)<<abs(disposition[0].getR()+Origine.r)<<abs(disposition[0].getS()+Origine.s)<<" /\n\n";
    }
}

void Tuile::tourner() {
    Hexagone tmp = disposition[0];

    disposition[0] = disposition[1];
    disposition[0].setQ(tmp.getQ());
    disposition[0].setR(tmp.getR());

    int q = disposition[1].getQ();
    int r = disposition[1].getR();
    disposition[1] = disposition[2];
    disposition[1].setQ(q);
    disposition[1].setR(r);

    q = disposition[2].getQ();
    r = disposition[2].getR();
    disposition[2] = tmp;
    disposition[2].setQ(q);
    disposition[2].setR(r);
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






void Tuile::changeorientation(){
    if (orientation ==1) {
        disposition[0].setR(0);
        disposition[0].setQ(1);
        disposition[2].setQ(1);
        disposition[2].setR(-1);
        orientation++;
    }
    else{
        disposition[0].setR(1); 
        disposition[0].setQ(0); 
        disposition[2].setR(0);
        disposition[2].setQ(1);
        orientation--;
    }
}

std::string Tuile::ToString() const {
    std::ostringstream f;
    std::ostringstream fdispo;
    const std::map<TypeHexagone, std::string> Conversion = {
        {TypeHexagone::Carriere,   "Carriere"},
        {TypeHexagone::Caserne,    "Caserne"},
        {TypeHexagone::Jardin,     "Jardin"},
        {TypeHexagone::Temple,     "Temple"},
        {TypeHexagone::Marche,     "Marche"},
        {TypeHexagone::Habitation, "Habitation"}
    };

    // Mise sous forme de la disposition : q,r,typeQuartier,place,q,r,typeQuartier,place,q,r,typeQuartier,place,
    for (const auto& dispo : disposition){
        fdispo<<dispo.getQ()<<","<<dispo.getR()<<","<<Conversion.at(dispo.getTypeHexagone())<< ","<<dispo.isPlace()<<",";
    }

    f << fdispo.str() <<  id_tuile;
    return f.str();
}


