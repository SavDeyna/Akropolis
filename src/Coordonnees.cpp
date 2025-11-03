#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Plateau.h"
#include "Tuile.h"
#include "Coordonnees.h"

using namespace std;

bool Coordonnees::operator<(const Coordonnees& other) const {
        if (q != other.q) return q < other.q;
        else if (r != other.r) return r < other.r;
        else return s < other.s;
}