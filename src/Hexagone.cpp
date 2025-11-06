#pragma once
#include <iostream>
#include <string>
#include <vector>

#include <Hexagone.h>

using namespace std;

bool Hexagone::operator<(const Hexagone& other) const {
        if (q != other.q) return q < other.q;
        else if (r != other.r) return r < other.r;
        else return s < other.s;
}