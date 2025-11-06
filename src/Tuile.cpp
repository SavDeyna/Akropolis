
#include <iostream>
#include <string>
#include <vector>

#include "Tuile.h"

using namespace std;

bool Tuile::formeValide() const {
    // on vérifie que les 3 hexas sont adjacents et forment un triangle
    int dist01 = distance(disposition[0], disposition[1]);
    int dist12 = distance(disposition[1], disposition[2]);
    int dist02 = distance(disposition[0], disposition[2]);

    return (dist01 == 1 && dist12 == 1 && dist02 == 1);
}