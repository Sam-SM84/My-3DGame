#include "Color.h"
#include "Position.h"
#include "Bullet.h"
#include<GL/glut.h>

Bullet::Bullet(Position p, Color c,double dx,double dy,double dz)
{
	this->p = p;
	this->c = c;
	this->render = true;
	this->bornOn = glutGet(GLUT_ELAPSED_TIME);
	this->dx = dx;
	this->dy = dy;
	this->dz = dz;
}

void Bullet::makeShape(double dt)
{
	p.Xmax += dx * dt * 0.1;
	p.Xmin += dx * dt * 0.1;
	p.Ymax += dy * dt * 0.1;
	p.Ymin += dy * dt * 0.1;
	p.Zmax += dz * dt * 0.1;
	p.Zmin += dz * dt * 0.1;
	if (glutGet(GLUT_ELAPSED_TIME) - bornOn > 1000) this->render = false;
	else if (p.Ymax <= -1) this->render = false;
	glBegin(GL_QUADS);
	glColor3f(c.r, c.g, c.b);
	glVertex3f(p.Xmin, p.Ymin, p.Zmin);
	glVertex3f(p.Xmax, p.Ymin, p.Zmin);
	glVertex3f(p.Xmax, p.Ymin, p.Zmax);
	glVertex3f(p.Xmin, p.Ymin, p.Zmax);
	glEnd();

}
