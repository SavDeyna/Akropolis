#include "Hexagone.h"
#include <sstream>
bool Hexagone::operator<(const Hexagone& other) const {
    if (q != other.getQ()) return q < other.getQ();
    else if (r != other.getR()) return r < other.getR();
    else return getS() < other.getS();
}

std::string HexagoneCoord::toString() const {
    std::ostringstream f;
    f << q << "," << r << "," << s;
    return f.str();
}


HexagoneCoord HexagoneCoord::fromString(const std::string& str) {
    std::istringstream f(str);

    int q, r, s;
    char c1, c2;

    //On vérifie la structure et on lit les valeurs
    if (!(f >> q)) {
        throw std::runtime_error("q invalide: " + str);
    }

    // virgule
    if (!(f >> c1) || c1 != ',') {
        throw std::runtime_error("virgule manquante: " + str);
    }

    if (!(f >> r)) {
        throw std::runtime_error("r invalide: " + str);
    }

    //virgule
    if (!(f >> c2) || c2 != ',') {
        throw std::runtime_error("virgule manquante: " + str);
    }

    if (!(f >> s)) {
        throw std::runtime_error("s invalide: " + str);
    }

    return HexagoneCoord{q, r, s};
}

std::string HexState::toString() const {
    std::ostringstream f;

    const std::map<TypeHexagone, std::string> Conversion = {
    {TypeHexagone::Carriere,   "Carriere"},
    {TypeHexagone::Caserne,    "Caserne"},
    {TypeHexagone::Jardin,     "Jardin"},
    {TypeHexagone::Temple,     "Temple"},
    {TypeHexagone::Marche,     "Marche"},
    {TypeHexagone::Habitation, "Habitation"}
    };

    f << Conversion.at(type) << "," << hauteur << "," << id_tuile<< "," << place;
    return f.str();
}
    
HexState HexState::fromString(const std::string& str){
    const std::map<std::string, TypeHexagone> Conversion = {
    {"Carriere",   TypeHexagone::Carriere},
    {"Caserne",    TypeHexagone::Caserne},
    {"Jardin",     TypeHexagone::Jardin},
    {"Temple",     TypeHexagone::Temple},
    {"Marche",     TypeHexagone::Marche},
    {"Habitation", TypeHexagone::Habitation}
    };

    std::istringstream f(str);

    int hauteur, id_tuile;
    std::string typetemp ;
    TypeHexagone type;
    bool place;

    char c1, c2,c3;

    //On vérifie la structure et on lit les valeurs
    if (!(f>> typetemp)) {
        throw std::runtime_error("q invalide: " + str);
    }
    type=Conversion.at(typetemp);

    // virgule
    if (!(f >> c1) || c1 != ',') {
        throw std::runtime_error("virgule manquante: " + str);
    }

    if (!(f >> hauteur)) {
        throw std::runtime_error("r invalide: " + str);
    }

    //virgule
    if (!(f >> c2) || c2 != ',') {
        throw std::runtime_error("virgule manquante: " + str);
    }

    if (!(f >> id_tuile)) {
        throw std::runtime_error("s invalide: " + str);
    }
    //virgule
    if (!(f >> c3) || c3 != ',') {
        throw std::runtime_error("virgule manquante: " + str);
    }

    if (!(f >> place)) {
        throw std::runtime_error("s invalide: " + str);
    }

    return HexState{type,hauteur,id_tuile,place};
}