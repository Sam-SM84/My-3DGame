#include "Sphere.h"

Sphere::Sphere(double xp, double yp, double zp, double size, Color c) : Object(xp, yp, zp, size, c) {};

void Sphere::make()
{
    glPushMatrix();
    glTranslatef(this->xp, this->yp, this->zp);
    glColor3f(this->color.r, this->color.g, this->color.b);
    glutSolidSphere(this->size, 20, 20);
    glPopMatrix();
}

void Sphere::makeWireCube()
{
    glPushMatrix();
    glTranslatef(this->xp, this->yp, this->zp);
    glutWireCube(this->size * 2);
    glColor3f(1, 1, 1);
    glLineWidth(5);
    glPopMatrix();
}

Position Sphere::getDim()
{
    double xn = this->xp - (this->size);
    double xm = this->xp + (this->size);
    double yn = this->yp - (this->size);
    double ym = this->yp + (this->size);
    double zn = this->zp - (this->size);
    double zm = this->zp + (this->size);
    return Position(xn, xm, yn, ym, zn, zm);
}