#include "Hexagone.h"

bool Hexagone::operator<(const Hexagone& other) const {
    if (q != other.getQ()) return q < other.getQ();
    else if (r != other.getR()) return r < other.getR();
    else return getS() < other.getS();
}