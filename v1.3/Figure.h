#pragma once

#ifndef FIGURE_H
#define FIGURE_H

#include "Color.h"
#include "Position.h"
#include <functional>

class Figure {
public:
    double xp,yp,zp;
    double size;
    Color color;

    Figure(double xp,double yp,double zp,double s, Color c);
    bool collides(double nx, double nz,double ny, Position p);
    void moveFigure(double x,double y,double z,bool fp);
};

#endif