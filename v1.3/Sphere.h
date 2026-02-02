#pragma once

#ifndef SPHERE_H
#define SPHERE_H

#include "Color.h"
#include "Object.h"
#include <GL/glut.h>

class Sphere : public Object
{
public:

    Sphere(double xp, double yp, double zp, double size, Color c);
    void make();
    void makeWireCube();
    Position getDim() override;
};

#endif

