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
    
