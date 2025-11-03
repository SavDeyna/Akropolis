#pragma once
#include <iostream>
#include <string>
#include <map>

using namespace std;

struct Coordonnees {
    int q;
    int r;
    int s;
    Coordonnees(int q, int r, int s): q(q), r(r), s(s) {
        if ((q + r + s) != 0)
            throw invalid_argument("q+r+s n'est pas égal à 0");
    }
    bool operator<(const Coordonnees& other) const;
};