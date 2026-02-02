#pragma once

#ifndef BULLET_H
#define BULLET_H

#include "Color.h"
#include "Position.h"

class Bullet {
public:
    Position p;
    Color c;
    double dx, dy, dz;
    float bornOn;
    bool render;

    Bullet(Position p, Color c,double dx,double dy,double dz);
    void makeShape(double dt);

};


#endif

